/* win32.c - v0.90 - MIT - Björn Nilsson, 2019
 *
 * LICENSE
 *   See end of file for license information.
 */
enum OSNamespace_Type
{
	OSNamespaceType_NONE,
	OSNamespaceType_DiskDesignator,
	OSNamespaceType_Relative,
	OSNamespaceType_UNC,
	OSNamespaceType_File,
	OSNamespaceType_Device,
	OSNamespaceType_Error,
	OSNamespaceType_COUNT
};
typedef enum OSNamespace_Type OSNamespace_Type;

#define str16__first_free_char(a) ((a)->str + (a)->len)
#define str16__free_chars_left(a) (((a)->cap - (a)->len))

static inline void
win32__str16_append_8(Str16 *dst, i8 *src_p, u32 src_len)
{
	umm len = utf_8_to_16le(str16__first_free_char(dst)
								  , str16__free_chars_left(dst)
								  , src_p, src_len);
	dst->len += len;
}

Str8 *
win32_get_command_line(Arena *arena)
{
	Str16 cmd16 = str16_wrap16(GetCommandLineW());
	Str8 *cmd8 = str16_to8p(&cmd16, arena);
	return cmd8;
}

OSNamespace_Type
bni_win32__get_path_namespce(Str8 *path)
{
	OSNamespace_Type res = OSNamespaceType_NONE;	
	i8 *p = path->str;

	if ((p[0] == '\\') && (p[1] == '\\')){
		if ((p[2] == '?') && (p[3] == '\\')){ 
			res = OSNamespaceType_File;
			p += 4;
		}
		else if ((p[2] == '.') && (p[3] == '\\')){ 
			res = OSNamespaceType_Device;
			p += 4;
		}
		else{
			res = OSNamespaceType_UNC;
			p += 2;
		}
	}
	else if ((   ((p[0] >= 'A') && (p[0] <= 'Z'))
				 || ((p[0] >= 'a') && (p[0] <= 'z'))
			 ) && (p[1] == ':')){
		res = OSNamespaceType_DiskDesignator;
		p += 2;
	} 
	else if ( p[0] == '.'){
		if ((p[1] == '\\') || ((p[1] == '.') && (p[2] == '\\'))){
			res = OSNamespaceType_Relative;
		}
		else {
			res = OSNamespaceType_NONE;
		}
	}
	else{
		res = OSNamespaceType_NONE;
	}
	return res;
}

