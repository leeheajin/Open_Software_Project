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

Mat unsharpmask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k);
Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);
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
	output = unsharpmask(input_gray, 3, 3, 3, "zero-paddle", 0.5);  // Boundary process: zero-paddle, mirroring, adjustkernel & K < 1

	namedWindow("Unsharp Mask", WINDOW_AUTOSIZE);
	imshow("Unsharp Mask", output);


	waitKey(0);

	return 0;
}


Mat unsharpmask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k) {

	int row = input.rows;
	int col = input.cols;

	Mat temp;
	Mat lowpass;
	Mat output = Mat::zeros(row, col, input.type());

	lowpass = gaussianfilter(input, n, sigmaT, sigmaS, opt);

	lowpass *= k;

	temp = (input - lowpass) / (1 - k);

	output = laplacianfilter(temp);

	return output;

}


Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	float denom;
	float kernelvalue;

	// Initialiazing Kernel Matrix 
	kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);

	denom = 0.0;
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
			kernel.at<float>(a + n, b + n) = value1; // Kernel boundary
			denom += value1; // Save for later division
		}
	}

	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			kernel.at<float>(a + n, b + n) /= denom; // Low-pass filter
		}
	}

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {


			if (!strcmp(opt, "zero-paddle")) {
				float sum1 = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {

						/* Gaussian filter with Zero-paddle boundary process:

						Fill the code:
						*/

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { // if the pixel is not a border pixel
							kernelvalue = kernel.at<float>(n + a, n + b);
							sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
						}

					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1 = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {

						/* Gaussian filter with "mirroring" process:

						Fill the code:
						*/

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
						kernelvalue = kernel.at<float>(n + a, n + b);
						sum1 += kernelvalue * (float)(input.at<G>(tempa, tempb));
					}
				}
				output.at<G>(i, j) = (G)sum1;

			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1 = 0.0;
				float sum2 = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {

						/* Gaussian filter with "adjustkernel" process:

						Fill the code:
						*/

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							kernelvalue = kernel.at<float>(n + a, n + b);
							sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
							sum2 += kernelvalue;

						}
						output.at<G>(i, j) = (G)(sum1 / sum2);
					}
				}
			}
		}
	}
	return output;
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