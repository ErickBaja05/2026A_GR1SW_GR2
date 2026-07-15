#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Texturas cargadas por Assimp
uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_specular1;

// Posición de la cámara
uniform vec3 viewPos;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// En tu LightManager se llama flashLight, no spotLight
struct FlashLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Definimos un máximo de luces por seguridad
#define MAX_POINT_LIGHTS 25
uniform int nr_point_lights;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform FlashLight flashLight;

// Funciones
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 texColor, vec4 specColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 texColor, vec4 specColor);
vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 texColor, vec4 specColor);

void main() { 
    // Obtenemos los colores de las texturas
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    vec4 specColor = texture(texture_specular1, TexCoords);
    
    // Transparencia para ventanas (Vidrio)
    if(texColor.a < 0.05) {
        discard;
    }

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 1. Luz de luna/sol (Direccional)
    vec3 result = CalcDirLight(dirLight, norm, viewDir, texColor, specColor);
    
    // 2. Focos de las habitaciones (Point Lights)
    int limit = min(nr_point_lights, MAX_POINT_LIGHTS);
    for(int i = 0; i < limit; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texColor, specColor);    
    }
    
    // 3. Linterna del Jugador (FlashLight)
    result += CalcFlashLight(flashLight, norm, FragPos, viewDir, texColor, specColor);    
    
    // Guardamos el color final, respetando el canal Alpha
    FragColor = vec4(result, texColor.a);	
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 texColor, vec4 specColor) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 ambient = light.ambient * vec3(texColor);
    vec3 diffuse = light.diffuse * diff * vec3(texColor);
    vec3 specular = light.specular * spec * vec3(specColor);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 texColor, vec4 specColor) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 ambient = light.ambient * vec3(texColor);
    vec3 diffuse = light.diffuse * diff * vec3(texColor);
    vec3 specular = light.specular * spec * vec3(specColor); // Reflejo
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 texColor, vec4 specColor) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * vec3(texColor);
    vec3 diffuse = light.diffuse * diff * vec3(texColor);
    vec3 specular = light.specular * spec * vec3(specColor);
    
    return (ambient + diffuse + specular) * attenuation * intensity;
}