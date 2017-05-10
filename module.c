/*
Copyright (c) 2017 Eran Sandler <eran at sandler dot co dot il>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <stdio.h>
#include "snowflake.h"

#include "redismodule.h"

int GetIdCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  RedisModule_ReplyWithLongLong(ctx, snowflake_id());
  return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (RedisModule_Init(ctx, "snowflake", 1, REDISMODULE_APIVER_1) ==
      REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  if (argc != 2) {
    RedisModule_Log(ctx, "error", "Missing region_id and worker_id parameters");
    return REDISMODULE_ERR;
  }

  long long region_id;
  if (RedisModule_StringToLongLong(argv[0], &region_id) == REDISMODULE_ERR) {
    RedisModule_Log(ctx, "error", "Invalid region_id value");
    return REDISMODULE_ERR;
  }

  long long worker_id;
  if (RedisModule_StringToLongLong(argv[1], &worker_id) == REDISMODULE_ERR) {
    RedisModule_Log(ctx, "error", "Invalid worker_id value");
    return REDISMODULE_ERR;
  }

  RedisModule_Log(ctx, "info", "using region_id: %u  worker_id: %u", region_id, worker_id);

  if (snowflake_init(region_id, worker_id) != 1) {
    RedisModule_Log(ctx, "error", "Failed to initialize snowflake");
    return REDISMODULE_ERR;
  }

  if (RedisModule_CreateCommand(ctx, "snowflake.getid", GetIdCommand, "readonly",
                                1, 1, 1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  return REDISMODULE_OK;
}
