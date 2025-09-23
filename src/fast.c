//#define _POSIX_C_SOURCE 199309L
// #include <unistd.h>
#include <time.h>
#include <stdio.h>

// void delay_ms(int milliseconds) {
//     // usleep(milliseconds * 1000); // мс → мкс
//     struct timespec ts;
//     ts.tv_sec = milliseconds / 1000;                  // секунды
//     ts.tv_nsec = (milliseconds % 1000) * 1000000L;    // наносекунды
//     nanosleep(&ts, NULL);
// }

long long get_time() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
  return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000LL;
}

void delay_ms(int milliseconds) {
    long long time = get_time();
    while(get_time() - time < milliseconds) ;
}

int main() {
    delay_ms(500);
    // sleep(1);
    
    // printf("Seconds: %ld, Microseconds: %ld\n", seconds, microseconds);
    return 0;
}