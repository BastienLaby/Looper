#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cameraparams.h"
#include "opencv/patterndetector.h"

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

    VideoCapture cap(0);
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


    // associate and load patterns with sounds
	patternSoundAssociation[pattern.loadPattern("media/img/Pattern_Loop.png")] = soundPlayer.loadSound((MUSIC_PATH+"36 - Nami_Login_Music_v1.mp3").c_str());
	patternSoundAssociation[pattern.loadPattern("media/img/Pattern_Start.png")] =  soundPlayer.loadSound((MUSIC_PATH+"03 Thrift Shop (feat. Wanz).mp3").c_str());
	patternSoundAssociation[pattern.loadPattern("media/img/Pattern_C.png")] = soundPlayer.loadSound((MUSIC_PATH+"Get Jinxed.mp3").c_str());
	patternSoundAssociation[pattern.loadPattern("media/img/Pattern_D.png")] = soundPlayer.loadSound((MUSIC_PATH+"Vi_Music_Master_v16.mp3").c_str());

	typedef map<size_t, size_t>::const_iterator MapIterator;
	for (MapIterator iter = patternSoundAssociation.begin(); iter != patternSoundAssociation.end(); iter++)
	{
	    cout << "Key: " << iter->first <<"\tValue: " << iter->second << endl;
	}

    std::cerr<<"pattern library size : "<<pattern.getPatterns().size()<<std::endl;

    double fixed_thresh = 40;
	double adapt_thresh = 5;//non-used with FIXED_THRESHOLD mode
	int adapt_block_size = 45;//non-used with FIXED_THRESHOLD mode
	double confidenceThreshold = 0.5;
	int mode = 2;//1:FIXED_THRESHOLD, 2: ADAPTIVE_THRESHOLD

	PatternDetector myDetector( fixed_thresh, adapt_thresh, adapt_block_size, confidenceThreshold, Pattern::patternSize, mode);
	std::vector<cv::Point2f> patternPositions, patternStart, patternLoopTop, patternLoopBot;

    fouch::Timer timer;

	do
	{

        //
        // Handle user inputs
        //

        long loopStart = glfwGetTime(); // Last time when the loop started
        long loopTime = 3000; // Time in ms before looping

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

        float currentPosition = static_cast<float>(loopStart - glfwGetTime()) / static_cast<float>(loopTime);

        std::vector<Pattern> detectedPattern;
        myDetector.detect(frame, cameraMatrix, distortions, pattern.getPatterns(), detectedPattern);

        // sort the detectedPattern to get the start and loop patterns first
        std::sort(detectedPattern.begin(), detectedPattern.end(), sortPattern);
        bool isCalibrated = false;

        for (unsigned int i = 0; i < detectedPattern.size(); i++)
        {

            // TODO calibrer la grille
            if(i == 0 && detectedPattern.at(i).id == 1)
            {
                // S, coin haut gauche
                patternStart = detectedPattern.at(i).getPositions(frame, cameraMatrix, distortions);
            }

            if(i == 1 && detectedPattern.at(i).id == 2)
            {
                // Loop, coin haut droit par défaut
                patternLoopTop = detectedPattern.at(i).getPositions(frame, cameraMatrix, distortions);
            }

            if(i == 2 && detectedPattern.at(i).id == 2)
            {
                // Loop, coin haut droit ou gauche, a déterminer en fonction de la position et des infos de S et de Loop
                patternLoopBot = detectedPattern.at(i).getPositions(frame, cameraMatrix, distortions);

                isCalibrated = true;
                // TODO calculer l'homographie de la grille et tracer la ligne
            }

            if(isCalibrated)
            {
                
                // TODO regarder autour de currentPosition les patterns détectés

                //detectedPattern.at(i).showPattern();

                patternPositions = detectedPattern.at(i).getPositions(frame, cameraMatrix, distortions);

                glm::vec3 topleft, topright, botright, botleft;
            
                topleft.x = - (patternPositions.at(1).x / widthf)*2 + 1;
                topleft.y = - (patternPositions.at(1).y / heightf)*2 + 1;
                topleft.z = 0;

                topright.x = - (patternPositions.at(0).x / widthf)*2 + 1;
                topright.y = - (patternPositions.at(0).y / heightf)*2 + 1;
                topright.z = 0;

                botright.x = - (patternPositions.at(3).x / widthf)*2 + 1;
                botright.y = - (patternPositions.at(3).y / heightf)*2 + 1;
                botright.z = 0;

                botleft.x = - (patternPositions.at(2).x / widthf)*2 + 1;
                botleft.y = - (patternPositions.at(2).y / heightf)*2 + 1;
                botleft.z = 0;

                fprintf(stderr, "pattern detected : TL(%f, %f), TR(%f, %f), BR(%f, %f), BL(%f, %f)\n", topleft.x, topleft.y, topright.x, topright.y, botright.x, botright.y, botleft.x, botleft.y);

                rectangle.setCorners(topleft, topright, botright, botleft);

                rectangle.draw();

                rectangle.resetCorners();

                // Play sound if pattern is detected

                if ( patternSoundAssociation.find(detectedPattern.at(i).id) != patternSoundAssociation.end() )
                {
                    std::cerr<<"Detected pattern "<<detectedPattern.at(i).id<<", playing sound "<< patternSoundAssociation[detectedPattern.at(i).id] <<" : "<<std::endl;
                    //soundPlayer.play(patternSoundAssociation[detectedPattern.at(i).id]);
                }
            }
        }

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

	}
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
           glfwGetWindowParam( GLFW_OPENED ) );

    cap.release();

	return 0;

}
