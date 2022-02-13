/* win32.h - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */

#ifndef WIN32_H_INCLUDED
#define WIN32_H_INCLUDED

typedef u32 PlatformError;

// Begin fdecl

internal Str8 * win32_get_command_line(Arena *arena);
internal PlatformError win32_get_current_directory(Arena *mem, Str16 *working_dir);
internal PlatformError win32_get_file_attributes(Str16 *path, u32 *file_attr);
internal PlatformError win32_create_directory(Str16 *path);
internal void win32_close_handle(HANDLE handle);
internal bool win32_path_exists(Str16 *path);

// End fdecl
#endif //WIN32_H_INCLUDED

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
