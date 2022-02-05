/* thread_ctx.h - v0.90 - MIT - Björn Nilsson, 2021
 *
 * LICENSE
 *   See end of file for license information.
 */

void
tls_alloc()
{
	for(i32 i = 0; i < TLS_INDEX_COUNT; ++i) {
		i32 idx = TlsAlloc();
		if (idx != TLS_OUT_OF_INDEXES)
			tls_indices[i] = idx;
	}
}

void
tls_free()
{
	for(i32 i = 0; i < TLS_INDEX_COUNT; ++i) {
	TlsFree(tls_indices[i]);
	}
}

void
setup_thread_context()
{
	for(i32 i = 0; i < TLS_INDEX_COUNT; ++i) {
		Arena scratch = arena_reserve(kb(64));
		TCTX *tctx = arena_push_struct(&scratch, TCTX);
		tctx->arena   = scratch;
		tctx->in_use  = false;
		TlsSetValue(tls_indices[i], tctx);
	}
}

void
teardown_thread_context()
{
	for(i32 i = 0; i < TLS_INDEX_COUNT; ++i) {
		TCTX *tctx = (TCTX*)TlsGetValue(tls_indices[i]);
		VirtualFree(tctx->arena.base, 0, MEM_RELEASE);
	}
}

Arena *
get_scratch(Arena *arena)
{
	Arena *result = NULL;
	for(i32 i = 0; i < TLS_INDEX_COUNT; ++i) {
		TCTX *tctx = (TCTX*)TlsGetValue(tls_indices[i]);
		if (!tctx->in_use && (&tctx->arena != arena)) {
			tctx->in_use = true;
			result = &tctx->arena;
			break;
		}
	}
	return result;
}

void
release_scratch(Arena *arena)
{
	for(i32 i = 0; i < TLS_INDEX_COUNT; ++i) {
		TCTX *tctx = (TCTX*)TlsGetValue(tls_indices[i]);
		if (&tctx->arena == arena) {
			arena->pos = sizeof(TCTX);
			tctx->in_use = false;
		}
	}
}

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
