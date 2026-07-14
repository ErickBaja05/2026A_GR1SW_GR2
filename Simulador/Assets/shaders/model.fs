#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct FlashLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

#define MAX_LIGHTS 10

uniform int nbPointLights;
uniform int nbFlashLights;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform FlashLight flashLights[MAX_LIGHTS];

// Implementación de Light Maps
uniform sampler2D texture_diffuse1;  // Mapea el color difuso de la superficie
uniform sampler2D texture_specular1; // Mapea la capacidad reflectiva del material
uniform vec3 viewPos;                // Posición de la cámara en espacio de mundo

// Prototipos de funciones de cálculo Phong
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffColor, vec3 specColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor, vec3 specColor);
vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor, vec3 specColor);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Muestreo de texturas (Light Maps)
    vec3 diffColor = vec3(texture(texture_diffuse1, TexCoords));
    vec3 specColor = vec3(texture(texture_specular1, TexCoords));

    // Componente ambiental base para que la escena no sea completamente negra
    vec3 ambient = vec3(0.1) * diffColor;
    vec3 result = ambient;

    // 1. Añadir el Sol (Luz Direccional)
    result += CalcDirLight(dirLight, norm, viewDir, diffColor, specColor);

    // 2. Añadir focos del techo (Point Lights)
    for(int i = 0; i < nbPointLights && i < MAX_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, diffColor, specColor);
    }

    // 3. Añadir linternas (Flash Lights)
    for(int i = 0; i < nbFlashLights && i < MAX_LIGHTS; i++) {
        result += CalcFlashLight(flashLights[i], norm, FragPos, viewDir, diffColor, specColor);
    }

    FragColor = vec4(result, 1.0);
}

// Implementación de Luz Direccional (El Sol)
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffColor, vec3 specColor) {
    vec3 lightDir = normalize(-light.direction);
    
    // Difuso
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * diffColor;
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Shininess de 32
    vec3 specular = light.color * spec * specColor;
    
    return (diffuse + specular);
}

// Implementación de Punto de Luz (Focos del techo)
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor, vec3 specColor) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Difuso
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * diffColor;
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.color * spec * specColor;
    
    // Atenuación por distancia
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    return (diffuse + specular) * attenuation;
}

// Implementación de Linterna (Cono de Luz con bordes suaves)
vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor, vec3 specColor) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Cálculo del coseno del ángulo actual respecto a la dirección de la linterna
    float theta = dot(lightDir, normalize(-light.direction));
    
    // Transición suave entre el cono interior (cutOff) y el exterior (outerCutOff)
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Difuso
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * diffColor;
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.color * spec * specColor;
    
    // Atenuación
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    return (diffuse + specular) * attenuation * intensity;
}