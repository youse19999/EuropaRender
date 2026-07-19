//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_IMGUIGRAPHICSDEBUGGER_H
#define EUROPARENDER_IMGUIGRAPHICSDEBUGGER_H
#include "GameWorld.h"
#include "ImGUIRenderModule.h"

class ImGUIGraphicsDebugger:public ImGUIRenderModule {
public:
    ImGUIGraphicsDebugger();
    void RenderGUI() override;
    void SetWorld(GameWorld* world);
    void SetGraphcis(GameOpenGLModule* module);
private:
    GameWorld* world;
    GameOpenGLModule* module;
};
#endif //EUROPARENDER_IMGUIGRAPHICSDEBUGGER_H
