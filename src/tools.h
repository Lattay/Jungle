#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>
#include <math.h>

#define RAND() (((float) rand())/RAND_MAX)
#define PI 3.141592653589793
#define HALF_PI 1.570796326794897

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

point add(const point p1, const point p2);
point sub(const point p1, const point p2);
point mul(const point p, float t);
point rotate(const point p1, float angle);
float dot(const point p1, const point p2);
float det(const point p1, const point p2);
float norm(const point p);
float angle(const point p1, const point p2);

#endif
