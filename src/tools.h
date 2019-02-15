#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>
#include <math.h>

#define RAND() (((float) rand())/RAND_MAX)

float map(float x, float bin, float tin, float bout, float tout);
float map1(float x, float bout, float tout);
float sat(float x, float min, float max);
float sat1(float x);

void warning(char* msg);

// Basic geometry
typedef struct {
    float x;
    float y;
} point;

point add(point p1, point p2);
point sub(point p1, point p2);
point mul(point p, float t);
point rotate(point p1, float angle);
float dot(point p1, point p2);
float det(point p1, point p2);
float norm(point p);
float angle(point p1, point p2);

#endif
