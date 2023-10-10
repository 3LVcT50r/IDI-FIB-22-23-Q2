#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform int camera;
uniform vec3 camuflaje;

out vec3 fcolor;

void main() {
    if (camera == 0) fcolor = color;
    else fcolor = color*vec3(0,1,0);

    if (camuflaje != vec3(1,1,1)) fcolor = camuflaje;

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
