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

Mat laplacianfilter(const Mat input);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat input_gray;
	Mat output;

	cvtColor(input, input_gray, COLOR_RGB2GRAY);



	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);
	output = laplacianfilter(input_gray); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Laplacian Filter", WINDOW_AUTOSIZE);
	imshow("Laplacian Filter", output);


	waitKey(0);

	return 0;
}


Mat laplacianfilter(const Mat input) {

	int row = input.rows;
	int col = input.cols;
	int tempa;
	int tempb;
	int n = 1; // Laplacian Filter Kernel N

	// Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
	//Fill code to initialize Laplacian filter kernel matrix
	Mat kernel = Mat::zeros(2 * n + 1, 2 * n + 1, CV_32F);

	kernel.at<float>(0, 1) = 1;
	kernel.at<float>(1, 0) = 1;
	kernel.at<float>(1, 1) = -4;
	kernel.at<float>(1, 2) = 1;
	kernel.at<float>(2, 1) = 1;

	// cout << kernel << endl;

	Mat output = Mat::zeros(row, col, input.type());


	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			float sum = 0.0;
			for (int a = -n; a <= n; a++) {
				for (int b = -n; b <= n; b++) {
					// Use mirroring boundary process

					if (i + a > row - 1) {  // mirroring for the border pixels
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
					sum += kernel.at<float>(n + a, n + b) * input.at<G>(tempa, tempb);

				}

			}
			output.at<G>(i, j) = abs(sum) * 5;

		}
	}
	return output;
}