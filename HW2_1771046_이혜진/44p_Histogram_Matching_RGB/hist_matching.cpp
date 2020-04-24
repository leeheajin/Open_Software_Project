#include "hist_func.h" 

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);
void matching_func(Mat& input, Mat& equalized, G* trans_func, float* CDF);

int main() {

	Mat input = imread("input.jpg", IMREAD_COLOR);
	Mat reference = imread("reference.jpg", IMREAD_COLOR);
	Mat equalized_YUV_input;
	Mat equalized_YUV_reference;

	// RGB -> YUV
	cvtColor(input, equalized_YUV_input, COLOR_RGB2YUV);
	cvtColor(reference, equalized_YUV_reference, COLOR_RGB2YUV);	
	Mat matching_input = equalized_YUV_input.clone();

	// namedWindow("matching_test_input", WINDOW_AUTOSIZE); // 애초에 초록빛으로 출력
	// imshow("matching_test_input", matching_input);

	// split each channel(Y, U, V) input
	Mat channels1[3];
	split(equalized_YUV_input, channels1);
	Mat Y1 = channels1[0];

	// split each channel(Y, U, V) reference
	Mat channels2[3];
	split(equalized_YUV_reference, channels2);
	Mat Y2 = channels2[0];

	// PDF or transfer function txt files
	FILE* f_PDF_RGB_input;
	FILE* f_equalized_PDF_YUV_input;
	FILE* f_trans_func_eq_YUV_input;
	FILE* f_matching;
	FILE* f_PDF_matching;
	FILE* f_matching_PDF_YUV_input;

	FILE* f_PDF_RGB_reference;
	FILE* f_equalized_PDF_YUV_reference;
	FILE* f_trans_func_eq_YUV_reference;
	FILE* f_inverse;

	fopen_s(&f_PDF_RGB_input, "PDF_RGB_input.txt", "w+");
	fopen_s(&f_equalized_PDF_YUV_input, "equalized_PDF_YUV_input.txt", "w+");
	fopen_s(&f_trans_func_eq_YUV_input, "trans_func_eq_YUV_input.txt", "w+");
	fopen_s(&f_matching, "matching.txt", "w+");
	fopen_s(&f_PDF_matching, "PDF_matching.txt", "w+");
	fopen_s(&f_matching_PDF_YUV_input, "matching_PDF_YUV_input.txt", "w+");

	fopen_s(&f_PDF_RGB_reference, "PDF_RGB_reference.txt", "w+");
	fopen_s(&f_equalized_PDF_YUV_reference, "equalized_PDF_YUV_reference.txt", "w+");
	fopen_s(&f_trans_func_eq_YUV_reference, "trans_func_eq_YUV_reference.txt", "w+");
	fopen_s(&f_inverse, "inverse.txt", "w+");

	float** PDF_RGB_input = cal_PDF_RGB(input);		// PDF of Input image(RGB) : [L][3]
	float* CDF_YUV_input = cal_CDF(Y1);				// CDF of Y channel image

	float** PDF_RGB_reference = cal_PDF_RGB(reference);		// PDF of reference image(RGB) : [L][3]
	float* CDF_YUV_reference = cal_CDF(Y2);					// CDF of Y channel image

	G trans_func_eq_YUV_input[L] = { 0 };			// transfer function
	G trans_func_eq_YUV_reference[L] = { 0 };		// transfer function

	// histogram equalization on Y channel
	hist_eq(equalized_YUV_input, Y1, trans_func_eq_YUV_input, CDF_YUV_input);
	hist_eq(equalized_YUV_reference, Y2, trans_func_eq_YUV_reference, CDF_YUV_reference);

	// merge Y, U, V channels
	merge(channels1, 3, equalized_YUV_input);
	merge(channels2, 3, equalized_YUV_reference);

	// YUV -> RGB (use "CV_YUV2RGB" flag)
	cvtColor(equalized_YUV_input, equalized_YUV_input, COLOR_YUV2RGB);
	cvtColor(equalized_YUV_reference, equalized_YUV_reference, COLOR_YUV2RGB);

	// equalized PDF (YUV)
	float** equalized_PDF_YUV_input = cal_PDF_RGB(equalized_YUV_input);
	float** equalized_PDF_YUV_reference = cal_PDF_RGB(equalized_YUV_reference);


	namedWindow("Equalized_input", WINDOW_AUTOSIZE);
	imshow("Equalized_input", equalized_YUV_input);

	// namedWindow("Equalizedtest1_reference", WINDOW_AUTOSIZE);
	// imshow("Equalizedtest1_reference", equalized_YUV_reference);

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF_RGB_input, "%d\t%f\t%f\t%f\n", i, PDF_RGB_input[i][0], PDF_RGB_input[i][1], PDF_RGB_input[i][2]);
		fprintf(f_equalized_PDF_YUV_input, "%d\t%f\t%f\t%f\n", i, equalized_PDF_YUV_input[i][0], equalized_PDF_YUV_input[i][1], equalized_PDF_YUV_input[i][2]);

		fprintf(f_PDF_RGB_reference, "%d\t%f\t%f\t%f\n", i, PDF_RGB_reference[i][0], PDF_RGB_reference[i][1], PDF_RGB_reference[i][2]);
		fprintf(f_equalized_PDF_YUV_reference, "%d\t%f\t%f\t%f\n", i, equalized_PDF_YUV_reference[i][0], equalized_PDF_YUV_reference[i][1], equalized_PDF_YUV_reference[i][2]);

		// write transfer functions
		fprintf(f_trans_func_eq_YUV_input, "%d\t%d\n", i, trans_func_eq_YUV_input[i]);
		fprintf(f_trans_func_eq_YUV_reference, "%d\t%d\n", i, trans_func_eq_YUV_reference[i]);
	}
	
	////////// 아래에서 변수 재사용하기 때문에 여기서 출력 //////////

	namedWindow("RGB_input", WINDOW_AUTOSIZE);
	imshow("RGB_input", input);

	namedWindow("RGB_reference", WINDOW_AUTOSIZE);
	imshow("RGB_reference", reference);

	namedWindow("Equalized_reference", WINDOW_AUTOSIZE);
	imshow("Equalized_reference", equalized_YUV_reference);

	////////// 위의 코드 까지는 그냥 equalized를 하기 위한 내용들; 변수 초기화 하고 재시작//////////

	// RGB -> YUV
	cvtColor(input, equalized_YUV_input, COLOR_RGB2YUV);
	cvtColor(reference, equalized_YUV_reference, COLOR_RGB2YUV);

	// resplit each channel(Y, U, V) input
	split(equalized_YUV_input, channels1);
	Y1 = channels1[0];

	// resplit each channel(Y, U, V) reference
	split(equalized_YUV_reference, channels2);
	Y2 = channels2[0];

	// inverse of G (G: reference trans func)
	G inverse[L] = { 0 };

	int count = 0;
	for (int i = 0; i < L; i++) {
		if (trans_func_eq_YUV_reference[i] >= count) {
			inverse[trans_func_eq_YUV_reference[i]] = i;
			count = trans_func_eq_YUV_reference[i];
		}
		if ((i>0) && (trans_func_eq_YUV_reference[i] - trans_func_eq_YUV_reference[i - 1] > 1)) { // 역함수를 취했을 때 빈공간이 만들어지는 경우
			for (int j = trans_func_eq_YUV_reference[i - 1] + 1; j < trans_func_eq_YUV_reference[i]; j++)
				inverse[j] = i; // 빈 공간에 현재 제일 큰 값을 대입
		}
	}
	if (inverse[L - 1] == 0)  // inverse[255] == 0 에 대한 예외처리
		inverse[L - 1] = inverse[L - 2]; // inverse[255] = inverse[254]
	
	// write inverse
	for (int i = 0; i < L; i++) {
		fprintf(f_inverse, "%d\t%d\n", i, inverse[i]);
	}

	// namedWindow("Equalizedtest1_input", WINDOW_AUTOSIZE);
	// imshow("Equalizedtest1_input", equalized_YUV_input);

	// make and write matching func
	G matching[L] = { 0 };
	for (int i = 0; i < L; i++) {
		matching[i] = inverse[trans_func_eq_YUV_input[i]];
		// printf("matching[%d]=%d\n", i, matching[i]);
		fprintf(f_matching, "%d\t%d\n", i, matching[i]);
	}
	
	matching_func(equalized_YUV_input, Y1, matching, CDF_YUV_input);		// histogram equalization on Y channel using above func
	float* matching_PDF_YUV_input = cal_PDF(matching_input);				// equalized PDF (RGB)
	
	// merge Y, U, V channels
	merge(channels1, 3, equalized_YUV_input);

	// YUV -> RGB (use "CV_YUV2RGB" flag)
	cvtColor(equalized_YUV_input, equalized_YUV_input, COLOR_YUV2RGB);

	// namedWindow("Equalizedtest1_input", WINDOW_AUTOSIZE);
	// imshow("Equalizedtest1_input", equalized_YUV_input);

	// equalized PDF (YUV)
	float** matching_PDF_RGB_input = cal_PDF_RGB(equalized_YUV_input);

	for (int i = 0; i < L; i++) {
		// write PDF of matching input
		fprintf(f_PDF_matching, "%d\t%f\t%f\t%f\n", i, matching_PDF_RGB_input[i][0], matching_PDF_RGB_input[i][1], matching_PDF_RGB_input[i][2]);
		fprintf(f_matching_PDF_YUV_input, "%d\t%f\n", i, matching_PDF_YUV_input[i]);
	}

	// memory release
	free(PDF_RGB_input);
	free(CDF_YUV_input);
	fclose(f_PDF_RGB_input);
	fclose(f_equalized_PDF_YUV_input);
	fclose(f_trans_func_eq_YUV_input);
	fclose(f_matching);
	fclose(f_PDF_RGB_input);
	fclose(f_matching_PDF_YUV_input);

	free(PDF_RGB_reference);
	free(CDF_YUV_reference);
	fclose(f_PDF_RGB_reference);
	fclose(f_equalized_PDF_YUV_reference);
	fclose(f_trans_func_eq_YUV_reference);
	fclose(f_inverse);
	////////////////////// Show each image ///////////////////////

	namedWindow("Matching_input", WINDOW_AUTOSIZE);
	imshow("Matching_input", equalized_YUV_input);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}

// histogram equalization
void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF) {

	// compute transfer function
	for (int i = 0; i < L; i++)
		trans_func[i] = (G)((L - 1) * CDF[i]);

	// perform the transfer function
	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			equalized.at<G>(i, j) = trans_func[input.at<Vec3b>(i, j)[0]];

}

void matching_func(Mat& input, Mat& equalized, G* trans_func, float* CDF) {

	// perform the transfer function
	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			equalized.at<G>(i, j) = trans_func[input.at<Vec3b>(i, j)[0]];

}