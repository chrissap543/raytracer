#pragma once
#include "scenes/scene.h"

class ComplexScene : public Scene {
public:
    std::vector<std::shared_ptr<Hittable>> create_objects() override;
    SceneConfig get_config() override;
    const char* get_name() override;
    
private:
    Color random_color() const;
};