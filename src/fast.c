#include <stdio.h>
#include <math.h>

double my_pow(double x, int y) {
    double res = 1;
    for (int i = 0; i < y; i++)
        res *= x;
    return res;
}

int calcSpeed(int level) {
    double res = 1;
    for (int i = 0; i < level; i++)
        res *= 0.8;
    return (int)1250 * res;
}

int main() {
    
    // for (int i=1; i < 11; i++) 
    //     printf("%d - mypow=%d - calcspeed=%d\n", (int)(1250 * pow(0.8, i)), (int)(1250 * my_pow(0.8, i)), calcSpeed(i));
    
    printf("%d\n", 600 / 600);//1
    printf("%d\n", 700 / 600);//1 надо 2
    printf("%d\n", 1300 / 600);//2
    printf("%d\n", 2000 / 600);//3
    return 0;
}