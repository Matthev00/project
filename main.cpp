#include <iostream>
#include <fstream>
#include <GL/glut.h>

char* img1;
int width1, height1;

void readBMP(const char* filename, char* &pixels, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    char header[54];
    file.read(header, 54);

    width = *reinterpret_cast<int*>(&header[18]);
    height = *reinterpret_cast<int*>(&header[22]);
    int imgSize = width * height * 4;

    pixels = new char[imgSize];
    file.read(pixels, imgSize);

    //file.close();
}

extern "C" void alpha_blending(char* image1, char* image2, int x_value, int y_value,  int width1);

void renderScene() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(static_cast<GLfloat>(-1.0), static_cast<GLfloat>(-1.0));
    glDrawPixels(width1, height1, GL_BGR_EXT, GL_UNSIGNED_BYTE, img1);

    glFlush();
}

void mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Wywołanie funkcji alphaBlending po kliknięciu myszką
        char* image2;
        int width2, height2;
        readBMP("obiekt.bmp", image2, width2, height2);

        if (!image2) {
            std::cerr << "Error reading image." << std::endl;
            return;
        }

        alpha_blending(img1, image2, x, y, width1);

        delete[] image2;

        glutPostRedisplay(); // Odświeżenie sceny
    }
}

int main(int argc, char** argv) {
    // Wczytaj obraz i wykonaj alphaBlending
    readBMP("tło.bmp", img1, width1, height1);

    if (!img1) {
        std::cerr << "Error reading image." << std::endl;
        return 0;
    }

    // Inicjalizuj okno OpenGL i wyświetl obraz
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width1, height1);
    glutCreateWindow("Alpha Blending");
    glutDisplayFunc(renderScene);
    glutMouseFunc(mouse_click);
    glutMainLoop();

    // Zwolnij pamięć
    delete[] img1;

    return 0;
}