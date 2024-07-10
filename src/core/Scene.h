#ifndef _SCENE_
#define _SCENE_

#include <string>
#include <vector>

#include "Camera.h"
#include "../renderer/Model.h"

class DScene {
public:
    DScene();
    ~DScene();

    int Init();
    void Shutdown();

    void LoadScene(const std::string& scene_name);

    void SetCamera(DCamera &camera);
    DCamera * GetCamera();
    
    void AddModel(DModel *model);
    void AddModel(const std::string& model_name);

    void Update(float delta);

    void PrintSceneInfo();

    DCamera *main_camera;
    std::vector<DModel *> scene_models;
    //std::vector<DParticles> scene_particels
    //std::vector<DVolume> scene_volumes;

	//DDirectLight *direct_light;
	//std::vector<DSpotLight> spot_lights
	//std::vector<DPointLight> point_lights
};

#endif //_SCENE_