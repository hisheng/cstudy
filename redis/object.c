#include "redis.h"
#include <math.h>
#include <ctype.h>




robj *createObject(int type , void *ptr)
{
	robj *o = zmalloc(sizeof(*o));

	o->type = type;
	o->encoding = REDIS_ENCODING_RAW;
	o->ptr = ptr;
	o->refcount = 1;
	o->lru = LRU_CLOCK();
	return o;
}

robj *createRawStringObject(char *ptr , size_t len)
{
	return createObject(REDIS_STRING,sdsnewlen(ptr,len));
}

robj *createEmbeddedStringObject(char *ptr , size_t len)
{
	robj  *o = zmalloc(sizeof(robj)+sizeof(struct sdshdr)+len+1);

	struct sdshdr *sh = (void*)(o+1);

}