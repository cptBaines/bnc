/* os.h - v0.90 - MIT - Björn Nilsson, 2019 
 * 
 * LICENSE
 *   See end of file for license information.
 */

#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED

// This is the high part of the u64 error value
// the low part is whatever platform errorcode caused
// the call to fail.
enum OSError {
	OSError_None,
	OSError_AlreadyExists,
	OSError_PathNotFound,
	OSError_CreationFailed,
	OSError_InvalidPath,
	OSError_File,
	OSError_Undefined
};
typedef enum OSError OSError;

typedef enum BniFileMode
{
	BniFileMode_read = 0x01,
	BniFileMode_write = 0x02,
	BniFileMode_append = 0x04,
} BniFileMode;

typedef struct OSReturn OSReturn;
struct OSReturn {
	void *handle;
	u64  error;
};

typedef struct OSFile OSFile;
struct OSFile {
	HANDLE handle;
	u64  error;
};

#define is_os_error(ret) (((ret).error>>32) != 0)
#define abort_if_os_error(ret) abort_if_os_error_((ret), __FILE_NAME__, __LINE__)

internal inline void
abort_if_os_error_(OSReturn ret, i8* file, u32 line)
{
	// TODO(bjorn): Print error message reporting os and platform error before exiting
	if (is_os_error(ret))
		ExitProcess(3);
}

// Begin fdecl

internal void OS_assert(i8 *expr, i8* file, u32 line);
internal OSReturn OS_get_command_line(Arena *arena);
internal OSFile OS_get_stdout();
internal OSFile OS_get_stderr();
internal OSFile OS_open_file(Str8 *path, i8 *mode);

internal umm OS_read_file(OSFile file, u8 *buf, umm size);
internal umm OS_write_file(OSFile file, u8* data, umm size);
internal umm OS_file_get_size(OSFile file);

internal u32 OS_unix_systime(void);
internal u64 OS_systime(void);

// End fdecl
#endif //OS_H_INCLUDED

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
