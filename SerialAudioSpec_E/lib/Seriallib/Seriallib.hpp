#ifndef Seriallib

#include "Arduino.h"

typedef struct {
  char str[3] = {0};
  char mode = 0;
} initial_t;

class Parser {
 private:
  char initial_temp[3] = {0};
  initial_t temp_initial_t;

  int receive_serial_chunk(float *_dest, const int size, const int offset);

  int temp_offset = 0;
  int _offset = 0;

 public:
  const initial_t getInitial();
  void receive_serial_block(float*_dest, int chunk_size);

  Parser();
  ~Parser();
};
#endif  // Seriallib