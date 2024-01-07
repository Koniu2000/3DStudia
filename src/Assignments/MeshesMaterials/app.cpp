//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Application/utils.h"


void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".

    xe::ColorMaterial::init();

    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            0.5f, 0.0f, 0.5f, // pierwsza podstawa
            -0.5f, 0.0f, -0.5f,
            0.5f, 0.0f, -0.5f,
            0.5f, 0.0f, 0.5f, // druga podstawa
            -0.5f, 0.0f, 0.5f,
            -0.5f, 0.0f, -0.5f,
            0.0f, 1.0f, 0.0f,  // sciana czerowna
            0.5f, 0.0f, 0.5f,
            0.5f, 0.0f, -0.5f,
            0.0f, 1.0f, 0.0f,  // sciana niebieska
            0.5f, 0.0f, -0.5f,
            -0.5f, 0.0f, -0.5f,
            0.0f, 1.0f, 0.0f, // sciana turkusowa
            -0.5f, 0.0f, -0.5f,
            -0.5f, 0.0f, 0.5f,
            0.0f, 1.0f, 0.0f,  // sciana fioletowa
            -0.5f, 0.0f, 0.5f,
            0.5f, 0.0f, 0.5f,
            };

    std::vector<GLushort> indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

    //MeshMaterial
    auto pyramid = new xe::Mesh;

    pyramid->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);

    pyramid->allocate_index_buffer(indices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_indices(0, indices.size() * sizeof(GLfloat), indices.data());

    pyramid->add_submesh(0, 6, new xe::ColorMaterial({1.0f, 1.0f, 0.0f, 1.0f}));
    pyramid->add_submesh(6, 9, new xe::ColorMaterial({0.0f, 1.0f, 0.0f, 1.0f}));
    pyramid->add_submesh(9, 12, new xe::ColorMaterial({1.0f, 0.0f, 0.0f, 1.0f}));
    pyramid->add_submesh(12, 15, new xe::ColorMaterial({0.0f, 0.0f, 1.0f, 1.0f}));
    pyramid->add_submesh(15, 18, new xe::ColorMaterial({0.0f, 1.0f, 1.0f, 1.0f}));
    add_submesh(pyramid);

    //uniforms_modifier
    GLuint UM_buffer_handle;
    glGenBuffers(1, &UM_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, UM_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UM_buffer_handle);

    float strength = 0.75f;
    float color[3] = {0.5f, 0.3f, 0.2f};

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 3 * sizeof(GLfloat), color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //uniforms_transformations
    glGenBuffers(1, &UT_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, UT_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, UT_buffer_handle);

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

    glBindBuffer(GL_UNIFORM_BUFFER, UT_buffer_handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
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