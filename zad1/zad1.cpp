#define GLEW_STATIC
#include <GL/glew.h>  // pierwsza,inaczej moga byc GLchar udefined etc.

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void display();
void reshape(int, int);
void timer(int);
void mouse(int, int, int, int);
void mouse_active_motion(int, int);
void DrawCube(GLfloat, GLfloat, GLfloat, GLfloat);


int color_loc;

GLuint shader;
float angle_x = 0.0;
float angle_y = 0.0;
float angle_z = 0.0;


static GLuint CompileShader(GLuint type, const std::string& source) {

    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);


    GLint isCompiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
        GLchar* errorLog = (GLchar*)malloc(maxLength * sizeof(GLchar));
        glGetShaderInfoLog(id, maxLength, &maxLength, errorLog);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << errorLog << std::endl;

        free(errorLog);
        glDeleteShader(id);
        getchar();
        return 0;
    }
    return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);


    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

void display()
{

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 3);

  /*  glDrawElements(GL_TRIANGLES, 12 * 3 , GL_UNSIGNED_INT, nullptr);
    */
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
    //DrawCube(0.0, 0.0, 0.0, 1.0);

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    // docs.GL - czytelna dokumentacja
    glutInit(&argc, argv);
    glutInitContextVersion(2, 1); // open gl version
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(500, 200);
    glutInitWindowSize(720, 720);
    glutCreateWindow("Programowanie gier - zad 8 - modern pipeline");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        printf("Err %d", err);
        return 0;
    }

    // todo: dowiedziec sie co to i po co to
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo; // vertex buffer - przechowuje dane o obiektach ktore rysujemy, jest to odnosnik do miejsca gdzie sa one przechowywane na GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //static const GLfloat positions[] = { 0.0f, 0.0f, 0.0f,
    //                                     1.0f, 0.0f, 0.0f,
    //                                     1.0f, 1.0f, 0.0f,
    //                                     0.0f, 1.0f, 0.0f,
    //                                     0.0f, 0.0f, -1.0f,
    //                                     1.0f, 0.0f, -1.0f,
    //                                     1.0f, 1.0f, -1.0f,
    //                                     0.0f, 1.0f, -1.0f };
    //glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    //GLuint indices[] = {
    //    0, 1, 2,
    //    0, 2, 3,
    //    3, 2, 6,
    //    3, 6, 7,
    //    1, 5, 6,
    //    1, 6, 2,
    //    5, 4, 7,
    //    5, 7, 6,
    //    0, 4, 3,
    //    4, 7, 3,
    //    1, 5, 7,
    //    1, 4, 0
    //};

    //GLuint ibo; // index buffer
    //glGenBuffers(1, &ibo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    static const GLfloat g_color_buffer_data[] = {
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f
    };

    //GLuint colorbuffer;
    //glGenBuffers(1, &colorbuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);


    ShaderProgramSource source = ParseShader("shaders/Basic.shader");

    // vertex shader - called for each vertex, used to tell where you want it to be on the screen, it's called before fragment shader
    // fragment shader - called for each pixel that needs to be rasterized (drawn on screen), primally it's used to determine color of each pixel

    shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    GLint posAttrib = glGetAttribLocation(shader, "position");
    GLint colorAttrib = glGetAttribLocation(shader, "vertexColor");
    //odblokowanie atrybutow
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(colorAttrib);

    glVertexAttribPointer(  // definiuje jak ulozone sa dane w naszych wierzcholkach
        posAttrib,      // pozycja atrybutu
        3,              // rozmiar elementow dla kazdego wierzcholka  (ilosc elementow musi byc jedno z 1,2,3,4)
        GL_FLOAT,       // typ
        GL_FALSE,       // czy znormalizowany (blad? - wedlug dokumentacji na odwrot - czy dane maja byc znormalizowane)
        0,              // "stride" w przypadku gdy zero oznacza, że mamy ciągla upakowana tablice (ilosc bajtow na kazdy wierzcholek, zamiast 0 mozna uzyc np sizeof(float)*3)
        0               // wskaznik do pierwszego atrybutu, jeśli zero to od początku (ile bajtow musimy sie przesunac do wskazanego atrybutu)
    );

    glVertexAttribPointer(
        colorAttrib,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    color_loc = glGetUniformLocation(shader, "u_color");
    glUniform4f(color_loc, 0.8, 0.2, 0.8, 1.0);


    float model_matrix[16] = { 4.0, 0.0, 0.0, 0.0,
                               0.0, 2.0, 0.0, 0.0,
                               0.0, 0.0, 1.0, 0.0,
                               -0.5, -0.5, -1.0, 1.0 };


    float view_matrix[16] = { 1.0, 0.0, 0.0, 0.0, // nie uzywam kamery, ale w razie potrzeby jest taka mozliwosc
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 };

    // orthographic projection
    float r = 10.0;   // right
    float l = -10.0;  // left
    float b = -10.0;  // bottom
    float t = 10.0;   // top
    float f = 100.0;  // far
    float n = 0.1;    // near

    float projection_matrix[16] = { 2.0 / (r - l), 0.0, 0.0, -(r + l)/(r - l),
                                    0.0, 2.0 / (t - b), 0.0, -(t + b) / (t - b),
                                    0.0, 0.0, -2.0 / (f - n), -(f + n) / (f - n),
                                    0.0, 0.0, 0.0, 1.0 };

    int loc_model = glGetUniformLocation(shader, "u_model_matrix");
    int loc_view = glGetUniformLocation(shader, "u_view_matrix");
    int loc_projection = glGetUniformLocation(shader, "u_projection_matrix");


    glUniformMatrix4fv(loc_model, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, view_matrix);
    glUniformMatrix4fv(loc_projection, 1, GL_FALSE, projection_matrix);


    // specify callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_active_motion);
    glutTimerFunc(0, timer, 0);  // call timer function immediately

    glutMainLoop();


    glDeleteProgram(shader);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}

