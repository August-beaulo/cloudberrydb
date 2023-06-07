/*-------------------------------------------------------------------------
 *
 * binary_upgrade.h
 *	  variables used for binary upgrades
 *
 *
 * Portions Copyright (c) 1996-2021, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/catalog/binary_upgrade.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef BINARY_UPGRADE_H
#define BINARY_UPGRADE_H

/*
 * These are not used in GPDB. We use the generic OID pre-assignment
 * machinery instead.
 */
#if 0
extern PGDLLIMPORT Oid binary_upgrade_next_pg_type_oid;
extern PGDLLIMPORT Oid binary_upgrade_next_array_pg_type_oid;
extern PGDLLIMPORT Oid binary_upgrade_next_mrng_pg_type_oid;
extern PGDLLIMPORT Oid binary_upgrade_next_mrng_array_pg_type_oid;

extern PGDLLIMPORT Oid binary_upgrade_next_heap_pg_class_oid;
extern PGDLLIMPORT Oid binary_upgrade_next_index_pg_class_oid;
extern PGDLLIMPORT Oid binary_upgrade_next_toast_pg_class_oid;

extern PGDLLIMPORT Oid binary_upgrade_next_pg_enum_oid;
extern PGDLLIMPORT Oid binary_upgrade_next_pg_authid_oid;
#endif

extern PGDLLIMPORT bool binary_upgrade_record_init_privs;

#endif							/* BINARY_UPGRADE_H */
