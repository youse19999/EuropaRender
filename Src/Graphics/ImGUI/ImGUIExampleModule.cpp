//
// Created by youse on 2026/07/18.
//
#include <format>
#include <logger.h>
#include <ImGUIExampleModule.h>
#include <sstream>

#include "GameWorld.h"

void ImGUIExampleModule::RenderGUI() {
    ImGui::Begin("LOG");
    ImGui::Text("%s",LOGGER_STREAM.str().c_str());
    ImGui::Text("%s",LOGGER_RENDER.str().c_str());
    ImGui::End();
    ImGui::Begin("WORLD");
    for (auto obj : GameWorld::GetInstance().get()->GetAllGameObject()) {
        std::stringstream ssName;
        ssName << obj->GetID() << obj->GetName();
        if (obj->GetParent() == nullptr) {
            if (ImGui::TreeNode(ssName.str().c_str())) {
                //ImGui::SliderFloat("%s_x",&obj->GetPosition().x,-10,10,ssName.str().c_str());
                //ImGui::SliderFloat("%s_y",&obj->GetPosition().y,-10,10,ssName.str().c_str());
                //ImGui::SliderFloat("%s_z",&obj->GetPosition().z,-10,10,ssName.str().c_str());
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}
