Open-Software Project (01)
Assignment8 : OSP-Lec12-Backpropagation and MLP

 Python3과 Jupyter Notebook을 사용하였으며 pip 명령어를 이용해 필요한 라이브러리를 설치해 이용하였다.

1. nerual_net.py
 input - fully connected layer - ReLU - fully connected layer - softmax - L2 Loss 형태를 갖는 Two Layer Net을 생성하는 코드이다. 해당 파이썬 코드에서 짠 모델을 바탕으로 이후의 실제 학습이 수행된다.

2.two_later_net.ipynb
 실제 학습이 일어나는 코드이다. 앞서 짠 코드를 바탕으로, 어떠한 변수를 어떠한 방식으로 선택해서 학습을 진행할지를 결정 해야한다. Forward pass 방법으로 score와 loss를 계산한다. 실제 Backward pass 방법으로 데이터들을 한번 학습시킨다. 이 때, 0.27의 정확도를 보이기 때문에 매우 좋지 않은 결과를 도출했다고 할 수 있다. 더 많은 학습을 하기 위해 변수와 값들을 설정해 자신만의 학습을 실행한다. 학습 과정에서 loss에 대한 미분값들을 이용해 W와 b값을 조금씩 조정해준다. 각자만의 값을 갖고 학습을 실행하기 때문에 모두 정확도가 다르게 도출될 것이라고 예상할 수 있다. 이후 test data set을 통해 검증 과정을 거치면 이에 대한 정확도를 얻을 수 있다.