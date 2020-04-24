Open-Software Project (01)
Assignment1 : Lec2 - Image display and geometric transform

 필자는 Visual Stduio 2019와 Open CV 4.2.13 버전을 사용하였으며, 주어진 skeleton code에서 환경에 맞추기 위해 몇 가지 변수들을 수정하였다.
1. 색상 변환 변수
 : RGB, GRAY, YUV 간의 변환과 관련된 변수(ex.CV_RGB2GRAY, CV_RGB2YUV)를 모두 CV -> COLOR로 바꾸었다.
   CV_RGB2GRAY -> COLOR_RGB2GRAY
2. 이미지 읽기 변수
: IMREAD와 관련하여 이미지를 어떠한 형태로 불러오는지와 관련된 변수(ex. CV_LOAD_IMAGE_COLOR)를 모두 CV_LOAD_IMAGE -> IMREAD로 바꾸었다.
   CV_LOAD_IMAGE_COLOR -> IMREAD_COLOR

--------------------------------------------------------------------------------------------------------------------------------

1. 41p_Image_Rotation
  주어진 코드를 이용하여 인물 사진을 45도로 회전시키는 코드이다. 이 때 interpolation시 nearest와 bilinear의 두 가지 방법 중 하나로 사진을 회전시킨다. 코드 내부의 string 값을 변경해 어떠한 방법을 채택해 이미지를 회전시킬 것인지 정할 수 있다. 해당 예시에서는 큰 차이를 보이지는 않지만, nearest와 bilinear 중 비교적 bilinear이 성능이 좋은 것으로 알려져 있다. 이 보다도 성능이 좋은 것이 bicubic이다. 하지만 성능이 좋을수록 계산량이 많고 복잡해진다.


2. 42p_Image_Stitching_using_AffineTransformation
  오른쪽과 왼쪽의 사진을 이용해 2장의 이미지를 자연스럽게 이어 붙이는 코드이다. 이 때 이미지에 겹치는 부분이 있어야 하며, 해당 이미지의 좌표가 변수에 저장된다. 해당 코드에서는 affine transformation matrix가 이용된다. 값을 계산하는 과정에서 inverse warping 방법을 사용해 부드러운 연결을 하고자 하였다. 또한 bilinear interpolation을 사용해 nearest보다 좋은 성능을 보인다. ( +nearest로 구현 시 빈 공간이 생길 수 있음)