#include "ray.h"
#include "GL/gl.h"

#include <stdio.h>
#include <stdlib.h>

void step_ray(ray_t *ray) {
  const float step_size = 0.005f;

  ray->position.x += ray->dir.x * step_size;
  ray->position.y += ray->dir.y * step_size;
}

void draw_ray(const ray_t *ray, int width, int height) {
  const float aspect_ratio = (float)width / (float)height;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(1.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glPointSize(2.0f);

  glBegin(GL_LINE_STRIP);
  for (size_t i = 0; i < N; i += 1) {
    glVertex2f(ray->position.x / aspect_ratio, ray->position.y);
  }
  glEnd();
}

ray_t *init_ray(dir_t dir, position_t position) {
  ray_t *ray = malloc(sizeof(ray_t));
  if (!ray) {
    perror("Failed to allocate memory for ray");
    exit(EXIT_FAILURE);
  }

  ray->position = position;
  ray->dir = dir;
  ray->trail = (trail_t){0, 0};

  return ray;
}
