#include "arith.h"

void init_encoder()
{
    low = 0;
    high = 0xffffffff;
    underflow_bits = 0;
}

void encode_symbol(FILE *output, symbol *sym)
{
    unsigned _int64 range;

    range = (unsigned _int64) (high-low) + 1;
    high = low + (unsigned  _int32)((range*sym->high)/Scale - 1);
    low = low + (unsigned  _int32)((range *sym->low)/Scale);

	while (1)
    {
        if ((high & 0x80000000) == (low & 0x80000000))
        {
            output_bit(output, high & 0x80000000 );
            while (underflow_bits > 0)
            {
                output_bit(output, ~high & 0x80000000 );
                underflow_bits--;
            }
        }
        else if ((low & 0x40000000) && !(high & 0x40000000))
        {
            underflow_bits++;
            low &= 0x3fffffff;
            high |= 0x40000000;
        }
        else
        {
			return ;
		}
		low <<= 1;
        high <<= 1;
        high |= 1;
    }
}

void finish_encoding(FILE *output)
{
    output_bit(output, low & 0x40000000);
    underflow_bits++;
    while(underflow_bits-- > 0)
	{
        output_bit(output, ~low & 0x40000000);
	}
}

unsigned _int32 get_current_number()
{
    unsigned _int64 range = 0;
    unsigned _int32 count = 0;

    range = (unsigned _int64) (high - low) + 1;
    count = (unsigned _int32)((((unsigned _int64)(code - low) + 1 )*Scale-1 )/range);
    return count;
}


void init_decoder(FILE *input)
{
    int i;
    code = 0;
    for (i = 0 ; i < 32 ; i++)
    {
        code <<= 1;
        code += input_bit(input);
    }
    low = 0;
    high = 0xffffffff;
}

void decode_symbol(FILE *input, symbol *sym)
{
    unsigned _int64 range = (unsigned _int64)(high - low) + 1;
    high = low + (unsigned _int32)((range*sym->high)/Scale - 1);
    low = low + (unsigned _int32)((range*sym->low)/Scale);

   while(1)
    {
        if((high & 0x80000000) == (low & 0x80000000))
        {
        }
        else if ((low & 0x40000000) == 0x40000000  && (high & 0x40000000) == 0)
        {
            code ^= 0x40000000;
            low  &= 0x3fffffff;
            high |= 0x40000000;
        }
        else
		{
			return;
		}
		low <<= 1;
        high <<= 1;
        high |= 1;
        code <<= 1;
        code += input_bit(input);
    }
}


