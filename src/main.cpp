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

int main(int argc, char** argv) {

    //
	// Initialise GLFW
    //

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }
    int width = 800;
    int height = 450;
    float widthf = (float) width;
    float heightf = (float) height;

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

    VideoCapture cap(0);
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    Mat frame;
    cap >> frame;

    looper::Texture frameTexture;
    frameTexture.setSource(frame);

    //
    // Load Shader
    //

    looper::Shader shader;
    shader.load("shader.glsl");
    
    renderer.useShaderProgram(shader);
    
    shader.createShaderLocation("Projection");
    shader.createShaderLocation("View");
    shader.createShaderLocation("DiffuseTexture");
  
    shader.sendUniformInteger("DiffuseTexture", 0);
    

  	//
 	// Create quad model
    //

    // Load geometry
    float w = widthf/100;
    float h = heightf/100;
    int plane_triangleCount = 2;
    int plane_triangleList[] = {0, 1, 2, 0, 2, 3}; 
    float plane_uvs[] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f};
    float plane_vertices[] = {-w, h, 0.0, w, h, 0.0, w, -h, 0.0, -w, -h, 0.0};
    float plane_normals[] = {0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 0};

    //
    // Create and bind VAO and VBO
    //

    GLuint vbos[4];
    glGenBuffers(4, vbos);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_triangleList), plane_triangleList, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_normals), plane_normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_uvs), plane_uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);

    // Unbind everything. Potentially illegal on some implementations
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	double confidenceThreshold = 0.35;
	int mode = 2;//1:FIXED_THRESHOLD, 2: ADAPTIVE_THRESHOLD

	PatternDetector myDetector( fixed_thresh, adapt_thresh, adapt_block_size, confidenceThreshold, Pattern::patternSize, mode);
	std::vector<cv::Point2f> patternPositions;

    fouch::Timer timer;

	do
	{

        timer.breakpoint("Image capture   ");
        cap >> frame;

        timer.breakpoint("Pattern detection");


    	std::vector<Pattern> detectedPattern;
    	myDetector.detect(frame, cameraMatrix, distortions, pattern.getPatterns(), detectedPattern);

        //augment the input frame (and print out the properties of pattern if you want)
        for (unsigned int i =0; i<detectedPattern.size(); i++){
//    		detectedPattern.at(i).showPattern();
    		patternPositions = detectedPattern.at(i).getPositions( frame, cameraMatrix, distortions);
    		std::cerr<<"Detected pattern "<<detectedPattern.at(i).id<<", playing sound "<< patternSoundAssociation[detectedPattern.at(i).id] <<" : "<<std::endl;
    		//playing song :
    		if ( patternSoundAssociation.find(detectedPattern.at(i).id) != patternSoundAssociation.end() ) {
    			soundPlayer.play(patternSoundAssociation[detectedPattern.at(i).id]);
    		}
//    		for(int j = 0; j < 4; ++j){
//    			std::cerr<<" point "<<j<<" ("<<patternPositions.at(j).x<<", "<<patternPositions.at(j).y<<")"<<std::endl;
//    		}


        }

        timer.breakpoint("Drawing scene   ");

        frameTexture.setSource(frame);

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

        
        glm::mat4 projection = glm::perspective(45.0f, widthf / heightf, 0.1f, 100.f); 
        glm::mat4 worldToView = glm::lookAt(cammg.getEye(), cammg.getOrigin(), cammg.getUp());

        renderer.beginDraw();

        renderer.useShaderProgram(shader);
        shader.sendUniformMatrix4fv("Projection", projection);
        shader.sendUniformMatrix4fv("View", worldToView);

        frameTexture.bind();

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, plane_triangleCount*3, GL_UNSIGNED_INT, 0);

        frameTexture.unbind();

        renderer.endDraw();

        // Draw UI
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
