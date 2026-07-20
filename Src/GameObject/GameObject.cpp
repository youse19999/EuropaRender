//
// Created by youse on 2026/07/19.
//
unsigned int lastIndexPosition;

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include <GameObject.h>
#include <iostream>

#include "logger.h"

using namespace tinygltf;

nlohmann::json GameObject::GetJson() {
    return nlohmann::json();
}

std::vector<Vertex> GameObject::GetMeshes() {
    return vertices;
}
std::vector<unsigned int> GameObject::GetIndices() {
    return this->indices;
}

VirtualTexture* GameObject::GetTexture() {
    return texture;
}

bool GameObject::GetTextureHasLoaded() {
    return textureHasLoaded;
}

void GameObject::SetTexture(VirtualTexture* texture) {
    this->texture = texture;
    textureHasLoaded = true;
}

glm::mat4 GameObject::GetModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, scale);
    return model;
}

void GameObject::LoadMeshes(std::string path) {
    std::string err;
    std::string warn;
    bool ret = false;
    tinygltf::Model model;

    tinygltf::TinyGLTF loader;
    ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
    if (!ret) {
        LOG( err << std::endl);
    }

    std::vector<int> indices;
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    /*
     *AIが記述　TODO: ここをassimpにAIに置き換えさせる。
     */
    // 1. モデル内のすべてのメッシュをループ
    for (const auto& mesh : model.meshes) {
        // 2. メッシュ内のプリミティブ（サブメッシュ）をループ
        for (const auto& primitive : mesh.primitives) {
            LOG(mesh.primitives.size() << std::endl);
            Vertex vertex;
            // --- 頂点座標 (POSITION) の取得 ---
            auto posIt = primitive.attributes.find("POSITION");
            if (posIt != primitive.attributes.end()) {
                // アクセッサー、バッファビュー、バッファの取得
                const tinygltf::Accessor& accessor = model.accessors[posIt->second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                // データの開始位置（ポインタ）を計算
                // bufferView.byteOffset と accessor.byteOffset の両方を足す必要があります
                const float* positions = reinterpret_cast<const float*>(
                    &buffer.data[bufferView.byteOffset + accessor.byteOffset]
                );
                // データの読み出し (POSITIONは通常 float が3つで1頂点)
                for (size_t i = 0; i < accessor.count; ++i) {
                    float x = positions[i * 3 + 0];
                    float y = positions[i * 3 + 1];
                    float z = positions[i * 3 + 2];
                    // 必要に応じて自前の頂点配列などに格納する
                    position.push_back(glm::vec3(x,y,z));
                }
            }

            // --- 法線 (NORMAL) の取得 ---
            auto normIt = primitive.attributes.find("NORMAL");
            if (normIt != primitive.attributes.end()) {
                // アクセッサー、バッファビュー、バッファの取得
                const tinygltf::Accessor& accessor = model.accessors[normIt->second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                // データの開始位置（ポインタ）を計算
                const float* _normals = reinterpret_cast<const float*>(
                    &buffer.data[bufferView.byteOffset + accessor.byteOffset]
                );

                // データの読み出し (NORMALは通常 float が3つで1頂点)
                for (size_t i = 0; i < accessor.count; ++i) {
                    float nx = _normals[i * 3 + 0];
                    float ny = _normals[i * 3 + 1];
                    float nz = _normals[i * 3 + 2];

                    // 既存のvertices配列の各頂点データに追加する場合の例
                    // ※POSITIONのループと同じインデックスで同期させるか、
                    //  以下のように対象オブジェクトにプロパティを追加します
                    if (i < vertices.size()) {
                        vertex.Normal.x = nx;
                        vertex.Normal.y = ny;
                        vertex.Normal.z = nz;
                    }
                    normals.push_back(glm::vec3(nx,ny,nx));
                }
            }

            // --- インデックス (Indices) の取得 ---
            if (primitive.indices >= 0) {
                const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                const unsigned char* dataPtr = &buffer.data[bufferView.byteOffset + accessor.byteOffset];

                // インデックスは型が複数（型サイズが1, 2, 4バイト）あるため分岐処理が必要
                for (size_t i = 0; i < accessor.count; ++i) {
                    uint32_t index = 0;
                    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                        index = reinterpret_cast<const uint32_t*>(dataPtr)[i];
                    } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                        index = reinterpret_cast<const uint16_t*>(dataPtr)[i];
                    } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                        index = reinterpret_cast<const uint8_t*>(dataPtr)[i];
                    }
                    // 得られたインデックスの処理
                    indices.push_back(index+lastIndexPosition);
                }
            }
            int accessorIndex = posIt->second;
            const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            if (accessor.type != TINYGLTF_TYPE_VEC2) {
                const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
                size_t byteStride = accessor.ByteStride(bufferView);
                nlohmann::json uvJson;
                // 3. データの読み出し
                for (size_t i = 0; i < accessor.count; ++i) {
                    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                        const float* uv = reinterpret_cast<const float*>(dataPtr + (i * byteStride));
                        float u = uv[0];
                        float v = uv[1];
                        uvs.push_back(glm::vec2(v,u));
                    }
                    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                        // 正規化された（Normalized）値として格納されているケース
                        const unsigned char* uv = reinterpret_cast<const unsigned char*>(dataPtr + (i * byteStride));
                        float u = accessor.normalized ? (uv[0] / 255.0f) : static_cast<float>(uv[0]);
                        float v = accessor.normalized ? (uv[1] / 255.0f) : static_cast<float>(uv[1]);
                        vertex.UV.x = u;
                        vertex.UV.y = v;
                        uvs.push_back(glm::vec2(v,u));
                    }
                    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                        const unsigned short* uv = reinterpret_cast<const unsigned short*>(dataPtr + (i * byteStride));
                        float u = accessor.normalized ? (uv[0] / 65535.0f) : static_cast<float>(uv[0]);
                        float v = accessor.normalized ? (uv[1] / 65535.0f) : static_cast<float>(uv[1]);
                        vertex.UV.x = u;
                        vertex.UV.y = v;
                        uvs.push_back(glm::vec2(v,u));
                    }
                }
            }
        }
        LOG("---INPUT RESULT---\n" << "POSITION SIZE:" << position.size() << "\nNORMAL SIZE" << normals.size() <<  "\nUV SIZES" << uvs.size() << "\nLastIndexPosition:" << lastIndexPosition << "\n---INPUT RESULT---" << std::endl);
        for (int i = 0;i<position.size();i++) {
            Vertex vertex;
            vertex.Position = position[i];
            vertex.Normal = normals[i];
            vertex.UV = uvs[i];
            vertices.push_back(vertex);
        }
        this->indices.insert(this->indices.end(),indices.begin(),indices.end());

        lastIndexPosition+=this->vertices.size();
    }
}

GameObject::~GameObject() {

}