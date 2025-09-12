#include <time.h>
#include <stdio.h>

int main() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    
    long seconds = ts.tv_sec;
    long nanoseconds = ts.tv_nsec;
    long microseconds = nanoseconds / 1000;
    
    printf("Seconds: %ld, Microseconds: %ld\n", seconds, microseconds);
    return 0;
}