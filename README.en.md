# KYM-PBRT-cpp

This is a physically based rennderring engine realized using cpp. See master branch for renderring part (by [曾许曌秋](https://zxzq.me)) and cuda-dev branch for cuda support (by [洪亮](https://github.com/heathcliff233)).

Other language: [English](README.en.md), [中文](README.md)

Please update `WorkspaceDif` macro in settings.h before compiling and use "-fopenmp" as compiling argument in order to use openmp

<img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\02cylinder_20000spp.png">    <br>     <p align="center">P1 Sample<img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\02cylinder_20000spp.png">    <br>     <p align="center">图1 样例</p> </p> 

## functions:

- basic geometry: (currently) sphere/cube/plane/intersect/triangle/cylindder
- basic material: (currently) glass/lambertian/metal/source
- support dispersion
- support source/specular oriented Monte-Carlo sampling