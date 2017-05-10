# Redis Snowflake

Written by Eran Sandler ([@erans](https://twitter.com/erans))

This is a simple redis module which generates unique IDs based on [Twitter's Snowflake](https://github.com/twitter/snowflake/tree/snowflake-2010) at high scale with some simple guarantees.

Some of Snowflake's benefits include:

* **Uncoordinated** - For high availability within and across data centers, machines generating ids should not have to coordinate with each other.

* **(Roughly) Time Ordered** - it can guarantee that the id numbers will be k-sorted (references: http://portal.acm.org/citation.cfm?id=70413.70419 and http://portal.acm.org/citation.cfm?id=110778.110783) within a reasonable bound (1 second is currently promised).

* **Directly Sortable** - The ids should be sortable without loading the full objects that the represent. This sorting should be the above ordering.

* **Compact** - There are many otherwise reasonable solutions to this problem that require 128bit numbers. For various reasons, we need to keep our ids under 64bits.

### ID Composition
* time - 41 bits (millisecond precision w/ a custom epoch gives us 69 years)
* region_id - 4 bits - gives us 16 different region or data centers to work with
* worker_id - 10 bits - gives us up to 100 workers per region_id
* sequence_id - 8 bits - rolls over every 4096 per machine (with protection to avoid rollover in the same ms)

## Compilation

Just `make` it.

## Usage

`redis-server --loadmodule redissnowflake.so [region_id] [worker_id]`

Example:

`redis-server --loadmodule redissnowflake.so 1 1`

If you require multiple such server/services in the same region, change the worker_id value.

If you require multiple such server/services in different region change the region_id value.
