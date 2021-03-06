#include <flags.hpp>

#ifndef OOP
#include <Arduino.h>

float audio_data[500] = {0};
char initial_[3] = {0};

int temp_offset = 0;
int _offset = 0;

// offset must be >= 0
// size of initial must be == 3
// size of _dest must be >= size + offset
int receive_serial_block(float *_dest, char *initial, const int size, const int offset);
void print_and_test(const int flashled_index, const float flashled_value);
void receive_serial_chunk();

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(15);

  digitalWrite(PC13, LOW);
  pinMode(PC13, OUTPUT);
}

void loop() {
  receive_serial_chunk();
  print_and_test(1, 6969);
}

void receive_serial_chunk() {
  temp_offset = receive_serial_block(audio_data, initial_, 10, _offset);

  char mode = initial_[strlen(initial_) - 1];

  if (mode == 'I' || mode == 'A') {
    _offset += temp_offset;
  } else {
    _offset = 0;
    temp_offset = 0;
  }
}

int receive_serial_block(float *_dest, char *initial, const int size, const int offset) {
  char buffer[192] = {'\0'};

SerialAV:
  if (Serial.available() >= 6)
    Serial.readBytesUntil('&', buffer, (size + 6) + (size * 6));
  else
    goto SerialAV;

  if (buffer[0] != '@') {
    return 0;
  }

  for (int i = 0; i < 3; i++) {
    initial[i] = buffer[i + 1];
  }

  for (int i = 0; i < 192 - 4; i++) {
    buffer[i] = buffer[i + 4];
  }

  for (int i = 0; i < 2; i++) {
    if (initial[i] == '_') {
      initial[i] = ' ';
    }
  }

  char *p = initial;
  int len = strlen(p);

  while (isspace(p[len - 1])) p[--len] = 0;
  while (*p && isspace(*p)) ++p, --len;

  memmove(initial, p, len + 1);

  const char *delim = "#";
  char *pch;

  pch = strtok(buffer, delim);

  _dest[0 + offset] = atof(pch);

  int i = 1;
  while (pch != NULL && i <= size) {
    pch = strtok(NULL, delim);
    _dest[i + offset] = atof(pch);
    i++;
  }

  return i - 1;
}

void print_and_test(const int flashled_index, const float flashled_value) {
#ifdef DEBUG
#ifdef INFO_DBG
  Serial.print("Initial String: ");
  Serial.println(initial_);
  Serial.print("Initial Mode: ");
  Serial.println(initial_[strlen(initial_) - 1]);
  Serial.print("Offset: ");
  Serial.println(_offset);
#endif  // INFO_DBG
#ifdef DATA_DBG
  int audio_data_len = round(sizeof(audio_data) / sizeof(audio_data[0]));

  Serial.print("\n{");
  for (int i = 0; i < audio_data_len / 2; i++) {
    Serial.print(audio_data[i]);
    if (i < (audio_data_len / 2) - 1) Serial.print(", ");
  }
  Serial.println("}\n");
#endif  // DATA_DBG
#ifdef FLASH_DBG
  int flash_time = 3000;

  if (audio_data[flashled_index] == flashled_value) {
    for (int i = 0; i < flash_time / 50; i++) {
      digitalWrite(PC13, digitalRead(PC13) ^ 1);
      delay(50);
    }
    digitalWrite(PC13, 0);
  }
#endif  // FLASH_DBG
#endif  // DEBUG
}

#endif // !OOP