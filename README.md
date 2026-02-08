# Graphics Engine
This project serves as a test for my experiments into OpenGL and graphics rendering
Right now my plans for this project are quite undecided (it doesn't even have a real name after all) but I hope to try some interesting things

## Current Features
- A window initialized with an opengl context using [glfw](https://github.com/glfw/glfw) and [glad](https://github.com/Dav1dde/glad)
- DearImgui windows using [cimgui](https://github.com/cimgui/cimgui)
- The ability to create scenes, and create objects within the scene tree
- Simple component system allowing for reusable components to be loaded onto objects

## Next Steps
- Add the ability to save scenes to a file and load them again
- Add resource system for easier loading of resources such as shaders, meshes, textures, etc...
- Add simple rendering system
- Create input system for easy input handling

## Future plans
the following are things I hope to do with this engine that will require more fleshed out systems to be developed first:
- Software raytracing engine with opengl scene editor for preview
- Simple video game to showcase features
- Scripting language for components
- Simple physics engine
