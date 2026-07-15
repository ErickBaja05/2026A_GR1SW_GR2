#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1; 
uniform vec3 viewPos;
uniform bool isTransparentPass; 

// ==========================================
// 1. ESTRUCTURAS DE LUCES
// ==========================================
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

#define MAX_POINT_LIGHTS 25
uniform int nr_point_lights;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform FlashLight flashLight;

// ==========================================
// 2. DECLARACIÓN DE FUNCIONES
// ==========================================
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 texColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor);
vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor);

void main() { 
    vec4 texColor = texture(texture_diffuse1, TexCoords);
     // ============================================================
    // SOLUCIÓN Z-BUFFER: Renderizado Condicional de 2 Pasadas
    // ============================================================
    // Pasada 1 (Opaca): Descartamos los vidrios
    // Z-BUFFER: Renderizado Condicional de 2 Pasadas
    if(!isTransparentPass && texColor.a < 0.98) { discard; }
    if(isTransparentPass && texColor.a >= 0.98) { discard; }
    if(isTransparentPass && texColor.a >= 0.98) { discard; }

    // Ajuste de Normales (Doble Cara para interiores)
    vec3 norm = Normal;
    if(length(norm) < 0.01) { 
        norm = vec3(0.0, 1.0, 0.0); 
    } else { 
        norm = normalize(norm); 
    }
    
    // Invertir normal si vemos la cara por detrás
    if(!gl_FrontFacing) { 
        norm = -norm; 
    }
    
    // Vector de vista (hacia la cámara)
    vec3 viewDir = normalize(viewPos - FragPos);

    // ==========================================
    // 3. CÁLCULO DE ILUMINACIÓN TOTAL
    // ==========================================
    vec3 result = vec3(0.0);

    // A. Luz Direccional (Luna/Sol)
    result += CalcDirLight(dirLight, norm, viewDir, texColor.rgb);

    // B. Luces Puntuales (Focos de la casa)
    int limit = min(nr_point_lights, MAX_POINT_LIGHTS);
    for(int i = 0; i < limit; i++) {
        // Optimización: Solo procesar si la luz está encendida
        if(length(pointLights[i].diffuse) > 0.01) {
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texColor.rgb);
        }
    }
    
    // C. Linterna del Jugador
    if(length(flashLight.diffuse) > 0.01) {
        result += CalcFlashLight(flashLight, norm, FragPos, viewDir, texColor.rgb);
    }

    // TONE MAPPING: Evita que la luz acumulada queme la imagen a blanco puro
    result = result / (result + vec3(1.0)); 

    FragColor = vec4(result, texColor.a);	
}

// ==========================================
// IMPLEMENTACIÓN DE FUNCIONES DE LUZ (Modelo Phong)
// ==========================================

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 texColor) {
    vec3 lightDir = normalize(-light.direction);
    
    // Componente Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Componente Especular (Brillo)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // 32.0 es el factor de pulido (shininess)
    
    vec3 ambient  = light.ambient  * texColor;
    vec3 diffuse  = light.diffuse  * diff * texColor;
    vec3 specular = light.specular * spec * vec3(0.3); // Reducimos la intensidad del specular asumiendo falta de mapa especular
    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    // Atenuación basada en la distancia
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 ambient  = light.ambient  * texColor;
    vec3 diffuse  = light.diffuse  * diff * texColor;
    vec3 specular = light.specular * spec * vec3(0.3);
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // Intensidad del cono (Bordes suaves)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient  = light.ambient  * texColor;
    vec3 diffuse  = light.diffuse  * diff * texColor;
    vec3 specular = light.specular * spec * vec3(0.3);
    
    return (ambient + diffuse + specular) * attenuation * intensity;
}