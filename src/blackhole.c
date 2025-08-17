#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "GL/gl.h"
#include "blackhole.h"
#include "consts.h"

void draw_blackhole(const blackhole_t *bh, int32_t width, int32_t height) {
  float aspect_ratio = (float)width / (float)height;

  glColor3f(1.0, 0, 0);
  glBegin(GL_TRIANGLE_FAN);

  glVertex2f(bh->x, bh->y);
  for (int32_t i = 0; i <= 100; i += 1) {
    float angle = 2.0f * M_PI * i / 100;
    float dx = bh->radius * cosf(angle);
    float dy = bh->radius * sinf(angle);
    glVertex2f(bh->x + (dx / aspect_ratio), bh->y + dy);
  }

  glEnd();
}

blackhole_t *init_blackhole(double mass) {
  blackhole_t *bh = malloc(sizeof(blackhole_t));
  if (!bh) {
    perror("Failed to allocate memory for black hole");
    exit(EXIT_FAILURE);
  }
  bh->mass = mass;
  bh->r_s = (2 * G * bh->mass) / (C * C);

  bh->x = 0.0f;
  bh->y = 0.0f;
  bh->radius = 0.5f;

  return bh;
}
