/*
** Agartha-Software, 2023
** C++evy
** File description:
** Engine
*/

#include "Engine.hpp"
#include "App.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "DefaultPlugin.hpp"
#include "EntityCommands.hpp"
#include "Event.hpp"
#include "Line.hpp"
#include "Target.hpp"
#include "Transform.hpp"
#include "Velocity.hpp"
#include "ecs.hpp"
#include "imgui.h"

#include "rendering.hpp"
#include "rlImGui.h"

void init_window() {
#ifdef DEBUG
  SetTraceLogLevel(LOG_ALL);
#else
  SetTraceLogLevel(LOG_ERROR);
#endif
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 450, "raylib [core] example - basic window");
  SetTargetFPS(60);
  rlImGuiSetup(true);
}

void close_game(cevy::ecs::EventWriter<cevy::ecs::AppExit> close) {
  if (WindowShouldClose())
    close.send(cevy::ecs::AppExit{});
}

void update_window(cevy::ecs::Query<cevy::engine::Camera> cams,
                   cevy::ecs::Query<cevy::engine::Line, option<cevy::engine::Transform>,
                                    option<cevy::engine::Color>>
                       lines,
                   cevy::ecs::Query<option<cevy::engine::Transform>,
                                    cevy::engine::Handle<cevy::engine::Mesh>,
                                    option<cevy::engine::Handle<cevy::engine::Diffuse>>,
                                    option<cevy::engine::Color>>
                       models) {
  ClearBackground(WHITE);
  for (auto [cam] : cams) {
    BeginMode3D(cam);
    DrawGrid(100, 1.0f);
    render_models(models);
    render_lines(lines);
    EndMode3D();
  }
  EndDrawing();
}

void cevy::engine::Engine::build(cevy::ecs::App &app) {
  app.add_plugins(cevy::ecs::DefaultPlugin());
  app.add_stage<StartupRenderStage>();
  app.add_stage<PreStartupRenderStage>();
  app.add_stage<PostStartupRenderStage>();
  app.add_stage<RenderStage>();
  app.add_stage<PreRenderStage>();
  app.add_stage<PostRenderStage>();
  app.init_component<cevy::engine::Camera>();
  app.init_component<cevy::engine::Velocity>();
  app.init_component<cevy::engine::Target>();
  app.init_component<cevy::engine::Line>();
  app.init_component<cevy::engine::Transform>();
  app.init_component<cevy::engine::TransformVelocity>();
  app.init_component<cevy::engine::Color>();
  app.add_plugins(cevy::engine::AssetManagerPlugin());
  app.add_systems<cevy::engine::PreStartupRenderStage>(init_window);
  app.add_systems<cevy::engine::PreRenderStage>(close_game);
  app.add_systems<cevy::engine::PreRenderStage>(update_camera);
  app.add_systems<cevy::engine::RenderStage>(update_window);
  // app.add_systems<ecs::core_stage::Update>(TransformVelocity::system);
}

/*

void cevy::Engine::DebugWindow (void) {
    if (IsKeyPressed(KEY_F10)) {
        debugMode = (debugMode == false);
        if (debugMode)
            EnableCursor();
    }
    if(debugMode) {
        rlImGuiBegin();
        if(!ImGui::Begin("Test WIndow", &debugMode)) {
            ImGui::End();
        } else {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }
        rlImGuiEnd();
    } else {
        DisableCursor();
    }
}

void cevy::Engine::Fullscreen (void) {
    if (IsWindowFullscreen()) {
        int display = GetCurrentMonitor();
        this->screenWidth = GetMonitorWidth(display);
        this->screenHeight = GetMonitorHeight(display);
    } else {
        this->screenWidth = GetScreenWidth();
        this->screenHeight = GetScreenHeight();
    }
    if (!IsKeyPressed(KEY_F11))
        return;
    SetWindowSize(this->screenWidth, this->screenWidth);
        ToggleFullscreen();
}
*/
