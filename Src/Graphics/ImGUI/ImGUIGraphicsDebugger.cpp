//
// Created by youse on 2026/07/19.
//

#include "ImGUIGraphicsDebugger.h"

#include <imgui.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "logger.h"
#include "mat42str.h"

void ImGUIGraphicsDebugger::RenderGUI() {
    if (this->module == nullptr) {
        LOG("GL module is missing");
        return;
    }
    if (this->world == nullptr) {
        LOG("World is missing");
        return;
    }
    //GFXの表示
    ImGui::Begin("GFX DEBUG");
    std::stringstream gfxPointerText;
    //ポインタのポインタを表示
    gfxPointerText << "Ah...GFX Pointer at->" << (&world);
    ImGui::Text(gfxPointerText.str().c_str());
    //カメラ関連の情報を出す。
    std::stringstream gfxCameraText;
    gfxCameraText << "Camera;" << (&world->GetCamera()) << "\n\nViewMatrix:\n";
    gfxCameraText << Mat42Str(world->GetCamera().GetViewMatrix()).str() << "\n\nProjectionMatrix\n";
    gfxCameraText << Mat42Str(world->GetCamera().GetProjectionMatrix()).str() << "\n\n";
    ImGui::Text(gfxCameraText.str().c_str());
    ImGui::SliderFloat("px",&world->GetCamera().GetPosition().x,-10.0f,10.0f);
    ImGui::SliderFloat("py",&world->GetCamera().GetPosition().y,-10.0f,10.0f);
    ImGui::SliderFloat("pz",&world->GetCamera().GetPosition().z,-10.0f,10.0f);
    ImGui::End();
    ImGui::Begin("SHADER DEBUG");
    //シェダーのエラーを取得
    GLint logLength;
    glGetShaderiv(this->module->GetShaderProgram(), GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> infoLog(logLength);
    glGetShaderInfoLog(this->module->GetShaderProgram(), logLength, NULL, &infoLog[0]);
    std::stringstream shaderStrBuff;
    shaderStrBuff << "Shader info:\n" << &infoLog[0] << "\n\n";
    ImGui::Text(shaderStrBuff.str().c_str());
    ImGui::End();
}
ImGUIGraphicsDebugger::ImGUIGraphicsDebugger() {

}
void ImGUIGraphicsDebugger::SetGraphcis(GameOpenGLModule* module) {
    this->module = module;
}

void ImGUIGraphicsDebugger::SetWorld(GameWorld *world) {
    this->world = world;
}
