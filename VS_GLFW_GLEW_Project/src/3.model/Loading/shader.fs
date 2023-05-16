#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D emission;
	float shininess;
};
uniform DirLight dirLight;
uniform Material material;
uniform vec3 viewPos;
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight,normal,viewDir);
	FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir,normal),0.0);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,TexCoords));

	return (ambient + diffuse + specular);
}