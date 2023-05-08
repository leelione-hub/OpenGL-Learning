#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;



void main()
{
	vec3 diffuseTex = vec3(texture(material.diffuse,TexCoords));

	//ambient
	vec3 ambient = light.ambient * diffuseTex;

	//diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = light.diffuse * (diff * diffuseTex);

	//specular
	vec3 specularTex = vec3(texture(material.specular,TexCoords));
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * specularTex);

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result,1.0);
}