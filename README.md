avrdude
=======

avrdude with a Linux SPI programmer type

Kevin Cuzner Jun 2013
kevin@kevincuzner.com

Using baud-rate to control SPI frequency

Rui Azevedo (neu-rah) Jun 2013
ruihfazevedo[arroba]gmail.com

How to use avrdue to write firmware to Arduino
----------------------------------------------

### ARDUINO UNO
avrdude -v -c arduino -p m328p -P /dev/ttyACM0 -b 115200

### ARDUINO MEGA 256
avrdude -v -c wiring -p m2560 -P /dev/ttyACM0 -b 115200

### avrdude & roborobo ISP fro Arduino boards (based on avrisp2)

I bought USB/ISP[SR0M-N003] in the RoboRobo web store, but it was not compatible with the avrdude. I changed avrdude source code like this git repo and successfuly read and wrote data from/to arduino boards with that one.
When you write data to arduino boards, you should select the "avrisp2".

I modified the codes like as below in the stk500v2.c file.
```
// original source code
pgm->read_byte = avr_read_byte_default;
pgm->write_byte = avr_write_byte_default;

// modified source code
pgm->read_byte = stk500isp_read_byte;
pgm->write_byte = stk500isp_write_byte;
```

Additional information in Korean
---------------------------------------------

필요하신 분에게 도움이 될 수 있도록 정보 공유합니다.
(제가 USB/ISP[SR0M-N003]의 내부 동작 로직을 알 수 없기에, 문제 없이 완벽히 동작하는 지는 알 수 없습니다...^^)

로보로보에서 구입한 USB/ISP[SR0M-N003]를 이용해서 avrdude로 arduino 보드 등에 code writing을 하려 했는데, 잘 안되더군요. avrdude의 -c 옵션으로 다양한 프로그래머 type를 시도해 보았는데, 그 중에서 avrisp2가 그나마 어느정도 진행하다 오류 메시지를 print하면 진행이 멈추었습니다.

그래서, 오류 및 avrdude 6.1 source code를 분석해보니, avrdude가 USB/ISP[SR0M-N003]와 연결된 타겟 CPU의 device id를 얻기 위해서, 직접 타겟 CPU의 SPI로 command(USB/ISP를 거쳐서)를 보내는데, 이 부분에서 failed to send command 오류 메시지를 출력하면서 진행이 종료됩니다.
(avrdude가 잘 못 된 command를 보내는 것인지, 아니면 USB/ISP[SR0M-N003]에서 그 command를 타겟 CPU로 전달 안하는 것인지는 잘 모르겠습니다)

avrdude 6.1 source code를 보면, avrisp2를 위한 stk500v2.c 파일에 stk500 protocol의 command를 이용하여 타겟 CPU의 device id를 읽어오도록 ISP보드에게 명령할 수 있습니다 (stk500isp_read_byte() 기반)

이 함수를 이용하여 avrdue source 수정 후에 build하니까, avrdude에서 USB/ISP[SR0M-N003]를 avrisp2로 인지하여, 타겟 CPU(arduino UNO의 Atmega328p)에 read/write할 수 있었습니다.

avrdude 6.1 source의 stk500v2.c 파일내에서

stk500v2_initpgm() 함수와 stk600_setup_isp() 함수 내에 있는 아래 원본 source 부분을 수정된 source 부분으로 바꾸어 build하여 생성된 avrdude를 이용하시면 됩니다.
