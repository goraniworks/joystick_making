# 포텐시오미터와 아두이노우노를 이용한 조이스틱 만들기

## 개요
이 프로젝트에서는 포텐시오미터와 아두이노 우노를 이용하여 가상 조이스틱을 만드는 방법을 소개합니다. 아두이노로부터 데이터를 읽어 파이썬을 통해 가상 조이스틱을 제어합니다.

## 준비물
- 아두이노 우노
- 포텐시오미터
- 점퍼 케이블
- 브레드보드
- USB 케이블
- 컴퓨터
- Arduino IDE
- Python 및 필요한 라이브러리 (`pyserial`, `pyvjoy`)
- vJoy 드라이버

## 하드웨어 연결

1. 포텐시오미터를 브레드보드에 연결합니다.
2. 포텐시오미터의 중간 핀을 아두이노의 아날로그 핀 A0에 연결합니다.
3. 포텐시오미터의 나머지 두 핀을 각각 5V와 GND에 연결합니다.
4. 아두이노를 USB 케이블을 이용해 컴퓨터에 연결합니다.

## 아두이노 코드 작성 및 업로드

아래 코드를 Arduino IDE에 복사하고 아두이노에 업로드합니다.

```cpp
#include "AlignedJoy.h"

// 포텐시오미터가 연결된 아날로그 핀 지정
const uint8_t potPin = A0;

// AlignedJoy 객체 생성, Y 핀은 사용하지 않으므로 임의의 핀 번호를 할당 (예: A1)
AlignedJoy racingWheel(potPin, A1);

void setup() {
  Serial.begin(9600);
  
  // 포텐시오미터의 높은 저항값을 고려하여 적절한 시간 지연을 추가
  delay(500); // 초기 설정에 필요한 추가 시간을 제공

  // 중앙 칼리브레이션 수행 시간을 증가
  racingWheel.middleCalibration(2000);  // 2초 동안 중앙값을 자동으로 설정
  
  // 극단 칼리브레이션 수행 시간을 증가
  racingWheel.axesCalibration(4000);  // 4초 동안 극단값을 자동으로 설정
}

void loop() {
  // X축 값을 안정적으로 읽기 위한 노이즈 감소를 위해 평균값 사용
  int averageValue = 0;
  for (int i = 0; i < 10; i++) {
    averageValue += racingWheel.read(X);
    delay(10); // 각 읽기 사이에 충분한 지연 시간을 제공
  }
  averageValue /= 10;

  // X축 값을 읽어서 처리 (0~1023 범위의 값을 0~255 범위로 매핑)
  uint16_t xValue = map(averageValue, 0, 1023, 0, 255);

  // 디버그를 위해 시리얼 모니터에 값 출력
  Serial.println(xValue);
  
  delay(100);
}
```

## vJoy 드라이버 설치 및 설정

1. **vJoy 다운로드 및 설치**:
   - [vJoy 다운로드 페이지](http://vjoystick.sourceforge.net/site/index.php/download-a-install)에서 설치 파일을 다운로드합니다.
   - 설치 파일을 실행하고 지시에 따라 설치를 완료합니다.

2. **vJoy 설정**:
   - vJoy Configurator를 실행합니다.
   - "General" 탭에서 "Number of Axes"와 "Number of Buttons"를 설정합니다.
   - "Enable vJoy" 버튼을 클릭하여 가상 조이스틱을 활성화합니다.

## 파이썬 코드 작성 및 실행

아래 파이썬 코드를 사용하여 아두이노에서 전송한 데이터를 읽고 가상 조이스틱에 적용합니다.

```python
import serial
import pyvjoy

# 시리얼 포트와 통신 속도 설정
ser = serial.Serial('COM4', 9600, timeout=1)

# 가상 조이스틱 초기화
vj = pyvjoy.VJoyDevice(1)

def read_joystick_data():
    """
    시리얼 포트에서 숫자를 읽어 조이스틱의 X축 데이터로 변환
    """
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        try:
            x_value = int(line)
            return x_value
        except ValueError:
            return None
    return None

try:
    while True:
        x_value = read_joystick_data()
        if x_value is not None:
            print(f"X: {x_value}")
            
            # X 값을 0-32767 범위로 스케일링 (0-255 범위에서)
            x_val = int(x_value * 32767 / 255)
            vj.data.wAxisX = x_val
            vj.update()
except KeyboardInterrupt:
    print("종료합니다.")
finally:
    ser.close()
```

### 라이브러리 설치
아래 명령어를 사용하여 필요한 파이썬 라이브러리를 설치합니다.

```sh
pip install pyserial pyvjoy
```

## 실행 방법

1. 아두이노 코드가 업로드된 상태에서 아두이노를 컴퓨터에 연결합니다.
2. 파이썬 코드를 실행하여 시리얼 포트에서 데이터를 읽고 가상 조이스틱을 제어합니다.
3. 포텐시오미터를 돌리면 해당 값이 가상 조이스틱의 X축으로 전송됩니다.

이제 포텐시오미터를 통해 가상 조이스틱의 X축을 제어할 수 있습니다. 이 프로젝트는 다양한 입력 장치와의 상호작용을 배우는 훌륭한 기회가 될 것입니다.
