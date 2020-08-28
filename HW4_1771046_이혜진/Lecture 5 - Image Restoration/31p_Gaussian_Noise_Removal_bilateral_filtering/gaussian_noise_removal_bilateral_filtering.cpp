#include <opencv2/opencv.hpp>
#include <stdio.h>

#define IM_TYPE	CV_64FC3

using namespace cv;

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

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);
Mat Bilateralfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char* opt);
Mat Bilateralfilter_RGB(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char* opt);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat input_gray;

	// check for validation
	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, COLOR_RGB2GRAY);	// convert RGB to Grayscale

	// 8-bit unsigned char -> 64-bit floating point
	input.convertTo(input, CV_64FC3, 1.0 / 255);
	input_gray.convertTo(input_gray, CV_64F, 1.0 / 255);

	// Add noise to original image
	Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
	Mat noise_RGB = Add_Gaussian_noise(input, 0, 0.1);

	// Denoise, using bilateral filter
	Mat Denoised_Gray = Bilateralfilter_Gray(noise_Gray, 3, 10, 10, 10, "mirroring");
	Mat Denoised_RGB = Bilateralfilter_RGB(noise_RGB, 3, 10, 10, 10, "mirroring");

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	/*not print noise image
	namedWindow("Gaussian Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (Grayscale)", noise_Gray);

	namedWindow("Gaussian Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (RGB)", noise_RGB); */

	namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Grayscale)", Denoised_Gray);

	namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (RGB)", Denoised_RGB);

	waitKey(0);

	return 0;
}

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma) {

	Mat NoiseArr = Mat::zeros(input.rows, input.cols, input.type());
	RNG rng;
	rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);

	add(input, NoiseArr, NoiseArr);

	return NoiseArr;
}

Mat Bilateralfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	double kernelvalue;
	int tempx;
	int tempy;

	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_64F);
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-padding")) {

				// kernel making
				double denom = 0.0;
				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						double value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
						double p = (double)input.at<G>(i, j);
						double q = 0;
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { // if the pixel is not a border pixel
							q = (double)input.at<G>(i + a, j + b);
						}
						double value2 = exp(-(pow(p - q, 2) / (2 * pow(sigmaR, 2))));
						kernel.at<double>(a + n, b + n) = value1 * value2; // Kernel boundary
						denom += value1 * value2; // Save for later division
					}
				}

				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						if (kernel.at<double>(a + n, b + n) != 0) {
							kernel.at<double>(a + n, b + n) /= denom; // Low-pass filter
						}
					}
				}

				double sum1 = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) { // if the pixel is not a border pixel
							kernelvalue = kernel.at<double>(n + x, n + y);
							sum1 += kernelvalue * (double)(input.at<G>(i + x, j + y));
						}
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "mirroring")) {

				// kernel making
				double denom = 0.0;
				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						double value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
						double p = (double)input.at<G>(i, j);
						double q = 0;
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { // if the pixel is not a border pixel
							q = (double)input.at<G>(i + a, j + b);
						}
						double value2 = exp(-(pow(p - q, 2) / (2 * pow(sigmaR, 2))));
						kernel.at<double>(a + n, b + n) = value1 * value2; // Kernel boundary
						denom += value1 * value2; // Save for later division
					}
				}

				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						if (kernel.at<double>(a + n, b + n) != 0) {
							kernel.at<double>(a + n, b + n) /= denom; // Low-pass filter
						}
					}
				}

				double sum1 = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if (i + x > row - 1) {  //mirroring for the border pixels
							tempx = i - x + 1;
						}
						else if (i + x < 0) {
							tempx = -(i + x - 1);
						}
						else {
							tempx = i + x;
						}
						if (j + y > col - 1) {
							tempy = j - y + 1;
						}
						else if (j + y < 0) {
							tempy = -(j + y - 1);
						}
						else {
							tempy = j + y;
						}
						kernelvalue = kernel.at<double>(n + x, n + y);
						sum1 += kernelvalue * (double)(input.at<G>(tempx, tempy));
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "adjustkernel")) {

				// kernel making
				double denom = 0.0;
				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						double value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
						double p = (double)input.at<G>(i, j);
						if ((i + a > row - 1) || (i + a < 0) || (j + b > col - 1) || (j + b < 0)) { // if the pixel is a border pixel
							kernel.at<double>(a + n, b + n) = 0;
							continue;
						}
						double q = (double)input.at<G>(i + a, j + b);
						double value2 = exp(-(pow(p - q, 2) / (2 * pow(sigmaR, 2))));
						kernel.at<double>(a + n, b + n) = value1 * value2; // Kernel boundary
						// printf("(%d, %d) / (%d, %d) = %p \n",i, j, a, b, kernel.at<double>(a + n, b + n));
						denom += value1 * value2; // Save for later division
					}
				}

				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						if (kernel.at<double>(a + n, b + n) != 0) {
							kernel.at<double>(a + n, b + n) /= denom; // Low-pass filter
						}
					}
				}

				double sum1 = 0.0;
				double sum2 = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							kernelvalue = kernel.at<double>(n + x, n + y);
							sum1 += kernelvalue * (double)(input.at<G>(i + x, j + y));
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

