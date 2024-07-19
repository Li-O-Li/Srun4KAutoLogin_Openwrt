#include "../includes/memBlock.h"


memBlock* allocateMemBlock(const size_t cellSize ,const uint32_t nCell){
    memBlock* res=(memBlock*)malloc(sizeof(memBlock));
    res->capacity = nCell*cellSize;
    res->data = malloc(nCell * cellSize);
    return res;
}


memBlock* copyMemBlock(const memBlock*m){
    memBlock* res = allocateMemBlock(1, m->capacity);
    memcpy(res->data, m->data, m->capacity);
    return res;
}


memBlock* reallocateMemBlock(memBlock* m, const size_t cellSize, const uint32_t newN){
    m->data = (uint32_t*)realloc(m->data, newN * cellSize);
    if(newN > m->capacity)
        memset((char *)m->data+ m->capacity*cellSize, 0, cellSize*(newN - m->capacity));
    m->capacity = newN;
    return m;
}


void freeMemBlock(memBlock* b){
    free(b->data);
    free(b);
}


void memBlockprintAsString(const memBlock* b){
    for(uint32_t i=0;i!=b->capacity-1;i++)
        putchar(((char *)b->data)[i]);
    putchar(((char *)b->data)[b->capacity-1]);
}