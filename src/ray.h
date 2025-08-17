#ifndef RAY_H
#define RAY_H

typedef struct {
  float x, y;
} dir_t;

typedef struct {
  float x, y;
} position_t;

typedef struct {
  float x, y;
} trail_t;

typedef struct {
  position_t position;
  dir_t dir;
  trail_t trail;
} ray_t;

void step_ray(ray_t *ray);

void draw_ray(const ray_t *ray, int width, int height);

ray_t *init_ray(dir_t dir, position_t position);

#endif /* RAY_H */
