#include <GL/glut.h>
#include <cmath>
// Nama: Kevin Adi Santoso
// NIM: 24060123130081
// LAB: C1
// Fungsi untuk menggambar lingkaran
void drawCircle(float radius, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
}

// Fungsi untuk menggambar roda
void drawWheel() {
    glPushMatrix();
    // Lingkaran luar (ban hitam)
    glColor3f(0.0f, 0.0f, 0.0f); 
    drawCircle(0.15f, 36);
    
    // Lingkaran dalam (pusat roda putih)
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(0.08f, 36);
    glPopMatrix();
}


// Fungsi untuk menggambar background
void drawBackground() {
    // Langit (biru muda)
    glColor3f(0.53f, 0.81f, 0.98f);
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(10.0f, 0.0f);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(-10.0f, 10.0f);
    glEnd();

    // Jalan (abu-abu)
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, -10.0f);
    glVertex2f(10.0f, -10.0f);
    glVertex2f(10.0f, 0.0f);
    glVertex2f(-10.0f, 0.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnd();

}
// Fungsi untuk menggambar mobil
void drawVan() {
    glPushMatrix();
    
    // Body mobil
    glColor3f(0.1f, 0.7f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, 0.0f);  
    glVertex2f(0.7f, 0.0f);  
    glVertex2f(0.7f, -0.4f);  
    glVertex2f(-0.7f, -0.4f); 
    glEnd();
    
    // Atap van
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, 0.0f);   
    glVertex2f(-0.6f, 0.2f);  
    glVertex2f(0.5f, 0.2f);   
    glVertex2f(0.7f, 0.0f);   
    glEnd();
    
    // Bagian bawah (abu-abu)
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, -0.4f);
    glVertex2f(0.7f, -0.4f);
    glVertex2f(0.7f, -0.5f);
    glVertex2f(-0.7f, -0.5f);
    glEnd();
    
    // Warna jendela
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Jendela kiri
    glBegin(GL_POLYGON);
    glVertex2f(-0.65f, 0.0f);
    glVertex2f(-0.57f, 0.15f);
    glVertex2f(-0.35f, 0.15f);
    glVertex2f(-0.35f, 0.0f);
    glEnd();
    
    // Jendela tengah
    glBegin(GL_POLYGON);
    glVertex2f(-0.3f, 0.0f);
    glVertex2f(-0.3f, 0.15f);
    glVertex2f(0.0f, 0.15f);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    
    // Jendela kanan
    glBegin(GL_POLYGON);
    glVertex2f(0.05f, 0.0f);
    glVertex2f(0.05f, 0.15f);
    glVertex2f(0.45f, 0.15f);
    glVertex2f(0.6f, 0.0f);
    glEnd();
    
    // Warna lampu depan/belakang
    glColor3f(1.0f, 0.843f, 0.0f);
    
    // Lampu depan
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.7f, -0.3f);
    glVertex2f(-0.7f, -0.4f);
    glVertex2f(-0.6f, -0.4f);
    glEnd();
    
    // Lampu belakang
    glBegin(GL_TRIANGLES);
    glVertex2f(0.7f, -0.3f);
    glVertex2f(0.7f, -0.4f);
    glVertex2f(0.6f, -0.4f);
    glEnd();
    
    // Warna handle
    glColor3f(0.0f, 0.0f, 0.0f);
    
    // Handle kiri
    glBegin(GL_QUADS);
    glVertex2f(-0.27f, -0.1f);
    glVertex2f(-0.17f, -0.1f);
    glVertex2f(-0.17f, -0.15f);
    glVertex2f(-0.27f, -0.15f);
    glEnd();
    
    // Handle kanan
    glBegin(GL_QUADS);
    glVertex2f(0.1f, -0.1f);
    glVertex2f(0.2f, -0.1f);
    glVertex2f(0.2f, -0.15f);
    glVertex2f(0.1f, -0.15f);
    glEnd();
    
    // Roda
    glPushMatrix();
    glTranslatef(-0.45f, -0.5f, 0.0f); 
    drawWheel();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.45f, -0.5f, 0.0f);  
    drawWheel();
    glPopMatrix();
    
    glPopMatrix();
}

// Fungsi untuk menggambar matahari
void drawSun() {
    glPushMatrix();
    glTranslatef(0.7f, 0.7f, 0.0f);
    
    // Matahari luar (kuning)
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(0.15f, 36);
    
    // Matahari dalam (oren)
    glColor3f(1.0f, 0.647f, 0.0f);
    drawCircle(0.1f, 36);
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Gambar Background
    drawBackground();
    
    // Gambar matahari
    drawSun();
    
    // Gambar van
    glPushMatrix();
    
    // Posisi mobil
    glTranslatef(0.0f, 0.2f, 0.0f);
    drawVan();
    glPopMatrix();
    
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tugas Bikin Mobil Kevin");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
