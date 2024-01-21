#version 330


layout (location = 0) in vec3 pos;

out vec3 wPos;

uniform mat4 model;

void main()
{
    wPos = (model * vec4(pos, 1.0)).xyz;
    gl_Position = vec4(pos.xy, 1.0, 1.0);
}