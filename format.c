/* format.c - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */

#define BNI_SIGN_MASK (1<<31)
#define BNI_F32_EXP_MASK 0x7F800000
#define BNI_F32_SIGNIFICAND_MASK 0x007FFFFF
#define BNI_F32_TO_BITPATTERN(val) (*(unsigned int*)(&(val)))
#define BNI_F32_SIGNIFICAND_BITS(val) \
	 (BNI_F32_TO_BITPATTERN(val) & BNI_F32_SIGNIFICAND_MASK)
#define BNI_F32_EXP_STORED(val) \
	 (unsigned char)((BNI_F32_TO_BITPATTERN(val)&BNI_F32_EXP_MASK)>>23)
#define BNI_F32_EXP_REAL(val) (char)(BNI_F32_EXP_STORED(val) - 127)
#define BNI_F32_INF_POS 0x7F800000
#define BNI_F32_INF_NEG 0xFF800000
#define BNI_F32_IS_NAN(val) ((BNI_F32_TO_BITPATTERN(val) & BNI_F32_INF_POS) \
                && (BNI_F32_TO_BITPATTERN(val) & BNI_F32_SIGNIFICAND_MASK))
#define BNI_BASE_2_TO_10 0.30103f
#define BNI_DIGITS_PER_ITERATION 8
#define BNI_F32_IS_ZERO(val) (((*(u32*)(&(val))) & 0x7FFFFFFF) == 0)
#define BNI_F32_MAX_SIGNIFICAT_DIGITS 10

#define BNI_FA_E_OR_G(fa) \
	((((fa)->format_info) & BNI_FA_SIENTIFIC) \
	 || (((fa)->format_info) & BNI_FA_EXP))
#define BNI_FAT_IS_STRING(fa) \
	((((fa)->format_info) & BNI_FAT_STR) == BNI_FAT_STR)
#define BNI_FAT_IS_CSTRING(fa) \
	((((fa)->format_info) & BNI_FAT_CSTR) == BNI_FAT_CSTR)
#define BNI_FAT_IS_POINTER(fa) \
	((((fa)->format_info) & BNI_FAT_PTR) == BNI_FAT_PTR)
#define BNI_FAT_IS_SIGNED(fa) \
	((((fa)->format_info) & BNI_FAT_SIGNED) >  0)
#define BNI_FAT_IS_INTEGER(fa) \
	((((fa)->format_info) & (BNI_FAT_UNSIGNED | BNI_FAT_SIGNED)) >  0)
#define BNI_FAT_IS_FLOAT(fa) \
	((((fa)->format_info) & (BNI_FAT_F32 | BNI_FAT_F64)) >  0)

#define BNI_FA_HAS_HEX(fa) \
	((((fa)->format_info) & BNI_FA_HEX) == BNI_FA_HEX)
#define BNI_FA_HAS_PLUS(fa) \
	((((fa)->format_info) & BNI_FA_PLUS) == BNI_FA_PLUS)
#define BNI_FA_HAS_PAD(fa) \
	((((fa)->format_info) & BNI_FA_SPACE | BNI_FA_PAD_ZERO) > 0)
#define BNI_FA_HAS_SPACE(fa) \
	((((fa)->format_info) & BNI_FA_SPACE) == BNI_FA_SPACE)
#define BNI_FA_HAS_PAD_ZERO(fa) \
	((((fa)->format_info) & BNI_FA_PAD_ZERO) == BNI_FA_PAD_ZERO)
#define BNI_FA_HAS_JUSITIFY_LEFT(fa) \
	((((fa)->format_info) & BNI_FA_JUSTIFY_LEFT) == BNI_FA_JUSTIFY_LEFT)

typedef struct BniInternalFormatBuffer
{
	u8  mem[32];
	u16 precision;
	u16 width;
	i8  exp;
	i16 left_part_zeros;
	i16 right_part_zeros;
	u8 sd_div;
	u8 sd_end;
	u8 sd_start;
	u8 significant_digits;
	i8 sign;
	i8 pad_char;
	i8 pad_count;
	u16 output_len;
} BniInternaFormatBuffer;

