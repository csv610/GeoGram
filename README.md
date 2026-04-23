# GeoGram

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

**GeoGram** is a C++ programming library for geometric algorithms, providing a comprehensive set of geometry-processing functionalities.

> This repository is a maintained fork of the [original Geogram](https://github.com/BrunoLevy/geogram) by Bruno Levy. The fork adds CLI applications, a comprehensive unit test suite, and enhanced documentation.

## Features

### Core Functionality
- Surface reconstruction
- Remeshing and mesh repair
- Parameterization and texturing
- Boolean operations (intersections, unions, differences)
- Constructive Solid Geometry (CSG)

### Algorithms
- Exact predicates and arithmetic
- Delaunay triangulations (2D/3D, parallel)
- Constrained Delaunay triangulation (2D)
- Geometric search data structures (AABBs, KdTrees)
- Spectral mesh processing
- Linear solvers (CPU/GPU)

### New in This Fork

#### CLI Applications (`app/`)
| Application | Description |
|------------|------------|
| `geogram_app_csg` | CSG compilation (OpenSCAD-like syntax) |
| `geogram_app_compare` | Mesh comparison (Hausdorff distance) |
| `geogram_app_fillholes` | Automatic hole filling |
| `geogram_app_framefield` | Tangent direction field generation |
| `geogram_app_harmonics` | Manifold harmonics computation |
| `geogram_app_hull` | Convex hull (2D/3D) |
| `geogram_app_partition` | Mesh partitioning |
| `geogram_app_sample` | Random mesh sampling |
| `geogram_app_smooth` | Mesh smoothing |
| `geogram_app_tetra` | Tetrahedralization |

#### Unit Tests
Comprehensive test coverage across multiple categories:

| Test Suite | Description |
|-----------|------------|
| `test_memory` | Memory leak detection |
| `test_stress` | Large mesh handling |
| `test_edge_cases` | Numerical precision |
| `test_thread_safety` | Concurrency tests |
| `test_error_recovery` | Invalid input handling |
| `test_api_contract` | API verification |
| `test_pipeline` | End-to-end pipelines |
| `test_compare` | Mesh comparison |
| `test_partition` | Mesh partitioning |
| `test_fill_holes` | Hole filling |
| `test_framefield` | Frame field generation |
| `test_csg` | CSG operations |
| `test_sampling` | Mesh sampling |
| `test_tetrahedralize` | Tetrahedralization |
| `bench_perf` | Performance benchmarks |

## Building

### Prerequisites
- CMake 3.18+
- C++ compiler with C++17 support
- Optional: TetGen (for tetrahedralization)

### Build Instructions

```bash
mkdir build && cd build
cmake .. -DGEOGRAM_BUILD_TESTS=ON -DGEOGRAM_WITH_TETGEN=ON
make -j$(nproc)
```

### Running Tests

```bash
# Unit tests
./bin/test_*

# Benchmarks
./bin/bench_perf
```

## License

BSD 3-Clause License

Copyright (c) 2024-2026, csv610
Portions copyright (c) 2004-2024, Bruno Levy and contributors

## Acknowledgments

This code is derived from the [original Geogram project](https://github.com/BrunoLevy/geogram) by Bruno Levy. Geogram received the [Symposium on Geometry Processing Software Award](http://awards.geometryprocessing.org/) in 2023.

Original development was supported by:
- ERC GOODSHAPE and VORPALINE grants
- Inria exploratory action grants (EXPLORAGRAM, COSMOGRAM)
- PARMA Inria project

## Links

- [Original Geogram](https://github.com/BrunoLevy/geogram)
- [Original Documentation](https://brunolevy.github.io/geogram/)
- [Programmer's Reference](https://brunolevy.github.io/geogram/)