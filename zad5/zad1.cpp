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
void mouse(int, int, int, int);
void mouse_active_motion(int, int);

static const char* items[]{ "perspektywiczna", "ortogonalna" };
static int selectedItem = 0;

float rotation_speed = 1.0f;
static float translation[] = { 0.0, 0.0, -8.0 };
static float rotation[] = { 0.0, 0.0, 0.0 };
static bool rotation_bool[] = { false, false, false};
static float scale[] = { 1.0, 1.0, 1.0 };


static float mouse_start_pos[] = { -1.0, -1.0 };
static float mouse_move_vector[] = { 0.0, 0.0 };
static float mouse_move_vector_norm[] = { 0.0, 0.0 };

bool left_mouse_button_pressed = false;
bool mouse_rotation_enabled = true;


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
        
        ImGui::Checkbox("rotacja mysza", &mouse_rotation_enabled);
        ImGui::Text("UWAGA: obrot mysza dziala uzywajac UI.");
        ImGui::Text("Przed uzyciem sliderow najlepiej wylaczyc rotacje mysza.");


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
    glutCreateWindow("Programowanie gier - zad 5 - obrot myszka");
    glEnable(GL_DEPTH_TEST);

    // specify callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_active_motion);
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

void mouse(int button, int state, int x, int y) {
    ImGui_ImplGLUT_MouseFunc(button, state, x, y);  // call imgui mouse callback function

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
    ImGui_ImplGLUT_MotionFunc(x, y); // imgui mouse callback function

    if (mouse_rotation_enabled && left_mouse_button_pressed) {
        mouse_move_vector[0] = mouse_start_pos[0] - x;
        mouse_move_vector[1] = mouse_start_pos[1] - y;

        mouse_start_pos[0] = x;
        mouse_start_pos[1] = y;

        // window size is 720x720, to normalize movement vector from 0 to 1 we first devide by 720
        // next i multiply by 360 because i want the drag for the whole screen be equal to full rotation
        // this is the same as division by 2, but I leave it this way to make it more clear
        // right now performence is not my biggest issue

        mouse_move_vector_norm[0] = mouse_move_vector[0] / 720.0;
        mouse_move_vector_norm[1] = mouse_move_vector[1] / 720.0;

        mouse_move_vector_norm[0] *= 360.0;
        mouse_move_vector_norm[1] *= 360.0;

        rotation[1] += -mouse_move_vector_norm[0];  
        rotation[0] += -mouse_move_vector_norm[1];
    }
}

void reshape(int w, int h) // called at the start and whenever window is reshaped
{
    ImGui_ImplGLUT_ReshapeFunc(w, h); // call imgui reshape callback function

    // viewport settings
    glViewport(0, 0, w, h);  // adjust viewport to always match the whole window

}

void timer(int)
{
    glutPostRedisplay();  // call display function
    glutTimerFunc(1000 / 60, timer, 0);  // calls itself to keep 60 FPS

    for (int i = 0; i < 3; i++) {
        if (rotation[i] > 360.0)
            rotation[i] -= 360.0;
        if (rotation_bool[i]) {
            rotation[i] += rotation_speed;
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

