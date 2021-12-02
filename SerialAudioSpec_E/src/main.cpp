#include <Arduino.h>

float audio_data[9600] = {0};
char initial_[3] = {0};

//offset must be > 0
//size of initial must be 3
//size of _dest must be > size + offset
int receive_serial_block(float *_dest, char *initial, int size, int offset, bool _cont = 1);

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(25);
  pinMode(PC13, OUTPUT);
}

void loop()
{
  int n = 0;
  receive_serial_block(audio_data, initial_, 32, 0);
  for (int i = 0; i < 192; i++)
  {
    Serial.print(audio_data[i]);
    if (i < 191)
    {
      Serial.print(", ");
    }
    if (audio_data[i] == 25.00f)
    {
      n++;
    }
  }
  Serial.println('\n');
  Serial.print("n: ");
  Serial.println(n);
  Serial.println();

  if (audio_data[1] == 21.24f)
  {
    for (int i = 0; i < 30; i++)
    {
      digitalWrite(PC13, digitalRead(PC13) ^ 1);
      delay(150);
    }
    digitalWrite(PC13, 0);
  }
}

int receive_serial_block(float *_dest, char *initial, int size, int offset, bool _cont)
{
  while (Serial.available() < 6)
  {
  }

  char buffer[192] = {'\0'};
  Serial.readBytesUntil('&', buffer, size + 8 + (size * 4));

  if (buffer[0] != '@')
  {
    return 0;
  }

  for (int i = 0; i < 3; i++)
  {
    initial[i] = buffer[i + 1];
  }

  for (int i = 0; i < 192 - 4; i++)
  {
    buffer[i] = buffer[i + 4];
  }

  char temp = initial[2];
  for (int i = 0; i < 2; i++)
  {
    if (initial[i] == '_')
    {
      initial[i] = ' ';
    }
  }

  char *p = initial;
  int len = strlen(p);

  while (isspace(p[len - 1]))
    p[--len] = 0;
  while (*p && isspace(*p))
    ++p, --len;

  memmove(initial, p, len + 1);

  const char *delim = "#";
  char *pch;

  pch = strtok(buffer, delim);
  _dest[0 + offset] = atof(pch);

  int i = 1;
  while (pch != NULL && i <= size)
  {
    pch = strtok(NULL, delim);
    _dest[i + offset] = atof(pch);
    i++;
  }

  int _offset = 0;
  Serial.println(initial); //@__R#21.24#&
  char mode = initial[strlen(initial) - 1];
  Serial.println(mode);
  if (mode == 'R')
    _cont = 0;

  if (_cont)
  {
    while (mode != 'E')
    {
      _offset += receive_serial_block(_dest, initial, 32, _offset + i - 1, false);
    }
  }

  return i + _offset;
}