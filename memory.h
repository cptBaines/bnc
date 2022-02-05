/* memory.h - v0.90 - MIT - Björn Nilsson, 2020 
 * 
 * LICENSE
 *   See end of file for license information.
 */

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#define M_GRANULARITY 0x10000
#define M_BLOCK_SIZE  0x01000

#ifdef MEM_FIXED_ADDRESS
#define M_BASE_ADDR 0x20000000
#else
#define M_BASE_ADDR NULL
#endif

#define internal		static
#define local_assign_once	static
#define global			static


typedef struct Arena Arena;
struct Arena {
	u8 *base;
	umm commit_pos;
	umm commit_max;
	umm pos;
};

#define arena_push_array(arena, type, count) \
	(type*)arena_commit((arena), sizeof(type) * (count))

#define arena_push_struct(arena, type) \
	(type*)arena_commit((arena), sizeof(type))

/* arg: arena to align
 * arg: byte boundary to align to. Shold be a power of 2, ex 8, 16, 32, etc.
 * doc: push enough bytes onto the arena so that the returned address of the 
 *      next push will be aligned on the requested byte boundary
 */
#define arena_push_align(arena, bytes) \
	arena_commit((arena), (((bytes)-(((arena)->pos) & ((bytes)-1))) & ((bytes)-1)))

/* arg: Size in bytes of the virtual address space to reserve
 * ret: An arena with a valid base pointer if successfull otherwise
 *      the base pointer will be null and any use of the arena in a commit
 *      call will fail.
 * doc: reserves a block of virtual address space for the process.
 */
internal Arena
arena_reserve(umm size)
{
	Arena result = {0};
	u32 err = 0;
	umm reserve_size = ((size + (M_GRANULARITY-1)) & ~(M_GRANULARITY-1));
	//bni_assert(reserve_size & (~(M_GRANULARITY-1)) == 0);

	result.base = VirtualAlloc(NULL, reserve_size, MEM_RESERVE, PAGE_READWRITE);
	if (result.base != NULL) {
		result.commit_pos = 0;
		result.commit_max = reserve_size;
		result.pos = 0;
	}
	else {
		err = GetLastError();
	}

	return result;
}

/* arg: Arena from which to commit some block of memory
 * arg: Size in bytes of memory block to commit
 * ret: Pointer to valid memory block of requested size or NULL if unsucessfull 
 * doc: Fast path of this function just allocates from allready commited
 *      memory by increasing a pointer. If requested size will not fit in 
 *      currently commited memory and ther are space left in the reserved
 *      space of the arena a new block of memory big enough to satisfy the request
 *      rounded up to the nearest 64K will be committed and a valid pointer 
 *      returned. If we run out of reserved space or the commit call fails
 *      returne vallue will be NULL. 
 */
internal u8 *
arena_commit(Arena *arena, umm size)
{
	u8 *mem = NULL;
	if (arena->pos + size < arena->commit_pos) {
		mem = arena->base + arena->pos;
		arena->pos += size;
	} else if (arena->commit_pos < arena->commit_max) {
		umm commit_size = ((size + (M_BLOCK_SIZE-1)) & ~(M_BLOCK_SIZE-1));
		//bni_assert(commit_size & (~(M_BLOCK_SIZE-1)) == 0);
		if (VirtualAlloc(arena->base + arena->commit_pos
				, commit_size, MEM_COMMIT, PAGE_READWRITE) != NULL) {
			arena->commit_pos += commit_size;
			mem = arena->base + arena->pos;
			arena->pos += size;
		}
		else {
		mem = NULL;
		}
	}
	else
		mem = NULL;

	return mem;
}

internal inline void
arena_rewind(Arena *arena, umm subtract)
{
	arena->pos -= subtract;
}
#endif // MEMORY_H_INCLUDED

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
