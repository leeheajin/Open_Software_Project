Open-Software Project (01)
Assignment3 : OSP-Lec04-Region-wise-Operation

 필자는 Visual Stduio 2019와 Open CV 4.2.13 버전을 사용하였으며, 주어진 skeleton code에서 환경에 맞추기 위해 몇 가지 변수들을 수정하였다.
1. 색상 변환 변수
 : RGB, GRAY, YUV 간의 변환과 관련된 변수(ex.CV_RGB2GRAY, CV_RGB2YUV)를 모두 CV -> COLOR로 바꾸었다.
   CV_RGB2GRAY -> COLOR_RGB2GRAY
2. 이미지 읽기 변수
: IMREAD와 관련하여 이미지를 어떠한 형태로 불러오는지와 관련된 변수(ex. CV_LOAD_IMAGE_COLOR)를 모두 CV_LOAD_IMAGE -> IMREAD로 바꾸었다.
   CV_LOAD_IMAGE_COLOR -> IMREAD_COLOR

 또한 코드 내에 name space std를 이용하는 cout가 있으나 std가 선언되어 있지 않아 빌드가 되지 않는 문제가 있었다.
따라서 선언부에 using namespace std 를 추가하였다.

--------------------------------------------------------------------------------------------------------------------------------

 제출 코드가 많기 때문에 skeleton code 중에서 스스로 구현한 부분에 대해서만 technical report에서 언급하였다.
또한 39p_Convolution의 경우 수기로 작성해 제출하였기 때문에 추가 설명을 포함하지 않는다.

--------------------------------------------------------------------------------------------------------------------------------

1. 34p_Uniform_Mean_Filter
   mean filter를 구현하는 가장 기본적인 코드이다. 직접 구현하는 부분은 없었으며 zero-paddle, mirroring, adjustkernel의 세 가지 방법이 모두 구현되어 있었다. 
   이를 통해 커널을 적용하는 세 가지 방법에 대해 이해할 수 있다.
   main 함수에 포함된 [ output = meanfilter(input_gray, 3, "mirroring"); //Boundary process: zero-paddle, mirroring, adjustkernel ]
   부분을 통해 커널의 크기 n과 커널을 적용하는 3가지 방법중 한가지를 임의대로 선택할 수 있다.
   해당 과정에서 mirroring 부분의 오류라고 생각되는 부분을 고쳤다. 이는 techincal report에 자세히 기술한다.
   Color의 경우 해당 필터를 RGB 채널에 대해 각각 적용시키는 것을 제외하면 Grayscale과 동일하다.

2. 35p_Gaussian_Filter
   GaussianSkeleton를 이용해 실제 가우시안 필터를 구현하는 코드이다. 1번 과제와 매우 유사하지만 이용하는 필터만 다르다.
   1번 과제는 Uniform filter를 사용하지만, 여기에서는 가우시안 필터를 이용한다. 가우시안 필터를 생성하는 코드는 구현되어 있다.
   따라서 zero-paddle, mirroring, adjustkernel의 방법을 이용한 필터 적용을 1번 과제를 참고하여 작성하면 된다.
   마찬가지로 Color의 경우 해당 필터를 RGB 채널에 대해 각각 적용시키는 것을 제외하면 Grayscale과 동일하다.

3. 36p_Sobel_Filter_Laplacian_Filter
   현재까지 한 개의 필터만을 사용하였다면, sobel filter는 두 개의 필터를 이용한다. 필터 Sx, Sy를 각각 정의한다.
   필터의 3x3으로 고정되어 있어 값이 많지 않기 때문에 직접 대입하는 방식을 이용하였다.
   필터를 적용하는 방식은 mirroring 방식을 이용하였기 때문에, 해당 부분의 구현만 앞선 과제들을 토대로 하면 된다.
   laplacian filter는 위를 다시 한 번 미분한 필터라고 생각하면 된다. 따라서 한 개의 필터만 존재하게 된다.
   sobel filter의 구현과 유사하게 mirroirng 방식을 이용하면 되고, 필터의 값 대입 부분만 수정하면 쉽게 구현이 가능하다.
   sobel color와 laplacian color의 경우 해당 필터를 RGB 채널에 대해 각각 적용시키는 것을 제외하면 Grayscale과 동일하다.
   
4. 37p_Gaussian_Filter_Sep
   2번 과제에서 구현한 코드를 separate하는 코드이다. 가우시안 필터는 separable하므로 Lec4_21p와 Lec4_24p를 참고해 필터를 분리한다.
   input 이미지의 모든 픽셀에 대해 수평차원으로 필터를 적용한 다음, 이 결과 이미지를 다시 수직차원으로 필터를 적용한다.
   그렇게 하면 2번 과제와 같은 가우시안 필터를 적용한 이미지가 출력된다. 놀랍게도 시간이 매우 단축되는 결과를 얻었다.
   해당 결과에 대한 분석은 technical report에 자세히 기술한다.

5. 38p_Unsharp_Masking
   Mean filter로 가우시안 필터를 이용한 후, sobel filter와 laplacian filter를 구현하는 방법이다.
   맨 마지막에 해당 필터들을 적용하기 전, unshap 된 이미지의 상태를 보면 이미지가 비교적 선명해지는 것을 알 수 있다.
   선명해진 이미지를 갖고 다시 sobel filter 혹은 laplacian filter를 적용한다.
   마찬가지로 Color의 경우 해당 필터를 RGB 채널에 대해 각각 적용시키는 것을 제외하면 Grayscale과 동일하다.

6. 39p_Convolution
   The report homework should be done by hand, NOT using C/C++, so i include the scanned picture in to the report