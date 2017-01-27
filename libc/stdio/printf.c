#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

static char* i2a(unsigned int value,int base,char** ps)
{
	int low = value % base;
	int height = value / base;
	if (height > 0) {
		i2a(height,base,ps);
	}
	*(*ps)++ = (low < 10) ?(low + '0'):((low - 10) + 'A');
	return *ps;
}

int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format;

		if ( *(++format) == '%' )
			goto print_c;

		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else if( *format == 'd' || *format == 'i'){
			format++;
			int value = (int)va_arg(parameters,int);
			char buf[32];
			memset(buf,0,32);
			char* p = buf;
			i2a(value,10,&p);
			print(buf,strlen(buf));
		}else if( *format == 'x'){
			format++;
			int value = (int)va_arg(parameters,int);
			char buf[32];
			memset(buf,0,32);
			buf[0] = '0';
			buf[1] = 'x';
			char* p = buf + 2;
			i2a(value,16,&p);
			print(buf,strlen(buf));
		}
		else
		{
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}
