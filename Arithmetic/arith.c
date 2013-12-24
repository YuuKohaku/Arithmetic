#include "arith.h"


void main()
{
	char *input = NULL;
	FILE *message = NULL;
	char c;
	int i = 0, j = 0;
	int unique = 0, charcode = 0;
	int counters[AB_SZ] = {0};

	while(1)
	{
	message = fopen("input.txt", "r");
	if (NULL == message)
	{
		fprintf(stderr,  "Unable to open input file" );
		exit(1);
	}
/*
	for(i = 0; i < AB_SZ; i++)
	{
		counters[i]++;
	}
	Scale = AB_SZ-1;
	unique = AB_SZ;*/
	while((charcode = fgetc(message)) != EOF)		 
		{
			counters[charcode]++;
			Scale++;
		}
		Scale++;
		counters[(int)'\0']++;	
		
		for ( i = 0 ; i < AB_SZ ; i++ )
		{
			if( counters[i] != 0 )
			{
				unique++;
			}
		}
		Intervals = (struct probability *)malloc((unique)*sizeof(struct probability));

		j = 0;
		for ( i = 0 ; i < AB_SZ ; i++ )
		{
			if( counters[i] != 0 )
			{
				Intervals[j].c = (char)i;
				Intervals[j].low = low;
				low = low + counters[i];
				Intervals[j].high = low;
				j++;
			} 
		}


		fclose(message);
		encode();
		decode();

		free(Intervals);
		unique = 0;
		low = 0;

		free(input);
		input = NULL;
		Scale = 0;
		memset(counters, 0, sizeof(counters));

		getchar();
	}

}


void encode()
{
	int i = 0, charcode = 0, sz = 0;
	char c;
	symbol s;
	FILE *output = NULL;
	FILE *input = NULL;
	clock_t start, end;
	double diff = 0.0, speed = 0.0;

	input = fopen("input.txt", "r");
	if (NULL == input)
	{
		fprintf(stderr,  "Unable to open input file" );
		exit(1);
	}
	fseek(input, 0L, SEEK_END);
	sz = ftell(input);
	printf("Initial file size: %d bytes\n", sz);
	rewind(input);

	output = fopen("encoded", "wb");
	if (NULL == output)
	{
		fprintf(stderr,  "Unable to open output file" );
		exit(1);
	}

	start = clock();
	init_output();
	init_encoder();
	while((charcode = fgetc(input)) != EOF)		 
	{
		c = (char)charcode;
		to_symbol(c,&s);
		encode_symbol(output, &s);
	}
	c = '\0';
	to_symbol(c,&s);
	encode_symbol(output, &s);
	
	finish_encoding(output);
	finish_output(output);

	end = clock();
	diff = difftime(end, start)/CLOCKS_PER_SEC;
	speed = (double)sz/diff/1024/1024;
	printf("Encoding speed: %f MB/sec\n", speed);
	
	fclose(output);
	fclose(input);
}

void decode()
{
	FILE *input = NULL;
	symbol s;
	char c;
	unsigned _int32 number = 0, sz = 0, enc_sz = 0;
	FILE *output = NULL;
	clock_t start, end;
	double diff = 0.0, speed = 0.0;

	output = fopen("output.txt", "w");
	if (NULL == output)
	{
		fprintf(stderr,  "Unable to open decoding output file" );
		exit(1);
	}

	input = fopen("encoded", "rb");
	if (NULL == input)
	{
		fprintf(stderr,  "Unable to open input file.\n" );
		exit(1);
	}

	
	fseek(input, 0L, SEEK_END);
	enc_sz = ftell(input);
	printf("Encoded file size: %d bytes\n", enc_sz);
	rewind(input);

	start = clock();

	init_input();
	init_decoder(input);
	for ( ; ; )
	{
		number = get_current_number();
		c = to_char(number, &s);
		if ('\0' == c)
		{
			break;
		}
		decode_symbol(input, &s);
		fprintf(output,"%c", c);
	}
	end = clock();

	rewind(output);
	fseek(output, 0L, SEEK_END);
	sz = ftell(output);
	printf("Decoded file size: %d bytes\n", sz);
	printf("Compression: %f\n", 100*(1.0 - (double)enc_sz/sz));
	
	diff = difftime(end, start)/CLOCKS_PER_SEC;
	speed = (double)sz/diff/1024/1024;
	printf("Decoding speed: %f MB/sec\n", speed);

	fclose(output);
	fclose(input);
}

void to_symbol( char c, symbol *sym )
{
	int i = 0;
	for ( ; ; i++)
	{
		if ( c == Intervals[i].c )
		{
			sym->low = Intervals[i].low;
			sym->high = Intervals[i].high;
			return;
		}
	}
}

char to_char(unsigned _int64 count, symbol *sym)
{
	int i = 0;
	for ( ; ; i++)
	{
		if (count >= Intervals[i].low && count < Intervals[i].high)
		{
			sym->low = Intervals[i].low;
			sym->high = Intervals[i].high;
			return Intervals[i].c;
		}
	}
}
