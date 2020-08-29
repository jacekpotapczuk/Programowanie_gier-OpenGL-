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
void key_handler(unsigned char, int, int);


GLuint shader;
bool auto_rotate = false;
static float rotation[] = { 0.0, 0.0, 0.0 };

int loc_rotate_x, loc_rotate_y, loc_rotate_z;


static float mouse_start_pos[] = { -1.0, -1.0 };
static float mouse_move_vector[] = { 0.0, 0.0 };
static float mouse_move_vector_norm[] = { 0.0, 0.0 };

bool left_mouse_button_pressed = false;
bool mouse_rotation_enabled = true;


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

    //glDrawElements(GL_TRIANGLES, 12 * 3 , GL_UNSIGNED_INT, nullptr);
    
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


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

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    static const GLfloat positions[] = {
    -1.0f,-1.0f,-1.0f, 
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f,-1.0f, 
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);


    ShaderProgramSource source = ParseShader("shaders/Basic.shader");

    shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    GLint posAttrib = glGetAttribLocation(shader, "position");
    //odblokowanie atrybutow
    glEnableVertexAttribArray(posAttrib);

    glVertexAttribPointer(  // definiuje jak ulozone sa dane w naszych wierzcholkach
        posAttrib,      // pozycja atrybutu
        3,              // rozmiar elementow dla kazdego wierzcholka  (ilosc elementow musi byc jedno z 1,2,3,4)
        GL_FLOAT,       // typ
        GL_FALSE,       // czy znormalizowany (blad? - wedlug dokumentacji na odwrot - czy dane maja byc znormalizowane)
        0,              // "stride" w przypadku gdy zero oznacza, że mamy ciągla upakowana tablice (ilosc bajtow na kazdy wierzcholek, zamiast 0 mozna uzyc np sizeof(float)*3)
        0               // wskaznik do pierwszego atrybutu, jeśli zero to od początku (ile bajtow musimy sie przesunac do wskazanego atrybutu)
    );

    float model_matrix[16] = { 2.0, 0.0, 0.0, 0.0,
                               0.0, 2.0, 0.0, 0.0,
                               0.0, 0.0, 1.0, 0.0,
                               -0.5, -0.5, -20.0, 1.0 };


    float view_matrix[16] = { 1.0, 0.0, 0.0, 0.0, // nie uzywam kamery, ale w razie potrzeby jest taka mozliwosc
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 };

    //// orthographic projection
    //float r = 10.0;   // right
    //float l = -10.0;  // left
    //float b = -10.0;  // bottom
    //float t = 10.0;   // top
    //float f = 100.0;  // far
    //float n = 0.1;    // near

    //float projection_matrix[16] = { 2.0 / (r - l), 0.0, 0.0, -(r + l)/(r - l),
    //                                0.0, 2.0 / (t - b), 0.0, -(t + b) / (t - b),
    //                                0.0, 0.0, -2.0 / (f - n), -(f + n) / (f - n),
    //                                0.0, 0.0, 0.0, 1.0 };

    // perspective projection
    float fov = 60.0;
    float n = 0.1;
    float f = 100.0;
    float s = 1.0 / (tanf((fov / 2.0) * (3.1415 / 180.0)));

    float projection_matrix[16] = { s, 0.0, 0.0, 0.0,
                                    0.0, s, 0.0, 0.0,
                                    0.0, 0.0, - (f)/(f - n), -1.0,
                                    0.0, 0.0, - (f*n)/(f - n), 0.0 };

    int loc_model = glGetUniformLocation(shader, "u_model_matrix");
    int loc_view = glGetUniformLocation(shader, "u_view_matrix");
    int loc_projection = glGetUniformLocation(shader, "u_projection_matrix");

    glUniformMatrix4fv(loc_model, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, view_matrix);
    glUniformMatrix4fv(loc_projection, 1, GL_FALSE, projection_matrix);


    loc_rotate_x = glGetUniformLocation(shader, "u_rotate_x_matrix");
    loc_rotate_y = glGetUniformLocation(shader, "u_rotate_y_matrix");
    loc_rotate_z = glGetUniformLocation(shader, "u_rotate_z_matrix");


    // specify callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_active_motion);
    glutKeyboardFunc(key_handler);
    glutTimerFunc(0, timer, 0);  // call timer function immediately

    glutMainLoop();


    glDeleteProgram(shader);

    //glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}

void key_handler(unsigned char key, int x, int y) {
    if (key == 'q') {
        if (auto_rotate)
            auto_rotate = false;
        else
            auto_rotate = true;
    }

}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouse_start_pos[0] = x;
        mouse_start_pos[1] = y;
        left_mouse_button_pressed = true;

    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        left_mouse_button_pressed = false;
    }
}

void mouse_active_motion(int x, int y) {
    if (mouse_rotation_enabled && left_mouse_button_pressed) {
        mouse_move_vector[0] = mouse_start_pos[0] - x;
        mouse_move_vector[1] = mouse_start_pos[1] - y;

        mouse_start_pos[0] = x;
        mouse_start_pos[1] = y;

        mouse_move_vector_norm[0] = mouse_move_vector[0] / 720.0;
        mouse_move_vector_norm[1] = mouse_move_vector[1] / 720.0;

        mouse_move_vector_norm[0] *= 2 * 3.1415;
        mouse_move_vector_norm[1] *= 2 * 3.1415;

        rotation[1] += -mouse_move_vector_norm[0];
        rotation[0] += mouse_move_vector_norm[1];
    }
}

void reshape(int w, int h) // called at the start and whenever window is reshaped
{
    glViewport(0, 0, w, h);  // adjust viewport to always match the whole window
}

void timer(int)
{
    glutPostRedisplay();  // call display function
    glutTimerFunc(1000 / 60, timer, 0);  // calls itself to keep 60 FPS
    if (auto_rotate) {
        rotation[0] += 0.001;
        rotation[1] += 0.02;
        rotation[2] -= 0.015;
    }

    float rotate_x_matrix[16] = { 1.0, 0.0, 0.0, 0.0,
                                  0.0, cosf(rotation[0]), -sinf(rotation[0]), 0.0,
                                  0.0, sinf(rotation[0]), cosf(rotation[0]), 0.0,
                                  0.0, 0.0, 0.0, 1.0 };

    float rotate_y_matrix[16] = { cosf(rotation[1]), 0.0, -sinf(rotation[1]), 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              sinf(rotation[1]), 0.0, cosf(rotation[1]), 0.0,
                              0.0, 0.0, 0.0, 1.0 };

    float rotate_z_matrix[16] = { cosf(rotation[2]), -sinf(rotation[2]), 0.0, 0.0,
                                  sinf(rotation[2]), cosf(rotation[2]), 0.0, 0.0,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0 };

    glUniformMatrix4fv(loc_rotate_x, 1, GL_FALSE, rotate_x_matrix);
    glUniformMatrix4fv(loc_rotate_y, 1, GL_FALSE, rotate_y_matrix);
    glUniformMatrix4fv(loc_rotate_z, 1, GL_FALSE, rotate_z_matrix);
}
