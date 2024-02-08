//
// Created by Piotr Białas on 20/11/2021.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <memory>

#include "Material.h"
#include "Application/utils.h"

namespace xe {

    class PhongMaterial : public Material {
    public:

        PhongMaterial(const glm::vec4 color, const glm::vec4 amb, const glm::vec4 spec, float specShi) : Kd(color), Ka(amb), Ks(spec), Ns(specShi) {}

        PhongMaterial(GLuint texture, GLuint texture_unit) : Kd((glm::vec4){1.0, 1.0, 1.0, 1.0}){
            set_texture(texture);
        }

        void bind();

        static void init();

        static GLuint program() { return shader_; }

        void set_texture(GLuint texture) { map_Kd = texture; }

        glm::vec4 Kd;
        glm::vec4 Ka;
        glm::vec4 Ks;
        float Ns;
        GLuint map_Kd;
        const GLuint map_Kd_unit = 0;

    private:
        static GLuint shader_;
        static GLuint color_uniform_buffer_;
        static GLint uniform_map_Kd_location_;
    };

}