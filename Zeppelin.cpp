/*******************************************************************
		   Zeppelin Hierarchical Multi-Part Model
********************************************************************/
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
	#include <glut/glut.h>
	#include <OpenGL/glu.h>
#else
	#include <gl/glut.h>
#endif
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "surfaceModeller.h"
#include "stb_image.h"

const int vWidth = 1200;  // Viewport width in pixels
const int vHeight = 1200; // Viewport height in pixels

// Camera Position X,Y,Z.
float cameraZ = 65.0;
float cameraY = 35.0;
float cameraX = 0.0;

// Camera Ref X,Y,Z.
float cameraRefX = 0.0;
float cameraRefY = 0.0;
float cameraRefZ = 0.0;

// Camera Mode.
bool fov = false;

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
float lightBaseWidth = 0.25 * commandCenterWidth;
float lightBaseLength = 0.2 * commandCenterLength;
float lightBaseDepth = 0.25 * commandCenterDepth;
float lightWidth = 0.25 * lightBaseWidth;
float lightLength = 2.0 * lightBaseLength;
float missileHolderWidth = 0.035 * zeppelinBodyLength;
float missileHolderLength = 0.035 * zeppelinBodyLength;
float missileHolderDepth = 0.3 * zeppelinBodyDepth;
float missileBodyWidth = 0.4 * zeppelinBodyWidth;
float missileBodyLength = 0.02 * zeppelinBodyWidth;
float missileBodyDepth = 0.02 * zeppelinBodyWidth;
float missileHeadWidth = 0.125 * missileBodyWidth;
float missileHeadLength = missileBodyLength;
float missileHeadDepth = missileBodyDepth;

// Zeppelin Forward Direction vector.
VECTOR3D forwardDirection = VECTOR3D(1.0, 0.0, 0.0);
VECTOR3D zeppelinCenter = VECTOR3D();

// Zeppelin Ally Missile Center.
VECTOR3D missileCenter = VECTOR3D();

// Enemy Bounding Box.
float allyBoundX[2] = {zeppelinCenter.GetX() - (0.5 * zeppelinBodyWidth), zeppelinCenter.GetX() + (1.5 * zeppelinBodyWidth)};
float allyBoundY[2] = {zeppelinCenter.GetY() - (0.5 * zeppelinBodyLength), zeppelinCenter.GetY() + (0.5 * zeppelinBodyLength)};
float allyBoundZ[2] = {zeppelinCenter.GetZ() - (0.5 * zeppelinBodyDepth), zeppelinCenter.GetZ() + (0.5 * zeppelinBodyDepth)};

// Ally Destroyed State.
bool allyDestroyed = false;

// Zeppelin Ally Missile Boolean State.
bool missileFire = false;

// Zeppelin Rotation Angle around Y.
float zeppelinAngle = 0.0;

// Zeppelin Height.
float zeppelinHeight = 0.0;

// Zeppelin Propeller blade angle.
float bladeAngle = 0.0;

// Enemy Zeppelin Height.
float enemyZeppelinHeight = 10.0;

// Enemy Zeppelin Forward Direction.
VECTOR3D enemyForwardDirection = VECTOR3D(1.0, 0.0, 0.0);
VECTOR3D enemyZeppelinCenter = VECTOR3D(0.0, enemyZeppelinHeight, -35.0);

// Enemy Bounding Box.
float enemyBoundX[2] = {enemyZeppelinCenter.GetX() - (0.5 * zeppelinBodyWidth), enemyZeppelinCenter.GetX() + (1.5 * zeppelinBodyWidth)};
float enemyBoundY[2] = {enemyZeppelinCenter.GetY() - (0.5 * zeppelinBodyLength), enemyZeppelinCenter.GetY() + (0.5 * zeppelinBodyLength)};
float enemyBoundZ[2] = {enemyZeppelinCenter.GetZ() - (0.5 * zeppelinBodyDepth), enemyZeppelinCenter.GetZ() + (0.5 * zeppelinBodyDepth)};

// Enemy Destroyed State.
bool enemyDestroyed = false;

// Enemy Zeppelin Rotation Angle around Y.
float enemyZeppelinAngle = 0.0;

// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat zeppelinBody_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat zeppelinBody_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat zeppelinBody_mat_diffuse[] = { 0.9f,0.9f,0.9f,1.0f };
GLfloat zeppelinBody_mat_shininess[] = { 32.0F };

GLfloat fin_mat_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
GLfloat fin_mat_diffuse[] = { 0.5f,0.0f,0.0f,1.0f };
GLfloat fin_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat fin_mat_shininess[] = { 32.0F };

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.01f,0.0f,0.01f,0.01f };
GLfloat gun_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat command_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat command_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat command_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat command_mat_shininess[] = { 100.0F };

GLfloat blade_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat blade_mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat blade_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat blade_mat_shininess[] = { 76.8F };

GLfloat shaft_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat shaft_mat_diffuse[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat shaft_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat shaft_mat_shininess[] = { 32.0F };


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
int meshSize = 128;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);

// Draw Functions.
void drawZeppelin();
void drawEnemyZeppelin();
void drawZeppelinBody();
void drawEnemyZeppelinBody();
void drawCommandCenter();
void drawLights();
void drawTopFin();
void drawLeftFin();
void drawRightFin();
void drawPropeller();
void drawBodyPropeller();
void drawBlade(float initAngle, float bladeLength);
void drawMissile();

// Renderer Helpers.
void renderGluSphere(GLuint textureID);
void renderGluCylinder();

// User Interaction Utility Functions.
void onRotate(float angleIncrement);
void onMove();
void onHeightChange(float heightIncrement);

// Camera Utility Functions.
void updateFOVCameraPosition();
void updateFOVCameraRef();
void onCameraSwap();

// Missile Utility.
bool checkMissileIntersection(VECTOR3D missileCenter, float boundX[2], float boundY[2], float boundZ[2]);

// File System Helpers.
void readOBJ();

// Texture Helpers.
GLuint loadTexture(const char *filename);


GLuint loadTexture(const char *filename) {
	GLuint textureID;
    int width, height, channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 3);

    if (!image) {
        printf("Texture loading failed!\n");
        return 0;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Enable Modulate


	return textureID;
}

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
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);

	// Activate Propeller Animation.
	glutTimerFunc(10, animationHandler, 0);

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
	gluLookAt(cameraX, cameraY, cameraZ, cameraRefX, cameraRefY, cameraRefZ, 0.0, 1.0, 0.0);

	// Draw Zeppelin

	// Apply modelling transformations M to move robot
	// Current transformation matrix is set to IV, where I is identity matrix
	// CTM = IV
	if (!allyDestroyed) drawZeppelin();

	// Draw Enemy Zeppelin.
	if (!enemyDestroyed) drawEnemyZeppelin();

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

		// Rotate the Zeppelin about its Y Axis.
		glRotatef(zeppelinAngle, 0.0, 1.0, 0.0);

		drawZeppelinBody();
		drawCommandCenter();
		drawTopFin();
		drawLeftFin();
		drawRightFin();
		drawBodyPropeller();
		drawMissile();
	glPopMatrix();

	glPopMatrix();
}

// Draw Enemy Zeppelin.
void drawEnemyZeppelin() {
	glPushMatrix();
		// Translate Up/Down based on Zeppelin Height (Move Zeppelin up/down)
		glTranslatef(0.0, enemyZeppelinHeight, 0.0);
		glTranslatef(enemyZeppelinCenter.GetX(), 0.0, enemyZeppelinCenter.GetZ());

		// Rotate the Zeppelin about its Y Axis.
		glRotatef(enemyZeppelinAngle, 0.0, 1.0, 0.0);

		glPushMatrix();
			glTranslatef(15.0, 0.0, 0.0);
			glutSolidCube(1.0);
		glPopMatrix();

		drawEnemyZeppelinBody();
		drawCommandCenter();
		drawTopFin();
		drawLeftFin();
		drawRightFin();
		drawBodyPropeller();
		drawMissile();
	glPopMatrix();

	glPopMatrix();
}

// Draw Zeppelin Body Object.
void drawZeppelinBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeppelinBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zeppelinBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, zeppelinBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, zeppelinBody_mat_shininess);

	glPushMatrix();
		glScalef(zeppelinBodyWidth, zeppelinBodyLength, zeppelinBodyDepth);
		// Old Render using Glut.
		// glutSolidSphere(1.0, 100, 100);

		// Load Body Texture.
		GLuint bodyTexture = loadTexture("textures/marble.png");
		// Render using Glu.
		renderGluSphere(bodyTexture);
	glPopMatrix();
}

