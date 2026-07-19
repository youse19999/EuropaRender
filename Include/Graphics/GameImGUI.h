//
// Created by youse on 2026/07/18.
//

#ifndef EUROPARENDER_GAMEIMGUI_H
#define EUROPARENDER_GAMEIMGUI_H
#include "GameRenderModule.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

#include "ImGUIRenderModule.h"

class GameImGUI:public GameRenderModule {
private:
    std::vector<ImGUIRenderModule*> imguiModules;
public:
    GameImGUI(GLFWwindow* window);
    void Dispose() override;
    void PollEvent() override;
    void Init() override;
    void Render() override;
    void AddImGUIModule(ImGUIRenderModule* module);
};


#endif //EUROPARENDER_GAMEIMGUI_H