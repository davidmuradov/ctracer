# The Ray Tracer Challenge in C.

## Chapter 5
![Final render Chapter 5](./out/chap5.png "Final render Chapter 5")


## Chapter 6
![Final render Chapter 6](./out/chap6.png "Final render Chapter 6")


## Chapter 7
![Final render Chapter 7](./out/chap7.png "Final render Chapter 7")


## Chapter 8
The same scene from Chapter 7 was rendered with shadows using 2 point light sources.
![Final render Chapter 8](./out/chap8.png "Final render Chapter 8")


## Chapter 9
A scene renderer with planes for the floor and the walls.
![Final render Chapter 9](./out/chap9.png "Final render Chapter 9")


## Chapter 10
Patterns. Only used gradient and checker patterns for this scene, but
stripes and ring patterns are available too.
![Final render Chapter 10](./out/chap10.png "Final render Chapter 10")


## Chapter 11
Reflection and refraction. Scene with reflection only :
![Final renders Chapter 11](./out/chap11_1.png "Final renders Chapter 11")

Scene with reflection and refraction :
![Final renders Chapter 11](./out/chap11_2.png "Final renders Chapter 11")


## Chapter 12
A scene with cubes.
![Final render Chapter 12](./out/chap12.png "Final render Chapter 12")


## Chapter 13
Cylinders and cones. Some of the patterns don't seem to work very well in 3D 
(for example, the ring and checker patterns are very noisy in 3D).
![Final render Chapter 13](./out/chap13.png "Final render Chapter 13")


## Chapter 14
Groups.
![Final render Chapter 14](./out/chap14.png "Final render Chapter 14")


## Bounding boxes
1000 reflective spheres rendered with bounding boxes around groups. 100
groups were used (I think) to render this image in 233.74 seconds, resolution of
3840x2160 pixels (single threaded).
![Final render bounding boxes](./out/bbox.png "Final render bounding boxes")


## Bounding Volume Hierarchy
Same scene as the previous one, this time with BVH. Image rendered in 255.77
seconds. It is a bit slower compared to the previous iteration. My guess is
that I manually managed to create a better hierarchy than the automatic BVH
implementation that I built. Also, there's overhead with creating a BVH, which
might of contributed to the slowdown. However, it was much easier to build this
scene programmatically, which is good I guess.

Without BVH, this scene rendered about 7.727 times slower.
![Final render BVH](./out/bvh.png "Final render BVH")


## Soft shadows and area lights
Scene with an area light and soft shadows.
![Final render Soft Shadows](./out/soft_shadows.png "Final render Soft Shadows")