Mat Bilateralfilter_RGB(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	double kernelvalue;
	double tempa;
	double tempb;

	// Initialiazing Gaussian Kernel Matrix
	// Fill code to initialize Gaussian filter kernel matrix
	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_64F);
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-padding")) {

				// kernel making
				double denom = 0.0;
				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						double value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
						double p = (double)input.at<G>(i, j);
						double q = 0;
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { // if the pixel is not a border pixel
							q = (double)input.at<G>(i + a, j + b);
						}
						double value2 = exp(-(pow(p - q, 2) / (2 * pow(sigmaR, 2))));
						kernel.at<double>(a + n, b + n) = value1 * value2; // Kernel boundary
						denom += value1 * value2; // Save for later division
					}
				}

				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						if (kernel.at<double>(a + n, b + n) != 0) {
							kernel.at<double>(a + n, b + n) /= denom; // Low-pass filter
						}
					}
				}

				double sum1_r = 0.0;
				double sum1_g = 0.0;
				double sum1_b = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) { // if the pixel is not a border pixel
							kernelvalue = kernel.at<double>(n + x, n + y);
							sum1_b += kernelvalue * (double)(input.at<C>(i + x, j + y)[0]);
							sum1_g += kernelvalue * (double)(input.at<C>(i + x, j + y)[1]);
							sum1_r += kernelvalue * (double)(input.at<C>(i + x, j + y)[2]);
						}
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_b;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_r;
			}

			else if (!strcmp(opt, "mirroring")) {

				// kernel making
				double denom = 0.0;
				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						double value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
						double p = (double)input.at<G>(i, j);
						double q = 0;
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { // if the pixel is not a border pixel
							q = (double)input.at<G>(i + a, j + b);
						}
						double value2 = exp(-(pow(p - q, 2) / (2 * pow(sigmaR, 2))));
						kernel.at<double>(a + n, b + n) = value1 * value2; // Kernel boundary
						denom += value1 * value2; // Save for later division
					}
				}

				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						if (kernel.at<double>(a + n, b + n) != 0) {
							kernel.at<double>(a + n, b + n) /= denom; // Low-pass filter
						}
					}
				}

				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if (i + x > row - 1) {  //mirroring for the border pixels
							tempa = i - x + 1;
						}
						else if (i + x < 0) {
							tempa = -(i + x - 1);
						}
						else {
							tempa = i + x;
						}
						if (j + y > col - 1) {
							tempb = j - y + 1;
						}
						else if (j + y < 0) {
							tempb = -(j + y - 1);
						}
						else {
							tempb = j + y;
						}
						kernelvalue = kernel.at<double>(n + x, n + y);
						sum1_b += kernelvalue * (double)(input.at<C>(tempa, tempb)[0]);
						sum1_g += kernelvalue * (double)(input.at<C>(tempa, tempb)[1]);
						sum1_r += kernelvalue * (double)(input.at<C>(tempa, tempb)[2]);
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_b;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_r;
			}

			else if (!strcmp(opt, "adjustkernel")) {

				// kernel making
				double denom = 0.0;
				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						double value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
						double p = (double)input.at<G>(i, j);
						if ((i + a > row - 1) || (i + a < 0) || (j + b > col - 1) || (j + b < 0)) { // if the pixel is a border pixel
							kernel.at<double>(a + n, b + n) = 0;
							continue;
						}
						double q = (double)input.at<G>(i + a, j + b);
						double value2 = exp(-(pow(p - q, 2) / (2 * pow(sigmaR, 2))));
						kernel.at<double>(a + n, b + n) = value1 * value2; // Kernel boundary
						// printf("(%d, %d) / (%d, %d) = %p \n",i, j, a, b, kernel.at<double>(a + n, b + n));
						denom += value1 * value2; // Save for later division
					}
				}

				for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
					for (int b = -n; b <= n; b++) {
						if (kernel.at<double>(a + n, b + n) != 0) {
							kernel.at<double>(a + n, b + n) /= denom; // Low-pass filter
						}
					}
				}

				double sum1_r = 0.0;
				double sum1_g = 0.0;
				double sum1_b = 0.0;
				double sum2 = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							kernelvalue = kernel.at<double>(n + x, n + y);
							sum1_b += kernelvalue * (double)(input.at<C>(i + x, j + y)[0]);
							sum1_g += kernelvalue * (double)(input.at<C>(i + x, j + y)[1]);
							sum1_r += kernelvalue * (double)(input.at<C>(i + x, j + y)[2]);
							sum2 += kernelvalue;
						}
					}
				}
				output.at<C>(i, j)[0] = (G)(sum1_b / sum2);
				output.at<C>(i, j)[1] = (G)(sum1_g / sum2);
				output.at<C>(i, j)[2] = (G)(sum1_r / sum2);
			}

		}
	}

	return output;
}