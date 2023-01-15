# Groei - A Graphics Engine in C

This is a simpe graphics engine that uses [GLFW](https://www.glfw.org/) for window management and [Vulkan](https://www.vulkan.org/) for rendering to that window. Potentially I'll add [OpenGL](https://www.opengl.org/) support too.

This is a learning exercise in bringing together the maths to render models onto a screen.

It may turn into something more, but for now, it's just that.

## You want to run it?

If you are kinda adventurous, and would like to see it in action, you'll need the following:

* [GLFW](https://www.glfw.org/)
* [Vulkan](https://www.vulkan.org/)
* [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/)
* [GCC](https://gcc.gnu.org/) - though Clang or MSVC should be fine but you'd need to update the `build` script.
* [CGLM](https://github.com/recp/cglm) - `cglm` because I'm using Linux, but GLM would be fine on another OS.
* [OpenGL](https://www.opengl.org/)

### NOTE

I'm building this on a Linux machine with a Ubuntu-like distro (I don't use Arch, by the way), your setup may be quite different. For starters, you may want to swap `build` for a `build.bat`. _I'm not The Batman_.
