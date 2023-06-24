#include "stdlib.h"

u32 strlen(const char *str)
{
	const char *p = str;
	while (*p++)
		;
	return p - str - 1;
}

void assert(bool cond)
{

}

void memset8(u8 *ptr, u8 value, u32 count)
{
	while(count--)
	{
		*ptr++ = value;
	}
}

void memset16(u16 *ptr, u16 value, u32 count)
{
	while(count--)
	{
		*ptr++ = value;
	}
}

void memset32(u32 *ptr, u32 value, u32 count)
{
	while(count--)
	{
		*ptr++ = value;
	}
}

void *memcpy(void *dest, const void *src, u32 count)
{
	u8 *d, *s, *end;

	s = (u8 *)src;
	d = (u8 *)dest;
	end = d + count;

	if(d != s)
	{
		while(d != end)
		{
			*d++ = *s++;
		}
	}

	return dest;
}

void *memmove(void *dest, const void *src, u32 len)
{
	char *d = dest;
	const char *s = src;
	if (d < s)
	{
		while (len--)
		{
			*d++ = *s++;
		}
	}
	else
	{
		const char *lasts = s + (len - 1);
		char *lastd = d + (len - 1);
		while (len--)
		{
			*lastd-- = *lasts--;
		}
	}

	return dest;
}

void strrev(char *str)
{
	u32 i;
	u32 j;
	char a;
	u32 len = strlen((const char *)str);
	for (i = 0, j = len - 1; i < j; ++i, --j)
	{
		a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
}

int itoa(int num, char *str, int base)
{
	int sum = num;
	int digit;
	int i = 0;
	do
	{
		digit = sum % base;
		if (digit < 0xA)
			str[i++] = '0' + digit;
		else
			str[i++] = 'A' + digit - 0xA;
		sum /= base;
	} while (sum);

	str[i] = '\0';
	strrev(str);
	return 0;
}

void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compare)(const void *, const void *))
{
	const char *base0 = (const char *)base;
	const void *p;
	i32 lim, cmp;

	for(lim = nitems; lim; lim >>= 1)
	{
		p = base0 + (lim >> 1) * size;
		cmp = (*compare)(key, p);
		if(!cmp)
		{
			return (void *)p;
		}
		else if(cmp > 0)
		{
			base0 = (const char *)p + size;
			--lim;
		}
	}

	return NULL;
}

i32 codepoint_utf8(i32 codepoint, char *out)
{
	if(codepoint <= 0x7F)
	{
		out[0] = codepoint;
		return 1;
	}
	else if(codepoint <= 0x07FF)
	{
		out[0] = ((codepoint >> 6) & 0x1F) | 0xC0;
		out[1] = (codepoint        & 0x3F) | 0x80;
		return 2;
	}
	else if(codepoint <= 0xFFFF)
	{
		out[0] = ((codepoint >> 12) & 0x0F) | 0xE0;
		out[1] = ((codepoint >>  6) & 0x3F) | 0x80;
		out[2] = (codepoint         & 0x3F) | 0x80;
		return 3;
	}
	else if(codepoint <= 0x10FFFF)
	{
		out[0] = ((codepoint >> 18) & 0x07) | 0xF0;
		out[1] = ((codepoint >> 12) & 0x3F) | 0x80;
		out[2] = ((codepoint >>  6) & 0x3F) | 0x80;
		out[3] = (codepoint         & 0x3F) | 0x80;
		return 4;
	}

	return 0;
}

const char *utf8_codepoint(const char *s, i32 *out)
{
	i32 v;
	if((s[0] & 0xF8) == 0xF0)
	{
		v = ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
		s += 4;
	}
	else if((s[0] & 0xF0) == 0xE0)
	{
		v = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
		s += 3;
	}
	else if((s[0] & 0xE0) == 0xC0)
	{
		v = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
		s += 2;
	}
	else
	{
		v = s[0];
		s += 1;
	}

	*out = v;
	return s;
}

u32 utf8_length(const char *s)
{
	char c;
	u32 length = 0;

	while((c = *s))
	{
		if((c & 0xF8) == 0xF0)
		{
			s += 4;
		}
		else if((c & 0xf0) == 0xE0)
		{
			s += 3;
		}
		else if((c & 0xE0) == 0xC0)
		{
			s += 2;
		}
		else
		{
			s += 1;
		}

		++length;
	}

	return length;
}

u32 rand(void)
{
	/*static u32 x = 3459173429;
	x ^= (x << 21);
	x ^= (x >> 35);
	x ^= (x << 4);
	return x;*/
}

i32 sprintf(char *str, const char *format, ...)
{
	i32 r;
	va_list args;
	va_start(args, format);
	r = vsnprintf(str, UINT32_MAX, format, args);
	va_end(args);
	return r;
}

i32 snprintf(char *str, u32 size, const char *format, ...)
{
	i32 r;
	va_list args;
	va_start(args, format);
	r = vsnprintf(str, size, format, args);
	va_end(args);
	return r;
}

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

/** strnlen (compat implementation) */
static int
my_strnlen(const char* s, int max)
{
	int i;
	for(i=0; i<max; i++)
		if(s[i]==0)
			return i;
	return max;
}

/** print %s */
static void
print_str(char** at, size_t* left, int* ret, char* s,
	int minw, int precision, int prgiven, int minus)
{
	int w;
	/* with prec: no more than x characters from this string, stop at 0 */
	if(prgiven)
		w = my_strnlen(s, precision);
	else	w = (int)strlen(s); /* up to the nul */
	if(w < minw && !minus)
		print_pad(at, left, ret, ' ', minw - w);
	spool_str(at, left, ret, s, w);
	if(w < minw && minus)
		print_pad(at, left, ret, ' ', minw - w);
}

/** print %c */
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

		/* get flags in any order */
		for(;;) {
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
		case 0: break;
		}
	}

	if(left > 0)
		*at = 0;
	return ret;
}
