<HW1_1>

  주어진 코드를 이용하여 인물 사진을 45도로 회전시키는 코드이다. 이 때 interpolation시 nearest와 bilinear의 두 가지 방법 중 하나로 사진을 회전시킨다. 코드 내부의 string 값을 변경해 어떠한 방법을 채택해 이미지를 회전시킬 것인지 정할 수 있다. 해당 예시에서는 큰 차이를 보이지는 않지만, nearest와 bilinear 중 비교적 bilinear이 성능이 좋은 것으로 알려져 있다. 이 보다도 성능이 좋은 것이 bicubic이다. 하지만 성능이 좋을수록 계산량이 많고 복잡해진다.


<HW1_2>
  
  오른쪽과 왼쪽의 사진을 이용해 2장의 이미지를 자연스럽게 이어 붙이는 코드이다. 이 때 이미지에 겹치는 부분이 있어야 하며, 해당 이미지의 좌표가 변수에 저장된다. 해당 코드에서는 affine transformation matrix가 이용된다. 값을 계산하는 과정에서 inverse warping 방법을 사용해 부드러운 연결을 하고자 하였다. 또한 bilinear interpolation을 사용해 nearest보다 좋은 성능을 보인다. ( +nearest로 구현 시 빈 공간이 생길 수 있음)