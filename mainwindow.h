//COPYRIGHT AND PERMISSION NOTICE
//
//Copyright (c) 1999,2000,2001 Brian Bennewitz
//
//All rights reserved.
//
//Permission is hereby granted, free of charge, to any person obtaining a
//copy of this software and associated documentation files (the
//"Software"), to deal in the Software without restriction, including
//without limitation the rights to use, copy, modify, merge, publish,
//distribute, and/or sell copies of the Software, and to permit persons
//to whom the Software is furnished to do so, provided that the above
//copyright notice(s) and this permission notice appear in all copies of
//the Software and that both the above copyright notice(s) and this
//permission notice appear in supporting documentation.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
//OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
//HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
//INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
//FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
//NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
//WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
//Except as contained in this notice, the name of a copyright holder
//shall not be used in advertising or otherwise to promote the sale, use
//or other dealings in this Software without prior written authorization
//of the copyright holder.

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>
#include <string.h>
#include <stdio.h>
#include <commctrl.h>
#include <objbase.h>
#include <shlobj.h>

#include "resource.h"

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;
#include "bstring.h"
#include "bvect.h"
#include "japanese.h"
#include "constants.h"
#include "structs.h"
#include "globals.h"
#include "hex.h"
#include "progress.h"
#include "strutil.h"
#include "table.h"

#include "filevect.cpp"

HINSTANCE hMainInstance;
LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);

//FILE MENU
BOOL CALLBACK OpenPartiallyDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TmplDisplayDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//SELECTION MENU
BOOL CALLBACK CutDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CopyDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FastPasteDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PasteDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SelectBlockDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FillWithDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CopyHexdumpDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//OFFSET MENU
BOOL CALLBACK GoToDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BitManipDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ChooseDiffDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//INSERT MENU
BOOL CALLBACK EnterDecimalValueDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AppendDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//SEARCH MENU
BOOL CALLBACK FindDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindWithTableDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReplaceDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReplaceWithTableDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SearchRelativeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ScanRelativeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ValScanRelativeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//BOOKMARK MENU
BOOL CALLBACK AddBookmarkDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RemoveBookmarkDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//SCRIPT MENU
BOOL CALLBACK TableConvertDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ScriptDumpDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ScriptReplaceDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ViewScriptDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//OPTIONS MENU
BOOL CALLBACK ViewSettingsDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CharacterSetDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ColorSettingsDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BinaryModeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OffsetModeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//HELP MENU
BOOL CALLBACK AboutDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
//TOOL BAR
BOOL CALLBACK ToolBarDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK IMEBarDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ProgressDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK InfoDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam);

int iNumInstances;

#define IN_BOUNDS( i, a, b ) ( ( i >= a && i <= b ) || ( i >= b && i <= a ) )
#define bitval(base,pos) ((base)[(pos)/8]&(1<<((pos)%8)))

int find_bytes_relatively (char* ps, int ls, vector<relation>& offsets, int findstringlength, int mode);
int find_bytes (char* ps, int ls, char* pb, int lb, int mode, char (*cmp) (char));
HRESULT ResolveIt( HWND hwnd, LPCSTR lpszLinkFile, LPSTR lpszPath );

void TextToClipboard( char* pcText )
{
	
	int len = 1 + strlen( pcText );
	HGLOBAL hGlobal = GlobalAlloc( GHND, len );
	if( hGlobal != NULL )
	{
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		char* pd = (char*) GlobalLock (hGlobal);
		if( pd )
		{
			
			strcpy( pd, pcText );
			GlobalUnlock (hGlobal);
			if(OpenClipboard(NULL))
			{
				
				EmptyClipboard();
				SetClipboardData (CF_TEXT, hGlobal);
				CloseClipboard (); 
			}
			else 
				MessageBox (NULL,"Cannot get access to clipboard.","Copy",MB_OK | MB_ICONERROR);
		}
		else
		{
			GlobalFree(hGlobal);
			MessageBox (NULL,"Cannot lock clipboard.","Copy",MB_OK | MB_ICONERROR);
		}
		SetCursor (LoadCursor (NULL, IDC_ARROW));
	}
	else
		MessageBox (NULL, "Not enough memory for copying.", "Copy", MB_OK | MB_ICONERROR);
}

