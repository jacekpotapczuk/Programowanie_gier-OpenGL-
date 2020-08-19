#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <ball.h>

#include <iostream>

void imgui_display();
void display();
void reshape(int, int);
void timer(int);

std::vector<Ball> balls;

void imgui_display()
{
    {
        ImGui::Begin("Info");     
        ImGui::Text("Kulki:");           
            
        for (int i = 0; i < balls.size(); i++) {
            ImGui::Text(balls[i].name.c_str()); ImGui::SameLine();
            ImGui::Text(": "); ImGui::SameLine();
            ImGui::Text("m: %.2f", balls[i].m); ImGui::SameLine();
            ImGui::Text("r: %.2f", balls[i].r); ImGui::SameLine();
            ImGui::Text(" vx: %.2f", balls[i].xv); ImGui::SameLine();
            ImGui::Text(" vy: %.2f", balls[i].yv); 
        }
        ImGui::Text("Kolor kulki oznacza mase.");
        ImGui::Text("Czarna kulka = max masa, biala kulka = min masa.");
        ImGui::Text("m - masa, r - promien");
        ImGui::Text("xv - predkosc x, yv - predkosc y");

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

    glClear(GL_COLOR_BUFFER_BIT);  // frame buffer flag
    glClearColor(0.2, 0.75, 0.92, 1.0);

    glLoadIdentity();  //  clears any transformations of current matrix (in this place its modelview) 

    
    for (int i = 0; i < balls.size(); i++) {
        balls[i].draw();
    }

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    // Create GLUT window,
    glutInit(&argc, argv);
    glutInitContextVersion(2, 1); // open gl version
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);  
    glutInitWindowPosition(500, 200);
    glutInitWindowSize(720, 720);
    glutCreateWindow("Programowanie gier - zad 3 - kulki");


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

    // setup starting balls
    balls.push_back(Ball("Ball 0", 0.0, 0.0, 1.0, 6.5, -0.015, 0.05));
    balls.push_back(Ball("Ball 1", -5.0, -5.0, 2.1, 2.5, 0.35, 0.35));
    balls.push_back(Ball("Ball 2", 5.0, -5.0, 0.4, 1.0, 0.0, -0.55));
    balls.push_back(Ball("Ball 3", 5.0, 5.0, 2.5, 9.99, 0.0, 0.0));

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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW); // always go back to model view matrix after making changes to other ones


}

void timer(int)
{
    glutPostRedisplay();  // call display function
    glutTimerFunc(1000 / 60, timer, 0);  // calls itself to keep 60 FPS

    for (int i = 0; i < balls.size(); i++) {
        balls[i].update(-10.0, 10.0, -10.0, 10.0);
    }

    for (int i = 0; i < balls.size() - 1; i++) {
        for (int j = i + 1; j < balls.size(); j++) {
            Ball::resolve_potential_collision(&balls[i], &balls[j]);
        }
    }
}

