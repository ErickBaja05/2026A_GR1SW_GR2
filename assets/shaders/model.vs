#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calculamos la posición del vértice en el mundo
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Calculamos la normal asegurando que las rotaciones/escalas no la deformen
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    TexCoords = aTexCoords;    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}