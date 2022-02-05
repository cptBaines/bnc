/* bnc.h - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */
#include "types.h"
#include "memory.h"
#include "thread_ctx.h"
#include "helpers.h"
#include "assert.h"
#include "utf.h"
#include "string.h"
#include "win32.h"
#include "os.h"
#include "format.h"
#include "nocrt.h"

#include "thread_ctx.c"
#include "helpers.c"
#include "utf.c"
#include "string.c"
#include "format.c"
#include "win32.c"
#include "os.c"
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
