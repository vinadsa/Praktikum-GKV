#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

// inisialisasi global var
// var tilting objek
double rx = 0.0;
double ry = 0.0;
double rz = 0.0;

// mode proyeksi
int projectionMode = 0; // 0 = perspektif, 1 = ortografik
int viewMode = 0;

// koordinat sumber cahaya
float l[] = { 0.0, 80.0, 0.0 }; 
float n[] = { 0.0, -40.0, 0.0 };
float e[] = { 0.0, -60.0, 0.0 };

// posisi kamera dan arah pandang
float camera_pos[] = {0.0, 0.0, -150.0};
float camera_front[] = {0.0, 0.0, 1.0};
float camera_up[] = {0.0, 1.0, 0.0};
float camera_right[] = {1.0, 0.0, 0.0};
float yaw = 90.0f;   // menghadap ke arah sumbu Z positif
float pitch = 0.0f;
float camera_speed = 5.0f;

void help();

// set material properties untuk bagian yang berbeda
void setMaterial(float r, float g, float b, float shininess)
{
    float ambient[] = { r * 0.2f, g * 0.2f, b * 0.2f, 1.0f };
    float diffuse[] = { r, g, b, 1.0f };
    float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

// objek pistol yang akan digambar
void draw()
{
    glPushMatrix();
    
    // scaling dan positioning
    glScalef(5.0, 5.0, 5.0);
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef(90, 0, 1, 0);
    
    // body pistol
    glPushMatrix();
    setMaterial(0.0, 0.5, 1.0, 60.0); 
    glScalef(3.0, 1.0, 0.7);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // barrel pistol
    glPushMatrix();
    setMaterial(0.8, 0.8, 0.9, 100.0); 
    glTranslatef(5.5, 0.3, 0.0); 
    glScalef(3.5, 0.4, 0.4);   
    glutSolidCube(2.0);
    glPopMatrix();
    
    // grip pistol
    glPushMatrix();
    setMaterial(1.0, 0.2, 0.2, 10.0); 
    glTranslatef(-1.0, -2.0, 0.0);
    glRotatef(-10, 0, 0, 1);
    glScalef(0.8, 2.0, 0.6);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // guard pistol
    glPushMatrix();
    setMaterial(1.0, 0.84, 0.0, 80.0); 
    glTranslatef(0.0, -1.5, 0.0);
    glScalef(0.8, 0.6, 0.2);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // slide pistol
    glPushMatrix();
    setMaterial(0.6, 0.2, 0.8, 70.0); 
    glTranslatef(2.0, 0.8, 0.0);
    glScalef(2.5, 0.3, 0.6);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // iron sight 
    glPushMatrix();
    setMaterial(0.0, 1.0, 0.4, 90.0); 
    glTranslatef(4.0, 1.2, 0.0);
    glScalef(0.2, 0.3, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    setMaterial(1.0, 0.4, 0.7, 50.0); 
    glTranslatef(1.0, -0.5, 0.0);
    glScalef(0.3, 0.3, 0.8);
    glutSolidTorus(0.5, 1.0, 20, 20);
    glPopMatrix();
    
    glPopMatrix();
}

// update vektor camera_front berdasarkan nilai pitch dan yaw
void updateCameraVectors()
{
    // derajat ke radian
    float yaw_rad = yaw * 3.14159 / 180.0;
    float pitch_rad = pitch * 3.14159 / 180.0;
    
    // vektor arah kamera (camera_front)
    camera_front[0] = cos(yaw_rad) * cos(pitch_rad);
    camera_front[1] = sin(pitch_rad);
    camera_front[2] = sin(yaw_rad) * cos(pitch_rad);
    
    // normalisasi vektor
    float length = sqrt(camera_front[0]*camera_front[0] + 
                        camera_front[1]*camera_front[1] + 
                        camera_front[2]*camera_front[2]);
    camera_front[0] /= length;
    camera_front[1] /= length;
    camera_front[2] /= length;
    
    // vektor kanan (camera_right)
    camera_right[0] = -camera_front[2]; 
    camera_right[1] = 0.0;
    camera_right[2] = camera_front[0];
    
    // normalisasi vektor
    length = sqrt(camera_right[0]*camera_right[0] + 
                 camera_right[1]*camera_right[1] + 
                 camera_right[2]*camera_right[2]);
    if (length > 0.0001) {
        camera_right[0] /= length;
        camera_right[1] /= length;
        camera_right[2] /= length;
    }
    
    // vektor atas
    camera_up[0] = camera_right[1]*camera_front[2] - camera_right[2]*camera_front[1];
    camera_up[1] = camera_right[2]*camera_front[0] - camera_right[0]*camera_front[2];
    camera_up[2] = camera_right[0]*camera_front[1] - camera_right[1]*camera_front[0];
    
    // normalisasi vektor
    length = sqrt(camera_up[0]*camera_up[0] + 
                 camera_up[1]*camera_up[1] + 
                 camera_up[2]*camera_up[2]);
    if (length > 0.0001) {
        camera_up[0] /= length;
        camera_up[1] /= length;
        camera_up[2] /= length;
    }
}

// membuat proyeksi bayangan
void glShadowProjection(float * l, float * e, float * n)
{
    float d, c;
    float mat[16];
    d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
    c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;
    mat[0] = l[0]*n[0]+c; // membuat matrik. OpenGLmenggunakan kolom matrik
    mat[4] = n[1]*l[0];
    mat[8] = n[2]*l[0];
    mat[12] = -l[0]*c-l[0]*d;
    mat[1] = n[0]*l[1];
    mat[5] = l[1]*n[1]+c;
    mat[9] = n[2]*l[1];
    mat[13] = -l[1]*c-l[1]*d;
    mat[2] = n[0]*l[2];
    mat[6] = n[1]*l[2];
    mat[10] = l[2]*n[2]+c;
    mat[14] = -l[2]*c-l[2]*d;
    mat[3] = n[0];
    mat[7] = n[1];
    mat[11] = n[2];
    mat[15] = -d;
    glMultMatrixf(mat); // kalikan matrik
}

void render()
{
    glClearColor(0.0,0.6,0.9,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // titik tengah objek (target)
    float target_x = 0.0;
    float target_y = 0.0;
    float target_z = 0.0;
    
    if (viewMode == 0) { // mode free cam
        float center[3];
        center[0] = camera_pos[0] + camera_front[0];
        center[1] = camera_pos[1] + camera_front[1];
        center[2] = camera_pos[2] + camera_front[2];
        
        gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2],
                  center[0], center[1], center[2],
                  0.0, 1.0, 0.0);
    } else {
        // mode proyeksi ortogonal, selalu melihat ke pusat objek
        gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2],
                  target_x, target_y, target_z,
                  0.0, 1.0, 0.0);
    }
    
    // pencahayaan
    float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, l);
    
    // gambar titik sumber cahaya
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POINTS);
    glVertex3f(l[0],l[1],l[2]);
    glEnd();
    
    // gambar lantai
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glNormal3f(0.0,1.0,0.0);
    glVertex3f(-1300.0,e[1]-0.1, 1300.0);
    glVertex3f( 1300.0,e[1]-0.1, 1300.0);
    glVertex3f( 1300.0,e[1]-0.1,-1300.0);
    glVertex3f(-1300.0,e[1]-0.1,-1300.0);
    glEnd();
    
    // gambar objek
    glPushMatrix();
    glRotatef(ry,0,1,0);
    glRotatef(rx,1,0,0);
    glRotatef(rz,0,0,1);  
    glEnable(GL_LIGHTING);
    draw();
    glPopMatrix();
    
    // gambar bayangan yang muncul
    glPushMatrix();
    glShadowProjection(l,e,n);
    glRotatef(ry,0,1,0);
    glRotatef(rx,1,0,0);
    glRotatef(rz,0,0,1);  
    glDisable(GL_LIGHTING);
    glColor3f(0.4,0.4,0.4);
    draw();
    glPopMatrix();
    
    glutSwapBuffers();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (projectionMode == 0) {
        // perspektif
        gluPerspective(60.0f, (float)w/h, 1.0, 2000.0);
    } else {
        // orthographic
        float aspect = (float)w/h;
        float size = 100.0f;
        glOrtho(-size * aspect, size * aspect, -size, size, 1.0, 2000.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

void setOrthographicView(int view)
{
    // ganti ke orthographic
    projectionMode = 1;
    viewMode = 1;
    
    // set titik tengah objek (target)
    float target_x = 0.0;
    float target_y = 0.0;
    float target_z = 0.0;
    
    float distance = 200.0; // jarak kamera dari pusat objek
    
    // switch case posisi kamera
    switch (view) {
        case 1: // top/plan view
            camera_pos[0] = target_x;
            camera_pos[1] = target_y + distance;
            camera_pos[2] = target_z;
            yaw = -90.0f;
            pitch = -90.0f;
            break;
        case 2: // side view
            camera_pos[0] = target_x + distance;
            camera_pos[1] = target_y;
            camera_pos[2] = target_z;
            yaw = 180.0f;
            pitch = 0.0f;
            break;
        case 3: // front view
            camera_pos[0] = target_x;
            camera_pos[1] = target_y;
            camera_pos[2] = target_z + distance;
            yaw = -90.0f;
            pitch = 0.0f;
            break;
        case 4: // isometric view
            // sudut isometric: 45° terhadap sumbu y, 35.264° terhadap bidang horizontal
            camera_pos[0] = target_x + distance * 0.577; // cos(45°) * cos(35.264°) ~ 0.577
            camera_pos[1] = target_y + distance * 0.577; // sin(35.264°) ~ 0.577
            camera_pos[2] = target_z + distance * 0.577; // sin(45°) * cos(35.264°) ~ 0.577
            yaw = -45.0f;
            pitch = -35.264f; // arctan(1/sqrt(2))
            break;
        case 5: // dimetric view
            // pakai sudut: 70° terhadap sumbu x, 20° terhadap bidang horizontal
            camera_pos[0] = target_x + distance * 0.342; // cos(70°) * cos(20°) ~ 0.342
            camera_pos[1] = target_y + distance * 0.342; // sin(20°) ~ 0.342
            camera_pos[2] = target_z + distance * 0.883; // sin(70°) * cos(20°) ~ 0.883
            yaw = -33.7f;
            pitch = -20.0f;
            break;
        case 6: // trimetric view
            // sudut dipakai bisa bebas, misalnya: 60° terhadap sumbu x, 25° terhadap bidang horizontal
            camera_pos[0] = target_x + distance * 0.454; // cos(60°) * cos(25°) ~ 0.454
            camera_pos[1] = target_y + distance * 0.423; // sin(25°) ~ 0.423
            camera_pos[2] = target_z + distance * 0.784; // sin(60°) * cos(25°) ~ 0.784
            yaw = -47.0f;
            pitch = -25.0f;
            break;
    }
    
    // update kamera
    updateCameraVectors();
    
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    resize(w, h);
}

void setPerspectiveView()
{
    // ganti ke perspektif
    projectionMode = 0;
    viewMode = 0;
    
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    resize(w, h);
}

// panduan
void help() // OUTPUTNYA DI TERMINAL/KONSOL, DI DEV C++ GA BISA LANGSUNG KELIHATAN, PAKAI IDE LAIN ATAU SETTING DLU
{
    printf("==== Kontrol Perspektif Pistol ====\n");
    printf("Kontrol proyeksi:\n");
    printf("  0: Proyeksi perspektif (default)\n");
    printf("  1: Proyeksi orthogonal - Top/Plan view\n");
    printf("  2: Proyeksi orthogonal - Side view\n");
    printf("  3: Proyeksi orthogonal - Front view\n");
    printf("  4: Proyeksi orthogonal - Isometric view\n");
    printf("  5: Proyeksi orthogonal - Dimetric view\n");
    printf("  6: Proyeksi orthogonal - Trimetric view\n\n");
    printf("Kontrol kamera (hanya dalam mode perspektif):\n");
    printf("  Arrow keys: maju/mundur dan strafe kiri/kanan\n");
    printf("  i/k: rotasi kamera ke atas/bawah\n");
    printf("  j/l: rotasi kamera ke kiri/kanan\n");
    printf("  p/;: kamera naik/turun\n\n");
    printf("Rotasi objek:\n");
    printf("  w/s: rotasi objek ke atas/bawah\n");
    printf("  a/d: rotasi objek ke kiri/kanan\n");
    printf("  q/e: memiringkan objek (rotasi sumbu Z)\n\n");
    printf("Posisi cahaya:\n");
    printf("  u/o: geser cahaya ke depan/belakang\n\n");
    printf("h: menampilkan panduan\n");
    printf("ESC: keluar program\n");
}

// kontrol tombol keyboard
void keypress(unsigned char c, int a, int b)
{
    if (c == 27) exit(0);
    
    // Orthographic projection controls
    else if (c == '0') setPerspectiveView();       // perspective view
    else if (c == '1') setOrthographicView(1);     // top/plan view
    else if (c == '2') setOrthographicView(2);     // side view
    else if (c == '3') setOrthographicView(3);     // front view
    else if (c == '4') setOrthographicView(4);     // isometric view
    else if (c == '5') setOrthographicView(5);     // dimetric view
    else if (c == '6') setOrthographicView(6);     // trimetric view
    
    if (viewMode == 0) { // hanya bisa di free cam 
        if (c == 'i') pitch += 5.0;
        else if (c == 'k') pitch -= 5.0;
        else if (c == 'j') yaw -= 5.0;
        else if (c == 'l') yaw += 5.0;
        else if (c == 'p') camera_pos[1] += camera_speed;
        else if (c == ';') camera_pos[1] -= camera_speed;
        
        if (pitch > 89.0) pitch = 89.0; // supaya ga terbalik
        if (pitch < -89.0) pitch = -89.0;
         
        // update kamera
        updateCameraVectors();
    }
    
    // kontrol default
    if (c == 'u') l[2] -= 5.0;
    else if (c == 'o') l[2] += 5.0;
    else if (c == 'w') rx -= 5.0;
    else if (c == 's') rx += 5.0;
    else if (c == 'a') ry -= 5.0;
    else if (c == 'd') ry += 5.0;
    else if (c == 'q') rz -= 5.0;
    else if (c == 'e') rz += 5.0;
    else if (c == 'h') help(); // OUTPUTNYA DI TERMINAL/KONSOL, DI DEV C++ GA BISA LANGSUNG KELIHATAN, PAKAI IDE LAIN ATAU SETTING DLU
    
    glutPostRedisplay();
}

// kontrol tombol khusus (arrow keys)
void specialKeypress(int key, int x, int y)
{
    // hanya bisa di free cam
    if (viewMode == 0) {
        if (key == GLUT_KEY_UP) {
            camera_pos[0] += camera_front[0] * camera_speed;
            camera_pos[1] += camera_front[1] * camera_speed;
            camera_pos[2] += camera_front[2] * camera_speed;
        }
        else if (key == GLUT_KEY_DOWN) {
            camera_pos[0] -= camera_front[0] * camera_speed;
            camera_pos[1] -= camera_front[1] * camera_speed;
            camera_pos[2] -= camera_front[2] * camera_speed;
        }
        else if (key == GLUT_KEY_LEFT) {
            camera_pos[0] -= camera_right[0] * camera_speed;
            camera_pos[1] -= camera_right[1] * camera_speed;
            camera_pos[2] -= camera_right[2] * camera_speed;
        }
        else if (key == GLUT_KEY_RIGHT) {
            camera_pos[0] += camera_right[0] * camera_speed;
            camera_pos[1] += camera_right[1] * camera_speed;
            camera_pos[2] += camera_right[2] * camera_speed;
        }
    }
    
    glutPostRedisplay();
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(300,30);
    glutCreateWindow("Pistol Perspektif Kevin Adi Santoso_24060123130081");
    glutReshapeFunc(resize);
    glutReshapeWindow(800,600);
    glutKeyboardFunc(keypress);
    glutSpecialFunc(specialKeypress);
    glutDisplayFunc(render);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 800.0/600.0, 1.0, 2000.0);
    
    // inisialisasi 
    projectionMode = 0;
    viewMode = 0;
    camera_pos[0] = 0.0;
    camera_pos[1] = 0.0;
    camera_pos[2] = 150.0;
    yaw = -90.0f;
    
    // inisialisasi vektor kamera
    updateCameraVectors();
    
    // tampilkan panduan
    help(); // OUTPUTNYA DI TERMINAL/KONSOL, DI DEV C++ GA BISA LANGSUNG KELIHATAN, PAKAI IDE LAIN ATAU SETTING DLU
    
    glutMainLoop();
    return 0;
}
