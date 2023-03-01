//詹倚飞 2021201684
#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct
{
    int valid;
    int tag;
    int timestamp;
} Block;
int s = -1, E = -1, b = -1, verbosity = 0;
int S = 0, B = 0;
int hit = 0, miss = 0, eviction = 0;
int alltime = 0;
FILE* file = NULL;
Block** cache;
void Init()
{
    cache = (Block **)malloc(sizeof(Block) * S);
    for (int i = 0; i < S; ++i)
    {
        cache[i] = (Block *)malloc(24LL * E);
        for (int j = 0; j < E; ++j)
        {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0LL;
            cache[i][j].timestamp = 0LL;
        }
    }
}
void visit(unsigned long long addr)
{
    Block *set = cache[(addr >> b) % S];
    unsigned long tag = addr >> (s + b);
    unsigned int line = 0;
    unsigned long mintime = ~0;
    int i;
    for (i = 0;; ++i)
    {
        if (set[i].tag == tag && set[i].valid)
        {
            hit++;
            if (verbosity)
                printf("hit ");
            set[i].timestamp = alltime++;
            break;
        }
        if (i >= E)
        {
            miss++;
            if (verbosity)
                printf("miss ");
            for (int templine = 0; templine < E; templine++)
            {
                if (set[templine].timestamp < mintime)
                {
                    line = templine;
                    mintime = set[templine].timestamp;
                }
            }
            if (set[line].valid)
            {
                eviction++;
                if (verbosity)
                    printf("eviction ");
            }
            set[line].valid = 1;
            set[line].tag = tag;
            set[line].timestamp = alltime++;
            return;
        }
    }
}
void Deal()
{
    unsigned len = 0;
    unsigned long long addr = 0;
    char buf[1000];
    while (fgets(buf, 1000, file))
    {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M')
        {
            sscanf(&buf[3], "%llx,%u", &addr, &len);
            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);
            visit(addr);
            if (buf[1] == 'M')
                visit(addr);
            if (verbosity)
                puts("\n");
        }
    }
}
void Help(char **argv)
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", *argv);
    puts("Options:");
    puts("  -h         Print this help message.");
    puts("  -v         Optional verbose flag.");
    puts("  -s <num>   Number of set index bits.");
    puts("  -E <num>   Number of lines per set.");
    puts("  -b <num>   Number of block offset bits.");
    puts("  -t <file>  Trace file.");
    puts("\nExamples:");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", *argv);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", *argv);
    exit(0);
}
int main(int argc, const char **argv, const char **envp)
{
    char op = 0;
    while ((op = getopt(argc, (char *const *)argv, "s:E:b:t:vh")) != -1)
    {
        switch (op)
        {
        case 's':
            s = atoi(optarg);
            S = 1 << s;
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            B = 1 << b;
            break;
        case 't':
            file = fopen(optarg, "r");
            break;
        case 'h':
            Help((char **)argv);
        case 'v':
            verbosity = 1;
            break;
        default:
            Help((char **)argv);
        }
    }
    if (s == -1 || E == -1 || b == -1 || file == NULL)
    {
        printf("%s: Missing required command line argument\n", *argv);
        Help((char **)argv);
    }
    Init();
    Deal();
    fclose(file);
    free(cache);
    printSummary(hit, miss, eviction);
    return 0;
}
