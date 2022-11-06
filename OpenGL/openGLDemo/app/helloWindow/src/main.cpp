
/*
* @see https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/03%20Hello%20Window/
* @see https://learnopengl.com/Getting-started/Hello-Window
* @see https://learnopengl.com/code_viewer.php?code=getting-started/hellowindow2
*/

//#include <GL/glew.h>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

//! input
//! keyboard event input
static void key_callback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    std::cout << "key: " << key << std::endl;
    //! press ESC, close windows
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    //! create window
    GLFWwindow* pWindow = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (!pWindow)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //! viewport
    int nW = 0;
    int nH = 0;
    glfwGetFramebufferSize(pWindow, &nW, &nH);
    glViewport(0, 0, nW, nH);

    //! set keyboard event callback
    glfwSetKeyCallback(pWindow, key_callback);

    //! prepare engine
    while (!glfwWindowShouldClose(pWindow))
    {
        //! check event
        glfwPollEvents();

        //! render command
        glClearColor(0.8f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //! swap buffer
        glfwSwapBuffers(pWindow);
    }

    glfwTerminate();
    
    cout << "Hello window!" << endl;
    return 0;
}