internal u32
bni_format_float
(
	u8 *out
	, umm size
	, BniStringFormatArg *arg
)
{
	u32 written = 0;
	u32 dot_space = 1;
	bool nan = false;
	BniInternaFormatBuffer buf;
	memset(&buf, 255, sizeof(BniInternaFormatBuffer));
	buf.precision = (u16)((arg->format_info & BNI_FAM_PRECISION)>>16);
	buf.width = arg->format_info & BNI_FAM_WIDTH;
	f32 val = arg->v.f;
	bool output_zero = false;

	assert((umm)buf.width <= size);

	i16 exp_b2 = BNI_F32_EXP_REAL(val);
	u32 bits = BNI_F32_SIGNIFICAND_BITS(val);
	if (buf.precision == 0)
		dot_space = 0;

	if (exp_b2 == -127) {
		// stored exp == 0
		if (bits == 0) {
			//NOTE(bjorn): 0.0f -- output zeros according to format
			// no further processing needed.
			if (BNI_FA_E_OR_G(arg)) {
				// 0.[precision]e+00
				buf.output_len = buf.precision + 5 + dot_space;
				buf.significant_digits = 0;

			}
			else {
				// 0.[precision]
				buf.output_len = buf.precision + 1 + dot_space;
				buf.significant_digits = 0;
			}
			output_zero = true;
		}
		else {
			// NOTE(bjorn): subnormal number. Adapt binary exponent
			// to what it would have been if it where a normal number
			// -150 < exp_b2 < -127 on completion
			i32 count = 23;
			for (;bits != 0
				; bits >>= 1, --count)
			{
				;
			}
			exp_b2 -= count + 1; // 23 == sigificand bit count
		}
	}
	else if (exp_b2 == -128) { 
		// stored exp == 255
		if (bits == 0) {
			memcpy(buf.mem, "+INF", 4);
			buf.output_len = 4;
			if (BNI_F32_TO_BITPATTERN(val) & BNI_SIGN_MASK) {
				*buf.mem = '-';
				val = (float)(BNI_F32_TO_BITPATTERN(val) & (~BNI_SIGN_MASK));
			}
		}
		else {
			memcpy(buf.mem, "NAN", 3);
			buf.output_len = 3;
		}
		nan = true;
	}

	if (!nan && !output_zero) {

		buf.exp = (i16)((f32)exp_b2 * BNI_BASE_2_TO_10 + 0.5f);

		i32 digits_to_generate = 0;
		if (BNI_FA_E_OR_G(arg)) {
			digits_to_generate = buf.precision + 6; // 1.[precision]e+xx
			buf.output_len = buf.precision + 5 + dot_space;
			buf.significant_digits = buf.precision + 1; // %g
			buf.sd_div = 1;
		}
		else { 
			if (buf.exp < 0 && buf.precision < -buf.exp) {
				// NOTE(Bjorn): Not enough precision output zero as 0.[precision]
				buf.output_len = buf.precision + 1 + dot_space;
				buf.significant_digits = 0;
				digits_to_generate = 0;
			}
			else {
				buf.output_len = buf.precision + buf.exp + 1 + dot_space;
				buf.significant_digits = buf.precision + buf.exp + 1;
				digits_to_generate = buf.precision + buf.exp
									+ BNI_F32_MAX_SIGNIFICAT_DIGITS;
			}
		}

		// x.yz...e+38 / 10e+36 = x.yz...e+02
		// x.yz...e-38 * 10e+40 = x.yz...e+02
		f32 scaled = val;
		if (scaled < 0)
			scaled = -scaled;

		if (buf.exp > 0) {
			// NOTE(bjorn): As we aim for 10 digits make sure
			// we get 2 in the first iteration an 8 in the second
			// leading zeros are free while trailing zeros costs
			// a division
			i8 scale_exp = buf.exp - 2;
			if (scale_exp < 0) {
				scale_exp = 0;
			}
			buf.exp = scale_exp;
			f32 scale_factor = 1.0f;
			f32 base = 10.0f;
			while (scale_exp) {
				if (scale_exp & 1)
					scale_factor *= base;
				base *= base;
				scale_exp >>= 1;
			}
			scaled /= scale_factor;
		}
		else if (buf.exp <= 0) {
			i8 scale_exp = (-buf.exp + 2);
			buf.exp = -scale_exp;
			if (scale_exp & 0x20)
				scaled *= 1e32L;
			if (scale_exp & 0x10)
				scaled *= 1e16L;
			if (scale_exp & 0x08)
				scaled *= 1e8L;
			if (scale_exp & 0x04)
				scaled *= 1e4L;
			if (scale_exp & 0x02)
				scaled *= 1e2L;
			if (scale_exp & 0x01)
				scaled *= 1e1L;
		}

		u32 at = 1;
		u32 digits_generated = 0;
		if (digits_to_generate > 0) {
			// NOTE(bjorn): Output at most 10 digits 1 more that the 6-9
			// digits of precision in a float the extra digit is for rounding
			if (digits_to_generate > BNI_F32_MAX_SIGNIFICAT_DIGITS)
				digits_to_generate = BNI_F32_MAX_SIGNIFICAT_DIGITS;

			*buf.mem = '0';

			while( 0 < digits_to_generate && 0.0f < scaled) {
				unsigned int integral_part = (unsigned int)scaled;

				digits_to_generate -= BNI_DIGITS_PER_ITERATION;
				if (0 < digits_to_generate)
					scaled = (scaled - (float)integral_part) * 1e8f;

				i32 n = BNI_DIGITS_PER_ITERATION;
				u8 *p = buf.mem + at + BNI_DIGITS_PER_ITERATION;
				for(;n > 0 && 0 < integral_part ; --n) {
					*--p = '0' + integral_part % 10;
					integral_part /= 10;
				}
				while(--n >= 0)
					*--p = '0';

				at += BNI_DIGITS_PER_ITERATION;
			}
			digits_generated = at - 1;
		}
		if (digits_generated > 0) {
			// NOTE(bjorn): Undo the exponent scaling, account for
			// any leading zeros introduced by the conversion
			buf.exp += BNI_DIGITS_PER_ITERATION-1;


			// NOTE(bjorn): Find first significant digit
			// and adjust exponent to reflect the change
			u8 *p = &buf.mem[1];
			while(*p == '0') {
				--digits_generated;
				--buf.exp;
				++p;
			}

			// Clamp significant digits to digits_generated
			// and round if we have generated more digits
			// than we need
			if (digits_generated < buf.significant_digits)
				buf.significant_digits = digits_generated;
			else if (p[buf.significant_digits] >= '5') {
				i8 n = buf.significant_digits -1;
				while(p[n] == '9') {
					--buf.significant_digits;
					--n;
				}
				++p[n];
			}

			buf.sd_start = at - digits_generated;
		}
	}

	if (BNI_FA_HAS_PLUS(arg) || val < 0) {
		++buf.output_len;
		buf.sign = val < 0 ? '-' : '+';
	}
	else if (BNI_FA_HAS_SPACE(arg)) {
		++buf.output_len;
		buf.sign = ' ';
	}
	else {
		buf.sign = 'x';
	}

	u32 at = 0;
	buf.pad_count = 0;
	if (BNI_FA_HAS_PAD(arg) && buf.width > buf.output_len) {
		buf.pad_char = ' ';
		if (BNI_FA_HAS_PAD_ZERO(arg)) {
			buf.pad_char = '0';
		}
		buf.pad_count = buf.width - buf.output_len;
		if (!BNI_FA_HAS_JUSITIFY_LEFT(arg)) {
			if (BNI_FA_HAS_PAD_ZERO(arg) && buf.sign != 'x') {
				*out = buf.sign;
				++at;
			}
			memset(out, buf.pad_char, buf.pad_count);
			at += buf.pad_count;
		}
	}

	if (nan) {
		memcpy(out + at, buf.mem, buf.output_len);
	}
	else {
		if (buf.sign != 'x')
		{
				*out = buf.sign;
				++at;
		}

		if (buf.significant_digits == 0) {
			//NOTE (bjorn): output 0 correctly
			if (BNI_FA_E_OR_G(arg)) {
				memset(out + at, '0', buf.output_len - 4);
				memcpy(out + at + buf.output_len - 4, "e+00", 4);
			}
			else {
				memset(out + at, '0', buf.output_len);
			}

			if (dot_space == 1)
				out[at + 1] = '.';

			at += buf.output_len;
		}
		else {
			u8 *d = out + at;
			u8 *s = buf.mem + buf.sd_start;
			if (BNI_FA_E_OR_G(arg)) {
				i8 exp_sign = '+';
				if (buf.exp < 0) {
					buf.exp = -1*buf.exp;
					exp_sign = '-';
				}

				// x.
				*d++ = *s++;
				if (dot_space == 1)
					*d++ = '.';

				buf.right_part_zeros = buf.precision - (buf.significant_digits - 1);
				if (buf.right_part_zeros > 0) {
					buf.sd_end = buf.significant_digits;
				}
				else {
					buf.right_part_zeros = 0;
					buf.sd_end = buf.precision + 1;
				}

				i32 n;
				for (n = buf.sd_div; n < buf.sd_end; ++n) {
					*d++ = *s++;
				}
				memset(d, '0', buf.right_part_zeros);
				d += buf.right_part_zeros;
				*d++ = 'e';
				*d++ = exp_sign;

				d += 2;
				for(n = 2
					; n > 0 && 0 < buf.exp
					; --n) {
					*--d = '0' + buf.exp % 10;
					buf.exp /= 10;
				}
				while (--n >= 0)
					*--d = '0';
				//d += 2;
				at = d + 3 - out;
			}
			else {
				// %f
				if (buf.exp > 0) {
					buf.left_part_zeros = buf.exp + 1 - buf.significant_digits;
					if (buf.left_part_zeros > 0) {
						buf.sd_div = buf.significant_digits;
						buf.right_part_zeros = buf.precision;
					}
					else {
						buf.right_part_zeros = buf.precision + buf.left_part_zeros;
						buf.left_part_zeros = 0;
						if (buf.right_part_zeros < 0)
							buf.right_part_zeros = 0;
					}

					for (int n = 0; n < buf.sd_div; ++n) {
						*d++ = *s++;
					}
					memset(d, '0', buf.left_part_zeros);
					d += buf.left_part_zeros;

					if (dot_space == 1)
						*d++ = '.';

					for (int n = buf.sd_div; n < buf.significant_digits; ++n) {
						*d++ = *s++;
					}
					memset(d, '0', buf.right_part_zeros);
					d += buf.right_part_zeros;
				}
				else {
					// NOTE(Bjorn): We only need to handle the case
					// where precision >= -exp the case when only 0.0...0
					// fits within requested percision where handled erlier
					// i.e. buf.sd_div > 0

					buf.left_part_zeros = -buf.exp - 1;
					buf.sd_div = buf.precision - buf.left_part_zeros;
					buf.right_part_zeros = 0;
					if (buf.sd_div > buf.significant_digits) {
						buf.right_part_zeros = buf.precision
							-  buf.left_part_zeros - buf.significant_digits;
						buf.sd_div = buf.significant_digits;
					}

					memset(d, '0', buf.left_part_zeros + 2);
					d += buf.left_part_zeros + 2;
					for (int n = 0; n < buf.sd_div; ++n) {
						*d++ = *s++;
					}
					memset(d, '0', buf.right_part_zeros);
					d += buf.right_part_zeros;
					out[at + 1] = '.';
				}
				at = d - out;
			}
		}
	}

	if (BNI_FA_HAS_JUSITIFY_LEFT(arg) && buf.pad_count) {
			memset(out + at, buf.pad_char, buf.pad_count);
	}

	return buf.output_len + buf.pad_count;
}

