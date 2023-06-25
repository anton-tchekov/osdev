#include "vsnprintf.h"
#include "strlen.h"

static void print_pad(char** at, size_t* left, int* ret, char p, int num)
{
	while(num--)
	{
		if(*left > 1)
		{
			*(*at)++ = p;
			(*left)--;
		}

		(*ret)++;
	}
}

static char get_negsign(int negative, int plus, int space)
{
	if(negative)
		return '-';
	if(plus)
		return '+';
	if(space)
		return ' ';
	return 0;
}

#define PRINT_DEC_BUFSZ 32

static int print_dec(char* buf, int max, unsigned int value)
{
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = '0' + value % 10;
		value /= 10;
	}

	return i;
}

static int print_hex(char* buf, int max, unsigned int value)
{
	const char* h = "0123456789abcdef";
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = h[value & 0x0f];
		value >>= 4;
	}
	return i;
}

static void
spool_str_rev(char** at, size_t* left, int* ret, const char* buf, int len)
{
	int i = len;
	while(i) {
		if(*left > 1) {
			*(*at)++ = buf[--i];
			(*left)--;
		} else --i;
		(*ret)++;
	}
}

static void
spool_str(char** at, size_t* left, int* ret, const char* buf, int len)
{
	int i;
	for(i=0; i<len; i++) {
		if(*left > 1) {
			*(*at)++ = buf[i];
			(*left)--;
		}
		(*ret)++;
	}
}

static void
print_num(char** at, size_t* left, int* ret, int minw, int precision,
	int prgiven, int zeropad, int minus, int plus, int space,
	int zero, int negative, char* buf, int len)
{
	int w = len;
	char s = get_negsign(negative, plus, space);
	if(minus) {
		int numw = w;
		if(precision == 0 && zero) numw = 0;
		if(numw < precision) numw = precision;
		if(s) numw++;

		if(s) print_pad(at, left, ret, s, 1);

		if(precision == 0 && zero) {
		} else {
			if(w < precision)
				print_pad(at, left, ret, '0', precision - w);
			spool_str_rev(at, left, ret, buf, len);
		}

		if(numw < minw)
			print_pad(at, left, ret, ' ', minw - numw);
	} else {
		int numw = w;
		if(precision == 0 && zero) numw = 0;
		if(numw < precision) numw = precision;
		if(!prgiven && zeropad && numw < minw) numw = minw;
		else if(s) numw++;

		if(numw < minw)
			print_pad(at, left, ret, ' ', minw - numw);
		if(s) {
			print_pad(at, left, ret, s, 1);
			numw--;
		}
		if(w < numw)
			print_pad(at, left, ret, '0', numw - w);
		if(precision == 0 && zero)
			return;
		spool_str_rev(at, left, ret, buf, len);
	}
}

static void
print_num_d(char** at, size_t* left, int* ret, int value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = (value < 0);
	int zero = (value == 0);
	int len = print_dec(buf, (int)sizeof(buf),
		(unsigned int)(negative?-value:value));
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

static void
print_num_u(char** at, size_t* left, int* ret, unsigned int value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_dec(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

static void
print_num_x(char** at, size_t* left, int* ret, unsigned int value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_hex(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}


static void
print_str(char** at, size_t* left, int* ret, char* s,
	int minw, int precision, int prgiven, int minus)
{
	int w;
	/* with prec: no more than x characters from this string, stop at 0 */
	if(prgiven)
		w = strnlen(s, precision);
	else	w = (int)strlen(s); /* up to the nul */
	if(w < minw && !minus)
		print_pad(at, left, ret, ' ', minw - w);
	spool_str(at, left, ret, s, w);
	if(w < minw && minus)
		print_pad(at, left, ret, ' ', minw - w);
}

static void
print_char(char** at, size_t* left, int* ret, int c,
	int minw, int minus)
{
	if(1 < minw && !minus)
		print_pad(at, left, ret, ' ', minw - 1);
	print_pad(at, left, ret, c, 1);
	if(1 < minw && minus)
		print_pad(at, left, ret, ' ', minw - 1);
}

i32 vsnprintf(char *str, u32 size, const char *format, va_list arg)
{
	char* at = str;
	size_t left = size;
	int ret = 0;
	const char* fmt = format;
	int conv, minw, precision, prgiven, zeropad, minus, plus, space;
	while(*fmt)
	{
		while(*fmt && *fmt!='%') {
			if(left > 1) {
				*at++ = *fmt++;
				left--;
			} else fmt++;
			ret++;
		}

		if(!*fmt) break;

		fmt++;
		minw = 0;
		precision = 1;
		prgiven = 0;
		zeropad = 0;
		minus = 0;
		plus = 0;
		space = 0;

		for(;;)
		{
			if(*fmt == '0')
				zeropad = 1;
			else if(*fmt == '-')
				minus = 1;
			else if(*fmt == '+')
				plus = 1;
			else if(*fmt == ' ')
				space = 1;
			else break;
			fmt++;
		}

		/* field width */
		if(*fmt == '*') {
			fmt++; /* skip char */
			minw = va_arg(arg, int);
			if(minw < 0) {
				minus = 1;
				minw = -minw;
			}
		} else while(*fmt >= '0' && *fmt <= '9') {
			minw = minw*10 + (*fmt++)-'0';
		}

		/* precision */
		if(*fmt == '.') {
			fmt++; /* skip period */
			prgiven = 1;
			precision = 0;
			if(*fmt == '*') {
				fmt++; /* skip char */
				precision = va_arg(arg, int);
				if(precision < 0)
					precision = 0;
			} else while(*fmt >= '0' && *fmt <= '9') {
				precision = precision*10 + (*fmt++)-'0';
			}
		}


		if(!*fmt) conv = 0;
		else conv = *fmt++;

		switch(conv)
		{
		case 'd':
			print_num_d(&at, &left, &ret, va_arg(arg, int),
			minw, precision, prgiven, zeropad, minus, plus, space);
			break;

		case 'u':
			print_num_u(&at, &left, &ret,
			va_arg(arg, unsigned int),
			minw, precision, prgiven, zeropad, minus, plus, space);
			break;

		case 'x':
			print_num_x(&at, &left, &ret,
			va_arg(arg, unsigned int),
			minw, precision, prgiven, zeropad, minus, plus, space);
			break;

		case 's':
			print_str(&at, &left, &ret, va_arg(arg, char*),
				minw, precision, prgiven, minus);
			break;

		case 'c':
			print_char(&at, &left, &ret, va_arg(arg, int),
				minw, minus);
			break;

		case '%':
			print_pad(&at, &left, &ret, '%', 1);
			break;

		default:
			break;
		}
	}

	if(left > 0)
	{
		*at = 0;
	}

	return ret;
}