void MessageCopyBox(HWND hWnd, LPTSTR lpText, LPCTSTR lpCaption, UINT uType, HWND hwnd);

class Translhextion
{
public:
	
	int iWindowShowCmd;
	int iWindowX;
	int iWindowY;
	int iWindowWidth;
	int iWindowHeight;
	int	iCurrentLine;
	__int64 iCurrentByte;
	__int64 iCurrentNibble;
	Translhextion();
	~Translhextion();
	//MESSAGE PROCESSING
	inline int OnWndMsg( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam );
	int dropfiles( HANDLE hDrop );
	//HELPER FUNCTIONS
	inline char TranslateAnsiToOem (char c);
	void draw_border_client_rect (HDC hdc);
	void destroy_backbuffer ();
	void adjust_view_for_selection ();
	void update_size_change ();
	void wread_table_file(fstream& fs);
	void adjust_view_for_cursor ();
	void print_line( HDC hdc, int iLine, char* linebuffer, HBRUSH hbr );
	void invert_char (HDC hdc);
	void thingy_view_invert_char (HDC hdc, int iLine);
	void update_horizontal_scrollbar ();
	void update_vertical_scrollbar ();
	void clear_all ();
	void update_window_status ();
	void place_cursor ();
	int timer (WPARAM w, LPARAM l);
	int make_font ();
	int make_jfont ();
	int close ();
	int translate_text_to_binary( string& in, FileVector<char>& out );
	int	translate_binary_to_text( string& dest, char* src, int len );
	int find_byte_pos (char* src, char c);
	int find_and_select_data( string& finddata, int finddir, int do_repaint, char (*cmp) (char) );
	int replace_selected_data( string& replacedata, int do_repaint = TRUE );
	int save_ini_data ();
	int read_ini_data ();
	int get_diffs (char* ps, int sl, char* pd, int dl, intpair* pdiff);
	int compare_arrays (char* ps, int sl, char* pd, int dl);
	int initmenupopup (WPARAM w, LPARAM l);
	int repaint (int iLine=-1);
	int load_file (char* fFileName);
	int file_can_load (char* fFileName);
	int on_window_init (HWND hw, HINSTANCE hI);
	int resize_window (int cx, int cy);
	int set_focus ();
	int kill_focus ();
	int lbuttondown (int xPos, int yPos);
	int keydown (int key);
	int character (char ch);
	int vertical_scroll (int cmd, int pos);
	int horizontal_scroll (int cmd, int pos);
	int paint ();
	int command (int cmd);
	int destroy_window ();
	string create_tablefind_string(string x);
	//BYTECODE
	int byte_to_bytecode_destinationlength (char* src, int iLengthgthOfSource);
	int translate_bytes_to_bytecode (char* pd, unsigned char* src, int iLengthgthOfSource);
	int is_bytecode (char* src, int len);
	int calc_bytecode_translation_destinationlength (char* src, int iLengthgthOfSource);
	int translate_bytecode (char* dest, char* src, int iLengthgthOfSource, int binmode=LITTLE_ENDIAN);
	int bytecode_translate (char* dest, char* src, int iLengthgthOfSource, int charmode=ANSI_FIXED_FONT, int binmode=LITTLE_ENDIAN);
	int bytecode_translate (char** ppd, char* src, int iLengthgthOfSource, int charmode=ANSI_FIXED_FONT, int binmode=LITTLE_ENDIAN);
	//LISTS
	int make_most_recent_list (HMENU menu);
	int update_most_recent ();
	int make_most_recent_table_list (HMENU menu);
	int update_most_recent_table ();
	int make_bookmark_list (HMENU menu);
	int make_scriptmark_list (HMENU menu);
	//TEMPLATES
	int ignore_non_code( char* pcTpl, int tpl_len, int& index );
	int read_tpl_token( char* pcTpl, int tpl_len, int& index, char* name );
	int apply_template_on_memory( char* pcTpl, int tpl_len, FileVector<char>& ResultArray );
	int apply_template( char* pcTemplate );
	//COMMAND
	int mousemove (int xPos, int yPos);
	int lbuttonup (int xPos, int yPos);
	int rbuttonup (int xPos, int yPos);
	int on_backspace ();
	int on_deletekey ();
	int select_with_arrowkeys (int key);
	//FILE
	int _New ();
	int open ();
	int save ();
	int save_as ();
	int open_partially ();
	void revert();
	void deletefile();
	int properties ();
	int apply_template();
	int summon_text_edit();
	int most_recent_selection (int cmd);
	//SELECTION
	int edit_cut ();
	int edit_copy ();
	int thingy_copy ();
	int fast_paste ();
	int edit_paste ();
	int select_all ();
	int select_block ();
	void fill_with();
	int copy_hexdump ();
	void save_selection_as();
	//OFFSET
	int _Goto();
	int manipulate_bits ();
	int compare ();
	//INSERT
	int toggle_insertmode ();
	int edit_enterdecimalvalue ();
	int edit_append ();
	void insertfile();
	//SEARCH
	int find ();
	int find_with_table();
	int findnext();
	int findnext_relative();
	int findprev();
	int findprev_relative();
	void replace();
	void replace_with_table();
	relativescandata guess_table_values(long start);
	int search_relative();
	void save_relative_results(fstream& fs);
	long scan_relative_helper(int iLengthgthOfSource);
	int scan_relative();
	int value_scan_relative();
	//BOOKMARK
	int add_bookmark ();
	int remove_bookmark ();
	int clear_all_bookmark ();
	int goto_bookmark (int cmd);
	//SCRIPT
	int open_table();
	int most_recent_table_selection (int cmd);
	int convert_table();
	int script_dump ();
	int replace_script ();
	void optimize_script_file();
	//OPTIONS
	int view_settings ();
	int color_settings ();
	int binarymode ();
	int offsetmode ();
	//SCROLL
	//HELP
	int iGetStartOfSelection()
	{
		if( iStartOfSelection < iEndOfSelection )
			return iStartOfSelection;
		else
			return iEndOfSelection;
	}

