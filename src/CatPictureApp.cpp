#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"
#include <cmath>

// This project includes:
//	A: Rectangle, Circle, Gradient, Copy
//	B: Laplace
//	C: Draw()
//	D: ScreenShot()
//	E: Animation, Mouse Interaction


using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void prepareSettings(Settings*);
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	// added functionality includes Rectangle, Circle, Gradient, Copy, Laplace, Screenshot (save picture), animation, and mouse click events
	void rect(int[], int[]);
	void rect(int[], int[], uint8_t*);
	void circle(int[], int, uint8_t*);
	void semi_circle(int[], int, uint8_t*);
	void gradient(int, uint8_t*, uint8_t*);
	void copy(int[], int[], int[]);
	void laplace();
	void screenShot();
	// animation and mouse click included on things I included in the project.
	void curtain(int, uint8_t*, uint8_t*); // uses a varient of gradient to draw a theater-like curtain across the surface.
	// Curtain is unused in the project but included for both future reference and other students

  private:
	void phaseInc();
	float red, green, blue;
	int phase, gradientX;
	int timer;
	int screenShotNum;
	Surface* m_surface;
	// create surface consants
	static const int surfaceSize_ = 1028;
	static const int imgWidth_ = 800;
	static const int imgHeight_ = 600;
	// create ball variables
	struct pBall{
		int x, y, r;
	};
	uint8_t* imgData_;
	uint8_t* surfaceData_;
	int radius(int, int, int, int);
};

// get the window set up to be the correct size
void CatPictureApp::prepareSettings(Settings* settings){
	(*settings).setWindowSize(imgWidth_, imgHeight_);
	(*settings).setResizable(false);
}

// fill rectangular region with black color
void CatPictureApp::rect(int* pos1, int* pos2){
	if(pos1[0] > pos2[0] ||
		pos1[1] > pos2[1] ||
		pos2[0] > imgWidth_ ||
		pos2[1] > imgHeight_){
			console() << "Copy parameters out of range" << std::endl;

			if(pos1[0] > pos2[0]){
				console() << "First condition " << pos1[0] << " " << pos2[0] << endl;
			}else if(pos1[1] > pos2[1]){
				console() << "Second condition " << pos1[1] << " " << pos2[1] << endl;
			}else if(pos2[0] > imgWidth_){
				console() << "Third condition " << pos2[0] << " " << imgWidth_ << endl;
			}else{
				console() << "Fourth condition " << pos2[1] << " " << imgHeight_ << endl;
			}
	}


	for(int y = pos1[1]; y < pos2[1] && y < surfaceSize_; y++){
		for(int x = pos1[0]; x < pos2[0] && x < surfaceSize_; x++){
			if(x < imgWidth_ && y < imgHeight_ && x >=0 && y >= 0){
				int offset = 3*(x + y *surfaceSize_);
				surfaceData_[offset] = 0;
				surfaceData_[offset + 1] = 0;
				surfaceData_[offset + 2] = 0;
			}
		}
	}
}

// fill rectangular region with a specified color
void CatPictureApp::rect(int* pos1, int* pos2, uint8_t* color){
	if(pos1[0] > pos2[0] ||
		pos1[1] > pos2[1] ||
		pos2[0] > imgWidth_ ||
		pos2[1] > imgHeight_){
			console() << "Copy parameters out of range" << std::endl;

			if(pos1[0] > pos2[0]){
				console() << "First condition " << pos1[0] << " " << pos2[0] << endl;
			}else if(pos1[1] > pos2[1]){
				console() << "Second condition " << pos1[1] << " " << pos2[1] << endl;
			}else if(pos2[0] > imgWidth_){
				console() << "Third condition " << pos2[0] << " " << imgWidth_ << endl;
			}else{
				console() << "Fourth condition " << pos2[1] << " " << imgHeight_ << endl;
			}
	}


	for(int y = pos1[1]; y < pos2[1] && y < surfaceSize_; y++){
		for(int x = pos1[0]; x < pos2[0] && x < surfaceSize_; x++){
			if(x < imgWidth_ && y < imgHeight_ && x >=0 && y >= 0){
				int offset = 3*(x + y *surfaceSize_);
				surfaceData_[offset] = color[0];
				surfaceData_[offset + 1] = color[1];
				surfaceData_[offset + 2] = color[2];
			}
		}
	}
}

