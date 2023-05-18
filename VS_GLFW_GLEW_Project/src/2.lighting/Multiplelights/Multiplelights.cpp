#include <glad/glad.h>
#include<GLFW/glfw3.h>

#include <iostream>  
#include <filesystem>  

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<learnopengl/shader_s.h>
#include<learnopengl/camera.h>
#define STB_IMAGE_IMPLEMENTATION
#include<learnopengl/stb_image.h>

using namespace std;

#define NR_POINT_LIGHTS 4

struct Light
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec4 direction;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outCutOff;
};

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

GLFWwindow* InitGLFW();
void framebuffer_size_callbalc(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 0.0f, 2.0f);

int main()
{
	GLFWwindow* window = InitGLFW();
	if (window == NULL)
	{
		cout << "Failed to Create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callbalc);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader lightShader("./src/2.lighting/Lighting/shader.vs", "./src/2.lighting/Color/light.fs");
	Shader ourShader("./src/2.lighting/Multiplelights/shader.vs", "./src/2.lighting/Multiplelights/shader.fs");

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glm::vec3 lightAttenuation[] =
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


	DirLight dirLight
	{
		glm::vec3(-0.2f, -1.0f, -0.3f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.5f, 0.5f, 0.5f)
	};

	Spotlight spotLight
	{
		camera.Position,
		camera.Front,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		lightAttenuation[4].x,
		lightAttenuation[4].y,
		lightAttenuation[4].z, 
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(15.0f))
	};

	PointLight pointLights[NR_POINT_LIGHTS];
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		pointLights[i] =
		{
			pointLightPositions[i],
			glm::vec3(0.05f, 0.05f, 0.05f),
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			lightAttenuation[4].x,
			lightAttenuation[4].y,
			lightAttenuation[4].z
		};
	}

	unsigned int diffuse = loadTexture("./resources/textures/container2.png");
	unsigned int specular = loadTexture("./resources/textures/container2_specular.png");
	unsigned int emission = loadTexture("./resources/textures/1000.png");
	ourShader.use();
	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);
	ourShader.setInt("material.emission", 2);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.6275f, 0.3216f, 0.1765f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightShader.SetMatrix4fv("projection", glm::value_ptr(projection));
		lightShader.SetMatrix4fv("view", glm::value_ptr(view));
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		glBindVertexArray(VAO);
		float pointLightLength = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
		for (int i = 0; i < pointLightLength; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.25f));
			lightShader.SetMatrix4fv("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		ourShader.use();
		ourShader.setFloat("_time", currentFrame);


		ourShader.SetMatrix4fv("view", glm::value_ptr(view));
		ourShader.SetMatrix4fv("projection", glm::value_ptr(projection));
		ourShader.SetVec3("viewPos", camera.Position);
		ourShader.setFloat("material.shininess", 32.0f);


		//directionLight
		ourShader.SetVec3("dirLight.direction", dirLight.direction);
		ourShader.SetVec3("dirLight.ambient", dirLight.ambient);
		ourShader.SetVec3("dirLight.diffuse", dirLight.diffuse);
		ourShader.SetVec3("dirLight.specular", dirLight.specular);

		//PointLight
		ourShader.SetVec3("pointLights[0].position", pointLights[0].position);
		ourShader.SetVec3("pointLights[0].ambient", pointLights[0].ambient);
		ourShader.SetVec3("pointLights[0].diffuse", pointLights[0].diffuse);
		ourShader.SetVec3("pointLights[0].specular", pointLights[0].specular);
		ourShader.setFloat("pointLights[0].constant", pointLights[0].constant);
		ourShader.setFloat("pointLights[0].linear", pointLights[0].linear);
		ourShader.setFloat("pointLights[0].quadratic", pointLights[0].quadratic);

		ourShader.SetVec3("pointLights[1].position", pointLights[1].position);
		ourShader.SetVec3("pointLights[1].ambient", pointLights[1].ambient);
		ourShader.SetVec3("pointLights[1].diffuse", pointLights[1].diffuse);
		ourShader.SetVec3("pointLights[1].specular", pointLights[1].specular);
		ourShader.setFloat("pointLights[1].constant", pointLights[1].constant);
		ourShader.setFloat("pointLights[1].linear", pointLights[1].linear);
		ourShader.setFloat("pointLights[1].quadratic", pointLights[1].quadratic);

		ourShader.SetVec3("pointLights[2].position", pointLights[2].position);
		ourShader.SetVec3("pointLights[2].ambient", pointLights[2].ambient);
		ourShader.SetVec3("pointLights[2].diffuse", pointLights[2].diffuse);
		ourShader.SetVec3("pointLights[2].specular", pointLights[2].specular);
		ourShader.setFloat("pointLights[2].constant", pointLights[2].constant);
		ourShader.setFloat("pointLights[2].linear", pointLights[2].linear);
		ourShader.setFloat("pointLights[2].quadratic", pointLights[2].quadratic);

		ourShader.SetVec3("pointLights[3].position", pointLights[3].position);
		ourShader.SetVec3("pointLights[3].ambient", pointLights[3].ambient);
		ourShader.SetVec3("pointLights[3].diffuse", pointLights[3].diffuse);
		ourShader.SetVec3("pointLights[3].specular", pointLights[3].specular);
		ourShader.setFloat("pointLights[3].constant", pointLights[3].constant);
		ourShader.setFloat("pointLights[3].linear", pointLights[3].linear);
		ourShader.setFloat("pointLights[3].quadratic", pointLights[3].quadratic);

		//SpotLight
		ourShader.SetVec3("spotLight.position", camera.Position);
		ourShader.SetVec3("spotLight.direction", camera.Front);
		ourShader.SetVec3("spotLight.ambient", spotLight.ambient);
		ourShader.SetVec3("spotLight.diffuse", spotLight.diffuse);
		ourShader.SetVec3("spotLight.specular", spotLight.specular);
		ourShader.setFloat("spotLight.constant", spotLight.constant);
		ourShader.setFloat("spotLight.linear", spotLight.linear);
		ourShader.setFloat("spotLight.quadratic", spotLight.quadratic);
		ourShader.setFloat("spotLight.cutOff", spotLight.cutOff);
		ourShader.setFloat("spotLight.outCutOff", spotLight.outCutOff);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emission);

		glBindVertexArray(VAO);

		int cubelength = sizeof(cubePositions) / sizeof(cubePositions[0]);
		for (int i = 0; i < cubelength; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(25.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			ourShader.SetMatrix4fv("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

GLFWwindow* InitGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	return window;
}

void framebuffer_size_callbalc(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT_SHIFT, deltaTime);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	cout << path << endl;
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}