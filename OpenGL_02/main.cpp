//#include"../../../OpenGL/Commen/Commen.h"
#include<iostream>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>

using namespace std;

//使用char数组存放 GLSL代码
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
	//用于测试键盘事件
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
	//实例化GLFW窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RELEASE, GLFW_FALSE);

	//创建一个窗口对象
	GLFWwindow* window = glfwCreateWindow(800,600,"Learn OpenGL",nullptr,nullptr);
	if (window == nullptr) {
		cout << "Fail to Creat GLFW Window";
		glfwTerminate();							//调用glfwTerminate()函数来释放GLFW分配的内存
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);		//通过glfw注册回调函数
	
													//初始化GLEW	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Fail to Init GLEW Window";
		return -1;
	}
	
	//设置窗口维度（大小及坐标）
	int Height,Width;
	glfwGetFramebufferSize(window, &Width, &Height);
	glViewport( 1, 1, Width, Height);
	
	GLfloat Vertices[] = {								//三角形坐标
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f, 0.5f,0.0f
	};

	GLuint vertexShader;									//顶点着色器
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

	GLuint fragmentShader;								//片段着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLTION_FAILD" << infolog << endl;
	}

	GLuint shaderProgram;							//着色器程序
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
	//将顶点数据复制到缓冲中 供OpenGL使用	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	GLuint VAO;							//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	//设置顶点属性指针
	glEnableVertexAttribArray(0);

	//保持窗口一直处于被渲染状态
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();							//检查触发事件

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);		//使用自定义颜色清空屏幕
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);				//渲染三角形
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);					//交换前后缓冲区
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(0, &VBO);
	glfwTerminate();								//调用glfwTerminate()函数来释放GLFW分配的内存

	return 0;
}
