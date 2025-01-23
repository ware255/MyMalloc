#ifndef _MYMALLOC_HPP_
#define _MYMALLOC_HPP_

void *MyMalloc(size_t size);
void *MyRealloc(void *ptr, size_t size);
void MyFree(void *ptr);

#endif
