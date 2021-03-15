#version 330 core										
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;

out vec3 normal;
out vec3 fragPos;

//declaring transformation matrices as uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
    
void main() {		
    normal = mat3(transpose(inverse(model))) * inNormal;

    fragPos = vec3(model * vec4(inVertex, 1.0));
    //Multipy transformation matrices with vertex coordinates 
    //Note that we read the multiplication from right to left
    gl_Position = projection * view * vec4(fragPos, 1.0);
}														
