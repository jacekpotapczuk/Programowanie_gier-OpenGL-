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


static float camera_translation[] = { 0.0, 15.0, 12.0 };
static float camera_rotation[] = { 40.0, 0.0, 0.0 };
static bool rotation_bool[] = { false, false, false};


// poczatkowo nie wiedzialem ze w imgui mozna latwo zmienic kolor, dlatego swiatla mialy byc ustalone sztywno na czew, ziel, nieb,
// dlatego nazwy zmiennych pochodza od r,g,b, a nie sa nazwane ogolniej

GLfloat lightColor0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
float redSpotAngle = 30.0;
float redSpotExponent = 20.0;
float redSpotDirection[] = { 0.0, -1.0, 0.0 };
float redSpotPos[] = { 0.0, 7.0, 0.0, 1.0 };
bool redSpotEnabled = true;
bool redSpotAutoMove = true;
bool redSpotLockOnCenter = true;
float redT = 0.0;

GLfloat lightColor1[] = { 0.0f, 1.0f, 0.0f, 1.0f };
float greenSpotAngle = 30.0;
float greenSpotExponent = 20.0;
float greenSpotDirection[] = { 0.0, -1.0, 0.0 };
float greenSpotPos[] = { 0.0, 7.0, 0.0, 1.0 };
bool greenSpotEnabled = true;
bool greenSpotAutoMove = true;
bool greenSpotLockOnCenter = true;
float greenT = 0.5;

GLfloat lightColor2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
float blueSpotAngle = 30.0;
float blueSpotExponent = 20.0;
float blueSpotDirection[] = { 0.0, -1.0, 0.0 };
float blueSpotPos[] = { 0.0, 7.0, 0.0, 1.0 };
bool blueSpotEnabled = true;
bool blueSpotAutoMove = true;
bool blueSpotLockOnCenter = true;
float blueT = 1.66;


