/*******************************************************************
		   Hierarchical Multi-Part Model Example
********************************************************************/
#define GL_SILENCE_DEPRECATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
	#include <glut/glut.h>
#else
	#include <gl/glut.h>
#endif
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "QuadMesh.h"

const int vWidth = 1200;  // Viewport width in pixels
const int vHeight = 1200; // Viewport height in pixels

// Camera Position (Y and Z).
const float cameraZ = 35.0;
const float cameraY = 0.0;

// Note how everything depends on robot body dimensions so that can scale entire robot proportionately
// just by changing robot body scale
float robotBodyWidth = 8.0;
float robotBodyLength = 10.0;
float robotBodyDepth = 6.0;
float headWidth = 0.4*robotBodyWidth;
float headLength = headWidth;
float headDepth = headWidth;
float upperArmLength = robotBodyLength;
float upperArmWidth = 0.125*robotBodyWidth;
float gunLength = upperArmLength / 4.0;
float gunWidth = upperArmWidth;
float gunDepth = upperArmWidth;
float stanchionLength = robotBodyLength;
float stanchionRadius = 0.1*robotBodyDepth;
float baseWidth = 2 * robotBodyWidth;
float baseLength = 0.25*stanchionLength;

// Control Robot body rotation on base
float robotAngle = 0.0;

// Control arm rotation
float shoulderAngle = -40.0;
float gunAngle = -25.0;

// Zeppelin Global Object Configs.
float zeppelinBodyWidth = 15.0;
float zeppelinBodyLength = 5.0;
float zeppelinBodyDepth = 5.0;
float commandCenterWidth = 0.5 * zeppelinBodyWidth;
float commandCenterLength = 0.5 * zeppelinBodyLength;
float commandCenterDepth = 0.85 * zeppelinBodyDepth;
float finWidth = 0.2;
float finLength = 0.5 * zeppelinBodyLength;
float finDepth = 0.8 * zeppelinBodyDepth;
float driveShaftWidth = 0.125 * commandCenterWidth;
float driveShaftLength = 0.075 * commandCenterLength;
float driveShaftDepth = 0.075 * commandCenterDepth;
float bladeWidth = 0.2 * driveShaftWidth;
float bodyBladeLength = 8.0 * driveShaftLength;
float commandBladeLength = 4.0 * driveShaftLength;
float bladeDepth = 0.4 * driveShaftDepth;

// Zeppelin Forward Direction vector.
VECTOR3D forwardDirection = VECTOR3D(1.0, 0.0, 0.0);
VECTOR3D zeppelinCenter = VECTOR3D();

// Zeppelin Rotation Angle around Y.
float zeppelinAngle = 0.0;

// Zeppelin Height.
float zeppelinHeight = 0.0;

// Zeppelin Propeller blade angle.
float bladeAngle = 0.0;


// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat robotBody_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotBody_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat robotBody_mat_diffuse[] = { 0.1f,0.35f,0.1f,1.0f };
GLfloat robotBody_mat_shininess[] = { 32.0F };


GLfloat robotArm_mat_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
GLfloat robotArm_mat_diffuse[] = { 0.5f,0.0f,0.0f,1.0f };
GLfloat robotArm_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat robotArm_mat_shininess[] = { 32.0F };

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.01f,0.0f,0.01f,0.01f };
GLfloat gun_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat robotLowerBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotLowerBody_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotLowerBody_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat robotLowerBody_mat_shininess[] = { 76.8F };


// Light properties
GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };


// Mouse button
int currentButton;

// A flat open mesh
QuadMesh *groundMesh = NULL;

// Structure defining a bounding box, currently unused
typedef struct BoundingBox {
    VECTOR3D min;
	VECTOR3D max;
} BBox;

// Default Mesh Size
int meshSize = 16;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void drawRobot();
void drawBody();
void drawHead();
void drawLowerBody();
void drawLeftArm();
void drawRightArm();

void drawZeppelin();
void drawZeppelinBody();
void drawCommandCenter();
void drawTopFin();
void drawLeftFin();
void drawRightFin();
void drawPropeller();
void drawBodyPropeller();
void drawBlade(float initAngle, float bladeLength);

void onRotate(float angleIncrement);
void onMove();

int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("Zeppelin");

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);

	// Start event loop, never returns
	glutMainLoop();

	return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). 
void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);   // This second light is currently off

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Other initializatuion
	// Set up ground quad mesh
	VECTOR3D origin = VECTOR3D(-16.0f, 0.0f, 16.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 32.0, 32.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(0.0f, 0.05f, 0.0f);
	VECTOR3D diffuse = VECTOR3D(0.4f, 0.8f, 0.4f);
	VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
	float shininess = 0.2;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);

}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// Create Viewing Matrix V
	// Set up the camera at position (0, cameraY, cameraZ) looking at the origin, up along positive y axis
	gluLookAt(0.0, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Draw Zeppelin

	// Apply modelling transformations M to move robot
	// Current transformation matrix is set to IV, where I is identity matrix
	// CTM = IV
	drawZeppelin();

	// Draw ground
	glPushMatrix();
	glTranslatef(0.0, -20.0, 0.0);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	glutSwapBuffers();   // Double buffering, swap buffers
}

