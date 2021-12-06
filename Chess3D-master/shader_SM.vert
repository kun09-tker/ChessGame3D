#version 400

in vec3 vertex;


uniform mat4 MVP_matrix;

void main(){
    gl_Position =  MVP_matrix * vec4(vertex,1.0);
}
