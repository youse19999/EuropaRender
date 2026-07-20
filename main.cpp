#include <iostream>



#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

//OpenGLモジュールはGameWindowよりも先に呼び出さないといけない。
#include "GameCamera.h"
#include "GameOpenGLModule.h"

#include "GameWindow.h"
#include "GameImGUI.h"
#include "GameWorld.h"
#include "ImGUIExampleModule.h"
#include "ImGUIGraphicsDebugger.h"
#include "ImGUIRenderModule.h"
#include "logger.h"
#include "StbGraphicsTexture.h"
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

//ログ機能を使う場合必須。
std::stringstream LOGGER_STREAM;

int main() {

    std::string logName = "[CORE]";

    LOG(logName<<"MAIN ENTRY");
    //ウィンドウを作成
    GameWindow* gameWindow = new GameWindow();
    //ウィンドウの作成処理を実行
    gameWindow->Create();
    LOG(logName<<"GAME WINDOW CREATED");

    //ゲーム世界
    GameWorld* world = new GameWorld(gameWindow->GetWindow());
    LOG(logName<<"GAME WORLD CREATED");
    //OpenGLのモジュール
    GameOpenGLModule* openGLModule = new GameOpenGLModule(gameWindow->GetWindow());
    LOG(logName<<"OPENGL MODULE CREATED");
    //ImGUIのメインモジュール
    GameImGUI* imgui = new GameImGUI(gameWindow->GetWindow());
    LOG(logName<<"IMGUI MODULE CREATED");

    LOG(logName<<"DBG MODULE LOADING");
    //ImGUIの仮モジュール
    ImGUIExampleModule* module = new ImGUIExampleModule();
    ImGUIGraphicsDebugger* graphicsDebuger = new ImGUIGraphicsDebugger();
    //OpenGLモジュール
    gameWindow->AddModule(openGLModule);
    gameWindow->AddModule(world);
    LOG(logName<<"CREATED");
    //imguiのモジュールを追加
    gameWindow->AddModule(imgui);
    LOG(logName<<"IMGUI MODULE ADD DONE");
    //imguiモジュールにテスト用モジュールを追加

    LOG(logName<<"ADDING IMGUI MODULE");
    imgui->AddImGUIModule(module);
    imgui->AddImGUIModule(graphicsDebuger);

    graphicsDebuger->SetGraphcis(openGLModule);
    graphicsDebuger->SetWorld(world);
    LOG(logName<<"ADDING IMGUI MODULE DONE");

    LOG(logName<<"SET TEXTURE");
    StbGraphicsTexture* texture = new StbGraphicsTexture();
    texture->SetTexture("image.png");
    texture->BindTexture();

    LOG(logName<<"SET CAMERA");
    //カメラを作成
    GameCamera* camera = new GameCamera();
    //カメラを設定
    world->SetCamera(camera);
    LOG(logName<<"ADD CAMERA TO WORLD");
    //カメラを追加
    world->AddGameObject(camera);

    std::string err;
    std::string warn;
    bool ret = false;

    tinygltf::Model model;

    tinygltf::TinyGLTF loader;
    ret = loader.LoadBinaryFromFile(&model, &err, &warn, "mesh.glb");
    if (!ret) {
        LOG( err << std::endl);
    }
    LOG("---GLTF NODES---");
    unsigned int objectSum = 0;
    //ノード回帰
    for (int i = 0;i<model.nodes.size();i++) {
        //ノードの実体の取得
        auto node = model.nodes[i];
        //子ですか？（親は子の情報を持ちません。逆です。）
        bool isChild = (node.children.size() != 0);
        //子であることｗ宣言。
        LOG("I AM " << node.name << "|NO:"<<i);
        if (isChild) {
            //親を宣言
            for (auto parent : node.children) {
                LOG(node.name<< ">BE CHILD>" << model.nodes[parent].name);
            }
        }

        //実体作成
        LOG(logName<<"SETUP OBJ TO WORLD");
        GameCamera* gameObject = new GameCamera();
        gameObject->SetTexture(texture);
        gameObject->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        gameObject->SetID(i);
        gameObject->SetName(node.name);
        tinygltf::Animation* animation = nullptr;
        tinygltf::Skin* skin = nullptr;
        tinygltf::Mesh* mesh = nullptr;
        if (node.mesh < model.meshes.size()) {
            mesh = &model.meshes[node.mesh];
        }
        if (node.skin < model.skins.size()) {
            skin = &model.skins[node.skin];
        }
        gameObject->LoadMeshes(model,mesh,skin);
        LOG(logName<<"ADD OBJ TO WORLD");
        world->AddGameObject(gameObject);
        LOG(logName<<"ADD VERTICES");
        openGLModule->AddVertices(0,gameObject->GetMeshes());
        LOG(logName<<"ADD INDICES");
        openGLModule->AddIndices(gameObject->GetIndices());
        if (isChild) {
            for (auto parent : node.children) {
                GameWorld::GetInstance().get()->GetGameObject(parent)->SetParent(gameObject);
            }
        }
        objectSum++;
    }
    GameWorld::GetInstance().get()->GetGameObject(0)->ProcessAnimation(model,&model.animations[0]);
    gameObjectLastOffset += objectSum;
    LOG("---GLTF NODES---");
    //const tinygltf::Mesh mesh

    //モジュールを初期化
    gameWindow->InitModule();
    LOG(logName<<"INIT MODULE DONE");

    //シェダープログラムをセット
    world->SetShaderProgram(openGLModule->GetShaderProgram());
    LOG(logName<<"SETTING UP SHADER PROGRAM");

    while (!gameWindow->ShouldCloseWindow()) {
        world->Update();
        gameWindow->Render();
        gameWindow->PollEvent();
    }

    delete gameWindow;

    return 0;
}
