//
// Created by Piotr Białas on 20/11/2021.
//

#include "PhongMaterial.h"

namespace xe {

    GLuint PhongMaterial::color_uniform_buffer_ = 0u;
    GLuint PhongMaterial::shader_ = 0u;
    GLint PhongMaterial::uniform_map_Kd_location_ = 0;

    void PhongMaterial::bind() {
        glUseProgram(program());
        int use_map_Kd = 0;
        if (map_Kd > 0) {
            glUniform1i(uniform_map_Kd_location_, map_Kd_unit);
            glActiveTexture(GL_TEXTURE0 + map_Kd_unit);
            glBindTexture(GL_TEXTURE_2D, map_Kd);
            use_map_Kd = 1;
        }
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, color_uniform_buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &Ka[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &Ks[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(float), &Ns);
        glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(float), sizeof(int), &use_map_Kd);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
    }


    void PhongMaterial::init() {

        auto program = xe::utils::create_program(
                {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/phong_vs.glsl"},
                 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/phong_fs.glsl"}});
        if (!program) {
            std::cerr << "Invalid program12" << std::endl;
            exit(-1);
        }

        shader_ = program;

        glGenBuffers(1, &color_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(float) + sizeof(int), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);

#if __APPLE__
        auto u_modifiers_index = glGetUniformBlockIndex(program, "Color");
        if (u_modifiers_index == -1) {
            std::cerr << "Cannot find Color uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_modifiers_index, 0);
        }
#endif

#if __APPLE__
        auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
        if (u_transformations_index == -1) {
            std::cerr << "Cannot find Transformations uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_transformations_index, 1);
        }
#endif

        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        if (uniform_map_Kd_location_ == -1) {
            //spdlog::warn("Cannot get uniform {} location", "map_Kd"); spdlog nie dziala
            std::cerr << "Invalid program111" << std::endl;
        }
    }
}