// copies a rectangular area of the surface and pastes it elsewhere
void CatPictureApp::copy(int* cpyFrom1, int* cpyFrom2, int* cpyTo){
	if(cpyFrom1[0] > cpyFrom2[0] ||
		cpyFrom1[1] > cpyFrom2[1] ||
		cpyFrom2[0] > imgWidth_ ||
		cpyFrom2[1] > imgHeight_){
			console() << "Copy parameters out of range" << std::endl;

			if(cpyFrom1[0] > cpyFrom2[0]){
				console() << "First condition " << cpyFrom1[0] << " " << cpyFrom2[0] << endl;
			}else if(cpyFrom1[1] > cpyFrom2[1]){
				console() << "Second condition " << cpyFrom1[1] << " " << cpyFrom2[1] << endl;
			}else if(cpyFrom2[0] > imgWidth_){
				console() << "Third condition " << cpyFrom2[0] << " " << imgWidth_ << endl;
			}else{
				console() << "Fourth condition " << cpyFrom2[1] << " " << imgHeight_ << endl;
			}
	}

	int* difference = new int[2];
	difference[0] = cpyFrom1[0] - cpyTo[0];
	difference[1] = cpyFrom1[1] - cpyTo[1];

	for(int y = cpyFrom1[1]; y < cpyFrom2[1] && y < surfaceSize_; y++){
		for(int x = cpyFrom1[0]; x < cpyFrom2[0] && x < surfaceSize_; x++){
			if((x - difference[0]) < imgWidth_ && (y - difference[1]) < imgHeight_ && 
				(x - difference[0]) >=0 && (y - difference[1]) >= 0){
				int offset1 = 3*(x - difference[0] + (y - difference[1])*surfaceSize_);
				int offset2 = 3*(x + y*surfaceSize_);
				surfaceData_[offset1] = surfaceData_[offset2];
				surfaceData_[offset1 + 1] = surfaceData_[offset2 + 1];
				surfaceData_[offset1 + 2] = surfaceData_[offset2 + 2];
			}
		}
	}

	delete(difference);
}

// draws a gradient across the window from x = 0 to toX using two input colors
// This is easily animated
void CatPictureApp::gradient(int toX, uint8_t*  col1, uint8_t* col2){
	double* difference = new double[3];
	
	difference[0] = col1[0] - col2[0];
	difference[1] = col1[1] - col2[1];
	difference[2] = col1[2] - col2[2];

	for(int y = 0; y < imgHeight_; y++){
		for(int x = 0; x < toX && x < imgWidth_; x++){
			int offset = 3*(x + y *surfaceSize_);
			double fraction = (double)x / imgWidth_;
			
			surfaceData_[offset] = (uint8_t)abs(col1[0] - fraction * difference[0]);
			surfaceData_[offset + 1] = (uint8_t)abs(col1[1] - fraction * difference[1]);
			surfaceData_[offset + 2] = (uint8_t)abs(col1[2] - fraction * difference[0]);
			
			/*
			surfaceData_[offset] = (uint8_t)(difference[0] + col1[0]);
			surfaceData_[offset + 1] = (uint8_t)(difference[1] + col1[1]);
			surfaceData_[offset + 2] = (uint8_t)(difference[0] + col1[2]);
			*/
		}
	}

	delete(difference);
}

// draws a nice theater curtain effect with col1 = red and col2 = blue
// I do not use it here, but I left it for others to implement if they want
void CatPictureApp::curtain(int toX, uint8_t*  col1, uint8_t* col2){
	double* difference = new double[3];
	
	difference[0] = col1[0] - col2[0];
	difference[1] = col1[1] - col2[1];
	difference[2] = col1[2] - col2[2];

	for(int y = 0; y < imgHeight_; y++){
		for(int x = 0; x < toX && x < imgWidth_; x++){
			int offset = 3*(x + y *surfaceSize_);
			double fraction = (double)x / imgWidth_;

			surfaceData_[offset] = (uint8_t)(fraction * difference[0] * col1[0]);
			surfaceData_[offset + 1] = (uint8_t)(fraction * difference[1] * col1[1]);
			surfaceData_[offset + 2] = (uint8_t)(fraction * difference[0] * col1[2]);

		}
	}

	delete(difference);
}

