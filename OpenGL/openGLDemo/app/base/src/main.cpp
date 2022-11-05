
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
	//! 1. ��ʼ��glfw
	glfwInit();

	//! 2. ����GLFW�汾
	//! ���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//! �ΰ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//! ʹ�ú���ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//! 3. �������ڶ���
	GLFWwindow* pWindow = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (!pWindow)
	{
		cout << __FUNCTION__ << " Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);

	//! 4. ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << __FUNCTION__ << " Failed to initialize GLAD" << endl;
		return -1;
	}

	//! 5. �����ӿ�
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

	//! 6. ׼�������棬��Ϣ�¼���ѯ
	while (!glfwWindowShouldClose(pWindow))
	{
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	//! 7. ����
	

	//! 8. �ͷ���Դ
	glfwTerminate();

	return 0;
}



