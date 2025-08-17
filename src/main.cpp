#include "blackhole.hpp"
#include "ray.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <vector>

#define NUM_RAYS 100
#define DELTA_LAMBDA 0.001

glm::vec2
calculate_total_acceleration(glm::vec2 ray_pos, glm::vec2 ray_vel,
                             const std::vector<Blackhole> &blackholes) {
  glm::vec2 total_accel = {0.0f, 0.0f};

  for (const auto &bh : blackholes) {
    glm::vec2 rel_pos = ray_pos - bh.get_position();
    double r_sq = glm::length2(rel_pos);

    if (r_sq < 1e-9)
      continue;

    double rs = bh.get_rs();

    double dphi = (rel_pos.x * ray_vel.y - rel_pos.y * ray_vel.x) / r_sq;
    double dphi_sq = dphi * dphi;

    float accel_magnitude = static_cast<float>(1.5 * rs * dphi_sq);

    glm::vec2 accel_vec = -glm::normalize(rel_pos) * accel_magnitude;

    total_accel += accel_vec;
  }
  return total_accel;
}

void geodesic_cartesian(const double y_in[4], double dy_out[4],
                        const std::vector<Blackhole> &blackholes) {
  glm::vec2 pos = {static_cast<float>(y_in[0]), static_cast<float>(y_in[1])};
  glm::vec2 vel = {static_cast<float>(y_in[2]), static_cast<float>(y_in[3])};

  dy_out[0] = vel.x;
  dy_out[1] = vel.y;

  glm::vec2 accel = calculate_total_acceleration(pos, vel, blackholes);
  dy_out[2] = accel.x;
  dy_out[3] = accel.y;
}

void rk4_step(Ray &ray, const std::vector<Blackhole> &blackholes, double dl) {
  double y0[4] = {ray.get_position().x, ray.get_position().y,
                  ray.get_velocity().x, ray.get_velocity().y};
  double y_final[4];
  double k1[4], k2[4], k3[4], k4[4];
  double y_temp[4];

  geodesic_cartesian(y0, k1, blackholes);
  for (int i = 0; i < 4; ++i)
    y_temp[i] = y0[i] + 0.5 * dl * k1[i];
  geodesic_cartesian(y_temp, k2, blackholes);
  for (int i = 0; i < 4; ++i)
    y_temp[i] = y0[i] + 0.5 * dl * k2[i];
  geodesic_cartesian(y_temp, k3, blackholes);
  for (int i = 0; i < 4; ++i)
    y_temp[i] = y0[i] + dl * k3[i];
  geodesic_cartesian(y_temp, k4, blackholes);

  for (int i = 0; i < 4; ++i) {
    y_final[i] =
        y0[i] + (dl / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
  }

  ray.step({(float)y_final[0], (float)y_final[1]},
           {(float)y_final[2], (float)y_final[3]});
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  (void)scancode;
  (void)mods;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int32_t main(void) {
  bool init = glfwInit();
  if (!init) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  GLFWwindow *window = glfwCreateWindow(800, 600, "Blackhole", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  int32_t width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  std::vector<Blackhole> blackholes;
  blackholes.emplace_back(Blackhole({-0.4f, 0.0f}, 0.2f));
  blackholes.emplace_back(Blackhole({0.4f, 0.3f}, 0.3f));
  blackholes.emplace_back(Blackhole({0.6f, -0.8f}, 0.6f));

  std::vector<Ray> rays;
  for (int i = 0; i < NUM_RAYS; ++i) {
    float y_pos = -1.2f + 2.4f * static_cast<float>(i) / (NUM_RAYS - 1);
    rays.emplace_back(glm::vec2(1.0f, 0.0f), glm::vec2(-1.5f, y_pos));
  }

  while (!glfwWindowShouldClose(window)) {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &bh : blackholes) {
      bh.draw_blackhole(width, height);
    }

    for (auto &ray : rays) {
      bool captured = false;
      for (const auto &bh : blackholes) {
        if (ray.is_captured(bh)) {
          captured = true;
          break;
        }
      }

      if (!captured) {
        rk4_step(ray, blackholes, DELTA_LAMBDA);
      }
      ray.draw(width, height);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  std::cout << "Closing window." << std::endl;

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
