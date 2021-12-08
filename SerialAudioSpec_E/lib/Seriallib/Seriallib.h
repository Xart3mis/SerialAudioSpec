#ifndef Seriallib

class Parser
{
private:
    int receive_serial_block(float *_dest, char *initial, const int size, const int offset, bool _cont);

public:
    Parser();
    ~Parser();
};
#endif // Seriallib