#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw3.lib")

using namespace std;

float vertices[];

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int creatShaderProc();
void drawTriangle();
void drawSquare();

int main()
        {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //����glfw��߰汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//����glfw��Ͱ汾
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //����Ϊ����ģʽ
    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //��������
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //��Ϊ��ǰ����

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //��ǰ���ڵĻص����������Ա仯���ڴ�С

    //��ʼ��glad������glfs��������
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 400, 300); //������ʾ��λ�ô�С

    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);  //������벢����Ӧ����

        //��Ⱦָ��
        //�����Ļ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //״̬���ú�����������պ����Ļ��ɫ����ʱΪ������ɫ
        glClear(GL_COLOR_BUFFER_BIT);  //״̬ʹ�ú�����ֻ�����ɫ���壬��������λ������GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT
        //drawSquare();
        drawTriangle();


        glfwSwapBuffers(window);  //������ɫ���壬����ʾ������
        glfwPollEvents();  //��鴥���¼������´���״̬�������ö�Ӧ�ص�����
    }

    glfwTerminate();  //�ͷ�/ɾ���������Դ
    return 0;

}
//�ı䴰�ڴ�Сʱ�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//������룬�Ƿ��а�������
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //�û�û��ESC���£�����GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
}

//������ɫ�����򣬴��������ɫ������������
unsigned int creatShaderProc()
{
    //����������ɫ����������ɫ��Դ��
    //GLSL���� GLSL330��Ӧopengl3.3�汾 ����ģʽ
    //�����ַ��� R"(...)"�С��ͣ������м��
    //layoutλ�� in ������ vec3����ά���� apos��������
    const char ShaderSource[] = R"(
        #version 330 core  
        layout(location = 0) in vec3 aPos; 

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    const GLchar* vertexShaderSource = (GLchar*)&ShaderSource;  //glShaderSource����ָ����ɫ��Դ��ʱҪ����������͵�����
    //���붥����ɫ��
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //��������
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {   
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    ///
    //����Ƭ����ɫ��
    const char fragSource[] = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    } 
    )";
    const GLchar* fragShaderSource = (GLchar*)fragSource;

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);
    ///
    //������ɫ��
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); //��ɫ�����ӵ��������
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //��������Ƿ�ɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "shader link to program failed!" << infoLog << endl;
    }
    return shaderProgram;
}

///��Ⱦһ��������,����VAO�����������
void drawTriangle()
{
    //������ά����
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);  //1:Ҫ���ɻ�������������VBO:�����洢������������
    //1.���ƶ������鵽��������openglʹ��
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //��ͬʱ�󶨶����ͬ���͵Ļ��� 
    //�����ݸ��Ƶ���ǰ�󶨻�����ڴ��У���ָ���Կ����������ʽ,STATIC,DYNAMIC,STREAM
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //
        /*2.****���ö�������ָ��
        ����glVertexAttribPointer�������������α�ʾ��locationֵ���������Դ�С��ά�ȣ���
        �������ͣ��Ƿ��׼����ӳ�䵽0��-1 ��1 ֮�䣩����������������������֮��ļ����С����
        λ�����ݾ໺�忪ʼλ�õ�ƫ����
        */
        //�ӵ�ǰ�󶨵�GL_ARRAY_BUFFER��VBO�л�ȡ���ݣ���������0���ڻ����ӵ����Ķ�������
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //    glEnableVertexAttribArray(0); //�Զ�������ֵΪ���������ö������ԣ���������Ĭ���ǽ��õ�
        //3.ʹ����ɫ������
    //    glUseProgram(shaderProgram); //����������

    //    glDeleteShader(vertexShader); //�Ѿ��ò�����ɫ�������ˣ�ɾ��֮
    //    glDeleteShader(fragmentShader);
        // 3. ��������
        //someOpenGLFunctionThatDrawsOurTriangle();


        /*��VAO����������󱣴涥������ָ������  v    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);*/
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //1.��VAO
    glBindVertexArray(VAO);
    //2.��VBO����ȡ������������ָ���Կ�������ķ�ʽ
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //3.���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //...

    //4.��������
    unsigned int shaderProgram = creatShaderProc();
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);  //GL_TRIANGLE:Ҫ���Ƶ�ͼԪ���� 0�������������ʼ������3��Ҫ���ƵĶ������
    //
}

//��Ⱦһ�����Σ�����EBO��������
void drawSquare()
{

    //������������
    float vertices[] = {  //��������
    0.5f, 0.5f, 0.0f,   // ���Ͻ�
    0.5f, -0.5f, 0.0f,  // ���½�
    -0.5f, -0.5f, 0.0f, // ���½�
    -0.5f, 0.5f, 0.0f   // ���Ͻ�
    };

    unsigned int indices[] = { //�������� ע��������0��ʼ! 
        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };

    unsigned int VAO, VBO, EBO;
    //��VAO,VBO,EBO  VAO��洢VBO�ĵ��ã���VAO���ǰ���VBO������洢��û��
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int shaderProgram = creatShaderProc();

    //��Ⱦѭ��
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //�߿�ģʽ polygon������� GL_FRONT_AND_BACK��Ӧ�õ�ͼ�ε�����ͱ��� GL_LINE:����������
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}