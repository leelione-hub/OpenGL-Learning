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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //设置glfw最高版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//设置glfw最低版本
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //设置为核心模式
    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //设为当前窗口

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //当前窗口的回调函数，用以变化窗口大小

    //初始化glad，方便glfs函数调用
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600); //设置显示区位置大小

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);  //检测输入并做相应处理

        //渲染指令
        //清空屏幕
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //状态设置函数：设置清空后的屏幕颜色，此时为深蓝绿色
        glClear(GL_COLOR_BUFFER_BIT);  //状态使用函数：只清空颜色缓冲，其他缓冲位参数有GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
        //drawSquare();
        drawTriangle();


        glfwSwapBuffers(window);  //交换颜色缓冲，并显示到窗口
        glfwPollEvents();  //检查触发事件，更新窗口状态，并调用对应回调函数
    }

    glfwTerminate();  //释放/删除分配的资源
    return 0;

}
//改变窗口大小时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//检测输入，是否有按键发生
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //用户没有ESC按下，返回GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
}

//创建着色器程序，创建多个着色器并链接他们
unsigned int creatShaderProc()
{
    //创建顶点着色器，顶点着色器源码
    //GLSL语言 GLSL330对应opengl3.3版本 核心模式
    //多行字符串 R"(...)"中”和（不能有间隔
    //layout位置 in ：输入 vec3：三维向量 apos：变量名
    const char ShaderSource[] = R"(
        #version 330 core  
        layout(location = 0) in vec3 aPos; 

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    const GLchar* vertexShaderSource = (GLchar*)&ShaderSource;  //glShaderSource函数指定着色器源码时要是用这个类型的数据
    //编译顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //检测编译结果
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {   
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    ///
    //创建片段着色器
    const char fragSource[] = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 ourColor;
    void main()
    {
        //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        FragColor = ourColor;
    } 
    )";
    const GLchar* fragShaderSource = (GLchar*)fragSource;

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);
    ///
    //链接着色器
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); //着色器附加到程序对象
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检测链接是否成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "shader link to program failed!" << infoLog << endl;
    }
    return shaderProgram;
}

///渲染一个三角形,采用VAO顶点数组对象
void drawTriangle()
{
    //顶点三维坐标
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);  //1:要生成缓冲对象的数量，VBO:用来存储缓冲对象的数组
    //1.复制顶点数组到缓冲区供opengl使用
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //可同时绑定多个不同类型的缓冲 
    //将数据复制到当前绑定缓冲的内存中，并指定显卡管理缓存的形式,STATIC,DYNAMIC,STREAM
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //
        /*2.****设置顶点属性指针
        对于glVertexAttribPointer函数，参数依次表示：location值，顶点属性大小（维度），
        数据类型，是否标准化（映射到0或-1 到1 之间），步长（连续顶点属性组之间的间隔大小），
        位置数据距缓冲开始位置的偏移量
        */
        //从当前绑定的GL_ARRAY_BUFFER中VBO中获取数据，顶点属性0现在会链接到他的顶点数据
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //    glEnableVertexAttribArray(0); //以顶点属性值为参数，启用顶点属性，顶点属性默认是禁用的
        //3.使用着色器程序
    //    glUseProgram(shaderProgram); //激活程序对象

    //    glDeleteShader(vertexShader); //已经用不到着色器对象了，删除之
    //    glDeleteShader(fragmentShader);
        // 3. 绘制物体
        //someOpenGLFunctionThatDrawsOurTriangle();


        /*用VAO顶点数组对象保存顶点属性指针配置  v    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);*/
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //1.绑定VAO
    glBindVertexArray(VAO);
    //2.绑定VBO并读取到缓冲区，并指定显卡管理缓存的方式
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //3.设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //...

    //4.绘制物体
    unsigned int shaderProgram = creatShaderProc();
    //激活着色器
    glUseProgram(shaderProgram);

    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) * 0.5f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);  //GL_TRIANGLE:要绘制的图元类型 0：顶点数组的起始索引，3：要绘制的顶点个数
    //
}

//渲染一个矩形，采用EBO索引缓冲
void drawSquare()
{

    //引入索引缓冲
    float vertices[] = {  //顶点数据
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = { //索引数据 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    unsigned int VAO, VBO, EBO;
    //绑定VAO,VBO,EBO  VAO会存储VBO的调用，在VAO解绑前解绑VBO，否则存储就没了
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int shaderProgram = creatShaderProc();

    //渲染循环
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //线框模式 polygon：多边形 GL_FRONT_AND_BACK：应用到图形的正面和背面 GL_LINE:用线条绘制
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}