// Draw Enemy Zeppelin Body Object.
void drawEnemyZeppelinBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeppelinBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zeppelinBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, zeppelinBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, zeppelinBody_mat_shininess);

	glPushMatrix();
		glScalef(zeppelinBodyWidth, zeppelinBodyLength, zeppelinBodyDepth);
		// Old Render using Glut.
		// glutSolidSphere(1.0, 100, 100);

		// Load Body Texture.
		GLuint bodyTexture = loadTexture("textures/lava.png");
		// Render using Glu.
		renderGluSphere(bodyTexture);
	glPopMatrix();
}

// Draw Zeppelin Command Center.
void drawCommandCenter() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, command_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, command_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, command_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, command_mat_shininess);

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

	// Draw Undercarriage Lights
	drawLights();
	glPopMatrix();
}

// Draw Undercarriage Lights
void drawLights() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeppelinBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zeppelinBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, zeppelinBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, zeppelinBody_mat_shininess);

	glPushMatrix();
		// Position Light Base with Respect to Parent (Command Center)
		glTranslatef(0.0, -(0.25*lightBaseLength + 0.5*commandCenterLength), 0.0);

		// Build Light Base.
		glPushMatrix();
			glScalef(lightBaseWidth, lightBaseLength, lightBaseDepth);
			glutSolidSphere(1.0, 100, 100);
		glPopMatrix();

		// Change Materials for Lights
		glMaterialfv(GL_FRONT, GL_AMBIENT, command_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, command_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, command_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, command_mat_shininess);

		glPushMatrix();
			// Position First Light with Respect to Parent (Light Base)
			glTranslatef(0.0, -(0.85 * lightLength + 0.5 * lightBaseLength), 0.35 * lightBaseDepth);

			// Build Light
			glPushMatrix();
				glRotatef(-110.0, 1.0, 0.0, 0.0);
				glutSolidCone(lightWidth, lightLength, 25, 25);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			// Position Second Light with Respect to Parent (Light Base)
			glTranslatef(0.0, -(0.85 * lightLength + 0.5 * lightBaseLength), -0.5 * lightBaseDepth);

			// Build Light
			glPushMatrix();
				glRotatef(-70.0, 1.0, 0.0, 0.0);
				glutSolidCone(lightWidth, lightLength, 25, 25);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

// Draw Zeppelin Top Fin.
void drawTopFin() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, fin_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fin_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, fin_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, fin_mat_shininess);

	glPushMatrix();
	// Position Top Fin with Respect to Parent (body)
	glTranslatef(-(finWidth + 0.65 * zeppelinBodyWidth), finLength + 0.10*zeppelinBodyLength, 0.0);

	// Build the top fin.
	glPushMatrix();
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glRotatef(165.0, 1.0, 0.0, 0.0);

		glScalef(finWidth, finLength, finDepth);
		glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

// Draw Zeppelin Left Fin.
void drawLeftFin() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, fin_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fin_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, fin_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, fin_mat_shininess);

	glPushMatrix();
	// Position Top Fin with Respect to Parent (body)
	glTranslatef(-(finWidth + 0.65 * zeppelinBodyWidth), 0.0, 0.45 * finDepth + 0.5 * zeppelinBodyDepth);

	// Build the top fin.
	glPushMatrix();
		glRotatef(-90.0, 0.0, 0.0, 1.0);
		glRotatef(60.0, 1.0, 0.0, 0.0);

		glScalef(finWidth, finLength, finDepth);
		glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

// Draw Zeppelin Right Fin.
void drawRightFin() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, fin_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fin_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, fin_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, fin_mat_shininess);

	glPushMatrix();
	// Position Top Fin with Respect to Parent (body)
	glTranslatef(-(finWidth + 0.65 * zeppelinBodyWidth), 0.0, -(0.45 * finDepth + 0.5 * zeppelinBodyDepth));

	// Build the top fin.
	glPushMatrix();
		glRotatef(90.0, 0.0, 0.0, 1.0);
		glRotatef(60.0, 1.0, 0.0, 0.0);

		glScalef(finWidth, finLength, finDepth);
		glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

