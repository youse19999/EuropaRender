//
// Created by youse on 2026/07/18.
//

#ifndef EUROPARENDER_GAMEWINDOW_H
#define EUROPARENDER_GAMEWINDOW_H

#include <vector>
#include <GLFW/glfw3.h>

#include  <GameRenderModule.h>

#include "GameImGUI.h"
#include "ImGUIRenderModule.h"

class GameWindow {
private:
    std::vector<GameRenderModule*> modules;

    GLFWwindow* window;

public:
    ~GameWindow();
    GLFWwindow* GetWindow();

    bool ShouldCloseWindow();
    void PollEvent();
    void Init();
    void Render();

    void AddModule(GameRenderModule* module);
    void Create();
    void InitModule();
};
#endif //EUROPARENDER_GAMEWINDOW_H

