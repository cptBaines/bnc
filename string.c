/* string.c - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */
void *
memcpy(void *dst, const void *src, umm size)
{
	u8 *d = (u8*)dst;
	u8 *s = (u8*)src;

	while (size > 0)
	{
		*d++ = *s++;
		--size;
	}
	return dst;
}

void *
memset(void *dst, i32 value, u64 size)
{
	u8 *p = (u8*)dst;
	while (size > 0)
	{
		*p++ = (u8)value;
		--size;
	}
	return dst;
}

	
internal umm
non_zero_bytes16(wchar_t *str)
{
	umm res = 0;
	wchar_t *p = str;
	for(;*p != 0; ++p)
		;
	return (p - str) * 2; 
}

internal void
str16_append_16(Str16 *dst, wchar_t *str)
{	
	u8 *d = (u8*)(dst->str + dst->len);
	u8 *s = (u8*)str;
	u64 count = min_u64((dst->cap - dst->len)* 2
		         , non_zero_bytes16(str));
	memcpy(d, s, count); 
	dst->len += count>>1;
}	

internal void
str16_append(Str16 *dst, Str16 *src)
{
	u8 *d = (u8*)(dst->str + dst->len);
	u8 *s = (u8*)src->str;
	u64 count = min_u64((dst->cap - dst->len)* 2, src->len * 2);
	memcpy(d, s, count); 
	dst->len += count>>1;
}

internal void
str16_copy(Str16 *dst, Str16 *src)
{
	dst->len = 0;
	str16_append(dst, src);
}

internal inline void
str16_zero_terminate(Str16 *str)
{
	if (str->cap > str->len)
		str->str[str->len] = 0;	
}

internal Str16 
str16_dirname(Str16 *path)
{
	Str16 parent = *path;
	wchar_t *p = path->str + (path->len-1);
	wchar_t *p_end = path->str;
	for(; p > p_end && *p != L'\\'; --p)
		;

	parent.len = p - p_end;
	return parent;
}

internal Str16
str16_wrap16(wchar_t *src)
{
	Str16 result;
	result.str = src;
	result.len = non_zero_bytes16(src)>>1;
	result.cap = result.len + 1;
	return result;
}

internal Str16 *
str16_dup(Arena *mem, Str16 *src)
{
	Str16 *result = str16_alloc(mem, src->cap);
	str16_copy(result, src);
	return result;
}

internal Str16 *
str16_alloc(Arena *mem, u32 cap)
{
	arena_push_align(mem, 8);
	Str16 *result = arena_push_struct(mem, Str16);
	result->str = arena_push_array(mem, wchar_t, cap);
	result->len = 0;
	result->cap = cap;
	if (result != NULL && result->str == NULL)
		return NULL;
	return result;
}

internal Str16
str16_init(Arena *mem, u32 cap)
{
	Str16 result;
	arena_push_align(mem, 2);
	result.str = arena_push_array(mem, wchar_t, cap);
	result.len = 0;
	result.cap = cap;
	return result;
}

internal void
str_trim_alloc16(Arena *arena, Str16 *arena_str, u32 used)
{
	assert((u8*)(arena_str->str + arena_str->cap) == arena->base + arena->pos);
	arena_rewind(arena, (arena_str->cap - used - 1) * 2);
	arena_str->len = used;
	arena_str->cap = used+1;
}

////////////////////////
// Str8
//
internal umm
non_zero_bytes8(i8 *str)
{
	umm res = 0;
	i8 *p = str;
	for(;*p != 0; ++p)
		;
	return (p - str);
}

internal Str8 *
str_alloc(Arena *mem, u32 cap)
{
	arena_push_align(mem, 8);
	Str8 *result = arena_push_struct(mem, Str8);
	result->str = arena_push_array(mem, i8, cap);
	result->len = 0;
	result->cap = cap;
	if (result != NULL && result->str == NULL)
		return NULL;

	return result;
}

internal Str8
str_wrap(i8 *src)
{
	Str8 result;
	result.str = src;
	result.len = non_zero_bytes8(src);
	result.cap = result.len + 1;
	return result;
}

internal bool
str_equal(Str8 *a, Str8 *b)
{
	if (a->len != b->len)	
		return false;

	i64 i;
	for (i = 0; i < a->len && a->str[i] == b->str[i]; ++i)
		;
	return (i == a->len) ? true : false;
}

internal void
str_append8(Str8 *dst, i8 *str)
{	
	u8 *d = (u8*)(dst->str + dst->len);
	u8 *s = (u8*)(str);
	u64 count = min_u64(dst->cap - dst->len, non_zero_bytes8(str));
	memcpy(d, s, count); 
	dst->len += count;
}

internal Str8
str_tail(Str8 *s)
{
	Str8 result = {0};
	result.str = s->str + s->len;
	result.cap = s->cap - s->len;
	return result;
}

internal void
str_append(Str8 *dst, Str8 *src)
{
	u8 *d = ((u8*)dst->str + dst->len);
	u8 *s = ((u8*)src->str);
	u64 count = min_u64(dst->cap - dst->len, src->len);
	memcpy(d, s, count); 
	dst->len += count;
}

internal void
str_copy(Str8 *dst, Str8 *src)
{
	dst->len = 0;
	str_append(dst, src);
}

internal inline void
str_zero_terminate(Str8 *str)
{
	if (str->cap > str->len)
		str->str[str->len] = 0;	
}

internal Str8 *
str_dup(Arena *mem, Str8 *src)
{
	Str8 *result = str_alloc(mem, src->cap);
	str_copy(result, src);
	return result;
}

