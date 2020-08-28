Open-Software Project (01)
Assignment5 : OSP-Lec06-Edge And Corner Detection

 필자는 Visual Stduio 2019와 Open CV 4.2.13 버전을 사용하였으며, 주어진 skeleton code에서 환경에 맞추기 위해 몇 가지 변수들을 수정하였다.
1. 색상 변환 변수
 : RGB, GRAY, YUV 간의 변환과 관련된 변수(ex.CV_RGB2GRAY, CV_RGB2YUV)를 모두 CV -> COLOR로 바꾸었다.
   CV_RGB2GRAY -> COLOR_RGB2GRAY
2. 이미지 읽기 변수
: IMREAD와 관련하여 이미지를 어떠한 형태로 불러오는지와 관련된 변수(ex. CV_LOAD_IMAGE_COLOR)를 모두 CV_LOAD_IMAGE -> IMREAD로 바꾸었다.
   CV_LOAD_IMAGE_COLOR -> IMREAD_COLOR
3. MINMAX 변수
   normalize 함수 내에서 사용되는 MINMAX 변수를 CV_MINMAX -> NORM_MINMAX로 바꾸었다.

4. #include<stdafx.h>
   stdafx.h 사용을 위해 프로젝트 속성 내의 '미리 컴파일된 헤더'를 사용 설정 옵션으로 바꾸었다.
   이 헤더의 사용을 위해서 프로젝트에 stdafx.cpp와 stdafx.h 파일을 생성했다.

5. TermCriteria 변수
   CV_TERMCRIT_ITER + CV_TERMCRIT_EPS -> TermCriteria::MAX_ITER + TermCriteria::EPS로 바꾸었다.

--------------------------------------------------------------------------------------------------------------------------------

 제출 코드가 많기 때문에 skeleton code 중에서 스스로 구현한 부분에 대해서만 technical report에서 언급하였다. (결과 및 분석은 모두 포함)
따라서 직접 구현하지 않은 부분에 대해서는 Readme.txt에서 간략히 설명한다.

--------------------------------------------------------------------------------------------------------------------------------

1. 40p_Laplacian_of_Gaussian
   LoG를 구현하는 코드이다. 가우시안 필터를 먼저 적용해 이미지의 노이즈를 제거한 후, 라플라시안 필터를 적용해 이미지의 외곽선을 검출한다.
   기존의 라플라시안 필터(가우시안 필터를 적용하지 않는)는 LoG를 적용한 이미지에 비해서 외곽선이 뚜렷하지 않다.
   또한 외곽선이 아님에도 불구하고 격자무늬의 배경이 검출되기도 하였다. 이러한 결과를 보아, LoG가 기존의 라플라시안 필터보다 더 성능이 좋음을 알 수 있다.
   
2. 41p_Canny_Edge_Detector
   Canny 검출기는 이미지의 외곽선을 검출하는 필터이다. OpenCV의 Canny 검출기에서는 가우시안 필터를 적용한 후 소벨 필터를 적용하는 방식을 채택하고 있다.
   이는 Low-pass 필터를 이용해 이미지의 미분(Sx, Sy)를 구하고, High-pass 필터를 처리하는 것이다. 이 과정 속의 핵심은 M(x,y):magnitude와 A(x,y):angle을 구하는 것이다.
   이러한 이론을 바탕으로 제작된 Canny 검출기는 Canny라는 이름의 함수로 OpenCV 내에서 제공된다.

3. 42p_Harri_Corner_Detector
   1, 2번이 이미지의 외곽선을 검출하는 코드라면, Harri Corner 검출기는 이름에서도 알 수 있듯이 코너를 검출하는 코드이다.
   마찬가지로 OpenCV에서 기본적으로 제공하고 있기 때문에 이를 이용해 구현한다. 하지만 제공되는 기본적인 코드는 R값을 이용해서 단순히 처리하기 때문에 어떠한 처리를 통해 성능을 향상 시킬 수 있다.
   그것은 non-maximum suppression을 적용하는 것인데, 이를 통해 주변의 값들과 비교해 코너가 아니라고 생각되는 값은 코너가 아닌 것으로 변경한다.
   또한 OpenCV에서 제공하는 cornerSubPix 함수를 적용해 현재 검출된 코너 값들을 보정해, 더 알맞은 결과를 얻을 수 있다.