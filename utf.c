/* utf.c - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */

internal inline i32
utf__8_to_cp(u32 *cp, i8 *buf, umm size)
{
	i8 enc_cp = *buf;
	int follow_bytes = 0;
	u32 tmp;
	if ((enc_cp & 0x80) == 0)
	{
		*cp = enc_cp;
		return 1;
	}

	if ((enc_cp & 0xF8) == 0xF0)
	{
		tmp = ((enc_cp &  0x07) << 18);
		follow_bytes =  3;
	}
	else if ((enc_cp & 0xF0) == 0xE0)
	{
		tmp = ((enc_cp &  0x0F) << 12);
		follow_bytes = 2;
	}
	else if ((enc_cp & 0xE0) == 0xC0)
	{
		tmp = ((enc_cp &  0x1F) << 6);
		follow_bytes = 1;
	}
	else
	{
		// TODO(bjor): Handle encoding error
		// Follow byte without a header byte
		// Skip for now
		assert(!"UTF-8 encoding error");
		return 1;
	}

	buf++;
	int j = follow_bytes;
	while (j > 0 && ((*buf & 0xC0) == 0x80))
	{
		tmp |= (*buf++ & 0x03F) << (--j * 6);
	}
	if (j > 0)
	{
		// TODO(bjor): handle encoding error
		// to few follow bytes
		assert(!"UTF-8 encoding error");
	} else {
		*cp = tmp;
	}
	return follow_bytes + 1 - j;
}

internal inline i32
utf__cp_to_8(i8 *buf, umm size, u32 cp)
{
	int bytes_consumed = 0;
	if (cp < 0x80)
	{
		*buf = (uint8_t)cp;
		bytes_consumed = 1;
	}
	else if (cp >= 0x80 && cp <= 0x07FF)
	{
		*(buf + 0) = (0xC0 | (uint8_t)(cp >> 6));
		*(buf + 1) = (0x80 | (uint8_t)(cp & 0x0000003F));
		bytes_consumed = 2;
	}
	else if (cp >= 0x0800 && cp <= 0xFFFF)
	{
		*(buf + 0) = (0xE0 | (uint8_t)(cp >> 12));
		*(buf + 1) = (0x80 | (uint8_t)((cp & 0x00000FC0) >> 6));
		*(buf + 2) = (0x80 | (uint8_t)(cp & 0x0000003F));
		bytes_consumed = 3;
	}
	else if (cp >= 0x10000 && cp <= 0x10FFFF)
	{
		*(buf + 0) = (0xF0 | (uint8_t)(cp >> 18));
		*(buf + 1) = (0x80 | (uint8_t)((cp & 0x0003F000) >> 12));
		*(buf + 2) = (0x80 | (uint8_t)((cp & 0x00000FC0) >> 6));
		*(buf + 3) = (0x80 | (uint8_t)(cp & 0x0000003F));
		bytes_consumed = 4;
	}
	else
	{
		assert(!"Code point out of range > 0x10FFFF");
	}
	return bytes_consumed;
}

internal inline i32
utf__cp_to_16le(wchar_t *buf, umm size16, u32 cp)
{
	int slots_consumed = 0;

	if (cp <= 0xD7FF || (cp >= 0xE000 && cp <= 0xFFFF))
	{
		*buf = (wchar_t)cp;
		slots_consumed = 1;
	}
	else if (cp > 0xFFFF)
	{
		cp -= 0x10000;
		*buf = (wchar_t)((cp >> 10) | 0xD800);
		*(buf + 1) = (wchar_t)((cp & 0x3FF) | 0xDC00);
		slots_consumed = 2;
	}
	else
	{
		assert(!"Invalid code point, 0xD800 - 0xDFFF");
	}
	return slots_consumed;
}

