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
bool LoadBitmapTex(LPCWSTR szFileName, GLuint& texid);


static float camera_translation[] = { 0.0, 0.0, 0.0};
static float camera_rotation[] = { 0.0, 0.0, 0.0 };


static float cube_translation[] = { 0.0, 0.0, 0.0 };
static float cube_rotation[] = { 0.0, 0.0, 0.0 };
static bool cube_rotation_bool[] = { false, false, false };
static float cube_rotation_speed = 1.0;
static float cube_scale[] = { 1.0, 1.0, 1.0 };


GLuint cube_texture;
GLuint triangle_texture;

void imgui_display()
{
    {
        ImGui::Begin("Sterowanie");   

        ImGui::Text("Kamera:");
        ImGui::SliderFloat3("translacjaK", camera_translation, -40.0, 40.0);
        ImGui::SliderFloat3("rotacjaK", camera_rotation, 0, 360.0);

        ImGui::Separator();
        ImGui::Text("Kostka:");
        ImGui::SliderFloat3("translacjaC", cube_translation, -40.0, 40.0);
        ImGui::SliderFloat3("rotacjaC", cube_rotation, 0, 360.0);
        ImGui::SliderFloat3("cube_scaleC", cube_scale, 0.1, 5.0);
        ImGui::Checkbox("x", &cube_rotation_bool[0]);
        ImGui::SameLine();
        ImGui::Checkbox("y", &cube_rotation_bool[1]);
        ImGui::SameLine();
        ImGui::Checkbox("z", &cube_rotation_bool[2]);
        ImGui::SameLine();
        ImGui::SliderFloat("auto obrot ", &cube_rotation_speed, 0.5f, 5.0f);


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


    //  camera position and rotation
    glTranslatef(-camera_translation[0], -camera_translation[1], -camera_translation[2]);

    glRotatef(camera_rotation[0], 1.0, 0.0, 0.0);
    glRotatef(camera_rotation[1], 0.0, 1.0, 0.0);
    glRotatef(camera_rotation[2], 0.0, 0.0, 1.0);


    glTranslatef(0.0, 0.0, -20.0); // move every object away from camera

    glBindTexture(GL_TEXTURE_2D, triangle_texture);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.25, 0.75); glVertex3f(-8.0, 3.0, 0.0);
    glTexCoord2f(0.25, 0.25); glVertex3f(-8.0, 0.0, 0.0);
    glTexCoord2f(0.75, 0.25); glVertex3f(-5.0, 0.0, 0.0);
    glEnd();
    
    glTranslatef(cube_translation[0], cube_translation[1], cube_translation[2]);
    glRotatef(cube_rotation[0], 1.0, 0.0, 0.0);
    glRotatef(cube_rotation[1], 0.0, 1.0, 0.0);
    glRotatef(cube_rotation[2], 0.0, 0.0, 1.0);
    glScalef(cube_scale[0], cube_scale[1], cube_scale[2]);

    
    {
        glBindTexture(GL_TEXTURE_2D, cube_texture);
        glBegin(GL_QUADS);
        //front
        glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, 1.0);
        glTexCoord2f(0, 0.66667); glVertex3f(-1.0, -1.0, 1.0);
        glTexCoord2f(0.5, 0.66667); glVertex3f(1.0, -1.0, 1.0);
        glTexCoord2f(0.5, 1); glVertex3f(1.0, 1.0, 1.0);
        ////back
        glTexCoord2f(0, 0.33334); glVertex3f(1.0, 1.0, -1.0);
        glTexCoord2f(0, 0); glVertex3f(1.0, -1.0, -1.0);
        glTexCoord2f(0.5, 0); glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(0.5, 0.33334); glVertex3f(-1.0, 1.0, -1.0);
        //right
        glTexCoord2f(0.5, 1); glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(0.5, 0.66667); glVertex3f(1.0, -1.0, 1.0);
        glTexCoord2f(1.0, 0.66667); glVertex3f(1.0, -1.0, -1.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
        //left
        glTexCoord2f(0.0, 0.66667); glVertex3f(-1.0, 1.0, -1.0);
        glTexCoord2f(0.0, 0.33334); glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(0.5, 0.33334); glVertex3f(-1.0, -1.0, 1.0);
        glTexCoord2f(0.5, 0.66667); glVertex3f(-1.0, 1.0, 1.0);
        ////top
        glTexCoord2f(0.5, 0.66667); glVertex3f(-1.0, 1.0, -1.0);
        glTexCoord2f(0.5, 0.33334); glVertex3f(-1.0, 1.0, 1.0);
        glTexCoord2f(1.0, 0.33334); glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(1.0, 0.66667); glVertex3f(1.0, 1.0, -1.0);
        ////bottom
        glTexCoord2f(0.5, 0.33334); glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(0.5, 0.0); glVertex3f(-1.0, -1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
        glTexCoord2f(1.0, 0.33334); glVertex3f(1.0, -1.0, -1.0);

        glEnd();
    } 



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
    glutCreateWindow("Programowanie gier - zad 7 - tekstury");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    

    // specify callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_active_motion);
    glutTimerFunc(0, timer, 0);  // call timer function immediately


    std::string s = "kostka.bmp";
    std::wstring stemp = std::wstring(s.begin(), s.end());
    LPCWSTR sw = stemp.c_str();

    bool wczytano = LoadBitmapTex(sw, cube_texture);


    std::string s2 = "trojkat.bmp";
    std::wstring stemp2 = std::wstring(s2.begin(), s2.end());
    LPCWSTR sw2 = stemp2.c_str();
    bool wczytano2 = LoadBitmapTex(sw2, triangle_texture);


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


    for (int i = 0; i < 3; i++) {
        if (cube_rotation_bool[i]) {
            cube_rotation[i] += cube_rotation_speed;
            if (cube_rotation[i] > 360.0)
                cube_rotation[i] -= 360.0;
        }
    }
}


bool LoadBitmapTex(LPCWSTR szFileName, GLuint& texid)
{
    HBITMAP hBMP;//uchwyt do bitmapy
    BITMAP BMP;//struktura opisujaca parametry bitmapy, wingdi.h, tez jest tu np BITMAPINFOHEADER
    //wczytaj bitmape, parametr tworzacy sekcje DIB kolokwialnie "tablice z pixelami"
    hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

    if (!hBMP)
    {
        return FALSE;
    }
    //pobierz informacje o obiekcie bitmapy, zapisane dane do struktury
    GetObject(hBMP, sizeof(BMP), &BMP);
    //wygeneruj teksture
    glGenTextures(1, &texid);
    //ustaw pwyrownanie struktury na 4 (slowo)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //zbinduj jako teksture 2D
    glBindTexture(GL_TEXTURE_2D, texid);
    //W jaki sposb powinna skalowac sie tekstura
    //generalnie tekstury maja rozmiary potegi 2, patrz mipmapy
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (BMP.bmBitsPixel != 24)//musi byc bitmapa 24bit per pixel
    {
        DeleteObject(hBMP);
        return FALSE;
    }
    //Dolacz bitmape do tekstury
    glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
    //usun bitmape z pamieci
    DeleteObject(hBMP);
    return TRUE;
}

