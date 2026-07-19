cmake_minimum_required(VERSION 4.2)
project(EuropaRender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "-static")

include(FetchContent)

include(FetchContent)

set(ASSIMP_BUILD_IFC_IMPORTER OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.92.8
)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})

add_library(imgui_lib STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

target_link_libraries(imgui_lib glfw)


find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(EuropaRender
        Src/Graphics/GameWindow.cpp
        Include/Graphics/GameWindow.h
        Include/Graphics/Game.h
        Include/Graphics/GameImGUI.h
        Src/Graphics/GameImGUI.cpp
        Include/Graphics/GameRenderModule.h
        Include/Graphics/ImGUIRenderModule.h
        Src/Graphics/ImGUIRenderModule.cpp
        Src/Graphics/ImGUI/ImGUIExampleModule.cpp
        Include/Graphics/ImGUI/ImGUIExampleModule.h
        Src/Graphics/GameOpenGLModule.cpp
        Include/Graphics/GameOpenGLModule.h
        Src/GameObject/GameCamera.cpp
        Include/GameObject/GameCamera.h
        Src/GameObject/GameObject.cpp
        Include/GameObject/GameObject.h
        Src/World/GameWorld.cpp
        Include/World/GameWorld.h
        Include/Graphics/Virtual/VirtualTexture.h
        Src/Graphics/Virtual/VirtualTexture.cpp
        Src/Graphics/StbGraphicsTexture.cpp
        Include/Graphics/StbGraphicsTexture.h
        Include/Vertex.h
        Src/Graphics/ImGUI/ImGUIGraphicsDebugger.cpp
        Include/Graphics/ImGUI/ImGUIGraphicsDebugger.h
        Include/logger.h
        Include/mat42str.h
)

target_compile_definitions(EuropaRender PRIVATE
        DEBUG_MODE
)

add_executable(EuropaTest main.cpp)

target_compile_options(EuropaRender PUBLIC -DEBUG_MODE)

target_precompile_headers(EuropaRender PUBLIC Include/pch.h)
target_precompile_headers(EuropaTest PUBLIC Include/pch.h)

target_include_directories(EuropaRender PUBLIC Include Include/Graphics Include/Graphics/ImGUI Include/GameObject Include/World Include/Graphics/Virtual Include/Stb Include)

target_include_directories(EuropaRender PRIVATE ${TINYGLTF_INCLUDE_DIRS})

target_link_libraries(EuropaRender PRIVATE GLEW::GLEW glfw imgui_lib glm::glm nlohmann_json::nlohmann_json)

target_link_libraries(EuropaTest EuropaRender GLEW::GLEW)