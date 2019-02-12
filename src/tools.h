#ifndef TOOLS_H
#define TOOLS_H

#define scale(x, in, out) (x * out / in)

float map(float x, float bin, float tin, float bout, float tout);
float map1(float x, float bout, float tout);
float sat(float x, float min, float max);
float sat1(float x);

#endif
