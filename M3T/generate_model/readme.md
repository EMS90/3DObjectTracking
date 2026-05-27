# generate_model — User Guide

Overview
- `generate_model` creates M3T models (`_region_model.bin`, `_depth_model.bin`) from a geometry file (`.obj`).
- The program uses the `m3t` library and expects an OBJ file as input.

Prerequisites
- C++17/C++14 toolchain (project uses Eigen and m3t)
- Project dependencies of the `m3t` library
- Eigen (for transform types)

Build
- Build the project with your usual workflow (CMake / Visual Studio depending on repository setup).

Usage
- Interactive (no command-line arguments):
  - Run `generate_model` ? enter the path, unit (optional) and geometry?body pose (optional).
  - If `_region_model.bin` or `_depth_model.bin` exist, the program asks: `Continue and overwrite? (j/n)`.

- Command line:
  - Minimal: `generate_model <path/to/model.obj>`
  - With unit: `generate_model <path/to/model.obj> 0.01`
  - With translation (geometry ? body pose): `generate_model <path/to/model.obj> <x> <y> <z>`
  - Unit + pose: `generate_model <path/to/model.obj> <unit> <x> <y> <z>`

Parameters / Behavior
- `geometry_unit_in_meter` scales the geometry (default `1.0`).
- `geometry2body_pose` is passed as `m3t::Transform3fA` to the `Body` constructor:
  - Semantics: `body = M * geometry + t` (matrix `M` and translation `t` are in body coordinates).

Common coordinate mappings
- Swap X?Y:- Example Unreal-like permutation (adjust and test):