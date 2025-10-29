/// Author: Benjamin Applegate
/// Defines the imgui backend to update and draw the UI
/// Significant portions of implementations came from Dear ImGui and cimgui examples, see imgui_handler.c for more information and links

#pragma once

/// Initialize the Dear ImGui Context
void imgui_init();

/// Shutdown and destroy the Dear ImGui context
void imgui_shutdown();

/// Initializes new Dear ImGui Frame
void imgui_update();

/// Renders the Dear ImGui frame
void imgui_render();