#define BNI_FORMAT_BUF_SIZE 32
i8 bni_integer_lut[16] = "0123456789abcdef";

internal u32
bni_format_integer
(
	u8 *out
	, umm size
	, BniStringFormatArg *arg
)
{
	u8 buf[BNI_FORMAT_BUF_SIZE];
	u8 buf_at = BNI_FORMAT_BUF_SIZE;

	u16 width = arg->format_info & BNI_FAM_WIDTH;
	u8 significant_digits = 0;
	u8 output_len = 1;
	u8 sign = 0;
	if (BNI_FA_HAS_HEX(arg)) {
		u64 val = arg->v.u;

		while(val != 0) {
			buf[--buf_at] =  bni_integer_lut[val & 0x0F];
			val >>= 4;
			++significant_digits;
		}
	}
	else {
		u64 val = 0;

		if (BNI_FA_HAS_SPACE(arg))
				sign = ' ';
		if (BNI_FA_HAS_PLUS(arg))
				sign = '+';
		if (BNI_FAT_IS_SIGNED(arg)) {
			if(arg->v.s < 0) {
				sign = '-';
				val = (u64)(-1 * arg->v.s);
			}
			else 
				val = (u64)arg->v.s;
		}
		else {
			val = arg->v.u;
		}

		while(val != 0) {
			buf[--buf_at] =  bni_integer_lut[val % 10];
			val /= 10;
			++significant_digits;
		}
	}

	if (significant_digits) {
		output_len += significant_digits - 1;
	}

	if (sign) {
		++output_len;
	}

	u8  pad_char = 0;
	u16 pad_count = 0;
	u32 at = 0;
	if (width > output_len) {
		if (BNI_FA_HAS_PAD_ZERO(arg))
			pad_char = '0';
		else
			pad_char = ' ';

		pad_count = width - output_len;
	}

	if (pad_char && !BNI_FA_HAS_JUSITIFY_LEFT(arg)) {
		if (pad_char == '0' && sign) {
			*out = sign;
			++at;
		}
		memset(out + at, pad_char, pad_count);
		at += pad_count;
		if (pad_char == ' ' && sign) {
			out[at] = sign;
			++at;
		}
	}
	else {
		if (sign) {
			out[at] = sign;
			++at;
		}
	}

	if (arg->v.u == 0) {
		out[at] = '0';
		++at;
	}
	else {
		memcpy(out + at, buf + buf_at, significant_digits);
	}

	if (pad_char == ' ' && BNI_FA_HAS_JUSITIFY_LEFT(arg))
	{
		memset(out + at, pad_char, pad_count);
	}

	return output_len + pad_count;
}

