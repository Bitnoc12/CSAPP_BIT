// 1820211062 洪子翔
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
    int valid;
    unsigned tag;
    int timestamp;
} CacheLine;

char* traceFilePath = NULL;
int setBits, lineCount, blockBits, numSets;
int hits = 0, misses = 0, evictions = 0;
CacheLine** cache = NULL;

// 函数声明
void initializeCache();
void accessCache(unsigned address);
void updateTimestamps();

int main(int argc, char *argv[]) {
    // 首先透过getopt得到参数
    int opt;
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch (opt) {
            case 's':
                setBits = atoi(optarg);
                break;
            case 'E':
                lineCount = atoi(optarg);
                break;
            case 'b':
                blockBits = atoi(optarg);
                break;
            case 't':
                traceFilePath = optarg;
                break;
        }
    }

    // 因为总共的大小是 2^s
    numSets = 1 << setBits;

    // 初始化缓存
    initializeCache();

    FILE* traceFile = fopen(traceFilePath, "r");
    if (traceFile == NULL) {
        printf("打开文件错误");
        exit(-1);
    }

    char operation;
    unsigned address;
    int size;
    while (fscanf(traceFile, " %c %x,%d", &operation, &address, &size) > 0) {
        switch (operation) {
            case 'L':
            case 'M':
            case 'S':
                accessCache(address);
                break;
        }
        updateTimestamps();
    }

    // 释放缓存
    for (int i = 0; i < numSets; i++)
        free(*(cache + i));
    free(cache);
    fclose(traceFile);
    printSummary(hits, misses, evictions);
    return 0;
}


// 初始化缓存
void initializeCache() {
    cache = (CacheLine**)malloc(sizeof(CacheLine*) * numSets);
    for (int i = 0; i < numSets; i++) {
        *(cache + i) = (CacheLine*)malloc(sizeof(CacheLine) * lineCount);
        for (int j = 0; j < lineCount; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0xffffffff;
            cache[i][j].timestamp = 0;
        }
    }
}

// 访问缓存
// 这里要实现击中or没击中两种情况(包括LRU)去做考虑
void accessCache(unsigned address) {
    unsigned setIndex = (address >> blockBits) & ((0xffffffff) >> (32 - setBits));
    unsigned tagIndex = address >> (setBits + blockBits);

    for (int i = 0; i < lineCount; i++) {
        CacheLine* currentLine = cache[setIndex] + i;
        if (currentLine->valid && currentLine->tag == tagIndex) {
            currentLine->timestamp = 0; // 命中，将时间戳设为0
            hits++;
            return;
        }
    }

    for (int i = 0; i < lineCount; i++) {
        CacheLine* currentLine = cache[setIndex] + i;
        if (!currentLine->valid) {
            currentLine->tag = tagIndex;
            currentLine->valid = 1;
            currentLine->timestamp = 0;
            misses++;
            return;
        }
    }

    int maxTimestamp = 0;
    int maxIndex = 0;
    for (int i = 0; i < lineCount; i++) {
        if (cache[setIndex][i].timestamp > maxTimestamp) {
            maxTimestamp = cache[setIndex][i].timestamp;
            maxIndex = i;
        }
    }
    evictions++;
    misses++;
    cache[setIndex][maxIndex].tag = tagIndex; // 实现LRU策略，替换
    cache[setIndex][maxIndex].timestamp = 0;
}

// 更新时间戳
void updateTimestamps() {
    for (int i = 0; i < numSets; i++) {
        for (int j = 0; j < lineCount; j++) {
            if (cache[i][j].valid)
                cache[i][j].timestamp++;
        }
    }
}

