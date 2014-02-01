/*#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "glew/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glfw.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include "sound/SoundPlayer.hpp"

#include "fouch/Timer.hpp"

#include <vector>
#include <string>

size_t WINDOW_WIDTH, WINDOW_HEIGHT;

static const int FRAMES_PER_SECOND = 60;
static const int MIN_LOOP_TIME = 1000 / FRAMES_PER_SECOND;

static const std::string APP_NAME = "TablaTurn";

static const std::string MEDIA_PATH = "media/";
static const std::string FONT_PATH = MEDIA_PATH + "font/";
static const std::string IMG_PATH = MEDIA_PATH + "img/";
static const std::string MUSIC_PATH = MEDIA_PATH + "music/";
static const std::string OUTPUT_IMG_PATH = MEDIA_PATH + "output/";

bool verbose = false;

using namespace cv;
using namespace std;


bool fexists(const char *filename)
{
	ifstream ifile(filename);
	return ifile;
}


int main(int argc, char* argv[]) {

	// Get Camera ID
	size_t cameraId = 0;
	if(argc > 1)
	{
		if(argv[1] == "1") cameraId = 1;
		if(argv[1] == "2") cameraId = 2;
		if(argv[1] == "3") cameraId = 3;
		if(argv[1] == "4") cameraId = 4;
	}

	std::cerr<<"Number of args : "<<argc<<std::endl;

	for(int i = 1; i < argc; ++i)
		if (strcmp(argv[i], "-v") == 0
				|| strcmp(argv[i], "--verbose") == 0
				|| strcmp(argv[i], "--debug") == 0
				|| strcmp(argv[i], "-d") == 0 )
			verbose =  true;

	if(verbose)		std::cerr<<std::endl;
	if(verbose)		std::cerr<<std::endl;
	if(verbose)		std::cerr<<"\t\t\t- Welcome to tablaturn ! -"<<std::endl;
	if(verbose)		std::cerr<<std::endl;
	if(verbose)		std::cerr<<std::endl;
	if(verbose)		std::cerr<<"Verbose mode activated"<<std::endl;
	if(verbose)		std::cerr<<std::endl;

	if(verbose)		std::cerr<<"Setting camera mode"<<std::endl;


	// Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    WINDOW_WIDTH = 1024;
	WINDOW_HEIGHT = 780;
		
	// Open a window and create its OpenGL context
    if( !glfwOpenWindow( WINDOW_WIDTH, WINDOW_HEIGHT, 0,0,0,0, 24, 0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // Uncomment if you are on full screen
    // glfwEnable(GLFW_MOUSE_CURSOR);
    glfwSetWindowTitle( "Looper" );
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
          // Problem: glewInit failed, something is seriously wrong.
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
          exit(EXIT_FAILURE);
    }
    // Ensure we can capture the escape key being pressed below
    glfwEnable(GLFW_STICKY_KEYS);
    // Enable vertical sync (on cards that support it)
    glfwSwapInterval(1);

	if(verbose)		std::cerr<<"Play a sound"<<std::endl;

	sound::SoundPlayer soundPlayer;
	// load every sounds in folder music
	soundPlayer.loadFromFolder(MUSIC_PATH.c_str());
	//soundPlayer.loadSound("03 Thrift Shop (feat. Wanz).mp3");
	soundPlayer.play(0);

	fouch::Timer timer;
	
	float fps = 0.f;
	double t;

	do {

		t = glfwGetTime();

		// Load image model + capture webcam image
		timer.breakpoint("Capture Images");
		
		// DETECTION

		timer.breakpoint("Detect pattern");


		// Swap buffers
        glfwSwapBuffers();
        double newTime = glfwGetTime();
        fps = 1.f/ (newTime - t);
	}
	while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED) );

	// Close OpenGL window and terminate GLFW
    glfwTerminate();

	return (EXIT_SUCCESS);
}ep
*/

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cameraparams.h"
#include "patterndetector.h"

using namespace std;
using namespace cv;
using namespace ARma;

