// SPDX-License-Identifier: MIT
// Copyright (c) 2023 Manuel Stoiber, German Aerospace Center (DLR)

#ifndef M3T_INCLUDE_M3T_RENDERER_GEOMETRY_H_
#define M3T_INCLUDE_M3T_RENDERER_GEOMETRY_H_

#if USE_OPENGL
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#endif
#include <m3t/body.h>
#include <m3t/common.h>

#include <filesystem/filesystem.h>
#include <Eigen/Dense>
#include <array>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace m3t {

/**
 * \brief Loads geometric information from referenced \ref Body objects in
 * *Vertex Array Objects (VAOs)* and *Vertex Buffers Objects (VBOs)* and
 * provides this information together with a *GLFW* context required for
 * rendering.
 *
 * \details Rendering data with *VAOs* and *VBOs* for all assigned
 * bodies is stored in `render_data_bodies`. The *GLFW* context can be handled
 * using `MakeContextCurrent()` and `DetachContext()`. Both the destructor and
 * the `SetUp()` method have to be called from the main thread to comply with
 * *GLFW* thread-safety requirements. Calling the `SetUp()` method initializes
 * *GLFW*, creates a *GLFW* context, and creates *VAOs* and *VBOs*. \ref Body
 * objects can be added and deleted without requiring a new call of `SetUp()`.
 * Setters and all main methods are thread-safe.
 *
 * @param body_ptrs referenced \ref Body objects that are considered in the
 * renderer geometry.
 */
class RendererGeometry {
 private:
  // Count the number of instances to manage the GLFW library
  static int n_instances_;

 public:
  // Data Structs
#if USE_OPENGL
  struct RenderDataBody {
    Body *body_ptr = nullptr;
    GLuint vao = 0;
    GLuint vbo = 0;
    unsigned n_vertices = 0;
  };
#endif

  // Constructor, destructor, and setup method
  RendererGeometry(const std::string &name);
  RendererGeometry(const RendererGeometry &) = delete;
  RendererGeometry &operator=(const RendererGeometry &) = delete;
  ~RendererGeometry();  // deletes glfw context
  virtual bool SetUp();        

  // Configure bodies
  virtual bool AddBody(const std::shared_ptr<Body> &body_ptr);
  bool DeleteBody(const std::string &name);
  void ClearBodies();

  // Handling of GLFW context
  bool MakeContextCurrent();
  bool DetachContext();

  // Getters
  const std::string &name() const;
  const std::vector<std::shared_ptr<Body>> &body_ptrs() const;
#if USE_OPENGL
  const std::vector<RenderDataBody> &render_data_bodies() const;
#endif
  bool set_up() const;

 private:
  // Helper methods
  static void AssembleVertexData(const Body &body,
                                 std::vector<float> *vertex_data);
#if USE_OPENGL                               
  static void CreateGLVertexObjects(const std::vector<float> &vertices,
                                    RenderDataBody *render_data_body);
  static void DeleteGLVertexObjects(RenderDataBody *render_data_body);
#endif

  // Variables
  std::string name_{};

  protected:
  std::vector<std::shared_ptr<Body>> body_ptrs_;
#if USE_OPENGL
  std::vector<RenderDataBody> render_data_bodies_;
  GLFWwindow *window_ = nullptr;  // Only used to hold a glfw context
#endif
  std::mutex mutex_;
  bool initial_set_up_ = false;
  bool set_up_ = false;
};

}  // namespace m3t

#endif  // M3T_INCLUDE_M3T_RENDERER_GEOMETRY_H_