// Draw Zeppelin Command Center Propeller.
void drawPropeller() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, shaft_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, shaft_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shaft_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, shaft_mat_shininess);

	// Drive Shaft.
	glPushMatrix();
		// Position Drive Shaft Relative to Parent (Command Center)
		glTranslatef(-(0.5*driveShaftWidth + 0.5*commandCenterWidth), -0.25 * commandCenterLength, 0.0);

		// Build Drive Shaft.
		glPushMatrix();
			glScalef(driveShaftWidth, driveShaftLength, driveShaftDepth);
			// Render using Glu.
			// glRotatef(90.0, 0.0, 1.0, 0.0);
			// renderGluCylinder();
			glutSolidCube(1.0);
		glPopMatrix();

		// Propeller Blades.
		drawBlade(45.0, commandBladeLength);
		drawBlade(135.0, commandBladeLength);
		
	glPopMatrix();
}

// Draw Zeppelin Body Propeller.
void drawBodyPropeller() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, shaft_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, shaft_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shaft_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, shaft_mat_shininess);

	// Drive Shaft.
	glPushMatrix();
		// Position Drive Shaft Relative to Parent (Body)
		glTranslatef(-((driveShaftWidth / 0.125) +  0.5 * zeppelinBodyWidth), 0.0, 0.0);

		// Build Drive Shaft.
		glPushMatrix();
			glScalef(driveShaftWidth, driveShaftLength, driveShaftDepth);
			// Render Using Glu.
			// glRotatef(90.0, 0.0, 1.0, 0.0);
			// renderGluCylinder();
			glutSolidCube(1.0);
		glPopMatrix();

		// Propeller Blades.
		drawBlade(45.0, bodyBladeLength);
		drawBlade(135.0, bodyBladeLength);
		
	glPopMatrix();
}

// Draw Propeller Blade on Drive Shaft - Blade Length customizable for propellers with different parents + initAngle to define starting orientation for the blade
void drawBlade(float initAngle, float bladeLength) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, blade_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blade_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blade_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, blade_mat_shininess);

	// Propeller Blade.
	glPushMatrix();
		// Position Blade Relative to Parent (Drive Shaft)
		glTranslatef(-(0.5 * driveShaftWidth), 0.0, 0.0);

		// Build Propeller Blade.
		glPushMatrix();
			glRotatef(initAngle + bladeAngle, 1.0, 0.0, 0.0);

			glScalef(bladeWidth, bladeLength, bladeDepth);
			glutSolidSphere(1.0, 100, 100);
			// Render Using Glu.
			// renderGluSphere();
		glPopMatrix();
	glPopMatrix();
}

// Draw Missile on Zeppelin Body
void drawMissile() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, blade_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blade_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blade_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, blade_mat_shininess);

	// Shaft.
	glPushMatrix();
		// Position Relative to Body
		glTranslatef(0.0, 0.0, zeppelinBodyDepth + 0.5 * missileHolderDepth);

		glPushMatrix();
			glScalef(missileHolderWidth, missileHolderLength, missileHolderDepth);
			glutSolidCube(1.0);
		glPopMatrix();

		// Missile.
		glPushMatrix();

			// Position Relative to Shaft
			// Move Missile.
			if (missileFire) 
			{
				glRotatef(-zeppelinAngle, 0.0, 1.0, 0.0);
				glTranslatef(-zeppelinCenter.GetX(), 0.0, -zeppelinCenter.GetZ());
				glTranslatef(missileCenter.GetX(), 0.0, missileCenter.GetZ());
				glRotatef(zeppelinAngle, 0.0, 1.0, 0.0);
			}
			glTranslatef(-0.5 * missileBodyWidth, 0.0, 0.5 * missileHolderDepth + 0.5 * missileBodyDepth);


			// Build Missile
			glPushMatrix();
				glScalef(missileBodyWidth, missileBodyLength, missileBodyDepth);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				renderGluCylinder();
			glPopMatrix();

			// Missile Head.
			glPushMatrix();
				glTranslatef(missileBodyWidth, 0.0, 0.0);
				// Build Head.
				glPushMatrix();
					glScalef(missileHeadWidth, missileHeadLength, missileHeadDepth);
					glRotatef(90.0, 0.0, 1.0, 0.0);
					glutSolidCone(1.0, 2.0, 100, 100);
				glPopMatrix();
			glPopMatrix();

		glPopMatrix();
	glPopMatrix();
}

