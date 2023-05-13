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

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Spotlight
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outCutOff;
};

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Spotlight spotLight;

uniform vec3 viewPos;
uniform Material material;
//uniform Light light;
uniform float _time;

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir);
vec3 CalcSpotLight(Spotlight light,vec3 normal,vec3 viewDir);


void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0);

	//DirectLight
	result = CalcDirLight(dirLight,normal,viewDir);
	//PointLight
	for(int i = 0;i < NR_POINT_LIGHTS;i++)
	{
		result += CalcPointLight(pointLights[i],normal,viewDir);
	}

	//SpotLight
	vec3 spot = CalcSpotLight(spotLight,normal,viewDir);
	result = (1.0 - spot.x) * result + spot.x * spot;

	FragColor = vec4(result,1.0);
}


vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir,normal),0.0);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(light.position-FragPos);
	float diff = max(dot(lightDir,normal),0.0);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(Spotlight light,vec3 normal,vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir,normalize(-light.direction));
	float epsilon = light.cutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff)/epsilon,0.0,1.0);

	float diff = max(dot(lightDir,normal),0.0);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
	vec3 emission = texture(material.emission,(-FragPos.xy + vec2(0.5,0.5))).xyz;

	return (ambient + (emission ) * intensity) * attenuation;
	//return emission;
}
