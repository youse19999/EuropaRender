//
// Created by youse on 2026/07/18.
//

#ifndef EUROPARENDER_IMGUIEXAMPLEMODULE_H
#define EUROPARENDER_IMGUIEXAMPLEMODULE_H
#include "ImGUIRenderModule.h"
#include <imgui.h>

class ImGUIExampleModule:public ImGUIRenderModule {
public:
    void RenderGUI() override;
};
#endif //EUROPARENDER_IMGUIEXAMPLEMODULE_H
