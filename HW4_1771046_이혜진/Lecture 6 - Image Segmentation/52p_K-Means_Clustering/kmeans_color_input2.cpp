#include <iostream>
#include <opencv2/opencv.hpp>

#define IM_TYPE	CV_8UC3

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

// Note that this code is for the case when an input data is a color value.
int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat output;

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);

	Mat samples(input.rows * input.cols, 5, CV_32F);
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++) {
			for (int z = 0; z < 3; z++)
				samples.at<float>(y + x * input.rows, z) = input.at<C>(y, x)[z] / (float)255;
			samples.at<float>(y + x * input.rows, 3) = x / (float)512;
			samples.at<float>(y + x * input.rows, 4) = y / (float)512;
		}

	// Clustering is performed for each channel (RGB)
	// Note that the intensity value is not normalized here (0~1). You should normalize both intensity and position when using them simultaneously.
	int clusterCount = 10;
	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	// printf("%d %d", labels.rows, labels.cols);
	// std::cout << centers << std::endl;

	Mat new_image(input.size(), input.type());
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * input.rows, 0);
			// Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
			new_image.at<C>(y, x)[0] = centers.at<float>(cluster_idx, 0) * 255;
			new_image.at<C>(y, x)[1] = centers.at<float>(cluster_idx, 1) * 255;
			new_image.at<C>(y, x)[2] = centers.at<float>(cluster_idx, 2) * 255;
		}

	imshow("clustered image", new_image);

	waitKey(0);

	return 0;
}
