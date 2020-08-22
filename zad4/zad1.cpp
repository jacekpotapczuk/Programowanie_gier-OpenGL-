#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

void imgui_display();
void display();
void reshape(int, int);
void timer(int);


static const char* items[]{ "perspektywiczna", "ortogonalna" };
static int selectedItem = 0;

float rotation_speed = 1.0f;
static float translation[] = { 0.0, 0.0, -8.0 };
static float rotation[] = { 0.0, 0.0, 0.0 };
static bool rotation_bool[] = { false, false, false};
static float scale[] = { 1.0, 1.0, 1.0 };

void imgui_display()
{
    {
        ImGui::Begin("Sterowanie");   

        ImGui::Text("Transformacje model-widok:");
        ImGui::SliderFloat3("translacja", translation, -10.0, 10.0);
        ImGui::SliderFloat3("rotacja", rotation, 0, 360.0);
        ImGui::SliderFloat3("skalowanie", scale, 0.1, 5.0);

        ImGui::Separator();
        ImGui::Text("Automatyczny obrot:");
        ImGui::Checkbox("x", &rotation_bool[0]);
        ImGui::SameLine();
        ImGui::Checkbox("y", &rotation_bool[1]);
        ImGui::SameLine();
        ImGui::Checkbox("z", &rotation_bool[2]);
        ImGui::SliderFloat("predkosc ", &rotation_speed, 0.5f, 5.0f);

        ImGui::Separator();
        ImGui::Text("Projektcja:");
        ImGui::ListBox("", &selectedItem, items, IM_ARRAYSIZE(items), 2);

        ImGui::Separator();
        ImGui::Text("Aktualnie: %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

void display()
{
    // Start ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    imgui_display();

    // ImGui rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();

    //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // frame buffer flag
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glLoadIdentity();  //  clears any transformations of current matrix (in this place its modelview) 

    glTranslatef(translation[0], translation[1], translation[2]);
    glRotatef(rotation[0], 1.0, 0.0, 0.0);
    glRotatef(rotation[1], 0.0, 1.0, 0.0);
    glRotatef(rotation[2], 0.0, 0.0, 1.0);
    glScalef(scale[0], scale[1], scale[2]);


    glBegin(GL_QUADS);
    //front
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    //back
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    //right
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    //left
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    //top
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    //bottom
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);

    glEnd();

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
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
    glutCreateWindow("Programowanie gier - zad 4 - kostka");
    glEnable(GL_DEPTH_TEST);

    // specify callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);  // call timer function immediately


    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();




    glutMainLoop();


    // Imgui cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}


void reshape(int w, int h) // called at the start and whenever window is reshaped
{
    ImGui_ImplGLUT_ReshapeFunc(w, h); // call imgui reshape callback function

    // viewport settings
    glViewport(0, 0, w, h);  // adjust viewport to always match the whole window

    // projection settings


}

void timer(int)
{
    glutPostRedisplay();  // call display function
    glutTimerFunc(1000 / 60, timer, 0);  // calls itself to keep 60 FPS

    for (int i = 0; i < 3; i++) {
        if (rotation_bool[i]) {
            rotation[i] += rotation_speed;
            if (rotation[i] > 360.0)
                rotation[i] -= 360.0;
        }
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (selectedItem == 0) {
        gluPerspective(60.0, 1.0, 0.1, 100.0);
    }
    else if(selectedItem == 1) {
        glOrtho(-4.0, 4.0, -4.0, 4.0, 0.1, 100.0);
    }
    else {
        std::cout << "Not supported selected item" << std::endl;
    }
    glMatrixMode(GL_MODELVIEW);

}