// This is used for calculating the distance between two points
int CatPictureApp::radius(int pos1x, int pos1y, int pos2x, int pos2y){
	return sqrt((double)(pos1x - pos2x) * (double)(pos1x - pos2x) + (double)(pos1y - pos2y) * (double)(pos1y - pos2y));
}

// Draws a circle on the surface in a specified color
void CatPictureApp::circle(int* pos, int radius_, uint8_t* color){
	pBall circle;
	circle.x = pos[0];
	circle.y = pos[1];
	circle.r = radius_;
	
	for(int y = 0; y < surfaceSize_; y++){
		for(int x = 0; x < surfaceSize_; x++){
			if(x < imgWidth_ && y < imgHeight_ && x >=0 && y >= 0){
				// check radius
				if(radius(pos[0], pos[1], x, y) <= radius_){
					int offset = 3*(x + y *surfaceSize_);
					surfaceData_[offset] = color[0];
					surfaceData_[offset + 1] = color[1];
					surfaceData_[offset + 2] = color[2];
				}
			}
		}
	}
}

// This works just like circle(), but it only overwrites half the circle
void CatPictureApp::semi_circle(int* pos, int radius_, uint8_t* color){
	pBall circle;
	circle.x = pos[0];
	circle.y = pos[1];
	circle.r = radius_;
	
	for(int y = 0; y < surfaceSize_ && y < pos[1]; y++){
		for(int x = 0; x < surfaceSize_; x++){
			if(x < imgWidth_ && y < imgHeight_ && x >=0 && y >= 0){
				// check radius
				if(radius(pos[0], pos[1], x, y) <= radius_){
					int offset = 3*(x + y *surfaceSize_);
					surfaceData_[offset] = color[0];
					surfaceData_[offset + 1] = color[1];
					surfaceData_[offset + 2] = color[2];
				}
			}
		}
	}
}

// this is the function called to setup the app.  Variables are initialized here.
void CatPictureApp::setup()
{
	// initialize variables
	red = 0;
	green = 0;
	blue = 0;

	phase = 0;
	gradientX = 0;
	timer = 0;
	screenShotNum = 0;

	// initialize surface
	m_surface = new Surface(surfaceSize_, surfaceSize_, false);
	Surface startScreen = loadImage( loadResource(RES_START) );
	imgData_ = startScreen.getData();

	surfaceData_ = (*m_surface).getData();

	// set surface data
	for(int y = 0; y < imgHeight_; y++){
		for(int x = 0; x < imgWidth_; x++){
			int offset1 = 3*(x + y*surfaceSize_);
			int offset2 = 3*(x + y*imgWidth_);
			surfaceData_[offset1] = imgData_[offset2];
			surfaceData_[offset1 + 1] = imgData_[offset2 + 1];
			surfaceData_[offset1 + 2] = imgData_[offset2 + 2];
		}
	}
}

// Take a screenshot and save it.
// This method dynamically allocates pictures per instance of the program.
// Running it multiple times will not save the screenShot count, so it will override previous images
void CatPictureApp::screenShot(){
	// Create the filename to use
	stringstream nameStream;
	nameStream << "MRMarine pic" << screenShotNum << ".png";
	string fileName = nameStream.str();

	// create the surface and save the current screen to it.
	Surface screenShot = loadImage( loadResource(RES_START) );
	uint8_t* surfaceData2_ = screenShot.getData();

	for(int y = 0; y < imgHeight_; y++){
		for(int x = 0; x < imgWidth_; x++){
			int offset2 = 3*(x + y*surfaceSize_);
			int offset1 = 3*(x + y*imgWidth_);
			surfaceData2_[offset1] = surfaceData_[offset2];
			surfaceData2_[offset1 + 1] = surfaceData_[offset2 + 1];
			surfaceData2_[offset1 + 2] = surfaceData_[offset2 + 2];
		}
	}

	// save the screenshot
	writeImage(fileName,screenShot);

	//delete(surfaceData2_);
}

