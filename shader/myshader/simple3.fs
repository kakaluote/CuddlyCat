#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 

void main()
{
	vec3 lightDir = normalize(vec3(0, -1, 1));
	vec3 lightColor = vec3(1.0,1.0,1.0);
	
	vec3 ambientColor = vec3(0.2,0.0,0.0);
	vec3 diffuseColor = vec3(1.0,0.0,0.0);
	vec3 specularColor = vec3(1.0,1.0,1.0);
	float gross = 32;
	
    // ambient
    vec3 ambient = ambientColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);  
    float spec = pow(min(max(dot(viewDir, reflectDir), 0.0), 1.0), gross);
    vec3 specular = spec * lightColor * specularColor;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 