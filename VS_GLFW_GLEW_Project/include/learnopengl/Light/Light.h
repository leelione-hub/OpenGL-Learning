#ifndef CUSTOM_LIGHTDATA
#define CUSTOM_LIGHTDATA
#include<glm/glm.hpp>

struct DirLight
{
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Spotlight
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outCutOff;
};

class CustomLight
{
public:
	glm::vec3 lightAttenuation[12] =
	{
		glm::vec3(1.0f,0.7f,1.8f),			//7
		glm::vec3(1.0f,0.35f,0.44f),		//13
		glm::vec3(1.0f,0.22f,0.20f),		//20
		glm::vec3(1.0f,0.14f,0.07f),		//32
		glm::vec3(1.0f,0.09f,0.032f),		//50
		glm::vec3(1.0f,0.07f,0.017f),		//65
		glm::vec3(1.0f,0.045f,0.0075f),		//100
		glm::vec3(1.0f,0.027f,0.0028f),		//160
		glm::vec3(1.0f,0.022f,0.0019f),		//200
		glm::vec3(1.0f,0.014f,0.0007f),		//325
		glm::vec3(1.0f,0.007f,0.0002f),		//600
		glm::vec3(1.0f,0.0014f,0.000007f),	//3250
	};
};


#endif // !CUSTOM_LIGHTDATA
