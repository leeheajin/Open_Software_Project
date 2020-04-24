#include "hist_func.h" 

void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF);
void matching_func(Mat& input, Mat& equalized, G* trans_func, float* CDF);

int main() {

	Mat input = imread("input.jpg", IMREAD_COLOR);
	Mat reference = imread("reference.jpg", IMREAD_COLOR);
	Mat input_gray;
	Mat reference_gray;

	cvtColor(input, input_gray, COLOR_RGB2GRAY);	// convert RGB to Grayscale
	cvtColor(reference, reference_gray, COLOR_RGB2GRAY);	

	Mat equalized_input = input_gray.clone();
	Mat equalized_reference = reference_gray.clone();
	Mat matching_input = input_gray.clone();

	// PDF or transfer function txt files
	FILE* f_PDF_input;
	FILE* f_equalized_PDF_gray_input;
	FILE* f_trans_func_eq_input;
	FILE* f_matching;
	FILE* f_PDF_matching;
	FILE* f_matching_PDF_gray_input;

	FILE* f_PDF_reference;
	FILE* f_equalized_PDF_gray_reference;
	FILE* f_trans_func_eq_reference;
	FILE* f_inverse;

	fopen_s(&f_PDF_input, "PDF_input.txt", "w+");
	fopen_s(&f_equalized_PDF_gray_input, "equalized_PDF_gray_input.txt", "w+");
	fopen_s(&f_trans_func_eq_input, "trans_func_eq_input.txt", "w+");
	fopen_s(&f_matching, "matching.txt", "w+");
	fopen_s(&f_PDF_matching, "PDF_matching.txt", "w+");
	fopen_s(&f_matching_PDF_gray_input, "matching_PDF_gray_input.txt", "w+");

	fopen_s(&f_PDF_reference, "PDF_reference.txt", "w+");
	fopen_s(&f_equalized_PDF_gray_reference, "equalized_PDF_gray_reference.txt", "w+");
	fopen_s(&f_trans_func_eq_reference, "trans_func_eq_reference.txt", "w+");
	fopen_s(&f_inverse, "inverse.txt", "w+");

	float* PDF_input = cal_PDF(input_gray);	// PDF of Input image(Grayscale) : [L]
	float* CDF_input = cal_CDF(input_gray);	// CDF of Input image(Grayscale) : [L]

	float* PDF_reference = cal_PDF(reference_gray);	// PDF of reference image(Grayscale) : [L]
	float* CDF_reference = cal_CDF(reference_gray);	// CDF of reference image(Grayscale) : [L]

	G trans_func_eq_input[L] = { 0 };			// transfer function
	G trans_func_eq_reference[L] = { 0 };		// transfer function

	hist_eq(input_gray, equalized_input, trans_func_eq_input, CDF_input);						// histogram equalization on grayscale image
	float* equalized_PDF_gray_input = cal_PDF(equalized_input);									// equalized PDF (grayscale)

	hist_eq(reference_gray, equalized_reference, trans_func_eq_reference, CDF_reference);		// histogram equalization on grayscale image
	float* equalized_PDF_gray_reference = cal_PDF(equalized_reference);

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF_input, "%d\t%f\n", i, PDF_input[i]);
		fprintf(f_equalized_PDF_gray_input, "%d\t%f\n", i, equalized_PDF_gray_input[i]);

		fprintf(f_PDF_reference, "%d\t%f\n", i, PDF_reference[i]);
		fprintf(f_equalized_PDF_gray_reference, "%d\t%f\n", i, equalized_PDF_gray_reference[i]);

		// write transfer functions
		fprintf(f_trans_func_eq_input, "%d\t%d\n", i, trans_func_eq_input[i]);

		fprintf(f_trans_func_eq_reference, "%d\t%d\n", i, trans_func_eq_reference[i]);
	}

	// inverse of G (G: reference trans func)
	G inverse[L] = { 0 };

	int count = 0;
	for (int i = 0; i < L; i++) {
		if (trans_func_eq_reference[i] >= count) {
			inverse[trans_func_eq_reference[i]] = i;
			count = trans_func_eq_reference[i];
		}
		if ((i>0) && (trans_func_eq_reference[i] - trans_func_eq_reference[i - 1] > 1)) { // 역함수를 취했을 때 빈공간이 만들어지는 경우
			for (int j = trans_func_eq_reference[i - 1] + 1; j < trans_func_eq_reference[i]; j++) 
				inverse[j] = i; // 빈 공간에 현재 제일 큰 값을 대입
		}
	}
	if (inverse[L - 1] == 0)  // inverse[255] == 0 에 대한 예외처리
		inverse[L - 1] = inverse[L - 2]; // inverse[255] = inverse[254]
	
	// write inverse
	for (int i = 0; i < L; i++) {
		fprintf(f_inverse, "%d\t%d\n", i, inverse[i]);
	}

	// make and write matching func
	G matching[L] = { 0 };
	for (int i = 0; i < L; i++) {
		matching[i] = inverse[trans_func_eq_input[i]];
		// printf("matching[%d]=%d\n", i, matching[i]);
		fprintf(f_matching, "%d\t%d\n", i, matching[i]);
	}

	matching_func(input_gray, matching_input, matching, CDF_input);		// histogram equalization on grayscale image using above func
	float* matching_PDF_gray_input = cal_PDF(matching_input);			// equalized PDF (grayscale)

	for (int i = 0; i < L; i++) {
		// write PDF of matching input
		fprintf(f_PDF_matching, "%d\t%f\n", i, matching_PDF_gray_input[i]);
		fprintf(f_matching_PDF_gray_input, "%d\t%f\n", i, matching_PDF_gray_input[i]);
	}

	// memory release
	free(PDF_input);
	free(CDF_input);
	fclose(f_PDF_input);
	fclose(f_equalized_PDF_gray_input);
	fclose(f_trans_func_eq_input);
	fclose(f_matching);
	fclose(f_PDF_input);
	fclose(f_matching_PDF_gray_input);

	free(PDF_reference);
	free(CDF_reference);
	fclose(f_PDF_reference);
	fclose(f_equalized_PDF_gray_reference);
	fclose(f_trans_func_eq_reference);
	fclose(f_inverse);
	////////////////////// Show each image ///////////////////////

	namedWindow("Grayscale_input", WINDOW_AUTOSIZE);
	imshow("Grayscale_input", input_gray);

	namedWindow("Equalized_input", WINDOW_AUTOSIZE);
	imshow("Equalized_input", equalized_input);

	namedWindow("Matching_input", WINDOW_AUTOSIZE);
	imshow("Matching_input", matching_input);

	namedWindow("Grayscale_reference", WINDOW_AUTOSIZE);
	imshow("Grayscale_reference", reference_gray);

	namedWindow("Equalized_reference", WINDOW_AUTOSIZE);
	imshow("Equalized_reference", equalized_reference);

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
			equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];

}

void matching_func(Mat& input, Mat& equalized, G* trans_func, float* CDF) {

	// perform the transfer function
	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++)
			equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];

}