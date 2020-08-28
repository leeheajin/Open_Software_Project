#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/nonfree/nonfree.hpp>
#include <vector>
#include <cmath>
#include <stdlib.h>
#define RATIO_THR 0.4

using namespace std;
using namespace cv;

template <typename T>

Mat cal_affine(int ptl_x[], int ptl_y[], int ptr_x[], int ptr_y[], int number_of_points);
void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int diff_x, int diff_y, float alpha);
double euclidDistance(Mat& vec1, Mat& vec2);
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors);
int nearestSecondNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, int first);
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold);

int main() {
	Mat I1, I2;

	// Read each image
	I1 = imread("input1.jpg", CV_LOAD_IMAGE_COLOR);
	I2 = imread("input2.jpg", CV_LOAD_IMAGE_COLOR);
	Mat I1_gray, I2_gray;

	if (!I1.data || !I2.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	cvtColor(I1, I1_gray, CV_RGB2GRAY);
	cvtColor(I2, I2_gray, CV_RGB2GRAY);

	I1.convertTo(I1, CV_32FC3, 1.0 / 255);
	I2.convertTo(I2, CV_32FC3, 1.0 / 255);

	FeatureDetector* detector = new SiftFeatureDetector(
		0,		// nFeatures
		4,		// nOctaveLayers
		0.04,	// contrastThreshold
		10,		// edgeThreshold
		1.6		// sigma
	);

	DescriptorExtractor* extractor = new SiftDescriptorExtractor();

	// Compute keypoints and descriptor from the source image in advance
	vector<KeyPoint> keypoints1;
	Mat descriptors1;
	vector<KeyPoint> keypoints2;
	Mat descriptors2;

	// Detect keypoints
	detector->detect(I1_gray, keypoints1);
	extractor->compute(I1_gray, keypoints1, descriptors1);

	detector->detect(I2_gray, keypoints2);
	extractor->compute(I2_gray, keypoints2, descriptors2);

	vector<Point2f> srcPoints1;
	vector<Point2f> dstPoints1;
	vector<Point2f> srcPoints2;
	vector<Point2f> dstPoints2;

	// Refine the feature matching
	bool crossCheck = true;
	bool ratio_threshold = true;
	findPairs(keypoints1, descriptors1, keypoints2, descriptors2, srcPoints1, dstPoints1, crossCheck, ratio_threshold); // I1->I2
	findPairs(keypoints2, descriptors2, keypoints1, descriptors1, srcPoints2, dstPoints2, crossCheck, ratio_threshold); // I2->I1

	int pointpair = srcPoints1.size() + srcPoints2.size();
	// int pointpair = srcPoints1.size();

	int *ptl_x = (int*)malloc(sizeof(int)*pointpair);
	int *ptl_y = (int*)malloc(sizeof(int)*pointpair);
	int *ptr_x = (int*)malloc(sizeof(int)*pointpair);
	int *ptr_y = (int*)malloc(sizeof(int)*pointpair);

	for (int i = 0; i < (int)srcPoints1.size(); i++) {
		ptl_x[i] = round(srcPoints1[i].x);
		ptl_y[i] = round(srcPoints1[i].y);
		ptr_x[i] = round(dstPoints1[i].x);
		ptr_y[i] = round(dstPoints1[i].y);
	}

	for (int i = 0; i < (int)srcPoints2.size(); i++) {
		ptl_x[(int)srcPoints1.size() + i] = round(dstPoints2[i].x);
		ptl_y[(int)srcPoints1.size() + i] = round(dstPoints2[i].y);
		ptr_x[(int)srcPoints1.size() + i] = round(srcPoints2[i].x);
		ptr_y[(int)srcPoints1.size() + i] = round(srcPoints2[i].y);
		/* printf("ptl_x[%d]=%d, ptl_y[%d]=%d\n", (int)srcPoints1.size() + i, ptl_x[(int)srcPoints1.size() + i], (int)srcPoints1.size() + i, ptl_y[(int)srcPoints1.size() + i]);
		printf("dst_x[%d]=%f, dst_y[%d]=%f\n", (int)srcPoints1.size() + i, round(dstPoints2[i].x), (int)srcPoints1.size() + i, round(dstPoints2[i].y));
		printf("ptr_x[%d]=%d, ptr_y[%d]=%d\n", (int)srcPoints1.size() + i, ptr_x[(int)srcPoints1.size() + i], (int)srcPoints1.size() + i, ptr_y[(int)srcPoints1.size() + i]);
		printf("src_x[%d]=%f, src_y[%d]=%f\n", (int)srcPoints1.size() + i, round(srcPoints2[i].x), (int)srcPoints1.size() + i, round(srcPoints2[i].y)); */
	}

	// corresponding pixels
	/* int ptl_x[28] = { 528,597,581,520,526,581,587,496,506,500,342,558,499,642,474,456,451,475,530,381,472,475,426,539,329,341,492,511 };
	int ptl_y[28] = { 509,558,605,649,680,689,705,730,734,768,795,802,818,837,877,889,894,902,917,924,930,948,964,969,980,988,994,998 };
	int ptr_x[28] = { 488,561,544,482,490,546,552,462,471,467,313,526,468,607,445,429,424,447,500,358,446,449,403,510,312,324,466,484 };
	int ptr_y[28] = { 45,89,142,194,226,230,246,279,281,314,352,345,365,372,421,434,439,446,456,472,471,488,506,503,527,532,528,531 }; */

	// height(row), width(col) of each image
	const float I1_row = I1.rows;
	const float I1_col = I1.cols;
	const float I2_row = I2.rows;
	const float I2_col = I2.cols;

	// calculate affine Matrix A12, A21
	Mat A12 = cal_affine<float>(ptl_x, ptl_y, ptr_x, ptr_y, pointpair);
	Mat A21 = cal_affine<float>(ptr_x, ptr_y, ptl_x, ptl_y, pointpair);

	// compute corners (p1, p2, p3, p4)
	Point2f p1(A21.at<float>(0) * 0 + A21.at<float>(1) * 0 + A21.at<float>(2),
		A21.at<float>(3) * 0 + A21.at<float>(4) * 0 + A21.at<float>(5));

	Point2f p2(A21.at<float>(0) * 0 + A21.at<float>(1) * I2_col + A21.at<float>(2),
		A21.at<float>(3) * 0 + A21.at<float>(4) * I2_col + A21.at<float>(5));

	Point2f p3(A21.at<float>(0) * I2_row + A21.at<float>(1) * 0 + A21.at<float>(2),
		A21.at<float>(3) * I2_row + A21.at<float>(4) * 0 + A21.at<float>(5));

	Point2f p4(A21.at<float>(0) * I2_row + A21.at<float>(1) * I2_col + A21.at<float>(2),
		A21.at<float>(3) * I2_row + A21.at<float>(4) * I2_col + A21.at<float>(5));

	// for inverse warping
	Point2f p1_(A12.at<float>(0) * 0 + A12.at<float>(1) * 0 + A12.at<float>(2),
		A12.at<float>(3) * 0 + A12.at<float>(4) * 0 + A12.at<float>(5));

	Point2f p2_(A12.at<float>(0) * 0 + A12.at<float>(1) * I1_col + A12.at<float>(2),
		A12.at<float>(3) * 0 + A12.at<float>(4) * I1_col + A12.at<float>(5));

	Point2f p3_(A12.at<float>(0) * I1_row + A12.at<float>(1) * 0 + A12.at<float>(2),
		A12.at<float>(3) * I1_row + A12.at<float>(4) * 0 + A12.at<float>(5));

	Point2f p4_(A12.at<float>(0) * I1_row + A12.at<float>(1) * I1_col + A12.at<float>(2),
		A12.at<float>(3) * I1_row + A12.at<float>(4) * I1_col + A12.at<float>(5));

	// compute boundary for merged image(I_f)
	int bound_u = (int)round(min(0.0f, min(p1.x, p2.x)));
	int bound_b = (int)round(std::max(I1_row, std::max(p3.x, p4.x)));
	int bound_l = (int)round(min(0.0f, min(p1.y, p3.y)));
	int bound_r = (int)round(std::max(I1_col, std::max(p2.y, p4.y)));

	// compute boundary for inverse warping
	int bound_u_ = (int)round(min(0.0f, min(p1_.x, p2_.x)));
	int bound_b_ = (int)round(std::max(I2_row, std::max(p3_.x, p4_.x)));
	int bound_l_ = (int)round(min(0.0f, min(p1_.y, p3_.y)));
	int bound_r_ = (int)round(std::max(I2_col, std::max(p2_.y, p4_.y)));

	int diff_x = abs(bound_u);
	int diff_y = abs(bound_l);

	int diff_x_ = abs(bound_u_);
	int diff_y_ = abs(bound_l_);

	// initialize merged image
	Mat I_f(bound_b - bound_u + 1, bound_r - bound_l + 1, CV_32FC3, Scalar(0));

	// inverse warping with bilinear interplolation
	for (int i = -diff_x_; i < I_f.rows - diff_x_; i++) {
		for (int j = -diff_y_; j < I_f.cols - diff_y_; j++) {
			float x = A12.at<float>(0) * i + A12.at<float>(1) * j + A12.at<float>(2) + diff_x_;
			float y = A12.at<float>(3) * i + A12.at<float>(4) * j + A12.at<float>(5) + diff_y_;

			float y1 = floor(y);
			float y2 = ceil(y);
			float x1 = floor(x);
			float x2 = ceil(x);

			float mu = y - y1;
			float lambda = x - x1;

			if (x1 >= 0 && x2 < I2_row && y1 >= 0 && y2 < I2_col) {
				I_f.at<Vec3f>(i + diff_x_, j + diff_y_) = lambda * (mu * I2.at<Vec3f>(x2, y2) + (1 - mu) * I2.at<Vec3f>(x2, y1)) + (1 - lambda) * (mu * I2.at<Vec3f>(x1, y2) + (1 - mu) * I2.at<Vec3f>(x1, y1));
			}
		}
	}

	// image stitching with blend
	blend_stitching(I1, I2, I_f, diff_x, diff_y, 0.5);

	namedWindow("Left Image");
	imshow("Left Image", I1);

	namedWindow("Right Image");
	imshow("Right Image", I2);

	namedWindow("result");
	imshow("result", I_f);

	waitKey(0);

	return 0;
}

template <typename T>
Mat cal_affine(int ptl_x[], int ptl_y[], int ptr_x[], int ptr_y[], int number_of_points) {

	Mat M(2 * number_of_points, 6, CV_32F, Scalar(0));
	Mat b(2 * number_of_points, 1, CV_32F);

	Mat M_trans, temp, affineM;

	// initialize matrix
	for (int i = 0; i < number_of_points; i++) {
		M.at<float>(i * 2, 0) = ptl_x[i];
		M.at<float>(i * 2, 1) = ptl_y[i];
		M.at<float>(i * 2, 2) = 1;
		M.at<float>(i * 2 + 1, 3) = ptl_x[i];
		M.at<float>(i * 2 + 1, 4) = ptl_y[i];
		M.at<float>(i * 2 + 1, 5) = 1;

		b.at<float>(i * 2) = ptr_x[i];
		b.at<float>(i * 2 + 1) = ptr_y[i];

	}

	temp = (M.t() * M).inv() * M.t() * b;

	affineM = temp;

	return affineM;
}

void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int diff_x, int diff_y, float alpha) {

	int bound_x = I1.rows + diff_x;
	int bound_y = I1.cols + diff_y;

	int col = I_f.cols;
	int row = I_f.rows;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			// for check validation of I1 & I2
			bool cond1 = (i < bound_x && i > diff_x) && (j < bound_y && j > diff_y) ? true : false;
			bool cond2 = I_f.at<Vec3f>(i, j) != Vec3f(0, 0, 0) ? true : false;

			// I2 is already in I_f by inverse warping
			// So, It is not necessary to check that only I2 is valid
			// if both are valid
			if (cond1 && cond2) {
				I_f.at<Vec3f>(i, j) = alpha * I1.at<Vec3f>(i - diff_x, j - diff_y) + (1 - alpha) * I_f.at<Vec3f>(i, j);
			}
			// only I1 is valid
			else if (cond1) {
				I_f.at<Vec3f>(i, j) = I1.at<Vec3f>(i - diff_x, j - diff_y);
			}
		}
	}
}