internal u32
bni_format_str
(
	  u8 *out
	, umm count
	, BniStringFormatArg *arg
)
{
	u16 width = arg->format_info & BNI_FAM_WIDTH;
	u32 output_len = (u16)((arg->format_info & BNI_FAM_PRECISION)>>16);
	u32 input_len = 0;
	i8 *p = arg->v.c_str;
	if (output_len == 0) {
		while(*p != 0)
			++p;
		output_len = p - arg->v.c_str;
	}

	if (BNI_FA_HAS_HEX(arg)) {
		input_len = output_len;
		output_len *= 2;
	}

	assert(count >= output_len);

	i32 pad_count = width - output_len;
	u32 at = 0;
	if (pad_count < 0) {
		pad_count = 0;
	}
	else {
		assert(count > output_len + pad_count);
		if (!BNI_FA_HAS_JUSITIFY_LEFT(arg)) {
			memset(out, ' ', pad_count);
			at += pad_count;
		}
	}

	if (BNI_FA_HAS_HEX(arg)) {
		i8 *end = arg->v.c_str + input_len;
		for(i8 *p = arg->v.c_str ; p < end ; ++p) {
			i8 v = *p;
			out[at++] = bni_integer_lut[(v>>4) & 0x0F];
			out[at++] = bni_integer_lut[v & 0x0F];
		}
	}
	else {
		i8 *end = arg->v.c_str + output_len;
		for(i8 *p = arg->v.c_str ; p < end ; ++p) {
			out[at++] = *p;
		}
	}

	if (pad_count && BNI_FA_HAS_JUSITIFY_LEFT(arg)) {
		memset(out + at, ' ', pad_count);
	}
	return output_len + pad_count;
}

