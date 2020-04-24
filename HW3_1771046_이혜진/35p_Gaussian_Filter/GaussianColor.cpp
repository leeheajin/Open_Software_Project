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

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

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

	namedWindow("RGBscale", WINDOW_AUTOSIZE);
	imshow("RGBscale", input);

	start = clock(); // time check
	output = gaussianfilter(input, 3,3,3, "zero-paddle"); //Boundary process: zero-paddle, mirroring, adjustkernel
	end = clock();

	res = (float)(end - start) / CLOCKS_PER_SEC;
	printf("time = %f", res);

	namedWindow("Gaussian Filter", WINDOW_AUTOSIZE);
	imshow("Gaussian Filter", output);


	waitKey(0);

	return 0;
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
			kernel.at<float>(a+n, b+n) = value1; // Kernel boundary
			denom += value1; // Save for later division
		}
	}

	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {			
			kernel.at<float>(a+n, b+n) /= denom; // Low-pass filter
		}
	}

	Mat output = Mat::zeros(row, col, input.type());
	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			
			if (!strcmp(opt, "zero-paddle")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {

						/* Gaussian filter with Zero-paddle boundary process:		

						Fill the code:
						*/

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { // if the pixel is not a border pixel
							kernelvalue = kernel.at<float>(n + a, n + b);
							sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
							sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
							sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
						}

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
						sum1_b += kernelvalue * (float)(input.at<C>(tempa, tempb)[0]);
						sum1_g += kernelvalue * (float)(input.at<C>(tempa, tempb)[1]);
						sum1_r += kernelvalue * (float)(input.at<C>(tempa, tempb)[2]);
					}
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
					for (int b = -n; b <= n; b++) {

						/* Gaussian filter with "adjustkernel" process:

						Fill the code:
						*/

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							kernelvalue = kernel.at<float>(n + a, n + b);
							sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
							sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
							sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
							sum2 += kernelvalue;
						}
						output.at<C>(i, j)[0] = (G)(sum1_b / sum2);
						output.at<C>(i, j)[1] = (G)(sum1_g / sum2);
						output.at<C>(i, j)[2] = (G)(sum1_r / sum2);
						
					}
				}
			}
		}
	}
	return output;
}