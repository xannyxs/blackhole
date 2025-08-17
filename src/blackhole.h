#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include <stdint.h>
typedef struct {
  double mass;
  double r_s;
  float x, y;
  float radius;
} blackhole_t;

blackhole_t *init_blackhole(double mass);

void draw_blackhole(const blackhole_t *bh, int32_t width, int32_t height);

void free_blackhole(blackhole_t *bh);

#endif /* BLACKHOLE_H */
