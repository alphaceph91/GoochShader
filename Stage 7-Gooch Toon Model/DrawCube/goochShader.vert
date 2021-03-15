#version 330 core										
layout (location = 0) in vec4 inVertex;
layout (location = 1) in vec3 inNormal;

out vec3 normal;
out vec3 fragPos;

//declaring transformation matrices as uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos; // defined in main

out float NdotL;
out vec3 ReflectVec;
out vec3 ViewVec;

void main() 
{		
  normal = mat3(transpose(inverse(model))) * inNormal;
  vec3 fragPos = vec3(model * inVertex);
  vec3 nnormal = normalize(normal); //nnormal is the normalized normal
  vec3 lightDir = normalize(lightPos - fragPos);
  ReflectVec = normalize(reflect(-lightDir, nnormal));
  ViewVec = normalize(-fragPos);

  NdotL = dot(nnormal, lightDir) * 0.5 + 0.5;

  gl_Position = projection * view * model * inVertex;
}														