/**
* Calculate euclid distance
*/
double euclidDistance(Mat& vec1, Mat& vec2) {
	double sum = 0.0;
	int dim = vec1.cols;
	for (int i = 0; i < 1; i++)
		for (int j = 0; j< dim; j++)
		{
			uchar * a = vec1.ptr(i);
			uchar * b = vec2.ptr(i);
			sum += (a[j] - b[j]) *(a[j] - b[j]);
		}

	return sqrt(sum);
}

/**
* Find the index of nearest neighbor point from keypoints.
*/
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors.rows; i++) {
		Mat v = descriptors.row(i);		// each row of descriptor
										//	Fill the code
		KeyPoint pt = keypoints[i];
		double d = euclidDistance(vec, v);
		if (d < minDist) {
			minDist = d;
			neighbor = i;
		}
	}

	return neighbor;
}

/**
* Find the index of nearest neighbor point from keypoints.
*/
int nearestSecondNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, int first) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors.rows; i++) {
		if (i != first) { // No check nearest neighbor
			Mat v = descriptors.row(i);		// each row of descriptor
											//	Fill the code
			KeyPoint pt = keypoints[i];
			double d = euclidDistance(vec, v);
			if (d < minDist) {
				minDist = d;
				neighbor = i;
			}
		}
	}

	return neighbor;
}

