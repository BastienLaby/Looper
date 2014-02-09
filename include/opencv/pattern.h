#ifndef _ARMA_PATTERN_
#define _ARMA_PATTERN_
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
using namespace cv;

namespace ARma {

struct PatternToken {
	unsigned int id;
	std::pair<float, float> positions[4];
};

class Pattern
{
protected :
	static int patternCount;
	static int patternSize;

	std::vector<cv::Mat> patternLibrary;

public:
	vector<Point2f> vertices;
	int id;
	int orientation;//{0,1,2,3}
	float size; //in milimeters
	double confidence;//min: -1, max: 1
	Mat rotVec, transVec, rotMat;

	Pattern(double param1=80);

	~Pattern(){};

	std::vector<PatternToken> detect(Mat& frame, const Mat& camMatrix, const Mat& distMatrix);

	//solves the exterior orientation problem between patten and camera
	void getExtrinsics(int patternSize, const Mat& cameraMatrix, const Mat& distortions);

	//augments image with 3D cubes. It;s too simple augmentation jsut for checking
	void draw(Mat& frame, const Mat& camMatrix, const Mat& distMatrix);

	//computes the rotation matrix from the rotation vector using Rodrigues
	void rotationMatrix(const Mat& rotation_vector, Mat& rotation_matrix);

	//prints the properties of the pattern and its transformation matrix
	void showPattern(void);

	int loadPattern(const char* filename);
	std::vector<cv::Mat>& getPatterns();

};


}

#endif