#define PAT_SIZE 64//equal to pattern_size variable (see below)
#define SAVE_VIDEO 0 //if true, it saves the video in "output.avi"
#define NUM_OF_PATTERNS 3// define the number of patterns you want to use

char* filename1="media/img/pattern1.png";//id=1
char* filename2="media/img/pattern2.png";//id=2
char* filename3="media/img/pattern3.png";//id=3

static int loadPattern(const char* , std::vector<cv::Mat>& , int& );

int main(int argc, char** argv) {

	std::vector<cv::Mat> patternLibrary;
	std::vector<Pattern> detectedPattern;
	int patternCount=0;

	/*create patterns' library using rotated versions of patterns 
	*/
	loadPattern(filename1, patternLibrary, patternCount);
#if (NUM_OF_PATTERNS==2)
	loadPattern(filename2, patternLibrary, patternCount);
#endif
#if (NUM_OF_PATTERNS==3)
	loadPattern(filename3, patternLibrary, patternCount);
#endif

	cout << patternCount << " patterns are loaded." << endl;
	
	int norm_pattern_size = PAT_SIZE;
	double fixed_thresh = 40;
	double adapt_thresh = 5;//non-used with FIXED_THRESHOLD mode
	int adapt_block_size = 45;//non-used with FIXED_THRESHOLD mode
	double confidenceThreshold = 0.35;
	int mode = 2;//1:FIXED_THRESHOLD, 2: ADAPTIVE_THRESHOLD

	PatternDetector myDetector( fixed_thresh, adapt_thresh, adapt_block_size, confidenceThreshold, norm_pattern_size, mode);
	CvCapture* capture = cvCaptureFromCAM(0);

	// Perso

#if (SAVE_VIDEO)
	CvVideoWriter *video_writer = cvCreateVideoWriter( "output.avi", -1, 25, cvSize(640,480) );
#endif

	int k=0;
	while(k<500) { //modify it for longer/shorter videos
		
		//mycapture >> imgMat; 
		IplImage* img = cvQueryFrame(capture);
		Mat imgMat = Mat(img);
		double tic=(double)cvGetTickCount();


		//run the detector
		myDetector.detect(imgMat, cameraMatrix, distortions, patternLibrary, detectedPattern); 

		double toc=(double)cvGetTickCount();
		double detectionTime = (toc-tic)/((double) cvGetTickFrequency()*1000);

		//augment the input frame (and print out the properties of pattern if you want)
		for (unsigned int i =0; i<detectedPattern.size(); i++){
			detectedPattern.at(i).showPattern();
			detectedPattern.at(i).draw( imgMat, cameraMatrix, distortions);
		}

#if (SAVE_VIDEO)
		cvWriteFrame(video_writer, &((IplImage) imgMat));
#endif
		imshow("result", imgMat);
		cvWaitKey(1);
		k++;

		detectedPattern.clear();
	}

#if (SAVE_VIDEO)
	cvReleaseVideoWriter(&video_writer);
#endif
	cvReleaseCapture(&capture);

	return 0;

}

int loadPattern(const char* filename, std::vector<cv::Mat>& library, int& patternCount){
	
	Mat img = imread(filename,0);
	


	if(img.cols!=img.rows){
		return -1;
		std::cout << "Not a square pattern" << std::endl;
	}

	int msize = PAT_SIZE; 

	Mat src(msize, msize, CV_8UC1);
	Point2f center((msize-1)/2.0f,(msize-1)/2.0f);
	Mat rot_mat(2,3,CV_32F);
	

	resize(img, src, Size(msize,msize));

	Mat subImg = src(Range(msize/4,3*msize/4), Range(msize/4,3*msize/4));
	library.push_back(subImg);

	rot_mat = getRotationMatrix2D( center, 90, 1.0);

	for (int i=1; i<50; i++){
		Mat dst= Mat(msize, msize, CV_8UC1);
		rot_mat = getRotationMatrix2D( center, -i*90, 1.0);
		warpAffine( src, dst , rot_mat, Size(msize,msize));
		Mat subImg = dst(Range(msize/4,3*msize/4), Range(msize/4,3*msize/4));
		library.push_back(subImg);	
	}

	patternCount++;
	return 1;
}

