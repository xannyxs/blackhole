#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "consts.hpp"
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <math.h>

class Blackhole {
public:
  Blackhole(glm::vec2 pos, float schwarzschild_radius)
      : _position(pos), _rs(schwarzschild_radius) {
    _radius = _rs * 0.4f;
  };

  ~Blackhole() = default;

  void draw_blackhole(int32_t width, int32_t height) const {
    float aspect_ratio = (float)width / (float)height;

    glColor3f(1.0, 0.0, 0.0); // Black
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(_position.x / aspect_ratio, _position.y);
    for (int32_t i = 0; i <= 100; ++i) {
      float angle = 2.0f * M_PI * i / 100;
      float dx = _radius * cosf(angle);
      float dy = _radius * sinf(angle);
      glVertex2f((_position.x + dx) / aspect_ratio, _position.y + dy);
    }
    glEnd();
  };

  float get_rs() const { return _rs; }
  float get_radius() const { return _radius; }
  glm::vec2 get_position() const { return _position; }

private:
  glm::vec2 _position;
  float _rs;
  float _radius;
};

#endif /* BLACKHOLE_H */