	int iGetEndOfSelection()
	{
		if( iStartOfSelection < iEndOfSelection )
			return iEndOfSelection;
		else
			return iStartOfSelection;
	}

private:
	int bDontMarkCurrentPos;
	int bInsertingHex;
	int iHexWidth;
	int bReadOnly;
	int bOpenReadOnly;
	int iPartialOffset;
	int bPartialOpen;
	int iBookmarkCount;
	int iMostRecentCount;
	int iMostRecentTableCount;
	int bFilestatusChanged;
	int iUpdateLine;
	int bTimerSet;
	int iMouseX;
	int iMouseY;
	int iBinaryMode;
	int bUnsignedView;
	int iFontSize;
	int iInsertMode;
	int iCharacterSet;
	int iTextColor;
	int iBkColor;
	int iTextColorE;
	int iTextColorO;
	int bFileNeverSaved;
	int bSelected;
	int bLButtonIsDown;
	int iLBDownX;
	int iLBDownY;
	int iStartOfSelection;
	int iEndOfSelection;
	int iTextEntryMode;
	int iFileChanged;
	int cCharXDim;
	int cxCaps;
	int cCharYDim;
	int cClientXDim;
	int cClientYDim;
	int cxBuffer;
	int cBufferY;
	int iNumlines;
	int iVerticalScrollMax;
	int iVerticalScrollPos;
	int iVerticalScrollInc;
	int	iHorizontalScrollMax;
	int iHorizontalScrollPos;
	int iHorizontalScrollInc;
	int iOffsetLength;
	int iByteSpace;
	int iCharSpace;
	int iCharsPerLine;
	char strMostRecent[iMostRecentMaximum][_MAX_PATH+1];
	char strMostRecentTable[iMostRecentTableMaximum][_MAX_PATH+1];
	char *filename;
	char *filename2;
	bookmark pbmkList[iBookmarkMax];
	HFONT hFont;
	COLORREF iDTEColorValue;
	COLORREF iChangesColorValue;
	COLORREF iHighlightColorValue;
	COLORREF iTextColorEValue;
	COLORREF iTextColorOValue;
	COLORREF iTextColorValue;
	COLORREF iBkColorValue;
	COLORREF iSepColorValue;
	COLORREF iSelBkColorValue;
	COLORREF iSelBkColor;
	COLORREF iSelTextColorValue;
	COLORREF iSelTextColor;
	COLORREF iBookmarkColor;
	string sTexteditorName;
	FileVector<char> Linebuffer;
	FileVector<unsigned char> FileBuffer;
	HWND hwnd, hwndStatusBar, hwndHBar, hwndVBar, hwndTBar;
	HINSTANCE hInstance;
};

