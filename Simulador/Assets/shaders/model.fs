#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1; 
uniform vec3 viewPos;
uniform bool isTransparentPass; // Nueva variable para el control de 2 pasadas

struct DirLight { vec3 direction; vec3 ambient; vec3 diffuse; vec3 specular; };
struct PointLight { vec3 position; float constant; float linear; float quadratic; vec3 ambient; vec3 diffuse; vec3 specular; };
struct FlashLight { vec3 position; vec3 direction; float cutOff; float outerCutOff; float constant; float linear; float quadratic; vec3 ambient; vec3 diffuse; vec3 specular; };

#define MAX_POINT_LIGHTS 25
uniform int nr_point_lights;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform FlashLight flashLight;

void main() { 
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    
    // ============================================================
    // SOLUCIÓN Z-BUFFER: Renderizado Condicional de 2 Pasadas
    // ============================================================
    // Pasada 1 (Opaca): Descartamos los vidrios
    if(!isTransparentPass && texColor.a < 0.98) { discard; }
    // Pasada 2 (Transparente): Descartamos las paredes
    if(isTransparentPass && texColor.a >= 0.98) { discard; }

    // ============================================================
    // SOLUCIÓN LUZ INTERIOR: Ajuste de Normales de Doble Cara
    // ============================================================
    vec3 norm = Normal;
    if(length(norm) < 0.01) { norm = vec3(0.0, 1.0, 0.0); } 
    else { norm = normalize(norm); }
    
    // Si la cámara ve la cara por detrás (desde adentro de la casa), volteamos la normal
    if(!gl_FrontFacing) { norm = -norm; }
    
    vec3 viewDir = normalize(viewPos - FragPos);

    // 1. SOL
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 ambient = dirLight.ambient * texColor.rgb;
    vec3 diffuse = dirLight.diffuse * diff * texColor.rgb;
    vec3 result = ambient + diffuse;

    // 2. LUCES INTERIORES
    int limit = min(nr_point_lights, MAX_POINT_LIGHTS);
    for(int i = 0; i < limit; i++) {
        if(length(pointLights[i].diffuse) > 0.01) {
            vec3 pLightDir = normalize(pointLights[i].position - FragPos);
            float pDiff = max(dot(norm, pLightDir), 0.0);
            float distance = length(pointLights[i].position - FragPos);
            float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));    
            result += (pointLights[i].ambient + pointLights[i].diffuse * pDiff) * texColor.rgb * attenuation;
        }
    }
    
    // 3. LINTERNA
    if(length(flashLight.diffuse) > 0.01) {
        vec3 fLightDir = normalize(flashLight.position - FragPos);
        float fDiff = max(dot(norm, fLightDir), 0.0);
        float fDistance = length(flashLight.position - FragPos);
        float fAttenuation = 1.0 / (flashLight.constant + flashLight.linear * fDistance + flashLight.quadratic * (fDistance * fDistance));    
        float theta = dot(fLightDir, normalize(-flashLight.direction)); 
        float epsilon = flashLight.cutOff - flashLight.outerCutOff;
        float intensity = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0, 1.0);
        result += (flashLight.ambient + flashLight.diffuse * fDiff) * texColor.rgb * fAttenuation * intensity;
    }

    // ============================================================
    // SOLUCIÓN SOBREEXPOSICIÓN: Control de Intensidad (Tone Mapping)
    // Evita que la luz difusa de 5.3 queme la imagen en blanco puro
    // ============================================================
    result = result / (result + vec3(1.0)); 

    FragColor = vec4(result, texColor.a);	
}