void imgui_display()
{
    {
        ImGui::Begin("Sterowanie");   

        ImGui::Text("Kamera:");
        ImGui::SliderFloat3("translacja", camera_translation, -40.0, 40.0);
        ImGui::SliderFloat3("rotacja", camera_rotation, 0, 360.0);

        ImGui::Separator();
        ImGui::Text("Swiatlo 1:");
        ImGui::Checkbox("wlacz 1", &redSpotEnabled);
        ImGui::ColorEdit3("kolor 1", lightColor0);
        ImGui::Checkbox("autmatyczne obracanie 1", &redSpotAutoMove);
        ImGui::Checkbox("skierowane na srodek 1", &redSpotLockOnCenter);
        ImGui::SliderFloat4("spotPosition 1", redSpotPos, -20.0, 20.0);
        ImGui::SliderFloat3("spotDirection 1", redSpotDirection, -1.0, 1.0);
        ImGui::SliderFloat("spotAngle 1", &redSpotAngle, 0.0, 90.0);
        ImGui::SliderFloat("spotExponent 1", &redSpotExponent, 0.0, 100.0);

        ImGui::Separator();
        ImGui::Text("Swiatlo 2:");
        ImGui::Checkbox("wlacz 2", &greenSpotEnabled);
        ImGui::ColorEdit3("kolor 2", lightColor1);
        ImGui::Checkbox("autmatyczne obracanie 2", &greenSpotAutoMove);
        ImGui::Checkbox("skierowane na srodek 2", &greenSpotLockOnCenter);
        ImGui::SliderFloat4("spotPosition 2", greenSpotPos, -20.0, 20.0);
        ImGui::SliderFloat3("spotDirection 2", greenSpotDirection, -1.0, 1.0);
        ImGui::SliderFloat("spotAngle 2", &greenSpotAngle, 0.0, 90.0);
        ImGui::SliderFloat("spotExponent 2", &greenSpotExponent, 0.0, 100.0);

        ImGui::Separator();
        ImGui::Text("Swiatlo 3:");
        ImGui::Checkbox("wlacz 3", &blueSpotEnabled);
        ImGui::ColorEdit3("kolor 3", lightColor2);
        ImGui::Checkbox("autmatyczne obracanie 3", &blueSpotAutoMove);
        ImGui::Checkbox("skierowane na srodek 3", &blueSpotLockOnCenter);
        ImGui::SliderFloat4("spotPosition 3", blueSpotPos, -20.0, 20.0);
        ImGui::SliderFloat3("spotDirection 3", blueSpotDirection, -1.0, 1.0);
        ImGui::SliderFloat("spotAngle 3", &blueSpotAngle, 0.0, 90.0);
        ImGui::SliderFloat("spotExponent 3", &blueSpotExponent, 0.0, 100.0);

        ImGui::Separator();
        ImGui::Text("Aktualnie: %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

void display()
{
    glEnable(GL_LIGHTING);
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


        // ambient light
    //GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //GLfloat lightColor0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //GLfloat lightPos0[] = { light_direction[0], light_direction[1], light_direction[2], 0.0f };
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);


    //  camera position and rotation
    glTranslatef(-camera_translation[0], -camera_translation[1], -camera_translation[2]);

    glRotatef(camera_rotation[0], 1.0, 0.0, 0.0);
    glRotatef(camera_rotation[1], 0.0, 1.0, 0.0);
    glRotatef(camera_rotation[2], 0.0, 0.0, 1.0);




    glTranslatef(0.0, -2.0, -20.0); // move every object away from camera

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, redSpotPos);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &redSpotAngle);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, redSpotDirection);
    glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, &redSpotExponent);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, greenSpotPos);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &greenSpotAngle);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, greenSpotDirection);
    glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, &greenSpotExponent);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
    glLightfv(GL_LIGHT2, GL_POSITION, blueSpotPos);
    glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, &blueSpotAngle);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, blueSpotDirection);
    glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, &blueSpotExponent);

    GLfloat materialColor3[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor3);

    float tileSize = 0.1;
    glBegin(GL_QUADS);
    for (int x = -200; x < 200; x++)
    {
        for (int z = -200; z < 200; z++)
        {
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x * tileSize, 0, z * tileSize);
            glVertex3f(x * tileSize, 0, z * tileSize + tileSize * 2);
            glVertex3f(x * tileSize + tileSize * 2, 0, z * tileSize + tileSize * 2);
            glVertex3f(x * tileSize + tileSize * 2, 0, z * tileSize);
        }
    }
    glEnd();

    GLfloat materialColor[] = { 0.6f, 0.2f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);

    glTranslatef(5.0, 0.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(3.0, 5.0, 100, 100);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glTranslatef(-5.0, 0.0, 0.0);

    GLfloat materialColor2[] = { 0.6f, 1.0f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor2);


    glTranslatef(-5.0, 1.0, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidTorus(1.0, 3.0, 100, 100);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glTranslatef(5.0, -1.0, 0.0);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor3);

    glDisable(GL_LIGHTING);

    if (redSpotEnabled) {
        glTranslatef(redSpotPos[0], redSpotPos[1], redSpotPos[2]);
        glColor3f(lightColor0[0], lightColor0[1], lightColor0[2]);
        glutSolidSphere(0.2, 20, 20);
        glTranslatef(-redSpotPos[0], -redSpotPos[1], -redSpotPos[2]);
    }

    if (greenSpotEnabled) {
        glTranslatef(greenSpotPos[0], greenSpotPos[1], greenSpotPos[2]);
        glColor3f(lightColor1[0], lightColor1[1], lightColor1[2]);
        glutSolidSphere(0.2, 20, 20);
        glTranslatef(-greenSpotPos[0], -greenSpotPos[1], -greenSpotPos[2]);
    }

    if (blueSpotEnabled) {
        glTranslatef(blueSpotPos[0], blueSpotPos[1], blueSpotPos[2]);
        glColor3f(lightColor2[0], lightColor2[1], lightColor2[2]);
        glutSolidSphere(0.2, 20, 20);
        glTranslatef(-blueSpotPos[0], -blueSpotPos[1], -blueSpotPos[2]);
    }
    glColor3f(1.0, 1.0, 1.0);

    glTranslatef(0.0, 2.0, 20.0); // not necesary, because nothing is drawn after


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
    glutCreateWindow("Programowanie gier - zad 6 - oswietlenie");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glDisable(GL_COLOR_MATERIAL);

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
}