// Render Helpers.
void renderGluSphere(GLuint textureID){
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Enable Modulate

	// Draw a sphere using gluSphere
    GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricTexture(sphere, GL_TRUE);  // Enable texture mapping
    gluSphere(sphere, 1.0, 100, 100);
	gluDeleteQuadric(sphere);

	glDisable(GL_TEXTURE_2D);
}

void renderGluCylinder() {
	// Draw a Cylinder using gluCylinder
	GLUquadricObj* cylinder = gluNewQuadric();
	gluCylinder(cylinder, 1.0, 1.0, 1.0, 100, 100);
	gluDeleteQuadric(cylinder);
}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	// Set up viewport, projection, then change to modelview matrix mode - 
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 240.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up the camera at position (0, cameraY, cameraZ) looking at the origin, up along positive y axis
	gluLookAt(cameraX, cameraY, cameraZ, cameraRefX, cameraRefY, cameraRefZ, 0.0, 1.0, 0.0);
}

// Callback, called when user rotates zeppelin in y-axis
void onRotate(float angleIncrement) {
	zeppelinAngle += angleIncrement;
	// Calculate/Update Forward Direction Vector based on Zeppeling Angle.
	forwardDirection.Set(cosf(zeppelinAngle * (M_PI/180.0)), 0.0, -sinf(zeppelinAngle  * (M_PI/180.0)));
	// Normalize the forward direction for a magnitude of 1
	forwardDirection.Normalize();

	if (fov) {
		updateFOVCameraRef();
	}
}

// Callback, called when user uses arrow keys to move forward
void onMove() {
	// Update the Zeppelin Center to be used to translate zeppeling forward.
	zeppelinCenter += forwardDirection;
	// Update Missile Center.
	missileCenter += forwardDirection;
	// Update Bounding Box.
	allyBoundX[0] += forwardDirection.GetX(), allyBoundX[1] += forwardDirection.GetX();
	allyBoundY[0] += forwardDirection.GetY(), allyBoundY[1] += forwardDirection.GetY();
	allyBoundZ[0] += forwardDirection.GetZ(), allyBoundZ[1] += forwardDirection.GetZ();
	// Update FOV Camera Position and Reference Point.
	if (fov) {
		updateFOVCameraPosition();
		updateFOVCameraRef();
	}
}

// Callback, called when user uses up/down arrow keys to ascend/descend
void onHeightChange(float heightIncrement) {
	zeppelinHeight += heightIncrement;
	missileCenter.SetY(zeppelinHeight);
	// Update FOV Camera Height.
	if (fov) {
		cameraY = zeppelinHeight + (zeppelinBodyLength + 2.0);
		cameraRefY = zeppelinHeight + (zeppelinBodyLength + 2.0);
	}
}

// FOV Camera Position Update.
void updateFOVCameraPosition () {
	cameraX = zeppelinCenter.GetX();
	cameraY = zeppelinHeight + (zeppelinBodyLength + 2.0);
	cameraZ = zeppelinCenter.GetZ();
}

// FOV Camera Reference Update.
void updateFOVCameraRef() {
	cameraRefX = cameraX + forwardDirection.GetX();
	cameraRefY = zeppelinHeight + (zeppelinBodyLength + 2.0);
	cameraRefZ = cameraZ + forwardDirection.GetZ();
}

// Callback, called when t is pressed and switches between FPV and World Camera.
void onCameraSwap() {
	if (!fov) {
		// Update to FOV view.
		updateFOVCameraPosition();
		updateFOVCameraRef();

		fov = true;
	} else {
		// Reset to World Camera.
		cameraX = 0.0;
		cameraY = 35.0;
		cameraZ = 35.0;
		
		cameraRefX = 0.0;
		cameraRefY = 0.0;
		cameraRefZ = 0.0;
		fov = false; 
	}
}

