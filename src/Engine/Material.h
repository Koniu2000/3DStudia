//
// Created by Piotr Białas on 02/11/2021.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <memory>

#include "Application/utils.h"

namespace xe {

    class Material {
    public:
        virtual void bind() = 0;
        virtual void unbind() {};
    };

    class ColorMaterial : public Material {
    public:

        ColorMaterial(const glm::vec4 color) : color_(color), texture_(0), texture_unit_(0) {}

        ColorMaterial(GLuint texture, GLuint texture_unit) : color_((glm::vec4){1.0, 1.0, 1.0, 1.0}){
            set_texture(texture);
            set_texture_unit(texture_unit);
        }

        void bind();

        static void init();

        static GLuint program() { return shader_; }

        void set_texture(GLuint texture) { texture_ = texture; }
        void set_texture_unit(GLuint texture_unit) { texture_unit_ = texture_unit; }

    private:
        static GLuint shader_;
        static GLuint color_uniform_buffer_;
        static GLint uniform_map_Kd_location_;

        glm::vec4 color_;
        GLuint texture_;
        GLuint texture_unit_;

    };

    GLuint create_texture(const std::string &name);

}


