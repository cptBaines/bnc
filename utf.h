/* utf.h - v0.90 - MIT - Björn Nilsson, 2019 
 * 
 * LICENSE
 *   See end of file for license information.
 */

#ifndef UTF_H_INCLUDED
#define UTF_H_INCLUDED


// Begin fdecl
internal umm utf_16le_to_8(i8 *utf8, umm len8, wchar_t *utf16, umm len16);
internal umm utf_8_to_16le(wchar_t *utf16, umm len16, i8 *utf8, umm len8);
internal umm utf_8_to_16le_size(i8 *utf8, umm len8);
internal umm utf_16le_to_8_size(wchar_t *utf16, umm len16);
internal umm utf_8_to_16le_size(i8 *utf8, umm len8);
// End fdecl
#endif //UTF_H_INCLUDED

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
