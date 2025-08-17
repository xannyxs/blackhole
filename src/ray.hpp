#ifndef RAY_H
#define RAY_H

#include <GL/gl.h>
#include <cmath>
#include <cstddef>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <vector>

class Ray {
public:
  Ray(void) = default;
  Ray(glm::vec2 dir, glm::vec2 pos) : x(pos.x), y(pos.y) {
    _r = std::hypot(x, y);
    _phi = std::atan2(y, x);

    if (_r > 0) {
      dr = (x * dir.x + y * dir.y) / _r;
      dphi = (x * dir.y - y * dir.x) / (_r * _r);
    } else {
      dr = 0;
      dphi = 0;
    }
    _trail.push_back({x, y});
  }

  ~Ray(void) = default;

  void step(double new_r, double new_phi, double new_dr, double new_dphi) {
    _r = new_r;
    _phi = new_phi;
    dr = new_dr;
    dphi = new_dphi;

    x = _r * std::cos(_phi);
    y = _r * std::sin(_phi);
    _trail.push_back({x, y});
  }

  void draw(int32_t width, int32_t height) {
    if (_trail.size() < 2) {
      return;
    }
    const float aspect_ratio = (float)width / (float)height;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.5f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);

    glBegin(GL_LINE_STRIP);
    for (std::size_t i = 0; i < _trail.size(); ++i) {
      float alpha = static_cast<float>(i) / (_trail.size() - 1);
      glColor4f(1.0, 1.0, 1.0, alpha);
      glVertex2f(_trail[i].x / aspect_ratio, _trail[i].y);
    }

    glEnd();
    glDisable(GL_BLEND);
  }

  double get_r() const { return _r; }
  double get_dr() const { return dr; }
  double get_phi() const { return _phi; }
  double get_dphi() const { return dphi; }

  bool is_captured(float visual_rs) const { return _r < visual_rs; }

private:
  double _r, _phi;
  double dr, dphi;

  double x, y;
  std::vector<glm::vec2> _trail;
};

#endif /* RAY_H */
