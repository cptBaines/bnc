/* helpers.c - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */
internal inline u64 max_u64(u64 a, u64 b) { return (a > b) ? a : b; }
internal inline u64 min_u64(u64 a, u64 b) { return (a < b) ? a : b; }
internal inline u32 max_u32(u32 a, u32 b) { return (a > b) ? a : b; }
internal inline u32 min_u32(u32 a, u32 b) { return (a < b) ? a : b; }
internal inline u16 max_u16(u16 a, u16 b) { return (a > b) ? a : b; }
internal inline u16 min_u16(u16 a, u16 b) { return (a < b) ? a : b; }
internal inline u8  max_u8(  u8 a, u8 b ) { return (a > b) ? a : b; }
internal inline u8  min_u8(  u8 a, u8 b ) { return (a < b) ? a : b; }

internal inline i64 max_i64(i64 a, i64 b) { return (a > b) ? a : b; }
internal inline i64 min_i64(i64 a, i64 b) { return (a < b) ? a : b; }
internal inline i32 max_i32(i32 a, i32 b) { return (a > b) ? a : b; }
internal inline i32 min_i32(i32 a, i32 b) { return (a < b) ? a : b; }
internal inline i16 max_i16(i16 a, i16 b) { return (a > b) ? a : b; }
internal inline i16 min_i16(i16 a, i16 b) { return (a < b) ? a : b; }
internal inline i8  max_i8 ( i8 a, i8 b ) { return (a > b) ? a : b; }
internal inline i8  min_i8 ( i8 a, i8 b ) { return (a < b) ? a : b; }

internal inline f32  max_f32 ( f32 a, f32 b ) { return (a > b) ? a : b; }
internal inline f32  min_f32 ( f32 a, f32 b ) { return (a < b) ? a : b; }

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
