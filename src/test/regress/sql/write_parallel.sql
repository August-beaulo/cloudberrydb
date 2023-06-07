--
-- PARALLEL
--

-- GPDB_96_MERGE_FIXME: We don't support parallel query. These tests won't actually
-- generate any parallel plans. Same as in 'select_parallel' test.

-- Serializable isolation would disable parallel query, so explicitly use an
-- arbitrary other level.
begin isolation level repeatable read;

-- encourage use of parallel plans
set parallel_setup_cost=0;
set parallel_tuple_cost=0;
set min_parallel_table_scan_size=0;
set max_parallel_workers_per_gather=4;

--
-- Test write operations that has an underlying query that is eligible
-- for parallel plans
--
explain (costs off) create table parallel_write as
    select length(stringu1) from tenk1 group by length(stringu1);
create table parallel_write as
    select length(stringu1) from tenk1 group by length(stringu1);
drop table parallel_write;

explain (costs off) select length(stringu1) into parallel_write
    from tenk1 group by length(stringu1);
select length(stringu1) into parallel_write
    from tenk1 group by length(stringu1);
drop table parallel_write;

explain (costs off) create materialized view parallel_mat_view as
    select length(stringu1) from tenk1 group by length(stringu1);
create materialized view parallel_mat_view as
    select length(stringu1) from tenk1 group by length(stringu1);
create unique index on parallel_mat_view(length);
refresh materialized view parallel_mat_view;
refresh materialized view concurrently parallel_mat_view;
drop materialized view parallel_mat_view;

prepare prep_stmt as select length(stringu1) from tenk1 group by length(stringu1);
explain (costs off) create table parallel_write as execute prep_stmt;
create table parallel_write as execute prep_stmt;
drop table parallel_write;

rollback;
