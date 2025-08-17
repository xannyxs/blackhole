#ifndef RAY_H
#define RAY_H

#include "blackhole.hpp"

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

class Blackhole;

class Ray {
public:
  Ray() = default;
  Ray(glm::vec2 dir, glm::vec2 pos) : _position(pos), _velocity(dir) {
    _trail.push_back(_position);
  }

  void step(glm::vec2 new_pos, glm::vec2 new_vel) {
    _position = new_pos;
    _velocity = new_vel;
    _trail.push_back(_position);
  }

  void draw(int32_t width, int32_t height) const {
    if (_trail.size() < 2)
      return;
    const float aspect_ratio = static_cast<float>(width) / height;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.5f);

    glBegin(GL_LINE_STRIP);
    for (size_t i = 0; i < _trail.size(); ++i) {
      float alpha = static_cast<float>(i) / (_trail.size() - 1);
      glColor4f(1.0, 1.0, 1.0, alpha);
      glVertex2f(_trail[i].x / aspect_ratio, _trail[i].y);
    }
    glEnd();
    glDisable(GL_BLEND);
  }

  glm::vec2 get_position() const { return _position; }
  glm::vec2 get_velocity() const { return _velocity; }
  bool is_captured(const Blackhole &bh) const {
    return glm::distance(_position, bh.get_position()) < bh.get_radius();
  }

private:
  glm::vec2 _position;
  glm::vec2 _velocity;
  std::vector<glm::vec2> _trail;
};

#endif // RAY_H
