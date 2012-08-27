#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
  private:
	float red, green, blue;
};

// this is the function called to setup the app.  Variables are initialized here.
void CatPictureApp::setup()
{
	red = 0;
	green = 0;
	blue = 0;
}

// this function is run whenever a mouse button is pressed down.
// I'm assuming it only works for left, middle, and right mouse buttons, as the forward/backward buttons on my mouse do not trigger this function.
void CatPictureApp::mouseDown( MouseEvent event )
{
	// change the screen color by checking the mouse position in the window
	// and dividing by the height/width to get a ratio from 0 to 1 for red and green
	
	// viewport saves as an array [0, 0, width, height] if I'm not mistaken.
	GLint window_viewport[4];

	glGetIntegerv( GL_VIEWPORT, window_viewport );
	
	red = (float)(event.getX()) / window_viewport[2];
	green = (float)(event.getY()) / window_viewport[3];
	// set the blue ratio to a random float between 0 and 1.
	blue = 0;
}

// This is the update function to update variables and such as needed.
void CatPictureApp::update()
{
	if(blue < 1.0){
		blue += 0.01;
	}
	else{
		blue = 0;
	}
}

// this is the draw function to display things in the window created.
void CatPictureApp::draw()
{
	// clear out the window to the value determined by the RGB set in mouseDown( MouseEvent )
	gl::clear( Color( red, green, blue ) ); 
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
