#include "fibonacci.h"
#include "library.h"
#include <stdint.h>

#define FIBO0 0
#define FIBO1 1

static uint32_t current = 0;
static uint64_t t1 = FIBO0, t2 = FIBO1;

static void printFirstFibos(uint8_t fd){
    printkfd(fd, "Fibonacci Series:\nFibo(0) = %d\nFibo(1) = %d\n",t1,t2);
}

void nextFibo(uint8_t fd){
    if(t1 == FIBO0 && t2 == FIBO1)
        printFirstFibos(fd);

    uint64_t nextTerm = t1 + t2;
    printkfd(fd, "Fibo(%d) = %d\n", current, nextTerm);
    t1 = t2;
    t2 = nextTerm;
    current++;

}

void restartFibo(){
    current = 0;
    t1 = FIBO0;
    t2 = FIBO1;
}

void fibo(){
    restartFibo();
    while(1){
        nextFibo(STDOUT);
    }
}