// A simple memBlock with capacity indicator
#ifndef _MEMBLOCK_H
#define _MEMBLOCK_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct memBlock{
    uint32_t capacity;
    void *data;
} memBlock;


// basic Functions:alloc, realloc & copy & free
memBlock* allocateMemBlock(const size_t cellSize ,const uint32_t capacity);

memBlock* reallocateMemBlock(memBlock* m, const size_t cellSize, const uint32_t newCapacity);

memBlock* copyMemBlock(const memBlock*m);

void freeMemBlock(memBlock* b);

void memBlockprintAsString(const memBlock* b);

#endif