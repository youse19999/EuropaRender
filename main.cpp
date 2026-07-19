#include <iostream>


//OpenGLモジュールはGameWindowよりも先に呼び出さないといけない。
#include "GameCamera.h"
#include "GameOpenGLModule.h"

#include "GameWindow.h"
#include "GameImGUI.h"
#include "GameWorld.h"
#include "ImGUIExampleModule.h"
#include "ImGUIRenderModule.h"
/*
 *順番 これを守らないと壊れます。
 *ゲームウィンドウ作成
 *ゲーム世界作成
 *OpenGLモジュールの作成
 *ImGUIのモジュールを作成
 *ImGUIの拡張Moduleを作成
 *OpenGLモジュールを追加
 *GameWindowを追加
 *ImGUIモジュールを追加
 *ワールドにカメラを設定
 *ワールドにゲームオブジェクトを追加
 *ゲームオブジェクトを追加
 *シェダープログラムをセット
 */
int main() {
    //ウィンドウを作成
    GameWindow* gameWindow = new GameWindow();
    //ウィンドウの作成処理を実行
    gameWindow->Create();

    //ゲーム世界
    GameWorld* world = new GameWorld(gameWindow->GetWindow());
    //OpenGLのモジュール
    GameOpenGLModule* openGLModule = new GameOpenGLModule(gameWindow->GetWindow());
    //ImGUIのメインモジュール
    GameImGUI* imgui = new GameImGUI(gameWindow->GetWindow());

    //ImGUIの仮モジュール
    ImGUIExampleModule* module = new ImGUIExampleModule();
    //OpenGLモジュール
    gameWindow->AddModule(openGLModule);
    gameWindow->AddModule(world);
    //imguiのモジュールを追加
    gameWindow->AddModule(imgui);
    //imguiモジュールにテスト用モジュールを追加
    imgui->AddImGUIModule(module);

    //カメラを作成
    GameCamera* camera = new GameCamera();
    //カメラを設定
    world->SetCamera(camera);
    //カメラを追加
    world->AddGameObject(camera);

    GameCamera* gameObject = new GameCamera();

    openGLModule->AddVertices(0,gameObject->GetMeshes());

    //モジュールを初期化
    gameWindow->InitModule();

    //シェダープログラムをセット
    world->SetShaderProgram(openGLModule->GetShaderProgram());

    while (!gameWindow->ShouldCloseWindow()) {
        world->Update();
        gameWindow->Render();
        gameWindow->PollEvent();
    }

    delete gameWindow;

    return 0;
}
