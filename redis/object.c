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

	struct sdshdr *sh = (void*)(o + 1);
	o->type = REDIS_STRING;
	o->encoding = REDIS_ENCODING_EMBSTR;
    o->ptr = sh+1;
    o->refcount = 1;
    o->lru = LRU_CLOCK();

    sh->len = len;
    sh->free = 0;
    if (ptr)
    {
    	memcpy(sh->buf,ptr,len);
    	/* code */
    	sh->buf[len] = '\0';
    }else{
    	memset(sh->buf,0,len+1);
    }
    return o;

}

#define REDIS_ENCODING_EMBSTR_SIZE_LIMIT 39
robj *createStringObject(char *ptr, size_t len) {
    if (len <= REDIS_ENCODING_EMBSTR_SIZE_LIMIT)
        return createEmbeddedStringObject(ptr,len);
    else
        return createRawStringObject(ptr,len);
}

robj *createStringObjectFromLongLong(long long value) {

    robj *o;

    // value 的大小符合 REDIS 共享整数的范围
    // 那么返回一个共享对象
    if (value >= 0 && value < REDIS_SHARED_INTEGERS) {
        incrRefCount(shared.integers[value]);
        o = shared.integers[value];

    // 不符合共享范围，创建一个新的整数对象
    } else {
        // 值可以用 long 类型保存，
        // 创建一个 REDIS_ENCODING_INT 编码的字符串对象
        if (value >= LONG_MIN && value <= LONG_MAX) {
            o = createObject(REDIS_STRING, NULL);
            o->encoding = REDIS_ENCODING_INT;
            o->ptr = (void*)((long)value);

        // 值不能用 long 类型保存（long long 类型），将值转换为字符串，
        // 并创建一个 REDIS_ENCODING_RAW 的字符串对象来保存值
        } else {
            o = createObject(REDIS_STRING,sdsfromlonglong(value));
        }
    }

    return o;
}

robj *createStringObjectFromLongDouble(long double value) {
    char buf[256];
    int len;

    /* We use 17 digits precision since with 128 bit floats that precision
     * after rounding is able to represent most small decimal numbers in a way
     * that is "non surprising" for the user (that is, most small decimal
     * numbers will be represented in a way that when converted back into
     * a string are exactly the same as what the user typed.) */
    // 使用 17 位小数精度，这种精度可以在大部分机器上被 rounding 而不改变
    len = snprintf(buf,sizeof(buf),"%.17Lf", value);

    /* Now remove trailing zeroes after the '.' */
    // 移除尾部的 0 
    // 比如 3.1400000 将变成 3.14
    // 而 3.00000 将变成 3
    if (strchr(buf,'.') != NULL) {
        char *p = buf+len-1;
        while(*p == '0') {
            p--;
            len--;
        }
        // 如果不需要小数点，那么移除它
        if (*p == '.') len--;
    }

    // 创建对象
    return createStringObject(buf,len);
}

robj *dupStringObject(robj *o) {
    robj *d;

    redisAssert(o->type == REDIS_STRING);

    switch(o->encoding) {

    case REDIS_ENCODING_RAW:
        return createRawStringObject(o->ptr,sdslen(o->ptr));

    case REDIS_ENCODING_EMBSTR:
        return createEmbeddedStringObject(o->ptr,sdslen(o->ptr));

    case REDIS_ENCODING_INT:
        d = createObject(REDIS_STRING, NULL);
        d->encoding = REDIS_ENCODING_INT;
        d->ptr = o->ptr;
        return d;

    default:
        redisPanic("Wrong encoding.");
        break;
    }
}


robj *createListObject(void)
{
	list *li = listCreate();
	robj *o  = createObject(REDIS_LIST,li);

	listSetFreeMethod(li,decrRefCountVoid);
	o->encoding = REDIS_ENCODING_LINKEDLIST;
	return o;
} 

robj *createZiplistObject(void) {

    unsigned char *zl = ziplistNew();

    robj *o = createObject(REDIS_LIST,zl);

    o->encoding = REDIS_ENCODING_ZIPLIST;

    return o;
}

robj *createSetObject(void) {

    dict *d = dictCreate(&setDictType,NULL);

    robj *o = createObject(REDIS_SET,d);

    o->encoding = REDIS_ENCODING_HT;

    return o;
}


robj *createIntsetObject(void) {

    intset *is = intsetNew();

    robj *o = createObject(REDIS_SET,is);

    o->encoding = REDIS_ENCODING_INTSET;

    return o;
}



robj *createHashObject(void) {

    unsigned char *zl = ziplistNew();

    robj *o = createObject(REDIS_HASH, zl);

    o->encoding = REDIS_ENCODING_ZIPLIST;

    return o;
}


robj *createZsetObject(void) {

    zset *zs = zmalloc(sizeof(*zs));

    robj *o;

    zs->dict = dictCreate(&zsetDictType,NULL);
    zs->zsl = zslCreate();

    o = createObject(REDIS_ZSET,zs);

    o->encoding = REDIS_ENCODING_SKIPLIST;

    return o;
}



robj *createZsetZiplistObject(void) {

    unsigned char *zl = ziplistNew();

    robj *o = createObject(REDIS_ZSET,zl);

    o->encoding = REDIS_ENCODING_ZIPLIST;

    return o;
}



void freeStringObject(robj *o) {
    if (o->encoding == REDIS_ENCODING_RAW) {
        sdsfree(o->ptr);
    }
}


void freeListObject(robj *o) {

    switch (o->encoding) {

    case REDIS_ENCODING_LINKEDLIST:
        listRelease((list*) o->ptr);
        break;

    case REDIS_ENCODING_ZIPLIST:
        zfree(o->ptr);
        break;

    default:
        redisPanic("Unknown list encoding type");
    }
}


void freeSetObject(robj *o) {

    switch (o->encoding) {

    case REDIS_ENCODING_HT:
        dictRelease((dict*) o->ptr);
        break;

    case REDIS_ENCODING_INTSET:
        zfree(o->ptr);
        break;

    default:
        redisPanic("Unknown set encoding type");
    }
}


void freeZsetObject(robj *o) {

    zset *zs;

    switch (o->encoding) {

    case REDIS_ENCODING_SKIPLIST:
        zs = o->ptr;
        dictRelease(zs->dict);
        zslFree(zs->zsl);
        zfree(zs);
        break;

    case REDIS_ENCODING_ZIPLIST:
        zfree(o->ptr);
        break;

    default:
        redisPanic("Unknown sorted set encoding");
    }
}



void freeHashObject(robj *o) {

    switch (o->encoding) {

    case REDIS_ENCODING_HT:
        dictRelease((dict*) o->ptr);
        break;

    case REDIS_ENCODING_ZIPLIST:
        zfree(o->ptr);
        break;

    default:
        redisPanic("Unknown hash encoding type");
        break;
    }
}


void incrRefCount(robj *o) {
    o->refcount++;
}
