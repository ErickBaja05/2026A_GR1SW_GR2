#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1; 
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

void main() { 
    // 1. Cargar Textura y Canal Alpha (Transparencia para Vidrios)
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    
    // Si el pixel es transparente (ej. el centro de una ventana), ¡bórralo!
    if(texColor.a < 0.1) {
        discard;
    }
    
    // Si la textura falla o es negra por defecto, forzamos un gris claro 
    // para que la luz funcione y no se vea negra.
    //if (length(texColor.rgb) < 0.05) {
       //texColor.rgb = vec3(0.8);
    //}

    // 2. Normales seguras (Para evitar más errores matemáticos)
    vec3 norm = Normal;
    if(length(norm) < 0.01) { norm = vec3(0.0, 1.0, 0.0); } 
    else { norm = normalize(norm); }
    
    vec3 viewDir = normalize(viewPos - FragPos);

    // ==========================================
    // 3. LUZ DEL SOL (Direccional)
    // ==========================================
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0); 
    
    vec3 ambient = dirLight.ambient * texColor.rgb;
    vec3 diffuse = dirLight.diffuse * diff * texColor.rgb;
    vec3 result = ambient + diffuse;

    // ==========================================
    // 4. LUCES DE LAS HABITACIONES
    // ==========================================
    int limit = min(nr_point_lights, MAX_POINT_LIGHTS);
    for(int i = 0; i < limit; i++) {
        // Solo sumamos luz si la luz realmente está encendida (color mayor a 0)
        if(length(pointLights[i].diffuse) > 0.01) {
            vec3 pLightDir = normalize(pointLights[i].position - FragPos);
            float pDiff = max(dot(norm, pLightDir), 0.0);
            float distance = length(pointLights[i].position - FragPos);
            float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));    
            
            vec3 pAmbient = pointLights[i].ambient * texColor.rgb;
            vec3 pDiffuse = pointLights[i].diffuse * pDiff * texColor.rgb;
            result += (pAmbient + pDiffuse) * attenuation;
        }
    }
    
    // ==========================================
    // 5. LINTERNA JUGADOR
    // ==========================================
    if(length(flashLight.diffuse) > 0.01) {
        vec3 fLightDir = normalize(flashLight.position - FragPos);
        float fDiff = max(dot(norm, fLightDir), 0.0);
        float fDistance = length(flashLight.position - FragPos);
        float fAttenuation = 1.0 / (flashLight.constant + flashLight.linear * fDistance + flashLight.quadratic * (fDistance * fDistance));    
        
        float theta = dot(fLightDir, normalize(-flashLight.direction)); 
        float epsilon = flashLight.cutOff - flashLight.outerCutOff;
        float intensity = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0, 1.0);
        
        vec3 fAmbient = flashLight.ambient * texColor.rgb;
        vec3 fDiffuse = flashLight.diffuse * fDiff * texColor.rgb;
        result += (fAmbient + fDiffuse) * fAttenuation * intensity;
    }

    // 6. Color Final
    FragColor = vec4(result, texColor.a);	
}