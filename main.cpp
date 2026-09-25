#include <windows.h>

#include <GL/glut.h>

#include<bits/stdc++.h>


void initGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


int sceneClock = 0;    
int scenePhase = 0;    /* 0 = clear day, 1 = evening, 2 = night */
GLfloat lightR = 1.0f; 
GLfloat lightG = 1.0f;
GLfloat lightB = 1.0f;
GLfloat wantR = 1.0f; 
GLfloat wantG = 1.0f;
GLfloat wantB = 1.0f;
GLfloat rainDrop = 0.0f;
GLfloat cloudDrift = 0.0f;

GLfloat nightMix = 0.0f; 
GLfloat sunMix = 1.0f; 

/* ---- how far each passenger has strolled from his own place ---- */
GLfloat walk1 = 0.00f;
GLfloat walkStep1 = 0.020f;
GLfloat walk2 = 1.20f;
GLfloat walkStep2 = -0.014f;
GLfloat walk3 = -0.60f;
GLfloat walkStep3 = 0.024f;
GLfloat walk4 = 0.80f;
GLfloat walkStep4 = -0.018f;
GLfloat walk5 = -1.40f;
GLfloat walkStep5 = 0.022f;
GLfloat walk6 = 0.30f;
GLfloat walkStep6 = -0.016f;

/* ---- where the two trains are ---- */
GLfloat trainPos = -26.0f;  /* the platform train, rolls in from the left    */
GLfloat trainSpeed = 0.16f; /* set to 0 when the DOWN key stops it           */
GLfloat train2Pos = 20.0f;  /* the through train, parked off the right edge  */
GLfloat train2Speed = 0.20f;

int train2Running = 0; /* 0 while it waits off screen, 1 while crossing */

int startAmbience = 0; /* raised when a new sound should be started     */

int rainOn = 0; /* 0 dry, 1 raining : the R key turns it over    */

