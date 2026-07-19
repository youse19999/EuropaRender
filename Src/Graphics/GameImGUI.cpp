//
// Created by youse on 2026/07/18.
//

#include <GameImGUI.h>
#include <iostream>

#include "ImGUIRenderModule.h"

void GameImGUI::Dispose() {
    if (window == nullptr) {
        std::cerr << "Have you set up the window?" << std::endl;
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

GameImGUI::GameImGUI(GLFWwindow* window) : GameRenderModule(window) {
}

void GameImGUI::Init() {
    if (window == nullptr) {
        std::cerr << "Have you set up the window?" << std::endl;
        return;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void GameImGUI::PollEvent() {
}

void GameImGUI::Render() {
    if (window == nullptr) {
        std::cerr << "Have you set up the window?" << std::endl;
        return;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto module : imguiModules) {
        module->RenderGUI();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GameImGUI::AddImGUIModule(ImGUIRenderModule* module) {
    imguiModules.push_back(module);
}
