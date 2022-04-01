#version 300 es

precision mediump float;

uniform sampler2D s_tex0;

uniform vec3 materialDiff;
uniform vec3 sourceDiff;

in vec3 v_normal;
in vec2 v_texCoord;
in vec3 v_lightDir;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 materialDiff = texture(s_tex0, v_texCoord).xyz;

    float diff = dot(v_normal, v_lightDir);
    diff = max(0.1, diff);

    vec3 color = diff * materialDiff * sourceDiff;
    fragColor = vec4(color, 1.0);
}
