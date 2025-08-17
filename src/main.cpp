#include "blackhole.hpp"
#include "ray.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#define NUM_RAYS 20
#define DELTA_LAMBDA 0.001

void geodesic(const double y_in[4], double dy_out[4], double rs) {
  double r = y_in[0];
  double dr = y_in[2];
  double dphi = y_in[3];

  if (r < 1e-6)
    return;

  dy_out[0] = dr;
  dy_out[1] = dphi;
  dy_out[2] = (r * dphi * dphi) - (1.5 * rs * dphi * dphi);
  dy_out[3] = -2.0 * dr * dphi / r;
}

void rk4_step(Ray &ray, double dl, double rs) {
  double y0[4] = {ray.get_r(), ray.get_phi(), ray.get_dr(), ray.get_dphi()};
  double y_final[4];
  double k1[4], k2[4], k3[4], k4[4];
  double y_temp[4];

  geodesic(y0, k1, rs);
  for (int i = 0; i < 4; ++i) {
    y_temp[i] = y0[i] + 0.5 * dl * k1[i];
  }
  geodesic(y_temp, k2, rs);

  for (int i = 0; i < 4; ++i) {
    y_temp[i] = y0[i] + 0.5 * dl * k2[i];
  }
  geodesic(y_temp, k3, rs);

  for (int i = 0; i < 4; ++i) {
    y_temp[i] = y0[i] + dl * k3[i];
  }
  geodesic(y_temp, k4, rs);

  for (int i = 0; i < 4; ++i) {
    y_final[i] =
        y0[i] + (dl / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
  }

  ray.step(y_final[0], y_final[1], y_final[2], y_final[3]);
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

  Blackhole bh;
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

    bh.draw_blackhole(width, height);
    const float visual_radius = bh.get_radius();

    for (auto &ray : rays) {
      if (!ray.is_captured(visual_radius)) {
        rk4_step(ray, DELTA_LAMBDA, visual_radius);
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
