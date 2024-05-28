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
