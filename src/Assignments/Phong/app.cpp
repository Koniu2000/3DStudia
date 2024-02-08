//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include "Engine/PhongMaterial.h"
#include "Application/utils.h"
#include "Engine/mesh_loader.h"


struct Lightstruct {
    xe::PointLight lights[24];
    glm::vec3 ambient;
    int n_p_lights;
};

void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle"

    xe::ColorMaterial::init();
    xe::PhongMaterial::init();

    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj",
                                                std::string(ROOT_DIR) + "/Models");

    add_submesh(pyramid);

    //uniforms_transformations
    glGenBuffers(1, &UT_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, UT_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, UT_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    float aspect_ = (float)w / h;
    float fov_ = glm::pi<float>()/4.0;
    float near_ = 0.1f;
    float far_ = 100.0f;

    set_camera(new Camera);
    camera_->look_at(glm::vec3(2.0f, 3.0f, 4.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));

    camera_->perspective(fov_, aspect_, near_, far_);

    set_controler(new CameraControler(camera()));

    add_light(xe::PointLight(glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0), 1.0));

    ambient_ = glm::vec3 (0.0, 0.5, 0.0);
    add_ambient(ambient_);

    glGenBuffers(1, &light_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Lightstruct), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, light_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);
    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glm::mat4 M(1.0f);
    auto PVM = camera_->projection() * camera_->view() * M;
    auto VM = camera_->view();
    auto R = glm::mat3(VM);
    auto N = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));

    glBindBuffer(GL_UNIFORM_BUFFER, UT_buffer_handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, light_buffer);

    Lightstruct light_data{};

    for (int i = 0; i < p_lights_.size(); ++i)
    {
        light_data.lights[i] = p_lights_[i];
    }

    light_data.n_p_lights = p_lights_.size();
    light_data.ambient = ambient_;

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Lightstruct), &light_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for (auto m: meshes_)
        m->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera_->set_aspect((float) w / h);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera_->zoom(yoffset / 30.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}