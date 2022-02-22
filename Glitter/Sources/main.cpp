// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

using namespace std;
void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow *window);
void drawTriangle();

int main() {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(1800, 1600, "learn", NULL, NULL);
    if (window == NULL) {
        cout << "failed to load" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "failed to load glad" << endl;
        return -1;
    }
    
    glViewport(0, 0, 1800, 1600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    while (!glfwWindowShouldClose(window)) {
        // 输入
        processInput(window);
        
        // 渲染
        drawTriangle();
        // 状态设置
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 状态使用
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
/// 窗口大小变更
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
/// 检查ESC输入
void processInput(GLFWwindow *window) {
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}
/// 画三角形
void drawTriangle() {
    // 创建着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 附加着色器源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检测着色器编译状态
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR Info\n" << infoLog << endl;
    }
    // 片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    // 三角形顶点
    float vertices[] = {
        -0.5f, -0.5f,0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    // VAO对象 VBO对象
    unsigned int VAO;
    glGenVertexArrays(1, & VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    // 绑定缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 复制顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 激活
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // 解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 绘制物体
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
