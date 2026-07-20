/*
 *
 */
#include <glm/gtc/type_ptr.hpp>

#include "Game.h"
#include "GameWorld.h"
#include "mat42str.h"
//
// Created by youse on 2026/07/19.
//
unsigned int lastIndexPosition;
unsigned int gameObjectLastOffset;

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

void GameObject::SetParent(GameObject *obj) {
    this->parent = obj;
}

void GameObject::LoadMeshes(tinygltf::Model model,tinygltf::Mesh* mesh,tinygltf::Skin* skin) {
    std::vector<int> indices;
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec4> joints;
    std::vector<glm::vec4> weights;
    /*
    +----------------------------------------+---------------+
    | TINYGLTF_COMPONENT_TYPE                | C++ Data Type |
    +----------------------------------------+---------------+
    | TINYGLTF_COMPONENT_TYPE_BYTE           | int8_t        |
    | TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE  | uint8_t       |
    | TINYGLTF_COMPONENT_TYPE_SHORT          | int16_t       |
    | TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT | uint16_t      |
    | TINYGLTF_COMPONENT_TYPE_INT            | int32_t       |
    | TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT   | uint32_t      |
    | TINYGLTF_COMPONENT_TYPE_FLOAT          | float         |
    | TINYGLTF_COMPONENT_TYPE_DOUBLE         | double        |
    +----------------------------------------+---------------+
     */
    if (mesh != nullptr) {
        // 2. メッシュ内のプリミティブ（サブメッシュ）をループ
        for (const auto& primitive : mesh->primitives) {
            LOG(mesh->primitives.size() << std::endl);
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
            auto uvIt = primitive.attributes.find("TEXCOORD_0");
            int accessorIndex = uvIt->second;
            const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            if (accessor.type == TINYGLTF_TYPE_VEC2) {
                const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
                size_t byteStride = accessor.ByteStride(bufferView);
                nlohmann::json uvJson;
                // 3. データの読み出し
                for (size_t i = 0; i < accessor.count; ++i) {
                    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                        const float* uv = reinterpret_cast<const float*>(dataPtr + (i * byteStride));
                        float u = uv[0];
                        float v = uv[1];
                        uvs.push_back(glm::vec2(u,v));
                    }
                    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                        // 正規化された（Normalized）値として格納されているケース
                        const unsigned char* uv = reinterpret_cast<const unsigned char*>(dataPtr + (i * byteStride));
                        float u = accessor.normalized ? (uv[0] / 255.0f) : static_cast<float>(uv[0]);
                        float v = accessor.normalized ? (uv[1] / 255.0f) : static_cast<float>(uv[1]);
                        vertex.UV.x = u;
                        vertex.UV.y = v;
                        uvs.push_back(glm::vec2(u,v));
                    }
                    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                        const unsigned short* uv = reinterpret_cast<const unsigned short*>(dataPtr + (i * byteStride));
                        float u = accessor.normalized ? (uv[0] / 65535.0f) : static_cast<float>(uv[0]);
                        float v = accessor.normalized ? (uv[1] / 65535.0f) : static_cast<float>(uv[1]);
                        vertex.UV.x = u;
                        vertex.UV.y = v;
                        uvs.push_back(glm::vec2(u,v));
                    }
                }
            }
            LOG("---START ABOUT BONE AND WEIGHTS START---\n");
            //最大4つまで。
            for (int n = 0;n<1;n++) {
                /*
                 *対応するPRIMITIVEのJOINTSとWEIGHTを受け取る。
                 */
                LOG(">PRIMITIVE" << &primitive);
                std::stringstream tag;
                tag << "JOINTS_";
                tag << n;
                LOG(">>" << tag.str() << "\n");
                auto jointsIt = primitive.attributes.find(tag.str());
                if (jointsIt != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[jointsIt->second];
                    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                    const uint8_t* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

                    size_t componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType);
                    size_t typeCount = tinygltf::GetNumComponentsInType(accessor.type); // VEC4なら 4
                    size_t defaultStride = componentSize * typeCount;

                    size_t byteStride = accessor.ByteStride(bufferView);
                    size_t stride = (byteStride > 0) ? byteStride : defaultStride;

                    for (size_t i = 0; i < accessor.count; ++i) {
                        const uint8_t* elementPtr = dataPtr + (i * stride);
                        std::stringstream jointSSteam;
                        // 型に応じたキャストが必要 (VEC4 として扱う)
                        // 例: uint16_t[4] または uint8_t[4]
                        //uint16_t
                        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                            const uint16_t* joint = reinterpret_cast<const uint16_t*>(elementPtr);
                            glm::vec4 jointData = glm::vec4(joint[0], joint[1], joint[2], joint[3]);
                            joints.push_back(jointData);
                        }
                        //uint8_t
                        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                            const uint8_t* joint = elementPtr;
                            glm::vec4 jointData = glm::vec4(joint[0], joint[1], joint[2], joint[3]);
                            joints.push_back(jointData);
                        }
                    }
                }
                std::stringstream tag2;
                tag2 << "WEIGHTS_";
                tag2 << n;
                LOG(">>" << tag2.str() << "\n");
                auto weightsIt = primitive.attributes.find(tag2.str());
                if (weightsIt != primitive.attributes.end()) {
                    const tinygltf::Accessor& accessor = model.accessors[weightsIt->second];
                    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                    const float* weightsData = reinterpret_cast<const float*>(
                        buffer.data.data() + bufferView.byteOffset + accessor.byteOffset
                    );

                    for (size_t i = 0; i < accessor.count; ++i) {
                        const float* weightVec4 = weightsData + (i * 4);
                        const glm::vec4 weightVec4Data = glm::vec4(weightVec4[0],weightVec4[1],weightVec4[2],weightVec4[3]);
                        weights.push_back(weightVec4Data);
                    }
                }
                LOG("---END ABOUT BONE AND WEIGHTS END---\n");
            }
        }
        LOG("---INPUT RESULT---\n" << "POSITION SIZE:" << position.size() << "\nNORMAL SIZE" << normals.size() <<  "\nUV SIZES" << uvs.size() << "\nLastIndexPosition:" << lastIndexPosition << "\n---INPUT RESULT---" << std::endl);
        for (int i = 0;i<position.size();i++) {
            Vertex vertex;
            vertex.Position = position[i];
            vertex.Normal = normals[i];
            if (joints.size() != 0) {
                vertex.Joint = joints[i];
            }
            if (weights.size() != 0) {
                vertex.Weight = weights[i];
            }
            vertex.UV = uvs[i];
            vertices.push_back(vertex);
        }
        this->indices.insert(this->indices.end(),indices.begin(),indices.end());

        lastIndexPosition+=this->vertices.size();
    }
    if (skin != nullptr) {
        LOG("---SKIN---");
        if (skin != nullptr) {
            for (auto index : skin->joints) {
                jointsNode.push_back(model.nodes[index]);
                jointsNodeIndices.push_back(index);
            }
            if (skin->inverseBindMatrices >= 0) {
                const tinygltf::Accessor& accessor = model.accessors[skin->inverseBindMatrices];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                const size_t byteStride = accessor.ByteStride(bufferView) ? accessor.ByteStride(bufferView) : sizeof(glm::mat4);

                const uint8_t* basePtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

                //glm::mat4に直接入れる。
                for (size_t i = 0; i < accessor.count; ++i) {
                    const float* matPtr = reinterpret_cast<const float*>(basePtr + (i * byteStride));
                    glm::mat4 matrixVec4Data = glm::make_mat4(matPtr);
                    jointMatrix.push_back(matrixVec4Data);
                }
            }
        }
    }
}
void GameObject::ProcessAnimation(tinygltf::Model model,tinygltf::Animation* animation) {
    //TODO:毎回呼び出されるため分離しないろいけない。
    if (animation != nullptr) {
        for (const auto& channel : animation->channels) {
            int targetNode = channel.target_node;
            const std::string& path = channel.target_path;

            const auto& sampler = animation->samplers[channel.sampler];
            const std::string& interpolation = sampler.interpolation;

            // --- Input (Time) Accessor ---
            const tinygltf::Accessor& inputAccessor = model.accessors[sampler.input];
            const tinygltf::BufferView& inputBufferView = model.bufferViews[inputAccessor.bufferView];
            const tinygltf::Buffer& inputBuffer = model.buffers[inputBufferView.buffer];

            const unsigned char* inputDataPtr = &inputBuffer.data[inputBufferView.byteOffset + inputAccessor.byteOffset];
            size_t inputStride = inputAccessor.ByteStride(inputBufferView);

            // --- Output (Value) Accessor ---
            const tinygltf::Accessor& outputAccessor = model.accessors[sampler.output];
            const tinygltf::BufferView& outputBufferView = model.bufferViews[outputAccessor.bufferView];
            const tinygltf::Buffer& outputBuffer = model.buffers[outputBufferView.buffer];

            const unsigned char* outputDataPtr = &outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset];
            size_t outputStride = outputAccessor.ByteStride(outputBufferView);

            GameObject* obj = GameWorld::GetInstance().get()->GetGameObject(targetNode);
            if (obj == nullptr) {
                continue;
            }

            // 例：各キーフレームのデータを安全に取得する処理
            for (size_t i = 0; i < inputAccessor.count; ++i) {
                // ストライドを考慮してTimeを取得
                const float time = *reinterpret_cast<const float*>(inputDataPtr + i * inputStride);

                // ストライドを考慮してValueの先頭ポインタを取得
                const float* valPtr = reinterpret_cast<const float*>(outputDataPtr + i * outputStride);

                // CUBICSPLINE補間の場合はインデックスのオフセット（inTangentのスキップ）が必要
                size_t valueOffset = 0;
                if (interpolation == "CUBICSPLINE") {
                    // 要素数 (translation/scale: 3, rotation: 4) 分の inTangent をスキップ
                    size_t numComponents = (path == "rotation") ? 4 : 3;
                    valueOffset = numComponents;
                }

                if (path == "translation") {
                    const float x = valPtr[valueOffset + 0];
                    const float y = valPtr[valueOffset + 1];
                    const float z = valPtr[valueOffset + 2];

                    // NOTE: アニメーション処理としては、ここで直接SetPositionするのではなく
                    // 時間(time)に応じた補間処理を行うか、クリップデータとして保持するのが一般的です。
                    obj->SetPosition(glm::vec3(x, y, z));
                }
                else if (path == "scale") {
                    const float x = valPtr[valueOffset + 0];
                    const float y = valPtr[valueOffset + 1];
                    const float z = valPtr[valueOffset + 2];
                    obj->SetScale(glm::vec3(x, y, z));
                }
                else if (path == "rotation") {
                    const float x = valPtr[valueOffset + 0];
                    const float y = valPtr[valueOffset + 1];
                    const float z = valPtr[valueOffset + 2];
                    const float w = valPtr[valueOffset + 3];
                    obj->SetRotation(glm::quat(w, x, y, z)); // ※使用する数学ライブラリのQuat引数順序(w,x,y,z か x,y,z,w か)に依存
                }
            }
        }
    }
}

std::vector<glm::mat4> GameObject::GetJointMatrix() {
    return jointMatrix;
}

std::vector<tinygltf::Node> GameObject::GetJoints() {
    return jointsNode;
}
std::vector<unsigned int> GameObject::GetJointsIndices() {
    return jointsNodeIndices;
}
void GameObject::SetPosition(glm::vec3 _position) {
    this->position = _position;
}
glm::vec3& GameObject::GetPosition() {
    return this->position;
}
void GameObject::SetRotation(glm::quat _rotation) {
    this->rotation = _rotation;
}
glm::quat& GameObject::GetRotation() {
    return this->rotation;
}
void GameObject::SetScale(glm::vec3 _scale) {
    this->scale = _scale;
}
glm::vec3& GameObject::GetScale() {
    return this->scale;
}

void GameObject::SetID(int _id) {
    this->id = gameObjectLastOffset + _id;
}

int GameObject::GetID() {
    return this->id;
}

std::string GameObject::GetName() {
    return this->name;
}

void GameObject::SetName(std::string _name) {
    this->name = _name;
}

GameObject *GameObject::GetParent() {
    return this->parent;
}

GameObject::GameObject() {

}
GameObject::~GameObject() {

}
