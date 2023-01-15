#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "load_shaders.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "graphics.h"

const int SCALE = 3;
const int WIDTH = 320;
const int HEIGHT = 240;

void initial_setup()
{
}

void create_window(unsigned int width, unsigned int height, unsigned int scale)
{
}

GLuint load_passthrough_shaders()
{
    shader_info info[] = {
        {GL_VERTEX_SHADER, "shaders/default.vert"},
        {GL_FRAGMENT_SHADER, "shaders/default.frag"},
        {GL_NONE, NULL}
    };

    return load_shaders(info);
}

VertexArray generate_quad()
{
    GLfloat vertices[] = {
        // x      y     z     u     v
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexBuffer vertex_buffer;
    vertex_buffer.init(vertices, 32);

    BufferLayout buffer_layout;
    buffer_layout.addElem(FLOAT, 3);
    buffer_layout.addElem(FLOAT, 2);
    vertex_buffer.setLayout(buffer_layout);

    IndexBuffer index_buffer;
    index_buffer.init(indices, 6);

    VertexArray quad;
    quad.addVertexBuffer(vertex_buffer);
    quad.addIndexBuffer(index_buffer);

    return quad;
}

GLuint load_texture_data()
{
    int image_data[WIDTH * HEIGHT];

    Bitmap canvas = bitmap_create(WIDTH, HEIGHT, image_data);
    graphics_output_set(&canvas);
    draw_fill_rect(0, 0, WIDTH, HEIGHT, 0x000000ff);
    draw_circle(WIDTH / 2, HEIGHT / 2, 5, 0x0f0fafff);
    draw_fill_rect(34, 67, 67, 80, 0x544653ff);
    draw_line(100, 0, 200, 200, 0xff1f04ff);

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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
    VertexArray quad = generate_quad();
    GLuint textureID = load_texture_data();
    glActiveTexture(GL_TEXTURE0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        quad.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        quad.unbind();

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