// this function is run whenever a mouse button is pressed down.
// I'm assuming it only works for left, middle, and right mouse buttons, as the forward/backward buttons on my mouse do not trigger this function.
void CatPictureApp::mouseDown( MouseEvent event )
{
	

	// check the location and react accordingly
	int x = event.getX();
	int y = event.getY();

	if(phase == 0){
		if(x >= 50 && x <= 675
			&& y >= 10 && y <= 300){
				phaseInc();
		}
		else{
			// take a screenshot and increment counter
			screenShot();
			screenShotNum++;
			// output the location clicked
			console() << "x: " << x << " y: " << y << std::endl;
		}
	}else if(phase == 1){
		phaseInc();
	}
	else{
		// take a screenshot and increment counter
		screenShot();
		screenShotNum++;
	}
}

// This inscreases the phase for animation and prints the phase to the console
void CatPictureApp::phaseInc(){
	console() << "Phase increased to " << ++phase << endl;
}

// Performs a laplace transform on the surface
void CatPictureApp::laplace(){
	uint8_t red_, blue_, green_;

	uint8_t kernel[9] = {
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1};

	// create the surface and save the current screen to it.
	Surface screenShot = loadImage( loadResource(RES_START) ); // this is to get the right size, since the image is the same dimensions as the visible Surface
	uint8_t* surfaceData2_ = screenShot.getData();

	for(int y = 0; y < imgHeight_; y++){
		for(int x = 0; x < imgWidth_; x++){
			int offset2 = 3*(x + y*surfaceSize_);
			int offset1 = 3*(x + y*imgWidth_);
			surfaceData2_[offset1] = surfaceData_[offset2];
			surfaceData2_[offset1 + 1] = surfaceData_[offset2 + 1];
			surfaceData2_[offset1 + 2] = surfaceData_[offset2 + 2];
		}
	}

	for(int y = 1; y < imgHeight_ - 1; y++){
		for(int x = 1; x < imgWidth_ - 1; x++){
			// reset colors
			red_ = 0;
			blue_ = 0;
			green_ = 0;

			for(int ky = -1; ky <= 1; ky++){
				for(int kx = -1; kx <= 1; kx++){
					int offset = 3*(x + kx + (y + ky) * surfaceSize_);
					int k = kernel[kx + 1 + (ky + 1) * 3];

					red_ += surfaceData_[offset] * k;
					blue_ += surfaceData_[offset + 1] * k;
					green_ += surfaceData_[offset + 2] * k;
				}
			}

			// set surface pixel
			int offset = 3*(x + y * imgWidth_);
			surfaceData2_[offset] = red_;
			surfaceData2_[offset + 1] = blue_;
			surfaceData2_[offset + 2] = green_;
		}
	}

	// copy the image over to the active surface
	for(int y = 0; y < imgHeight_; y++){
		for(int x = 0; x < imgWidth_; x++){
			int offset1 = 3*(x + y*surfaceSize_);
			int offset2 = 3*(x + y*imgWidth_);
			surfaceData_[offset1] = surfaceData2_[offset2];
			surfaceData_[offset1 + 1] = surfaceData2_[offset2 + 1];
			surfaceData_[offset1 + 2] = surfaceData2_[offset2 + 2];
		}
	}

}

