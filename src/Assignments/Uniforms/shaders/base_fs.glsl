#version 420

layout(location=0) out vec4 vFragColor;

in vec3 Color;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3 color;
};

void main() {
    vec3 UniformsColor = Color * (strength * color);
    vFragColor = vec4(UniformsColor, 1.0);
}

