// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.cpp"


// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

string currentPath = "/Users/talosguo/Glitter/Glitter/Sources/";

const char * normalVS = "/Users/talosguo/Glitter/ShaderPrograms/normal.vs";
const char * normalFS = "/Users/talosguo/Glitter/ShaderPrograms/normal.fs";
const char * greenFS = "/Users/talosguo/Glitter/ShaderPrograms/green.fs";

void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow *window);
void drawTriangle(GLFWwindow *window);
void drawTwoTrians(GLFWwindow *window);
void drawTriangleWithColor(GLFWwindow *window);
unsigned int createProgram();
GLFWwindow* createWindow(int width, int height, string title);
const char* getCharPath(string stringPath);

int main() {
    
    GLFWwindow *window = createWindow(800, 600, "learn OpenGL");
    drawTriangleWithColor(window);
    
    glfwTerminate();
    return 0;
}
/// 创建窗口
GLFWwindow* createWindow(int width, int height, string title) {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    char* titleChar = (char*)title.c_str();
    GLFWwindow* window = glfwCreateWindow(width, height, titleChar, NULL, NULL);
    if (window == NULL) {
        cout << "failed to load" << endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "failed to load glad" << endl;
        glfwTerminate();
    }
    
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
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
void drawTriangle(GLFWwindow* window) {
    Shader shaderProgram(normalVS, normalFS);
    Shader shaderProgramGreen(normalVS, greenFS);
    // MARK: - 创建顶点和缓冲对象
    // 三角形顶点
    float vertices[] = {
        -0.5f, -0.5f,0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        -0.5f, 0.7f, 0.0f,
        0.5f, 0.7f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    // VAO对象 VBO对象
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // 绑定缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 复制顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    // 循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        shaderProgram.use();
        // 更新动态颜色
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue)/2.0f + 0.5f;
        float value[4] = {0.0f, greenValue, 0.0f, 1.0f};
        shaderProgram.setVec4("ourColor", value);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        shaderProgramGreen.use();
        glDrawArrays(GL_TRIANGLES, 3, 6);
        // glBindVertexArray(0); // no need to unbind it every time
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
/// 画两个三角形（四边形）
void drawTwoTrians(GLFWwindow *window) {
    Shader shaderProgram(normalVS, normalFS);
    // MARK: - 创建顶点和缓冲对象
    // 四角形顶点
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    // 顶点绘制顺序
    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // VAO对象 VBO对象 EBO对象
    unsigned int VAO, VBO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, & VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // 绑定缓冲 复制顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        shaderProgram.use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // 线框模式
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
/// 画顶点带颜色属性的三角形
void drawTriangleWithColor(GLFWwindow *window) {
    Shader shaderProgram(normalVS, normalFS);
    // MARK: - 创建顶点和缓冲对象
    // 三角形顶点
    float vertices[] = {
        // 位置           //颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    // VAO对象 VBO对象
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // 绑定缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 复制顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // 循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        float timeValue = glfwGetTime();
        float xScaleValue = cos(timeValue) * 0.707f;
        float yScaleValue = sin(timeValue) * 0.707f;
        float zScaleValue = 0.0f;
        float scaleValues[3] = {xScaleValue, yScaleValue, zScaleValue};
        shaderProgram.use();
        shaderProgram.setVec3("positionOffset", scaleValues);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
