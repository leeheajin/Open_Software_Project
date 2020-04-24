#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#include <time.h>		/* clock */
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

Mat gaussianfilter_sep(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat output;

	clock_t start, end;
	float res;

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	start = clock(); // time check
	output = gaussianfilter_sep(input, 3, 3, 3, "zero-paddle"); //Boundary process: zero-paddle, mirroring, adjustkernel
	end = clock();

	res = (float)(end - start) / CLOCKS_PER_SEC;
	printf("time = %f", res);

	namedWindow("Gaussian Filter", WINDOW_AUTOSIZE);
	imshow("Gaussian Filter", output);


	waitKey(0);

	return 0;
}


Mat gaussianfilter_sep(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

	Mat kernel_x;
	Mat kernel_y;


	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	float denom1;
	float denom2;
	float kernelvalue;

	// Initialiazing Kernel Matrix 
	kernel_x = Mat::zeros(1, kernel_size, CV_32F);  // x direction kernel
	kernel_y = Mat::zeros(kernel_size, 1, CV_32F);  // y direction kernel


	denom1 = 0.0;
	denom2 = 0.0;
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))));
		kernel_x.at<float>(a + n) = value1; // Kernel boundary
		denom1 += value1; // Save for later division
	}

	for (int b = -n; b <= n; b++) {
		float value2 = exp(-(pow(b, 2) / (2 * pow(sigmaT, 2))));
		kernel_y.at<float>(b + n) = value2;
		denom2 += value2;
	}


	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		kernel_x.at<float>(a + n) /= denom1; // Low-pass filter
	}

	for (int b = -n; b <= n; b++) {
		kernel_y.at<float>(b + n) /= denom2;
	}

	Mat output = Mat::zeros(row, col, input.type());
	Mat temp = Mat::zeros(row, col, input.type());

	// use x direction kernel
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-paddle")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) {
					if ((j + a <= col - 1) && (j + a >= 0)) { // if the pixel is not a border pixel
						kernelvalue = kernel_x.at<float>(n + a);
						sum1_b += kernelvalue * (float)(input.at<C>(i, j + a)[0]);
						sum1_g += kernelvalue * (float)(input.at<C>(i, j + a)[1]);
						sum1_r += kernelvalue * (float)(input.at<C>(i, j + a)[2]);
					}
				}
				temp.at<C>(i, j)[0] = (G)sum1_b;
				temp.at<C>(i, j)[1] = (G)sum1_g;
				temp.at<C>(i, j)[2] = (G)sum1_r;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) {
					if (j + a > col - 1) {  //mirroring for the border pixels
						tempa = j - a + 1;
					}
					else if (j + a < 0) {
						tempa = -(j + a - 1);
					}
					else {
						tempa = j + a;
					}
					kernelvalue = kernel_x.at<float>(n + a);
					sum1_b += kernelvalue * (float)(input.at<C>(i, tempa)[0]);
					sum1_g += kernelvalue * (float)(input.at<C>(i, tempa)[1]);
					sum1_r += kernelvalue * (float)(input.at<C>(i, tempa)[2]);
				}
				temp.at<C>(i, j)[0] = (G)sum1_b;
				temp.at<C>(i, j)[1] = (G)sum1_g;
				temp.at<C>(i, j)[2] = (G)sum1_r;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				float sum2 = 0.0;
				for (int a = -n; a <= n; a++) {
					if ((j + a <= col - 1) && (j + a >= 0)) {
						kernelvalue = kernel_x.at<float>(n + a);
						sum1_b += kernelvalue * (float)(input.at<C>(i, j + a)[0]);
						sum1_g += kernelvalue * (float)(input.at<C>(i, j + a)[1]);
						sum1_r += kernelvalue * (float)(input.at<C>(i, j + a)[2]);
						sum2 += kernelvalue;
					}
					temp.at<C>(i, j)[0] = (G)(sum1_b / sum2);
					temp.at<C>(i, j)[1] = (G)(sum1_g / sum2);
					temp.at<C>(i, j)[2] = (G)(sum1_r / sum2);
				}
			}

		}
	}

	// user y direction kernel
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-paddle")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) {
					if ((i + a <= row - 1) && (i + a >= 0)) { // if the pixel is not a border pixel
						kernelvalue = kernel_y.at<float>(n + a);
						sum1_b += kernelvalue * (float)(temp.at<C>(i + a, j)[0]);
						sum1_g += kernelvalue * (float)(temp.at<C>(i + a, j)[1]);
						sum1_r += kernelvalue * (float)(temp.at<C>(i + a, j)[2]);
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_b;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_r;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) {
					if (i + a > row - 1) {  //mirroring for the border pixels
						tempa = i - a + 1;
					}
					else if (i + a < 0) {
						tempa = -(i + a - 1);
					}
					else {
						tempa = i + a;
					}
					kernelvalue = kernel_x.at<float>(n + a);
					sum1_b += kernelvalue * (float)(temp.at<C>(tempa, j)[0]);
					sum1_g += kernelvalue * (float)(temp.at<C>(tempa, j)[1]);
					sum1_r += kernelvalue * (float)(temp.at<C>(tempa, j)[2]);
				}
				output.at<C>(i, j)[0] = (G)sum1_b;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_r;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				float sum2 = 0.0;
				for (int a = -n; a <= n; a++) {
					if ((i + a <= row - 1) && (i + a >= 0)) {
						kernelvalue = kernel_y.at<float>(n + a);
						sum1_b += kernelvalue * (float)(temp.at<C>(i + a, j)[0]);
						sum1_g += kernelvalue * (float)(temp.at<C>(i + a, j)[1]);
						sum1_r += kernelvalue * (float)(temp.at<C>(i + a, j)[2]);
						sum2 += kernelvalue;
					}
					output.at<C>(i, j)[0] = (G)(sum1_b / sum2);
					output.at<C>(i, j)[1] = (G)(sum1_g / sum2);
					output.at<C>(i, j)[2] = (G)(sum1_r / sum2);
				}
			}

		}
	}
	return output;
}
