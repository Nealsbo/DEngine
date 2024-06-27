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

}

void DScene::LoadScene(const std::string& scene_name) {

}

void DScene::SetCamera(DCamera &camera) {

}

DCamera * DScene::GetCamera() {
    return main_camera;
}

void DScene::AddModel(DModel &model) {

}

void DScene::Update(float delta) {

}