Open-Software Project (01)
Assignment5 : OSP-Lec09-Feature Matching and Fitting

 필자는 Visual Stduio 2015와 Open CV 2.4 버전을 사용하였다. 
제공된 소스코드에서 버전이 맞지 않아 변경된 추가적인 변수는 없다.


1. #include<stdafx.h>
   stdafx.h 사용을 위해 프로젝트 속성 내의 '미리 컴파일된 헤더'를 사용 설정 옵션으로 바꾸었다.
   이 헤더의 사용을 위해서 프로젝트에 stdafx.cpp와 stdafx.h 파일을 생성했다.

--------------------------------------------------------------------------------------------------------------------------------

1. 28p_Image_Stictching_using_Affine_Transform_Case1
   아핀 변환을 이용해 이미지를 이어 붙이는 코드이다. 이를 이용하기 위해서는 두 개의 이미지의 매칭 점을 찾아야 하는데, 이는 지난 과제의 SIFT Case3을 사용했다.
   Mx=b의 식을 통해 이용할 매트릭스 x을 구하는 코드도 기존의 HW1에서 구현한 코드를 그대로 이용하였다.
   SIFT를 통해 매칭시킨 점들을 배열에 저장한 후, x를 계산했으나 이상한 그림이 결과로 나왔다.
   자세한 분석은 techincal report에 포함한다.

2. 29p_Image_Stictching_using_Affine_Transform_Case2
   Mx=b 뿐만 아니라, 매칭된 점들 중 일부를 반복적으로 추출해 최적의 subset만을 이용하는 RANSAC을 구현하였다.
   코드 자체의 오류는 발견하지 못하였지만, 제대로된 결과를 출력하지 못했다.

3. 30p_Line_Fitting_using_Hough_Transform_pseudo_code
   Hough Tranform내에서 직선의 start point와 end point(직선의 범위)를 찾기 위한 수도 코드이다.
   자세한 설명은 techincal report에 포함한다.

4. 31p_Line_Fitting_using_Hough_Transform
   OpenCV에서 제공하는 함수를 이용해 Hough Transform을 구현하였다.
   임계값에 따라서 검출 직선이 다양하게 변화하는 것을 확인할 수 있다.