// This is the update function to update variables and such as needed.
void CatPictureApp::update()
{
	if(phase == 0){
		int* from1, * from2, * to;
		from1 = new int[2];
		from2 = new int[2];
		to = new int[2];

		from1[0] = 50;
		from1[1] = 0;

		from2[0] = 675;
		from2[1] = 300;

		to[0] = 50;
		to[1] = 300;

		copy(from1, from2, to);

		delete(from1);
		delete(from2);
		delete(to);
	}else if(phase == 1){
		int* rect1, * rect2;
		rect1 = new int[2];
		rect2 = new int[2];

		rect1[0] = 50;
		rect1[1] = 300;

		rect2[0] = 665;
		rect2[1] = 590;

		rect(rect1, rect2);

		delete(rect1);
		delete(rect2);
	}else if(phase == 2){
		uint8_t* gradCol1 = new uint8_t[3];
		gradCol1[0] = 255;
		gradCol1[1] = 0;
		gradCol1[2] = 0;

		uint8_t* gradCol2 = new uint8_t[3];
		gradCol2[0] = 0;
		gradCol2[1] = 0;
		gradCol2[2] = 255;

		gradient(gradientX, gradCol1, gradCol2);

		delete(gradCol2);
		delete(gradCol1);
		gradientX += 5;

		if(gradientX >= imgWidth_ + 50){
			phaseInc();
		}
	}
	else if(phase == 3){
		uint8_t* white = new uint8_t[3];
		white[0] = 255;
		white[1] = 255;
		white[2] = 255;

		int* outerRect1, * outerRect2, * innerRect1, * innerRect2, * innerRect3, * innerRect4;
		outerRect1 = new int[2];
		outerRect2 = new int[2];
		innerRect1 = new int[2];
		innerRect2 = new int[2];
		innerRect3 = new int[2];
		innerRect4 = new int[2];

		outerRect1[0] = 50;
		outerRect1[1] = 50;

		outerRect2[0] = imgWidth_ - 50;
		outerRect2[1] = imgHeight_ - 50;

		innerRect1[0] = 70;
		innerRect1[1] = 70;

		innerRect2[0] = imgWidth_ / 2 - 10;
		innerRect2[1] = imgHeight_ - 70;

		innerRect3[0] = imgWidth_ / 2 + 10;
		innerRect3[1] = 70;

		innerRect4[0] = imgWidth_ - 70;
		innerRect4[1] = imgHeight_ - 70;


		// black rect
		rect(outerRect1, outerRect2);

		//white rectangles
		rect(innerRect1, innerRect2, white);
		rect(innerRect3, innerRect4, white);

		delete(white);
		delete(outerRect1);
		delete(outerRect2);
		delete(innerRect1);
		delete(innerRect2);
		delete(innerRect3);
		delete(innerRect4);

		timer += 1;

		if(timer >= 10){
			phaseInc();
			timer = 0;
		}
	}
	else if(phase == 4){

		int* cirPos = new int[2];
		cirPos[0] = imgWidth_ / 2;
		cirPos[1] = imgHeight_ / 2;

		int radius_1 = 100;
		int radius_2 = 90;
		int radius_3 = 20;
		int radius_4 = 15;
		int radius_5 = 10;

		// black
		uint8_t* cirCol1 = new uint8_t[3];
		cirCol1[0] = 0;
		cirCol1[1] = 0;
		cirCol1[2] = 0;

		// white
		uint8_t* cirCol2 = new uint8_t[3];
		cirCol2[0] = 255;
		cirCol2[1] = 255;
		cirCol2[2] = 255;

		// red
		uint8_t* cirCol3 = new uint8_t[3];
		cirCol2[0] = 255;
		cirCol2[1] = 0;
		cirCol2[2] = 0;
		
		// black circle
		circle(cirPos, radius_1, cirCol1);
		circle(cirPos, radius_2, cirCol3);
		semi_circle(cirPos, radius_2, cirCol2);
		circle(cirPos, radius_3, cirCol1);
		circle(cirPos, radius_4, cirCol3);
		circle(cirPos, radius_5, cirCol2);

		// time based step
		timer += 1;
		if(timer >= 10){
			phaseInc();
			timer = 0;
		}
	}
	else if(phase == 5){
		laplace();
		phaseInc();
	}

}

// this is the draw function to display things in the window created.
void CatPictureApp::draw()
{
	// clear out the window to the value determined by the RGB set in mouseDown( MouseEvent )
	gl::clear( Color( red, green, blue ) ); 
	gl::draw(*m_surface);
}



CINDER_APP_BASIC( CatPictureApp, RendererGl )
