#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cameraparams.h"
#include "opencv/patterndetector.h"
#include <sstream>
#include <iterator>
#include <map>

#include "glew/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glfw.h"
#include "stb/stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imguiRenderGL3.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

#include "render/CameraManager.hpp"
#include "render/IMGUITools.hpp"
#include "render/Texture.hpp"
#include "render/Renderer.hpp"
#include "render/Shader.hpp"
#include "render/Rectangle.hpp"

#include "sound/SoundPlayer.hpp"
#include "fouch/Timer.hpp"

static const std::string MEDIA_PATH = "media/";
static const std::string FONT_PATH = MEDIA_PATH + "font/";
static const std::string IMG_PATH = MEDIA_PATH + "img/";
static const std::string MUSIC_PATH = MEDIA_PATH + "music/";
static const std::string OUTPUT_IMG_PATH = MEDIA_PATH + "output/";


using namespace std;
using namespace cv;
using namespace ARma;


bool sortPattern (Pattern A,Pattern B) { return (A.id<B.id); }


int main(int argc, char** argv) {

	int camera = 0;
	if(argc < 2)
		fprintf(stderr, "No argument : Default camera will be choose (0)\n");
	else{
		istringstream convert(argv[1]);

		if ( !(convert >> camera) )
			camera = 0;
	}

	VideoCapture cap(camera);

	if(!cap.isOpened())  // check if we succeeded
		return -1;
	//cap.set(CV_CAP_PROP_FPS, 50.0);
	Mat frame;

	cap >> frame;


	//
	// Initialise GLFW
	//

	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit( EXIT_FAILURE );
	}
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	float widthf = (float) width;
	float heightf = (float) height;
	float w = widthf/100;
	float h = heightf/100;

	fprintf( stderr, "cam width : %f\n", widthf);
	fprintf( stderr, "cam height : %f\n", heightf);

	//
	// Open window and create GL Context
	//

	if( !glfwOpenWindow( width, height, 0,0,0,0, 24, 0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window\n" );

		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwEnable( GLFW_MOUSE_CURSOR );
	glfwSetWindowTitle( "Looper" );

	//
	// Init Glew
	//

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit( EXIT_FAILURE );
	}

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Enable vertical sync (on cards that support it)
	glfwSwapInterval( 1 );

	//
	// Load GL Camera
	//

	CameraManager cammg;
	int glcam = cammg.createCamera();
	cammg.switchTo(glcam);

	//
	// Init IMGUI
	//

	init_imgui();
	GUIStates guiStates;
	init_gui_states(guiStates);

	//
	// Load Texture
	//

	looper::Renderer renderer;


	looper::Texture frameTexture;
	frameTexture.setSource(frame);

	//
	// Load Shader
	//

	looper::Shader frameShader;
	frameShader.load("shaders/frame.glsl");
	renderer.useShaderProgram(frameShader);
	frameShader.createShaderLocation("Projection");
	frameShader.createShaderLocation("View");
	frameShader.createShaderLocation("Object");
	frameShader.createShaderLocation("DiffuseTexture");
	frameShader.sendUniformInteger("DiffuseTexture", 0);

	looper::Shader patternShader;
	patternShader.load("shaders/pattern.glsl");
	renderer.useShaderProgram(patternShader);
	patternShader.createShaderLocation("Projection");
	patternShader.createShaderLocation("View");
	patternShader.createShaderLocation("Object");
	patternShader.createShaderLocation("color");

	//
	// Create quad model
	//

	looper::Rectangle rectangle;

	//
	// Create and bind VAO and VBO
	//

	GLuint vbos[4];
	glGenBuffers(4, vbos);

	// Viewport
	glViewport(0, 0, width, height);

	std::map<size_t, size_t> patternSoundAssociation;

	sound::SoundPlayer soundPlayer;
	Pattern pattern;

	std::map<const char*, const char*> mapSounds;
	mapSounds.insert(std::make_pair("media/img/PatternBL.png", ""));
	mapSounds.insert(std::make_pair("media/img/Pattern_Loop.png", ""));
	mapSounds.insert(std::make_pair("media/img/Pattern_A.png", ""));
	mapSounds.insert(std::make_pair("media/img/Pattern7.png", ""));

	mapSounds.insert(std::make_pair("media/img/Pattern1.png", "caisseclair.mp3"));
	mapSounds.insert(std::make_pair("media/img/Pattern15.png", "bell.mp3"));
	mapSounds.insert(std::make_pair("media/img/Patter12.png", "tumba.mp3"));
	mapSounds.insert(std::make_pair("media/img/Pattern5.png", "grossecaisse.mp3"));
	mapSounds.insert(std::make_pair("media/img/Pattern6.png", "castagnettes.mp3"));
	mapSounds.insert(std::make_pair("media/img/Pattern9.png", "tambourin.mp3"));
	mapSounds.insert(std::make_pair("media/img/Pattern10.png", "03 Thrift Shop (feat. Wanz).mp3"));
	mapSounds.insert(std::make_pair("media/img/Pattern11.png", "castagnettes.mp3"));

	

	for (std::map<const char*, const char*>::iterator it = mapSounds.begin(); it != mapSounds.end(); ++it)
	{
		if(strcmp(it->second, "") == 0)
			patternSoundAssociation[pattern.loadPattern(it->first)] = 0;
		else
			patternSoundAssociation[pattern.loadPattern(it->first)] = soundPlayer.loadSound((MUSIC_PATH + it->second).c_str());
	}



	std::cerr << "pattern library size : " << pattern.getPatterns().size() << std::endl;

	double fixed_thresh = 40;
	double adapt_thresh = 5;//non-used with FIXED_THRESHOLD mode
	int adapt_block_size = 45;//non-used with FIXED_THRESHOLD mode
	double confidenceThreshold = 0.45;
	int mode = 2;//1:FIXED_THRESHOLD, 2: ADAPTIVE_THRESHOLD

	PatternDetector myDetector( fixed_thresh, adapt_thresh, adapt_block_size, confidenceThreshold, Pattern::patternSize, mode);
	std::vector<cv::Point2f> patternPositions, patternStartTop, patternStartBot, patternLoopTop, patternLoopBot;

	fouch::Timer timer;

	double loopStart = glfwGetTime() - 0.02; // Last time when the loop started
	double lastPosition = 0.;
	double loopTime = 1.9; // Time in s before looping


	do
	{
		if(glfwGetTime() - loopStart > loopTime)
			loopStart += loopTime;
		double currentPosition = (glfwGetTime() - loopStart) / loopTime;

		//
		// Handle user inputs
		//

		int leftButton = glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT );
		int rightButton = glfwGetMouseButton( GLFW_MOUSE_BUTTON_RIGHT );
		int middleButton = glfwGetMouseButton( GLFW_MOUSE_BUTTON_MIDDLE );

		if( leftButton == GLFW_PRESS )
			guiStates.turnLock = true;
		else
			guiStates.turnLock = false;

		if( rightButton == GLFW_PRESS )
			guiStates.zoomLock = true;
		else
			guiStates.zoomLock = false;

		if( middleButton == GLFW_PRESS )
			guiStates.panLock = true;
		else
			guiStates.panLock = false;

		// Camera movements
		int altPressed = glfwGetKey(GLFW_KEY_LSHIFT);
		if (!altPressed && (leftButton == GLFW_PRESS || rightButton == GLFW_PRESS || middleButton == GLFW_PRESS))
		{
			int x; int y;
			glfwGetMousePos(&x, &y);
			guiStates.lockPositionX = x;
			guiStates.lockPositionY = y;
		}
		if (altPressed == GLFW_PRESS)
		{
			int mousex; int mousey;
			glfwGetMousePos(&mousex, &mousey);
			int diffLockPositionX = mousex - guiStates.lockPositionX;
			int diffLockPositionY = mousey - guiStates.lockPositionY;
			if (guiStates.zoomLock)
			{
				float zoomDir = 0.0;
				if (diffLockPositionX > 0)
					zoomDir = -1.f;
				else if (diffLockPositionX < 0 )
					zoomDir = 1.f;
				cammg.zoom(zoomDir * GUIStates::MOUSE_ZOOM_SPEED);
			}
			else if (guiStates.turnLock)
			{
				cammg.turn(diffLockPositionY * GUIStates::MOUSE_TURN_SPEED, diffLockPositionX * GUIStates::MOUSE_TURN_SPEED);

			}
			else if (guiStates.panLock)
			{
				cammg.pan(diffLockPositionX * GUIStates::MOUSE_PAN_SPEED, diffLockPositionY * GUIStates::MOUSE_PAN_SPEED);
			}
			guiStates.lockPositionX = mousex;
			guiStates.lockPositionY = mousey;
		}


		//
		// Capture Video and draw image
		//

		timer.breakpoint("Image capture   ");
		cap >> frame;
		cv::flip(frame, frame, 1);
		cv::cvtColor(frame, frame, CV_BGR2RGB);

		timer.breakpoint("Drawing scene   ");

		frameTexture.setSource(frame);

		glm::mat4 projection = glm::perspective(45.0f, widthf / heightf, 0.1f, 100.f);
		glm::mat4 worldToView = glm::lookAt(cammg.getEye(), cammg.getOrigin(), cammg.getUp());
		glm::mat4 objectToWorld;

		renderer.beginDraw();
		renderer.useShaderProgram(frameShader);
		frameShader.sendUniformMatrix4fv("Projection", projection);
		frameShader.sendUniformMatrix4fv("View", worldToView);
		frameShader.sendUniformMatrix4fv("Object", glm::scale(objectToWorld, glm::vec3(w, h, 1)));
		frameTexture.bind();
		rectangle.draw();
		frameTexture.unbind();


		//
		// Detect Pattern
		//

		timer.breakpoint("Pattern detection");

		renderer.useShaderProgram(patternShader);
		patternShader.sendUniformMatrix4fv("Projection", projection);
		patternShader.sendUniformMatrix4fv("View", worldToView);
		patternShader.sendUniformMatrix4fv("Object", glm::scale(objectToWorld, glm::vec3(w, h, 1)));

		vector<Pattern> detectedPattern;

		myDetector.detect(frame, cameraMatrix, distortions, pattern.getPatterns(), detectedPattern);

		// sort the detectedPattern to get the start and loop patterns first
		sort(detectedPattern.begin(), detectedPattern.end(), sortPattern);
		bool isCalibrated = false;
		bool allCornerDetected = true;
		Mat gridHomography, gridInvHomography;


		// For each detected pattern
		for (unsigned int i = 0; i < detectedPattern.size(); i++) {

			//
			// Corners detection
			//

			// Start Top Corner > Red
			if(i == 0 && detectedPattern.at(i).id == 1 ) {
				patternShader.sendUniformVec3f("color", glm::vec3(0.8, 0.0, 0.0));
				patternStartTop = detectedPattern.at(i).getPositions( frame, cameraMatrix, distortions);
			} else if( i == 0 ) {
				allCornerDetected = false;
			}

			// Start Bottom Corner -> Red
			if( i == 1 && detectedPattern.at(i).id == 2 ) {
				patternShader.sendUniformVec3f("color", glm::vec3(0.4, 1.0, 0.0));
				patternLoopTop = detectedPattern.at(i).getPositions( frame, cameraMatrix, distortions);
			} else if( i == 1 ){
				allCornerDetected = false;
			}

			// Loop Top Corner -> Blue
			if( i == 2 && detectedPattern.at(i).id == 3 ){
				patternShader.sendUniformVec3f("color", glm::vec3(0.0, 0.0, 1.0));
				patternStartBot = detectedPattern.at(i).getPositions( frame, cameraMatrix, distortions);
			} else if( i == 2 ){
				allCornerDetected = false;
			}

			if( i > 3)
			{
				patternShader.sendUniformVec3f("color", glm::vec3(1.0, 1.0, 0.0));
			}

			if( i == 3 && detectedPattern.at(i).id == 4 && allCornerDetected) {

				patternLoopBot = detectedPattern.at(i).getPositions( frame, cameraMatrix, distortions);
				isCalibrated = true;
				vector<Point2f> srcPoints, dstPoints;
				srcPoints.push_back(Point2f(0, 0));
				srcPoints.push_back(Point2f(1., 0));
				srcPoints.push_back(Point2f(1., 1.));
				srcPoints.push_back(Point2f(0, 1.));

				dstPoints.push_back(patternStartTop[1]);
				dstPoints.push_back(patternLoopTop[1]);
				dstPoints.push_back(patternLoopBot[0]);
				dstPoints.push_back(patternStartBot[0]);

				//				fprintf(stderr, "find homo\n");
				gridHomography = findHomography(srcPoints, dstPoints);
				gridInvHomography = gridHomography.inv();

				//				fprintf(stderr, "find homo ok\n");

				std::vector<Point2f> linePosition(4);
				linePosition[0] = Point2f(lastPosition, 0);
				linePosition[1] = Point2f(currentPosition, 0);
				linePosition[2] = Point2f( currentPosition, 1. );
				linePosition[3] = Point2f(lastPosition, 1.);
				std::vector<Point2f> realLine(4);

				perspectiveTransform(linePosition, realLine, gridHomography);

				glm::vec3 topleft, topright, botright, botleft;

				topleft.x = -2*realLine[0].x / widthf +1;
				topleft.y = -2*realLine[0].y / heightf +1;
				topleft.z = 0;
				topright.x = -2*realLine[1].x / widthf + 1;
				topright.y = -2*realLine[1].y / heightf +1;
				topright.z = 0;

				botleft.x = -2*realLine[3].x / widthf +1;
				botleft.y = -2*realLine[3].y / heightf +1;
				botleft.z = 0;
				botright.x = -2*realLine[2].x / widthf +1;
				botright.y = -2*realLine[2].y / heightf +1;
				botright.z = 0;

				//				topleft.x = -2*patternStartTop[1].x / widthf +1;
				//				topleft.y = -2*patternStartTop[1].y / heightf +1;
				//				topleft.z = 0;
				//				topright.x = -2*patternLoopTop[1].x / widthf + 1;
				//				topright.y = -2*patternLoopTop[1].y / heightf +1;
				//				topright.z = 0;
				//
				//				botleft.x = -2*patternStartBot[0].x / widthf +1;
				//				botleft.y = -2*patternStartBot[0].y / heightf +1;
				//				botleft.z = 0;
				//				botright.x = -2*patternLoopBot[0].x / widthf +1;
				//				botright.y = -2*patternLoopBot[0].y / heightf +1;
				//				botright.z = 0;


				patternShader.sendUniformVec3f("color", glm::vec3(1.0, 0.0, 1.0));

				rectangle.setCorners(topleft, topright, botright, botleft);
				rectangle.draw();
				rectangle.resetCorners();

				patternShader.sendUniformVec3f("color", glm::vec3(1.0, 0.5, 0.5));
			}

			patternPositions = detectedPattern.at(i).getPositions(frame, cameraMatrix, distortions);
			glm::vec3 topleft, topright, botright, botleft;
			// TODO : code a function to convert.
			topleft.x = -2*patternPositions.at(1).x / widthf + 1;
			topleft.y = -2*patternPositions.at(1).y / heightf + 1;
			topleft.z = 0;
			topright.x = -2*patternPositions.at(0).x / widthf + 1;
			topright.y = -2*patternPositions.at(0).y / heightf + 1;
			topright.z = 0;
			botright.x = -2*patternPositions.at(3).x / widthf + 1;
			botright.y = -2*patternPositions.at(3).y / heightf + 1;
			botright.z = 0;
			botleft.x = -2*patternPositions.at(2).x / widthf + 1;
			botleft.y = -2*patternPositions.at(2).y / heightf + 1;
			botleft.z = 0;
			//fprintf(stderr, "pattern detected : TL(%f, %f), TR(%f, %f), BR(%f, %f), BL(%f, %f)\n", topleft.x, topleft.y, topright.x, topright.y, botright.x, botright.y, botleft.x, botleft.y);

			//patternShader.sendUniformVec3f("color", glm::vec3(1.0, 1.0, 0.0));

			patternPositions = detectedPattern.at(i).getPositions( frame, cameraMatrix, distortions);
			//playing song :

			if ( patternSoundAssociation.find(detectedPattern.at(i).id) != patternSoundAssociation.end() && isCalibrated) {
				// On regarde si au moins un coin du carré est entre lastPosition et currentPosition et le reste des points à droite de currentPosition.
				// On fait l'homographie inverse pour recaler les points dans un repere simple.
				std::vector<Point2f> simplePositions(4);
				perspectiveTransform(patternPositions, simplePositions, gridInvHomography);
				int nbIn = 0, nbOut = 0;
				for(int j = 0; j < 4; ++j){
					if(currentPosition > lastPosition){
						if(simplePositions.at(j).y > 1.3 || simplePositions.at(j).y < -0.3 ){
							++nbOut;
						} else if(simplePositions.at(j).x > lastPosition && simplePositions.at(j).x < currentPosition){
							++nbIn;
						} else if(simplePositions.at(j).x < lastPosition){
							++nbOut;
						}
					} else {
						if(simplePositions.at(j).y > 1.3 || simplePositions.at(j).y < -0.3 ){
							++nbOut;
						} else if(simplePositions.at(j).x < currentPosition && simplePositions.at(j).x > -0.1 ){
							++nbIn;
						} else if(simplePositions.at(j).x > lastPosition){
							++nbOut;
						}
					}
				}
				//					std::cerr<<"Nb in : "<<nbIn<<"\t nbOut : "<<nbOut<<std::endl;
				if(nbOut == 0 && nbIn > 0){
					std::cerr<<"Play sound for pattern : "<<detectedPattern.at(i).id<<std::endl;
					patternShader.sendUniformVec3f("color", glm::vec3(0.5, 0.5, .5));
					soundPlayer.play(patternSoundAssociation[detectedPattern.at(i).id]);
				}
			}

			rectangle.setCorners(topleft, topright, botright, botleft);
			rectangle.draw();
			rectangle.resetCorners();


			//			}

		}

		//
		// Draw Cursor
		//


		renderer.endDraw();

		//
		// Draw UI
		//

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, width, height);

		unsigned char mbut = 0;
		int mscroll = 0;
		int mousex; int mousey;
		glfwGetMousePos(&mousex, &mousey);
		mousey = height - mousey;

		if(leftButton == GLFW_PRESS)
			mbut |= IMGUI_MBUT_LEFT;

		imguiBeginFrame(mousex, mousey, mbut, mscroll);
		int logScroll = 0;
		char lineBuffer[512];
		imguiBeginScrollArea("Animation", 0, 0, 200, 100, &logScroll);
		sprintf(lineBuffer, "FPS %f", timer.fps());
		imguiLabel(lineBuffer);

		imguiEndScrollArea();
		imguiEndFrame();
		imguiRenderGLDraw(width, height);
		glDisable(GL_BLEND);

		glfwSwapBuffers();

		lastPosition = currentPosition;

	}
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
			glfwGetWindowParam( GLFW_OPENED ) );

	cap.release();

	return 0;

}
