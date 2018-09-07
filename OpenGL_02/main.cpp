//#include"../../../OpenGL/Commen/Commen.h"
#include<iostream>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>

using namespace std;

//ʹ��char������ GLSL����
const GLchar* vertexShaderSource[] = { "#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}\0" };
const GLchar* fragmentShaderSource[] = {"#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0" };

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//���ڲ��Լ����¼�
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		cout << "Press Up SUCCESS";
		//glfwSetWindowSize(window, 1366, 768);
		glfwSetWindowPos(window, 500, 500);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		cout << "Press Down SUCCESS";
		//glfwSetWindowSize(window, 800, 600);
		glfwSetWindowPos(window, 0, 35);
	}
	
}

int main(int agrc, char** agrv) 
{
	//ʵ����GLFW����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RELEASE, GLFW_FALSE);

	//����һ�����ڶ���
	GLFWwindow* window = glfwCreateWindow(800,600,"Learn OpenGL",nullptr,nullptr);
	if (window == nullptr) {
		cout << "Fail to Creat GLFW Window";
		glfwTerminate();							//����glfwTerminate()�������ͷ�GLFW������ڴ�
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);		//ͨ��glfwע��ص�����
	
													//��ʼ��GLEW	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Fail to Init GLEW Window";
		return -1;
	}
	
	//���ô���ά�ȣ���С�����꣩
	int Height,Width;
	glfwGetFramebufferSize(window, &Width, &Height);
	glViewport( 1, 1, Width, Height);
	
	GLfloat Vertices[] = {								//����������
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f, 0.5f,0.0f
	};

	GLuint vertexShader;									//������ɫ��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPLTION_FAILD" << infolog << endl;
	}

	GLuint fragmentShader;								//Ƭ����ɫ��
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLTION_FAILD" << infolog << endl;
	}

	GLuint shaderProgram;							//��ɫ������
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		cout << "ERROR::PROGRAM::COMPILE_FAILE" << infolog << endl;
	}
	//glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//glVertexAttribPointer(0, 3, GL_FLOAT, 3 * sizeof(GLfloat),(GLsizei)0,0);
	//glEnableVertexAttribArray(0);

	GLuint VBO;							//VBO
	glGenBuffers(1, &VBO);
	//���������ݸ��Ƶ������� ��OpenGLʹ��	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	GLuint VAO;							//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	//���ö�������ָ��
	glEnableVertexAttribArray(0);

	//���ִ���һֱ���ڱ���Ⱦ״̬
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();							//��鴥���¼�

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);		//ʹ���Զ�����ɫ�����Ļ
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);				//��Ⱦ������
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);					//����ǰ�󻺳���
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(0, &VBO);
	glfwTerminate();								//����glfwTerminate()�������ͷ�GLFW������ڴ�

	return 0;
}