internal Str8
str_init(Arena *mem, u32 cap)
{
	Str8 result;
	result.str = arena_push_array(mem, i8, cap);
	result.len = 0;
	result.cap = cap;
	return result;
}

internal void
str_trim_alloc(Arena *arena, Str8 *arena_str, u32 used)
{
	assert((u8*)(arena_str->str + arena_str->cap) == arena->base + arena->pos);
	arena_rewind(arena, arena_str->cap - used - 1);
	arena_str->len = used;
	arena_str->cap = used+1;
}

////////////////////////
// string manipulation routines

// NOTE(bjorn): As we make the substitutions inplace search and replace
// must have the same lengths.
// count determine the number of replacements to perform:
// if count < 0 replace at most count occurences from the back
// if count > 0 replace at most count occurences from the front
// if count = 0 replace all the occurences
internal void
str_replace(Str8 *str, Str8 *search, Str8 *replace, i32 count)
{
	assert(search->len == replace->len);
		
	if (search->len == 1){
		if (count > 0) {
			i8 *p = str->str;
			i8 *p_end = str->str + str->len;
			for(; p < p_end && count > 0; ++p){
				if (*p == search->str[0]){
					*p = replace->str[0];
					--count;
				}
			}
		}
		else if (count < 0) {
			i8 *p_end = str->str;
			i8 *p = str->str + str->len;
			count = count * -1;
			for(; p_end < p && count > 0; --p){
				if (*p == search->str[0]){
					*p = replace->str[0];
					--count;
				}
			}
		}
		else {
			i8 *p = str->str;
			i8 *p_end = str->str + str->len;
			for(; p < p_end; ++p){
				if (*p == search->str[0]){
					*p = replace->str[0];
				}
			}
		}
	}
	// TODO(bjorn): Implement multichar replace
}

////////////////////////
// string conversion routines

internal Str8
str16_to8(Str16 *src, Arena *arena)
{
	Str8 result = {0};
	umm max_size = src->len * 3 + 1;
	result = str_init(arena, max_size);
	if(result.str == NULL)
		return result;
	umm used = utf_16le_to_8(result.str, result.cap, src->str, src->len);
	str_trim_alloc(arena, &result, used);
	return result;
}

internal Str8*
str16_to8p(Str16 *src, Arena *arena)
{
	Str8 *result = 0;
	umm max_size = src->len * 2 + 1;
	result = str_alloc(arena, max_size);
	if(result == NULL)
		return result;
	umm used = utf_16le_to_8(result->str, result->cap, src->str, src->len);
	str_trim_alloc(arena, result, used);
	return result;
}

internal Str16 
str8_to16(Str8 *src, Arena *arena)
{
	Str16 result = {0};
	// TODO(bjorn): This is not always true if we have surogate pairs 
	// this will blow up. We should probalby count exactly
	umm max_size = src->len + 1;
	result = str16_init(arena, max_size);
	if(result.str == NULL)
		return result;

	umm used = utf_8_to_16le(result.str, result.cap, src->str, src->len);
	str_trim_alloc16(arena, &result, used);
	return result;
}

internal Str16* 
str8_to16p(Str8 *src, Arena *arena)
{
	Str16 *result = 0;
	// TODO(bjorn): This is not always true if we have surogate pairs 
	// this will blow up. We should probalby count exactly
	umm max_size = src->len + 1;
	result = str16_alloc(arena, max_size);
	if(result == NULL)
		return result;

	umm used = utf_8_to_16le(result->str, result->cap, src->str, src->len);
	str_trim_alloc16(arena, result, used);
	return result;
}

internal inline u32 
str_to16_size(Str8 *src)
{
	umm size = utf_8_to_16le_size(src->str, src->len);
	assert(size < U32_MAX);
	return (u32)size;
}

internal inline u32
str16_to8_size(Str16 *src)
{
	umm size = utf_16le_to_8_size(src->str, src->len);
	assert(size < U32_MAX);
	return (u32)size;
}

////////////////////////
// string builder routines
//

internal StrBuilder8 *
str_builder_alloc(Arena *arena)
{
	StrBuilder8 *result;
	arena_push_align(arena, 8);
	result = arena_push_struct(arena, StrBuilder8);
	result->arena = arena;
	result->str = str_init(arena, 0);
	return result;
}

internal void
str_builder_append_i8(StrBuilder8 *bldr, i8 *src)
{
	umm max_size = non_zero_bytes8(src);
	i8 *extended_str_storage = arena_push_array(bldr->arena, i8, max_size);
	assert(extended_str_storage != NULL);
	bldr->str.cap += max_size;
	str_append8(&bldr->str, src);
}

internal void
str_builder_append(StrBuilder8 *bldr, Str8 *src)
{
	i8 *extended_str_storage = arena_push_array(bldr->arena, i8, src->len);
	assert(extended_str_storage != NULL);
	bldr->str.cap += src->len;
	str_append(&bldr->str, src);
}

internal StrBuilder16 *
str16_builder_alloc(Arena *arena)
{
	StrBuilder16 *result;
	arena_push_align(arena, 8);
	result = arena_push_struct(arena, StrBuilder16);
	result->arena = arena;
	result->str = str16_init(arena, 0);
	return result;
}

internal void
str16_builder_append8(StrBuilder16 *bldr, Str8 *src)
{
	umm max_size = src->len * 3 + 1;
	wchar_t *extended_str_storage = arena_push_array(bldr->arena, wchar_t, max_size);
	assert(extended_str_storage != NULL);
	bldr->str.cap += max_size;

	umm used = utf_8_to_16le(bldr->str.str + bldr->str.len, max_size, src->str, src->len);
	str_trim_alloc16(bldr->arena, &bldr->str, bldr->str.len + used);
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
