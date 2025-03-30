#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
double rx = 0.0;
double ry = 0.0;
float l[] = { 0.0, 80.0, 0.0 }; // koordinat sumber cahaya
float n[] = { 0.0, -40.0, 0.0 };
float e[] = { 0.0, -60.0, 0.0 };
void help();

// obyek pistol yang akan digambar
void draw()
{
    glPushMatrix();
    
    // scaling dan positioning
    glScalef(5.0, 5.0, 5.0);
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef(90, 0, 1, 0);
    
    // body pistol
    glPushMatrix();
    glScalef(3.0, 1.0, 0.7);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // barrel pistol
    glPushMatrix();
    glTranslatef(5.5, 0.3, 0.0); 
    glScalef(3.5, 0.4, 0.4);   
    glutSolidCube(2.0);
    glPopMatrix();
    
    // grip pistol
    glPushMatrix();
    glTranslatef(-1.0, -2.0, 0.0);
    glRotatef(-10, 0, 0, 1);
    glScalef(0.8, 2.0, 0.6);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // guard pistol
    glPushMatrix();
    glTranslatef(0.0, -1.5, 0.0);
    glScalef(0.8, 0.6, 0.2);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // slide pistol
    glPushMatrix();
    glTranslatef(2.0, 0.8, 0.0);
    glScalef(2.5, 0.3, 0.6);
    glutSolidCube(2.0);
    glPopMatrix();
    
    // iron sight anjas
    glPushMatrix();
    glTranslatef(4.0, 1.2, 0.0);
    glScalef(0.2, 0.3, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix();
}

//membuat proyeksi bayangan
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
    glLightfv(GL_LIGHT0, GL_POSITION, l);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POINTS);
    glVertex3f(l[0],l[1],l[2]);
    glEnd();
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
    glEnable(GL_LIGHTING);
    glColor3f(0.0,0.0,0.8);
    draw();
    glPopMatrix();
    //sekarang gambar bayangan yang muncul
    glPushMatrix();
    glShadowProjection(l,e,n);
    glRotatef(ry,0,1,0);
    glRotatef(rx,1,0,0);
    glDisable(GL_LIGHTING);
    glColor3f(0.4,0.4,0.4);
    draw();
    glPopMatrix();
    glutSwapBuffers();
}

void keypress(unsigned char c, int a, int b)
{
    if ( c==27 ) exit(0);
    else if ( c=='s' ) l[1]-=5.0;
    else if ( c=='w' ) l[1]+=5.0;
    else if ( c=='a' ) l[0]-=5.0;
    else if ( c=='d' ) l[0]+=5.0;
    else if ( c=='q' ) l[2]-=5.0;
    else if ( c=='e' ) l[2]+=5.0;
    else if ( c=='h' ) help();
}

void help()
{
    printf("proyeksi contoh bayangan sebuah obyek pistol\n");
    printf("w/s: geser cahaya ke atas/bawah\n");
    printf("a/d: geser cahaya ke kiri/kanan\n");
    printf("q/e: geser cahaya ke depan/belakang\n");
    printf("h  : menampilkan bantuan\n");
    printf("ESC: keluar program\n");
}

void idle()
{
    rx+=0.1;
    ry+=0.1;
    render();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(300,30);
    glutCreateWindow("bayangan pistolnya kevin");
    glutReshapeFunc(resize);
    glutReshapeWindow(400,400);
    glutKeyboardFunc(keypress);
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.0, 1.0, 400.0);
    // Reset koordinat sebelum dimodifikasi/diubah
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -150.0);
    glutMainLoop();
    return 0;
}
