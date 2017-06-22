#include "redis.h"
#include "cluster.h"

#include <signal.h>
#include <ctype.h>

void slotToKeyAdd(robj *key);
void slotToKeyDel(robj *key);
void slotToKeyFlush(void);



robj *lookupKey(redisDb *db , robj *key)
{
	dictEntry *de = dictFind(db->dict,key->ptr);

	if (de)
	{
		/* code */
		robj *val = dictGetVal(de);
		if (server.rdb_child_pid == -1 && server.aof_child_pid == -1)
            val->lru = LRU_CLOCK();
        return val;
	}else{
		return NULL;
	}
}


robj *lookupKeyRead(redisDb *db, robj *key) {
    robj *val;

    // 检查 key 释放已经过期
    expireIfNeeded(db,key);

    // 从数据库中取出键的值
    val = lookupKey(db,key);

    // 更新命中/不命中信息
    if (val == NULL)
        server.stat_keyspace_misses++;
    else
        server.stat_keyspace_hits++;

    // 返回值
    return val;
}




robj *lookupKeyWrite(redisDb *db, robj *key) {

    // 删除过期键
    expireIfNeeded(db,key);

    // 查找并返回 key 的值对象
    return lookupKey(db,key);
}

void dbAdd(redisDb *db, robj *key, robj *val) {

    // 复制键名
    sds copy = sdsdup(key->ptr);

    // 尝试添加键值对
    int retval = dictAdd(db->dict, copy, val);

    // 如果键已经存在，那么停止
    redisAssertWithInfo(NULL,key,retval == REDIS_OK);

    // 如果开启了集群模式，那么将键保存到槽里面
    if (server.cluster_enabled) slotToKeyAdd(key);
 }