// Draw Full Zeppelin Model
void drawZeppelin()
{
	glPushMatrix();
		// Translate Up/Down based on Zeppelin Height (Move Zeppelin up/down)
		glTranslatef(0.0, zeppelinHeight, 0.0);
		glTranslatef(zeppelinCenter.GetX(), 0.0, zeppelinCenter.GetZ());

		// Rotate to Face Positive X-Axis (Since Model was built facing negative x originally).
		glRotatef(180.0, 0.0, 1.0, 0.0);
		// Rotate the Zeppelin about its Y Axis.
		glRotatef(zeppelinAngle, 0.0, 1.0, 0.0);

		drawZeppelinBody();
		drawCommandCenter();
		drawTopFin();
		drawLeftFin();
		drawRightFin();
		drawBodyPropeller();
	glPopMatrix();

	glPopMatrix();
}

// Draw Zeppelin Body Object.
void drawZeppelinBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glScalef(zeppelinBodyWidth, zeppelinBodyLength, zeppelinBodyDepth);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();
}

// Draw Zeppelin Command Center.
void drawCommandCenter() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

	glPushMatrix();
	// Position Command Center with Respect to Parent (body)
	glTranslatef(0, -(0.95 * commandCenterLength + 0.5 * zeppelinBodyLength), 0);

	// Build Command Center.
	glPushMatrix();
		glScalef(commandCenterWidth, commandCenterLength, commandCenterDepth);
		glutSolidCube(1.0);
	glPopMatrix();

	// Draw Propeller (Child Object)
	drawPropeller();
	glPopMatrix();
}

// Draw Zeppelin Top Fin.
void drawTopFin() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();
	// Position Top Fin with Respect to Parent (body)
	glTranslatef(finWidth + 0.65 * zeppelinBodyWidth, finLength + 0.10*zeppelinBodyLength, 0.0);

	// Build the top fin.
	glPushMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glRotatef(165.0, 1.0, 0.0, 0.0);

		glScalef(finWidth, finLength, finDepth);
		glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

// Draw Zeppelin Left Fin.
void drawLeftFin() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();
	// Position Top Fin with Respect to Parent (body)
	glTranslatef(finWidth + 0.65 * zeppelinBodyWidth, 0.0, 0.45 * finDepth + 0.5 * zeppelinBodyDepth);

	// Build the top fin.
	glPushMatrix();
		glRotatef(90.0, 0.0, 0.0, 1.0);
		glRotatef(60.0, 1.0, 0.0, 0.0);

		glScalef(finWidth, finLength, finDepth);
		glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

// Draw Zeppelin Right Fin.
void drawRightFin() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();
	// Position Top Fin with Respect to Parent (body)
	glTranslatef(finWidth + 0.65 * zeppelinBodyWidth, 0.0, -(0.45 * finDepth + 0.5 * zeppelinBodyDepth));

	// Build the top fin.
	glPushMatrix();
		glRotatef(-90.0, 0.0, 0.0, 1.0);
		glRotatef(60.0, 1.0, 0.0, 0.0);

		glScalef(finWidth, finLength, finDepth);
		glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

// Draw Zeppelin Command Center Propeller.
void drawPropeller() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	// Drive Shaft.
	glPushMatrix();
		// Position Drive Shaft Relative to Parent (Command Center)
		glTranslatef((0.5*driveShaftWidth + 0.5*commandCenterWidth), -0.25 * commandCenterLength, 0.0);

		// Build Drive Shaft.
		glPushMatrix();
			glScalef(driveShaftWidth, driveShaftLength, driveShaftDepth);
			glutSolidCube(1.0);
		glPopMatrix();

		// Propeller Blades.
		drawBlade(45.0, commandBladeLength);
		drawBlade(135.0, commandBladeLength);
		
	glPopMatrix();
}

// Draw Zeppelin Body Propeller.
void drawBodyPropeller() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	// Drive Shaft.
	glPushMatrix();
		// Position Drive Shaft Relative to Parent (Body)
		glTranslatef(((driveShaftWidth / 0.125) +  0.5 * zeppelinBodyWidth), 0.0, 0.0);

		// Build Drive Shaft.
		glPushMatrix();
			glScalef(driveShaftWidth, driveShaftLength, driveShaftDepth);
			glutSolidCube(1.0);
		glPopMatrix();

		// Propeller Blades.
		drawBlade(45.0, bodyBladeLength);
		drawBlade(135.0, bodyBladeLength);
		
	glPopMatrix();
}

// Draw Propeller Blade on Drive Shaft - Blade Length customizable for propellers with different parents + initAngle to define starting orientation for the blade
void drawBlade(float initAngle, float bladeLength) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	// Propeller Blade.
	glPushMatrix();
		// Position Blade Relative to Parent (Drive Shaft)
		glTranslatef(0.5 * driveShaftWidth, 0.0, 0.0);

		// Build Propeller Blade.
		glPushMatrix();
			glRotatef(initAngle + bladeAngle, 1.0, 0.0, 0.0);

			glScalef(bladeWidth, bladeLength, bladeDepth);
			glutSolidSphere(1.0, 100, 100);
		glPopMatrix();
	glPopMatrix();
}