Translhextion mainwindow;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, char* szCmdLine, int iCmdShow)
{
	hMainInstance = hInstance;

	// Enable DPI awareness for correct rendering on Windows 7/10/11 high-DPI displays.
	// Uses runtime lookup so the binary stays compatible with older systems.
	{
		HMODULE hUser32 = GetModuleHandle("user32.dll");
		if (hUser32)
		{
			typedef BOOL (WINAPI* PFN_DPI)(void);
			PFN_DPI pfn = (PFN_DPI)GetProcAddress(hUser32, "SetProcessDPIAware");
			if (pfn) pfn();
		}
	}

	HACCEL hAccel;

	static char szAppName[] = "Translhextion window";
	HWND hWnd;
	MSG msg;
	WNDCLASSEX window;

	window.cbSize = sizeof (window);
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = MainWndProc;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = hInstance;
	window.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON1));
	window.hCursor = LoadCursor( NULL, IDC_ARROW );
	window.hbrBackground = NULL;
	window.lpszMenuName = MAKEINTRESOURCE (IDR_MAINMENU);
	window.lpszClassName = szAppName;
	window.hIconSm =(HICON) LoadImage (hInstance, MAKEINTRESOURCE (IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	RegisterClassEx (&window);

	hWnd = CreateWindow (szAppName,
		"Translhextion window",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		mainwindow.iWindowX,
		mainwindow.iWindowY,
		mainwindow.iWindowWidth,
		mainwindow.iWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow( hWnd, mainwindow.iWindowShowCmd );
	UpdateWindow (hWnd);

	if( szCmdLine != NULL && strlen( szCmdLine ) != 0 )
	{
		char sz[MAX_PATH];
		char* p = szCmdLine;
		strncpy( sz, szCmdLine, sizeof( sz ) );
		for( int i = 0; i < MAX_PATH; ++i )
		{
			char c = sz[i];
			if( c == 0 )
			{
				*p = 0;
				break;
			}
			else if( c != '"' )
			{
				*p++ = c;
			}
		}
		
		char lpszPath[MAX_PATH];
		HRESULT hres = ResolveIt( hWnd, szCmdLine, lpszPath );
		if( SUCCEEDED( hres ) )
		{
			int ret = MessageBox( hWnd,
				"You are trying to open a link file.\n"
				"Click on Yes if you want to open the file linked to,\n"
				"or click on No if you want to open the link file itself.\n"
				"Choose Cancel if you want to abort opening.",
				"Translhextion", MB_YESNOCANCEL | MB_ICONQUESTION );
			switch( ret )
			{
			case IDYES:
				mainwindow.load_file( lpszPath );
				break;
			case IDNO:
				mainwindow.load_file( szCmdLine );
				break;
			case IDCANCEL:
				break;
			}
		}
		else
		{
			mainwindow.load_file( szCmdLine );
		}
	}

	hAccel = LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_ACCELERATOR1));

	while (GetMessage (&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator (hWnd, hAccel, &msg))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK MainWndProc( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return mainwindow.OnWndMsg( hwnd, iMsg, wParam, lParam );
}
