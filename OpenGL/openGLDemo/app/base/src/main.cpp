
/*
* @brief glad + glfw
* @see https://www.cnblogs.com/keguniang/p/9897420.html
*/

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
using namespace std;

static void framebuffer_size_callback(GLFWwindow* w, int width, int height)
{
	glViewport(0, 0, width, height);
	cout << __FUNCTION__ << " w: " << width << " h: " << height << endl;
}

int main(int argc, char* argv[])
{
	//! 1. 初始化glfw
	glfwInit();

	//! 2. 配置GLFW版本
	//! 主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//! 次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//! 使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//! 3. 创建窗口对象
	GLFWwindow* pWindow = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (!pWindow)
	{
		cout << __FUNCTION__ << " Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);

	//! 4. 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << __FUNCTION__ << " Failed to initialize GLAD" << endl;
		return -1;
	}

	//! 5. 设置视口
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

	//! 6. 准备好引擎，消息事件轮询
	while (!glfwWindowShouldClose(pWindow))
	{
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	//! 7. 输入
	

	//! 8. 释放资源
	glfwTerminate();

	return 0;
}



