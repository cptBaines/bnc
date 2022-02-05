/* thread_ctx.h - v0.90 - MIT - Björn Nilsson, 2021 *
 * LICENSE
 *   See end of file for license information.
 */
#ifndef THREAD_CTX_H_INCLUDED
#define THREAD_CTX_H_INCLUDED

#define TLS_INDEX_COUNT 8
global i32 tls_indices[TLS_INDEX_COUNT];


typedef struct TCTX TCTX;
struct TCTX {
	Arena arena;
	bool in_use;
};


// Begin fdecl
void
tls_alloc();
void
tls_free();
void
setup_thread_context();
void
teardown_thread_context();
Arena *
get_scratch(Arena *arena);
void
release_scratch(Arena *arena);
// End fdecl

#endif //THREAD_CTX_H_INCLUDED

/* Copyright 2021 Björn Nilsson <public@bnicon.se> 
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
