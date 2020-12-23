#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

enum class ShaderType 
{
    VERTEX,
    FRAGMENT,
};

void CheckCompilationError(ShaderType type, unsigned int shader)
{
    // check if any error in shader compilation
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::" 
            << (type == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") 
            << "::COMPILATION_FAILED\n" 
            << info_log 
            << std::endl;
    }
    else
    {
        std::cout << (type == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") 
            << " shader was compiled successfully." 
            << std::endl;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, 640, 480);

    float vertices[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    // Vertex Array Object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex Buffer Object
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Buffer Object
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // create and compile vertex shader
    const char* kVertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);"
        "}\0";
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &kVertexShaderSource, NULL);
    glCompileShader(vertex_shader);
    CheckCompilationError(ShaderType::VERTEX, vertex_shader);

    // create and compile fragment shader
    const char* kFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &kFragmentShaderSource, NULL);
    glCompileShader(fragment_shader);
    CheckCompilationError(ShaderType::FRAGMENT, fragment_shader);

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        // bind vertex array object
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        // front buffer contains the final output image that is shown at the screen
        // while all the rendering commands draw to the back buffer.
        // As soon as all the rendering commands are finished we swap the back buffer to the front buffer
        glfwSwapBuffers(window);

        // check if any events are triggered (keyboard input or mouse movement etc...)
        glfwPollEvents();
    }

    // de-allocate all resources 
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);

    // clear all previously allocated glfw sources
    glfwTerminate();
    return 0;
}