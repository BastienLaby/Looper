#include <iostream>
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


/**********************************************************************/
/*||                                   							    ||*/
/*||             				  MAIN   			                ||*/
/*||________________________________________________________________||*/
/**********************************************************************/


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
          /* Problem: glewInit failed, something is seriously wrong. */
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
          exit(EXIT_FAILURE);
    }
    // Ensure we can capture the escape key being pressed below
    glfwEnable(GLFW_STICKY_KEYS);
    // Enable vertical sync (on cards that support it)
    glfwSwapInterval(1);



	// Create videoCaptor
	cv::VideoCapture videoCaptor(cameraId);
	videoCaptor.set(CV_CAP_PROP_FRAME_WIDTH,WINDOW_WIDTH);
	videoCaptor.set(CV_CAP_PROP_FRAME_HEIGHT,WINDOW_HEIGHT);
	if(!videoCaptor.isOpened())
	{
		std::cout << "Failed to connect to the camera." << std::endl;
	}

	/* --------------------------------------------------------------------- */
	/* ----------------------- TRY PLAYING A SOUND ------------------------- */
	/* --------------------------------------------------------------------- */

	if(verbose)		std::cerr<<"Play a sound"<<std::endl;

	sound::SoundPlayer soundPlayer;
	// load every sounds in folder music
	soundPlayer.loadFromFolder(MUSIC_PATH.c_str());
	//soundPlayer.loadSound("03 Thrift Shop (feat. Wanz).mp3");
	soundPlayer.play(0);
	sleep(1);
	soundPlayer.play(0);

	fouch::Timer timer;
	
	float fps = 0.f;
	double t;
	
	do {

		t = glfwGetTime();

		// Load image model + capture webcam image
		timer.breakpoint("Capture Images");
		
		cv::Mat img_scene;
		videoCaptor >> img_scene;
		if(img_scene.empty())
		{
			std::cout << "Failed to capture the webcam image" << std::endl;
			// TODO : gérer le cas ou l'on rate une image
			return (EXIT_FAILURE);
		}

		// *****************************
		// DETECTION
		// *****************************

		timer.breakpoint("Detect pattern");

		Mat img_object = imread( (IMG_PATH + "pattern_lite.png").c_str());
		if(!img_object.data)
		{
			std::cout<< " --(!) Error reading obj image " << std::endl;
			return -1;
		}

		if(!img_scene.data)
		{
			std::cout<< " --(!) Error reading scene image " << std::endl;
			return -1;
		}

		// Step 1: Detect the keypoints using SURF Detector
		int minHessian = 400;
		if(verbose)	std::cerr<<"detector creation"<<std::endl;
		SurfFeatureDetector detector( minHessian );
		std::vector<KeyPoint> keypoints_object, keypoints_scene;
		
		if(verbose)	std::cerr<<"Points detection"<<std::endl;
		detector.detect( img_object, keypoints_object );
		detector.detect( img_scene, keypoints_scene );

		// Step 2: Calculate descriptors (feature vectors)
		SurfDescriptorExtractor extractor;
		Mat descriptors_object, descriptors_scene;
		if(verbose)	std::cerr<<"Objects extraction from points"<<std::endl;
		extractor.compute( img_object, keypoints_object, descriptors_object );
		extractor.compute( img_scene, keypoints_scene, descriptors_scene );

		// Step 3: Matching descriptor vectors using FLANN matcher
		FlannBasedMatcher matcher;
		std::vector< DMatch > matches;
		matcher.match( descriptors_object, descriptors_scene, matches );
		double max_dist = 0; double min_dist = 100;

		//	Quick calculation of max and min distances between keypoints
		for( int i = 0; i < descriptors_object.rows; i++ )
		{
			double dist = matches[i].distance;
			if( dist < min_dist ) min_dist = dist;
			if( dist > max_dist ) max_dist = dist;
		}

		if(verbose)	printf("-- Max dist : %f \n", max_dist );
		if(verbose)	printf("-- Min dist : %f \n", min_dist );

		//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
		timer.breakpoint("Drawing matches");
		std::vector< DMatch > good_matches;

		for( int i = 0; i < descriptors_object.rows; i++ )
		{
			if(matches[i].distance < 3*min_dist)
			{
				good_matches.push_back( matches[i]);
			}
		}

		Mat img_matches;
		drawMatches(img_object,
					keypoints_object,
					img_scene,
					keypoints_scene,
					good_matches,
					img_matches,
					Scalar::all(-1),
					Scalar::all(-1),
					vector<char>(),
					DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		// Localize the object
		std::vector<Point2f> obj;
		std::vector<Point2f> scene;

		for(int i = 0; i < good_matches.size(); i++)
		{
			// Get the keypoints from the good matches
			obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
			scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
		}

		Mat H = findHomography( obj, scene, CV_RANSAC );

		// Get the corners from the image_1 ( the object to be "detected" )
		std::vector<Point2f> obj_corners(4);
		obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
		obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
		std::vector<Point2f> scene_corners(4);
		perspectiveTransform( obj_corners, scene_corners, H);


		/* --------------------------------------------------------------------- */
		/* ----------------------- ROLLER COASTER LOOP ------------------------- */
		/* --------------------------------------------------------------------- */

		if(verbose)	 std::cerr<<"drawing lines"<<std::endl;

		// Draw lines between the corners (the mapped object in the scene - image_2 )
		line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
		line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
		line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
		line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

		// TODO : verify the img_matches matrix isn't empty
		unsigned char *input = (unsigned char*)(img_matches.data);
		if(verbose) 	std::cerr<<"Size of captured image : "<<img_matches.cols<<" x "<<img_matches.rows<<std::endl;

		timer.breakpoint("Convert format");

		// Show detected matches
		IplImage* img = new IplImage(img_matches);

		timer.breakpoint("Display content");

		timer.breakpoint("Events Handling");

		float fps = timer.fps();
		ostringstream convert;
		convert << fps;

		// Swap buffers
        glfwSwapBuffers();
        double newTime = glfwGetTime();
        fps = 1.f/ (newTime - t);
	}
	while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED) );

	// Close OpenGL window and terminate GLFW
    glfwTerminate();

	return (EXIT_SUCCESS);
}
