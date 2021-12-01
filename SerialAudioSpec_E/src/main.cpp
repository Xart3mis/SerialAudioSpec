#include <Arduino.h>

float audio_data[9600] = {0};
char initial[3] = {0};

void receive_serial_block(float *_dest, char *initial, int size);

void setup() { Serial.begin(115200); }

void loop() {

  if (Serial.available() > 6) {
    receive_serial_block(audio_data, initial, 24);
    Serial.println(initial);
  }
}

//@ADR#1#2#3#4#5#6#7#8#9#10#11#12#13#14#15#16#17#18#19#20#21#22#23#24#25#&
void receive_serial_block(float *_dest, char *initial, int size) {
  char buffer[192] = {'\0'};
  Serial.readBytesUntil('&', buffer, size + 8 + (size * 4));

  for (int i = 0; i < 3; i++) {
    initial[i] = buffer[i + 1];
  }

  for (int i = 0; i < 192 - 4; i++) {
    buffer[i] = buffer[i + 4];
  }

  Serial.println(buffer);
}