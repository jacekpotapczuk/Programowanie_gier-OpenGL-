#define GLEW_STATIC
#include <GL/glew.h>  // pierwsza,inaczej moga byc GLchar udefined etc.

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include <iostream>

void display();
void reshape(int, int);
void timer(int);
void mouse(int, int, int, int);
void mouse_active_motion(int, int);


float loc_color_r;


void display()
{

    glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //std::cout << "dispay" << std::endl;

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    // Create GLUT window,
    glutInit(&argc, argv);
    glutInitContextVersion(2, 1); // open gl version
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(500, 200);
    glutInitWindowSize(720, 720);
    glutCreateWindow("Programowanie gier - zad 7 - tekstury");


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


    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    static const GLfloat triangle_vertex[] = {-1.0f, -1.0f, 0.0f,
                                              1.0f, -1.0f, 0.0f,
                                              0.0f, 1.0f, 0.0f,};
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex), triangle_vertex, GL_STATIC_DRAW);

    const GLchar* simply_vertex_shader =
        "#version 150\n"
        "in vec3 position;"
        "void main()"
        "{"
        " gl_Position = vec4(position,1);"
        "}";
    const GLchar* simply_fragment_shader =
        "#version 150 core\n"
        "out vec4 out_color;"
        "uniform float r_color;"
        "void main()"
        "{"
        " out_color = vec4(r_color,0.5,0.5, 1.0);"
        "}";

    // Utworz i skompiluja vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &simply_vertex_shader, NULL);
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        GLchar* errorLog = (GLchar*)malloc(maxLength * sizeof(GLchar));
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, errorLog);
        printf(errorLog);
        free(errorLog);
        glDeleteShader(vertexShader);
        getchar();
        return 0;
    }

    // Utworz i skompiluja fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &simply_fragment_shader, NULL);
    glCompileShader(fragmentShader);


    // podlinkowanie shaderow do programu shaderow (utworzonego)
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Dobindowanie zmiennej out_color dla shadera
    glBindFragDataLocation(shaderProgram, 0, "out_color");
    glLinkProgram(shaderProgram); // linkowanie
    glUseProgram(shaderProgram); // uzycie

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    //odblokowanie atrybutow
    glEnableVertexAttribArray(posAttrib);
    //ustawienie atrybutow
    glVertexAttribPointer(
        posAttrib, //pozycja
        3, //rozmiar elementow dla kazdego wierzcholka
        GL_FLOAT, //typ
        GL_FALSE,//czy znormalizowany
        0,//"stride" w przypadku gdy zero oznacza, że mamy ciągla upakowana tablice
        0//wskaznik do pierwszego atrybutu, jeśli zero to od początku (domyslnie)
    );

    loc_color_r = glGetUniformLocation(shaderProgram, "r_color");
    glUniform1f(loc_color_r, 5.0);

    // specify callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_active_motion);
    glutTimerFunc(0, timer, 0);  // call timer function immediately

    glutMainLoop();


    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
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
}