void drawRobot()
{
	glPushMatrix();
	 // spin robot on base. 
	 glRotatef(robotAngle, 0.0, 1.0, 0.0);

	 drawBody();
	 drawHead();
	 drawLeftArm();
	 drawRightArm();
	glPopMatrix();
	
	// don't want to spin fixed base in this example
	drawLowerBody();

	glPopMatrix();
}


void drawBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glScalef(robotBodyWidth, robotBodyLength, robotBodyDepth);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawHead()
{
	// Set robot material properties per body part. Can have seperate material properties for each part
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	// Position head with respect to parent (body)
	glTranslatef(0, 0.5*robotBodyLength+0.5*headLength, 0); // this will be done last
	
	// Build Head
	glPushMatrix();
	glScalef(0.4*robotBodyWidth, 0.4*robotBodyWidth, 0.4*robotBodyWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void drawLowerBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

	glPushMatrix();
	// Position stanchion and base with respect to body
	glTranslatef(0, -1.5*robotBodyLength, 0.0); // this will be done last

	// stanchion
	glPushMatrix();
	glScalef(stanchionRadius, stanchionLength, stanchionRadius);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
	glPopMatrix();

	// base
	glPushMatrix();
	// Position base with respect to parent stanchion
	glTranslatef(0.0, -0.25*stanchionLength, 0.0);
	// Build base
	glScalef(baseWidth, baseLength, baseWidth);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void drawLeftArm()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();
    // Position arm with respect to parent body
	glTranslatef(0.5*robotBodyWidth + 0.5*upperArmWidth, 0, 0.0); // this will be done last

	// build arm
	glPushMatrix();
	glScalef(upperArmWidth, upperArmLength, upperArmWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void drawRightArm()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();

	// Rotate arm at shoulder
	glTranslatef(-(0.5*robotBodyWidth + 0.5*upperArmWidth), 0.5*upperArmLength, 0.0);
	glRotatef(shoulderAngle, 1.0, 0.0, 0.0);
	glTranslatef((0.5*robotBodyWidth + 0.5*upperArmWidth), -0.5*upperArmLength, 0.0);

	// Position arm and gun with respect to parent body
	glTranslatef(-(0.5*robotBodyWidth + 0.5*upperArmWidth), 0, 0.0);
	
	// build arm
	glPushMatrix();
	glScalef(upperArmWidth, upperArmLength, upperArmWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	//  Gun
	glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);

	glPushMatrix();
	// rotate gun
	glTranslatef(-(0.5*robotBodyWidth + 0.5*upperArmWidth), -(0.5*upperArmLength), 0.0);
	glRotatef(gunAngle, 1.0, 0.0, 0.0);
	glTranslatef((0.5*robotBodyWidth + 0.5*upperArmWidth), (0.5*upperArmLength ), 0.0);
	
	// Position gun with respect to parent arm 
	glTranslatef(0, -(0.5*upperArmLength + 0.5*gunLength), 0.0);

	// build gun
	glScalef(gunWidth, gunLength, gunDepth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();


}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	// Set up viewport, projection, then change to modelview matrix mode - 
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 80.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up the camera at position (0, cameraY, cameraZ) looking at the origin, up along positive y axis
	gluLookAt(0.0, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Callback, called when user rotates zeppelin in y-axis
void onRotate(float angleIncrement) {
	zeppelinAngle += angleIncrement;
	// Calculate/Update Forward Direction Vector based on Zeppeling Angle.
	forwardDirection.Set(cosf(zeppelinAngle * (M_PI/180.0)), 0.0, -sinf(zeppelinAngle  * (M_PI/180.0)));
	// Normalize the forward direction for a magnitude of 1
	forwardDirection.Normalize();
}

// Callback, called when user uses arrow keys to move forward
void onMove() {
	// Update the Zeppelin Center to be used to translate zeppeling forward.
	zeppelinCenter += forwardDirection;
}

bool stop = false;

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 't':

		break;
	case 'r':
		onRotate(2.0);
		break;
	case 'R':
		onRotate(-2.0);
		break;
	case 'a':
		zeppelinHeight += 2.0;
		break;
	case 'A':
		zeppelinHeight -= 2.0;
		break;
	case 'g':
		gunAngle += 2.0;
		break;
	case 'G':
		gunAngle -= 2.0;
		break;
	case 's':
		glutTimerFunc(10, animationHandler, 0);
		break;
	case 'S':
		stop = true;
		break;
	case 'w':
		onMove();
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


void animationHandler(int param)
{
	if (!stop)
	{
		// Animate the blade angle (Return to 0 after passing 360)
		if (bladeAngle == 360.0) bladeAngle = 0.0;
		else bladeAngle += 1.0;
		glutPostRedisplay();
		glutTimerFunc(10, animationHandler, 0);
	}
}



// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	// Help key
	if (key == GLUT_KEY_F1)
	{

	}
	// Do transformations with arrow keys
	//else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
	//{
	//}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;

		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

