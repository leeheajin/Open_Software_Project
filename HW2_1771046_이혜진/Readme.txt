Open-Software Project (01)
Assignment2 : 03-Pixel

 필자는 Visual Stduio 2019와 Open CV 4.2.13 버전을 사용하였으며, 주어진 skeleton code에서 환경에 맞추기 위해 몇 가지 변수들을 수정하였다.
1. 색상 변환 변수
 : RGB, GRAY, YUV 간의 변환과 관련된 변수(ex.CV_RGB2GRAY, CV_RGB2YUV)를 모두 CV -> COLOR로 바꾸었다.
   CV_RGB2GRAY -> COLOR_RGB2GRAY
2. 이미지 읽기 변수
: IMREAD와 관련하여 이미지를 어떠한 형태로 불러오는지와 관련된 변수(ex. CV_LOAD_IMAGE_COLOR)를 모두 CV_LOAD_IMAGE -> IMREAD로 바꾸었다.
   CV_LOAD_IMAGE_COLOR -> IMREAD_COLOR

--------------------------------------------------------------------------------------------------------------------------------

 제출 코드가 많기 때문에 skeleton code 중에서 스스로 구현한 부분에 대해서만 technical report에서 언급하였다.
또한 모든 histogram은 Excel 프로그램을 통해 작성하였으며, 이에 대한 캡처본과 파일을 모두 제출하였다.
히스토그램과 결과(result)에 대한 설명은 technical report에 포함하였다.

--------------------------------------------------------------------------------------------------------------------------------

1. 34p_PDF_CDF_Generation
   스스로 구현할 부분은 없었으며 실제 PDF와 CDF 계산 함수를 포함한 프로그램이다.
   엑셀 프로그램을 통해 히스토그램을 확인할 수 있다.
   PDF가 한 지점에서의 값을 의미한다면 CDF는 이 값들이 누적된 것을 의미한다.
   총 픽셀의 수로 나누는 과정이 포함되어 있기 때문에, PDF의 총 면적은 1이 된다.
   
2. 35p_Histogram_Stretching
   stretching 방식을 이용해 이미지의 대비를 높여 이미지를 보다 보기 좋게 만드는 프로그램이다.
   스스로 구현할 부분은 없었으며 실제 stretching이 어떻게 작동하는지를 알 수 있다.
   히스토그램의 변화를 통해 어떻게 intensity의 분포가 변화하는지 알 수 있다.

3. 37p_Histogram_Equalization_gray
   eqaulization방식을 이용해 이미지의 대비를 높여 gray 이미지를 보다 보기 좋게 만드는 프로그램이다.
   필요한 함수는 모두 hist_func.h 헤더 파일 내에 구현되어 있다.

4. 39p_Histogram_Equalization_RGB
   eqaulization방식을 이용해 이미지의 대비를 높여 RGB 이미지를 보다 보기 좋게 만드는 프로그램이다.
   hist_func.h 헤더파일의 RGB부분을 직접 코딩해야 한다.
   RGB 값은 각각 독립적인 값이 아님에도 불구하고 각각의 레이어에 따로 equalization을 적용하다 보니, color distortion이 발생한다.

5. 41p_Histogram_Equalization_YUV
   4번의 업그레이드 버전으로 RGB를 YUV로 바꾼 후 처리하고 다시 RGB로 변환하는 프로그램이다.
   YUV의 Y만이 intensity에 영향을 주기 때문에 0번째 레이어인 Y에만 equalization을 적용한다.
   분할, equalizaiotn, 통합 과정을 거치면 대비가 높아진 이미지를 얻을 수 있다.

6. 43p_Histogram_Matching_gray
   matching방식을 이용해 이미지의 대비를 높여 gray 이미지를 보다 보기 좋게 만드는 프로그램이다.
   주어진 skeleton code가 없으며 스스로 전체 코드를 구현 해야한다.
   하지만 hist_func.h 헤더파일과 eqalization 코드 파일들이 중요한 단서들이 된다.
   더하여 mathcing 함수를 찾는 것이 관건이라고 할 수 있는데, 역함수를 어떻게 처리하냐에 따라서 이미지와 matching 함수가 달라질 수 있다.

7. 44p_Histogram_Matching_RGB
   matching방식을 이용해 이미지의 대비를 높여 RGB 이미지를 보다 보기 좋게 만드는 프로그램이다.
   주어진 skeleton code는 없지만, 6의 업그레이드 버전이라고 생각하고 RGB와 YUV에 대한 처리만 해주면 된다.
   마찬가지로 matching 함수를 찾는 것이 포인트이다.
