/* os.c - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */

#define mk_os_error(os_err, pf_err) ((((u64)(os_err))<<32)|(pf_err))

internal inline u32
OS__file_mode(i8 *mode_str)
{
	u32 mode = 0;
	while(*mode_str != 0)
	{
		if (*mode_str == 'r')
			mode |= BniFileMode_read;
		if (*mode_str == 'w')
			mode |= BniFileMode_write;
		if (*mode_str == 'a')
			mode |= BniFileMode_append;
		++mode_str;
	}
	return mode;
}

internal void
OS_assert(i8 *expr, i8* file, u32 line)
{
	i8 nbr[ASSERT_LINENO_BUFSIZE];

	Str8 expr_str = str_wrap(expr);
	Str8 file_str = str_wrap(file);

	u32 len = 12 + expr_str.len + file_str.len;

	Arena *scratch = get_scratch(0);
	Str8 *msg = str_alloc(scratch, len);
	memset(msg->str, 0, len);
	
	str_append(msg, &expr_str);
	str_append8(msg, " [");
	str_append(msg, &file_str);

	i8 *p = nbr + ASSERT_LINENO_BUFSIZE - 1;
	*p = 0;
	--p;
	while(line > 0) {
		u32 digit = line % 10;
		line /= 10;
		*p-- = '0' + digit;
	}
	*p = ':';
	str_append8(msg, p);
	str_append8(msg, "]");

	Str16 *msg16 = str8_to16p( msg, scratch);
	str16_zero_terminate(msg16);

#if defined(WIN_CONSOLE)
	DWORD written = 0;
	HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);
	WriteConsoleW(hStderr, msg16->str, msg16->len, &written, (void*)0);
	ExitProcess(3);
#else
	i32 ret = MessageBoxW(NULL, msg16->str, L"Assert"
		, MB_ABORTRETRYIGNORE
		| MB_SETFOREGROUND
		| MB_TASKMODAL
		| MB_ICONERROR);
	if (ret == IDABORT)
		ExitProcess(3);
#endif

	release_scratch(scratch);
}

internal OSReturn
OS_get_command_line(Arena *arena)
{
	OSReturn res = {0};	
	// TODO(bjorn): Handle errors if any 
	Str8 *cmd8 = win32_get_command_line(arena);
	res.handle = cmd8;
	return res;
}

internal OSFile
OS_get_stdout()
{
	OSFile result = {0};
	result.handle = GetStdHandle(STD_OUTPUT_HANDLE);
	return result;
}

internal OSFile
OS_get_stderr()
{
	OSFile result = {0};
	result.handle = GetStdHandle(STD_ERROR_HANDLE);
	return result;
}

internal u64
OS_file_get_size(OSFile file)
{
	LARGE_INTEGER size = {0};
	if(!GetFileSizeEx(file.handle, &size))
	{
		file.error = mk_os_error(OSError_File, GetLastError());
	}
	return (u64)size.QuadPart;
}

internal umm 
OS_read_file(OSFile file, u8 *buf, umm size)
{
	DWORD bytes_read;
	if (!ReadFile(file.handle, buf, size, &bytes_read, 0))
	{
		file.error = mk_os_error(OSError_File, GetLastError());
	}
	return (u32)bytes_read;
}

internal umm 
OS_write_file(OSFile file, u8* data, umm size)
{
	DWORD bytes_written;
	if (!WriteFile(file.handle, data, size, &bytes_written, 0))
	{
		file.error = mk_os_error(OSError_File, GetLastError());
	}
	return (u32)bytes_written;
}

OSReturn
OS__expand_path(Arena *mem, Str16 *api_path, Str8 *path)
{
	OSReturn ret = {0,0};

	Str8 slash  = str_lit("/");
	Str8 bslash = str_lit("\\");

	Str8 *app_path = str_dup(mem, path);
	str_replace(app_path, &slash, &bslash, 0); 
	
	u32 ec = win32_path_expand(mem, api_path, app_path);
	if (ec != ERROR_SUCCESS)
		ret.error = mk_os_error(OSError_InvalidPath, ec);

	return ret;
}

internal OSFile
OS_open_file(Str8 *path, i8 *mode)
{
	OSFile file = {0};
	u32 error = 0;

	u32 file_mode = OS__file_mode(mode);

	Arena *scratch = get_scratch(0);
	Str16 api_path = {0};
	OSReturn ret = OS__expand_path(scratch, &api_path, path);
	if (is_os_error(ret))
		goto cleanup;

	str16_zero_terminate(&api_path);
	file.handle = win32_open_file(&api_path, file_mode, &error);
	if (error != NO_ERROR) 
		file.error = mk_os_error(OSError_File, error);

cleanup:
	release_scratch(scratch);

	return file;
}

internal void
OS_close_file(OSFile file)
{
	win32_close_handle(file.handle);
}

// 100-nanosecond intervals since 1601-01-01 00:00 in UTC
internal u64
OS_systime(void)
{

	u64 result;
	FILETIME ft = {0};
	GetSystemTimeAsFileTime(&ft);
	result = ft.dwHighDateTime;
	result = (result << 32) | ft.dwLowDateTime;
	return result; 
}

internal u32
OS_unix_systime(void)
{
	SYSTEMTIME st = {1970,1,4,1,0,0,0,0};
	FILETIME ft = {0};
	SystemTimeToFileTime(&st, &ft);

	u64 unix_epoch_start = ((u64)ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
	u64 result = OS_systime() - unix_epoch_start;
	result /= 100000000;
	return result;
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
