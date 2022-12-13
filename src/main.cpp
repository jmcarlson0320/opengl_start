#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "load_shaders.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

const int SCALE = 4;
const int WIDTH = 320;
const int HEIGHT = 240;

GLuint load_passthrough_shaders()
{
    shader_info info[] = {
        {GL_VERTEX_SHADER, "shaders/default.vert"},
        {GL_FRAGMENT_SHADER, "shaders/default.frag"},
        {GL_NONE, NULL}
    };

    return load_shaders(info);
}

VertexArray loadVertexData()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    };

    GLuint indices[] = {
        0, 1, 2,
        3, 2, 0
    };

    VertexBuffer vertex_buffer;
    vertex_buffer.init(vertices, 32);

    BufferLayout buffer_layout;
    buffer_layout.addElem(FLOAT, 3);
    buffer_layout.addElem(FLOAT, 2);
    vertex_buffer.setLayout(buffer_layout);

    IndexBuffer index_buffer;
    index_buffer.init(indices, 6);

    VertexArray triangle;
    triangle.addVertexBuffer(vertex_buffer);
    triangle.addIndexBuffer(index_buffer);

    return triangle;
}

GLuint load_texture_data()
{
    GLuint image_data[WIDTH * HEIGHT];

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        image_data[i] = i * 1000;
    }

    image_data[HEIGHT / 2 * WIDTH + WIDTH / 2] = 0xffffffff;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

int main(int argc, char *argv[])
{
    if (!glfwInit()) {
        printf("could not init glfw\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WIDTH * SCALE, HEIGHT * SCALE, "test window", NULL, NULL);
    if (!window) {
        printf("could not create window\n");
        glfwTerminate();
        return 1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    /*glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/
    /*glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);*/

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("could not init glew\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    int nx, ny;
    glfwGetFramebufferSize(window, &nx, &ny);
    glViewport(0, 0, nx, ny);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLuint shaderID = load_passthrough_shaders();
    VertexArray triangle = loadVertexData();
    GLuint textureID = load_texture_data();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        triangle.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        triangle.unbind();

        glfwSwapBuffers(window);

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
