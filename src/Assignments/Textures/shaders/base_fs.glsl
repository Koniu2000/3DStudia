#version 420

layout(location=0) out vec4 vFragColor;

in vec3 Color;

layout(std140, binding = 1) uniform Modifier {
    float strength;
    vec3 color;
};

void main() {
    vec3 NewColor = Color * strength * color;
    vFragColor = vec4(NewColor, 1.0);
}
