#pragma region Includes

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "console.h"

#pragma endregion

void InitWin32Console()																//For Console Window 
{
	static const WORD MAX_CONSOLE_LINES = 500;
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
																
	AllocConsole();																	// allocate a console for this app
																
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);			// set the screen buffer to be big enough to let us scroll text
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
														
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);								// redirect unbuffered STDOUT to the console
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
	
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);								// redirect unbuffered STDIN to the console
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );
	
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);								// redirect unbuffered STDERR to the console
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
	
	std::ios::sync_with_stdio();													// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
																					// point to console as well
}