void mouse_active_motion(int x, int y) {
    ImGui_ImplGLUT_MotionFunc(x, y); // imgui mouse callback function

}

void reshape(int w, int h) // called at the start and whenever window is reshaped
{
    ImGui_ImplGLUT_ReshapeFunc(w, h); // call imgui reshape callback function

    // viewport settings
    glViewport(0, 0, w, h);  // adjust viewport to always match the whole window

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int)
{
    glutPostRedisplay();  // call display function
    glutTimerFunc(1000 / 60, timer, 0);  // calls itself to keep 60 FPS


    if (!redSpotEnabled) {
        glDisable(GL_LIGHT0);
    }
    else {
        glEnable(GL_LIGHT0);
        if (redSpotAutoMove) {
            redSpotPos[0] = 10 * cosf(redT);
            redSpotPos[2] = 10 * sinf(redT);
            redT += 0.01;
        }
        if (redSpotLockOnCenter) {
            redSpotDirection[0] = -redSpotPos[0];
            redSpotDirection[1] = -redSpotPos[1];
            redSpotDirection[2] = -redSpotPos[2];

            float len = sqrtf(redSpotDirection[0] * redSpotDirection[0] + redSpotDirection[1] * redSpotDirection[1] + redSpotDirection[2] * redSpotDirection[2]);
            redSpotDirection[0] /= len;
            redSpotDirection[1] /= len;
            redSpotDirection[2] /= len;
        }
    }

    if (!greenSpotEnabled) {
        glDisable(GL_LIGHT1);
    } 
    else{
        glEnable(GL_LIGHT1);
        if (greenSpotAutoMove) {
            greenSpotPos[0] = 10 * cosf(greenT + 3.1415);
            greenSpotPos[2] = 10 * sinf(greenT + 3.1515);
            greenT += 0.01;
        }
        if (greenSpotLockOnCenter) {
            greenSpotDirection[0] = -greenSpotPos[0];
            greenSpotDirection[1] = -greenSpotPos[1];
            greenSpotDirection[2] = -greenSpotPos[2];

            float len2 = sqrtf(greenSpotDirection[0] * greenSpotDirection[0] + greenSpotDirection[1] * greenSpotDirection[1] + greenSpotDirection[2] * greenSpotDirection[2]);
            greenSpotDirection[0] /= len2;
            greenSpotDirection[1] /= len2;
            greenSpotDirection[2] /= len2;
        }
    }

    if (!blueSpotEnabled) {
        glDisable(GL_LIGHT2);
    }
    else {
        glEnable(GL_LIGHT2);
        if (blueSpotAutoMove) {
            blueSpotPos[0] = 10 * cosf(blueT);
            blueSpotPos[2] = 10 * sinf(blueT);
            blueT += 0.01;
        }
        if (blueSpotLockOnCenter) {
            blueSpotDirection[0] = -blueSpotPos[0];
            blueSpotDirection[1] = -blueSpotPos[1];
            blueSpotDirection[2] = -blueSpotPos[2];

            float len3 = sqrtf(blueSpotDirection[0] * blueSpotDirection[0] + blueSpotDirection[1] * blueSpotDirection[1] + blueSpotDirection[2] * blueSpotDirection[2]);
            blueSpotDirection[0] /= len3;
            blueSpotDirection[1] /= len3;
            blueSpotDirection[2] /= len3;
        }
    }

}

