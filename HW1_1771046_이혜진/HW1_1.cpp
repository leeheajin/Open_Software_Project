#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

template <typename T>
Mat myrotate(const Mat input, float angle, const char* opt);
int NN(float point);

int main()
{
	Mat input, rotated;
	
	// Read each image
	input = imread("lena.jpg");

	// Check for invalid input
	if (!input.data) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	
	// original image
	namedWindow("image");
	imshow("image", input);
	rotated = myrotate<Vec3b>(input, 45, "bilinear");

	// rotated image
	namedWindow("rotated");
	imshow("rotated", rotated);

	waitKey(0);

	return 0;
}

template <typename T> // template of function : it makes template instance function according to parameter
Mat myrotate(const Mat input, float angle, const char* opt) {
	int row = input.rows; // 512 X 512 size
	int col = input.cols;

	float radian = angle * CV_PI / 180;

	// fit size of window to rotation photo
	float sq_row = ceil(row * sin(radian) + col * cos(radian));
	float sq_col = ceil(col * sin(radian) + row * cos(radian));

	Mat output = Mat::zeros(sq_row, sq_col, input.type());

	for (int i = 0; i < sq_row; i++) {  // rotational x and y
		for (int j = 0; j < sq_col; j++) {
			float y = (j - sq_col / 2) * cos(radian) - (i - sq_row / 2) * sin(radian) + col / 2; // original x and y
			float x = (j - sq_col / 2) * sin(radian) + (i - sq_row / 2) * cos(radian) + row / 2;

			if ((y >= 0) && (y <= (row - 1)) && (x >= 0) && (x <= (col - 1))) {
				if (!strcmp(opt, "nearest")) { // 1. opt == nearest
				
					output.at<Vec3b>(i, j) = input.at<Vec3b>(NN(x), NN(y)); // use function and ssign the value

				}

				else if (!strcmp(opt, "bilinear")) { // 2. opt == bilinear

					int x_floor = floor(x); // Only integer part
					int y_floor = floor(y); 

					float alpha = x - x_floor; 
					float gamma = y - y_floor;

					Vec3b v1 = gamma * input.at<Vec3b>(x_floor, y_floor + 1) + (1 - gamma) * input.at<Vec3b>(x_floor, y_floor);
					Vec3b v2 = gamma * input.at<Vec3b>(x_floor + 1, y_floor + 1) + (1 - gamma) * input.at<Vec3b>(x_floor + 1, y_floor);

					output.at<Vec3b>(i, j) = alpha * v2 + (1 - alpha) * v1; // assign the value

				}
			}
		}
	}

	return output;
}

int NN(float point) { // choose more closer	point

	int point_int = floor(point); // Only integer part
	float gamma = point - point_int;

	if (gamma <= 0.5) return point_int;
	else return point_int + 1;

}