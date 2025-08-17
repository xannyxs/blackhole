#include "blackhole.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const double SOLAR_MASS = 1.989e30;

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
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  GLFWwindow *window = glfwCreateWindow(800, 600, "My Title", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  int32_t width, height;
  blackhole_t *bh = init_blackhole(10.0 * SOLAR_MASS);
  while (!glfwWindowShouldClose(window)) {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_blackhole(bh, width, height);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  printf("Closing window.\n");
  free_blackhole(bh);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
