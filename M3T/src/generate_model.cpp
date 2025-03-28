// SPDX-License-Identifier: MIT
// Copyright (c) 2023 Manuel Stoiber, German Aerospace Center (DLR)

#include <filesystem/filesystem.h>
#include <m3t/basic_depth_renderer.h>
#include <m3t/body.h>
#include <m3t/common.h>
#include <m3t/loader_camera.h>
#include <m3t/manual_detector.h>
#include <m3t/normal_viewer.h>
#include <m3t/region_modality.h>
#include <m3t/renderer_geometry.h>
#include <m3t/static_detector.h>
#include <m3t/tracker.h>

#include <Eigen/Geometry>
#include <memory>

// Example script for the detector usage on the data provided in data/sequence
// with the object triangle
int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cerr << "Not enough arguments: Provide geometry path";
    //return -1;
  }
  std::filesystem::path geometry_path;
  //{argv[1]};
  if (argc > 2 && argc != 5) {
    std::cerr << "Invalid number of arguments: Provide complete Translation X, Y, Z";
    return -1;
  }
  m3t::Transform3fA geometry2body_pose = m3t::Transform3fA::Identity();
  if (argc == 4) {  
    geometry2body_pose = m3t::Transform3fA(Eigen::Translation3f(
        (float)std::stod(argv[3]), (float)std::stod(argv[4]),
        (float)std::stod(argv[5])));
    geometry2body_pose =
        m3t::Transform3fA(Eigen::Translation3f(0, 0, -0.006));
  }
  geometry_path = std::filesystem::path(
      "C:\\Users\\emanu\\Documents\\Unreal Projects\\HoloLensPlugIn\\Plugins\\M3T4Unreal\\Content\\Meshes\\T-Probe\\TProbe.obj");
  geometry_path = std::filesystem::path(
      "C:\\Users\\emanu\\Documents\\Unreal "
      "Projects\\HoloLensPlugIn\\Plugins\\M3T4Unreal\\Con"
      "tent\\Meshes\\Triangle\\triangle.obj");
  // Set up tracker and renderer geometry
  //auto tracker_ptr{std::make_shared<m3t::Tracker>("tracker")};
  auto renderer_geometry_ptr{
      std::make_shared<m3t::RendererGeometry>("renderer_geometry")};
  // Set up body
  auto body_ptr{std::make_shared<m3t::Body>("body", geometry_path, 1.0, false,
                                            true, geometry2body_pose)};
  renderer_geometry_ptr->AddBody(body_ptr);
  body_ptr->SetUp(true);
  renderer_geometry_ptr->SetUp();
  // Set up region mode
  auto region_model_ptr{std::make_shared<m3t::RegionModel>(
      "region_model", body_ptr, geometry_path.replace_extension(".bin"))};

  region_model_ptr->SetUp();

  //// Set up region modality
  //auto region_modality_ptr{std::make_shared<m3t::RegionModality>(
  //    "region_modality", body_ptr, camera_ptr, region_model_ptr)};
  //region_modality_ptr->set_visualize_pose_result(true);
  ///*region_modality_ptr->set_visualize_points_silhouette_rendering_correspondence(
  //    true);*/
  //region_modality_ptr->set_visualize_lines_correspondence(true);
  //region_modality_ptr->set_visualize_gradient_optimization(true);
  //region_modality_ptr->set_visualize_hessian_optimization(true);
  //region_modality_ptr->set_visualize_points_silhouette_rendering_correspondence(
  //    true);
  //// region_modality_ptr->set_visualize_points_optimization(true);
  //region_modality_ptr->set_display_visualization(true);
  //// Set up link
  //auto link_ptr{std::make_shared<m3t::Link>("link", body_ptr)};
  //link_ptr->AddModality(region_modality_ptr);

  //// Set up optimizer
  //auto optimizer_ptr{std::make_shared<m3t::Optimizer>("optimizer", link_ptr)};
  //tracker_ptr->AddOptimizer(optimizer_ptr);

  //// Set up detector
  ///*auto detector_ptr{std::make_shared<m3t::ManualDetector>(
  //    "detector", detector_metafile_path, optimizer_ptr, camera_ptr)};*/
  //auto detector_ptr{std::make_shared<m3t::StaticDetector>(
  //    "detector", detector_metafile_path, optimizer_ptr)};
  //tracker_ptr->AddDetector(detector_ptr);

  //// Start tracking
  //if (!tracker_ptr->SetUp()) return -1;
  //if (!tracker_ptr->RunTrackerProcess(true, true)) return -1;
  return 0;
}