u32
bni_format(u8 *dest, umm count, BniStringFormater *formater)
{
	umm dest_at = 0;
	umm bytes_consumed = 0;
	i8 *template = formater->format_string;
	BniStringFormatArg *arg;
	u32 argc = 0;
	i8 buf[BNI_FORMAT_BUF_SIZE];

	// real printf code goes in here
	while(*template != 0) {
		if (*template == '%') {
			if (*(template + 1) == '%') {
				++template;
				*dest++ = '%';
				++dest_at;
			}
			else {
				assert(argc < formater->arg_count);
				arg = &formater->args[argc];

				i32 buf_at = BNI_FORMAT_BUF_SIZE;
				u16 width = arg->format_info & BNI_FAM_WIDTH;
				u16 precision = ((arg->format_info & BNI_FAM_PRECISION)>>16);
				if (BNI_FAT_IS_POINTER(arg)) {
					umm val = (umm)(((char*)arg->v.p) + 0);
					while(val != 0) {
						buf[--buf_at] =  bni_integer_lut[val & 0x0F];
						val >>= 4;
					}
				}
				else if (BNI_FAT_IS_INTEGER(arg)) {
					bytes_consumed = bni_format_integer(dest, count - dest_at, arg);
					dest += bytes_consumed;
					dest_at += bytes_consumed;
				}
				else if (BNI_FAT_IS_FLOAT(arg)) {
					bytes_consumed = bni_format_float(dest, count - dest_at, arg);
					dest += bytes_consumed;
					dest_at += bytes_consumed;
				}
				else if (BNI_FAT_IS_CSTRING(arg)) {
					bytes_consumed = bni_format_str(dest, count - dest_at, arg);
					dest += bytes_consumed;
					dest_at += bytes_consumed;
				}
#if 0
				else if (BNI_FAT_IS_DOUBLE(arg))
#endif
				else {
					memcpy(dest, "!Unkown format type!", 20);
					dest_at += 20;
				}
				++argc;
			}
		}
		else {
				*dest++ = *template;
				++dest_at;
		}
		++template;
	}
	return dest_at;
}