internal inline i32
utf__16le_to_cp(u32 *cp, wchar_t  *buf, umm  size)
{
	u16 enc_cp = *(buf);
	i32 slots_consumed = 0;

	if (enc_cp <= 0xD7FF || (enc_cp >= 0xE000 && enc_cp <= 0xFFFF))
	{
		*cp = enc_cp;
		slots_consumed =  1;
	}
	else if (enc_cp >= 0xD800 && enc_cp <= 0xDBFF)
	{
		if (2 <= size)
		{
			u16 hi_surrogate = enc_cp;
			u16 lo_surrogate = *(buf + 1);
			if (lo_surrogate >= 0xDC00 && lo_surrogate <= 0xDFFF)
			{
				uint16_t hi = (hi_surrogate - 0xD800) * 0x400;
				uint16_t lo = (lo_surrogate - 0xDC00);
				*cp = (u32)hi + (u32)lo + 0x10000;
				slots_consumed = 2;
			}
			else
			{
				// TODO(bjorn): Handle missmatch surrogate
				// pairs high but no low
				assert(!"Missmatch surrogate high, no low");
			}
		}
		else
		{
			// TODO(bjorn): Handle missmatch surrogate
			// pairs high but no low
			assert(!"high surrogate at end of buffer");
		}
	}
	else
	{
		// TODO(bjorn): Handle missmatch surrogate
		// pairs low but no high
		assert(!"Missmatch surrogate low, no high");
	}

	return slots_consumed;
}

internal umm
utf_8_to_16le(wchar_t *utf16, umm len16, i8 *utf8, umm len8)
{
	wchar_t *utf16_end = utf16 + len16;

	u32 cp = 0;
	umm nwrite = 0;

	// TODO(bjorn): Handle errors
	while(len8 > 0)
	{
		umm n;
		n = utf__8_to_cp(&cp, utf8, len8);
		utf8 += n;
		len8 -= n;

		n = utf__cp_to_16le(utf16, len16, cp);
		utf16 += n;
		len16 -= n;
		nwrite += n;
	}
	assert(utf16 <= utf16_end);
	return nwrite;
}

internal umm
utf_16le_to_8(i8 *utf8, umm len8, wchar_t *utf16, umm len16)
{
	i8 *utf8_end = utf8 + len8;

	u32 cp = 0;
	umm nwrite = 0;

	// TODO(bjorn): Handle errors
	while(len16 > 0)
	{
		umm n;
		n = utf__16le_to_cp(&cp, utf16, len16);
		utf16 += n;
		len16 -= n;

		n = utf__cp_to_8(utf8, len8, cp);
		utf8 += n;
		len8 -= n;
		nwrite += n;

	}
	assert(utf8 <= utf8_end);
	return nwrite;
}

static inline u32
utf8__encode_size(u32 cp)
{
	if(cp <= 0x007F) return 1;
	if(cp <= 0x07FF) return 2;
	if(cp <= 0xFFFF) return 3;
	return 4;
}

internal umm 
utf_16le_to_8_size(wchar_t *utf16, umm len16)
{
	umm res = 0;
	u16 *p = utf16;
	u16 *p_end = p + len16;
	while (p < p_end) {
		if ((*p <= 0xD7FF) || (*p >= 0xE000))
			res += utf8__encode_size(*p);
		else {	
			res += 4;
			++p;
		}
		++p;
	}	
	return res;
}

internal umm 
utf_8_to_16le_size(i8 *utf8, umm len8)
{
	umm res = 0;
	u8 *p = (u8*)utf8;
	u8 *p_end = p + len8;
	while (p < p_end) {
		res += 2;
		if ((*p & 0x80) == 0x00) {
			++p;
		} 
		else if ((*p & 0xE0) == 0xC0) {
			p += 2;	
		}
		else if ((*p & 0xF0) == 0xE0) {
			p += 3;	
		}
		else if ((*p & 0xF8) == 0xF0) {
			res += 2;
			p += 4;
		}
		else
			assert(!"Strange: Utf8 sequence");
	}
	return res;
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