/**
* Find pairs of points with the smallest distace between them
*/
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold) {
	for (int i = 0; i < descriptors1.rows; i++) {
		KeyPoint pt1 = keypoints1[i];
		Mat desc1 = descriptors1.row(i);

		int nn = nearestNeighbor(desc1, keypoints2, descriptors2);

		// Refine matching points using ratio_based thresholding
		if (ratio_threshold) {
			//	Fill the code
			int nn2 = nearestSecondNeighbor(desc1, keypoints2, descriptors2, nn);
			double d1 = euclidDistance(desc1, descriptors2.row(nn));
			double d2 = euclidDistance(desc1, descriptors2.row(nn2));
			if ((d1 / d2) >= RATIO_THR) { // ratio_based thresholding
				continue; // No use this points
			}
		}

		// Refine matching points using cross-checking
		if (crossCheck) {
			//	Fill the code
			KeyPoint pt = keypoints2[nn];
			Mat desc = descriptors2.row(nn);
			int n = nearestNeighbor(desc, keypoints1, descriptors1);
			if (i != n) { // cross-checking
				continue; // Not use this points
			}
		}
		if (nn >= 0) {
			KeyPoint pt2 = keypoints2[nn];
			srcPoints.push_back(pt1.pt);
			dstPoints.push_back(pt2.pt);
		}
	}
}