#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transformation.h"
#include "camera.h"

#define TITLE  "3D Plane Viewer"

#define WIDTH  800
#define HEIGHT 600

#define MAX 0x400

char cwd[MAX];
char fullfilename[MAX];

const char *full_name(const char *filename)
{
    strcpy(fullfilename, cwd);
    strcat(fullfilename, filename);
    return fullfilename;
}
int
main ( int argc, char *argv[] )
{
    readlink("/proc/self/exe", cwd, MAX);

    *(rindex(cwd, '/') + 1) = '\0';
    Display display(WIDTH, HEIGHT, TITLE);
    Vertex vertices[] = {   Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0, 0.0)),
                            Vertex(glm::vec3(-0  ,  0.5, 0), glm::vec2(0.5, 1.0)),
                            Vertex(glm::vec3( 0.5, -0.5, 0), glm::vec2(1.0, 0.0))
                        };

    unsigned indices[] = { 0, 1, 2 };
    Mesh mesh(vertices, sizeof(vertices) / sizeof (vertices[0]), indices, sizeof(indices) / sizeof(indices[0]) );
    Mesh mesh2(full_name("res/models/monkey3.obj"));
    Shader shader(full_name("res/shaders/basicShader"));
    Texture texture(full_name("res/textures/bricks.jpg"));
    Camera camera(glm::vec3(0,0,-5), 70.0f, (float) WIDTH / HEIGHT, 0.01f, 100.0f);
    Transformation transform;

    float counter = 0.0f;
    float sincounter;
    float coscounter;

    while (!display.is_closed())
    {
        display.clear(0.0f, 0.15f, 0.3f, 1.0f);

        sincounter = sinf(counter);
        coscounter = cosf(counter);
        transform.get_pos().x = 2 * sincounter;
        transform.get_pos().z = 2 * coscounter;
        transform.get_rot().x = 5 * counter;
        transform.get_rot().y = 4 * counter;
        transform.get_rot().z = 3 * counter;

        shader.bind();
        texture.bind(0);
        shader.update(transform, camera);
        mesh2.draw();
        display.update();

        counter += 0.004f;
    }

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