internal inline void
bni_formater_reset(BniStringFormater *formater)
{
	formater->arg_count = 0;
}

internal inline umm
bni_formater_size(BniStringFormater *formater)
{
	return sizeof(BniStringFormater)
		+ formater->arg_max * sizeof(BniStringFormatArg);
}

internal inline u32
args_in_format__string(i8 *fmt)
{
	i8 *p = fmt;
	u32 params = 0;
	while(*p != 0) {
		if (*p == '%') {
		   if (*(p+1) != '%')
				++params;
		   else 
			   ++p;
		}
		++p;
	}
	return params;		
}

#define pack_fa(flags, info, precision, width) \
	((flags) | (info)) | (precision) << 16 | (width);


internal inline BniStringFormatArg
bni_format_arg_f(u64 format_info, f32 val, u16 width, u16 precision)
{
	BniStringFormatArg result;
	result.format_info = pack_fa(BNI_FAT_F32, format_info, precision, width);
	result.v.f = val;
	return result;
}

internal inline BniStringFormatArg
bni_format_arg_i(u64 format_info, i64 val, u16 width, u16 precision)
{
	BniStringFormatArg result;
	result.format_info = pack_fa(BNI_FAT_S64, format_info, precision, width);
	result.v.s = val;
	return result;
}

internal inline BniStringFormatArg
bni_format_arg_u(u64 format_info, u64 val, u16 width, u16 precision)
{
	BniStringFormatArg result;
	result.format_info = pack_fa(BNI_FAT_U64, format_info, precision, width);
	result.v.u = val;
	return result;
}

