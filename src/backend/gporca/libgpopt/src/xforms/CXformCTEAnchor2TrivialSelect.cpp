//---------------------------------------------------------------------------
//	Cloudberry Database
//	Copyright (C) 2012 EMC Corp.
//
//	@filename:
//		CXformCTEAnchor2TrivialSelect.cpp
//
//	@doc:
//		Implementation of transform
//---------------------------------------------------------------------------

#include "gpopt/xforms/CXformCTEAnchor2TrivialSelect.h"

#include "gpos/base.h"

#include "gpopt/base/COptCtxt.h"
#include "gpopt/operators/CLogicalCTEAnchor.h"
#include "gpopt/operators/CLogicalSelect.h"
#include "gpopt/operators/CPatternLeaf.h"
#include "gpopt/xforms/CXformUtils.h"

using namespace gpopt;


//---------------------------------------------------------------------------
//	@function:
//		CXformCTEAnchor2TrivialSelect::CXformCTEAnchor2TrivialSelect
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CXformCTEAnchor2TrivialSelect::CXformCTEAnchor2TrivialSelect(CMemoryPool *mp)
	: CXformExploration(
		  // pattern
		  GPOS_NEW(mp) CExpression(
			  mp, GPOS_NEW(mp) CLogicalCTEAnchor(mp),
			  GPOS_NEW(mp) CExpression(mp, GPOS_NEW(mp) CPatternLeaf(mp))))
{
}

//---------------------------------------------------------------------------
//	@function:
//		CXformCTEAnchor2TrivialSelect::Exfp
//
//	@doc:
//		Compute promise of xform
//
//---------------------------------------------------------------------------
CXform::EXformPromise
CXformCTEAnchor2TrivialSelect::Exfp(CExpressionHandle &exprhdl) const
{
	ULONG id = CLogicalCTEAnchor::PopConvert(exprhdl.Pop())->Id();
	CCTEInfo *pcteinfo = COptCtxt::PoctxtFromTLS()->Pcteinfo();
	const ULONG ulConsumers = pcteinfo->UlConsumers(id);
	GPOS_ASSERT(0 < ulConsumers);

	if ((pcteinfo->FEnableInlining() || 1 == ulConsumers) &&
		CXformUtils::FInlinableCTE(id))
	{
		return CXform::ExfpHigh;
	}

	return CXform::ExfpNone;
}

//---------------------------------------------------------------------------
//	@function:
//		CXformCTEAnchor2TrivialSelect::Transform
//
//	@doc:
//		Actual transformation
//
//---------------------------------------------------------------------------
void
CXformCTEAnchor2TrivialSelect::Transform(CXformContext *pxfctxt,
										 CXformResult *pxfres,
										 CExpression *pexpr) const
{
	GPOS_ASSERT(nullptr != pxfctxt);
	GPOS_ASSERT(FPromising(pxfctxt->Pmp(), this, pexpr));
	GPOS_ASSERT(FCheckPattern(pexpr));

	// ExfExpandFullOuterJoin converts a LogicalFullJoin to a CTE with a UnionAll
	// of an LOJ and LASJ. Even if expansion is disabled by the trace flag, the
	// xform still needs to be triggered, so that the CTE operator generated
	// can be used to compute the statistics for the logical merge join group (see
	// CLogicalFullOuterJoin::Esp()).
	// Instead, we skip the implementation of the CTE, here, if the trace flag is
	// disabled and the CTE op was generated by ExfExpandFullOuterJoin.
	CGroupExpression *pgexprOrigin = pexpr->Pgexpr();
	if (!GPOS_FTRACE(EopttraceExpandFullJoin) && nullptr != pgexprOrigin &&
		CXform::ExfExpandFullOuterJoin == pgexprOrigin->ExfidOrigin())
	{
		return;
	}

	CMemoryPool *mp = pxfctxt->Pmp();

	// child of CTE anchor
	CExpression *pexprChild = (*pexpr)[0];
	pexprChild->AddRef();

	CExpression *pexprSelect = GPOS_NEW(mp)
		CExpression(mp, GPOS_NEW(mp) CLogicalSelect(mp), pexprChild,
					CUtils::PexprScalarConstBool(mp, true /*fValue*/));

	pxfres->Add(pexprSelect);
}

// EOF
