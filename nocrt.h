/* nocrt.h - v0.90 - MIT - Björn Nilsson, 2019 
 * 
 * LICENSE
 *   See end of file for license information.
 */

#ifndef NO_CRT_H_INCLUDED
#define NO_CRT_H_INCLUDED


#if defined(NOCRT_IMPLEMENTATION)


#ifndef CONFIG_ARENA_SIZE
#define CONFIG_ARENA_SIZE kb(64)
#endif

// TODO(bjorn): Properly document why _fltused needs to be defined 
int _fltused;

/* Arena for storing configuration information
 * The first thing on this arena will be the argv and its content
 */
global Arena config;

i32 main(i32 argc, char **argv);

#if defined(WIN_CONSOLE)
void __stdcall mainCRTStartup()
#else
void __stdcall WinMainCRTStartup()
#endif
{
	config = arena_reserve(CONFIG_ARENA_SIZE);

	tls_alloc();	
	setup_thread_context();	


	OSReturn ret = OS_get_command_line(&config);
	abort_if_os_error(ret);

	Str8 *cmd8 = (Str8*)ret.handle;
	arena_push_align(&config, sizeof(void*));
	i8 *args = arena_push_array(&config, i8, cmd8->len + 1);
	arena_push_align(&config, sizeof(void*));
	i8 **argv = (i8**)arena_push_array(&config, i8*, 0);
	i8 **argv_ind = 0;
	u32 argc = 0;
	i8 *src = cmd8->str;

	i8 *dst = args;
	i8 *beg_arg = args;

	bool in_string = false;
	while (*src != '\0')
	{
		if (*src == '"')
		{
			++src;
			in_string = !in_string;
		}
		else if (in_string || *src != ' ')
		{
			*dst++ = *src++;
		}
		else
		{
			++argc;
			*dst++ = '\0';
			while (*src == ' ')
				++src;

			argv_ind = (i8**)arena_push_array(&config, i8*, 1);
			*argv_ind = beg_arg;
			beg_arg = dst;
		}
	}
	++argc;	       // account for last arg
	*dst++ = '\0'; // zero terminate last string


	arena_push_align(&config, 8);
	argv_ind = (i8**)arena_push_array(&config, i8*, 1);
	*argv_ind = beg_arg;

	argv_ind = (i8**)arena_push_array(&config, i8*, 1);
	*argv_ind = 0;

	int Result = 0;
#if defined(WIN_CONSOLE)
	Result = main(argc,argv);
#else
	HMODULE hInstance = GetModuleHandleW(NULL);
	STARTUPINFOW info = {0};
	GetStartupInfoW(&info);

	i32 cmd_show = SW_SHOWNORMAL;	
	if (info.dwFlags & STARTF_USESHOWWINDOW)
		cmd_show = info.wShowWindow;
	Result = bncWinMain(hInstance, NULL, argc, argv, cmd_show);
#endif

	teardown_thread_context();	
	tls_free();	
	ExitProcess(Result);
}

#endif //NOCRT_IMPLEMENTATION 


#endif // NO_CRT_H_INCLUDED

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
