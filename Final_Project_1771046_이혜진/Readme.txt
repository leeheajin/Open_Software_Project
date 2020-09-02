Open-Software Project (01)
Final Project : OSP14-CNN Architecture / OPS15-Encoder-Decoder Architecture

 Python3과 Pycham, Anaconda prompt 환경에서 이를 구현하였다.

1. UNet_Skeleton.py
  ppt 15페이지를 참조해, Unet 모델 구현 코드를 작성 해야하는 파일이다. Unet 구조와 관련된 컨볼루션, 맥스풀 함수등이 작성되어 있다. 특히 컨볼루션 함수의 채널과 관련된 부분이 작성되어 있다. 대략적으로 어떠한 방법으로 Unet 내의 학습이 진행되는지 알 수 있다.

2. resnet_encoder_unet_skeleton.py
  이전 과제의 일부를 참조해 작성한 코드이다. resnet encoder를 사용해 Unet을 구현한 파일이다. Unet과 Resnet에 대한 모든 이해가 필요하다. Unet만을 사용한 코드와는 달리, bridge가 추가된 모습을 확인할 수 있다.

3. modulses_skeleton.py
 14장의 학습 / 테스트 코드를 이해해 작성한 코드이다. 모델과 loss를 불러오고, optimizer와 backpropagation을 실행한다. 해당 파일 내에서 정확도 체크와 loss를 판단하게 된다.

4. main_skeleton.py
 실제 Unet을 실행하는 파일이다. 모델을 로딩하고, 저장하고, 시작하는 등의 모델과 관련된 함수들이 저장되어 있다. 앞선 1, 2, 3 번에서 작성한 코드들을 import를 하여 사용한다.

5. datasets.py
 실제 구현을 하는 부분은 없다. 메인 함수에서 import 되어 사용되는 함수들을 포함하고 있다. Loader과 init 함수의 정보를 담고 있다.