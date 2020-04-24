#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#define IM_TYPE	CV_8UC3

using namespace cv;
using namespace std;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat sobelfilter(const Mat input);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat output;

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);
	output = sobelfilter(input); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Sobel Filter", WINDOW_AUTOSIZE);
	imshow("Sobel Filter", output);


	waitKey(0);

	return 0;
}


Mat sobelfilter(const Mat input) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int tempa;
	int tempb;
	int n = 1; // Sobel Filter Kernel N

	// Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
	//Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)
	Mat kernel_Sx = Mat::zeros(2 * n + 1, 2 * n + 1, CV_32F);
	Mat kernel_Sy = Mat::zeros(2 * n + 1, 2 * n + 1, CV_32F);

	kernel_Sx.at<float>(0, 0) = -1;
	kernel_Sx.at<float>(0, 2) = 1;
	kernel_Sx.at<float>(1, 0) = -2;
	kernel_Sx.at<float>(1, 2) = 2;
	kernel_Sx.at<float>(2, 0) = -1;
	kernel_Sx.at<float>(2, 2) = 1;

	kernel_Sy.at<float>(0, 0) = -1;
	kernel_Sy.at<float>(0, 1) = -2;
	kernel_Sy.at<float>(0, 2) = -1;
	kernel_Sy.at<float>(2, 0) = 1;
	kernel_Sy.at<float>(2, 1) = 2;
	kernel_Sy.at<float>(2, 2) = 1;

	// cout << kernel_Sx << endl;
	// cout << kernel_Sy << endl;

	Mat output = Mat::zeros(row, col, input.type());


	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			float sum_r_x = 0.0;
			float sum_g_x = 0.0;
			float sum_b_x = 0.0;
			float sum_r_y = 0.0;
			float sum_g_y = 0.0;
			float sum_b_y = 0.0;
			for (int a = -n; a <= n; a++) {
				for (int b = -n; b <= n; b++) {
					// Use mirroring boundary process
					// Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy ) 
					
					if (i + a > row - 1) {  //mirroring for the border pixels
						tempa = i - a + 1;
					}
					else if (i + a < 0) {
						tempa = -(i + a - 1);
					}
					else {
						tempa = i + a;
					}
					if (j + b > col - 1) {
						tempb = j - b + 1;
					}
					else if (j + b < 0) {
						tempb = -(j + b - 1);
					}
					else {
						tempb = j + b;
					}
					sum_b_x += kernel_Sx.at<float>(n + a, n + b) * input.at<C>(tempa, tempb)[0];
					sum_g_x += kernel_Sx.at<float>(n + a, n + b) * input.at<C>(tempa, tempb)[1];
					sum_r_x += kernel_Sx.at<float>(n + a, n + b) * input.at<C>(tempa, tempb)[2];

					sum_b_y += kernel_Sx.at<float>(n + a, n + b) * input.at<C>(tempa, tempb)[0];
					sum_g_y += kernel_Sx.at<float>(n + a, n + b) * input.at<C>(tempa, tempb)[1];
					sum_r_y += kernel_Sx.at<float>(n + a, n + b) * input.at<C>(tempa, tempb)[2];

				}
			  
			}
			output.at<C>(i, j)[0] = sqrt(pow(sum_b_x, 2) + pow(sum_b_y, 2));
			output.at<C>(i, j)[1] = sqrt(pow(sum_g_x, 2) + pow(sum_g_y, 2));
			output.at<C>(i, j)[2] = sqrt(pow(sum_r_x, 2) + pow(sum_r_y, 2));

		}
	}
	return output;
}