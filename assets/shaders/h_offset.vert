#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    flaot offset = 0.5
    gl_Position = vec4(aPos.x + offset, aPos.yz, 1.0);
}