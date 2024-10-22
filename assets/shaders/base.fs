#version 400 core


struct Material {
    sampler2D diffuse;
    vec3 specular;    
    float shininess;
}; 

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 color;

//void main() {
//	float lum = max(dot(normal, normalize(sun_position)), 0.0);
//	color = texture(tex, texcoord) * vec4((0.3 + 0.7 * lum) * sun_color, 1.0);
//}

void main()
{
    // ambient, color of ambient * diffuse texture
	vec3 ambient = (lightColor * 0.1) * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // negative lightDir?
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}