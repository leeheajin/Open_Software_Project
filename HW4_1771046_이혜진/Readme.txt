Open-Software Project (01)
Assignment4 : OSP-Lec05-Image Restoration, OSP-Lec06-Image Segmentation

 필자는 Visual Stduio 2019와 Open CV 4.2.13 버전을 사용하였으며, 주어진 skeleton code에서 환경에 맞추기 위해 몇 가지 변수들을 수정하였다.
1. 색상 변환 변수
 : RGB, GRAY, YUV 간의 변환과 관련된 변수(ex.CV_RGB2GRAY, CV_RGB2YUV)를 모두 CV -> COLOR로 바꾸었다.
   CV_RGB2GRAY -> COLOR_RGB2GRAY
2. 이미지 읽기 변수
: IMREAD와 관련하여 이미지를 어떠한 형태로 불러오는지와 관련된 변수(ex. CV_LOAD_IMAGE_COLOR)를 모두 CV_LOAD_IMAGE -> IMREAD로 바꾸었다.
   CV_LOAD_IMAGE_COLOR -> IMREAD_COLOR
3. TermCriteria 변수
: k-means 알고리즘에 이용되는 변수를 CV_TERMCRIT_ITER | CV_TERMCRIT_EPS -> TermCriteria::MAX_ITER | TermCriteria::EPS로 바꾸었다.

Lec05 코드의 일부분에 괄호가 맞지 않는 부분이 있었다. 해당 부분에 추가적인 } 괄호를 추가했다.

--------------------------------------------------------------------------------------------------------------------------------

 제출 코드가 많기 때문에 skeleton code 중에서 스스로 구현한 부분에 대해서만 technical report에서 언급하였다. (결과 및 분석은 모두 포함)
따라서 직접 구현하지 않은 부분에 대해서는 Readme.txt에서 간략히 설명한다.

--------------------------------------------------------------------------------------------------------------------------------

1. 27p_Salt_and_Pepper_Noise_Generate
   salt(255) and pepper(0) noise를 생성하는 코드이다. 직접 구현할 부분은 없다.
   skeleton 코드인 'salt_and_pepper'의 일부를 인용해 만들었으며, Gray와 Color에 대해 한 개의 코드 내에서 실행하였다.
   RNG는 난수 발생기이다. 랜덤하게 난수를 발생시켜 noise를 이미지에 추가한다.
   선택된 픽셀에 대해, Gray 이미지는 output.at<G>(rng.uniform(0, output.rows), rng.uniform(0, output.cols)) = 0; 처럼 한 개의 변수에만 값(0또는 1)을 할당한다.
   이에 반해, Color 이미지는 3개의 채널이 있기 때문에 output.at<C>(x, y)[0] = 255; 처럼 3개의 채널에 각각 값(0 또는 1)을 할당한다.
   
2. 28p_Salt_and_Pepper_Noise_Removal
   1번에서 이용한 코드를 이용해 불러온 이미지에 noise를 추가한다. 이후 median filter를 이용해 noise를 제거한다.
   mean(평균) filter가 아닌 median filter를 이용하는 이유는, ouutlier를 제거하기 위해서이다.
   salt와 pepper는 각각 255와 0의 intensity를 갖는다. 따라서 이들은 outlier라고 할 수 있다.
   필터를 이용해 주변의 값들 중 median(중간값)을 구해 해당 값으로 output의 픽셀값을 설정한다.
   필터를 적용할 때, zero-padding, mirroring, kerneladjusting의 3가지 방법을 통해 구현한다.
   이미지 내의 범위가 아닌 필터의 부분에 대한 처리를 고려하지 않았던 이전과는 달리, 필터의 범위에 대한 생각이 많이 필요하였다.
   이는 이전까지는 mean filter를 이용하였기 때문에 총 합만을 고려하였으나, median filter는 총 합이 아닌 모든 값들을 정렬하는 과정이 필요하기 때문이다.
   따라서 모든 필터에 대해 값들이 할당되어야 한다. 각 방법에 대해 필터를 이용해 output 값을 구하면, noise가 제거된 이미지를 얻을 수 있다.

