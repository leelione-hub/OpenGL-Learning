#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light
{
	vec3 position;
	vec4 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outCutOff;

};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float _time;



void main()
{
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir,normalize(-light.direction.xyz));	//spotlight
	float epsilon = light.cutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff)/epsilon,0.0,1.0);
	vec3 result = vec3(0.0);
	vec3 diffuseTex = vec3(texture(material.diffuse,TexCoords));

	//ambient
	vec3 ambient = light.ambient * diffuseTex;

	//diffuse
	float attenuation = 1.0;
	vec3 normal = normalize(Normal);
	if(light.direction.w > 0.0)
	{
		//lightDir = normalize(light.position - FragPos);
		float distance = length(light.position - FragPos);
		attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));
	}
	else
	{
		lightDir = normalize(-light.direction.xyz);
		attenuation = 1.0;
	}
	
	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = light.diffuse * (diff * diffuseTex) * intensity;

	//specular
	vec3 specularTex = vec3(texture(material.specular,TexCoords));
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * specularTex) * intensity;

	//emission
	vec2 uv = vec2(TexCoords.x,TexCoords.y + cos(_time * 0.3));
	vec3 emission = vec3(texture(material.emission,uv));

	result = (ambient + diffuse + specular) * attenuation;

	// if(theta > light.cutOff)
	// {
		
	// }
	// else
	// {
	// 	result = vec3(light.ambient * vec3(texture(material.diffuse,TexCoords)));
	// }
	//FragColor = vec4(light.diffuse,1.0);

	FragColor = vec4(result,1.0);
}