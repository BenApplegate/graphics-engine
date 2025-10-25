target_sources(graphics_engine PRIVATE engine.c engine.h mesh.c mesh.h shader.c shader.h imgui_handler.c imgui_handler.h
scene.c scene.h object.c object.h component.c component.h)

add_subdirectory(ui)