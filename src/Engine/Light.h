//
// Created by kamil on 04.02.2024.
//

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

namespace xe {
    struct PointLight {
        PointLight() = default;
        PointLight(const glm::vec3 &pos, const glm::vec3 &color, float intensity)
                : position_in_ws(pos),
                  color(color), intensity(intensity) {}

        alignas(16) glm::vec3 position_in_ws;
        alignas(16) glm::vec3 position_in_vs;
        alignas(16) glm::vec3 color;
        float intensity;
    };
}