3. 29p_Gaussian_Noise_Generate
   1번과 유사하게 가우시안 필터를 이용해 noise를 생성하는 코드이다. 마찬가지로 직접 구현할 부분은 없다.
   rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);  add(input, NoiseArr, NoiseArr); 와 난수 발생기를 이용해 매우 손 쉽게 noise를 추가할 수 있다.
   0또는 1만의 값을 갖는 salt and pepper noise와는 달리, 가우시안 noise는 임의의 값이 더해진다.
   가우시안 noise가 적용된 출력은 Output = Input + Noise의 형태를 띄는데, 이 때, output만을 이용해 나머지 변수들을 추정하기란 쉽지 않다.
   즉, 한 가지의 값을 가지고 2개의 변수를 추정하는 것이 어렵다는 것이다. 따라서 output image만을 통해 input image와 noise를 구분하는 것은 매우 어렵다.
   가우시안 noise는 noise의 확률 분포가 가우시안 함수와 유사하다는 것이다. 가우시안 함수는 0을 기준으로 대칭적인 정규 분포를 따른다.
   따라서 특정 시간(여러 장의 사진)이나 특정 공간(정해진 범위) 내에 존재하는 모든 픽셀값들을 합치면 noise가 0에 수렴할 것이라는 가정을 할 수 있다.
   이는 아래의 가우시안 noise 제거의 기본 원리이다.
   
4. 30p_Gaussian_Noise_Removal_gaussian_filtering
   3번에서 생성한 가우시안 noise를 제거하는 필터이다. 가우시안 nosie를 제거하기 위해 이용하는 필터에는 여러가지가 존재한다.
   해당 과제에서는 가우시안 필터를 이용해 노이즈를 제거하는 방법을 다룬다. mean filter중 하나인 가우시안 필터에 대한 정의가 필요하다.
   또한 위에서 언급된 필터를 적용하는 3가지 방법에 대한 구현이 필요했다. 두 부분 모두 변수를 double로 바꾼 것 이외의 변화는 거의 없었다.

5. 31p_Gaussian_Noise_Removal_bilateral_filtering
   가우시안 노이즈를 bilateral 필터를 이용해 제거하는 방법을 다룬다. 일반적으로 가우시안 필터를 이용하는 것보다 성능이 좋다.
   필터에 대한 정의 부분에서, intensity에 대한 고려가 이루어지는데, 이는 기존의 가우시안 필터와 다른 점이라고 할 수 있다.
   어떠한 픽셀 (i, j)에 접근할 때마다, 커널(i+x, j+y)과 픽셀과의 intensity는 계속 변화하기 때문에, 모든 픽셀에 대해서 필터를 다시 정의해야 하는 번거로움이 있다.

--------------------------------------------------------------------------------------------------------------------------------
   
6. 51p_Adaptive_Tresholding_Using_Moving_Averages
   moving averages를 이용해 적절한 임계값을 구하는 코드이다. 이러한 코드는 글자를 구별하는 상황에서 많이 쓰인다.
   단순한 uniform mean filter를 사용하였으며, 이를 이용해 모든 픽셀에 대해 각각의 임계값을 구한다.
   임계값보다 클 경우 255을, 작을 경우 0을 저장한다. 결과적으로 스캔을 한듯한 이미지를 얻을 수 있다.

7. 52p_K-Means_Clustering
   OpenCV에 내장된 kmeans함수를 이용해 이미지를 segment하는 코드이다. 같은 부분은 같은 색상(intensity)으로 칠해진다.
   이 때, 같은 부분을 결정하는 방법에는 1) intensity만을 고려하는 방법과 2) intensity와 x, y를 고려하는 방법이 있을 수 있다.
   2)는 색상이 같더라도 거리가 멀다면, 같은 분류가 아닐수도 있음을 뜻한다. 결과 분석은 레포트에서 자세히 한다.

8. 53p_Mean_Shift_Segmentation
   기존의 과제 방법이 올바르게 동작하지 않아서 추가로 제공받은 프로그램을 이용하여 이를 수행하였다.
   7번과 유사하게 이미지를 segment를 하는 프로그램이다. 따로 구현해야하는 코드는 없으며, 프로그램을 통해 쉽게 이미지를 segment할 수 있다. 이에 대한 결과는 레포트에 작성한다.
