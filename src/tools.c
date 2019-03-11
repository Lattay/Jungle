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

void warning(char* msg){
  fprintf(stderr, "[WARNING] %s\n", msg);
}

point add(const point p1, const point p2){
  return (point) {
    p1.x + p2.x,
      p1.y + p2.y
  };
}

point sub(const point p1, const point p2){
  return (point) {
    p1.x - p2.x,
      p1.y - p2.y
  };
}

point mul(const point p, float t){
  return (point) {
    t * p.x,
      t * p.y
  };
}

point neg(const point p){
  return (point) {
    -p.x,
      -p.y
  };
}

float dot(const point p1, const point p2){
  return p1.x * p2.x + p1.y * p2.y;
}

float det(const point p1, const point p2){
  return p1.x * p2.y - p1.y * p2.x;
}

float norm(const point p){
  return sqrt(p.x * p.x + p.y * p.y);
}

float angle(const point p1, const point p2){
  float cos = dot(p1, p2)/norm(p1)/norm(p2);
  float d = det(p1, p2);
  if(d == 0){
    return 0.0;      
  } else if(d > 0){
    return acos(cos);
  } else {
    return -acos(cos);
  }
}

point rotate(const point p1, float angle){
  return (point) {
    cos(angle) * p1.x - sin(angle) * p1.y,
      sin(angle) * p1.x + cos(angle) * p1.y
  };
}
