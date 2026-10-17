# OpenCV Polygon Color Filler

A small C++17 demo that detects connected regions in a binary mask and fills each region with the most frequent color from a source image.

## How it works

1. Loads `img/mask.png` as a grayscale mask and `img/color_mask.png` as a color image.
2. Thresholds the mask and finds its connected regions.
3. Finds the dominant source-image color in each region and fills that region.
4. Saves the result to `img/result_output.png` and prints the processing time.

## Requirements

- A C++17-compatible compiler
- [CMake](https://cmake.org/) 3.15 or newer
- [Conan 2](https://conan.io/)

OpenCV and its dependencies are declared in `conanfile.py` and installed by Conan.

## Build and run

From the repository root, run:

```powershell
conan install . --output-folder=build --build=missing
cmake -S . -B build/build -DCMAKE_TOOLCHAIN_FILE=build/build/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/build --config Release
.\build\build\Release\app.exe
```

The program expects the input images at the paths above, relative to the current working directory. Run it from the repository root; the generated result will be written to `img/result_output.png`.
