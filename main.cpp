
#include <stdlib.h>
#include <GL/glut.h>
#include <fmod.h>
#include <fmod_errors.h>
#include <iostream>

#ifdef WIN32
	#include <windows.h>
	// automatically link to fmod library
	#pragma comment(lib,"fmod.lib")
#else
	#include <wincompat.h>
#endif

/// this is a pointer to the mp3 stream we will be reading from the disk.
FSOUND_STREAM* g_mp3_stream = NULL;


//------------------------------------------------------------	OnReshape()
//
void OnReshape(int w, int h)
{
	if (h==0) {
		h=1;
	}

	// set the drawable region of the window
	glViewport(0,0,w,h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// just use a perspective projection
	glOrtho(0,532,0,10,-100,100);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//------------------------------------------------------------	OnDraw()
//
void OnDraw() {

	// clear the screen & depth buffer
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// clear the previous transform
	glLoadIdentity();

	// FSOUND_DSP_GetSpectrum returns a pointer to an array of 512
	// floats representing the frequencies of the sound. 
	// 
	const float* spectrum = FSOUND_DSP_GetSpectrum();

	// just walk through the array and render end spectrum value
	glBegin(GL_LINE_STRIP);
	for(unsigned int i=0;i!=512;++i) 
	{
		glColor3f(1.0-4*spectrum[i],4*spectrum[i],0);
		glVertex2f(10+i,0.5+20*spectrum[i]);
	}
	glEnd();

	// currently we've been drawing to the back buffer, we need
	// to swap the back buffer with the front one to make the image visible
	glutSwapBuffers();
}

//------------------------------------------------------------	OnIdle()
//
void OnIdle() {
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}


//------------------------------------------------------------	OnInit()
//
void OnInit() {
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
}

//------------------------------------------------------------	OnExit()
//
void OnExit() {

	// Stop and close the mp3 file
	FSOUND_Stream_Stop( g_mp3_stream );
	FSOUND_Stream_Close( g_mp3_stream );

	// kill off fmod
	FSOUND_Close();
}


//------------------------------------------------------------	main()
//
int main(int argc,char** argv) {

	// initialise fmod, 44000 Hz, 64 channels
	if( FSOUND_Init(32000,32,0) == FALSE )
	{
		std::cerr << "[ERROR] Could not initialise fmod\n";
		return 0;
	}

	// attempt to open the mp3 file as a stream
	g_mp3_stream = FSOUND_Stream_Open( "Hawaii5O.mp3" , FSOUND_2D , 0 , 0 );

	// make sure mp3 opened OK
	if(!g_mp3_stream) {
		std::cerr << "[ERROR] could not open file\n";
		return 0;
	}

	// play the mp3
	FSOUND_Stream_Play(0,g_mp3_stream);


	// get a pointer to fmods fft (fast fourier transform) unit 
	DLL_API FSOUND_DSPUNIT *fft = FSOUND_DSP_GetFFTUnit();

	// enable the fft unit 
	FSOUND_DSP_SetActive(fft,TRUE);


	// initialise glut
	glutInit(&argc,argv);

	// request a depth buffer, RGBA display mode, and we want double buffering
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);

	// set the initial window size
	glutInitWindowSize(640,480);

	// create the window
	glutCreateWindow("A Basic Queue");

	// set the function to use to draw our scene
	glutDisplayFunc(OnDraw);

	// set the function to handle changes in screen size
	glutReshapeFunc(OnReshape);

	// set the idle callback
	glutIdleFunc(OnIdle);

	// run our custom initialisation
	OnInit();

	// set the function to be called when we exit
	atexit(OnExit);

	// this function runs a while loop to keep the program running.
	glutMainLoop();
	return 0;
}