internal inline BniStringFormatArg 
bni_format_arg_s(u64 format_info, i8 *val, u16 width, u16 precision)
{
	BniStringFormatArg result;
	result.format_info = pack_fa(BNI_FAT_CSTR, format_info, precision, width);
	result.v.c_str = val;
	return result;
}

internal inline BniStringFormater *
formater_alloc(Arena *arena, u32 max, i8 *format_string)
{
	arena_push_align(arena, 8);
	BniStringFormater *result = arena_push_array(arena, BniStringFormater
			, sizeof(BniStringFormater) * (max + 1));
	result->format_string = format_string;
	result->arg_max = max;
	result->arg_count = 0;

	return result;
}

internal inline void
bni_push_format_arg
(
	  BniStringFormater *format
	, BniStringFormatArg arg
)
{
	assert(format->arg_count < format->arg_max);
	format->args[format->arg_count++] = arg;
}

internal void
bni_printf(i8 *fmt, ...)
{
	u32 argc =  args_in_format__string(fmt);
	umm size = kb(16);	

	Arena *scratch = get_scratch(0);
	BniStringFormater *formater = formater_alloc(scratch, argc, fmt);
	u8 *buf = arena_push_array(scratch, u8, size);

	va_list argp;
	va_start(argp, fmt);
	while(formater->arg_count < formater->arg_max)
	{
		bni_push_format_arg(formater, va_arg(argp, BniStringFormatArg));
	}
	
	umm used = bni_format(buf, size, formater);
	assert(used < size);

	
	OSFile stdout = OS_get_stdout();
	OS_write_file(stdout, buf, used);

	release_scratch(scratch);
}

internal void
bni_fprintf(OSFile f, i8 *fmt, ...)
{
	u32 argc =  args_in_format__string(fmt);
	u32 size = kb(16);

	Arena *scratch = get_scratch(0);
	BniStringFormater *formater = formater_alloc(scratch, argc, fmt);
	u8 *buf = arena_push_array(scratch, u8, size);

	va_list argp;
	va_start(argp, fmt);
	while(formater->arg_count < formater->arg_max)
	{
		bni_push_format_arg(formater, va_arg(argp, BniStringFormatArg));
	}
	
	umm used = bni_format(buf, size, formater);
	assert(used < size);

	OS_write_file(f, buf, used);
	release_scratch(scratch);
}

internal umm
bni_sprintf(Str8 *dst, u32 at, i8 *fmt , ...)
{
	u32 argc =  args_in_format__string(fmt);

	assert(at < dst->cap);

	Arena *scratch = get_scratch(0);
	BniStringFormater *formater = formater_alloc(scratch, argc, fmt);

	va_list argp;
	va_start(argp, fmt);
	while(formater->arg_count < formater->arg_max)
	{
		bni_push_format_arg(formater, va_arg(argp, BniStringFormatArg));
	}
	umm used = bni_format((u8*)dst->str + at, dst->cap - at, formater);
	release_scratch(scratch);

	va_end(argp);
	assert(used < dst->cap);

	return used;
}

internal umm
bni_sformats(Str8 *dst, u32 at, BniStringFormater *fmt , ...)
{
	assert(at < dst->cap);

	va_list argp;
	va_start(argp, fmt);
	while(fmt->arg_count < fmt->arg_max)
	{
		bni_push_format_arg(fmt, va_arg(argp, BniStringFormatArg));
	}
	va_end(argp);

	umm used = bni_format((u8*)dst->str + at, dst->cap - at, fmt);
	bni_formater_reset(fmt);

	assert(used < dst->cap);

	return used;
}

/* Copyright 2019 Björn Nilsson <public@bnicon.se>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 // vim: ts=4 sw=4 cindent cino=0(,\:0,l1
