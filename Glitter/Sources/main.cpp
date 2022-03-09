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

//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;

string currentPath = "/Users/talosguo/Glitter/Glitter/Sources/";

const char * normalVS = "/Users/talosguo/Glitter/ShaderPrograms/normal.vs";
const char * normalFS = "/Users/talosguo/Glitter/ShaderPrograms/normal.fs";
const char * greenFS = "/Users/talosguo/Glitter/ShaderPrograms/green.fs";
const char * textureVS = "/Users/talosguo/Glitter/ShaderPrograms/texture.vs";
const char * textureFS = "/Users/talosguo/Glitter/ShaderPrograms/texture.fs";
const char * imgPath = "/Users/talosguo/Glitter/container.jpg";
const char * hahaImgPath = "/Users/talosguo/Glitter/awesomeface.png";

void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow *window);
void drawTriangle(GLFWwindow *window);
void drawTwoTrians(GLFWwindow *window);
void drawTriangleWithColor(GLFWwindow *window);
void drawRectWithTexture(GLFWwindow *window);
GLFWwindow* createWindow(int width, int height, string title);

// MARK: - main函数入口
int main() {
    
    GLFWwindow *window = createWindow(800, 600, "learn OpenGL");
    drawRectWithTexture(window);
    
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
/// 生成并绑定纹理1和纹理2
void bindTexture(const char * path_1, const char * path_2, Shader shaderProgram) {
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理, nrChannels为颜色通道的个数
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path_1, &width, &height, &nrChannels, 0);
    if (data) {
        // 前面参数为目标设置，后面参数为源数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理, nrChannels为颜色通道的个数
    stbi_set_flip_vertically_on_load(true); // 翻转
    data = stbi_load(path_2, &width, &height, &nrChannels, 0);
    if (data) {
        // 前面参数为目标设置，后面参数为源数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    
    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);
    // 绑定纹理,此处会自动赋值给片段着色器的采样器
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
}

// MARK: - 画三角形
void drawTriangle(GLFWwindow* window) {
    Shader shaderProgram(normalVS, normalFS);
    Shader shaderProgramGreen(normalVS, greenFS);
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


// MARK: - 画两个三角形（四边形）
void drawTwoTrians(GLFWwindow *window) {
    Shader shaderProgram(normalVS, normalFS);
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

// MARK: - 画顶点带颜色属性的三角形
void drawTriangleWithColor(GLFWwindow *window) {
    Shader shaderProgram(normalVS, normalFS);
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
// MARK: - 画带纹理的矩形
void drawRectWithTexture(GLFWwindow *window) {
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    unsigned int indices[] = {
        0, 1, 3, // first tri
        1, 2, 3  // second tri
    };
    Shader shaderProgram(textureVS, textureFS);
    // VAO对象 VBO对象
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    // 绑定缓冲 复制顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 解析顶点数据
    // 坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 纹理
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 透明度
    float alpha = 0;

    bindTexture(imgPath, hahaImgPath, shaderProgram);
    glEnable(GL_DEPTH_TEST);
    // 循环
    while (!glfwWindowShouldClose(window))
    {
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
            if (alpha < 1.0) {
                alpha += 0.01;
            }
        }
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
            if (alpha > 0) {
                alpha -= 0.01;
            }
        }
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float radius = 1.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        // 调整坐标系统矩阵
        glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // draw our first triangle
        shaderProgram.use();
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setFloat("alpha", alpha);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        for(unsigned int i = 0; i < 10; i++)
        {
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(model, cubePositions[i]);
          float angle = 20.0f * i;
          model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          shaderProgram.setMat4("model", model);

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
