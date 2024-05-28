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
