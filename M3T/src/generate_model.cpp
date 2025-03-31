

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

/// <summary>
/// Generate a model for a given .obj file. First argument is filepath, second geometry_unit_in_meter (optional), third geometry2body_pose x,y,z (optional)
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char *argv[]) {
    std::string geometry_path;
    float geometry_unit_in_meter = 1.0f;
    m3t::Transform3fA geometry2body_pose = m3t::Transform3fA::Identity();

    if (argc == 1) {
        std::string userInput;
        //no arguments given, promt for user Input:
        std::cout << "Generate a M3T-Model for a given.obj file:\n";
        std::cout << "> ";
        std::getline(std::cin, userInput);
        geometry_path = userInput;
        std::cout << "Geometry unit in meter (optional, default 1.0):\n";
        std::cout << "> ";
        std::getline(std::cin, userInput);
        if(!userInput.empty()) geometry_unit_in_meter = (float)std::stod(userInput);
        std::cout << "Geometry to Body-Pose[m] (optional, default 0.0 0.0 0.0):\n";
        std::cout << "> ";
        std::getline(std::cin, userInput);
        std::istringstream iss(userInput);
        float x, y, z;
        iss >> x >> y >> z;
        geometry2body_pose = m3t::Transform3fA(Eigen::Translation3f(x, y, z));
    }
    else {
        geometry_path = argv[1];
    }

    if (argc == 3 || argc == 6)
    {
        geometry_unit_in_meter = (float)std::stod(argv[2]);
    }
    else if (argc == 5)
    {
        geometry2body_pose = m3t::Transform3fA(Eigen::Translation3f(
            (float)std::stod(argv[2]), (float)std::stod(argv[3]),
            (float)std::stod(argv[4])));
    }
    else if (argc == 6)
    {
        geometry_unit_in_meter = (float)std::stod(argv[2]);
        geometry2body_pose = m3t::Transform3fA(Eigen::Translation3f(
            (float)std::stod(argv[3]), (float)std::stod(argv[4]),
            (float)std::stod(argv[5])));
    }

    // Remove leading and trailing quotes
    if (geometry_path.front() == L'\"' && geometry_path.back() == L'\"') {
        geometry_path = geometry_path.substr(1, geometry_path.size() - 2);
    }

    if (!std::filesystem::exists(geometry_path)) {
        std::wcout << L"The path is invalid or the file does not exist: " << geometry_path.c_str() << std::endl;
        return -1;
    }
    std::filesystem::path path(geometry_path);
    std::filesystem::path parentDirectory = path.parent_path(); // Path to the folder
    std::wstring baseName = path.stem().wstring(); // Base name 

    auto renderer_geometry_ptr{
        std::make_shared<m3t::RendererGeometry>("renderer_geometry")};
    // Set up body
    auto body_ptr{std::make_shared<m3t::Body>("body", path, geometry_unit_in_meter, false,
                                              true, geometry2body_pose)};
    renderer_geometry_ptr->AddBody(body_ptr);
    body_ptr->SetUp(true);
    renderer_geometry_ptr->SetUp();
    // Set up region model
    auto region_model_ptr{std::make_shared<m3t::RegionModel>(
        "region_model", body_ptr, parentDirectory / (baseName + L"_region_model.bin"))};

    region_model_ptr->SetUp();
    return 0;
}
