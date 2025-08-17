#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "consts.hpp"

#include <GL/gl.h>
#include <glm/ext/vector_float2.hpp>
#include <math.h>

class Blackhole {
public:
  Blackhole(void) : _position(0.0f), _mass(10.0 * SOLAR_MASS) {
    _rs = (2 * G * _mass) / (C * C);
    _radius = 0.5f;
  };
  Blackhole(double mass) : _position(0.0f), _mass(mass) {
    _rs = (2 * G * _mass) / (C * C);
    _radius = 0.5f;
  };

  ~Blackhole(void) = default;

  void draw_blackhole(int32_t width, int32_t height) {
    float aspect_ratio = (float)width / (float)height;

    glColor3f(1.0, 0, 0);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(_position.x, _position.y);
    for (int32_t i = 0; i <= 100; i += 1) {
      float angle = 2.0f * M_PI * i / 100;
      float dx = _radius * cosf(angle);
      float dy = _radius * sinf(angle);
      glVertex2f(_position.x + (dx / aspect_ratio), _position.y + dy);
    }

    glEnd();
  };

  float get_rs() const { return _rs; }

  float get_radius() const { return _radius; }

private:
  glm::vec2 _position;
  double _mass;
  float _rs;
  float _radius;
};

#endif /* BLACKHOLE_H */
