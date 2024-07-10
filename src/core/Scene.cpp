#include "Scene.h"

DScene::DScene() {
    main_camera = new DCamera();
}

DScene::~DScene() {
    delete main_camera;
}

int DScene::Init() {
    return 0;
}

void DScene::Shutdown() {
    for(const auto& model: scene_models) {
        delete model;
    }
}

void DScene::LoadScene(const std::string& scene_name) {

}

void DScene::SetCamera(DCamera &camera) {

}

DCamera * DScene::GetCamera() {
    return main_camera;
}

void DScene::AddModel(DModel *model) {
    scene_models.push_back(model);
    PrintSceneInfo();
}

void DScene::AddModel(const std::string& model_name) {
    DModel *model = new DModel();
    model->LoadModel(model_name);
    DShader *shader = new DShader("../assets/shaders/base.vs", "../assets/shaders/base.fs");
    model->SetShader(shader);
    scene_models.push_back(model);
    PrintSceneInfo();
}

void DScene::Update(float delta) {

}

void DScene::PrintSceneInfo() {
    printf("Scene model count: %i\n", scene_models.size());
}