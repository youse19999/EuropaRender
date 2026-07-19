//
// Created by youse on 2026/07/18.
//

#include "GameWindow.h"

#include <iostream>

#include "Game.h"

void GameWindow::Create() {
    this->Init();
}

//モジュールを初期化
void GameWindow::InitModule() {
    for (auto module : modules) {
        module->Init();
    }
}

//ウィンドウを外部から取得できるようにする。（ポインタ）
GLFWwindow *GameWindow::GetWindow() {
    return this->window;
}

void GameWindow::Init() {
    if (!glfwInit()) {
        return;
    }
    //4.6のGLFWを起動
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //ウィンドウを作成 TODO:サイズ変更できるようにする。
    window = glfwCreateWindow(800, 600, "Window Title", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return;
    }
    //glfwのコンテキストを作成
    glfwMakeContextCurrent(window);


    //モジュールの初期化を実行
    for (auto module : modules) {
        module->Init();
    }
}

void GameWindow::PollEvent() {

}
void GameWindow::Render() {
    //レンダリングのモジュールをfor
    for (auto module : modules) {
        module->Render();
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
/*
 *これを分離しておくことで、高レイヤーの部分で楽です。
 */
bool GameWindow::ShouldCloseWindow() {
    return glfwWindowShouldClose(window);
}
/*
 *モジュールを追加します。
 */
void GameWindow::AddModule(GameRenderModule* module) {
    modules.push_back(module);
}
/*
 *ここで破棄される。
 */
GameWindow::~GameWindow() {
    //モジュールを全部破棄
    for (auto module : modules) {
        delete module;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}