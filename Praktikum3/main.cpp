#include <GL/glut.h>

float rotateX = 0.0f;
float rotateY = 0.0f;

// KONTROL: 
// W = Sudut++ 
// S = Sudut--
// arrow keys = kamera
// r/R = reset
// 1-5 = pilih jari
// ESC = keluar

// sudut sendi jari [jari][sendi]
// jari: 0=jempol, 1=telunjuk, 2=tengah, 3=manis, 4=kelingking
// joints: 0=bawah, 1=tengah, 2=ujung
float sudutJari[5][3] = {
    {0.0f, 0.0f, 0.0f},  // jempol
    {0.0f, 0.0f, 0.0f},  // telunjuk
    {0.0f, 0.0f, 0.0f},  // tengah
    {0.0f, 0.0f, 0.0f},  // manis
    {0.0f, 0.0f, 0.0f}   // kelingking
};

// jari dan sendi sekarang
int currentFinger = 0;
int currentJoint = 0;

void drawTelapak() {
    glPushMatrix();
    glScalef(1.5f, 0.5f, 2.0f);
    glutWireCube(1.0);
    glPopMatrix();
}

void drawJari(int indexJari, float xPos, float panjang) {
    glPushMatrix();
    glTranslatef(xPos, 0.0f, 1.0f);
    
    // bawah
    glPushMatrix();
    glRotatef(sudutJari[indexJari][0], 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, panjang/2);
    glPushMatrix();
    glScalef(0.3f, 0.3f, panjang);
    glutWireCube(1.0);
    glPopMatrix();
    
    // tengah
    glTranslatef(0.0f, 0.0f, panjang/2);
    glRotatef(sudutJari[indexJari][1], 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.4f);
    glPushMatrix();
    glScalef(0.3f, 0.3f, 0.8f);
    glutWireCube(1.0);
    glPopMatrix();
    
    // ujung
    glTranslatef(0.0f, 0.0f, 0.4f);
    glRotatef(sudutJari[indexJari][2], 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.3f);
    glPushMatrix();
    glScalef(0.3f, 0.3f, 0.6f);
    glutWireCube(1.0);
    glPopMatrix();
    
    glPopMatrix();
    glPopMatrix();
}

void drawJempol() {
    glPushMatrix();
    glTranslatef(-0.8f, 0.0f, 0.5f);
    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
    
    // bawah
    glRotatef(sudutJari[0][0], 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.4f);
    glPushMatrix();
    glScalef(0.3f, 0.3f, 0.8f);
    glutWireCube(1.0);
    glPopMatrix();
    
    // ujung
    glTranslatef(0.0f, 0.0f, 0.4f);
    glRotatef(sudutJari[0][1], 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.4f);
    glPushMatrix();
    glScalef(0.3f, 0.3f, 0.8f);
    glutWireCube(1.0);
    glPopMatrix();
    
    glPopMatrix();
}

void drawPergelangan() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glScalef(1.2f, 0.5f, 1.0f);
    glutWireCube(1.0);
    glPopMatrix();
}

void drawLengan() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -3.5f);
    glScalef(1.0f, 0.8f, 4.0f);
    glutWireCube(1.0);
    glPopMatrix();
}

// semua
void drawHand() {
    drawTelapak();
    
    drawJari(4, -0.6f, 1.0f); 
    drawJari(3, -0.2f, 1.1f);  
    drawJari(2, 0.2f, 1.2f);  
    drawJari(1, 0.6f, 1.1f);   
    
    drawJempol();
    
    drawPergelangan();
    
    drawLengan();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // kamera
    gluLookAt(0.0f, 3.0f, 12.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    
    drawHand();
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width/height, 0.1f, 100.0f);
}

// keyboard
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case '1': case '2': case '3': case '4': case '5':
            currentFinger = key - '1';
            break;
        case '\t': // tab key GANTI SENDI
            currentJoint = (currentJoint + 1) % 3;
            if (currentFinger == 0 && currentJoint > 1) currentJoint = 0;
            break;
        case 'w': case 'W':
            // sudut++
            if (sudutJari[currentFinger][currentJoint] < 90.0f)
                sudutJari[currentFinger][currentJoint] += 5.0f;
            break;
        case 's': case 'S':
            // sudut--
            if (sudutJari[currentFinger][currentJoint] > 0.0f)
                sudutJari[currentFinger][currentJoint] -= 5.0f;
            break;
        case 'r': case 'R': // reset
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 3; j++) {
                    sudutJari[i][j] = 0.0f;
                }
            }
            break;
        case 27:  // ESC
            exit(0);
            break;
    }
    glutPostRedisplay();
}

// key spesial utk kamera
void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            rotateX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            rotateX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            rotateY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            rotateY += 5.0f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tangannya Kevin");
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();
    return 0;
}
