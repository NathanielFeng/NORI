# NORI

<img src="images\final.png" />

<p align="center"><i>Use NORI Renderer, 1920×1080, 2048 spp, takes 2 hours. AMAZING!</i></p>

This is my own offline physically-based ray tracer, which aims to achieve photorealistic rendering. The program employs unbiased Monte Carlo integration of the rendering equation to simulate the complex process of light transport within the modeled scene.

This renderer is written in C++. It is first created in the [computer graphics course](https://cgl.ethz.ch/teaching/cg22/home.php) at ETH Zurich, and added with more functionalities afterwards.



## Framework

This renderer is built upon the educational ray tracer [Nori](https://github.com/wjakob/nori) by [Wenzel Jakob](https://rgl.epfl.ch/people/wjakob/).

The framework is written in C++ and runs on Windows, Linux, and Mac OS. It provides many features that would be tedious to implement from scratch. The following are included:

- A simple GUI to watch images as they render
- An XML-based scene file loader
- Basic point/vector/normal/ray/bounding box classes
- A pseudorandom number generator ([PCG32](http://www.pcg-random.org/))
- Support for saving output as [OpenEXR files](http://www.openexr.com/)
- A loader for Wavefront OBJ files
- Ray-triangle intersection
- Code for multi-threaded rendering
- Image reconstruction filters
- Statistical $χ^2$- and $t$-tests to verify sampling code
- A graphical visualization tool that can be used to inspect and validate sampling code.
- An efficient parallel [bounding volume hierarchy builder](http://www.sci.utah.edu/~wald/Publications/2007/ParallelBVHBuild/fastbuild.pdf)



## Features

Integrator: Path Tracing with Multiple Importance Sampling

Material: Diffuse, Mirror, Dielectric, Microfacet

Accelerator: BVH(SAH), Octree

Light: Area Light, Environment Map Light



## Results

|           BSDF Sampling            |   Multiple Importance Sampling    |
| :--------------------------------: | :-------------------------------: |
| <img src="images\cbox_mats.png" /> | <img src="images\cbox_mis.png" /> |

<p align="center"><i>Both rendered with 512 spp.</i></p>



|          Emitter Sampling          |    Multiple Importance Sampling    |
| :--------------------------------: | :--------------------------------: |
| <img src="images\veach_ems.png" /> | <img src="images\veach_mis.png" /> |

<p align="center"><i>Left rendered with 256 spp, right rendered with 128 spp.</i></p>



|             Ajax Rough              |             Ajax Smooth              |
| :---------------------------------: | :----------------------------------: |
| <img src="images\ajax-rough.png" /> | <img src="images\ajax-smooth.png" /> |

<p align="center"><i>Both rendered with 64 spp.</i></p>
