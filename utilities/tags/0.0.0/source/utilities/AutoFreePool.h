#ifndef __AUTO_FREE_POOL_H__
#define __AUTO_FREE_POOL_H__

// NOT THREAD SAFE

void AutoFreePool_push();
void AutoFreePool_pop();
void * AutoFreePool_add(void * address);
void AutoFreePool_addCallback(void (* callback)(void * context), void * context);
void AutoFreePool_empty();

#endif
