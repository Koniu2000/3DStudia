#version 460

layout(location = 0) out vec4 vFragColor;

#if __VERSION__ > 410
layout(std140, binding = 0) uniform Color {
#else
    layout(std140) uniform Color {
    #endif
    vec4 Kd;
    vec4 Ka;
    vec4 Ks;
    float Ns;
    bool use_map_Kd;
};

const int MAX_POINT_LIGHTS=24;

struct PointLight {
    vec3 position_in_world_space;
    vec3 position_in_view_space;
    vec3 color;
    float intensity;
};

layout(std140, binding = 2) uniform Lights {
    PointLight p_light[MAX_POINT_LIGHTS];
    vec3 ambient;
    int n_p_lights;
};

in vec2 vertex_texcoords;
in vec3 vertex_normals_in_vs;
in vec3 vertex_coords_in_vs;
uniform sampler2D map_Kd;

void main() {

    vec3 normal = normalize(vertex_normals_in_vs);
    vec3 view_direction = normalize(-vertex_coords_in_vs);

    vec3 diffuse;
    vec3 specular;

    for (int i = 0; i < n_p_lights; i++)
    {
        vec3 light_direction = normalize(p_light[i].position_in_view_space - vertex_coords_in_vs);
        float diff = max(dot(normal, light_direction), 0.0);
        diffuse = diff * p_light[i].color * p_light[i].intensity;

        vec3 reflect = reflect(-light_direction, normal);
        float spec = pow(max(dot(normal, reflect), 0.0), Ns);
        specular = spec * Ks.rgb * Ns;
    }
    vFragColor.a = Kd.a;
    vFragColor.rgb = Kd.rgb * (Ka.rgb + diffuse + specular);
}