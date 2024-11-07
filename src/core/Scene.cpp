#include "Scene.h"

#include "cmath"

DScene::DScene() {}

DScene::~DScene() {}

int DScene::Init(DInputHandler *in) {
    input = in;
    main_camera = new DCamera();
    return 0;
}

void DScene::Shutdown() {
    for(const auto& model: scene_models) {
        delete model;
    }

    for(const auto& light: point_lights) {
        delete light;
    }

    delete main_camera;
}

void DScene::LoadScene(const std::string& load_scene_name) {
    scene_name = load_scene_name;
    scene_filename = load_scene_name;

    PrintSceneInfo();
}

void DScene::LoadDefaultScene() {
    scene_name = "Default scene";
    scene_filename = "Default scene";

    //std::string mdl = "../assets/glTF/Suzanne.gltf";
    std::string mdl = "../assets/glTF2/DamagedHelmet.gltf";
	AddModel(mdl);

    DLight *light1 = new DLight();
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 pos   = glm::vec3(0.0f, 3.0f, 0.0f);
    light1->SetName("Light1");
    light1->SetColor(color);
    light1->SetPosition(pos);

    point_lights.push_back(light1);

    PrintSceneInfo();
}

void DScene::SetMainCamera(DCamera *camera) {
    main_camera = camera;
}

DCamera * DScene::GetMainCamera() {
    return main_camera;
}

void DScene::AddModel(DModel *model) {
    scene_models.push_back(model);
}

void DScene::AddModel(const std::string& model_name) {
    DModel *model = new DModel();
    model->LoadModel(model_name);
    scene_models.push_back(model);
}

void DScene::Update(float delta) {
    sceneTime += delta;

    glm::vec3 lpos = glm::vec3(2.0f * std::sin(sceneTime / 500.0f), 3.0f, 2.0f * std::cos(sceneTime / 500.0f));
    //glm::vec3 rot = glm::vec3(0.0f, 0.05f * delta, 0.0f);

    point_lights[0]->SetPosition(lpos);

    int camMoveForward = 0;
    int camMoveRight = 0;

    float mousex = (float)input->GetMouseXOffset() * delta / 10.0f;
    float mousey = (float)input->GetMouseYOffset() * delta / 10.0f;

    main_camera->ProcessMouse(mousex, mousey, true);
    
    if(input->IsKeyDown(KEY_W)) {
        camMoveForward++;
    }
    if(input->IsKeyDown(KEY_S)) {
        camMoveForward--;
    }
    if(input->IsKeyDown(KEY_D)) {
        camMoveRight++;
    }
    if(input->IsKeyDown(KEY_A)) {
        camMoveRight--;
    }
    
    main_camera->Move(camMoveForward, camMoveRight, delta / 1000.0f);
}

void DScene::PrintSceneInfo() {
    printf("Scene model count: %li\n", scene_models.size());
    for(const auto model: scene_models) {
        glm::vec3 model_pos = model->GetPosition();
        printf("| Model pos: %0.3f, %0.3f, %0.3f\n", model_pos.x, model_pos.y, model_pos.z);
    }
}