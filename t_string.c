#include "redis.h"
#include <math.h> /* isnan(), isinf() */


static int checkStringLength(redisClient *c , long long size)
{
	if (size > 512 * 1024 * 1024)
	{
		addReplyError(c,"string exceeds maximum allowed size (512MB)");
        return REDIS_ERR;
		/* code */
	}
	 return REDIS_OK;
}

#define REDIS_SET_NO_FLAGS 0
#define REDIS_SET_NX (1<<0)     /* Set if key not exists. */
#define REDIS_SET_XX (1<<1)     /* Set if key exists. */

void setGenericCommand(redisClient *c , int flags ,robj *key ,robj *val , robj *expire ,int unit , robj *ok_reply, robj *abort_reply){
	long long milliseconds = 0;
	if (expire) {
		if (getLongLongFromObjectOrReply(c, expire, &milliseconds, NULL) != REDIS_OK)
        	return;
        if (milliseconds <= 0) {
            addReplyError(c,"invalid expire time in SETEX");
            return;
        }
        if (unit == UNIT_SECONDS) milliseconds *= 1000;

        if ((flags & REDIS_SET_NX && lookupKeyWrite(c->db,key) != NULL) ||
        (flags & REDIS_SET_XX && lookupKeyWrite(c->db,key) == NULL))
		{
			addReply(c, abort_reply ? abort_reply : shared.nullbulk);
			return;
		}
		setKey(c->db,key,val);

    // 将数据库设为脏
    server.dirty++;

    // 为键设置过期时间
    if (expire) setExpire(c->db,key,mstime()+milliseconds);

    // 发送事件通知
    notifyKeyspaceEvent(REDIS_NOTIFY_STRING,"set",key,c->db->id);

    // 发送事件通知
    if (expire) notifyKeyspaceEvent(REDIS_NOTIFY_GENERIC,
        "expire",key,c->db->id);

    // 设置成功，向客户端发送回复
    // 回复的内容由 ok_reply 决定
    addReply(c, ok_reply ? ok_reply : shared.ok);


	}
}

void setnxCommand(redisClient *c) {
    c->argv[2] = tryObjectEncoding(c->argv[2]);
    setGenericCommand(c,REDIS_SET_NX,c->argv[1],c->argv[2],NULL,0,shared.cone,shared.czero);
}

void setexCommand(redisClient *c) {
    c->argv[3] = tryObjectEncoding(c->argv[3]);
    setGenericCommand(c,REDIS_SET_NO_FLAGS,c->argv[1],c->argv[3],c->argv[2],UNIT_SECONDS,NULL,NULL);
}

void psetexCommand(redisClient *c) {
    c->argv[3] = tryObjectEncoding(c->argv[3]);
    setGenericCommand(c,REDIS_SET_NO_FLAGS,c->argv[1],c->argv[3],c->argv[2],UNIT_MILLISECONDS,NULL,NULL);
}

