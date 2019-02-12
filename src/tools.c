#include "tools.h"

float map(float x, float bin, float tin, float bout, float tout){
    return bout + (x - bin) * (tout - bout) / (tin - bin);
}

float map1(float x, float bout, float tout){
    return bout + x  * (tout - bout);
}

float sat(float x, float min, float max){
    if(x > max){
        return max;
    } else if(x < min){
        return min;
    } else {
        return x;
    }
}

float sat1(float x){
    if(x > 1.0){
        return 1.0;
    } else if(x < 0.0){
        return 0.0;
    } else {
        return x;
    }
}
