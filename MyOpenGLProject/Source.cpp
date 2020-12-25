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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    glViewport(0, 0, 640, 480);

    float vertices[] = {
         0.05f,  0.8f, 0.0f, // top right of central line
         0.05f, -0.5f, 0.0f, // bottom right of central line
        -0.05f, -0.5f, 0.0f, // bottom left of central line
        -0.05f,  0.8f, 0.0f, // top left of central line

         0.6f,  0.8f, 0.0f, // top right of upper line of F
         0.05f,  0.65f, 0.0f, // bottom left of upper line of F
         0.6f,  0.65f, 0.0f, // bottom right of upper line of F

         0.4f,  0.4f, 0.0f, // top right of lower line of F
         0.05f,  0.25f, 0.0f, // bottom left of lower line of F
         0.4f,  0.25f, 0.0f, // bottom right of lower line of F
         0.05f,  0.4f, 0.0f, // top left of lower line of F
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
        0, 4, 5,
        4, 5, 6,
        7, 8, 10,
        7, 8, 9,
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

    /*
    Swizzling:
    vec2 someVec;
    vec4 differentVec = someVec.xyxx;
    vec3 anotherVec = differentVec.zyw;
    vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

    vec2 vect = vec2(0.5, 0.7);
    vec4 result = vec4(vect, 0.0, 0.0);
    vec4 otherResult = vec4(result.xyz, 1.0);
    */

    // create and compile vertex shader
    const char* kVertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0)\n"
        "in vec3 aPos;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
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
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = ourColor;\n"
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

        float time_value = glfwGetTime();
        float green_value = (sin(time_value) / 2.0f) + 0.5f;
        int vertex_color_location = glGetUniformLocation(shader_program, "ourColor");
        glUseProgram(shader_program);
        glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);
        // bind vertex array object
        glBindVertexArray(vao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
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