int mseconds = 0;
// Callback, called when spacebar pressed to fire the ally missile.
void onMissileFire(int param) {
	if (mseconds < 50) {
		if (!enemyDestroyed && checkMissileIntersection(missileCenter, enemyBoundX, enemyBoundY, enemyBoundZ)) enemyDestroyed = true;
		// Update Boolean Fire State.
		missileFire = true;
		// Move Missile.
		missileCenter += forwardDirection;
		// Increment Timer to Prevent Infinite Loop + Reload Missile.
		mseconds += 1;
		glutTimerFunc(1, onMissileFire, 0);
	} else {
		mseconds = 0;
		missileCenter = zeppelinCenter;
		missileFire = false;
	}
	glutPostRedisplay();
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		onRotate(2.0);
		break;
	case 'R':
		onRotate(-2.0);
		break;
	case 'y':
		onHeightChange(2.0);
		break;
	case 'Y':
		onHeightChange(-2.0);
		break;
	case 'w':
		onMove();
		break;
	// Handle FPV Camera Change
	case 't':
		onCameraSwap();
		break;
	// Fire Missile.
	case 32:
		if (mseconds == 0) glutTimerFunc(1, onMissileFire, 0);
		break;
	// Respawn Enemy.
	case 'q':
		enemyDestroyed = false;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


void animationHandler(int param)
{
	// Animate the blade angle (Return to 0 after passing 360)
	if (bladeAngle == 360.0) bladeAngle = 0.0;
	else bladeAngle += 1.0;
	glutPostRedisplay();
	glutTimerFunc(10, animationHandler, 0);
}


// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	// Ascend/Descend (Up/Down Arrow Keys)
	if (key == GLUT_KEY_UP) {
		onHeightChange(2.0);
	}
	else if (key == GLUT_KEY_DOWN) {
		onHeightChange(-2.0);
	}
	// Right/Left Turning (Right/Left Arrow Keys)
	if (key == GLUT_KEY_RIGHT) {
		onRotate(-2.0);
	}
	else if (key == GLUT_KEY_LEFT) {
		onRotate(2.0);
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

bool checkMissileIntersection(VECTOR3D missileCenter, float boundX[2], float boundY[2], float boundZ[2]) {
	float centerX = missileCenter.GetX(), centerY = missileCenter.GetY(), centerZ = missileCenter.GetZ();
	return (centerX >= boundX[0] && centerX <= boundX[1]) && (centerY >= boundY[0] && centerY <= boundY[1]) && (centerZ >= boundZ[0] && centerZ <= boundZ[1]);
}

// Global Num Vertices, Indices, Tris
unsigned int numTris = 0;
unsigned int numVertices = 0;
unsigned int numIndices = 0;

// Global Positions, Normals, and Indices.
Vector3D *positions;  // vertex positions - used for VBO draw
Vector3D *normals;    // normal vectors for each vertex - used for VBO draw

// Index Array of Triangles derived from Quad Array - break each quad into 2 triangles
// Used when drawing with VBO as glDrawElements(GL_QUADS,...) no longer supported in newer versions of OpenGL
GLuint *indices;

void readOBJ()
{
    char buf[1024];
    char key[1024];
    int n;
    FILE *fin;

    int fc = 0; // face count
    int vc = 0; // vertex count
    int nc = 0; // normal count

    if ((fin = fopen("mesh.obj", "r")))
    {
        /* Process each line of the OBJ file, invoking the handler for each. */

        while (fgets(buf, 1024, fin))
            if (sscanf(buf, "%s%n", key, &n) >= 1)
            {
                if (!strcmp(key, "f"))
                {
                    sscanf(buf+1, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &indices[fc], &indices[fc + 1], &indices[fc + 2]);
                    fc += 3;
                }
                else if (!strcmp(key, "v"))
                {
                    sscanf(buf+1, "%f %f %f", &positions[vc].x, &positions[vc].y, &positions[vc].z);
                    vc++;
                }
                else if (!strcmp(key, "vn"))
                {
                    sscanf(buf+2, "%f %f %f", &normals[nc].x, &normals[nc].y, &normals[nc].z);
                    nc++;
                }
            }
        fclose(fin);

        numTris = fc / 3;
        numIndices = fc;
        numVertices = vc;
    }
}
