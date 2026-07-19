//
// Created by youse on 2026/07/18.
//
/*
 *GLClear,glfwSwapBuffers,glfwPollEventsはこのクラスの継承で行わないでください。z
 */
#ifndef EUROPARENDER_GAMERENDERMODULE_H
#define EUROPARENDER_GAMERENDERMODULE_H
#include <pch.h>

class GameRenderModule {
protected:
    GLFWwindow* window;
public:
    virtual ~GameRenderModule() = default;
    GameRenderModule(GLFWwindow* window) {
        this->window = window;
    }

    virtual void Dispose() = 0;
    virtual void PollEvent() = 0;
    virtual void Init() = 0;
    virtual void Render() = 0;
};
#endif //EUROPARENDER_GAMERENDERMODULE_H