void mouse(int button, int state, int x, int y) {
 
}

void mouse_active_motion(int x, int y) {
  
}

void reshape(int w, int h) // called at the start and whenever window is reshaped
{
    glViewport(0, 0, w, h);  // adjust viewport to always match the whole window
}

void timer(int)
{
    glutPostRedisplay();  // call display function
    glutTimerFunc(1000 / 60, timer, 0);  // calls itself to keep 60 FPS

    angle_x += 0.01;
    angle_y += 0.02;
    angle_z -= 0.015;

    float rotate_x_matrix[16] = { 1.0, 0.0, 0.0, 0.0,
                                  0.0, cosf(angle_x), -sinf(angle_x), 0.0,
                                  0.0, sinf(angle_x), cosf(angle_x), 0.0,
                                  0.0, 0.0, 0.0, 1.0 };

    float rotate_y_matrix[16] = { cosf(angle_y), 0.0, -sinf(angle_y), 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              sinf(angle_y), 0.0, cosf(angle_y), 0.0,
                              0.0, 0.0, 0.0, 1.0 };

    float rotate_z_matrix[16] = { cosf(angle_z), -sinf(angle_z), 0.0, 0.0,
                                  sinf(angle_z), cosf(angle_z), 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0 };

    int loc_rotate_x = glGetUniformLocation(shader, "u_rotate_x_matrix");
    glUniformMatrix4fv(loc_rotate_x, 1, GL_FALSE, rotate_x_matrix);

    int loc_rotate_y = glGetUniformLocation(shader, "u_rotate_y_matrix");
    glUniformMatrix4fv(loc_rotate_y, 1, GL_FALSE, rotate_y_matrix);

    int loc_rotate_z = glGetUniformLocation(shader, "u_rotate_z_matrix");
    glUniformMatrix4fv(loc_rotate_z, 1, GL_FALSE, rotate_z_matrix);

}


void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength)
{
    GLfloat halfSideLength = edgeLength * 0.5f;

    GLfloat vertices[] =
    {
        // front face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // back face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom left

        // left face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // right face
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // top face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom left

        // top face
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength  // bottom left
    };

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glColor3f( colour[0], colour[1], colour[2] );
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_QUADS, 0, 24);

    glDisableClientState(GL_VERTEX_ARRAY);
}