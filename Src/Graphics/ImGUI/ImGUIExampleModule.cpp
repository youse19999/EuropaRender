//
// Created by youse on 2026/07/18.
//
#include <logger.h>
#include <ImGUIExampleModule.h>
#include <sstream>

void ImGUIExampleModule::RenderGUI() {
    ImGui::Begin("LOG");
    ImGui::Text("%s",LOGGER_STREAM.str().c_str());
    ImGui::Text("%s",LOGGER_RENDER.str().c_str());
    ImGui::End();
}
