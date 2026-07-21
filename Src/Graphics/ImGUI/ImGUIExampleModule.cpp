//
// Created by youse on 2026/07/18.
//
#include <format>
#include <logger.h>
#include <ImGUIExampleModule.h>
#include <sstream>
#include <pch.h>

#include "FKMath.h"
#include "GameWorld.h"
#include "mat42str.h"

std::stringstream line;

void DrawMatrix(GameObject* obj) {
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::mat4 a = CalculateNodeModelMatrix(obj);

    glm::decompose(
        a,
        scale,
        rotation,
        translation,
        skew,
        perspective
    );

    ImGui::Text("%s",Mat42Str(a).str().c_str());

    std::stringstream trsTxt;
    trsTxt << "--------------" << "\n";
    trsTxt << "scale" << "\n";
    trsTxt << scale[0] << "," << scale[1] << "," << scale[2] << "\n";
    trsTxt << "rotation" << "\n";
    trsTxt << rotation[0] << "," << rotation[1] << "," << rotation[2] << "\n";
    trsTxt << "translation" << "\n";
    trsTxt << translation[0] << "," << translation[1] << "," << translation[2] << "\n";
    trsTxt << "skew" << "\n";
    trsTxt << skew[0] << "," << skew[1] << "," << skew[2] << "\n";
    trsTxt << "perspective" << "\n";
    trsTxt << perspective[0] << "," << perspective[1] << "," << perspective[2] << "\n";

    trsTxt << "--------------";

    ImGui::Text("%s",Mat42Str(a).str().c_str());
    ImGui::Text("%s",trsTxt.str().c_str());
}
void DrawChildren(GameObject* obj) {
    //DrawMatrix(obj);
    line << "-";
    for (auto childObj : obj->GetChildren()) {
        std::stringstream txt;
        txt << line.str() << childObj->GetName();
        ImGui::Text("%s",txt.str().c_str());
        if (childObj->GetChildren().size() != 0) {
            DrawChildren(childObj);
            //DrawMatrix(childObj);
        }
    }
}
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
            line.str("");
            line.clear();
            ImGui::Text("%s",obj->GetName().c_str());
            DrawChildren(obj);
        }
    }
    ImGui::End();
}
