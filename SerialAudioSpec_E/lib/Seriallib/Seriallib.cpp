#include "Seriallib.hpp"

const initial_t Parser::getInitial() {
  Serial.println(initial_temp);

  return temp_initial_t;
}

int Parser::receive_serial_chunk(float *_dest, const int size, const int offset){
char buffer[192] = {'\0'};

SerialAV:
  if (Serial.available() >= 7)
    Serial.readBytesUntil('&', buffer, 192);
  else
    goto SerialAV;

  if (buffer[0] != '@') {
    return 0;
  }

  for (int i = 0; i < 3; i++) {
    this->initial_temp[i] = buffer[i + 1];
  }

  for (int i = 0; i < 192 - 4; i++) {
    buffer[i] = buffer[i + 4];
  }

  bool trim = 0;
  for (int i = 0; i < 3; i++) {
    if (initial_temp[i] == '_') {
      this->initial_temp[i] = ' ';
      trim = 1;
    }
  }

  if(trim) {
    char *p = initial_temp;
    int len = strlen(p);

    while (isspace(p[len - 1])) p[--len] = 0;
    while (*p && isspace(*p)) ++p, --len;

    memmove(this->initial_temp, p, len + 1);
  }

  const char *delim = "#";
  char *pch;

  pch = strtok(buffer, delim);

  _dest[0 + offset] = atof(pch);

  int i = 1;
  while (pch && i <= size) {
    pch = strtok(NULL, delim);
    _dest[i + offset] = atof(pch);
    i++;
  }

  return i - 1;
}

void Parser::receive_serial_block(float*_dest, int chunk_size){
  temp_offset = receive_serial_chunk(_dest, chunk_size, _offset);
  Serial.println();
  Serial.println(initial_temp);
  char mode = initial_temp[strlen(initial_temp) - 1];

  if (mode == 'I' || mode =='A') {
      this->_offset += temp_offset;
    } else {
      this->_offset = 0;
      this->temp_offset = 0;
  }
}

Parser::Parser(){}
Parser::~Parser(){}