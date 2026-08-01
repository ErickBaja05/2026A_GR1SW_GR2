#version 330 core
out vec4 FragColor;

// NUEVO: Recibimos el color exacto de la luz desde C++
uniform vec3 lightColor;
void main()
{
    //FragColor = vec4(1.0); // set alle 4 vector values to 1.0
    FragColor = vec4(lightColor, 1.0);

}