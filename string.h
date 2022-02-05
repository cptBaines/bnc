/* string.h - v0.90 - MIT - Björn Nilsson, 2019 
 * 
 * LICENSE
 *   See end of file for license information.
 */

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

typedef struct Str8 Str8;
struct Str8 {
	i8  *str;
	u32 len;
	u32 cap;
}; 

typedef struct Str16 Str16;
struct Str16 {
	wchar_t  *str;
	u32 len;
	u32 cap;
};

typedef struct StrBuilder8 {
	Arena *arena;
	Str8 str;
} StrBuilder8;

typedef struct StrBuilder16 {
	Arena *arena;
	Str16 str;
} StrBuilder16;

#define str_lit(a)   {(a),  sizeof((a))    -1,  sizeof((a))}
#define str16_lit(a) {(a), (sizeof((a))>>1)-1, (sizeof((a))>>1)}

// Begin fdecl

void *
memcpy(void *dst, const void *src, umm size);
void *
memset(void *dst, i32 value, u64 size);
internal umm
non_zero_bytes16(wchar_t *str);
internal void
str16_append_16(Str16 *dst, wchar_t *str);
internal void
str16_append(Str16 *dst, Str16 *src);
internal void
str16_copy(Str16 *dst, Str16 *src);
internal inline void
str16_zero_terminate(Str16 *str);
internal Str16 
str16_dirname(Str16 *path);
internal Str16
str16_wrap16(wchar_t *src);
internal Str16 *
str16_dup(Arena *mem, Str16 *src);
internal Str16 *
str16_alloc(Arena *mem, u32 cap);
internal Str16
str16_init(Arena *mem, u32 cap);
internal umm
non_zero_bytes8(i8 *str);
internal Str8 *
str_alloc(Arena *mem, u32 cap);
internal Str8
str_wrap(i8 *src);
internal void
str_append8(Str8 *dst, i8 *str);
internal void
str_append(Str8 *dst, Str8 *src);
internal void
str_copy(Str8 *dst, Str8 *src);
internal inline void
str_zero_terminate(Str8 *str);
internal Str8 *
str_dup(Arena *mem, Str8 *src);
internal Str8
str_init(Arena *mem, u32 cap);
internal void
str_replace(Str8 *str, Str8 *search, Str8 *replace, i32 count);
internal bool str_equal(Str8 *a, Str8 *b);

internal Str16 
str_to16(Str8 *src, Arena *arena);
internal Str16* 
str_to16p(Str8 *src, Arena *arena);
internal u32
str_to16_size(Str8 *src);

internal Str8 
str16_to8(Str16 *src, Arena *arena);
internal Str8 * 
str16_to8p(Str16 *src, Arena *arena);
internal u32
str16_to8_size(Str16 *src);

internal void
str16_builder_append8(StrBuilder16 *bldr, Str8 *src);

// End fdecl
#endif // STRING_H_INCLUDED

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
