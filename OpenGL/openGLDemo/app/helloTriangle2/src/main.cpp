
/*
* @see https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/03%20Hello%20Window/
* @see https://learnopengl.com/Getting-started/Hello-Window
* @see https://learnopengl.com/code_viewer.php?code=getting-started/hellotriangle2
*/

//#include <GL/glew.h>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

//! window dimensions
constexpr GLuint WIDTH = 800, HEIGHT = 600;

//! Shaders
constexpr GLchar* vertexShaderSource = "#version 300 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


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

//! The MAIN function from here we start the application and run the game loop
int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
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

    //! set keyboard event callback
    glfwSetKeyCallback(pWindow, key_callback);

    //! viewport
    int nW = 0;
    int nH = 0;
    glfwGetFramebufferSize(pWindow, &nW, &nH);
    glViewport(0, 0, nW, nH);

    //! Build and compile our shader program
    //! Vertex shader
    GLuint vertextShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertextShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertextShader);

    //! Check for compile time errors
    GLint nRet;
    GLchar infoLog[512];
    glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &nRet);
    if (!nRet)
    {
        glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    //! Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //! Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &nRet);
    if (!nRet)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    //! Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertextShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //! Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &nRet);
    if (!nRet)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }
    glDeleteShader(vertextShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f  // Bottom Left
        - 0.5f, 0.5f, 0.0f   // Top Left
    };
    GLuint indices[] = {    // Note that we start from 0!
        0, 1, 3,    // First Triangle
        1, 2, 3     // Second Triangle
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //! Bind the vertex Array object first, then bind and set vertex buffer(s)
    //! and attribute the pointer(s).
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //! Note that this is allowed, the call to glVertexAttribPointer registered VBO 
    //! as the currently bound vertex buffer object so afterwards 
    //! we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //! Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), 
    //! remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindVertexArray(0);

    //! Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //! prepare engine
    while (!glfwWindowShouldClose(pWindow))
    {
        //! check event
        glfwPollEvents();

        //! render command
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //! swap buffer
        glfwSwapBuffers(pWindow);
    }

    glfwTerminate();
    
    cout << "Hello window!" << endl;
    return 0;
}

