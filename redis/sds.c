#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "sds.h"
#include "zmalloc.h"


sds sdsnewlen(const voids *init ,size_t initlen)
{
	struct sdshdr *sh;
	if (init)
	{
		/* code */
		sh = zmalloc(sizeof(struct sdshdr) + initlen + 1); 
	} else {
		sh = zcalloc(sizeof(struct sdshdr)+initlen+1);
	}

	if (sh == NULL)
	{
		return NULL;
		/* code */
	}

	sh->len = initlen;
	sh->free = 0;

	if (initlen && init)
	{
		memcpy(sh->buf,init,initlen);
		/* code */
	}
	sh->buf[initlen] = '\0';

	return (char *) sh->buf;
}


sds sdsempty(void)
{
	return sdsnewlen("",0);
}

sds sdsnew(const char *init)
{
	size_t initlen = (init == NULL) ? 0 : strlen(init);
	return sdsnewlen(init,initlen);
}

sds sdsdup(const sds s)
{
	return sdsnewlen(s,sdslen(s))
}

void sdsfree(sds s)
{
	if (s == NULL)
	{
		return;
		/* code */
	}
	zfree(s-sizeof(struct sdshdr));
}


void sdsupdatelen(sds s)
{
	struct sdshdr *sh ;
	sh = (void* )(s-(sizeof(struct sdshdr)));
	int reallen = strlen(s);
	sh->free += (sh->len - reallen);
	sh->len = reallen;
}


void sdsclear(sds s)
{
	struct sdshdr *sh = (void*) (s - (sizeof(struct sdshdr)));
	sh->free += sh->len;
	sh->len = 0;
	sh->buf[0] = '\0';
}

sds sdsMakeRoomFor(sds s,size_t addlen)
{
	struct sdshdr *sh,*newsh;
	size_t len , newlen;

	if (free >= addlen)
	{
		return s;
		/* code */
	}

	len = sdslen(s);
	sh = (void*) (s-(sizeof(struct sdshdr)));

	newlen = len + addlen;

	if (newlen < SDS_MAX_PREALLOC)
	{
		/* code */
		newlen *= 2;
	} else {
		newlen += SDS_MAX_PREALLOC;
	}

	newsh = zrealloc(sh, sizeof(struct sdshdr)+newlen+1);

}

sds sdsRemoveFreeSpace(sds s)
{
	struct sdshdr *sh;
	sh = (void*) (s-(sizeof(struct sdshdr)));

	sh = zrealloc(sh, sizeof(struct sdshdr)+sh->len+1);
	sh->free = 0;
	return sh->buf;
}

size_t sdsAllocSize(sds s)
{
	struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
	 return sizeof(*sh)+sh->len+sh->free+1;
}
void sdsIncrLen(sds s,int incr)
{
	struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
	assert(sh->free >= incr);
	sh->len += incr;
	sh->free -= incr;

	assert(sh->free >= 0);
	s[sh->len] = '\0';
}




