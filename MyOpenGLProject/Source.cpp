#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

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

    Shader my_shader("shader.vert", "shader.frag");

    // F
    float vertices[] = {
         0.05f,  0.8f, 0.0f,  1.0f, 0.0f, 0.0f, // top right of central line
         0.05f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right of central line
        -0.05f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left of central line
        -0.05f,  0.8f, 0.0f,  0.0f, 1.0f, 0.0f, // top left of central line

         0.6f,  0.8f, 0.0f,   0.0f, 1.0f, 0.0f, // top right of upper line of F
         0.05f, 0.65f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left of upper line of F
         0.6f,  0.65f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right of upper line of F

         0.4f,  0.4f,  0.0f,  1.0f, 0.0f, 0.0f, // top right of lower line of F
         0.05f, 0.25f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left of lower line of F
         0.4f,  0.25f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom right of lower line of F
         0.05f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f, // top left of lower line of F
    };

    // Triangle
    /*float vertices[] = {
         0.00f,  0.02f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.02f, -0.02f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.02f, -0.02f, 0.0f,  0.0f, 0.0f, 1.0f,
    };*/

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
        0, 4, 5,
        4, 5, 6,
        7, 8, 10,
        7, 8, 9,
    };

    /*unsigned int indices[] = {
        0, 1, 2
    };*/

    unsigned int vao, vbo, ebo;

    // Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex Buffer Object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Buffer Object
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        my_shader.Use();

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

    // clear all previously allocated glfw sources
    glfwTerminate();
    return 0;
}