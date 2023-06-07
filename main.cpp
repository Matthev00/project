#include <iostream>
#include <fstream>
#include <GL/glut.h>

char* img1, img;
char *label1, *label2;
int width1, height1;

void readBMP(const char* filename, char* &pixels, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);

    char header[54];
    file.read(header, 54);

    width = *reinterpret_cast<int*>(&header[18]);
    height = *reinterpret_cast<int*>(&header[22]);
    int imgSize = width * height * 4;

    pixels = new char[imgSize];
    file.read(pixels, imgSize);

    //file.close();
}

extern "C" void alpha_blending(char* image1, char* image2, int x_value, int y_value,  int width, int size);

void makeWindow() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(static_cast<GLfloat>(-1.0), static_cast<GLfloat>(-1.0));
    glDrawPixels(width1, height1, GL_BGR_EXT, GL_UNSIGNED_BYTE, img1);

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Wywołanie funkcji alphaBlending po kliknięciu myszką
        char* image2;
        int width2, height2;
        std::cerr << "x = " << x << ", y = " << y << std::endl;
        readBMP(label1, img1, width1, height1);
        readBMP(label2, image2, width2, height2);
        alpha_blending(img1, image2, x, height1 - y, width1, width1*height1*3);

        delete[] image2;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    if (argc<2){
        std::cerr << "Missing arguments!" << std::endl;
        return 0;
    }
    label1 = argv[1];
    label2 = argv[2];
    readBMP(argv[1], img1, width1, height1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width1, height1);
    glutCreateWindow("Alpha Blending");
    glutDisplayFunc(makeWindow);
    glutMouseFunc(mouse);
    glutMainLoop();

    delete[] img1;

    return 0;
}