void update(int value)
{

    if (trainPos > 27.0f) /* it has left on the right side */
        trainPos = -20.0f;
    trainPos += trainSpeed; /* trainSpeed is 0 when stopped  */

    /* ---- the through train moves only after the RIGHT arrow key ---- */
    if (train2Running == 1)
    {
        train2Pos -= train2Speed;
        if (train2Pos < -29.0f)
        {                      /* it has left on the left side  */
            train2Pos = 20.0f; /* park it off the screen again  */
            train2Running = 0; /* and wait for the next press   */
            startAmbience = 1; /* the whistle is over : bed back*/
        }
    }

    /* ---- the hour turns by itself, but the rain holds it still ---- */
    if (rainOn == 0)
    {
        sceneClock = sceneClock + 1;
        if (sceneClock > 260)
        {
            sceneClock = 0;
            scenePhase = scenePhase + 1;
            if (scenePhase > 2)
                scenePhase = 0;
            startAmbience = 1; /* the hour has turned           */
        }
    }

    if (scenePhase == 0)
    { /* clear day */
        wantR = 1.00f;
        wantG = 1.00f;
        wantB = 1.00f;
    }
    if (scenePhase == 1)
    { /* evening */
        wantR = 1.00f;
        wantG = 0.72f;
        wantB = 0.48f;
    }
    if (scenePhase == 2)
    { /* night */
        wantR = 0.26f;
        wantG = 0.31f;
        wantB = 0.58f;
    }
    if (rainOn == 1 && scenePhase == 0)
    { /* rain greys the daylight down */
        wantR = 0.62f;
        wantG = 0.68f;
        wantB = 0.78f;
    }

    /* the light does not jump, it fades slowly to the new colour */
    if (lightR < wantR)
        lightR += 0.006f;
    if (lightR > wantR)
        lightR -= 0.006f;
    if (lightG < wantG)
        lightG += 0.006f;
    if (lightG > wantG)
        lightG -= 0.006f;
    if (lightB < wantB)
        lightB += 0.006f;
    if (lightB > wantB)
        lightB -= 0.006f;

    /* ---- the sun goes and the night things come, slowly ---- */
    if (scenePhase == 2)
    {
        if (nightMix < 1.0f)
            nightMix += 0.008f;
    }
    if (scenePhase != 2)
    {
        if (nightMix > 0.0f)
            nightMix -= 0.008f;
    }
    if (nightMix < 0.0f)
        nightMix = 0.0f;
    if (nightMix > 1.0f)
        nightMix = 1.0f;

    if (scenePhase == 0 && rainOn == 0)
    {
        if (sunMix < 1.0f)
            sunMix += 0.008f;
    }
    if (scenePhase == 1 || scenePhase == 2 || rainOn == 1)
    {
        if (sunMix > 0.0f)
            sunMix -= 0.008f;
    }
    if (sunMix < 0.0f)
        sunMix = 0.0f;
    if (sunMix > 1.0f)
        sunMix = 1.0f;

    /* ---- the passengers walk along the platform and turn back ---- */
    if (walk1 > 2.40f)
        walkStep1 = -0.020f;
    if (walk1 < -2.60f)
        walkStep1 = 0.020f;
    walk1 += walkStep1;

    if (walk2 > 3.00f)
        walkStep2 = -0.014f;
    if (walk2 < -1.80f)
        walkStep2 = 0.014f;
    walk2 += walkStep2;

    if (walk3 > 2.20f)
        walkStep3 = -0.024f;
    if (walk3 < -3.10f)
        walkStep3 = 0.024f;
    walk3 += walkStep3;

    if (walk4 > 2.60f)
        walkStep4 = -0.018f;
    if (walk4 < -2.40f)
        walkStep4 = 0.018f;
    walk4 += walkStep4;

    if (walk5 > 1.60f)
        walkStep5 = -0.022f;
    if (walk5 < -3.40f)
        walkStep5 = 0.022f;
    walk5 += walkStep5;

    if (walk6 > 2.00f)
        walkStep6 = -0.016f;
    if (walk6 < -2.20f)
        walkStep6 = 0.016f;
    walk6 += walkStep6;

    /* the rain keeps falling and starts again at the top */
    rainDrop -= 0.28f;
    if (rainDrop < -2.0f)
        rainDrop = 0.0f;

    /* the clouds drift along and come round again */
    cloudDrift = cloudDrift + 0.012f;
    if (cloudDrift > 24.0f)
        cloudDrift = 0.0f;

    /* ---- start the bed of this hour, but not over the whistle ---- */
    if (startAmbience == 1 && train2Running == 0)
    {
        startAmbience = 0;
        if (rainOn == 1) /* the rain is on, whatever the hour */
            PlaySound("rainny.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
        if (rainOn == 0 && scenePhase == 0) /* clear day */
            PlaySound("train_station.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
        if (rainOn == 0 && scenePhase == 1) /* evening   */
            PlaySound("evening.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
        if (rainOn == 0 && scenePhase == 2) /* night     */
            PlaySound("crickets_night.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    }

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void SpecialInput(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN:
        trainSpeed = 0.0f; /* stop the train at the platform */
        break;
    case GLUT_KEY_UP:
        trainSpeed = 0.16f; /* let the train go again         */
        break;
    case GLUT_KEY_LEFT:
        break;
    case GLUT_KEY_RIGHT:
        if (train2Running == 0)
        {                      /* the key does nothing while it */
            train2Pos = 20.0f; /* is already crossing, so one   */
            train2Running = 1; /* press cannot cut off another  */
            /* one blow, no SND_LOOP : update() puts the hour's sound back after */
            PlaySound("train_whistle.wav", NULL, SND_ASYNC | SND_FILENAME);
        }
        break;
    }
    glutPostRedisplay();
}

/* the letter keys : R turns the rain on, pressing it again turns it off */
void NormalInput(unsigned char key, int x, int y)
{
    if (key == 'r' || key == 'R')
    {
        if (rainOn == 0)
            rainOn = 1; /* let it rain            */
        else
            rainOn = 0; /* and stop it again      */
        startAmbience = 1; /* swap the sound over    */
    }
    glutPostRedisplay();
}

/* ---- one round shape, drawn once and used everywhere ---- */
/* cx cy is the centre, rx and ry the two radii : equal radii give a circle */
void drawCircle(float cx, float cy, float rx, float ry)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(rx, ry, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); /* the middle of the fan */
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.9659f, 0.2588f);
    glVertex2f(0.866f, 0.5f);
    glVertex2f(0.7071f, 0.7071f);
    glVertex2f(0.5f, 0.866f);
    glVertex2f(0.2588f, 0.9659f);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(-0.2588f, 0.9659f);
    glVertex2f(-0.5f, 0.866f);
    glVertex2f(-0.7071f, 0.7071f);
    glVertex2f(-0.866f, 0.5f);
    glVertex2f(-0.9659f, 0.2588f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(-0.9659f, -0.2588f);
    glVertex2f(-0.866f, -0.5f);
    glVertex2f(-0.7071f, -0.7071f);
    glVertex2f(-0.5f, -0.866f);
    glVertex2f(-0.2588f, -0.9659f);
    glVertex2f(-0.0f, -1.0f);
    glVertex2f(0.2588f, -0.9659f);
    glVertex2f(0.5f, -0.866f);
    glVertex2f(0.7071f, -0.7071f);
    glVertex2f(0.866f, -0.5f);
    glVertex2f(0.9659f, -0.2588f);
    glVertex2f(1.0f, -0.0f);
    glEnd();
    glPopMatrix();
}

/* ---- lettering : GLUT draws the strokes, so the scene holds no letter shapes ---- */

/* a word in the plain stroke font, x and y are the left end of the base line */
void drawText(const char *s, float x, float y, float sc)
{
    int i = 0;
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(sc, sc, 1.0f); /* a stroke letter is about 100 units tall */
    while (s[i] != '\0')
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
        i = i + 1;
    }
    glPopMatrix();
}

/* the same, in the fixed width font, so the digits of a number line up */
void drawNumber(const char *s, float x, float y, float sc)
{
    int i = 0;
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(sc, sc, 1.0f);
    while (s[i] != '\0')
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
        i = i + 1;
    }
    glPopMatrix();
}

/* ---- the three clouds : dx slides them along the sky ---- */
void drawClouds(float dx)
{
    glPushMatrix();
    glTranslatef(dx, 0.0f, 0.0f);
    glColor3ub(255, 255, 255);
    /* left cloud, left puff */
    drawCircle(-9.35f, 6.3f, 0.42f, 0.298f);
    /* left cloud, tall middle puff */
    drawCircle(-8.6f, 6.44f, 0.58f, 0.417f);
    /* left cloud, right puff */
    drawCircle(-7.88f, 6.3f, 0.46f, 0.318f);
    /* left cloud, flat underside */
    glBegin(GL_QUADS);
    glVertex2f(-9.380f, 6.040f);
    glVertex2f(-7.840f, 6.040f);
    glVertex2f(-7.840f, 6.340f);
    glVertex2f(-9.380f, 6.340f);
    glEnd();
    /* middle cloud, left puff */
    drawCircle(-1.2f, 6.05f, 0.336f, 0.238f);
    /* middle cloud, tall middle puff */
    drawCircle(-0.6f, 6.162f, 0.464f, 0.334f);
    /* middle cloud, right puff */
    drawCircle(-0.024f, 6.05f, 0.368f, 0.254f);
    /* middle cloud, flat underside */
    glBegin(GL_QUADS);
    glVertex2f(-1.224f, 5.842f);
    glVertex2f(0.008f, 5.842f);
    glVertex2f(0.008f, 6.082f);
    glVertex2f(-1.224f, 6.082f);
    glEnd();
    /* right cloud, left puff */
    drawCircle(8.275f, 6.45f, 0.294f, 0.208f);
    /* right cloud, tall middle puff */
    drawCircle(8.8f, 6.548f, 0.406f, 0.292f);
    /* right cloud, right puff */
    drawCircle(9.304f, 6.45f, 0.322f, 0.222f);
    /* right cloud, flat underside */
    glBegin(GL_QUADS);
    glVertex2f(8.254f, 6.268f);
    glVertex2f(9.332f, 6.268f);
    glVertex2f(9.332f, 6.478f);
    glVertex2f(8.254f, 6.478f);
    glEnd();
    glPopMatrix();
}


/* ---- one coach : dx puts it further back down the train ---- */
/* r1 g1 b1 is the colour of the lower half of the body */
void drawCoach(float dx, int r1, int g1, int b1)
{
    glPushMatrix();
    glTranslatef(dx, 0.0f, 0.0f);
    glColor3ub(38, 38, 44);
    /* black underframe box below the floor */
    glBegin(GL_QUADS);
    glVertex2f(-4.95f, -1.72f);
    glVertex2f(-0.55f, -1.72f);
    glVertex2f(-0.55f, -1.46f);
    glVertex2f(-4.95f, -1.46f);
    glEnd();
    glColor3ub(240, 234, 218);
    /* cream body panel of the coach */
    glBegin(GL_QUADS);
    glVertex2f(-5.05f, -1.46f);
    glVertex2f(-0.45f, -1.46f);
    glVertex2f(-0.45f, 0.36f);
    glVertex2f(-5.05f, 0.36f);
    glEnd();
    glColor3ub(r1, g1, b1);
    /* blue lower half of the body */
    glBegin(GL_QUADS);
    glVertex2f(-5.05f, -1.46f);
    glVertex2f(-0.45f, -1.46f);
    glVertex2f(-0.45f, -1.02f);
    glVertex2f(-5.05f, -1.02f);
    glEnd();
    glColor3ub(230, 190, 60);
    /* yellow stripe along the waist */
    glBegin(GL_QUADS);
    glVertex2f(-5.05f, -1.02f);
    glVertex2f(-0.45f, -1.02f);
    glVertex2f(-0.45f, -0.96f);
    glVertex2f(-5.05f, -0.96f);
    glEnd();
    glColor3ub(160, 50, 44);
    /* red stripe just under the roof */
    glBegin(GL_QUADS);
    glVertex2f(-5.05f, 0.26f);
    glVertex2f(-0.45f, 0.26f);
    glVertex2f(-0.45f, 0.31f);
    glVertex2f(-5.05f, 0.31f);
    glEnd();
    glColor3ub(70, 70, 76);
    /* the five window frames and the two dark door pillars */
    glBegin(GL_QUADS);
    glVertex2f(-4.4f, -0.34f);
    glVertex2f(-3.9f, -0.34f);
    glVertex2f(-3.9f, 0.18f);
    glVertex2f(-4.4f, 0.18f);
    glVertex2f(-3.74f, -0.34f);
    glVertex2f(-3.24f, -0.34f);
    glVertex2f(-3.24f, 0.18f);
    glVertex2f(-3.74f, 0.18f);
    glVertex2f(-3.08f, -0.34f);
    glVertex2f(-2.58f, -0.34f);
    glVertex2f(-2.58f, 0.18f);
    glVertex2f(-3.08f, 0.18f);
    glVertex2f(-2.42f, -0.34f);
    glVertex2f(-1.92f, -0.34f);
    glVertex2f(-1.92f, 0.18f);
    glVertex2f(-2.42f, 0.18f);
    glVertex2f(-1.76f, -0.34f);
    glVertex2f(-1.26f, -0.34f);
    glVertex2f(-1.26f, 0.18f);
    glVertex2f(-1.76f, 0.18f);
    glVertex2f(-4.91f, -1.4f);
    glVertex2f(-4.55f, -1.4f);
    glVertex2f(-4.55f, 0.2f);
    glVertex2f(-4.91f, 0.2f);
    glVertex2f(-0.95f, -1.4f);
    glVertex2f(-0.59f, -1.4f);
    glVertex2f(-0.59f, 0.2f);
    glVertex2f(-0.95f, 0.2f);
    glEnd();
    glColor3ub(152, 196, 222);
    /* the five window panes */
    glBegin(GL_QUADS);
    glVertex2f(-4.36f, -0.3f);
    glVertex2f(-3.94f, -0.3f);
    glVertex2f(-3.94f, 0.14f);
    glVertex2f(-4.36f, 0.14f);
    glVertex2f(-3.7f, -0.3f);
    glVertex2f(-3.28f, -0.3f);
    glVertex2f(-3.28f, 0.14f);
    glVertex2f(-3.7f, 0.14f);
    glVertex2f(-3.04f, -0.3f);
    glVertex2f(-2.62f, -0.3f);
    glVertex2f(-2.62f, 0.14f);
    glVertex2f(-3.04f, 0.14f);
    glVertex2f(-2.38f, -0.3f);
    glVertex2f(-1.96f, -0.3f);
    glVertex2f(-1.96f, 0.14f);
    glVertex2f(-2.38f, 0.14f);
    glVertex2f(-1.72f, -0.3f);
    glVertex2f(-1.3f, -0.3f);
    glVertex2f(-1.3f, 0.14f);
    glVertex2f(-1.72f, 0.14f);
    glEnd();
    glColor3ub(60, 55, 55);
    /* passenger seen through window 1 */
    drawCircle(-4.15f, -0.08f, 0.085f, 0.1f);
    /* passenger seen through window 2 */
    drawCircle(-3.49f, -0.08f, 0.085f, 0.1f);
    /* passenger seen through window 3 */
    drawCircle(-2.83f, -0.08f, 0.085f, 0.1f);
    /* passenger seen through window 4 */
    drawCircle(-2.17f, -0.08f, 0.085f, 0.1f);
    /* passenger seen through window 5 */
    drawCircle(-1.51f, -0.08f, 0.085f, 0.1f);
    glColor3ub(150, 150, 158);
    /* roof of the coach */
    glBegin(GL_QUADS);
    glVertex2f(-5.1f, 0.36f);
    glVertex2f(-0.4f, 0.36f);
    glVertex2f(-0.4f, 0.52f);
    glVertex2f(-5.1f, 0.52f);
    glEnd();
    glColor3ub(186, 186, 192);
    /* raised cap along the top of the roof */
    glBegin(GL_QUADS);
    glVertex2f(-5.1f, 0.52f);
    glVertex2f(-0.4f, 0.52f);
    glVertex2f(-0.24f, 0.63f);
    glVertex2f(-4.94f, 0.63f);
    glEnd();
    glColor3ub(48, 48, 56);
    /* the two bogie frames the wheels hang from */
    glBegin(GL_QUADS);
    glVertex2f(-4.768f, -1.9f);
    glVertex2f(-3.433f, -1.9f);
    glVertex2f(-3.433f, -1.54f);
    glVertex2f(-4.768f, -1.54f);
    glVertex2f(-2.068f, -1.9f);
    glVertex2f(-0.733f, -1.9f);
    glVertex2f(-0.733f, -1.54f);
    glVertex2f(-2.068f, -1.54f);
    glEnd();
    glColor3ub(26, 26, 30);
    /* coach wheel 1, tyre */
    drawCircle(-4.438f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* coach wheel 1, hub */
    drawCircle(-4.438f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* coach wheel 1, axle cap */
    drawCircle(-4.438f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* coach wheel 2, tyre */
    drawCircle(-3.763f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* coach wheel 2, hub */
    drawCircle(-3.763f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* coach wheel 2, axle cap */
    drawCircle(-3.763f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* coach wheel 3, tyre */
    drawCircle(-1.738f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* coach wheel 3, hub */
    drawCircle(-1.738f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* coach wheel 3, axle cap */
    drawCircle(-1.738f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* coach wheel 4, tyre */
    drawCircle(-1.063f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* coach wheel 4, hub */
    drawCircle(-1.063f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* coach wheel 4, axle cap */
    drawCircle(-1.063f, -1.87f, 0.048f, 0.048f);
    glPopMatrix();
}


/* ---- one train, drawn once and used twice ---- */
/* r1 g1 b1 is the body colour, r2 g2 b2 the darker band, r3 g3 b3 the darkest */
void drawTrain(int r1, int g1, int b1,
               int r2, int g2, int b2,
               int r3, int g3, int b3)
{
    /* ================= the train standing on the platform road ================= */
    /* ---- shadow of the train on the ballast ---- */
    glColor3ub(168, 164, 158);
    /* shadow the coach casts on the ballast */
    glBegin(GL_QUADS);
    glVertex2f(-14.85f, -2.33f);
    glVertex2f(5.1f, -2.33f);
    glVertex2f(5.1f, -2.13f);
    glVertex2f(-14.85f, -2.13f);
    glEnd();
    /* ---- three coaches, the same one drawn three times ---- */
    drawCoach(0.0f, r1, g1, b1);   /* the one coupled to the engine */
    drawCoach(-4.9f, r1, g1, b1);  /* 4.9 is one coach plus the gap */
    drawCoach(-9.8f, r1, g1, b1);
    /* ---- locomotive ---- */
    glColor3ub(38, 38, 44);
    /* black underframe deck of the locomotive */
    glBegin(GL_QUADS);
    glVertex2f(-0.07f, -1.75f);
    glVertex2f(4.97f, -1.75f);
    glVertex2f(4.97f, -1.44f);
    glVertex2f(-0.07f, -1.44f);
    glEnd();
    glColor3ub(52, 52, 60);
    /* fuel tank slung under the middle */
    glBegin(GL_QUADS);
    glVertex2f(1.75f, -1.72f);
    glVertex2f(2.85f, -1.72f);
    glVertex2f(2.85f, -1.5f);
    glVertex2f(1.75f, -1.5f);
    glEnd();
    glColor3ub(70, 70, 76);
    /* coupling that joins it to the coach */
    glBegin(GL_QUADS);
    glVertex2f(-0.29f, -1.66f);
    glVertex2f(-0.07f, -1.66f);
    glVertex2f(-0.07f, -1.4f);
    glVertex2f(-0.29f, -1.4f);
    glEnd();
    glColor3ub(60, 60, 66);
    /* skirt at the front, below the buffer beam */
    glBegin(GL_QUADS);
    glVertex2f(4.5f, -1.75f);
    glVertex2f(5.07f, -1.75f);
    glVertex2f(4.99f, -2.1f);
    glVertex2f(4.6f, -2.1f);
    glEnd();
    glColor3ub(r2, g2, b2);
    /* main body of the locomotive */
    glBegin(GL_QUADS);
    glVertex2f(-0.07f, -1.44f);
    glVertex2f(3.3f, -1.44f);
    glVertex2f(3.3f, 0.05f);
    glVertex2f(-0.07f, 0.05f);
    glVertex2f(3.3f, -1.44f);
    glVertex2f(5.0f, -1.44f);
    glVertex2f(5.0f, 0.14f);
    glVertex2f(3.3f, 0.14f);
    glVertex2f(3.3f, 0.14f);
    glVertex2f(5.0f, 0.14f);
    glVertex2f(4.63f, 0.42f);
    glVertex2f(3.3f, 0.42f);
    glEnd();
    glColor3ub(232, 186, 48);
    /* yellow band along the bottom of the body */
    glBegin(GL_QUADS);
    glVertex2f(-0.07f, -1.44f);
    glVertex2f(5.0f, -1.44f);
    glVertex2f(5.0f, -1.1f);
    glVertex2f(-0.07f, -1.1f);
    glEnd();
    glColor3ub(245, 245, 245);
    /* white line above the yellow band */
    glBegin(GL_QUADS);
    glVertex2f(-0.07f, -1.1f);
    glVertex2f(5.0f, -1.1f);
    glVertex2f(5.0f, -1.04f);
    glVertex2f(-0.07f, -1.04f);
    glEnd();
    glColor3ub(170, 40, 36);
    /* red stripe running along the side */
    glBegin(GL_QUADS);
    glVertex2f(-0.07f, -0.55f);
    glVertex2f(3.3f, -0.55f);
    glVertex2f(3.3f, -0.48f);
    glVertex2f(-0.07f, -0.48f);
    glEnd();
    glColor3ub(r3, g3, b3);
    /* the twelve louvre slats on the engine side */
    /* the twelve louvre slats on the engine side : one line each, 12 of them */
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(0.315f, -0.95f);
    glVertex2f(0.315f, -0.12f);
    glVertex2f(0.535f, -0.95f);
    glVertex2f(0.535f, -0.12f);
    glVertex2f(0.755f, -0.95f);
    glVertex2f(0.755f, -0.12f);
    glVertex2f(0.975f, -0.95f);
    glVertex2f(0.975f, -0.12f);
    glVertex2f(1.195f, -0.95f);
    glVertex2f(1.195f, -0.12f);
    glVertex2f(1.415f, -0.95f);
    glVertex2f(1.415f, -0.12f);
    glVertex2f(1.635f, -0.95f);
    glVertex2f(1.635f, -0.12f);
    glVertex2f(1.855f, -0.95f);
    glVertex2f(1.855f, -0.12f);
    glVertex2f(2.075f, -0.95f);
    glVertex2f(2.075f, -0.12f);
    glVertex2f(2.295f, -0.95f);
    glVertex2f(2.295f, -0.12f);
    glVertex2f(2.515f, -0.95f);
    glVertex2f(2.515f, -0.12f);
    glVertex2f(2.735f, -0.95f);
    glVertex2f(2.735f, -0.12f);
    glEnd();
    glLineWidth(1.0f);
    glColor3ub(255, 255, 255);
    /* the number painted on the side, in block letters */
    glColor3ub(70, 70, 76);
    /* the two cab window frames */
    glBegin(GL_QUADS);
    glVertex2f(4.1f, -0.1f);
    glVertex2f(4.83f, -0.1f);
    glVertex2f(4.83f, 0.24f);
    glVertex2f(4.1f, 0.24f);
    glVertex2f(3.42f, -0.06f);
    glVertex2f(3.96f, -0.06f);
    glVertex2f(3.96f, 0.24f);
    glVertex2f(3.42f, 0.24f);
    glEnd();
    glColor3ub(152, 196, 222);
    /* the two cab window panes */
    glBegin(GL_QUADS);
    glVertex2f(4.14f, -0.06f);
    glVertex2f(4.79f, -0.06f);
    glVertex2f(4.79f, 0.2f);
    glVertex2f(4.14f, 0.2f);
    glVertex2f(3.46f, -0.02f);
    glVertex2f(3.92f, -0.02f);
    glVertex2f(3.92f, 0.2f);
    glVertex2f(3.46f, 0.2f);
    glEnd();
    glColor3ub(60, 55, 55);
    /* the driver's head in the cab window */
    drawCircle(4.47f, 0.03f, 0.085f, 0.1f);
    glColor3ub(96, 96, 104);
    /* roof of the locomotive */
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, 0.05f);
    glVertex2f(3.3f, 0.05f);
    glVertex2f(3.3f, 0.19f);
    glVertex2f(-0.1f, 0.19f);
    glVertex2f(3.25f, 0.42f);
    glVertex2f(4.71f, 0.42f);
    glVertex2f(4.71f, 0.56f);
    glVertex2f(3.25f, 0.56f);
    glEnd();
    glColor3ub(60, 60, 66);
    /* raised roof over the cab */
    glBegin(GL_QUADS);
    glVertex2f(1.15f, 0.19f);
    glVertex2f(1.59f, 0.19f);
    glVertex2f(1.59f, 0.34f);
    glVertex2f(1.15f, 0.34f);
    glVertex2f(3.65f, 0.56f);
    glVertex2f(3.85f, 0.56f);
    glVertex2f(3.85f, 0.64f);
    glVertex2f(3.65f, 0.64f);
    glEnd();
    glColor3ub(60, 60, 66);
    /* rim of the headlamp */
    drawCircle(4.77f, -0.02f, 0.15f, 0.148f);
    glColor3ub(255, 250, 210);
    /* glass of the headlamp */
    drawCircle(4.77f, -0.02f, 0.105f, 0.103f);
    glColor3ub(210, 70, 60);
    /* red marker light, right */
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(4.75f, -1.12f);
    glVertex2f(4.825f, -1.12f);
    glVertex2f(4.818f, -1.087f);
    glVertex2f(4.797f, -1.061f);
    glVertex2f(4.767f, -1.047f);
    glVertex2f(4.733f, -1.047f);
    glVertex2f(4.703f, -1.061f);
    glVertex2f(4.682f, -1.087f);
    glVertex2f(4.675f, -1.12f);
    glVertex2f(4.682f, -1.153f);
    glVertex2f(4.703f, -1.179f);
    glVertex2f(4.733f, -1.193f);
    glVertex2f(4.767f, -1.193f);
    glVertex2f(4.797f, -1.179f);
    glVertex2f(4.818f, -1.153f);
    glVertex2f(4.825f, -1.12f);
    glEnd();
    /* red marker light, left */
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(4.33f, -1.12f);
    glVertex2f(4.405f, -1.12f);
    glVertex2f(4.398f, -1.087f);
    glVertex2f(4.377f, -1.061f);
    glVertex2f(4.347f, -1.047f);
    glVertex2f(4.313f, -1.047f);
    glVertex2f(4.283f, -1.061f);
    glVertex2f(4.262f, -1.087f);
    glVertex2f(4.255f, -1.12f);
    glVertex2f(4.262f, -1.153f);
    glVertex2f(4.283f, -1.179f);
    glVertex2f(4.313f, -1.193f);
    glVertex2f(4.347f, -1.193f);
    glVertex2f(4.377f, -1.179f);
    glVertex2f(4.398f, -1.153f);
    glVertex2f(4.405f, -1.12f);
    glEnd();
    glColor3ub(48, 48, 56);
    /* the two bogie frames the wheels hang from */
    glBegin(GL_QUADS);
    glVertex2f(0.095f, -1.9f);
    glVertex2f(2.105f, -1.9f);
    glVertex2f(2.105f, -1.54f);
    glVertex2f(0.095f, -1.54f);
    glVertex2f(2.745f, -1.9f);
    glVertex2f(4.755f, -1.9f);
    glVertex2f(4.755f, -1.54f);
    glVertex2f(2.745f, -1.54f);
    glEnd();
    glColor3ub(26, 26, 30);
    /* locomotive wheel 1, tyre */
    drawCircle(0.425f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* locomotive wheel 1, hub */
    drawCircle(0.425f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* locomotive wheel 1, axle cap */
    drawCircle(0.425f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* locomotive wheel 2, tyre */
    drawCircle(1.1f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* locomotive wheel 2, hub */
    drawCircle(1.1f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* locomotive wheel 2, axle cap */
    drawCircle(1.1f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* locomotive wheel 3, tyre */
    drawCircle(1.775f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* locomotive wheel 3, hub */
    drawCircle(1.775f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* locomotive wheel 3, axle cap */
    drawCircle(1.775f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* locomotive wheel 4, tyre */
    drawCircle(3.075f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* locomotive wheel 4, hub */
    drawCircle(3.075f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* locomotive wheel 4, axle cap */
    drawCircle(3.075f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* locomotive wheel 5, tyre */
    drawCircle(3.75f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* locomotive wheel 5, hub */
    drawCircle(3.75f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* locomotive wheel 5, axle cap */
    drawCircle(3.75f, -1.87f, 0.048f, 0.048f);
    glColor3ub(26, 26, 30);
    /* locomotive wheel 6, tyre */
    drawCircle(4.425f, -1.87f, 0.3f, 0.3f);
    glColor3ub(64, 64, 72);
    /* locomotive wheel 6, hub */
    drawCircle(4.425f, -1.87f, 0.174f, 0.174f);
    glColor3ub(120, 120, 130);
    /* locomotive wheel 6, axle cap */
    drawCircle(4.425f, -1.87f, 0.048f, 0.048f);

}

/* ---- one passenger, used for the five figures that share a shape ---- */
/* x is where he stands, r g b is the colour of his shirt */
void drawPassenger(float x, int r, int g, int b)
{
    glColor3ub(40, 42, 70);
    /* legs */
    glBegin(GL_QUADS);
    glVertex2f(x + -0.13f, 0.000f);
    glVertex2f(x + -0.025f, 0.000f);
    glVertex2f(x + -0.025f, 0.528f);
    glVertex2f(x + -0.13f, 0.528f);
    glVertex2f(x + 0.025f, 0.000f);
    glVertex2f(x + 0.13f, 0.000f);
    glVertex2f(x + 0.13f, 0.528f);
    glVertex2f(x + 0.025f, 0.528f);
    glEnd();
    glColor3ub(r, g, b);
    /* shirt and arms */
    glBegin(GL_QUADS);
    glVertex2f(x + -0.157f, 0.480f);
    glVertex2f(x + 0.157f, 0.480f);
    glVertex2f(x + 0.157f, 0.960f);
    glVertex2f(x + -0.157f, 0.960f);
    glVertex2f(x + -0.219f, 0.504f);
    glVertex2f(x + -0.157f, 0.504f);
    glVertex2f(x + -0.157f, 0.936f);
    glVertex2f(x + -0.219f, 0.936f);
    glVertex2f(x + 0.157f, 0.504f);
    glVertex2f(x + 0.219f, 0.504f);
    glVertex2f(x + 0.219f, 0.936f);
    glVertex2f(x + 0.157f, 0.936f);
    glEnd();
    glColor3ub(222, 178, 130);
    /* neck */
    glBegin(GL_QUADS);
    glVertex2f(x + -0.052f, 0.888f);
    glVertex2f(x + 0.052f, 0.888f);
    glVertex2f(x + 0.052f, 1.032f);
    glVertex2f(x + -0.052f, 1.032f);
    glEnd();
    glColor3ub(35, 30, 30);
    /* head and hair */
    drawCircle(x + 0.0f, 1.152f, 0.126f, 0.124f);
    glColor3ub(222, 178, 130);
    /* face */
    drawCircle(x + 0.0f, 1.08f, 0.105f, 0.103f);
}

/* ---- one bench, used for both of them ---- */
void drawBench(float x)
{
    glColor3ub(112, 66, 38);
    /* the two legs */
    glBegin(GL_QUADS);
    glVertex2f(x + -0.86f, 0.000f);
    glVertex2f(x + -0.72f, 0.000f);
    glVertex2f(x + -0.72f, 0.580f);
    glVertex2f(x + -0.86f, 0.580f);
    glVertex2f(x + 0.72f, 0.000f);
    glVertex2f(x + 0.86f, 0.000f);
    glVertex2f(x + 0.86f, 0.580f);
    glVertex2f(x + 0.72f, 0.580f);
    glEnd();
    glColor3ub(152, 96, 56);
    /* seat */
    glBegin(GL_QUADS);
    glVertex2f(x + -0.9f, 0.528f);
    glVertex2f(x + 0.9f, 0.528f);
    glVertex2f(x + 0.9f, 0.720f);
    glVertex2f(x + -0.9f, 0.720f);
    glVertex2f(x + -0.9f, 0.780f);
    glVertex2f(x + 0.9f, 0.780f);
    glVertex2f(x + 0.9f, 0.936f);
    glVertex2f(x + -0.9f, 0.936f);
    glVertex2f(x + -0.9f, 0.984f);
    glVertex2f(x + 0.9f, 0.984f);
    glVertex2f(x + 0.9f, 1.140f);
    glVertex2f(x + -0.9f, 1.140f);
    glEnd();
    glColor3ub(112, 66, 38);
    /* back rest */
    glBegin(GL_QUADS);
    glVertex2f(x + -0.86f, 0.720f);
    glVertex2f(x + -0.735f, 0.720f);
    glVertex2f(x + -0.735f, 1.188f);
    glVertex2f(x + -0.86f, 1.188f);
    glVertex2f(x + 0.735f, 0.720f);
    glVertex2f(x + 0.86f, 0.720f);
    glVertex2f(x + 0.86f, 1.188f);
    glVertex2f(x + 0.735f, 1.188f);
    glEnd();

}

/* draws the whole picture, back to front */
void display()
{

    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    glLoadIdentity(); // Reset the matrix

    gluOrtho2D(-12.0, 12.0, -5.0, 7.0); // Bigger world window = more drawing space

    /* ---- the light of this moment tints the whole picture ---- */
    glEnable(GL_LIGHTING);       // Enable Light Effect
    glEnable(GL_COLOR_MATERIAL); // let glColor3ub feed the material

    GLfloat global_ambient[] = {lightR, lightG, lightB, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    /* ================= sky ================= */
    /* sky, blue at the top fading pale down at the ground line */
    glBegin(GL_QUADS);
    glColor3ub(104, 172, 232);
    glVertex2f(-12.000f, 7.000f);
    glVertex2f(12.000f, 7.000f);
    glColor3ub(206, 234, 246);
    glVertex2f(12.000f, 0.000f);
    glVertex2f(-12.000f, 0.000f);
    glEnd();
    /* ================= stars and the moon : only at night ================= */
    if (nightMix > 0.0f)
    {
        glDisable(GL_LIGHTING); // the stars keep their own brightness
        /* sky behind the stars is about (139, 193, 237) before the tint */
        glColor3ub((int)(139.0f * lightR * (1.0f - nightMix) + 255.0f * nightMix),
                   (int)(193.0f * lightG * (1.0f - nightMix) + 255.0f * nightMix),
                   (int)(237.0f * lightB * (1.0f - nightMix) + 235.0f * nightMix));
        /* eighty little stars */
        /* eighty little stars : one point each, 80 of them */
        glPointSize(6.0f);
        glBegin(GL_POINTS);
        glVertex2f(-10.66f, 6.09f);
        glVertex2f(-6.733f, 2.965f);
        glVertex2f(-10.151f, 2.986f);
        glVertex2f(-10.405f, 5.003f);
        glVertex2f(3.083f, 5.078f);
        glVertex2f(1.82f, 4.286f);
        glVertex2f(-10.701f, 6.248f);
        glVertex2f(-4.52f, 6.069f);
        glVertex2f(-9.368f, 5.028f);
        glVertex2f(-3.011f, 4.928f);
        glVertex2f(4.257f, 4.417f);
        glVertex2f(-0.812f, 6.525f);
        glVertex2f(6.604f, 2.948f);
        glVertex2f(-1.208f, 5.188f);
        glVertex2f(-9.014f, 4.377f);
        glVertex2f(-8.213f, 4.678f);
        glVertex2f(-3.773f, 4.088f);
        glVertex2f(1.886f, 4.539f);
        glVertex2f(-10.266f, 5.707f);
        glVertex2f(3.472f, 6.821f);
        glVertex2f(-7.834f, 3.098f);
        glVertex2f(-5.243f, 3.182f);
        glVertex2f(8.59f, 3.783f);
        glVertex2f(11.481f, 5.502f);
        glVertex2f(10.802f, 3.241f);
        glVertex2f(-8.229f, 5.399f);
        glVertex2f(-3.086f, 5.007f);
        glVertex2f(3.657f, 5.744f);
        glVertex2f(7.03f, 4.268f);
        glVertex2f(-2.499f, 4.646f);
        glVertex2f(-10.331f, 2.886f);
        glVertex2f(-10.559f, 2.601f);
        glVertex2f(2.414f, 4.615f);
        glVertex2f(8.235f, 6.821f);
        glVertex2f(-0.463f, 3.925f);
        glVertex2f(-9.388f, 4.056f);
        glVertex2f(-3.263f, 5.533f);
        glVertex2f(8.574f, 5.559f);
        glVertex2f(0.434f, 6.46f);
        glVertex2f(-4.02f, 3.548f);
        glVertex2f(7.158f, 3.45f);
        glVertex2f(-3.409f, 2.723f);
        glVertex2f(6.847f, 4.607f);
        glVertex2f(7.282f, 5.673f);
        glVertex2f(10.738f, 4.15f);
        glVertex2f(-9.389f, 4.598f);
        glVertex2f(5.903f, 4.632f);
        glVertex2f(-1.559f, 5.302f);
        glEnd();
        glPointSize(9.0f);
        glBegin(GL_POINTS);
        glVertex2f(-4.158f, 3.241f);
        glVertex2f(-3.17f, 2.846f);
        glVertex2f(-1.908f, 4.898f);
        glVertex2f(1.519f, 5.231f);
        glVertex2f(-4.725f, 5.976f);
        glVertex2f(0.595f, 6.319f);
        glVertex2f(10.904f, 2.93f);
        glVertex2f(1.723f, 6.321f);
        glVertex2f(10.494f, 4.615f);
        glVertex2f(-5.084f, 4.24f);
        glVertex2f(-3.611f, 6.598f);
        glVertex2f(-6.65f, 3.822f);
        glVertex2f(-5.956f, 4.262f);
        glVertex2f(-9.898f, 4.509f);
        glVertex2f(-0.355f, 5.104f);
        glVertex2f(-5.146f, 3.219f);
        glVertex2f(4.496f, 4.791f);
        glVertex2f(9.429f, 5.915f);
        glVertex2f(-1.401f, 3.067f);
        glVertex2f(0.864f, 6.633f);
        glVertex2f(-11.198f, 6.316f);
        glVertex2f(-2.921f, 5.296f);
        glVertex2f(-0.505f, 5.541f);
        glVertex2f(-11.255f, 6.642f);
        glVertex2f(6.092f, 3.867f);
        glVertex2f(6.418f, 4.864f);
        glVertex2f(7.223f, 6.078f);
        glVertex2f(4.544f, 6.665f);
        glVertex2f(-6.977f, 5.252f);
        glVertex2f(8.034f, 4.638f);
        glVertex2f(-3.681f, 5.333f);
        glVertex2f(-8.97f, 4.251f);
        glEnd();
        glPointSize(1.0f);
        /* sky behind the moon is about (116, 180, 234) before the tint */
        glColor3ub((int)(116.0f * lightR * (1.0f - nightMix) + 232.0f * nightMix),
                   (int)(180.0f * lightG * (1.0f - nightMix) + 232.0f * nightMix),
                   (int)(234.0f * lightB * (1.0f - nightMix) + 210.0f * nightMix));
        /* moon, outer disc */
        drawCircle(-4.5f, 6.15f, 0.74f, 0.74f);
        glColor3ub((int)(116.0f * lightR * (1.0f - nightMix) + 255.0f * nightMix),
                   (int)(180.0f * lightG * (1.0f - nightMix) + 255.0f * nightMix),
                   (int)(234.0f * lightB * (1.0f - nightMix) + 240.0f * nightMix));
        /* moon, bright inner disc */
        drawCircle(-4.5f, 6.15f, 0.56f, 0.56f);
        glEnable(GL_LIGHTING);
    }

    if (sunMix > 0.0f)
    {
        /* ---- sun : sinks behind the station as sunMix falls to 00 ---- */
        glPushMatrix();
        glTranslatef(0.0f, -5.0f * (1.0f - sunMix), 0.0f);
        glColor3ub(255, 244, 196);
        /* sun, outer disc */
        drawCircle(6.2f, 6.1f, 0.75f, 0.745f);
        glColor3ub(255, 250, 220);
        /* sun, bright inner disc */
        drawCircle(6.2f, 6.1f, 0.52f, 0.516f);
        glPopMatrix();
    }

    /* ---- clouds : the second copy walks in as the first walks out ---- */
    drawClouds(cloudDrift);
    drawClouds(cloudDrift - 24.0f); /* 24 is the width of the world */

    /* ================= trees behind the station ================= */
    glColor3ub(66, 142, 74);
    /* far tree 1, back row */
    drawCircle(6.479f, 1.62f, 0.993f, 0.986f);
    /* far tree 2, back row */
    drawCircle(7.733f, 1.98f, 1.15f, 1.142f);
    /* far tree 3, back row */
    drawCircle(8.987f, 1.68f, 1.045f, 1.037f);
    /* far tree 4, back row */
    drawCircle(10.241f, 1.92f, 1.097f, 1.089f);
    /* far tree 5, back row */
    drawCircle(11.39f, 1.62f, 0.993f, 0.986f);
    glColor3ub(86, 165, 90);
    /* far tree 1, front row */
    drawCircle(7.21f, 2.22f, 0.784f, 0.778f);
    /* far tree 2, front row */
    drawCircle(8.464f, 2.34f, 0.732f, 0.727f);
    /* far tree 3, front row */
    drawCircle(9.719f, 2.28f, 0.783f, 0.777f);
    /* far tree 4, front row */
    drawCircle(10.868f, 2.16f, 0.732f, 0.727f);

    /* ================= boundary wall ================= */
    glColor3ub(226, 192, 182);
    /* face of the wall */
    glBegin(GL_QUADS);
    glVertex2f(-12.540f, 0.000f);
    glVertex2f(-6.165f, 0.000f);
    glVertex2f(-6.165f, 1.260f);
    glVertex2f(-12.540f, 1.260f);
    glEnd();
    glColor3ub(178, 74, 60);
    /* coping along the top of the wall */
    glBegin(GL_QUADS);
    glVertex2f(-12.540f, 1.260f);
    glVertex2f(-6.165f, 1.260f);
    glVertex2f(-6.165f, 1.488f);
    glVertex2f(-12.540f, 1.488f);
    glEnd();
    glColor3ub(205, 168, 158);
    /* the six pillars set into the wall */
    glBegin(GL_QUADS);
    glVertex2f(-12.122f, 0.000f);
    glVertex2f(-11.976f, 0.000f);
    glVertex2f(-11.976f, 1.260f);
    glVertex2f(-12.122f, 1.260f);
    glVertex2f(-10.973f, 0.000f);
    glVertex2f(-10.826f, 0.000f);
    glVertex2f(-10.826f, 1.260f);
    glVertex2f(-10.973f, 1.260f);
    glVertex2f(-9.823f, 0.000f);
    glVertex2f(-9.677f, 0.000f);
    glVertex2f(-9.677f, 1.260f);
    glVertex2f(-9.823f, 1.260f);
    glVertex2f(-8.674f, 0.000f);
    glVertex2f(-8.527f, 0.000f);
    glVertex2f(-8.527f, 1.260f);
    glVertex2f(-8.674f, 1.260f);
    glVertex2f(-7.524f, 0.000f);
    glVertex2f(-7.378f, 0.000f);
    glVertex2f(-7.378f, 1.260f);
    glVertex2f(-7.524f, 1.260f);
    glVertex2f(-6.375f, 0.000f);
    glVertex2f(-6.229f, 0.000f);
    glVertex2f(-6.229f, 1.260f);
    glVertex2f(-6.375f, 1.260f);
    glEnd();
    glColor3ub(196, 160, 150);
    /* dark plinth at the foot */
    glBegin(GL_QUADS);
    glVertex2f(-12.540f, 0.000f);
    glVertex2f(-6.165f, 0.000f);
    glVertex2f(-6.165f, 0.144f);
    glVertex2f(-12.540f, 0.144f);
    glEnd();

    /* ================= small house ================= */
    glColor3ub(244, 222, 140);
    /* wall of the house */
    glBegin(GL_QUADS);
    glVertex2f(-11.077f, 0.000f);
    glVertex2f(-8.987f, 0.000f);
    glVertex2f(-8.987f, 2.100f);
    glVertex2f(-11.077f, 2.100f);
    glEnd();
    glColor3ub(214, 192, 112);
    /* band under the eaves */
    glBegin(GL_QUADS);
    glVertex2f(-11.077f, 1.980f);
    glVertex2f(-8.987f, 1.980f);
    glVertex2f(-8.987f, 2.100f);
    glVertex2f(-11.077f, 2.100f);
    glEnd();
    glColor3ub(56, 130, 92);
    /* roof */
    glBegin(GL_QUADS);
    glVertex2f(-11.443f, 2.100f);
    glVertex2f(-8.621f, 2.100f);
    glVertex2f(-8.987f, 2.820f);
    glVertex2f(-11.077f, 2.820f);
    glEnd();
    glColor3ub(40, 105, 72);
    /* front edge of the roof */
    glBegin(GL_QUADS);
    glVertex2f(-11.443f, 2.100f);
    glVertex2f(-8.621f, 2.100f);
    glVertex2f(-8.621f, 2.232f);
    glVertex2f(-11.443f, 2.232f);
    glEnd();
    glColor3ub(160, 200, 225);
    /* the two windows */
    glBegin(GL_QUADS);
    glVertex2f(-10.764f, 1.584f);
    glVertex2f(-10.345f, 1.584f);
    glVertex2f(-10.345f, 1.920f);
    glVertex2f(-10.764f, 1.920f);
    glVertex2f(-9.614f, 1.584f);
    glVertex2f(-9.196f, 1.584f);
    glVertex2f(-9.196f, 1.920f);
    glVertex2f(-9.614f, 1.920f);
    glEnd();
    glColor3ub(95, 62, 38);
    /* sill under each window */
    glBegin(GL_QUADS);
    glVertex2f(-10.764f, 1.584f);
    glVertex2f(-10.345f, 1.584f);
    glVertex2f(-10.345f, 1.632f);
    glVertex2f(-10.764f, 1.632f);
    glVertex2f(-9.614f, 1.584f);
    glVertex2f(-9.196f, 1.584f);
    glVertex2f(-9.196f, 1.632f);
    glVertex2f(-9.614f, 1.632f);
    glEnd();

    /* ================= leafy trees ================= */
    glColor3ub(120, 80, 50);
    /* trunk */
    glBegin(GL_QUADS);
    glVertex2f(-11.946f, 0.000f);
    glVertex2f(-11.674f, 0.000f);
    glVertex2f(-11.674f, 3.240f);
    glVertex2f(-11.946f, 3.240f);
    glEnd();
    glColor3ub(58, 128, 62);
    /* crown, lower left puff */
    drawCircle(-12.41f, 3.24f, 0.878f, 0.872f);
    /* crown, lower right puff */
    drawCircle(-11.2f, 3.05f, 0.823f, 0.817f);
    glColor3ub(80, 158, 80);
    /* crown, top puff */
    drawCircle(-11.81f, 3.807f, 0.988f, 0.981f);
    /* crown, small puff on the left */
    drawCircle(-12.58f, 3.492f, 0.604f, 0.6f);
    glColor3ub(120, 80, 50);
    /* trunk */
    glBegin(GL_QUADS);
    glVertex2f(-10.560f, 0.000f);
    glVertex2f(-10.340f, 0.000f);
    glVertex2f(-10.340f, 3.120f);
    glVertex2f(-10.560f, 3.120f);
    glEnd();
    glColor3ub(58, 128, 62);
    /* crown, lower left puff */
    drawCircle(-10.936f, 3.12f, 0.711f, 0.706f);
    /* crown, lower right puff */
    drawCircle(-9.956f, 2.966f, 0.667f, 0.662f);
    glColor3ub(80, 158, 80);
    /* crown, top puff */
    drawCircle(-10.45f, 3.579f, 0.8f, 0.795f);
    /* crown, small puff on the left */
    drawCircle(-11.074f, 3.324f, 0.49f, 0.486f);
    glColor3ub(120, 80, 50);
    /* trunk */
    glBegin(GL_QUADS);
    glVertex2f(-7.611f, 0.000f);
    glVertex2f(-7.429f, 0.000f);
    glVertex2f(-7.429f, 2.760f);
    glVertex2f(-7.611f, 2.760f);
    glEnd();
    glColor3ub(58, 128, 62);
    /* crown, lower left puff */
    drawCircle(-7.922f, 2.76f, 0.588f, 0.584f);
    /* crown, lower right puff */
    drawCircle(-7.111f, 2.633f, 0.552f, 0.548f);
    glColor3ub(80, 158, 80);
    /* crown, top puff */
    drawCircle(-7.52f, 3.14f, 0.662f, 0.657f);
    /* crown, small puff on the left */
    drawCircle(-8.036f, 2.929f, 0.405f, 0.402f);

    /* ================= palm tree ================= */
    glColor3ub(155, 115, 68);
    /* trunk of the palm */
    glBegin(GL_QUADS);
    glVertex2f(-10.200f, 0.000f);
    glVertex2f(-9.865f, 0.000f);
    glVertex2f(-9.927f, 4.920f);
    glVertex2f(-10.136f, 4.920f);
    glEnd();
    glColor3ub(130, 92, 52);
    /* the rings up the trunk */
    glBegin(GL_QUADS);
    glVertex2f(-10.190f, 0.000f);
    glVertex2f(-9.875f, 0.000f);
    glVertex2f(-9.875f, 0.072f);
    glVertex2f(-10.190f, 0.072f);
    glVertex2f(-10.183f, 0.547f);
    glVertex2f(-9.882f, 0.547f);
    glVertex2f(-9.882f, 0.619f);
    glVertex2f(-10.183f, 0.619f);
    glVertex2f(-10.176f, 1.094f);
    glVertex2f(-9.889f, 1.094f);
    glVertex2f(-9.889f, 1.166f);
    glVertex2f(-10.176f, 1.166f);
    glVertex2f(-10.169f, 1.641f);
    glVertex2f(-9.896f, 1.641f);
    glVertex2f(-9.896f, 1.713f);
    glVertex2f(-10.169f, 1.713f);
    glVertex2f(-10.162f, 2.188f);
    glVertex2f(-9.903f, 2.188f);
    glVertex2f(-9.903f, 2.260f);
    glVertex2f(-10.162f, 2.260f);
    glVertex2f(-10.154f, 2.735f);
    glVertex2f(-9.909f, 2.735f);
    glVertex2f(-9.909f, 2.807f);
    glVertex2f(-10.154f, 2.807f);
    glVertex2f(-10.147f, 3.282f);
    glVertex2f(-9.916f, 3.282f);
    glVertex2f(-9.916f, 3.354f);
    glVertex2f(-10.147f, 3.354f);
    glVertex2f(-10.140f, 3.829f);
    glVertex2f(-9.923f, 3.829f);
    glVertex2f(-9.923f, 3.901f);
    glVertex2f(-10.140f, 3.901f);
    glVertex2f(-10.133f, 4.376f);
    glVertex2f(-9.930f, 4.376f);
    glVertex2f(-9.930f, 4.448f);
    glVertex2f(-10.133f, 4.448f);
    glEnd();
    glColor3ub(46, 132, 66);
    /* the seven long fronds */
    glBegin(GL_TRIANGLES);
    glVertex2f(-10.178f, 4.800f);
    glVertex2f(-9.886f, 4.800f);
    glVertex2f(-11.696f, 5.098f);
    glVertex2f(-10.178f, 4.800f);
    glVertex2f(-9.886f, 4.800f);
    glVertex2f(-11.355f, 5.565f);
    glVertex2f(-10.178f, 4.800f);
    glVertex2f(-9.886f, 4.800f);
    glVertex2f(-10.762f, 5.886f);
    glVertex2f(-10.178f, 4.800f);
    glVertex2f(-9.886f, 4.800f);
    glVertex2f(-10.030f, 6.000f);
    glVertex2f(-10.178f, 4.800f);
    glVertex2f(-9.886f, 4.800f);
    glVertex2f(-9.298f, 5.886f);
    glVertex2f(-10.178f, 4.800f);
    glVertex2f(-9.886f, 4.800f);
    glVertex2f(-8.705f, 5.565f);
    glVertex2f(-10.178f, 4.800f);
    glVertex2f(-9.886f, 4.800f);
    glVertex2f(-8.364f, 5.098f);
    glEnd();
    glColor3ub(34, 108, 52);
    /* four shorter fronds in front */
    glBegin(GL_TRIANGLES);
    glVertex2f(-10.158f, 4.860f);
    glVertex2f(-9.906f, 4.860f);
    glVertex2f(-11.314f, 5.123f);
    glVertex2f(-10.158f, 4.860f);
    glVertex2f(-9.906f, 4.860f);
    glVertex2f(-10.605f, 5.629f);
    glVertex2f(-10.158f, 4.860f);
    glVertex2f(-9.906f, 4.860f);
    glVertex2f(-9.533f, 5.650f);
    glVertex2f(-10.158f, 4.860f);
    glVertex2f(-9.906f, 4.860f);
    glVertex2f(-8.775f, 5.173f);
    glEnd();
    glColor3ub(120, 85, 45);
    /* coconut, left */
    drawCircle(-10.2f, 4.776f, 0.105f, 0.104f);
    /* coconut, right */
    drawCircle(-9.865f, 4.728f, 0.105f, 0.104f);

    /* ================= station building wall ================= */
    glColor3ub(238, 222, 178);
    /* left wall of the station, the taller part */
    glBegin(GL_QUADS);
    glVertex2f(-2.717f, 0.000f);
    glVertex2f(3.344f, 0.000f);
    glVertex2f(3.344f, 2.880f);
    glVertex2f(-2.717f, 2.880f);
    glEnd();
    glColor3ub(228, 208, 156);
    /* right wall of the station, the lower part */
    glBegin(GL_QUADS);
    glVertex2f(3.344f, 0.000f);
    glVertex2f(6.792f, 0.000f);
    glVertex2f(6.792f, 2.664f);
    glVertex2f(3.344f, 2.664f);
    glEnd();
    glColor3ub(248, 244, 232);
    /* darker end panel on the left */
    glBegin(GL_QUADS);
    glVertex2f(-2.717f, 0.000f);
    glVertex2f(-1.045f, 0.000f);
    glVertex2f(-1.045f, 2.880f);
    glVertex2f(-2.717f, 2.880f);
    glEnd();
    glColor3ub(205, 185, 135);
    /* band along the top of the left wall */
    glBegin(GL_QUADS);
    glVertex2f(-2.717f, 2.688f);
    glVertex2f(3.344f, 2.688f);
    glVertex2f(3.344f, 2.880f);
    glVertex2f(-2.717f, 2.880f);
    glEnd();
    glColor3ub(198, 178, 128);
    /* band along the top of the right wall */
    glBegin(GL_QUADS);
    glVertex2f(3.344f, 2.472f);
    glVertex2f(6.792f, 2.472f);
    glVertex2f(6.792f, 2.664f);
    glVertex2f(3.344f, 2.664f);
    glEnd();
    glColor3ub(160, 140, 105);
    /* dark plinth along the foot of the wall */
    glBegin(GL_QUADS);
    glVertex2f(-2.717f, 0.000f);
    glVertex2f(6.792f, 0.000f);
    glVertex2f(6.792f, 0.264f);
    glVertex2f(-2.717f, 0.264f);
    glEnd();
    /* ---- framed photographs ---- */
    glColor3ub(90, 60, 40);
    /* outer frame of the picture */
    glBegin(GL_QUADS);
    glVertex2f(-2.560f, 1.200f);
    glVertex2f(-1.202f, 1.200f);
    glVertex2f(-1.202f, 2.064f);
    glVertex2f(-2.560f, 2.064f);
    glEnd();
    glColor3ub(250, 250, 245);
    /* mount inside the frame */
    glBegin(GL_QUADS);
    glVertex2f(-2.508f, 1.260f);
    glVertex2f(-1.254f, 1.260f);
    glVertex2f(-1.254f, 2.004f);
    glVertex2f(-2.508f, 2.004f);
    glEnd();
    glColor3ub(190, 70, 60);
    /* left picture */
    glBegin(GL_QUADS);
    glVertex2f(-2.424f, 1.416f);
    glVertex2f(-2.006f, 1.416f);
    glVertex2f(-2.006f, 1.896f);
    glVertex2f(-2.424f, 1.896f);
    glEnd();
    glColor3ub(70, 110, 170);
    /* right picture */
    glBegin(GL_QUADS);
    glVertex2f(-1.933f, 1.416f);
    glVertex2f(-1.515f, 1.416f);
    glVertex2f(-1.515f, 1.896f);
    glVertex2f(-1.933f, 1.896f);
    glEnd();
    /* ---- platform number board 2 ---- */
    glColor3ub(255, 255, 255);
    /* frame of the platform board */
    glBegin(GL_QUADS);
    glVertex2f(3.450f, 1.740f);
    glVertex2f(4.620f, 1.740f);
    glVertex2f(4.620f, 2.660f);
    glVertex2f(3.450f, 2.660f);
    glEnd();
    glColor3ub(28, 62, 140);
    /* blue face of the board */
    glBegin(GL_QUADS);
    glVertex2f(3.510f, 1.800f);
    glVertex2f(4.560f, 1.800f);
    glVertex2f(4.560f, 2.600f);
    glVertex2f(3.510f, 2.600f);
    glEnd();
    glColor3ub(255, 255, 255);
    /* the word PLATFORM in block letters */
    glLineWidth(2.0f);
    drawText("PLATFORM", 3.577f, 2.36f, 0.00139f);
    glLineWidth(1.0f);
    /* the figure 2 below it */
    glLineWidth(4.0f);
    drawText("2", 3.86574f, 1.9f, 0.00434f);
    glLineWidth(1.0f);

    /* ---- yellow shutter door ---- */
    glColor3ub(60, 60, 60);
    /* frame of the shutter door */
    glBegin(GL_QUADS);
    glVertex2f(-0.648f, 0.180f);
    glVertex2f(0.543f, 0.180f);
    glVertex2f(0.543f, 2.232f);
    glVertex2f(-0.648f, 2.232f);
    glEnd();
    glColor3ub(222, 208, 60);
    /* the shutter itself */
    glBegin(GL_QUADS);
    glVertex2f(-0.564f, 0.240f);
    glVertex2f(0.460f, 0.240f);
    glVertex2f(0.460f, 2.148f);
    glVertex2f(-0.564f, 2.148f);
    glEnd();
    glColor3ub(150, 140, 40);
    /* the seven slats across the shutter */
    glBegin(GL_QUADS);
    glVertex2f(-0.522f, 0.240f);
    glVertex2f(-0.480f, 0.240f);
    glVertex2f(-0.480f, 2.148f);
    glVertex2f(-0.522f, 2.148f);
    glVertex2f(-0.376f, 0.240f);
    glVertex2f(-0.334f, 0.240f);
    glVertex2f(-0.334f, 2.148f);
    glVertex2f(-0.376f, 2.148f);
    glVertex2f(-0.230f, 0.240f);
    glVertex2f(-0.188f, 0.240f);
    glVertex2f(-0.188f, 2.148f);
    glVertex2f(-0.230f, 2.148f);
    glVertex2f(-0.084f, 0.240f);
    glVertex2f(-0.042f, 0.240f);
    glVertex2f(-0.042f, 2.148f);
    glVertex2f(-0.084f, 2.148f);
    glVertex2f(0.062f, 0.240f);
    glVertex2f(0.104f, 0.240f);
    glVertex2f(0.104f, 2.148f);
    glVertex2f(0.062f, 2.148f);
    glVertex2f(0.208f, 0.240f);
    glVertex2f(0.250f, 0.240f);
    glVertex2f(0.250f, 2.148f);
    glVertex2f(0.208f, 2.148f);
    glVertex2f(0.354f, 0.240f);
    glVertex2f(0.396f, 0.240f);
    glVertex2f(0.396f, 2.148f);
    glVertex2f(0.354f, 2.148f);
    glEnd();
    /* ---- wooden doors ---- */
    glColor3ub(95, 62, 38);
    /* frame of the double door */
    glBegin(GL_QUADS);
    glVertex2f(1.776f, 0.180f);
    glVertex2f(3.240f, 0.180f);
    glVertex2f(3.240f, 2.184f);
    glVertex2f(1.776f, 2.184f);
    glEnd();
    glColor3ub(196, 150, 92);
    /* the two door leaves */
    glBegin(GL_QUADS);
    glVertex2f(1.839f, 0.240f);
    glVertex2f(2.466f, 0.240f);
    glVertex2f(2.466f, 2.112f);
    glVertex2f(1.839f, 2.112f);
    glVertex2f(2.550f, 0.240f);
    glVertex2f(3.177f, 0.240f);
    glVertex2f(3.177f, 2.112f);
    glVertex2f(2.550f, 2.112f);
    glEnd();
    glColor3ub(150, 108, 62);
    /* upper panel in each leaf */
    glBegin(GL_QUADS);
    glVertex2f(1.923f, 1.176f);
    glVertex2f(2.383f, 1.176f);
    glVertex2f(2.383f, 1.992f);
    glVertex2f(1.923f, 1.992f);
    glVertex2f(2.633f, 1.176f);
    glVertex2f(3.093f, 1.176f);
    glVertex2f(3.093f, 1.992f);
    glVertex2f(2.633f, 1.992f);
    glEnd();
    glColor3ub(95, 62, 38);
    /* frame of the single door */
    glBegin(GL_QUADS);
    glVertex2f(4.441f, 0.180f);
    glVertex2f(5.277f, 0.180f);
    glVertex2f(5.277f, 2.184f);
    glVertex2f(4.441f, 2.184f);
    glEnd();
    glColor3ub(196, 150, 92);
    /* its leaf */
    glBegin(GL_QUADS);
    glVertex2f(4.504f, 0.240f);
    glVertex2f(5.215f, 0.240f);
    glVertex2f(5.215f, 2.112f);
    glVertex2f(4.504f, 2.112f);
    glEnd();
    glColor3ub(150, 108, 62);
    /* upper panel in the leaf */
    glBegin(GL_QUADS);
    glVertex2f(4.598f, 1.176f);
    glVertex2f(5.120f, 1.176f);
    glVertex2f(5.120f, 1.992f);
    glVertex2f(4.598f, 1.992f);
    glEnd();
    /* ---- window ---- */
    glColor3ub(95, 62, 38);
    /* window frame */
    glBegin(GL_QUADS);
    glVertex2f(5.800f, 1.296f);
    glVertex2f(6.636f, 1.296f);
    glVertex2f(6.636f, 2.232f);
    glVertex2f(5.800f, 2.232f);
    glEnd();
    glColor3ub(168, 205, 228);
    /* window pane */
    glBegin(GL_QUADS);
    glVertex2f(5.862f, 1.368f);
    glVertex2f(6.573f, 1.368f);
    glVertex2f(6.573f, 2.160f);
    glVertex2f(5.862f, 2.160f);
    glEnd();
    glColor3ub(95, 62, 38);
    /* the glazing bars across the pane */
    glBegin(GL_QUADS);
    glVertex2f(6.197f, 1.296f);
    glVertex2f(6.260f, 1.296f);
    glVertex2f(6.260f, 2.232f);
    glVertex2f(6.197f, 2.232f);
    glVertex2f(5.800f, 1.728f);
    glVertex2f(6.636f, 1.728f);
    glVertex2f(6.636f, 1.800f);
    glVertex2f(5.800f, 1.800f);
    glEnd();

    /* ================= canopy pillars ================= */
    glColor3ub(152, 62, 46);
    /* the five pillars holding the canopy */
    glBegin(GL_QUADS);
    glVertex2f(1.306f, 0.000f);
    glVertex2f(1.620f, 0.000f);
    glVertex2f(1.620f, 3.098f);
    glVertex2f(1.306f, 3.098f);
    glVertex2f(3.292f, 0.000f);
    glVertex2f(3.606f, 0.000f);
    glVertex2f(3.606f, 3.034f);
    glVertex2f(3.292f, 3.034f);
    glVertex2f(5.277f, 0.000f);
    glVertex2f(5.591f, 0.000f);
    glVertex2f(5.591f, 2.970f);
    glVertex2f(5.277f, 2.970f);
    glVertex2f(7.263f, 0.000f);
    glVertex2f(7.577f, 0.000f);
    glVertex2f(7.577f, 2.906f);
    glVertex2f(7.263f, 2.906f);
    glVertex2f(9.091f, 0.000f);
    glVertex2f(9.405f, 0.000f);
    glVertex2f(9.405f, 2.848f);
    glVertex2f(9.091f, 2.848f);
    glEnd();
    glColor3ub(120, 45, 34);
    /* the base under each pillar */
    glBegin(GL_QUADS);
    glVertex2f(1.212f, 0.000f);
    glVertex2f(1.714f, 0.000f);
    glVertex2f(1.714f, 0.264f);
    glVertex2f(1.212f, 0.264f);
    glVertex2f(3.198f, 0.000f);
    glVertex2f(3.700f, 0.000f);
    glVertex2f(3.700f, 0.264f);
    glVertex2f(3.198f, 0.264f);
    glVertex2f(5.183f, 0.000f);
    glVertex2f(5.685f, 0.000f);
    glVertex2f(5.685f, 0.264f);
    glVertex2f(5.183f, 0.264f);
    glVertex2f(7.169f, 0.000f);
    glVertex2f(7.671f, 0.000f);
    glVertex2f(7.671f, 0.264f);
    glVertex2f(7.169f, 0.264f);
    glVertex2f(8.997f, 0.000f);
    glVertex2f(9.499f, 0.000f);
    glVertex2f(9.499f, 0.264f);
    glVertex2f(8.997f, 0.264f);
    glEnd();
    glColor3ub(132, 52, 40);
    /* the brackets where each pillar meets the roof */
    glBegin(GL_TRIANGLES);
    glVertex2f(1.306f, 2.642f);
    glVertex2f(1.306f, 3.098f);
    glVertex2f(0.920f, 3.098f);
    glVertex2f(1.620f, 2.642f);
    glVertex2f(1.620f, 3.098f);
    glVertex2f(2.006f, 3.098f);
    glVertex2f(3.292f, 2.578f);
    glVertex2f(3.292f, 3.034f);
    glVertex2f(2.906f, 3.034f);
    glVertex2f(3.606f, 2.578f);
    glVertex2f(3.606f, 3.034f);
    glVertex2f(3.992f, 3.034f);
    glVertex2f(5.277f, 2.514f);
    glVertex2f(5.277f, 2.970f);
    glVertex2f(4.891f, 2.970f);
    glVertex2f(5.591f, 2.514f);
    glVertex2f(5.591f, 2.970f);
    glVertex2f(5.977f, 2.970f);
    glVertex2f(7.263f, 2.450f);
    glVertex2f(7.263f, 2.906f);
    glVertex2f(6.877f, 2.906f);
    glVertex2f(7.577f, 2.450f);
    glVertex2f(7.577f, 2.906f);
    glVertex2f(7.963f, 2.906f);
    glVertex2f(9.091f, 2.392f);
    glVertex2f(9.091f, 2.848f);
    glVertex2f(8.705f, 2.848f);
    glVertex2f(9.405f, 2.392f);
    glVertex2f(9.405f, 2.848f);
    glVertex2f(9.791f, 2.848f);
    glEnd();

    /* ================= the shade : long canopy roof ================= */
    glColor3ub(150, 48, 38);
    /* eave board under the canopy */
    glBegin(GL_QUADS);
    glVertex2f(-2.508f, 2.986f);
    glVertex2f(9.823f, 2.589f);
    glVertex2f(9.823f, 2.829f);
    glVertex2f(-2.508f, 3.226f);
    glEnd();
    glColor3ub(176, 62, 48);
    /* the canopy roof itself */
    glBegin(GL_QUADS);
    glVertex2f(-2.508f, 3.226f);
    glVertex2f(9.823f, 2.829f);
    glVertex2f(9.823f, 3.849f);
    glVertex2f(-2.508f, 4.246f);
    glEnd();
    glColor3ub(140, 42, 32);
    /* the corrugations running down the roof */
    /* the corrugations running down the roof : one line each, 34 of them */
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-2.185f, 3.216f);
    glVertex2f(-2.185f, 4.236f);
    glVertex2f(-1.829f, 3.205f);
    glVertex2f(-1.829f, 4.225f);
    glVertex2f(-1.474f, 3.193f);
    glVertex2f(-1.474f, 4.213f);
    glVertex2f(-1.118f, 3.182f);
    glVertex2f(-1.118f, 4.202f);
    glVertex2f(-0.763f, 3.17f);
    glVertex2f(-0.763f, 4.19f);
    glVertex2f(-0.407f, 3.159f);
    glVertex2f(-0.407f, 4.179f);
    glVertex2f(-0.052f, 3.147f);
    glVertex2f(-0.052f, 4.167f);
    glVertex2f(0.303f, 3.136f);
    glVertex2f(0.303f, 4.156f);
    glVertex2f(0.659f, 3.125f);
    glVertex2f(0.659f, 4.145f);
    glVertex2f(1.015f, 3.113f);
    glVertex2f(1.015f, 4.133f);
    glVertex2f(1.37f, 3.102f);
    glVertex2f(1.37f, 4.122f);
    glVertex2f(1.726f, 3.09f);
    glVertex2f(1.726f, 4.11f);
    glVertex2f(2.081f, 3.079f);
    glVertex2f(2.081f, 4.099f);
    glVertex2f(2.437f, 3.067f);
    glVertex2f(2.437f, 4.087f);
    glVertex2f(2.792f, 3.056f);
    glVertex2f(2.792f, 4.076f);
    glVertex2f(3.147f, 3.045f);
    glVertex2f(3.147f, 4.065f);
    glVertex2f(3.503f, 3.033f);
    glVertex2f(3.503f, 4.053f);
    glVertex2f(3.859f, 3.022f);
    glVertex2f(3.859f, 4.042f);
    glVertex2f(4.214f, 3.01f);
    glVertex2f(4.214f, 4.03f);
    glVertex2f(4.569f, 2.999f);
    glVertex2f(4.569f, 4.019f);
    glVertex2f(4.925f, 2.987f);
    glVertex2f(4.925f, 4.007f);
    glVertex2f(5.2805f, 2.976f);
    glVertex2f(5.2805f, 3.996f);
    glVertex2f(5.636f, 2.965f);
    glVertex2f(5.636f, 3.985f);
    glVertex2f(5.991f, 2.953f);
    glVertex2f(5.991f, 3.973f);
    glVertex2f(6.347f, 2.942f);
    glVertex2f(6.347f, 3.962f);
    glVertex2f(6.702f, 2.93f);
    glVertex2f(6.702f, 3.95f);
    glVertex2f(7.058f, 2.919f);
    glVertex2f(7.058f, 3.939f);
    glVertex2f(7.413f, 2.907f);
    glVertex2f(7.413f, 3.927f);
    glVertex2f(7.769f, 2.896f);
    glVertex2f(7.769f, 3.916f);
    glVertex2f(8.124f, 2.885f);
    glVertex2f(8.124f, 3.905f);
    glVertex2f(8.48f, 2.873f);
    glVertex2f(8.48f, 3.893f);
    glVertex2f(8.835f, 2.862f);
    glVertex2f(8.835f, 3.882f);
    glVertex2f(9.191f, 2.85f);
    glVertex2f(9.191f, 3.87f);
    glVertex2f(9.546f, 2.839f);
    glVertex2f(9.546f, 3.859f);
    glEnd();
    glLineWidth(1.0f);
    glColor3ub(118, 36, 28);
    /* ridge cap along the top */
    glBegin(GL_QUADS);
    glVertex2f(-2.508f, 4.246f);
    glVertex2f(9.823f, 3.849f);
    glVertex2f(9.823f, 4.041f);
    glVertex2f(-2.508f, 4.438f);
    glEnd();
    glColor3ub(132, 42, 34);
    /* end wall closing the right side */
    glBegin(GL_QUADS);
    glVertex2f(9.823f, 2.589f);
    glVertex2f(9.990f, 2.589f);
    glVertex2f(9.990f, 4.041f);
    glVertex2f(9.823f, 4.041f);
    glEnd();

    /* ================= gable roof over the entrance ================= */
    glColor3ub(120, 38, 30);
    /* fascia across the front */
    glBegin(GL_QUADS);
    glVertex2f(-7.054f, 2.760f);
    glVertex2f(-0.366f, 2.760f);
    glVertex2f(-0.366f, 3.384f);
    glVertex2f(-7.054f, 3.384f);
    glEnd();
    glColor3ub(150, 48, 38);
    /* eave band above it */
    glBegin(GL_QUADS);
    glVertex2f(-7.054f, 2.856f);
    glVertex2f(-0.366f, 2.856f);
    glVertex2f(-0.366f, 3.384f);
    glVertex2f(-7.054f, 3.384f);
    glEnd();
    glColor3ub(148, 46, 36);
    /* the gable triangle */
    glBegin(GL_TRIANGLES);
    glVertex2f(-7.054f, 3.288f);
    glVertex2f(-0.366f, 3.288f);
    glVertex2f(-3.657f, 5.304f);
    glEnd();
    glColor3ub(190, 74, 54);
    /* lighter triangle inside it */
    glBegin(GL_TRIANGLES);
    glVertex2f(-6.792f, 3.384f);
    glVertex2f(-0.575f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glEnd();
    glColor3ub(152, 50, 40);
    /* the ribs running down the gable */
    glBegin(GL_TRIANGLES);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-6.510f, 3.384f);
    glVertex2f(-6.480f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-6.227f, 3.384f);
    glVertex2f(-6.197f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-5.945f, 3.384f);
    glVertex2f(-5.915f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-5.662f, 3.384f);
    glVertex2f(-5.632f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-5.379f, 3.384f);
    glVertex2f(-5.349f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-5.096f, 3.384f);
    glVertex2f(-5.066f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-4.814f, 3.384f);
    glVertex2f(-4.784f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-4.531f, 3.384f);
    glVertex2f(-4.501f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-4.248f, 3.384f);
    glVertex2f(-4.218f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-3.966f, 3.384f);
    glVertex2f(-3.936f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-3.683f, 3.384f);
    glVertex2f(-3.653f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-3.400f, 3.384f);
    glVertex2f(-3.370f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-3.118f, 3.384f);
    glVertex2f(-3.088f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-2.835f, 3.384f);
    glVertex2f(-2.805f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-2.552f, 3.384f);
    glVertex2f(-2.522f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-2.269f, 3.384f);
    glVertex2f(-2.240f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-1.987f, 3.384f);
    glVertex2f(-1.957f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-1.704f, 3.384f);
    glVertex2f(-1.674f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-1.421f, 3.384f);
    glVertex2f(-1.391f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-1.139f, 3.384f);
    glVertex2f(-1.109f, 3.384f);
    glVertex2f(-3.657f, 5.100f);
    glVertex2f(-0.856f, 3.384f);
    glVertex2f(-0.826f, 3.384f);
    glEnd();
    /* ================= station name board ================= */
    glColor3ub(205, 205, 205);
    /* concrete base the post stands on */
    glBegin(GL_QUADS);
    glVertex2f(-6.009f, 0.000f);
    glVertex2f(-3.448f, 0.000f);
    glVertex2f(-3.448f, 0.552f);
    glVertex2f(-6.009f, 0.552f);
    glEnd();
    glColor3ub(172, 172, 172);
    /* top of that base */
    glBegin(GL_QUADS);
    glVertex2f(-6.009f, 0.456f);
    glVertex2f(-3.448f, 0.456f);
    glVertex2f(-3.448f, 0.552f);
    glVertex2f(-6.009f, 0.552f);
    glEnd();
    glColor3ub(160, 55, 42);
    /* the post */
    glBegin(GL_QUADS);
    glVertex2f(-5.068f, 0.384f);
    glVertex2f(-4.389f, 0.384f);
    glVertex2f(-4.389f, 2.304f);
    glVertex2f(-5.068f, 2.304f);
    glEnd();
    glColor3ub(132, 42, 32);
    /* bracket at the top of the post */
    glBegin(GL_QUADS);
    glVertex2f(-5.277f, 2.040f);
    glVertex2f(-4.180f, 2.040f);
    glVertex2f(-4.180f, 2.304f);
    glVertex2f(-5.277f, 2.304f);
    glEnd();
    glColor3ub(150, 48, 38);
    /* frame of the name board */
    glBegin(GL_QUADS);
    glVertex2f(-5.591f, 2.184f);
    glVertex2f(-1.829f, 2.184f);
    glVertex2f(-1.829f, 3.576f);
    glVertex2f(-5.591f, 3.576f);
    glEnd();
    glColor3ub(250, 248, 240);
    /* white face of the board */
    glBegin(GL_QUADS);
    glVertex2f(-5.455f, 2.328f);
    glVertex2f(-1.965f, 2.328f);
    glVertex2f(-1.965f, 3.432f);
    glVertex2f(-5.455f, 3.432f);
    glEnd();
    /* ---- the words  DINAJPUR  /  RAILWAY STATION ---- */
    glColor3ub(20, 20, 20);
    /* the word DINAJPUR in block letters */
    glLineWidth(4.0f);
    drawText("DINAJPUR", -5.237f, 2.808f, 0.00515f);
    glLineWidth(1.0f);
    glColor3ub(60, 60, 60);
    /* the words RAILWAY STATION below it */
    glLineWidth(2.0f);
    drawText("RAILWAY STATION", -4.912f, 2.424f, 0.00214f);
    glLineWidth(1.0f);



    /* ================= foot over bridge ================= */
    glColor3ub(150, 78, 60);
    /* the two legs of the bridge */
    glBegin(GL_QUADS);
    glVertex2f(9.980f, 0.000f);
    glVertex2f(10.178f, 0.000f);
    glVertex2f(10.178f, 4.440f);
    glVertex2f(9.980f, 4.440f);
    glVertex2f(10.607f, 0.000f);
    glVertex2f(10.805f, 0.000f);
    glVertex2f(10.805f, 4.440f);
    glVertex2f(10.607f, 4.440f);
    glEnd();
    glColor3ub(172, 96, 74);
    /* cross bracing between the legs */
    glBegin(GL_LINES);
    glVertex2f(10.178f, 0.300f);
    glVertex2f(10.607f, 1.092f);
    glVertex2f(10.178f, 1.092f);
    glVertex2f(10.607f, 0.300f);
    glVertex2f(10.178f, 1.092f);
    glVertex2f(10.607f, 1.884f);
    glVertex2f(10.178f, 1.884f);
    glVertex2f(10.607f, 1.092f);
    glVertex2f(10.178f, 1.884f);
    glVertex2f(10.607f, 2.676f);
    glVertex2f(10.178f, 2.676f);
    glVertex2f(10.607f, 1.884f);
    glVertex2f(10.178f, 2.676f);
    glVertex2f(10.607f, 3.468f);
    glVertex2f(10.178f, 3.468f);
    glVertex2f(10.607f, 2.676f);
    glVertex2f(10.178f, 3.468f);
    glVertex2f(10.607f, 4.260f);
    glVertex2f(10.178f, 4.260f);
    glVertex2f(10.607f, 3.468f);
    glEnd();
    glColor3ub(150, 78, 60);
    /* underside of the deck */
    glBegin(GL_QUADS);
    glVertex2f(9.353f, 3.624f);
    glVertex2f(12.070f, 3.624f);
    glVertex2f(12.070f, 3.864f);
    glVertex2f(9.353f, 3.864f);
    glEnd();
    glColor3ub(172, 96, 74);
    /* railing posts along the deck */
    glBegin(GL_QUADS);
    glVertex2f(9.353f, 4.272f);
    glVertex2f(12.070f, 4.272f);
    glVertex2f(12.070f, 4.428f);
    glVertex2f(9.353f, 4.428f);
    glVertex2f(9.426f, 3.864f);
    glVertex2f(9.509f, 3.864f);
    glVertex2f(9.509f, 4.272f);
    glVertex2f(9.426f, 4.272f);
    glVertex2f(9.739f, 3.864f);
    glVertex2f(9.822f, 3.864f);
    glVertex2f(9.822f, 4.272f);
    glVertex2f(9.739f, 4.272f);
    glVertex2f(10.053f, 3.864f);
    glVertex2f(10.136f, 3.864f);
    glVertex2f(10.136f, 4.272f);
    glVertex2f(10.053f, 4.272f);
    glVertex2f(10.367f, 3.864f);
    glVertex2f(10.450f, 3.864f);
    glVertex2f(10.450f, 4.272f);
    glVertex2f(10.367f, 4.272f);
    glVertex2f(10.680f, 3.864f);
    glVertex2f(10.763f, 3.864f);
    glVertex2f(10.763f, 4.272f);
    glVertex2f(10.680f, 4.272f);
    glVertex2f(10.994f, 3.864f);
    glVertex2f(11.077f, 3.864f);
    glVertex2f(11.077f, 4.272f);
    glVertex2f(10.994f, 4.272f);
    glVertex2f(11.307f, 3.864f);
    glVertex2f(11.390f, 3.864f);
    glVertex2f(11.390f, 4.272f);
    glVertex2f(11.307f, 4.272f);
    glVertex2f(11.620f, 3.864f);
    glVertex2f(11.704f, 3.864f);
    glVertex2f(11.704f, 4.272f);
    glVertex2f(11.620f, 4.272f);
    glVertex2f(11.934f, 3.864f);
    glVertex2f(12.017f, 3.864f);
    glVertex2f(12.017f, 4.272f);
    glVertex2f(11.934f, 4.272f);
    glEnd();
    glColor3ub(160, 88, 66);
    /* the steps of the staircase */
    glBegin(GL_QUADS);
    glVertex2f(10.973f, 3.504f);
    glVertex2f(11.286f, 3.504f);
    glVertex2f(11.286f, 3.624f);
    glVertex2f(10.973f, 3.624f);
    glVertex2f(11.140f, 3.144f);
    glVertex2f(11.453f, 3.144f);
    glVertex2f(11.453f, 3.264f);
    glVertex2f(11.140f, 3.264f);
    glVertex2f(11.307f, 2.784f);
    glVertex2f(11.620f, 2.784f);
    glVertex2f(11.620f, 2.904f);
    glVertex2f(11.307f, 2.904f);
    glVertex2f(11.474f, 2.424f);
    glVertex2f(11.787f, 2.424f);
    glVertex2f(11.787f, 2.544f);
    glVertex2f(11.474f, 2.544f);
    glVertex2f(11.641f, 2.064f);
    glVertex2f(11.954f, 2.064f);
    glVertex2f(11.954f, 2.184f);
    glVertex2f(11.641f, 2.184f);
    glVertex2f(11.808f, 1.704f);
    glVertex2f(12.121f, 1.704f);
    glVertex2f(12.121f, 1.824f);
    glVertex2f(11.808f, 1.824f);
    glVertex2f(11.975f, 1.344f);
    glVertex2f(12.288f, 1.344f);
    glVertex2f(12.288f, 1.464f);
    glVertex2f(11.975f, 1.464f);
    glVertex2f(12.142f, 0.984f);
    glVertex2f(12.455f, 0.984f);
    glVertex2f(12.455f, 1.104f);
    glVertex2f(12.142f, 1.104f);
    glEnd();
    glColor3ub(150, 78, 60);
    /* handrail beside the steps */
    glBegin(GL_LINES);
    glVertex2f(10.973f, 3.624f);
    glVertex2f(12.070f, 1.152f);
    glVertex2f(10.973f, 4.368f);
    glVertex2f(12.070f, 1.896f);
    glEnd();

    /* ================= platform floor ================= */
    glColor3ub(198, 198, 198);
    /* floor of the platform */
    glBegin(GL_QUADS);
    glVertex2f(-12.000f, 0.000f);
    glVertex2f(12.000f, 0.000f);
    glVertex2f(12.000f, 0.400f);
    glVertex2f(-12.000f, 0.400f);
    glEnd();
    glColor3ub(172, 172, 172);
    /* front edge of the floor */
    glBegin(GL_QUADS);
    glVertex2f(-12.000f, 0.360f);
    glVertex2f(12.000f, 0.360f);
    glVertex2f(12.000f, 0.400f);
    glVertex2f(-12.000f, 0.400f);
    glEnd();

    /* ================= platform side wall and edge ================= */
    glColor3ub(232, 232, 232);
    /* side wall below the platform */
    glBegin(GL_QUADS);
    glVertex2f(-12.000f, -1.100f);
    glVertex2f(12.000f, -1.100f);
    glVertex2f(12.000f, 0.000f);
    glVertex2f(-12.000f, 0.000f);
    glEnd();
    glColor3ub(214, 214, 214);
    /* the joints between its tiles */
    /* the joints between its tiles : one line each, 31 of them */
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(-11.9825f, -1.06f);
    glVertex2f(-11.9825f, -0.18f);
    glVertex2f(-11.1825f, -1.06f);
    glVertex2f(-11.1825f, -0.18f);
    glVertex2f(-10.3825f, -1.06f);
    glVertex2f(-10.3825f, -0.18f);
    glVertex2f(-9.5825f, -1.06f);
    glVertex2f(-9.5825f, -0.18f);
    glVertex2f(-8.7825f, -1.06f);
    glVertex2f(-8.7825f, -0.18f);
    glVertex2f(-7.9825f, -1.06f);
    glVertex2f(-7.9825f, -0.18f);
    glVertex2f(-7.1825f, -1.06f);
    glVertex2f(-7.1825f, -0.18f);
    glVertex2f(-6.3825f, -1.06f);
    glVertex2f(-6.3825f, -0.18f);
    glVertex2f(-5.5825f, -1.06f);
    glVertex2f(-5.5825f, -0.18f);
    glVertex2f(-4.7825f, -1.06f);
    glVertex2f(-4.7825f, -0.18f);
    glVertex2f(-3.9825f, -1.06f);
    glVertex2f(-3.9825f, -0.18f);
    glVertex2f(-3.1825f, -1.06f);
    glVertex2f(-3.1825f, -0.18f);
    glVertex2f(-2.3825f, -1.06f);
    glVertex2f(-2.3825f, -0.18f);
    glVertex2f(-1.5825f, -1.06f);
    glVertex2f(-1.5825f, -0.18f);
    glVertex2f(-0.7825f, -1.06f);
    glVertex2f(-0.7825f, -0.18f);
    glVertex2f(0.0175f, -1.06f);
    glVertex2f(0.0175f, -0.18f);
    glVertex2f(0.8175f, -1.06f);
    glVertex2f(0.8175f, -0.18f);
    glVertex2f(1.6175f, -1.06f);
    glVertex2f(1.6175f, -0.18f);
    glVertex2f(2.4175f, -1.06f);
    glVertex2f(2.4175f, -0.18f);
    glVertex2f(3.2175f, -1.06f);
    glVertex2f(3.2175f, -0.18f);
    glVertex2f(4.0175f, -1.06f);
    glVertex2f(4.0175f, -0.18f);
    glVertex2f(4.8175f, -1.06f);
    glVertex2f(4.8175f, -0.18f);
    glVertex2f(5.6175f, -1.06f);
    glVertex2f(5.6175f, -0.18f);
    glVertex2f(6.4175f, -1.06f);
    glVertex2f(6.4175f, -0.18f);
    glVertex2f(7.2175f, -1.06f);
    glVertex2f(7.2175f, -0.18f);
    glVertex2f(8.0175f, -1.06f);
    glVertex2f(8.0175f, -0.18f);
    glVertex2f(8.8175f, -1.06f);
    glVertex2f(8.8175f, -0.18f);
    glVertex2f(9.6175f, -1.06f);
    glVertex2f(9.6175f, -0.18f);
    glVertex2f(10.4175f, -1.06f);
    glVertex2f(10.4175f, -0.18f);
    glVertex2f(11.2175f, -1.06f);
    glVertex2f(11.2175f, -0.18f);
    glVertex2f(12.0175f, -1.06f);
    glVertex2f(12.0175f, -0.18f);
    glEnd();
    glLineWidth(1.0f);
    glColor3ub(240, 196, 60);
    /* yellow safety line along the edge */
    glBegin(GL_QUADS);
    glVertex2f(-12.000f, -0.160f);
    glVertex2f(12.000f, -0.160f);
    glVertex2f(12.000f, 0.000f);
    glVertex2f(-12.000f, 0.000f);
    glEnd();
    glColor3ub(150, 75, 60);
    /* shadow at the foot of the wall */
    glBegin(GL_QUADS);
    glVertex2f(-12.000f, -1.250f);
    glVertex2f(12.000f, -1.250f);
    glVertex2f(12.000f, -1.100f);
    glVertex2f(-12.000f, -1.100f);
    glEnd();

    /* ================= street lamp at x = -8.26f ================= */
    glColor3ub(118, 120, 128);
    /* post */
    glBegin(GL_QUADS);
    glVertex2f(-8.328f, 0.000f);
    glVertex2f(-8.182f, 0.000f);
    glVertex2f(-8.182f, 4.740f);
    glVertex2f(-8.328f, 4.740f);
    glEnd();
    glColor3ub(88, 90, 98);
    /* square base at the foot of the post */
    glBegin(GL_QUADS);
    glVertex2f(-8.422f, 0.000f);
    glVertex2f(-8.088f, 0.000f);
    glVertex2f(-8.088f, 0.384f);
    glVertex2f(-8.422f, 0.384f);
    glEnd();
    glColor3ub(118, 120, 128);
    /* arm reaching out from the post */
    glBegin(GL_QUADS);
    glVertex2f(-8.255f, 4.500f);
    glVertex2f(-7.419f, 4.716f);
    glVertex2f(-7.419f, 4.932f);
    glVertex2f(-8.255f, 4.788f);
    glEnd();
    glColor3ub(238, 238, 228);
    /* lamp housing at the end of the arm */
    glBegin(GL_QUADS);
    glVertex2f(-7.523f, 4.920f);
    glVertex2f(-6.959f, 4.860f);
    glVertex2f(-7.043f, 4.716f);
    glVertex2f(-7.461f, 4.764f);
    glEnd();
    glColor3ub(250, 244, 186);
    /* glass under the housing */
    glBegin(GL_QUADS);
    glVertex2f(-7.461f, 4.764f);
    glVertex2f(-7.043f, 4.716f);
    glVertex2f(-7.106f, 4.620f);
    glVertex2f(-7.419f, 4.668f);
    glEnd();

    /* ================= street lamp at x = 4.81f ================= */
    glColor3ub(118, 120, 128);
    /* post */
    glBegin(GL_QUADS);
    glVertex2f(4.734f, 0.000f);
    glVertex2f(4.880f, 0.000f);
    glVertex2f(4.880f, 4.560f);
    glVertex2f(4.734f, 4.560f);
    glEnd();
    glColor3ub(88, 90, 98);
    /* square base at the foot of the post */
    glBegin(GL_QUADS);
    glVertex2f(4.640f, 0.000f);
    glVertex2f(4.974f, 0.000f);
    glVertex2f(4.974f, 0.384f);
    glVertex2f(4.640f, 0.384f);
    glEnd();
    glColor3ub(118, 120, 128);
    /* arm reaching out from the post */
    glBegin(GL_QUADS);
    glVertex2f(4.807f, 4.320f);
    glVertex2f(3.971f, 4.536f);
    glVertex2f(3.971f, 4.752f);
    glVertex2f(4.807f, 4.608f);
    glEnd();
    glColor3ub(238, 238, 228);
    /* lamp housing at the end of the arm */
    glBegin(GL_QUADS);
    glVertex2f(4.075f, 4.740f);
    glVertex2f(3.511f, 4.680f);
    glVertex2f(3.595f, 4.536f);
    glVertex2f(4.013f, 4.584f);
    glEnd();
    glColor3ub(250, 244, 186);
    /* glass under the housing */
    glBegin(GL_QUADS);
    glVertex2f(4.013f, 4.584f);
    glVertex2f(3.595f, 4.536f);
    glVertex2f(3.658f, 4.440f);
    glVertex2f(3.971f, 4.488f);
    glEnd();

    /* ================= street lamp at x = 8.26f ================= */
    glColor3ub(118, 120, 128);
    /* post */
    glBegin(GL_QUADS);
    glVertex2f(8.182f, 0.000f);
    glVertex2f(8.328f, 0.000f);
    glVertex2f(8.328f, 5.460f);
    glVertex2f(8.182f, 5.460f);
    glEnd();
    glColor3ub(88, 90, 98);
    /* square base at the foot of the post */
    glBegin(GL_QUADS);
    glVertex2f(8.088f, 0.000f);
    glVertex2f(8.422f, 0.000f);
    glVertex2f(8.422f, 0.384f);
    glVertex2f(8.088f, 0.384f);
    glEnd();
    glColor3ub(118, 120, 128);
    /* arm reaching out from the post */
    glBegin(GL_QUADS);
    glVertex2f(8.255f, 5.220f);
    glVertex2f(7.419f, 5.436f);
    glVertex2f(7.419f, 5.652f);
    glVertex2f(8.255f, 5.508f);
    glEnd();
    glColor3ub(238, 238, 228);
    /* lamp housing at the end of the arm */
    glBegin(GL_QUADS);
    glVertex2f(7.523f, 5.640f);
    glVertex2f(6.959f, 5.580f);
    glVertex2f(7.043f, 5.436f);
    glVertex2f(7.461f, 5.484f);
    glEnd();
    glColor3ub(250, 244, 186);
    /* glass under the housing */
    glBegin(GL_QUADS);
    glVertex2f(7.461f, 5.484f);
    glVertex2f(7.043f, 5.436f);
    glVertex2f(7.106f, 5.340f);
    glVertex2f(7.419f, 5.388f);
    glEnd();

    /* ================= the lamps are lit at night ================= */
    if (nightMix > 0.0f)
    {
        glDisable(GL_LIGHTING);
        /* sky behind the halos is about (136, 191, 236) before the tint */
        glColor3ub((int)(136.0f * lightR * (1.0f - nightMix) + 120.0f * nightMix),
                   (int)(191.0f * lightG * (1.0f - nightMix) + 116.0f * nightMix),
                   (int)(236.0f * lightB * (1.0f - nightMix) + 70.0f * nightMix));
        /* wide halo around the left lamp */
        drawCircle(-7.25f, 4.8f, 0.46f, 0.46f);
        /* wide halo around the middle lamp */
        drawCircle(3.8f, 4.62f, 0.46f, 0.46f);
        /* wide halo around the right lamp */
        drawCircle(7.25f, 5.52f, 0.46f, 0.46f);
        glColor3ub((int)(136.0f * lightR * (1.0f - nightMix) + 255.0f * nightMix),
                   (int)(191.0f * lightG * (1.0f - nightMix) + 246.0f * nightMix),
                   (int)(236.0f * lightB * (1.0f - nightMix) + 190.0f * nightMix));
        /* bright centre of the left lamp */
        drawCircle(-7.25f, 4.8f, 0.22f, 0.22f);
        /* bright centre of the middle lamp */
        drawCircle(3.8f, 4.62f, 0.22f, 0.22f);
        /* bright centre of the right lamp */
        drawCircle(7.25f, 5.52f, 0.22f, 0.22f);
        glEnable(GL_LIGHTING);
    }

    /* ================= bench at x = 0.99f ================= */
    drawBench(0.99f);
    /* ================= bench at x = 6.58f ================= */
    drawBench(6.58f);
    /* ================= dust bins ================= */
    glColor3ub(82, 82, 88);
    /* the two bins */
    glBegin(GL_QUADS);
    glVertex2f(4.632f, 0.000f);
    glVertex2f(4.988f, 0.000f);
    glVertex2f(5.040f, 0.624f);
    glVertex2f(4.580f, 0.624f);
    glVertex2f(8.182f, 0.000f);
    glVertex2f(8.538f, 0.000f);
    glVertex2f(8.590f, 0.624f);
    glVertex2f(8.130f, 0.624f);
    glEnd();
    glColor3ub(55, 55, 60);
    /* lid on each bin */
    glBegin(GL_QUADS);
    glVertex2f(4.538f, 0.624f);
    glVertex2f(5.082f, 0.624f);
    glVertex2f(5.082f, 0.756f);
    glVertex2f(4.538f, 0.756f);
    glVertex2f(8.088f, 0.624f);
    glVertex2f(8.632f, 0.624f);
    glVertex2f(8.632f, 0.756f);
    glVertex2f(8.088f, 0.756f);
    glEnd();

    /* ================= bollards ================= */
    glColor3ub(45, 45, 48);
    /* the five bollards */
    glBegin(GL_QUADS);
    glVertex2f(-2.320f, 0.000f);
    glVertex2f(-2.070f, 0.000f);
    glVertex2f(-2.070f, 0.504f);
    glVertex2f(-2.320f, 0.504f);
    glVertex2f(0.188f, 0.000f);
    glVertex2f(0.438f, 0.000f);
    glVertex2f(0.438f, 0.504f);
    glVertex2f(0.188f, 0.504f);
    glVertex2f(4.002f, 0.000f);
    glVertex2f(4.252f, 0.000f);
    glVertex2f(4.252f, 0.504f);
    glVertex2f(4.002f, 0.504f);
    glVertex2f(6.928f, 0.000f);
    glVertex2f(7.178f, 0.000f);
    glVertex2f(7.178f, 0.504f);
    glVertex2f(6.928f, 0.504f);
    glVertex2f(8.548f, 0.000f);
    glVertex2f(8.798f, 0.000f);
    glVertex2f(8.798f, 0.504f);
    glVertex2f(8.548f, 0.504f);
    glEnd();
    glColor3ub(72, 72, 78);
    /* rounded cap on each bollard */
    glBegin(GL_QUADS);
    glVertex2f(-2.352f, 0.480f);
    glVertex2f(-2.038f, 0.480f);
    glVertex2f(-2.038f, 0.576f);
    glVertex2f(-2.352f, 0.576f);
    glVertex2f(0.156f, 0.480f);
    glVertex2f(0.470f, 0.480f);
    glVertex2f(0.470f, 0.576f);
    glVertex2f(0.156f, 0.576f);
    glVertex2f(3.970f, 0.480f);
    glVertex2f(4.284f, 0.480f);
    glVertex2f(4.284f, 0.576f);
    glVertex2f(3.970f, 0.576f);
    glVertex2f(6.896f, 0.480f);
    glVertex2f(7.210f, 0.480f);
    glVertex2f(7.210f, 0.576f);
    glVertex2f(6.896f, 0.576f);
    glVertex2f(8.516f, 0.480f);
    glVertex2f(8.830f, 0.480f);
    glVertex2f(8.830f, 0.576f);
    glVertex2f(8.516f, 0.576f);
    glEnd();

    /* ================= passenger at x = -4.49f ================= */
    glPushMatrix();
    glTranslatef(walk1, 0.0f, 0.0f); // walking along the platform
    drawPassenger(-4.49f, 45, 80, 170);
    glPopMatrix();

    /* ================= passenger at x = -3.81f ================= */
    glPushMatrix();
    glTranslatef(walk2, 0.0f, 0.0f); // walking along the platform
    glColor3ub(205, 45, 125);
    /* legs */
    glBegin(GL_QUADS);
    glVertex2f(-4.070f, 0.000f);
    glVertex2f(-3.550f, 0.000f);
    glVertex2f(-3.660f, 0.950f);
    glVertex2f(-3.960f, 0.950f);
    glEnd();
    glColor3ub(205, 45, 125);
    /* shirt and arms */
    glBegin(GL_QUADS);
    glVertex2f(-3.967f, 0.480f);
    glVertex2f(-3.653f, 0.480f);
    glVertex2f(-3.653f, 0.960f);
    glVertex2f(-3.967f, 0.960f);
    glVertex2f(-4.029f, 0.504f);
    glVertex2f(-3.967f, 0.504f);
    glVertex2f(-3.967f, 0.936f);
    glVertex2f(-4.029f, 0.936f);
    glVertex2f(-3.653f, 0.504f);
    glVertex2f(-3.591f, 0.504f);
    glVertex2f(-3.591f, 0.936f);
    glVertex2f(-3.653f, 0.936f);
    glEnd();
    glColor3ub(222, 178, 130);
    /* neck */
    glBegin(GL_QUADS);
    glVertex2f(-3.862f, 0.888f);
    glVertex2f(-3.758f, 0.888f);
    glVertex2f(-3.758f, 1.032f);
    glVertex2f(-3.862f, 1.032f);
    glEnd();
    glColor3ub(35, 30, 30);
    /* head and hair */
    drawCircle(-3.81f, 1.152f, 0.126f, 0.124f);
    glColor3ub(222, 178, 130);
    /* face */
    drawCircle(-3.81f, 1.08f, 0.105f, 0.103f);
    glPopMatrix();

    /* ================= passenger at x = -2.93f ================= */
    glPushMatrix();
    glTranslatef(walk3, 0.0f, 0.0f); // walking along the platform
    drawPassenger(-2.93f, 70, 175, 95);
    glPopMatrix();

    /* ================= passenger at x = 2.40f ================= */
    glPushMatrix();
    glTranslatef(walk4, 0.0f, 0.0f); // walking along the platform
    drawPassenger(2.4f, 35, 95, 185);
    glPopMatrix();

    /* ================= passenger at x = 3.60f ================= */
    glPushMatrix();
    glTranslatef(walk5, 0.0f, 0.0f); // walking along the platform
    drawPassenger(3.6f, 228, 112, 42);
    glPopMatrix();

    /* ================= passenger at x = 7.94f ================= */
    glPushMatrix();
    glTranslatef(walk6, 0.0f, 0.0f); // walking along the platform
    drawPassenger(7.94f, 55, 55, 95);
    glPopMatrix();

    /* ================= ballast ================= */
    /* the ballast bed the tracks sit on */
    glBegin(GL_QUADS);
    glColor3ub(174, 170, 164);
    glVertex2f(-12.000f, -1.250f);
    glVertex2f(12.000f, -1.250f);
    glColor3ub(208, 205, 199);
    glVertex2f(12.000f, -4.350f);
    glVertex2f(-12.000f, -4.350f);
    glEnd();
    /* ---- small stones ---- */
    glColor3ub(158, 155, 148);
    /* scattered stones, first layer */
    /* scattered stones : one point each, 115 of them */
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    glVertex2f(-16.59f, -4.35f);
    glVertex2f(-0.9225f, -4.2835f);
    glVertex2f(14.6805f, -4.217f);
    glVertex2f(-2.7565f, -4.1505f);
    glVertex2f(12.786f, -4.0845f);
    glVertex2f(-4.5755f, -4.0195f);
    glVertex2f(10.9075f, -3.9535f);
    glVertex2f(-6.379f, -3.8885f);
    glVertex2f(9.044f, -3.8235f);
    glVertex2f(-8.169f, -3.759f);
    glVertex2f(7.196f, -3.695f);
    glVertex2f(-9.9435f, -3.631f);
    glVertex2f(5.3625f, -3.567f);
    glVertex2f(-11.7045f, -3.503f);
    glVertex2f(3.544f, -3.44f);
    glVertex2f(-13.4505f, -3.377f);
    glVertex2f(1.74f, -3.314f);
    glVertex2f(-15.183f, -3.2515f);
    glVertex2f(-0.05f, -3.189f);
    glVertex2f(15.0235f, -3.127f);
    glVertex2f(-1.825f, -3.0655f);
    glVertex2f(13.192f, -3.0035f);
    glVertex2f(-3.5865f, -2.9425f);
    glVertex2f(11.3745f, -2.8815f);
    glVertex2f(-5.334f, -2.8205f);
    glVertex2f(9.572f, -2.76f);
    glVertex2f(-7.0675f, -2.6995f);
    glVertex2f(7.783f, -2.6395f);
    glVertex2f(-8.788f, -2.5795f);
    glVertex2f(6.008f, -2.52f);
    glVertex2f(-10.495f, -2.4605f);
    glVertex2f(4.2475f, -2.401f);
    glVertex2f(-12.1885f, -2.342f);
    glVertex2f(2.5f, -2.284f);
    glVertex2f(-13.8685f, -2.225f);
    glVertex2f(0.766f, -2.167f);
    glVertex2f(15.3445f, -2.109f);
    glVertex2f(-0.9545f, -2.051f);
    glVertex2f(13.571f, -1.9935f);
    glVertex2f(-2.662f, -1.936f);
    glVertex2f(11.812f, -1.8785f);
    glVertex2f(-4.356f, -1.8215f);
    glVertex2f(10.0655f, -1.765f);
    glVertex2f(-6.038f, -1.7085f);
    glVertex2f(8.3325f, -1.6525f);
    glVertex2f(-7.7065f, -1.596f);
    glVertex2f(6.6125f, -1.54f);
    glVertex2f(-9.3625f, -1.4845f);
    glVertex2f(4.9055f, -1.429f);
    glVertex2f(-11.0065f, -1.374f);
    glVertex2f(3.2115f, -1.319f);
    glVertex2f(-12.638f, -1.264f);
    glVertex2f(1.692f, -4.3005f);
    glVertex2f(-15.763f, -4.234f);
    glVertex2f(-0.1535f, -4.1675f);
    glVertex2f(15.3915f, -4.1015f);
    glVertex2f(-1.984f, -4.0355f);
    glVertex2f(13.501f, -3.9705f);
    glVertex2f(-3.8f, -3.9055f);
    glVertex2f(11.626f, -3.8405f);
    glVertex2f(-5.6005f, -3.7755f);
    glVertex2f(9.767f, -3.711f);
    glVertex2f(-7.3865f, -3.647f);
    glVertex2f(7.9225f, -3.583f);
    glVertex2f(-9.1585f, -3.5195f);
    glVertex2f(6.0925f, -3.456f);
    glVertex2f(-10.916f, -3.393f);
    glVertex2f(4.277f, -3.33f);
    glVertex2f(-12.6595f, -3.2675f);
    glVertex2f(2.477f, -3.205f);
    glVertex2f(-14.389f, -3.143f);
    glVertex2f(0.69f, -3.081f);
    glVertex2f(15.71f, -3.0195f);
    glVertex2f(-1.082f, -2.958f);
    glVertex2f(13.8815f, -2.8965f);
    glVertex2f(-2.8405f, -2.836f);
    glVertex2f(12.068f, -2.7755f);
    glVertex2f(-4.5845f, -2.715f);
    glVertex2f(10.269f, -2.6545f);
    glVertex2f(-6.315f, -2.595f);
    glVertex2f(8.483f, -2.535f);
    glVertex2f(-8.033f, -2.4755f);
    glVertex2f(6.712f, -2.416f);
    glVertex2f(-9.7365f, -2.357f);
    glVertex2f(4.954f, -2.2985f);
    glVertex2f(-11.4275f, -2.24f);
    glVertex2f(3.2095f, -2.1815f);
    glVertex2f(-13.1055f, -2.1235f);
    glVertex2f(1.479f, -2.0655f);
    glVertex2f(-14.77f, -2.008f);
    glVertex2f(-0.2385f, -1.9505f);
    glVertex2f(14.238f, -1.8935f);
    glVertex2f(-1.943f, -1.8365f);
    glVertex2f(12.481f, -1.7795f);
    glVertex2f(-3.6345f, -1.7225f);
    glVertex2f(10.7385f, -1.6665f);
    glVertex2f(-5.3135f, -1.6105f);
    glVertex2f(9.0085f, -1.5545f);
    glVertex2f(-6.9795f, -1.4985f);
    glVertex2f(7.2915f, -1.443f);
    glVertex2f(-8.6325f, -1.388f);
    glVertex2f(5.588f, -1.333f);
    glVertex2f(-10.274f, -1.278f);
    glVertex2f(4.309f, -4.3175f);
    glVertex2f(-13.1605f, -4.2505f);
    glVertex2f(2.4515f, -4.1845f);
    glVertex2f(-14.942f, -4.1185f);
    glVertex2f(0.6095f, -4.0525f);
    glVertex2f(16.098f, -3.9865f);
    glVertex2f(-1.217f, -3.9215f);
    glVertex2f(14.2115f, -3.8565f);
    glVertex2f(-3.0295f, -3.792f);
    glVertex2f(12.3405f, -3.7275f);
    glVertex2f(-4.827f, -3.663f);
    glVertex2f(10.4845f, -3.599f);
    glEnd();
    glPointSize(1.0f);
    glColor3ub(168, 165, 158);
    /* scattered stones, second layer */
    /* scattered stones : one point each, 115 of them */
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    glVertex2f(3.626f, -1.9535f);
    glVertex2f(-12.567f, -1.8965f);
    glVertex2f(1.907f, -1.8395f);
    glVertex2f(-14.221f, -1.7825f);
    glVertex2f(0.2005f, -1.726f);
    glVertex2f(14.5675f, -1.6695f);
    glVertex2f(-1.4925f, -1.6135f);
    glVertex2f(12.8225f, -1.5575f);
    glVertex2f(-3.1735f, -1.5015f);
    glVertex2f(11.0915f, -1.446f);
    glVertex2f(-4.8415f, -1.391f);
    glVertex2f(9.373f, -1.336f);
    glVertex2f(-6.497f, -1.281f);
    glVertex2f(8.4795f, -4.321f);
    glVertex2f(-9.0005f, -4.2545f);
    glVertex2f(6.6055f, -4.188f);
    glVertex2f(-10.7995f, -4.122f);
    glVertex2f(4.7455f, -4.0565f);
    glVertex2f(-12.5835f, -3.9905f);
    glVertex2f(2.902f, -3.9255f);
    glVertex2f(-14.353f, -3.8605f);
    glVertex2f(1.0725f, -3.7955f);
    glVertex2f(-16.109f, -3.731f);
    glVertex2f(-0.742f, -3.667f);
    glVertex2f(14.5635f, -3.603f);
    glVertex2f(-2.5415f, -3.539f);
    glVertex2f(12.7055f, -3.476f);
    glVertex2f(-4.3265f, -3.412f);
    glVertex2f(10.8625f, -3.3495f);
    glVertex2f(-6.098f, -3.287f);
    glVertex2f(9.035f, -3.2245f);
    glVertex2f(-7.8545f, -3.162f);
    glVertex2f(7.221f, -3.1f);
    glVertex2f(-9.5975f, -3.0385f);
    glVertex2f(5.422f, -2.977f);
    glVertex2f(-11.3265f, -2.9155f);
    glVertex2f(3.637f, -2.8545f);
    glVertex2f(-13.042f, -2.7945f);
    glVertex2f(1.866f, -2.7335f);
    glVertex2f(-14.744f, -2.6735f);
    glVertex2f(0.109f, -2.6135f);
    glVertex2f(14.904f, -2.5535f);
    glVertex2f(-1.634f, -2.494f);
    glVertex2f(13.1065f, -2.435f);
    glVertex2f(-3.364f, -2.376f);
    glVertex2f(11.323f, -2.317f);
    glVertex2f(-5.0805f, -2.258f);
    glVertex2f(9.5535f, -2.2f);
    glVertex2f(-6.7835f, -2.1415f);
    glVertex2f(7.797f, -2.0835f);
    glVertex2f(-8.474f, -2.0255f);
    glVertex2f(6.054f, -1.9685f);
    glVertex2f(-10.151f, -1.911f);
    glVertex2f(4.325f, -1.8535f);
    glVertex2f(-11.8155f, -1.7965f);
    glVertex2f(2.6085f, -1.7405f);
    glVertex2f(-13.4675f, -1.6835f);
    glVertex2f(0.905f, -1.6275f);
    glVertex2f(-15.1065f, -1.5715f);
    glVertex2f(-0.7855f, -1.516f);
    glVertex2f(13.482f, -1.46f);
    glVertex2f(-2.463f, -1.405f);
    glVertex2f(11.754f, -1.35f);
    glVertex2f(-4.128f, -1.295f);
    glVertex2f(11.102f, -4.338f);
    glVertex2f(-6.3925f, -4.2715f);
    glVertex2f(9.2155f, -4.205f);
    glVertex2f(-8.203f, -4.1385f);
    glVertex2f(7.3445f, -4.0725f);
    glVertex2f(-9.999f, -4.0075f);
    glVertex2f(5.489f, -3.9415f);
    glVertex2f(-11.78f, -3.8765f);
    glVertex2f(3.6485f, -3.812f);
    glVertex2f(-13.547f, -3.7475f);
    glVertex2f(1.823f, -3.683f);
    glVertex2f(-15.2995f, -3.619f);
    glVertex2f(0.0115f, -3.555f);
    glVertex2f(15.2615f, -3.492f);
    glVertex2f(-1.7845f, -3.428f);
    glVertex2f(13.4075f, -3.3655f);
    glVertex2f(-3.5665f, -3.303f);
    glVertex2f(11.568f, -3.24f);
    glVertex2f(-5.334f, -3.178f);
    glVertex2f(9.744f, -3.116f);
    glVertex2f(-7.088f, -3.054f);
    glVertex2f(7.9335f, -2.9925f);
    glVertex2f(-8.8285f, -2.9315f);
    glVertex2f(6.1385f, -2.8705f);
    glVertex2f(-10.5545f, -2.8095f);
    glVertex2f(4.3565f, -2.749f);
    glVertex2f(-12.267f, -2.6885f);
    glVertex2f(2.589f, -2.6285f);
    glVertex2f(-13.966f, -2.5685f);
    glVertex2f(0.835f, -2.509f);
    glVertex2f(15.578f, -2.45f);
    glVertex2f(-0.9055f, -2.3905f);
    glVertex2f(13.784f, -2.332f);
    glVertex2f(-2.6325f, -2.273f);
    glVertex2f(12.0035f, -2.2145f);
    glVertex2f(-4.3455f, -2.156f);
    glVertex2f(10.2375f, -2.098f);
    glVertex2f(-6.046f, -2.0405f);
    glVertex2f(8.4845f, -1.9825f);
    glVertex2f(-7.733f, -1.9255f);
    glVertex2f(6.745f, -1.8685f);
    glVertex2f(-9.408f, -1.8115f);
    glVertex2f(5.0185f, -1.7545f);
    glVertex2f(-11.0695f, -1.6985f);
    glVertex2f(3.3055f, -1.642f);
    glVertex2f(-12.7185f, -1.586f);
    glVertex2f(1.6045f, -1.53f);
    glVertex2f(-14.3555f, -1.4745f);
    glVertex2f(-0.0825f, -1.419f);
    glVertex2f(14.1365f, -1.364f);
    glVertex2f(-1.7575f, -1.309f);
    glEnd();
    glPointSize(1.0f);
    glColor3ub(178, 175, 168);
    /* scattered stones, third layer */
    /* scattered stones : one point each, 115 of them */
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    glVertex2f(-8.3085f, -2.692f);
    glVertex2f(6.541f, -2.632f);
    glVertex2f(-10.023f, -2.572f);
    glVertex2f(4.772f, -2.5125f);
    glVertex2f(-11.725f, -2.453f);
    glVertex2f(3.016f, -2.394f);
    glVertex2f(-13.4135f, -2.335f);
    glVertex2f(1.2735f, -2.276f);
    glVertex2f(-15.0885f, -2.218f);
    glVertex2f(-0.455f, -2.1595f);
    glVertex2f(14.1225f, -2.1015f);
    glVertex2f(-2.1705f, -2.0435f);
    glVertex2f(12.354f, -1.986f);
    glVertex2f(-3.873f, -1.9285f);
    glVertex2f(10.6f, -1.8715f);
    glVertex2f(-5.562f, -1.8145f);
    glVertex2f(8.8585f, -1.7575f);
    glVertex2f(-7.2385f, -1.7015f);
    glVertex2f(7.1305f, -1.645f);
    glVertex2f(-8.9025f, -1.589f);
    glVertex2f(5.4155f, -1.533f);
    glVertex2f(-10.5535f, -1.4775f);
    glVertex2f(3.7135f, -1.422f);
    glVertex2f(-12.1925f, -1.367f);
    glVertex2f(2.024f, -1.312f);
    glVertex2f(-13.819f, -1.257f);
    glVertex2f(0.3845f, -4.292f);
    glVertex2f(15.9885f, -4.2255f);
    glVertex2f(-1.4555f, -4.159f);
    glVertex2f(14.0885f, -4.0935f);
    glVertex2f(-3.28f, -4.0275f);
    glVertex2f(12.204f, -3.9625f);
    glVertex2f(-5.09f, -3.8965f);
    glVertex2f(10.335f, -3.832f);
    glVertex2f(-6.885f, -3.7675f);
    glVertex2f(8.481f, -3.703f);
    glVertex2f(-8.6655f, -3.639f);
    glVertex2f(6.642f, -3.575f);
    glVertex2f(-10.4315f, -3.511f);
    glVertex2f(4.8175f, -3.448f);
    glVertex2f(-12.1835f, -3.385f);
    glVertex2f(3.008f, -3.322f);
    glVertex2f(-13.922f, -3.2595f);
    glVertex2f(1.213f, -3.197f);
    glVertex2f(-15.646f, -3.135f);
    glVertex2f(-0.568f, -3.0735f);
    glVertex2f(14.4505f, -3.0115f);
    glVertex2f(-2.3345f, -2.9505f);
    glVertex2f(12.6275f, -2.8895f);
    glVertex2f(-4.0875f, -2.8285f);
    glVertex2f(10.8195f, -2.7675f);
    glVertex2f(-5.8265f, -2.7075f);
    glVertex2f(9.0255f, -2.647f);
    glVertex2f(-7.552f, -2.587f);
    glVertex2f(7.2455f, -2.5275f);
    glVertex2f(-9.264f, -2.468f);
    glVertex2f(5.479f, -2.409f);
    glVertex2f(-10.9625f, -2.35f);
    glVertex2f(3.7265f, -2.291f);
    glVertex2f(-12.6485f, -2.2325f);
    glVertex2f(1.9875f, -2.174f);
    glVertex2f(-14.321f, -2.116f);
    glVertex2f(0.262f, -2.058f);
    glVertex2f(14.789f, -2.0005f);
    glVertex2f(-1.4505f, -1.9435f);
    glVertex2f(13.024f, -1.886f);
    glVertex2f(-3.15f, -1.829f);
    glVertex2f(11.273f, -1.7725f);
    glVertex2f(-4.8365f, -1.7155f);
    glVertex2f(9.535f, -1.6595f);
    glVertex2f(-6.5105f, -1.6035f);
    glVertex2f(7.8105f, -1.5475f);
    glVertex2f(-8.1715f, -1.4915f);
    glVertex2f(6.0985f, -1.436f);
    glVertex2f(-9.82f, -1.381f);
    glVertex2f(4.399f, -1.326f);
    glVertex2f(-11.456f, -1.271f);
    glVertex2f(3.0f, -4.309f);
    glVertex2f(-14.462f, -4.2425f);
    glVertex2f(1.1485f, -4.176f);
    glVertex2f(-16.238f, -4.11f);
    glVertex2f(-0.6875f, -4.0445f);
    glVertex2f(14.799f, -3.9785f);
    glVertex2f(-2.509f, -3.9135f);
    glVertex2f(12.919f, -3.8485f);
    glVertex2f(-4.315f, -3.7835f);
    glVertex2f(11.053f, -3.7195f);
    glVertex2f(-6.107f, -3.655f);
    glVertex2f(9.203f, -3.591f);
    glVertex2f(-7.8845f, -3.5275f);
    glVertex2f(7.3675f, -3.464f);
    glVertex2f(-9.6475f, -3.401f);
    glVertex2f(5.547f, -3.338f);
    glVertex2f(-11.397f, -3.2755f);
    glVertex2f(3.741f, -3.213f);
    glVertex2f(-13.132f, -3.151f);
    glVertex2f(1.949f, -3.089f);
    glVertex2f(-14.853f, -3.0275f);
    glVertex2f(0.1715f, -2.9655f);
    glVertex2f(15.1365f, -2.9045f);
    glVertex2f(-1.5925f, -2.8435f);
    glVertex2f(13.3175f, -2.783f);
    glVertex2f(-3.342f, -2.7225f);
    glVertex2f(11.5125f, -2.6625f);
    glVertex2f(-5.078f, -2.6025f);
    glVertex2f(9.722f, -2.5425f);
    glVertex2f(-6.801f, -2.483f);
    glVertex2f(7.945f, -2.424f);
    glVertex2f(-8.51f, -2.365f);
    glVertex2f(6.182f, -2.306f);
    glVertex2f(-10.2055f, -2.247f);
    glVertex2f(4.4325f, -2.189f);
    glVertex2f(-11.8885f, -2.131f);
    glVertex2f(2.6965f, -2.073f);
    glVertex2f(-13.5585f, -2.0155f);
    glEnd();
    glPointSize(1.0f);
    glColor3ub(188, 185, 178);
    /* scattered stones, fourth layer */
    /* scattered stones : one point each, 115 of them */
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    glVertex2f(11.4285f, -3.468f);
    glVertex2f(-5.597f, -3.404f);
    glVertex2f(9.591f, -3.3415f);
    glVertex2f(-7.362f, -3.279f);
    glVertex2f(7.769f, -3.2165f);
    glVertex2f(-9.114f, -3.1545f);
    glVertex2f(5.961f, -3.0925f);
    glVertex2f(-10.851f, -3.0305f);
    glVertex2f(4.167f, -2.9695f);
    glVertex2f(-12.5745f, -2.9085f);
    glVertex2f(2.3875f, -2.8475f);
    glVertex2f(-14.285f, -2.7865f);
    glVertex2f(0.622f, -2.726f);
    glVertex2f(15.47f, -2.6655f);
    glVertex2f(-1.13f, -2.6055f);
    glVertex2f(13.664f, -2.546f);
    glVertex2f(-2.868f, -2.4865f);
    glVertex2f(11.8715f, -2.427f);
    glVertex2f(-4.5925f, -2.368f);
    glVertex2f(10.0935f, -2.309f);
    glVertex2f(-6.3035f, -2.251f);
    glVertex2f(8.3285f, -2.192f);
    glVertex2f(-8.002f, -2.134f);
    glVertex2f(6.5775f, -2.076f);
    glVertex2f(-9.687f, -2.0185f);
    glVertex2f(4.84f, -1.961f);
    glVertex2f(-11.359f, -1.9035f);
    glVertex2f(3.1155f, -1.8465f);
    glVertex2f(-13.0185f, -1.7895f);
    glVertex2f(1.404f, -1.7335f);
    glVertex2f(-14.6655f, -1.6765f);
    glVertex2f(-0.2945f, -1.6205f);
    glVertex2f(14.0225f, -1.5645f);
    glVertex2f(-1.9795f, -1.509f);
    glVertex2f(12.2865f, -1.453f);
    glVertex2f(-3.6525f, -1.398f);
    glVertex2f(10.563f, -1.343f);
    glVertex2f(-5.313f, -1.288f);
    glVertex2f(9.7905f, -4.3295f);
    glVertex2f(-7.6965f, -4.263f);
    glVertex2f(7.9105f, -4.1965f);
    glVertex2f(-9.5015f, -4.1305f);
    glVertex2f(6.045f, -4.0645f);
    glVertex2f(-11.2915f, -3.9985f);
    glVertex2f(4.195f, -3.9335f);
    glVertex2f(-13.067f, -3.8685f);
    glVertex2f(2.36f, -3.8035f);
    glVertex2f(-14.828f, -3.739f);
    glVertex2f(0.54f, -3.675f);
    glVertex2f(15.8465f, -3.611f);
    glVertex2f(-1.2655f, -3.547f);
    glVertex2f(13.9835f, -3.484f);
    glVertex2f(-3.056f, -3.42f);
    glVertex2f(12.135f, -3.3575f);
    glVertex2f(-4.8325f, -3.295f);
    glVertex2f(10.301f, -3.232f);
    glVertex2f(-6.595f, -3.17f);
    glVertex2f(8.482f, -3.108f);
    glVertex2f(-8.343f, -3.0465f);
    glVertex2f(6.6775f, -2.9845f);
    glVertex2f(-10.0775f, -2.9235f);
    glVertex2f(4.8875f, -2.8625f);
    glVertex2f(-11.7985f, -2.8015f);
    glVertex2f(3.111f, -2.7415f);
    glVertex2f(-13.506f, -2.681f);
    glVertex2f(1.3485f, -2.621f);
    glVertex2f(-15.2f, -2.561f);
    glVertex2f(-0.4f, -2.5015f);
    glVertex2f(14.342f, -2.442f);
    glVertex2f(-2.135f, -2.383f);
    glVertex2f(12.5535f, -2.324f);
    glVertex2f(-3.8565f, -2.2655f);
    glVertex2f(10.7785f, -2.207f);
    glVertex2f(-5.565f, -2.149f);
    glVertex2f(9.017f, -2.091f);
    glVertex2f(-7.2605f, -2.033f);
    glVertex2f(7.269f, -1.9755f);
    glVertex2f(-8.943f, -1.9185f);
    glVertex2f(5.5345f, -1.8615f);
    glVertex2f(-10.612f, -1.8045f);
    glVertex2f(3.813f, -1.7475f);
    glVertex2f(-12.2685f, -1.691f);
    glVertex2f(2.1045f, -1.6345f);
    glVertex2f(-13.9135f, -1.5785f);
    glVertex2f(0.4095f, -1.523f);
    glVertex2f(14.678f, -1.4675f);
    glVertex2f(-1.273f, -1.412f);
    glVertex2f(12.945f, -1.357f);
    glVertex2f(-2.943f, -1.302f);
    glVertex2f(12.4145f, -4.3465f);
    glVertex2f(-5.0875f, -4.28f);
    glVertex2f(10.5225f, -4.2135f);
    glVertex2f(-6.9035f, -4.147f);
    glVertex2f(8.6455f, -4.0815f);
    glVertex2f(-8.7055f, -4.0155f);
    glVertex2f(6.784f, -3.9505f);
    glVertex2f(-10.4925f, -3.885f);
    glVertex2f(4.9375f, -3.8205f);
    glVertex2f(-12.265f, -3.7555f);
    glVertex2f(3.106f, -3.691f);
    glVertex2f(-14.023f, -3.627f);
    glVertex2f(1.2895f, -3.563f);
    glVertex2f(-15.7675f, -3.5f);
    glVertex2f(-0.5125f, -3.4365f);
    glVertex2f(14.681f, -3.373f);
    glVertex2f(-2.3f, -3.311f);
    glVertex2f(12.836f, -3.248f);
    glVertex2f(-4.073f, -3.186f);
    glVertex2f(11.006f, -3.124f);
    glVertex2f(-5.8325f, -3.062f);
    glVertex2f(9.1905f, -3.0005f);
    glVertex2f(-7.578f, -2.9395f);
    glVertex2f(7.3895f, -2.8785f);
    glVertex2f(-9.3095f, -2.8175f);
    glVertex2f(5.6025f, -2.7565f);
    glEnd();
    glPointSize(1.0f);

    /* ================= track 1  (the platform road) : sleepers laid in perspective ================= */
    glColor3ub(96, 66, 42);
    /* the sleepers, smaller and higher the further away they are */
    glBegin(GL_QUADS);
    for (int i = 0; i < 47; i = i + 1)
    {
        glVertex2f(-14.73152f + 0.62554f * i, -2.545f);
        glVertex2f(-14.46041f + 0.62554f * i, -2.545f);
        glVertex2f(-14.09315f + 0.60965f * i, -1.721f);
        glVertex2f(-14.35727f + 0.60965f * i, -1.721f);
    }
    glEnd();
    glColor3ub(124, 86, 56);
    /* the lit top face of each sleeper */
    glBegin(GL_QUADS);
    for (int i = 0; i < 47; i = i + 1)
    {
        glVertex2f(-14.73152f + 0.62554f * i, -2.495f);
        glVertex2f(-14.46041f + 0.62554f * i, -2.495f);
        glVertex2f(-14.09315f + 0.60965f * i, -1.721f);
        glVertex2f(-14.35727f + 0.60965f * i, -1.721f);
    }
    glEnd();
    /* ---- rails ---- */
    glColor3ub(58, 58, 64);
    /* shadow under both rails */
    glBegin(GL_QUADS);
    glVertex2f(-13.000f, -2.334f);
    glVertex2f(13.000f, -2.334f);
    glVertex2f(13.000f, -2.298f);
    glVertex2f(-13.000f, -2.298f);
    glVertex2f(-13.000f, -1.875f);
    glVertex2f(13.000f, -1.875f);
    glVertex2f(13.000f, -1.839f);
    glVertex2f(-13.000f, -1.839f);
    glEnd();
    glColor3ub(96, 96, 104);
    /* the two rails */
    glBegin(GL_QUADS);
    glVertex2f(-13.000f, -2.298f);
    glVertex2f(13.000f, -2.298f);
    glVertex2f(13.000f, -2.169f);
    glVertex2f(-13.000f, -2.169f);
    glVertex2f(-13.000f, -1.839f);
    glVertex2f(13.000f, -1.839f);
    glVertex2f(13.000f, -1.711f);
    glVertex2f(-13.000f, -1.711f);
    glEnd();
    glColor3ub(198, 198, 204);
    /* the shine along the top of each rail */
    glBegin(GL_QUADS);
    glVertex2f(-13.000f, -2.205f);
    glVertex2f(13.000f, -2.205f);
    glVertex2f(13.000f, -2.169f);
    glVertex2f(-13.000f, -2.169f);
    glVertex2f(-13.000f, -1.747f);
    glVertex2f(13.000f, -1.747f);
    glVertex2f(13.000f, -1.711f);
    glVertex2f(-13.000f, -1.711f);
    glEnd();

    /* ---- the train that stops at the platform : moved by trainPos ---- */
    glPushMatrix();
    glTranslatef(trainPos, 0.0f, 0.0f);
    /* the train that stops at the platform, in Bangladesh Railway blue */
    drawTrain(28, 72, 140, 26, 62, 120, 20, 48, 96);
    glColor3ub(255, 255, 255);
        glLineWidth(2.0f);
        drawNumber("BR 6512", 0.4f, -0.42f, 0.00252f);
        glLineWidth(1.0f);
    glPopMatrix();
    /* ================= track 2  (the road nearer to us) : sleepers laid in perspective ================= */
    glColor3ub(96, 66, 42);
    /* the sleepers, smaller and higher the further away they are */
    glBegin(GL_QUADS);
    for (int i = 0; i < 47; i = i + 1)
    {
        glVertex2f(-15.37265f + 0.65277f * i, -3.872f);
        glVertex2f(-15.08975f + 0.65276f * i, -3.872f);
        glVertex2f(-14.69018f + 0.63548f * i, -2.98f);
        glVertex2f(-14.96566f + 0.63548f * i, -2.98f);
    }
    glEnd();
    glColor3ub(124, 86, 56);
    /* the lit top face of each sleeper */
    glBegin(GL_QUADS);
    for (int i = 0; i < 47; i = i + 1)
    {
        glVertex2f(-15.37265f + 0.65277f * i, -3.822f);
        glVertex2f(-15.08975f + 0.65276f * i, -3.822f);
        glVertex2f(-14.69018f + 0.63548f * i, -2.98f);
        glVertex2f(-14.96566f + 0.63548f * i, -2.98f);
    }
    glEnd();
    /* ---- rails ---- */
    glColor3ub(58, 58, 64);
    /* shadow under both rails */
    glBegin(GL_QUADS);
    glVertex2f(-13.000f, -3.647f);
    glVertex2f(13.000f, -3.647f);
    glVertex2f(13.000f, -3.609f);
    glVertex2f(-13.000f, -3.609f);
    glVertex2f(-13.000f, -3.147f);
    glVertex2f(13.000f, -3.147f);
    glVertex2f(13.000f, -3.110f);
    glVertex2f(-13.000f, -3.110f);
    glEnd();
    glColor3ub(96, 96, 104);
    /* the two rails */
    glBegin(GL_QUADS);
    glVertex2f(-13.000f, -3.609f);
    glVertex2f(13.000f, -3.609f);
    glVertex2f(13.000f, -3.474f);
    glVertex2f(-13.000f, -3.474f);
    glVertex2f(-13.000f, -3.110f);
    glVertex2f(13.000f, -3.110f);
    glVertex2f(13.000f, -2.977f);
    glVertex2f(-13.000f, -2.977f);
    glEnd();
    glColor3ub(198, 198, 204);
    /* the shine along the top of each rail */
    glBegin(GL_QUADS);
    glVertex2f(-13.000f, -3.512f);
    glVertex2f(13.000f, -3.512f);
    glVertex2f(13.000f, -3.474f);
    glVertex2f(-13.000f, -3.474f);
    glVertex2f(-13.000f, -3.014f);
    glVertex2f(13.000f, -3.014f);
    glVertex2f(13.000f, -2.977f);
    glVertex2f(-13.000f, -2.977f);
    glEnd();

    /* ---- the train that never stops : moved by train2Pos ---- */
    glPushMatrix();
    glTranslatef(train2Pos, 0.0f, 0.0f);
    /* the same train, mirrored and a little larger, on the road nearer to us */
    glPushMatrix();
    glTranslatef(0.0f, -1.1165f, 0.0f);
    glScalef(-1.0826f, 1.0826f, 1.0f); /* the minus turns it to face the other way */
    drawTrain(178, 60, 58, 150, 42, 46, 108, 28, 32);
    glPopMatrix();
    glColor3ub(255, 255, 255);
        glLineWidth(2.0f);
        drawNumber("BR 2407", -2.43f, -1.572f, 0.00272f);
        glLineWidth(1.0f);
    glPopMatrix();

    /* ================= grass ================= */
    /* the grass strip along the front */
    glBegin(GL_QUADS);
    glColor3ub(104, 178, 86);
    glVertex2f(-12.000f, -4.350f);
    glVertex2f(12.000f, -4.350f);
    glColor3ub(132, 206, 100);
    glVertex2f(12.000f, -5.000f);
    glVertex2f(-12.000f, -5.000f);
    glEnd();
    glColor3ub(70, 165, 72);
    /* the blades of grass */
    glBegin(GL_TRIANGLES);
    glVertex2f(-11.900f, -4.980f);
    glVertex2f(-11.790f, -4.980f);
    glVertex2f(-11.880f, -4.680f);
    glVertex2f(-11.750f, -4.980f);
    glVertex2f(-11.640f, -4.980f);
    glVertex2f(-11.620f, -4.740f);
    glVertex2f(-11.548f, -4.980f);
    glVertex2f(-11.438f, -4.980f);
    glVertex2f(-11.528f, -4.569f);
    glVertex2f(-11.398f, -4.980f);
    glVertex2f(-11.288f, -4.980f);
    glVertex2f(-11.268f, -4.651f);
    glVertex2f(-11.196f, -4.980f);
    glVertex2f(-11.086f, -4.980f);
    glVertex2f(-11.176f, -4.638f);
    glVertex2f(-11.046f, -4.980f);
    glVertex2f(-10.936f, -4.980f);
    glVertex2f(-10.916f, -4.706f);
    glVertex2f(-10.844f, -4.980f);
    glVertex2f(-10.734f, -4.980f);
    glVertex2f(-10.824f, -4.526f);
    glVertex2f(-10.694f, -4.980f);
    glVertex2f(-10.584f, -4.980f);
    glVertex2f(-10.564f, -4.617f);
    glVertex2f(-10.492f, -4.980f);
    glVertex2f(-10.382f, -4.980f);
    glVertex2f(-10.472f, -4.595f);
    glVertex2f(-10.342f, -4.980f);
    glVertex2f(-10.232f, -4.980f);
    glVertex2f(-10.212f, -4.672f);
    glVertex2f(-10.140f, -4.980f);
    glVertex2f(-10.030f, -4.980f);
    glVertex2f(-10.120f, -4.664f);
    glVertex2f(-9.990f, -4.980f);
    glVertex2f(-9.880f, -4.980f);
    glVertex2f(-9.860f, -4.727f);
    glVertex2f(-9.788f, -4.980f);
    glVertex2f(-9.678f, -4.980f);
    glVertex2f(-9.768f, -4.553f);
    glVertex2f(-9.638f, -4.980f);
    glVertex2f(-9.528f, -4.980f);
    glVertex2f(-9.508f, -4.638f);
    glVertex2f(-9.436f, -4.980f);
    glVertex2f(-9.326f, -4.980f);
    glVertex2f(-9.416f, -4.621f);
    glVertex2f(-9.286f, -4.980f);
    glVertex2f(-9.176f, -4.980f);
    glVertex2f(-9.156f, -4.693f);
    glVertex2f(-9.084f, -4.980f);
    glVertex2f(-8.974f, -4.980f);
    glVertex2f(-9.064f, -4.510f);
    glVertex2f(-8.934f, -4.980f);
    glVertex2f(-8.824f, -4.980f);
    glVertex2f(-8.804f, -4.604f);
    glVertex2f(-8.732f, -4.980f);
    glVertex2f(-8.622f, -4.980f);
    glVertex2f(-8.712f, -4.579f);
    glVertex2f(-8.582f, -4.980f);
    glVertex2f(-8.472f, -4.980f);
    glVertex2f(-8.452f, -4.659f);
    glVertex2f(-8.380f, -4.980f);
    glVertex2f(-8.270f, -4.980f);
    glVertex2f(-8.360f, -4.648f);
    glVertex2f(-8.230f, -4.980f);
    glVertex2f(-8.120f, -4.980f);
    glVertex2f(-8.100f, -4.714f);
    glVertex2f(-8.028f, -4.980f);
    glVertex2f(-7.918f, -4.980f);
    glVertex2f(-8.008f, -4.536f);
    glVertex2f(-7.878f, -4.980f);
    glVertex2f(-7.768f, -4.980f);
    glVertex2f(-7.748f, -4.625f);
    glVertex2f(-7.676f, -4.980f);
    glVertex2f(-7.566f, -4.980f);
    glVertex2f(-7.656f, -4.605f);
    glVertex2f(-7.526f, -4.980f);
    glVertex2f(-7.416f, -4.980f);
    glVertex2f(-7.396f, -4.680f);
    glVertex2f(-7.324f, -4.980f);
    glVertex2f(-7.214f, -4.980f);
    glVertex2f(-7.304f, -4.674f);
    glVertex2f(-7.174f, -4.980f);
    glVertex2f(-7.064f, -4.980f);
    glVertex2f(-7.044f, -4.735f);
    glVertex2f(-6.972f, -4.980f);
    glVertex2f(-6.862f, -4.980f);
    glVertex2f(-6.952f, -4.563f);
    glVertex2f(-6.822f, -4.980f);
    glVertex2f(-6.712f, -4.980f);
    glVertex2f(-6.692f, -4.646f);
    glVertex2f(-6.620f, -4.980f);
    glVertex2f(-6.510f, -4.980f);
    glVertex2f(-6.600f, -4.631f);
    glVertex2f(-6.470f, -4.980f);
    glVertex2f(-6.360f, -4.980f);
    glVertex2f(-6.340f, -4.701f);
    glVertex2f(-6.268f, -4.980f);
    glVertex2f(-6.158f, -4.980f);
    glVertex2f(-6.248f, -4.520f);
    glVertex2f(-6.118f, -4.980f);
    glVertex2f(-6.008f, -4.980f);
    glVertex2f(-5.988f, -4.612f);
    glVertex2f(-5.916f, -4.980f);
    glVertex2f(-5.806f, -4.980f);
    glVertex2f(-5.896f, -4.589f);
    glVertex2f(-5.766f, -4.980f);
    glVertex2f(-5.656f, -4.980f);
    glVertex2f(-5.636f, -4.667f);
    glVertex2f(-5.564f, -4.980f);
    glVertex2f(-5.454f, -4.980f);
    glVertex2f(-5.544f, -4.658f);
    glVertex2f(-5.414f, -4.980f);
    glVertex2f(-5.304f, -4.980f);
    glVertex2f(-5.284f, -4.722f);
    glVertex2f(-5.212f, -4.980f);
    glVertex2f(-5.102f, -4.980f);
    glVertex2f(-5.192f, -4.546f);
    glVertex2f(-5.062f, -4.980f);
    glVertex2f(-4.952f, -4.980f);
    glVertex2f(-4.932f, -4.633f);
    glVertex2f(-4.860f, -4.980f);
    glVertex2f(-4.750f, -4.980f);
    glVertex2f(-4.840f, -4.615f);
    glVertex2f(-4.710f, -4.980f);
    glVertex2f(-4.600f, -4.980f);
    glVertex2f(-4.580f, -4.688f);
    glVertex2f(-4.508f, -4.980f);
    glVertex2f(-4.398f, -4.980f);
    glVertex2f(-4.488f, -4.504f);
    glVertex2f(-4.358f, -4.980f);
    glVertex2f(-4.248f, -4.980f);
    glVertex2f(-4.228f, -4.599f);
    glVertex2f(-4.156f, -4.980f);
    glVertex2f(-4.046f, -4.980f);
    glVertex2f(-4.136f, -4.573f);
    glVertex2f(-4.006f, -4.980f);
    glVertex2f(-3.896f, -4.980f);
    glVertex2f(-3.876f, -4.654f);
    glVertex2f(-3.804f, -4.980f);
    glVertex2f(-3.694f, -4.980f);
    glVertex2f(-3.784f, -4.641f);
    glVertex2f(-3.654f, -4.980f);
    glVertex2f(-3.544f, -4.980f);
    glVertex2f(-3.524f, -4.709f);
    glVertex2f(-3.452f, -4.980f);
    glVertex2f(-3.342f, -4.980f);
    glVertex2f(-3.432f, -4.530f);
    glVertex2f(-3.302f, -4.980f);
    glVertex2f(-3.192f, -4.980f);
    glVertex2f(-3.172f, -4.620f);
    glVertex2f(-3.100f, -4.980f);
    glVertex2f(-2.990f, -4.980f);
    glVertex2f(-3.080f, -4.599f);
    glVertex2f(-2.950f, -4.980f);
    glVertex2f(-2.840f, -4.980f);
    glVertex2f(-2.820f, -4.675f);
    glVertex2f(-2.748f, -4.980f);
    glVertex2f(-2.638f, -4.980f);
    glVertex2f(-2.728f, -4.668f);
    glVertex2f(-2.598f, -4.980f);
    glVertex2f(-2.488f, -4.980f);
    glVertex2f(-2.468f, -4.730f);
    glVertex2f(-2.396f, -4.980f);
    glVertex2f(-2.286f, -4.980f);
    glVertex2f(-2.376f, -4.556f);
    glVertex2f(-2.246f, -4.980f);
    glVertex2f(-2.136f, -4.980f);
    glVertex2f(-2.116f, -4.641f);
    glVertex2f(-2.044f, -4.980f);
    glVertex2f(-1.934f, -4.980f);
    glVertex2f(-2.024f, -4.625f);
    glVertex2f(-1.894f, -4.980f);
    glVertex2f(-1.784f, -4.980f);
    glVertex2f(-1.764f, -4.696f);
    glVertex2f(-1.692f, -4.980f);
    glVertex2f(-1.582f, -4.980f);
    glVertex2f(-1.672f, -4.514f);
    glVertex2f(-1.542f, -4.980f);
    glVertex2f(-1.432f, -4.980f);
    glVertex2f(-1.412f, -4.607f);
    glVertex2f(-1.340f, -4.980f);
    glVertex2f(-1.230f, -4.980f);
    glVertex2f(-1.320f, -4.583f);
    glVertex2f(-1.190f, -4.980f);
    glVertex2f(-1.080f, -4.980f);
    glVertex2f(-1.060f, -4.662f);
    glVertex2f(-0.988f, -4.980f);
    glVertex2f(-0.878f, -4.980f);
    glVertex2f(-0.968f, -4.651f);
    glVertex2f(-0.838f, -4.980f);
    glVertex2f(-0.728f, -4.980f);
    glVertex2f(-0.708f, -4.717f);
    glVertex2f(-0.636f, -4.980f);
    glVertex2f(-0.526f, -4.980f);
    glVertex2f(-0.616f, -4.540f);
    glVertex2f(-0.486f, -4.980f);
    glVertex2f(-0.376f, -4.980f);
    glVertex2f(-0.356f, -4.628f);
    glVertex2f(-0.284f, -4.980f);
    glVertex2f(-0.174f, -4.980f);
    glVertex2f(-0.264f, -4.609f);
    glVertex2f(-0.134f, -4.980f);
    glVertex2f(-0.024f, -4.980f);
    glVertex2f(-0.004f, -4.683f);
    glVertex2f(0.068f, -4.980f);
    glVertex2f(0.178f, -4.980f);
    glVertex2f(0.088f, -4.678f);
    glVertex2f(0.218f, -4.980f);
    glVertex2f(0.328f, -4.980f);
    glVertex2f(0.348f, -4.738f);
    glVertex2f(0.420f, -4.980f);
    glVertex2f(0.530f, -4.980f);
    glVertex2f(0.440f, -4.566f);
    glVertex2f(0.570f, -4.980f);
    glVertex2f(0.680f, -4.980f);
    glVertex2f(0.700f, -4.649f);
    glVertex2f(0.772f, -4.980f);
    glVertex2f(0.882f, -4.980f);
    glVertex2f(0.792f, -4.635f);
    glVertex2f(0.922f, -4.980f);
    glVertex2f(1.032f, -4.980f);
    glVertex2f(1.052f, -4.704f);
    glVertex2f(1.124f, -4.980f);
    glVertex2f(1.234f, -4.980f);
    glVertex2f(1.144f, -4.524f);
    glVertex2f(1.274f, -4.980f);
    glVertex2f(1.384f, -4.980f);
    glVertex2f(1.404f, -4.615f);
    glVertex2f(1.476f, -4.980f);
    glVertex2f(1.586f, -4.980f);
    glVertex2f(1.496f, -4.593f);
    glVertex2f(1.626f, -4.980f);
    glVertex2f(1.736f, -4.980f);
    glVertex2f(1.756f, -4.670f);
    glVertex2f(1.828f, -4.980f);
    glVertex2f(1.938f, -4.980f);
    glVertex2f(1.848f, -4.661f);
    glVertex2f(1.978f, -4.980f);
    glVertex2f(2.088f, -4.980f);
    glVertex2f(2.108f, -4.725f);
    glVertex2f(2.180f, -4.980f);
    glVertex2f(2.290f, -4.980f);
    glVertex2f(2.200f, -4.550f);
    glVertex2f(2.330f, -4.980f);
    glVertex2f(2.440f, -4.980f);
    glVertex2f(2.460f, -4.636f);
    glVertex2f(2.532f, -4.980f);
    glVertex2f(2.642f, -4.980f);
    glVertex2f(2.552f, -4.619f);
    glVertex2f(2.682f, -4.980f);
    glVertex2f(2.792f, -4.980f);
    glVertex2f(2.812f, -4.691f);
    glVertex2f(2.884f, -4.980f);
    glVertex2f(2.994f, -4.980f);
    glVertex2f(2.904f, -4.508f);
    glVertex2f(3.034f, -4.980f);
    glVertex2f(3.144f, -4.980f);
    glVertex2f(3.164f, -4.602f);
    glVertex2f(3.236f, -4.980f);
    glVertex2f(3.346f, -4.980f);
    glVertex2f(3.256f, -4.576f);
    glVertex2f(3.386f, -4.980f);
    glVertex2f(3.496f, -4.980f);
    glVertex2f(3.516f, -4.657f);
    glVertex2f(3.588f, -4.980f);
    glVertex2f(3.698f, -4.980f);
    glVertex2f(3.608f, -4.645f);
    glVertex2f(3.738f, -4.980f);
    glVertex2f(3.848f, -4.980f);
    glVertex2f(3.868f, -4.712f);
    glVertex2f(3.940f, -4.980f);
    glVertex2f(4.050f, -4.980f);
    glVertex2f(3.960f, -4.534f);
    glVertex2f(4.090f, -4.980f);
    glVertex2f(4.200f, -4.980f);
    glVertex2f(4.220f, -4.623f);
    glVertex2f(4.292f, -4.980f);
    glVertex2f(4.402f, -4.980f);
    glVertex2f(4.312f, -4.603f);
    glVertex2f(4.442f, -4.980f);
    glVertex2f(4.552f, -4.980f);
    glVertex2f(4.572f, -4.678f);
    glVertex2f(4.644f, -4.980f);
    glVertex2f(4.754f, -4.980f);
    glVertex2f(4.664f, -4.671f);
    glVertex2f(4.794f, -4.980f);
    glVertex2f(4.904f, -4.980f);
    glVertex2f(4.924f, -4.733f);
    glVertex2f(4.996f, -4.980f);
    glVertex2f(5.106f, -4.980f);
    glVertex2f(5.016f, -4.560f);
    glVertex2f(5.146f, -4.980f);
    glVertex2f(5.256f, -4.980f);
    glVertex2f(5.276f, -4.644f);
    glVertex2f(5.348f, -4.980f);
    glVertex2f(5.458f, -4.980f);
    glVertex2f(5.368f, -4.629f);
    glVertex2f(5.498f, -4.980f);
    glVertex2f(5.608f, -4.980f);
    glVertex2f(5.628f, -4.699f);
    glVertex2f(5.700f, -4.980f);
    glVertex2f(5.810f, -4.980f);
    glVertex2f(5.720f, -4.518f);
    glVertex2f(5.850f, -4.980f);
    glVertex2f(5.960f, -4.980f);
    glVertex2f(5.980f, -4.610f);
    glVertex2f(6.052f, -4.980f);
    glVertex2f(6.162f, -4.980f);
    glVertex2f(6.072f, -4.586f);
    glVertex2f(6.202f, -4.980f);
    glVertex2f(6.312f, -4.980f);
    glVertex2f(6.332f, -4.665f);
    glVertex2f(6.404f, -4.980f);
    glVertex2f(6.514f, -4.980f);
    glVertex2f(6.424f, -4.655f);
    glVertex2f(6.554f, -4.980f);
    glVertex2f(6.664f, -4.980f);
    glVertex2f(6.684f, -4.720f);
    glVertex2f(6.756f, -4.980f);
    glVertex2f(6.866f, -4.980f);
    glVertex2f(6.776f, -4.544f);
    glVertex2f(6.906f, -4.980f);
    glVertex2f(7.016f, -4.980f);
    glVertex2f(7.036f, -4.631f);
    glVertex2f(7.108f, -4.980f);
    glVertex2f(7.218f, -4.980f);
    glVertex2f(7.128f, -4.613f);
    glVertex2f(7.258f, -4.980f);
    glVertex2f(7.368f, -4.980f);
    glVertex2f(7.388f, -4.686f);
    glVertex2f(7.460f, -4.980f);
    glVertex2f(7.570f, -4.980f);
    glVertex2f(7.480f, -4.501f);
    glVertex2f(7.610f, -4.980f);
    glVertex2f(7.720f, -4.980f);
    glVertex2f(7.740f, -4.597f);
    glVertex2f(7.812f, -4.980f);
    glVertex2f(7.922f, -4.980f);
    glVertex2f(7.832f, -4.570f);
    glVertex2f(7.962f, -4.980f);
    glVertex2f(8.072f, -4.980f);
    glVertex2f(8.092f, -4.652f);
    glVertex2f(8.164f, -4.980f);
    glVertex2f(8.274f, -4.980f);
    glVertex2f(8.184f, -4.639f);
    glVertex2f(8.314f, -4.980f);
    glVertex2f(8.424f, -4.980f);
    glVertex2f(8.444f, -4.707f);
    glVertex2f(8.516f, -4.980f);
    glVertex2f(8.626f, -4.980f);
    glVertex2f(8.536f, -4.528f);
    glVertex2f(8.666f, -4.980f);
    glVertex2f(8.776f, -4.980f);
    glVertex2f(8.796f, -4.618f);
    glVertex2f(8.868f, -4.980f);
    glVertex2f(8.978f, -4.980f);
    glVertex2f(8.888f, -4.596f);
    glVertex2f(9.018f, -4.980f);
    glVertex2f(9.128f, -4.980f);
    glVertex2f(9.148f, -4.673f);
    glVertex2f(9.220f, -4.980f);
    glVertex2f(9.330f, -4.980f);
    glVertex2f(9.240f, -4.665f);
    glVertex2f(9.370f, -4.980f);
    glVertex2f(9.480f, -4.980f);
    glVertex2f(9.500f, -4.728f);
    glVertex2f(9.572f, -4.980f);
    glVertex2f(9.682f, -4.980f);
    glVertex2f(9.592f, -4.554f);
    glVertex2f(9.722f, -4.980f);
    glVertex2f(9.832f, -4.980f);
    glVertex2f(9.852f, -4.639f);
    glVertex2f(9.924f, -4.980f);
    glVertex2f(10.034f, -4.980f);
    glVertex2f(9.944f, -4.623f);
    glVertex2f(10.074f, -4.980f);
    glVertex2f(10.184f, -4.980f);
    glVertex2f(10.204f, -4.694f);
    glVertex2f(10.276f, -4.980f);
    glVertex2f(10.386f, -4.980f);
    glVertex2f(10.296f, -4.511f);
    glVertex2f(10.426f, -4.980f);
    glVertex2f(10.536f, -4.980f);
    glVertex2f(10.556f, -4.605f);
    glVertex2f(10.628f, -4.980f);
    glVertex2f(10.738f, -4.980f);
    glVertex2f(10.648f, -4.580f);
    glVertex2f(10.778f, -4.980f);
    glVertex2f(10.888f, -4.980f);
    glVertex2f(10.908f, -4.660f);
    glVertex2f(10.980f, -4.980f);
    glVertex2f(11.090f, -4.980f);
    glVertex2f(11.000f, -4.649f);
    glVertex2f(11.130f, -4.980f);
    glVertex2f(11.240f, -4.980f);
    glVertex2f(11.260f, -4.715f);
    glVertex2f(11.332f, -4.980f);
    glVertex2f(11.442f, -4.980f);
    glVertex2f(11.352f, -4.538f);
    glVertex2f(11.482f, -4.980f);
    glVertex2f(11.592f, -4.980f);
    glVertex2f(11.612f, -4.626f);
    glVertex2f(11.684f, -4.980f);
    glVertex2f(11.794f, -4.980f);
    glVertex2f(11.704f, -4.607f);
    glVertex2f(11.834f, -4.980f);
    glVertex2f(11.944f, -4.980f);
    glVertex2f(11.964f, -4.681f);
    glEnd();

    glDisable(GL_LIGHTING); // back to ordinary colours

    /* ================= rain of the wet season ================= */
    if (rainOn == 1)
    {
        glPushMatrix();
        glTranslatef(0.0f, rainDrop, 0.0f);
        glColor3ub(188, 214, 236);
        /* 280 slanted streaks of rain */
        /* where each column of rain starts : 35 columns, 8 drops in each */
        float rainX[35] = {
            -12.43f, -11.916f, -11.079f, -10.574f, -9.574f, -9.127f, -8.107f,
            -7.614f, -6.973f, -5.95f, -5.39f, -4.704f, -3.843f, -3.329f,
            -2.613f, -1.887f, -1.213f, -0.413f, 0.39f, 1.051f, 1.801f,
            2.528f, 3.218f, 3.781f, 4.562f, 5.481f, 6.057f, 6.86f,
            7.418f, 8.189f, 9.098f, 9.742f, 10.588f, 11.201f, 11.884f
        };
        float rainY[35] = {
            -4.398f, -5.208f, -4.106f, -5.66f, -5.698f, -4.387f, -4.347f,
            -4.685f, -4.903f, -5.972f, -4.701f, -4.133f, -4.257f, -5.578f,
            -5.414f, -4.827f, -5.162f, -4.18f, -5.084f, -4.191f, -4.165f,
            -4.936f, -5.963f, -5.634f, -4.402f, -5.053f, -4.887f, -4.963f,
            -4.431f, -4.879f, -5.446f, -4.985f, -4.48f, -5.114f, -4.989f
        };
        glBegin(GL_LINES);
        for (int c = 0; c < 35; c = c + 1)
        {
            for (int d = 0; d < 8; d = d + 1)
            {
                glVertex2f(rainX[c], rainY[c] + 2.0f * d);
                glVertex2f(rainX[c] + -0.17f, rainY[c] + 2.0f * d + -0.52f);
            }
        }
        glEnd();
        glPopMatrix();
    }

    glutSwapBuffers(); // show the finished picture (double buffering)
}

/* Main function: GLUT runs as a console application starting at main() */

int main(int argc, char **argv)
{

    glutInit(&argc, argv); // Initialize GLUT

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // DOUBLE buffer : no flicker

    glutInitWindowSize(1920, 1200); // Window width & height

    glutInitWindowPosition(40, 40); // Top-left corner

    glutCreateWindow("Dinajpur Railway Station"); // Create window with the given title

    glutDisplayFunc(display); // Register display callback handler

    initGL(); // Our own OpenGL initialization

    glutSpecialFunc(SpecialInput); // arrow keys stop / start the train

    glutKeyboardFunc(NormalInput); // the R key turns the rain on and off

    glutTimerFunc(30, update, 0); // start the animation

    /* the picture opens on a clear day, so open on the station sound too */
    PlaySound("train_station.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

    glutMainLoop(); // Enter the event-processing loop

    return 0;
}