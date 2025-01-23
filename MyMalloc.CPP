#include <cstddef>
#include "MyMalloc.HPP"

#define MemoryMax 500000000

static char memory[MemoryMax] = {0};
static size_t NextPos = 0;
static bool FirstFreed = false;

struct block {
    size_t size;
    bool free;
};

void *MyMalloc(size_t size) {
    if (size <= 0 || size + sizeof(block) >= MemoryMax)
        return NULL;

    if (NextPos + size + sizeof(block) > MemoryMax)
        return NULL;

    if (FirstFreed) {
        block *Block;
        for (size_t i = 0; i < NextPos; i += Block->size + sizeof(block)) {
            Block = (block *)(memory + i);
            if (Block->free && Block->size >= size) {
                Block->free = false; // used
                return (void *)(Block + 1);
            }
        }
    }

    block *newBlock = (block *)(memory + NextPos);
    newBlock->size = size;
    newBlock->free = false; // used
    NextPos = NextPos + size + sizeof(block);

    return (void *)(newBlock + 1);
}

void *MyRealloc(void *ptr, size_t size) {
    if (size <= 0 || size + sizeof(block) >= MemoryMax)
        return NULL;

    MyFree(ptr);
    void *newPtr = MyMalloc(size);

    if (newPtr == ptr) return newPtr;

    block *oldBlock = (block *)((char *)ptr - sizeof(block));
    size_t len = oldBlock->size;
    char *dest = (char *)newPtr;
    const char *src = (const char*)ptr;
    while (len--) *dest++ = *src++;

    return (void *)newPtr;
}

void MyFree(void *ptr) {
    if (!ptr) return;
    block *Block = (block *)((char *)ptr - sizeof(block));
    Block->free = true; // unused
    if (!FirstFreed) FirstFreed = true;
}