u32
win32_path_expand(Arena *mem, Str16 *os_path, Str8 *app_path)
{
	i8 *p = app_path->str;
	i8 *p_end = p + app_path->len;

	OSNamespace_Type ns = bni_win32__get_path_namespce(app_path);
	// NOTE(bjorn): Intentional falltrhoug to advance pointer
	// past the namespace part of the path
	switch(ns) {
	case OSNamespaceType_Device: 
	case OSNamespaceType_File: 
		p += 2;
	case OSNamespaceType_DiskDesignator: 
	case OSNamespaceType_UNC: 
		p += 2;
	default:
		;
	}

	// NOTE(bjorn): Maybe we could make this SIMD to test 2
	// bytes from the string against all 8 reserved chars
	// in one go. That would mean app_path.len >> 2 comparations
	// instead of app_path.len * 8 comaprations thats a reduction
	// of x16
	// abcdefgh|abcdefgh
	// 11111111|22222222

	i8 *reserved = "<>:\"|?*";
	i8 *p_sav = p;
	i32 reserved_len = non_zero_bytes8(reserved);
	for (;p < p_end; ++p) {
		for (i32 i=0; i < reserved_len; ++i)
			break;
	}

	if(p != p_end)
		return ERROR_BAD_PATHNAME;

	p = p_sav;
	umm os_cap = 0;
	Str16 prefix_dir = {0};

	// NOTE(bjorn): Expand relative paths. Not safe in multithreaded code
	// if any of the threads set the current directory. If not this should
	// be fine.
	if (ns == OSNamespaceType_NONE || ns == OSNamespaceType_Relative){
		Str16 working_dir = {0};
		PlatformError pe = win32_get_current_directory(mem, &working_dir);
		assert(pe == NO_ERROR);
		prefix_dir = working_dir; 
		while(p[0] == '.'){
			if  (p[1] == '\\') {
				p += 2;
			}
			else if ((p[1] == '.') && (p[2] == '\\')){
				prefix_dir = str16_dirname(&prefix_dir);
				p += 3;
			}
			else
				++p;
		}

		// NOTE(bjorn): rewind dots if the where not part of '.\\' or '..\\'
		// constructs
		while (p > app_path->str && *(p-1) == '.')
			--p;
	}	

	Str16 file_ns = str16_lit(L"\\\\?\\");
	u32 app_len = 0;
	if(ns == OSNamespaceType_DiskDesignator || ns == OSNamespaceType_UNC) {
		app_len = app_path->len;
		os_cap = utf_8_to_16le_size(p, app_len);
	}
	else {
		app_len = app_path->len - (p - app_path->str);
		os_cap = utf_8_to_16le_size(p, app_len);
	}
	os_cap >>= 1;
	os_cap += prefix_dir.len + file_ns.len + 2; // slash and zero termination

	Str16 *api_path = str16_alloc(mem, os_cap);
	memset(api_path->str, 0, os_cap * 2);

	if (ns != OSNamespaceType_File && ns != OSNamespaceType_Device){

		str16_append(api_path, &file_ns);
		if(ns == OSNamespaceType_DiskDesignator || ns == OSNamespaceType_UNC) {
			win32__str16_append_8(api_path, p - 2, 2); 
		}
		else if (ns == OSNamespaceType_NONE || ns == OSNamespaceType_Relative)
			str16_append(api_path, &prefix_dir);

		if (*(api_path->str + api_path->len) != '\\' && *p != '\\') {
			*(api_path->str + api_path->len++) = '\\';
		}

		win32__str16_append_8(api_path, p, app_path->len - (p - app_path->str));
		os_cap = utf_8_to_16le_size(p, app_len);
	}
	else if (ns == OSNamespaceType_File || ns == OSNamespaceType_Device){
		// NOTE(bjorn): File and device are passed through unchanged lenght
		// wise so lenght of file_ns should compensate for any advancement
		// of p for these kinds up to this point. 
		win32__str16_append_8(api_path, app_path->str, app_path->len);
	}

	*os_path = *api_path;

	return ERROR_SUCCESS;
}

PlatformError
win32_get_current_directory(Arena *mem, Str16 *working_dir)
{
	u32 cap = GetCurrentDirectoryW(0, NULL);
	*working_dir = str16_init(mem, cap);

	working_dir->len = GetCurrentDirectoryW(working_dir->cap, (LPWSTR)working_dir->str);
	if (working_dir->len == 0)
	  	return GetLastError(); 	

	assert(working_dir->cap == working_dir->len + 1);

	return ERROR_SUCCESS;
}

PlatformError
win32_get_file_attributes(Str16 *path, u32 *file_attr)
{
	*file_attr = GetFileAttributesW((LPCWSTR)path->str);
	if (*file_attr == INVALID_FILE_ATTRIBUTES)
		return GetLastError();

	return ERROR_SUCCESS;
}

PlatformError
win32_create_directory(Str16 *path)
{
	// NOTE(bjorn): Maybe handle security attributes in the future	
	if (CreateDirectoryW((LPCWSTR)path->str, NULL) == false) {
		return GetLastError();
	}

	return ERROR_SUCCESS;
}

HANDLE
win32_open_file(Str16 *file_name, BniFileMode file_mode, u32 *error)
{
	u32 access = 0;
	u32 create = 0;
	HANDLE file = INVALID_HANDLE_VALUE;

	if (file_mode == BniFileMode_read)
	{
		access |= GENERIC_READ;
		create |= OPEN_EXISTING;
	}
	else if (file_mode == BniFileMode_write)
	{
		access |= GENERIC_WRITE;
		create |= CREATE_ALWAYS;
	}
	else if ((file_mode & (BniFileMode_write | BniFileMode_append))
			== (BniFileMode_write | BniFileMode_append))
	{
		access |= (GENERIC_READ | GENERIC_WRITE);
		create |= OPEN_EXISTING;
	}


	file = CreateFileW(file_name->str, access, FILE_SHARE_READ | FILE_SHARE_WRITE
			, 0, create, FILE_ATTRIBUTE_NORMAL, 0);

	if (file == INVALID_HANDLE_VALUE)  *error = GetLastError();

	return file;
}

internal void
win32_close_handle(HANDLE handle)
{
	BOOL result = CloseHandle(handle);
	assert(result);
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
