//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "camera_controler.h"
#include "Engine/Mesh.h"
#include "Engine/Light.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug){}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void set_controler(CameraControler *controler) { controler_ = controler; }

    void scroll_callback(double xoffset, double yoffset) override;

    void mouse_button_callback(int button, int action, int mods);

    void cursor_position_callback(double x, double y);

    void add_submesh(xe::Mesh *mesh) {
        meshes_.push_back(mesh);
    }

    void add_light(const xe::PointLight &p_light) {
        p_lights_.push_back(p_light);
    }

    void add_ambient(glm::vec3 ambient) {
        ambient_ = ambient;
    }

private:
    GLuint vao_;
    Camera *camera_;
    CameraControler *controler_;
    GLuint UT_buffer_handle;
    GLuint light_buffer;
    std::vector<xe::Mesh*> meshes_;

    glm::vec3 ambient_;
    std::vector<xe::PointLight> p_lights_;
};