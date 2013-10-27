#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


#define BUF_SZ 128
#define AB_SZ 128

struct probability 
{
	char c;
	unsigned _int32 low;
	unsigned _int32 high;
};

typedef struct
{
	unsigned _int32 low;
	unsigned _int32 high;
} symbol;

static char buffer[BUF_SZ + 2]; 
static char *current_byte;             

static unsigned _int32 output_mask;             

static unsigned _int32 bytes_left;           
static unsigned _int32 bits_left;            
static unsigned _int32 is_eof;                   

unsigned _int64 underflow_bits; 

unsigned _int64 Scale;

static unsigned _int32 code;
static unsigned _int32 low; 
static unsigned _int32 high;  

struct probability *Intervals;

//coder
void init_decoder(FILE *input);
void decode_symbol(FILE *input, symbol *sym);
void init_encoder(void);
void encode_symbol(FILE *output, symbol *sym);
void finish_encoding(FILE *output);
unsigned _int32 get_current_number(void);


//arithm
void encode(void);
void decode(void);
void to_symbol( char c, symbol *sym);
char to_char(unsigned _int64 count, symbol *sym );

//IO
unsigned _int64 input_bit( FILE *stream );
void init_output( void );
void output_bit( FILE *stream, unsigned _int64 bit );
void finish_output( FILE *stream );
void init_input( void );
