#include "arith.h"

void init_output()
{
    current_byte = buffer;
    *current_byte = 0;
    output_mask = 0x80;
}

void output_bit(FILE *output, unsigned _int64 bit)
{
    if (bit)
	{
        *current_byte |= output_mask;
	}
	output_mask >>= 1;
    if (0 == output_mask)
    {
        output_mask = 0x80;
        current_byte++;
        if (current_byte == (buffer + BUF_SZ))
        {
            fwrite(buffer, 1, BUF_SZ, output);
            current_byte = buffer;
        }
        *current_byte = 0;
    }
}

void finish_output(FILE *output)
{
    fwrite(buffer, 1, (size_t)(current_byte - buffer) + 1, output);
    current_byte = buffer;
}

void init_input()
{
    bits_left = 0;
    bytes_left = 1;
    is_eof = 0;
}

unsigned _int64 input_bit(FILE *input)
{
    if ( bits_left == 0 )
    {
        current_byte++;
        bytes_left--;
        bits_left = 8;
        if(bytes_left == 0)
        {
            bytes_left = fread(buffer, 1, BUF_SZ, input);
            if (bytes_left == 0)
            {
                if (!is_eof)
                {
                    is_eof = 1;
                    bytes_left = 2;
				}
                else
                {
					return NULL;
                }
            }
            current_byte = buffer;
        }
    }
    bits_left--;
    return ((*current_byte >> bits_left)&1);
}

