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

#include "mainwindow.h"
#include "fstream.h"
#include <string.h>

//takes filestream
//reads in table values
void Translhextion::wread_table_file(fstream& fs)
{
	bReadingTable = true;
	clear_all_bookmark ();
	read_table_file(fs, hwnd, pbmkList, iBookmarkCount);
}

template<class T>void sswap(T& x, T& y){
	T temp = x;
	x = y;
	y = temp;
}

BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
	char cBuf[64];
	if( GetClassName( hwnd, cBuf, 64 ) != 0 )
	{
		if( strcmp( cBuf, (sName + " wndclass").c_str() ) == 0 )
		{
			iNumInstances++;
		}
	}
	return TRUE;
}

inline char equal (char c)
{
	return c;
}

inline char lower_case (char c)
{
	if (c >= 'A' && c <= 'Z')
		return 'a' + c - 'A';
	else
		return c;
}
void count_instances()
{
	iNumInstances = 0;
	EnumWindows( (WNDENUMPROC) EnumWindowsProc, 0 );
}

Translhextion::Translhextion ()
{
	char * acPath;
	acPath = new char[MAX_PATH];
	if(GetModuleFileName(NULL,acPath,MAX_PATH) != 0)
	{
		// guaranteed file name of at least one character after path 
		* (strrchr(acPath,'\\')+1) = '\0';
		//	AfxMessageBox(acPath);   // Use it 
	}
	sAppPath = (string) acPath;
	delete [] acPath;
	char *winpath;
	winpath = new char[MAX_PATH];
	GetWindowsDirectory(winpath,MAX_PATH);
	sWindowsPath = (string) winpath;
	delete [] winpath;
	bDontMarkCurrentPos = FALSE;

	bInsertingHex = FALSE;

	sBrowserName = "iexplore";
	sTexteditorName = "notepad.exe";

	iWindowX = CW_USEDEFAULT;
	iWindowY = CW_USEDEFAULT;
	iWindowWidth = CW_USEDEFAULT;
	iWindowHeight = CW_USEDEFAULT;
	iWindowShowCmd = SW_SHOW;
	IMEPos.left = 0;
	IMEPos.right = 0;
	IMEPos.top = 0;
	IMEPos.bottom = 0;
	iHexWidth = 3;
	iChangesColorValue = cChangesColor;
	iHighlightColorValue = cHighlightColor;
	iBookmarkColor = cBookmarkColor;
	iSelBkColorValue = cSelBkColor;
	iSelTextColorValue = cSelTextColor;
	iTextColorValue = cTextColor;
	iTextColorEValue = cTextColorE;
	iTextColorOValue = cTextColorO;
	iBkColorValue = cBkColor;
	iSepColorValue = cSepColor;
	iDTEColorValue = cDTEColor;
	pcGotoDialogBuffer = NULL;
	iGotoDialogBufLength = 0;
	bOpenReadOnly = bReadOnly = FALSE;
	iPartialOffset=0;
	bPartialOpen=FALSE;
	iBookmarkCount=0;
	int i;
	for (i=1; i<=iMostRecentMaximum; i++)
		sprintf (&(strMostRecent[i-1][0]), "dummy%d", i);
	for (i=1; i<=iMostRecentTableMaximum; i++)
		sprintf (&(strMostRecentTable[i-1][0]), "dummy%d", i);
	iMostRecentCount = 0;
	iMostRecentTableCount = 0;
	bFilestatusChanged = TRUE;
	iBinaryMode = LITTLE_ENDIAN;
	cFileName[0] = '\0';
	bUnsignedView = TRUE;
	iFontSize = 10;
	iInsertMode = FALSE;
	bAutomaticBPL = BST_CHECKED;
	bSelected = FALSE;
	bLButtonIsDown = FALSE;
	iStartOfSelection = 0;
	iEndOfSelection = 0;
	hwnd = 0;
	iOffsetLength = 8;
	iByteSpace = 2;
	iBytesPerLine = 16;
	iCharSpace = 1;
	iCharsPerLine = iOffsetLength + iByteSpace + iBytesPerLine*3 + iCharSpace + iBytesPerLine;
	filename = new char[_MAX_PATH];
	filename2 = new char[_MAX_PATH];
	filename[0] = '\0';
	iTextEntryMode = BYTES;
	iFileChanged = FALSE;
	bFileNeverSaved = TRUE;

	iFindDialogLastLength = 0;
	pcFindDialogBuffer = NULL;
	iFindDialogBufLength = 0;

	iCopyHexdumpDialogStart = 0;
	iCopyHexdumpDialogEnd = 0;
	iCharacterSet = ANSI_FIXED_FONT;
	
	count_instances();
	read_ini_data ();

	bFileNeverSaved = TRUE;
	bSelected = FALSE;
	bLButtonIsDown = FALSE;
	iFileChanged = FALSE;
	iVerticalScrollMax = 0;
	iVerticalScrollPos = 0;
	iVerticalScrollInc = 0;
	iHorizontalScrollMax = 0;
	iHorizontalScrollPos = 0;
	iHorizontalScrollInc = 0;
	iCurrentLine = 0;
	iCurrentByte = 0;
	FileBuffer.Reset ();
	vChanges = vEmpty;
	vHighlights = vEmpty;
	vDTEs = vEmpty;
	FileBuffer.SetExpandBy (100);
	sprintf (filename, "Untitled");
	AddFontResource("Januschan JOME Regular.ttf");
	LoadEncoding();
}

Translhextion::~Translhextion ()
{
	if (hFont != NULL)
		DeleteObject (hFont);
	if(!RemoveFontResource("Januschan JOME Regular.ttf"))
	{
		MessageBox(hwnd,"Japanese font could not be removed from memory!","Garbage collection error:",MB_OK | MB_ICONERROR);
	}
	if (hfJap != NULL)
		DeleteObject (hfJap);
	if (filename != NULL)
		delete [] filename;
	if (pcFindDialogBuffer != NULL)
		delete [] pcFindDialogBuffer;
	if( pcGotoDialogBuffer != NULL )
		delete [] pcGotoDialogBuffer;
}


int Translhextion::load_file (char* fFileName)
{
	if (file_can_load (fFileName))
	{		
		int iFileHandle;
		if ((iFileHandle = _open (fFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			long iFileLength = _filelength (iFileHandle);
			FileBuffer.Reset ();
			vChanges = vEmpty;
			vHighlights = vEmpty;
			vDTEs = vEmpty;
			
			if (FileBuffer.SetLength (iFileLength) == TRUE)
			{
				SetCursor (LoadCursor (NULL, IDC_WAIT));
				vector<string> temp(iFileLength);
				vChanges = temp;
				vHighlights = temp;
				vDTEs = temp;
				//long m = 0;
				//for(m = 0;m<vChanges.size();m++)
				//	vChanges[m] = COLORNORMAL;
				// If read-only mode on opening is enabled:
				if( bOpenReadOnly )
					bReadOnly = TRUE;
				else
					bReadOnly = FALSE;

				if( iFileLength == 0)
				{
					_close( iFileHandle );
					strcpy( filename, fFileName );
					bFileNeverSaved = FALSE;
					bPartialOpen=FALSE;
					update_most_recent ();
					bFilestatusChanged = TRUE;
					update_size_change();
					return TRUE;
				}
				else
				{
					FileBuffer.SetUpperBound (iFileLength-1);
					if (_read (iFileHandle, FileBuffer, FileBuffer.Length ()) != -1)
					{
						_close (iFileHandle);
						strcpy (filename, fFileName);
						bFileNeverSaved = FALSE;
						bPartialOpen=FALSE;
						/*if(bHighlight)
						{
							if(vChanges.size() == 1)
							{
								if(vTwoByteHighlightTable[(unsigned char) FileBuffer[0]] == "1")
								{
									vChanges[0] = COLORHIGHLIGHT;
								}
							}
							else if(vChanges.size() == 2)
							{
								if(vTwoByteHighlightTable[dualtileindex("" + (unsigned char) FileBuffer[0] + (unsigned char) FileBuffer[1])] == "1")
								{
									vChanges[0] = COLORHIGHLIGHT;
									vChanges[1] = COLORHIGHLIGHT;
								}
								else if(vTwoByteHighlightTable[(unsigned char) FileBuffer[0]] == "1")
								{
									vChanges[0] = COLORHIGHLIGHT;
									if(vTwoByteHighlightTable[(unsigned char) FileBuffer[1]] == "1")
									{
										vChanges[1] = COLORHIGHLIGHT;
									}
								}
							}
							else if(vChanges.size() > 2)
							{
								long k = 0;
								string temp;
								for(k = 0;k<vChanges.size()-1;k++)
								{
									temp = "";
									temp += (unsigned char) FileBuffer[k];
									temp += (unsigned char) FileBuffer[k+1];
									//assert(temp.length() == 2);
									if(temp.length() == 2 && vTwoByteHighlightTable[dualtileindex(temp)] == "1")
									{
										vChanges[k] = COLORHIGHLIGHT;
										vChanges[k+1] = COLORHIGHLIGHT;
									}
									else if(vTwoByteHighlightTable[(unsigned char) FileBuffer[k]] == "1")
									{
										vChanges[k] = COLORHIGHLIGHT;
									}
								}
								if(vTwoByteHighlightTable[(unsigned char) FileBuffer[k]] == "1")
								{
									vChanges[k] = COLORHIGHLIGHT;
								}
							}
						}*/
						update_most_recent ();
						bFilestatusChanged = TRUE;
						update_size_change();
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						return TRUE;
					}
					else
					{
						_close (iFileHandle);
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						MessageBox (hwnd, "Error occurred while reading from file.", "File load error:", MB_OK | MB_ICONERROR);
						return FALSE;
					}
				}
				SetCursor (LoadCursor (NULL, IDC_ARROW));
			}
			else
			{
				MessageBox (hwnd, "Not enough memory to load file.", "File load error:", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
		else
		{
			char cBuf[500];
			sprintf (cBuf, "Error code 0x%X occured while opening file %s.", errno, fFileName);
			MessageBox (hwnd, cBuf, "File load error:", MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}
	else
		return FALSE;
}

int Translhextion::file_can_load (char* fFileName)
{
	int iFileHandle;
	if ((iFileHandle = _open (fFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
	{
		_close (iFileHandle);
		return TRUE;
	}
	else
		return FALSE;
}

int Translhextion::on_window_init (HWND hw, HINSTANCE hI)
{
	hwnd = hw;
	hMainWnd = hw;
	hInstance = hI;

	InitCommonControls ();
	hwndStatusBar = CreateStatusWindow (
		 CCS_BOTTOM | WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		"Ready", hwnd, 2);

	hwndHBar = CreateWindow ("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
		0,0,0,0, hwnd, (HMENU) 1, hInstance, NULL);
	hwndVBar = CreateWindow ("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT,
		0,0,0,0, hwnd, (HMENU) 2, hInstance, NULL);

	iVerticalScrollMax = 0;
	iVerticalScrollPos = 0;
	iVerticalScrollInc = 0;
	iHorizontalScrollMax = 0;
	iHorizontalScrollPos = 0;
	iHorizontalScrollInc = 0;

	iCurrentLine = 0;
	iCurrentByte = 0;
	iCurrentNibble = 0;
	
	DragAcceptFiles( hwnd, TRUE ); 
	return TRUE;
}

BOOL CALLBACK IMEBarDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			if(bInsertMode)
				SetWindowText (hDialog, "Thingy IME           MODE=Insert              File updated every 100 bytes or by pressing 'Enter'");
			else
				SetWindowText (hDialog, "Thingy IME           MODE=Overwrite           File updated every 100 bytes or by pressing 'Enter'");
			if(IMEPos.left == 0 && IMEPos.top == 0)
				SetWindowPos(hDialog,HWND_TOP,mainwindow.iWindowX,mainwindow.iWindowY+mainwindow.iWindowHeight-100,0,0,SWP_NOSIZE);// | SWP_NOZORDER);
			else
				SetWindowPos(hDialog,HWND_TOP,IMEPos.left,IMEPos.top,0,0,SWP_NOSIZE);			
			SetFocus (GetDlgItem (hDialog, IDC_EDIT1)); 
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDC_EDIT1:
			{
				bDoingIME = true;
				char cBuf[500];
				GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 500);
				string a = (string) cBuf;
				string b;
				b = "";
				long numbytes = 0;
				//if(a.length() > 0)
				vector<string> phrase;
				int c = 0;
				for(c = 0;c<a.length();c++)
				{
					if(a.substr(c,1) == "\\")
					{
						b += sByteOpen + "END" + sByteClose + "\n";
						phrase.push_back(b);
						phrase.push_back("\n");
						b = "";
					}
					else if(a.substr(c,1) == "*")
					{
						b += "\n";
						phrase.push_back(b);
						b = "";
					}
					else
					{ 
						b += a.substr(c,1);
					}
				}
				phrase.push_back(b);
				bool oldbmark = bByteMark;
				bByteMark = false;
				descript(phrase);
				bByteMark = oldbmark;
				detranslate(phrase);
				string output2 = "";
				for(c = 0;c < phrase.size();c++)
				{
					output2 += phrase[c];
				}
				vIMEBytes = phrase;
				numbytes = phrase.size();
				bByteMark = true;
				bSplitMultiByte = false;
				translate(phrase);
				bSplitMultiByte = true;
				bByteMark = oldbmark;
				string output = "";
				int i = 0;
				for(i = 0;i<phrase.size();i++)
				{
					if(phrase[i] == "\n")
					{
						output += "*";
					}
					else if(phrase[i].length() >= 5 && phrase[i].substr(1,3) == "END")
					{
						output += sByteOpen + "END" + sByteClose;
					}
					else
					{
						output += phrase[i];
					}
				}
				if(output.length() > 50)
				{
					output = output.substr(output.length()-50,50);
				}
				if(output2.length() > 50)
				{
					output2 = output2.substr(output2.length()-50,50);
				}
				SetDlgItemText(hDialog,IDC_INPUT,output.c_str());
				SetDlgItemText(hDialog,IDC_BYTES,(stringvalue(numbytes) + " bytes").c_str());
				SetDlgItemText(hDialog,IDC_HEX,output2.c_str());   
				
				bDoingIME = false;
				if(numbytes >= 100)
				{
					GetWindowRect(hDialog,&IMEPos);
					EndDialog(hDialog,0);
				}
				return TRUE;
			}
		case IDOK:
			{
				bIMEDone = true;
				GetWindowRect(hDialog,&IMEPos);
				EndDialog(hDialog,0);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK ToolBarDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			//if(bTableActive)
			//{
			//	CheckDlgButton( hDialog, IDC_CHECK1, BST_CHECKED );
			//}
			if(bThingyView)
			{
				CheckDlgButton( hDialog, IDC_CHECK2, BST_CHECKED );
			}
			if(bJapanese)
			{
				if(bJPShift)
				{
					CheckDlgButton( hDialog, IDC_RADIO3, BST_CHECKED );
				}
				else
				{
					CheckDlgButton( hDialog, IDC_RADIO4, BST_CHECKED );
				}
			}
			if(!bJPuncSwitch)
			{
				CheckDlgButton( hDialog, IDC_CHECK4, BST_CHECKED );
			}
			if(bDumpClean)
			{
				CheckDlgButton( hDialog, IDC_CHECK3, BST_CHECKED );
			}
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		//case IDC_CHECK1:
		//	{
		//		if(IsDlgButtonChecked(hDialog,IDC_CHECK1))
		//		{
		///			bTableActive = true;
		//		}
		//		else
		//		{
		//			bTableActive = false;
		//		}
		///		SetFocus(GetParent(hDialog));
		//		return TRUE;
		//	}

		case IDC_CHECK2:
			{
			if(IsDlgButtonChecked(hDialog,IDC_CHECK2))
				{
					bThingyView = true;
					if(bAutomaticBPL)
					{
						bAutomaticBPL = false;
						iOldBPL = iBytesPerLine;
						iBytesPerLine = 16;
						bOldAutomaticBPL = true;
					}
					mainwindow.repaint();
					mainwindow.update_size_change();
				}
				else
				{
					bThingyView = false;
					if(bOldAutomaticBPL)
					{
						bAutomaticBPL = true;
						iBytesPerLine = iOldBPL;
						bOldAutomaticBPL = false;
					}
					mainwindow.repaint();
					mainwindow.update_size_change();
				}
				SetFocus(GetParent(hDialog));
			return TRUE;
			}
		case IDC_CHECK6:
			{
				if(IsDlgButtonChecked(hDialog,IDC_CHECK6))
				{
					bJapanese = true;
					bJPShift = true;
					bJPEUC = false;
					CheckDlgButton(hDialog, IDC_RADIO3, BST_CHECKED);
					mainwindow.repaint(); 
					mainwindow.update_size_change();
				}
				else
				{
					bJapanese = false;
					bJPShift = false;
					bJPEUC = false;
					CheckDlgButton(hDialog, IDC_RADIO3, BST_UNCHECKED);
					CheckDlgButton(hDialog, IDC_RADIO4, BST_UNCHECKED);
					mainwindow.repaint(); 
					mainwindow.update_size_change();
				}
				SetFocus(GetParent(hDialog));
			return TRUE;
			}	
			case IDC_RADIO3:
			{
				if(IsDlgButtonChecked(hDialog,IDC_RADIO3))
				{
					bJapanese = true;
					CheckDlgButton(hDialog,IDC_CHECK6,BST_CHECKED);
					bJPShift = true;
					bJPEUC = false;
					mainwindow.repaint(); 
					mainwindow.update_size_change();
				}
				SetFocus(GetParent(hDialog));
			return TRUE;
			}
			case IDC_RADIO4:
			{
				if(IsDlgButtonChecked(hDialog,IDC_RADIO4))
				{
					bJapanese = true;
					CheckDlgButton(hDialog,IDC_CHECK6,BST_CHECKED);
					bJPEUC = true;
					bJPShift = false;
					mainwindow.repaint(); 
					mainwindow.update_size_change();
				}
				SetFocus(GetParent(hDialog));
			return TRUE;
			}
		case IDC_CHECK3:
			{
				if(IsDlgButtonChecked(hDialog,IDC_CHECK3))
				{
					bDumpClean = true;
					vTableFileLeft.push_back("#");
					vTableFileRight.push_back("");
					fstream fs;
					fs.open(sLoadedTable.c_str(),ios::out);
					save_table_file(fs);
					fs.close();
				}
				else
				{
					bDumpClean = false;
					int k = 0;
					for(k = 0;k<vTableFileLeft.size();k++)
					{
						if(vTableFileLeft[k] == "#")
						{
							vTableFileLeft.remove(k);
							vTableFileRight.remove(k);
							break;
						}
					}
					fstream fs;
					fs.open(sLoadedTable.c_str(),ios::out);
					save_table_file(fs);
					fs.close();
				}
				
				SetFocus(GetParent(hDialog));
				return TRUE;
			}
		case IDC_CHECK4:
			{
				if(IsDlgButtonChecked(hDialog,IDC_CHECK4))
				{
					bJPuncSwitch = false;
					vTableFileLeft.push_back(">");
					vTableFileRight.push_back("");
					fstream fs;
					fs.open(sLoadedTable.c_str(),ios::out);
					save_table_file(fs);
					fs.close();
				}
				else
				{
					bJPuncSwitch = true;
					int k = 0;
					for(k = 0;k<vTableFileLeft.size();k++)
					{
						if(vTableFileLeft[k] == ">")
						{
							vTableFileLeft.remove(k);
							vTableFileRight.remove(k);
							break;
						}
					}
					fstream fs;
					fs.open(sLoadedTable.c_str(),ios::out);
					save_table_file(fs);
					fs.close();
				}
				
				SetFocus(GetParent(hDialog));
				return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

int Translhextion::resize_window (int cx, int cy)
{
	HDC hdc = GetDC (hwnd);
	make_font ();
	make_jfont ();
	HFONT of = (HFONT) SelectObject( hdc, hFont );
	TEXTMETRIC tm;
	GetTextMetrics (hdc, &tm);
	cCharXDim = tm.tmAveCharWidth;
	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cCharXDim / 2;
	cCharYDim = tm.tmHeight + tm.tmExternalLeading;
	SelectObject (hdc, of);
	ReleaseDC (hwnd, hdc);
	
	int x, y, x2, y2;
	RECT rWindow;
	GetWindowRect (hwndStatusBar, &rWindow);
	y2 = rWindow.bottom - rWindow.top;
	x = 0;
	y = cy - y2;
	x2 = cx;
	MoveWindow (hwndStatusBar, x, y, x2, y2, TRUE);

	cClientXDim = cx;
	cClientYDim = cy - y2;

	int cyh = GetSystemMetrics (SM_CYHSCROLL),
		cxv = GetSystemMetrics (SM_CXVSCROLL);
	MoveWindow (hwndHBar, 0, cClientYDim - cyh, cClientXDim - cxv, cyh, FALSE);
	MoveWindow (hwndVBar, cClientXDim - cxv, 0, cxv, cClientYDim, FALSE);
	RedrawWindow (hwndHBar, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW | RDW_ALLCHILDREN);
	RedrawWindow (hwndVBar, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW | RDW_ALLCHILDREN);

	cClientXDim -= cxv;
	cClientYDim -= cyh;

	int iStatusBarXDim;
	if (IsZoomed (hwnd))
		iStatusBarXDim = cx;
	else
		iStatusBarXDim = cClientXDim;
    
    HLOCAL hloc = LocalAlloc (LHND, sizeof(int) * 3);
    int* lpParts = (int*) LocalLock(hloc); 
 
    lpParts[0] = iStatusBarXDim*4/6;
	lpParts[1] = iStatusBarXDim*5/6;
	lpParts[2] = iStatusBarXDim;

    SendMessage (hwndStatusBar, SB_SETPARTS, (WPARAM) 3,
        (LPARAM) lpParts);
 
    LocalUnlock(hloc); 
    LocalFree(hloc);
	
	cxBuffer = max (1, cClientXDim / cCharXDim);
	cBufferY = max (1, cClientYDim / cCharYDim);
	if( bAutomaticBPL )
	{
		int bytemax = cxBuffer-iOffsetLength-iByteSpace-iCharSpace;
		iBytesPerLine = bytemax / 4;
		if (iBytesPerLine < 1)
			iBytesPerLine = 1;
		//Autosize alternating colors
		else if(iBytesPerLine % 2 != 0)
			iBytesPerLine--;
	}
	
	if( bSelected )
	{
		if( iEndOfSelection / iBytesPerLine < iCurrentLine || iEndOfSelection / iBytesPerLine > iCurrentLine + cBufferY )
			iCurrentLine = max( 0, iEndOfSelection / iBytesPerLine - cBufferY / 2 );
	}
	else
	{
		if(iBytesPerLine == 0)
			iBytesPerLine = 1;
		if( iCurrentByte/iBytesPerLine < iCurrentLine || iCurrentByte/iBytesPerLine > iCurrentLine + cBufferY )
			iCurrentLine = max( 0, iCurrentByte/iBytesPerLine-cBufferY/2 );
	}
	update_vertical_scrollbar();
	
	if(!bThingyView)
	{
		iCharsPerLine = iOffsetLength + iByteSpace + iBytesPerLine*3 + iCharSpace + iBytesPerLine;
	}
	else
	{
		iCharsPerLine = iOffsetLength + iByteSpace + iBytesPerLine*2 + iMaxCharLine + iCharSpace + iBytesPerLine;
	}
	if ((FileBuffer.Length()+1) % iBytesPerLine == 0)
		iNumlines = (FileBuffer.Length()+1) / iBytesPerLine;
	else
		iNumlines = (FileBuffer.Length()+1) / iBytesPerLine + 1;
	
	if (iNumlines <= 0xffff)
		iVerticalScrollMax = iNumlines-1;
	else
		iVerticalScrollMax = 0xffff;

	SetScrollRange (hwndVBar, SB_CTL, 0, iVerticalScrollMax, FALSE);
	SetScrollPos (hwndVBar, SB_CTL, iVerticalScrollPos, TRUE);
	iHorizontalScrollMax = iCharsPerLine - 1;
	iHorizontalScrollPos = 0;
	SetScrollRange (hwndHBar, SB_CTL, 0, iHorizontalScrollMax, FALSE);
	SetScrollPos (hwndHBar, SB_CTL, iHorizontalScrollPos, TRUE);

	update_window_status ();
	if (hwnd == GetFocus ())
		place_cursor ();
	repaint ();
	return TRUE;
}

int Translhextion::set_focus ()
{
	if (cCharXDim == 0 || cCharYDim == 0)
	{
		make_font ();
		make_jfont ();
		HDC hdc = GetDC (hwnd);
		HFONT of = (HFONT) SelectObject (hdc, hFont);
		TEXTMETRIC tm;
		GetTextMetrics (hdc, &tm);
		cCharXDim = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cCharXDim / 2;
		cCharYDim = tm.tmHeight + tm.tmExternalLeading;
		SelectObject (hdc, of);
		ReleaseDC (hwnd, hdc);
	}
	update_size_change ();
	CreateCaret (hwnd, NULL, cCharXDim, cCharYDim);
	place_cursor ();
	ShowCaret (hwnd);
	return 0;
}

int Translhextion::kill_focus ()
{
	HideCaret (hwnd);
	DestroyCaret ();
	return TRUE;
}



int Translhextion::keydown (int key)
{
	if (filename[0] == '\0' || iCurrentByte<0)
		return 0;

	int iColumn_logical, iScrollFlag;

	if (bSelected)
		switch (key)
		{
		case VK_END:
		case VK_HOME:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
			iCurrentByte = iEndOfSelection;
			bSelected = FALSE;
			update_vertical_scrollbar ();
			repaint ();
		default:
			break;
		}

	switch (key)
	{
	case VK_HOME:
	{
		iCurrentByte = (iCurrentByte / iBytesPerLine) * iBytesPerLine;
		iCurrentNibble = 0;
		if (iTextEntryMode == BYTES)
			iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
		else
			iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));
		if (iColumn_logical < iHorizontalScrollPos) 
		{
			iHorizontalScrollPos = iColumn_logical;
			update_horizontal_scrollbar ();
			repaint ();
		}
		else
		{
			repaint ((iCurrentByte / iBytesPerLine));
		}
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
		{
			iCurrentLine = max (0, iCurrentByte/iBytesPerLine-cBufferY/2);
			update_vertical_scrollbar ();
			repaint ();
		}
		break;
	}
	
	case VK_END:
	{
		iCurrentByte = ((iCurrentByte / iBytesPerLine) + 1) * iBytesPerLine - 1;
		if (iCurrentByte > (FileBuffer.Upper ())+1)
			iCurrentByte = (FileBuffer.Upper ())+1;
		iCurrentNibble = 1;

		if (iTextEntryMode == BYTES)
			iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
		else
			iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));
		
		if (iColumn_logical > (iHorizontalScrollPos + cxBuffer - 1))
		{
			iHorizontalScrollPos = iColumn_logical - cxBuffer + 1;
			update_horizontal_scrollbar ();
			repaint ();
		}
		else
		{
			repaint ((iCurrentByte / iBytesPerLine));
		}
		
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
		{
			iCurrentLine = max (0, iCurrentByte/iBytesPerLine-cBufferY/2);
			update_vertical_scrollbar ();
			repaint ();
		}
		break;
	}

	case VK_UP:
		if (iCurrentByte >= iBytesPerLine)
			iCurrentByte -= iBytesPerLine;
		iScrollFlag = FALSE;
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
		{
			update_vertical_scrollbar ();
			iScrollFlag = TRUE;
		}

		if (iTextEntryMode == BYTES)
			iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
		else
			iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));

		if (iColumn_logical < iHorizontalScrollPos || iColumn_logical > (iHorizontalScrollPos + cxBuffer - 1))
		{
			iHorizontalScrollPos = iColumn_logical;
			update_horizontal_scrollbar ();
			repaint ();
			break;
		}
		else if (!iScrollFlag)
		{
			repaint ((iCurrentByte / iBytesPerLine)+1);
			repaint ((iCurrentByte / iBytesPerLine));
		}
		else 
		{
			if (iCurrentLine-(iCurrentByte / iBytesPerLine)==1) 
			{
				iCurrentLine = (iCurrentByte / iBytesPerLine);
				RECT r;
				r.left = BORDER_CLIENT_XDIM;
				r.top = BORDER_CLIENT_XDIM;
				r.right = cClientXDim - BORDER_CLIENT_XDIM;
				r.bottom = cClientYDim - BORDER_CLIENT_XDIM;
				ScrollWindow (hwnd, 0, cCharYDim, &r, &r);
				repaint ((iCurrentByte / iBytesPerLine)+1);
				repaint ((iCurrentByte / iBytesPerLine));
			}
			else
			{
				iCurrentLine = (iCurrentByte / iBytesPerLine);
				repaint ();
			}
		}
		break;

	case VK_DOWN:
		
		if (iCurrentByte+iBytesPerLine <= FileBuffer.Length())
			iCurrentByte += iBytesPerLine;
		
		iScrollFlag = FALSE;
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
		{
			update_vertical_scrollbar ();
			iScrollFlag = TRUE;
		}

		if (iTextEntryMode == BYTES)
			iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
		else
			iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));

		if (iColumn_logical < iHorizontalScrollPos || iColumn_logical > (iHorizontalScrollPos + cxBuffer - 1))
		{
			iHorizontalScrollPos = iColumn_logical;
			update_horizontal_scrollbar ();
			repaint ();
			break;
		}
		else if (!iScrollFlag)
		{
			repaint ((iCurrentByte / iBytesPerLine)-1);
			repaint ((iCurrentByte / iBytesPerLine));
		}
		else 
		{
			if ((iCurrentByte / iBytesPerLine)==iCurrentLine+cBufferY) 
			{
				iCurrentLine = max (0, (iCurrentByte / iBytesPerLine)-(cBufferY-1)); 
				RECT r;
				r.left = BORDER_CLIENT_XDIM;
				r.top = BORDER_CLIENT_XDIM;
				r.right = cClientXDim - BORDER_CLIENT_XDIM;
				r.bottom = cClientYDim - BORDER_CLIENT_XDIM;
				ScrollWindow (hwnd, 0, -cCharYDim, &r, &r);
				repaint ((iCurrentByte / iBytesPerLine)-1);
				repaint ((iCurrentByte / iBytesPerLine));
				repaint ((iCurrentByte / iBytesPerLine)+1);
				//if(!bThingyView)
				//{
					invert_char( 0 );
				//}
				//else
				//{
				//	thingy_view_invert_char (0);
				//}
				
			}
			else
			{
				iCurrentLine = max (0, (iCurrentByte / iBytesPerLine)-(cBufferY-1));
				repaint ();
			}
		}
		break;
	
	case VK_LEFT:
	{
		if (iTextEntryMode == BYTES) 
		{
			if (iCurrentNibble == 1) 
				iCurrentNibble = 0;
			else if (iCurrentByte != 0) 
			{
				iCurrentByte--;
				iCurrentNibble = 1;
			}
		}
		else 
		{
			if (iCurrentByte != 0)
				iCurrentByte--; 
		}

		
		iScrollFlag = FALSE;
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
		{
			iCurrentLine = (iCurrentByte / iBytesPerLine);
			update_vertical_scrollbar ();
			iScrollFlag = TRUE;
		}
		
		if (iTextEntryMode == BYTES)
			iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
		else
			iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));

		if (iColumn_logical < iHorizontalScrollPos)
		{
			iHorizontalScrollPos = iColumn_logical;
			update_horizontal_scrollbar ();
			repaint ();
			break;
		}
		else if (iColumn_logical > (iHorizontalScrollPos + cxBuffer - 1))
		{
			iHorizontalScrollPos = iColumn_logical-(cxBuffer-1);
			update_horizontal_scrollbar ();
			repaint ();
			break;
		}
		else if (!iScrollFlag)
		{
			if (iCurrentByte%iBytesPerLine==iBytesPerLine-1) 
				repaint ((iCurrentByte / iBytesPerLine)+1);
			repaint ((iCurrentByte / iBytesPerLine));
		}
		else
			repaint ();
		break;
	}

	case VK_RIGHT:
	{
		if (iTextEntryMode == BYTES)
		{
			if (iCurrentNibble == 0)
				iCurrentNibble = 1;
			else if (iCurrentByte <= (FileBuffer.Upper ()))
			{
				iCurrentNibble = 0;
				iCurrentByte++;
			}
		}
		else
		{
			if (iCurrentByte <= (FileBuffer.Upper ()))
				iCurrentByte++;
		}
		
		iScrollFlag = FALSE;
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
		{
			iCurrentLine = max ((iCurrentByte / iBytesPerLine)-cBufferY+1, 0);
			update_vertical_scrollbar ();
			iScrollFlag = TRUE;
		}

		if (iTextEntryMode == BYTES)
			iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
		else
			iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));

		if (iColumn_logical >= iHorizontalScrollPos+cxBuffer)
		{
			iHorizontalScrollPos = iColumn_logical-(cxBuffer-1);
			update_horizontal_scrollbar ();
			repaint ();
			break;
		}
		else if (iColumn_logical < iHorizontalScrollPos)
		{
			iHorizontalScrollPos = iColumn_logical;
			update_horizontal_scrollbar ();
			repaint ();
			break;
		}
		else if (iScrollFlag != TRUE) 
		{
			if (iCurrentByte%iBytesPerLine==0)
				repaint ((iCurrentByte / iBytesPerLine)-1);
			repaint ((iCurrentByte / iBytesPerLine));
		}
		else
			repaint ();
		break;
	}

	case VK_PRIOR:
		if ((iCurrentByte / iBytesPerLine) >= cBufferY)
		{
			iCurrentByte -= cBufferY * iBytesPerLine; 
			iCurrentLine -= cBufferY;
			if (iCurrentLine < 0)
				iCurrentLine = (iCurrentByte / iBytesPerLine);
		}
		else
		{
			iCurrentLine = 0; 
			iCurrentByte = (iCurrentByte % iBytesPerLine);
		}
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
			iCurrentLine = iCurrentByte/iBytesPerLine;
		update_vertical_scrollbar ();
		repaint ();
		break;

	case VK_NEXT:
		iCurrentByte += cBufferY*iBytesPerLine;
		if (iCurrentByte > (FileBuffer.Upper ())+1)
		{
			iCurrentByte = ((FileBuffer.Upper ())+1)/iBytesPerLine*iBytesPerLine + (iCurrentByte % iBytesPerLine);
			if (iCurrentByte > (FileBuffer.Upper ())+1)
				iCurrentByte = (FileBuffer.Upper ())+1;
			iCurrentLine = (iCurrentByte / iBytesPerLine);
			adjust_view_for_cursor ();
		}
		else
		{
			iCurrentLine += cBufferY;
			if (iCurrentLine > (iNumlines-1))
				iCurrentLine = (iCurrentByte / iBytesPerLine);
		}
		if ((iCurrentLine > (iCurrentByte / iBytesPerLine)) || (iCurrentLine+cBufferY-1 < (iCurrentByte / iBytesPerLine)))
			iCurrentLine = max ((iCurrentByte / iBytesPerLine)-cBufferY+1, 0);
		update_vertical_scrollbar ();
		repaint ();
		break;
	}
	return 0;
}

int Translhextion::character (char ch)
{
	if (bSelected)
		return 0;

	if( bReadOnly && ch != '\t' )
	{
		MessageBox( hwnd, "File cannot be changed in read-only mode!", "Editing error:", MB_OK | MB_ICONERROR );
		return 0;
	}

	char x, c = tolower (ch);
	if (ch == '\t') 
	{
		if (iTextEntryMode == BYTES)
			iTextEntryMode = CHARS;
		else
			iTextEntryMode = BYTES;
		
		int iColumn_logical;
		if (iTextEntryMode == BYTES)
			iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
		else
			iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));

		if (iColumn_logical >= iHorizontalScrollPos+cxBuffer) 
			iHorizontalScrollPos = iColumn_logical-(cxBuffer-1); 
		else if (iColumn_logical < iHorizontalScrollPos) 
			iHorizontalScrollPos = iColumn_logical; 
		update_horizontal_scrollbar ();
		repaint ();
		return 0;
	}

	if( bReadOnly )
		return 1;
	
	if (iTextEntryMode==BYTES && !((c>='a'&&c<='f')||(c>='0'&&c<='9')))
		return 1;

	if (iCurrentByte == FileBuffer.Length())
	{
		if (FileBuffer.InsertExpandOn(iCurrentByte, 0, 1) == TRUE)
		{
			vChanges.push_back(COLORCHANGE);
			iCurrentNibble = 0;
			iInsertMode = FALSE;
			character (ch);
			update_size_change ();
			return 1;
		}
		else
		{
			MessageBox (hwnd, "Not enough memory to insert character.", "Insertion error:", MB_OK | MB_ICONERROR);
		}
		return 0;
	}

	if( iInsertMode )
	{
		if( iTextEntryMode == BYTES )
		{
			if( ( c >= 'a' && c <= 'f' ) || ( c >= '0' && c <= '9' ) )
			{
				if( bInsertingHex )
				{
					bInsertingHex = FALSE;
					if (c >= 'a' && c <= 'f')
						x = c - 0x61 + 0x0a;
					else
						x = c - 0x30;
					FileBuffer[iCurrentByte] = (FileBuffer[iCurrentByte] & 0xf0) | x;
					vChanges[iCurrentByte] = COLORCHANGE;
					iFileChanged = TRUE;
					bFilestatusChanged = TRUE;
					iCurrentByte++;
					iCurrentNibble = 0;
					update_size_change();
				}
				else
				{
					if( FileBuffer.InsertExpandOn( iCurrentByte, 0, 1 ) == TRUE )
					{
						vChanges.push_back(COLORCHANGE);
						bInsertingHex = TRUE;
						if (c >= 'a' && c <= 'f')
							x = c - 0x61 + 0x0a;
						else
							x = c - 0x30;
						FileBuffer[iCurrentByte] = (FileBuffer[iCurrentByte] & 0x0f) | (x << 4);
						vChanges[iCurrentByte] = COLORCHANGE;
						iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
						iCurrentNibble = 1;
						update_size_change();
					}
					else
					{
						MessageBox (hwnd, "Not enough memory to insert character.", "Insertion error:", MB_OK | MB_ICONERROR);
						return 0;
					}
				}
			}
			return 1;
		}
		else if (iTextEntryMode == CHARS)
		{
			if (FileBuffer.InsertExpandOn(iCurrentByte, 0, 1) == TRUE)
			{
				vChanges.push_back(COLORCHANGE);
				iCurrentNibble = 0;
				iInsertMode = FALSE;
				character (ch);
				iInsertMode = TRUE;
				iCurrentNibble = 0;
				update_size_change ();
			}
			else
			{
				MessageBox (hwnd, "Not enough memory to insert character.", "Insertion error:", MB_OK | MB_ICONERROR);
				return 0;
			}
		}
		return 1;
	}
	else
	{
		if ((iTextEntryMode == BYTES) && ((c >= 'a' && c <= 'f') || (c >= '0' && c <= '9')))
		{
			if (c >= 'a' && c <= 'f')
				x = c - 0x61 + 0x0a;
			else
				x = c - 0x30;
			if (iCurrentNibble == 0)
			{
				FileBuffer[iCurrentByte] = (FileBuffer[iCurrentByte] & 0x0f) | (x << 4);
				vChanges[iCurrentByte] = COLORCHANGE;
			}
			else
			{
				FileBuffer[iCurrentByte] = (FileBuffer[iCurrentByte] & 0xf0) | x;
				vChanges[iCurrentByte] = COLORCHANGE;
			}
			iFileChanged = TRUE;
			bFilestatusChanged = TRUE;
			keydown (VK_RIGHT);
		}
		else if (iTextEntryMode == CHARS)
		{
			switch (iCharacterSet)
			{
			case ANSI_FIXED_FONT:
				FileBuffer[iCurrentByte] = ch;
				vChanges[iCurrentByte] = COLORCHANGE;
				break;

			case OEM_FIXED_FONT:
				{
					char src[2], dst[2];
					src[0] = ch;
					src[1] = 0;
					CharToOem (src, dst);
					FileBuffer[iCurrentByte] = dst[0];
					vChanges[iCurrentByte] = COLORCHANGE;
				}
				break;
			}
			iFileChanged = TRUE;
			bFilestatusChanged = TRUE;
			keydown (VK_RIGHT);
		}
	}
	return 0;
}

int Translhextion::vertical_scroll (int cmd, int pos)
{
	if ((filename[0] == '\0') || FileBuffer.Length()==0)
		return 0;

	iVerticalScrollInc = 0;
	switch (cmd)
	{
	case SB_TOP:
		iCurrentLine = 0;
		break;
	case SB_BOTTOM:
		iCurrentLine = iNumlines-1;
		break;
	case SB_LINEUP:
		if (iCurrentLine > 0)
			iCurrentLine -= 1;
		break;
	case SB_LINEDOWN:
		if (iCurrentLine < iNumlines-1)
			iCurrentLine += 1;
		break;
	case SB_PAGEUP:
		if (iCurrentLine >= cBufferY)
			iCurrentLine -= cBufferY;
		else
			iCurrentLine = 0;
		break;
	case SB_PAGEDOWN:
		if (iCurrentLine <= iNumlines-1-cBufferY)
			iCurrentLine += cBufferY;
		else
			iCurrentLine = iNumlines-1;
		break;
	case SB_THUMBTRACK:
		iCurrentLine = (int) (pos * ((float)(iNumlines-1)/(float)iVerticalScrollMax));
		SetScrollPos (hwndVBar, SB_CTL, pos, TRUE);
		if (iCurrentLine > iNumlines-1)
			iCurrentLine = iNumlines-1;
		if( iNumlines - iCurrentLine < cBufferY )
		{
			iCurrentLine = ( ( FileBuffer.Upper() + 1 ) / iBytesPerLine ) - ( cBufferY - 1 );
			if( iCurrentLine < 0 )
				iCurrentLine = 0;
		}
		repaint();
		return 0;
	default:
		break;
	}
	iVerticalScrollPos = (int) ((float)iCurrentLine * ((float)iVerticalScrollMax)/(float)(iNumlines-1));
	SetScrollPos (hwndVBar, SB_CTL, iVerticalScrollPos, TRUE);
	if (iCurrentLine > iNumlines-1)
		iCurrentLine = iNumlines-1;
	repaint ();
	return 0;
}

int Translhextion::horizontal_scroll (int cmd, int pos)
{
	if ((filename[0] == '\0') || FileBuffer.Length()==0)
		return 0;

	iHorizontalScrollInc = 0;
	switch (cmd)
	{
	case SB_TOP:
		iHorizontalScrollInc = -iHorizontalScrollPos;
		break;
	case SB_BOTTOM:
		iHorizontalScrollInc = iHorizontalScrollMax - iHorizontalScrollPos;
		break;
	case SB_LINEUP:
		if (iHorizontalScrollPos > 0)
			iHorizontalScrollInc = -1;
		break;
	case SB_LINEDOWN:
		if (iHorizontalScrollPos < iHorizontalScrollMax)
			iHorizontalScrollInc = 1;
		break;
	case SB_PAGEUP:
		if (iHorizontalScrollPos >= cxBuffer)
			iHorizontalScrollInc = -cxBuffer;
		else
			iHorizontalScrollInc = -iHorizontalScrollPos;
		break;
	case SB_PAGEDOWN:
		if (iHorizontalScrollPos <= iHorizontalScrollMax-cxBuffer)
			iHorizontalScrollInc = cxBuffer;
		else
			iHorizontalScrollInc = iHorizontalScrollMax - iHorizontalScrollPos;
		break;
	case SB_THUMBTRACK:
		iHorizontalScrollInc = pos - iHorizontalScrollPos;
		break;
	default:
		break;
	}
	iHorizontalScrollPos += iHorizontalScrollInc;
	SetScrollPos (hwndHBar, SB_CTL, iHorizontalScrollPos, TRUE);
	InvalidateRect (hwnd, NULL, FALSE);
	UpdateWindow (hwnd);
	return 0;
}

int Translhextion::paint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	HideCaret (hwnd);
	int a, b;
	b = min (iCurrentLine + cBufferY, iNumlines-1);
	iBkColor = PALETTERGB (GetRValue(iBkColorValue),GetGValue(iBkColorValue),GetBValue(iBkColorValue));
	iTextColor = PALETTERGB (GetRValue(iTextColorValue),GetGValue(iTextColorValue),GetBValue(iTextColorValue));
	iTextColorE = PALETTERGB (GetRValue(iTextColorEValue),GetGValue(iTextColorEValue),GetBValue(iTextColorEValue));
	iTextColorO = PALETTERGB (GetRValue(iTextColorOValue),GetGValue(iTextColorOValue),GetBValue(iTextColorOValue));
	SetTextColor (hdc, iTextColor);
	SetBkColor (hdc, iBkColor);
	HPEN pen1 = CreatePen (PS_SOLID, 1, iBkColor);
	HPEN oldpen = (HPEN) SelectObject (hdc, pen1);
	HBRUSH brush1 = CreateSolidBrush (iBkColor);
	HBRUSH oldbrush = (HBRUSH) SelectObject (hdc, brush1);
	if ((b-iCurrentLine+1)*cCharYDim+BORDER_CLIENT_XDIM < cClientYDim-BORDER_CLIENT_XDIM)
		Rectangle (hdc, BORDER_CLIENT_XDIM, (b-iCurrentLine+1)*cCharYDim+BORDER_CLIENT_XDIM, cClientXDim-BORDER_CLIENT_XDIM,
			cClientYDim-BORDER_CLIENT_XDIM);
	if(!bThingyView)
	{
		Rectangle (hdc, ((iHorizontalScrollMax+1)-iHorizontalScrollPos)*cCharXDim+BORDER_CLIENT_XDIM, BORDER_CLIENT_XDIM,
			cClientXDim-BORDER_CLIENT_XDIM, cClientYDim-BORDER_CLIENT_XDIM);
	}
	else
	{
		//NOT GREAT BB
		Rectangle (hdc, ((iHorizontalScrollMax+1)-iHorizontalScrollPos)*cCharXDim+BORDER_CLIENT_XDIM, BORDER_CLIENT_XDIM,
			cClientXDim-BORDER_CLIENT_XDIM, cClientYDim-BORDER_CLIENT_XDIM);
	}
	SelectObject (hdc, oldpen);
	SelectObject (hdc, oldbrush);
	DeleteObject (pen1);
	DeleteObject (brush1);
	
	HFONT oldfont = (HFONT) SelectObject (hdc, hFont);
	HPEN sep_pen = CreatePen (PS_SOLID, 1, iSepColorValue);
	oldpen = (HPEN) SelectObject (hdc, sep_pen);

	if( Linebuffer.GetSize() < iCharsPerLine )
	{
		if( Linebuffer.SetLength( iCharsPerLine ) )
		{
			Linebuffer.ExpandToSize();
			HBRUSH hbr = CreateSolidBrush( iBookmarkColor );
			if (iUpdateLine == -1)
			{
				for (a = iCurrentLine; a <= b; a++)
					print_line( hdc, a, Linebuffer, hbr );
			}
			else
			{
				print_line( hdc, iUpdateLine, Linebuffer, hbr );
			}
			DeleteObject( hbr );
			SelectObject (hdc, oldpen);
			DeleteObject (sep_pen);
			SelectObject (hdc, oldfont);
			if(!bThingyView)
			{
				invert_char (hdc);
			}
			else
			{
				if (iUpdateLine == -1)
				{
					thingy_view_invert_char (hdc,a);
				}
				else
				{
					thingy_view_invert_char (hdc,iUpdateLine);
				}
				
			}
			draw_border_client_rect (hdc);
			ShowCaret (hwnd);
			EndPaint (hwnd, &ps);
			place_cursor ();
			update_window_status ();
			iUpdateLine = -1;
			return 0;
		}
		else
		{
			Rectangle( hdc, 0, 0, cClientXDim, cClientYDim );
			RECT r;
			r.top = BORDER_CLIENT_XDIM;
			r.left = BORDER_CLIENT_XDIM;
			r.right = cClientXDim;
			r.bottom = cClientYDim;
			DrawText( hdc, ("Error: could not allocate iLine buffer.\nPlease save your changes and restart " + sName + ".").c_str(), -1, &r, DT_LEFT );
		}
		
	}
	else
	{
			HBRUSH hbr = CreateSolidBrush( iBookmarkColor );
			if (iUpdateLine == -1)
			{
				for (a = iCurrentLine; a <= b; a++)
					print_line( hdc, a, Linebuffer, hbr );
			}
			else
			{
				print_line( hdc, iUpdateLine, Linebuffer, hbr );
			}
			DeleteObject( hbr );
			SelectObject (hdc, oldpen);
			DeleteObject (sep_pen);
			SelectObject (hdc, oldfont);
			if(!bThingyView)
			{
				invert_char (hdc);
			}
			else
			{
				if (iUpdateLine == -1)
				{
					thingy_view_invert_char (hdc,a);
				}
				else
				{
					thingy_view_invert_char (hdc,iUpdateLine);
				}
			}
			draw_border_client_rect (hdc);
			ShowCaret (hwnd);
			EndPaint (hwnd, &ps);
			place_cursor ();
			update_window_status ();
			iUpdateLine = -1;
			return 0;
	}
	return 0;
}

int Translhextion::destroy_window ()
{
	return 0;
}

void Translhextion::update_window_status()
{
	char cBuf[512];
	if (strlen (filename) != 0)
	{
		if (bFilestatusChanged)
		{
			sprintf (cBuf, "[%s", filename);
			if (iFileChanged == TRUE)
				strcat (cBuf, " *");
			strcat (cBuf, "]");
			if (bPartialOpen==TRUE)
				strcat (cBuf, " - P");
			strcat (cBuf, (" - " + sName).c_str());
			SetWindowText (hwnd, cBuf);
			bFilestatusChanged = FALSE;
		}
		if (bSelected == TRUE)
		{
			if (iEndOfSelection >= iStartOfSelection)
			{
				if(iOffsetMode == LOROM)
					sprintf (cBuf, "Selected: Offset %d=0x%X to %d=0x%X (%d byte(s))", iStartOfSelection, toLOROM(iStartOfSelection),
					iEndOfSelection, toLOROM(iEndOfSelection), iEndOfSelection-iStartOfSelection+1);
				else if(iOffsetMode == HIROM)
					sprintf (cBuf, "Selected: Offset %d=0x%X to %d=0x%X (%d byte(s))", iStartOfSelection, toHIROM(iStartOfSelection),
					iEndOfSelection, toHIROM(iEndOfSelection), iEndOfSelection-iStartOfSelection+1);
				else
					sprintf (cBuf, "Selected: Offset %d=0x%X to %d=0x%X (%d byte(s))", iStartOfSelection, iStartOfSelection,
					iEndOfSelection, iEndOfSelection, iEndOfSelection-iStartOfSelection+1);

			}
			else
			{
				if(iOffsetMode == LOROM)
					sprintf (cBuf, "Selected: Offset %d=0x%X to %d=0x%X (%d byte(s))", iEndOfSelection, toLOROM(iEndOfSelection),
					iStartOfSelection, toLOROM(iStartOfSelection), iStartOfSelection-iEndOfSelection+1);
				else if(iOffsetMode == HIROM)
					sprintf (cBuf, "Selected: Offset %d=0x%X to %d=0x%X (%d byte(s))", iEndOfSelection, toHIROM(iEndOfSelection),
					iStartOfSelection, toHIROM(iStartOfSelection), iStartOfSelection-iEndOfSelection+1);
				else
					sprintf (cBuf, "Selected: Offset %d=0x%X to %d=0x%X (%d byte(s))", iEndOfSelection, iEndOfSelection,
					iStartOfSelection, iStartOfSelection, iStartOfSelection-iEndOfSelection+1);
			}
			SendMessage (hwndStatusBar, SB_SETTEXT, 0, (LPARAM) cBuf);
		}
		else 
		{
			if(iOffsetMode == LOROM)	
				sprintf (cBuf, "Offset %d=0x%X", iCurrentByte, toLOROM(iCurrentByte));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "Offset %d=0x%X", iCurrentByte, toHIROM(iCurrentByte));
			else
				sprintf (cBuf, "Offset %d=0x%X", iCurrentByte, iCurrentByte);	
			int wordval, longval;
			char buf2[80];
			if (FileBuffer.Length()-iCurrentByte > 0)
			{
				strcat (cBuf, "   Bits=");
				unsigned char zzz=FileBuffer[iCurrentByte];
				for(int i=0;i<8;i++)buf2[i]=((zzz>>i)&0x1?'1':'0');
				for(i=0;i<4;i++)sswap(buf2[i],buf2[7-i]);
				buf2[8]='\0';
				strcat (cBuf, buf2);
				if(bTableLoaded && bTableActive)
				{
					bool oldbmark = bByteMark;
					int wherebyte = 0;
					bByteMark = false;
					vector<string> temp;
					temp = vEmpty;
					if(iCurrentByte-2 >= 0)
						temp.push_back(hex_char(FileBuffer[iCurrentByte-2]));
					if(iCurrentByte-1 >= 0)
						temp.push_back(hex_char(FileBuffer[iCurrentByte-1]));
					temp.push_back(hex_char(FileBuffer[iCurrentByte]));
					wherebyte = temp.size()-1;
					if(iCurrentByte + 1 < FileBuffer.Length())
						temp.push_back(hex_char(FileBuffer[iCurrentByte+1]));
					if(iCurrentByte + 2 < FileBuffer.Length())
						temp.push_back(hex_char(FileBuffer[iCurrentByte+2]));
					bSplitMultiByte = false;
					translate(temp);
					bSplitMultiByte = true;
					if(temp[wherebyte] == "")
					{
						if(wherebyte + 1 < temp.size() && temp[wherebyte+1] == "")
						{
							if(wherebyte - 1 >= 0)
							{
								strcat(cBuf,"   Table=");
								strcat(cBuf,temp[wherebyte-1].c_str());
							}
							else
							{
								strcat(cBuf,"   Table=");
								strcat(cBuf,"ERROR!");
							}
						}
						else if(wherebyte - 1 >= 0 && temp[wherebyte-1] == "")
						{
							if(wherebyte - 2 >= 0)
							{
								strcat(cBuf,"   Table=");
								strcat(cBuf,temp[wherebyte-2].c_str());
							}
							else
							{
								strcat(cBuf,"   Table=");
								strcat(cBuf,"ERROR!");
							}
						}
						else if(wherebyte - 1 >= 0)
						{
							strcat(cBuf,"   Table=");
							strcat(cBuf,temp[wherebyte-1].c_str());
						}
						else
						{
							strcat(cBuf,"   Table=");
							strcat(cBuf,"ERROR!");
						}
					}
					else
					{
						strcat(cBuf,"   Table=");
						strcat(cBuf,temp[wherebyte].c_str());
					}
					bByteMark = oldbmark;
				}
			}
			strcat (cBuf, "\t");
			if (bUnsignedView) 
			{
				
				if (iBinaryMode == LITTLE_ENDIAN)
				{
					if (FileBuffer.Length ()-iCurrentByte >= 1)
					{
						sprintf (buf2, "\tUnsigned: B:%u", (unsigned int) FileBuffer[iCurrentByte]);
						strcat (cBuf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (cBuf, buf2);
					}
					if (FileBuffer.Length ()-iCurrentByte >= 2)
					{
						wordval = (FileBuffer[iCurrentByte+1] << 8) | FileBuffer[iCurrentByte];
						sprintf (buf2, ",W:%u", (unsigned int) wordval);
						strcat (cBuf, buf2);
					}
					if (FileBuffer.Length ()-iCurrentByte >= 4)
					{
						longval = wordval | (((FileBuffer[iCurrentByte + 3] << 8) | FileBuffer[iCurrentByte + 2]) << 16);
						sprintf (buf2, ",L:%u", (unsigned int) longval);
						strcat (cBuf, buf2);
					}
				}
				else
				{
					if (FileBuffer.Length ()-iCurrentByte >= 1)
					{
						sprintf (buf2, "\tUnsigned: B:%u", (unsigned int) FileBuffer[iCurrentByte]);
						strcat (cBuf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (cBuf, buf2);
					}
					if (FileBuffer.Length ()-iCurrentByte >= 2)
					{
						wordval = (FileBuffer[iCurrentByte] << 8) | FileBuffer[iCurrentByte+1];
						sprintf (buf2, ",W:%u", (unsigned int) wordval);
						strcat (cBuf, buf2);
					}
					if (FileBuffer.Length ()-iCurrentByte >= 4)
					{
						longval = (wordval<<16) | (FileBuffer[iCurrentByte+2]<<8) | (FileBuffer[iCurrentByte+3]);
						sprintf (buf2, ",L:%u", (unsigned int) longval);
						strcat (cBuf, buf2);
					}
				}
			}
			else 
			{
				if (iBinaryMode == LITTLE_ENDIAN)
				{
					if (FileBuffer.Length ()-iCurrentByte >= 1)
					{
						sprintf (buf2, "\tSigned: B:%d", (int) (signed char) FileBuffer[iCurrentByte]);
						strcat (cBuf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (cBuf, buf2);
					}
					
					if (FileBuffer.Length ()-iCurrentByte >= 2)
					{
						
						wordval = (FileBuffer[iCurrentByte + 1] << 8) | FileBuffer[iCurrentByte];
						sprintf (buf2, ",W:%d", (int) (signed short) wordval);
						strcat (cBuf, buf2);
					}
					if (FileBuffer.Length ()-iCurrentByte >= 4)
					{
						longval = wordval | (((FileBuffer[iCurrentByte + 3] << 8) | FileBuffer[iCurrentByte + 2]) << 16);
						sprintf (buf2, ",L:%d", (signed int) longval);
						strcat (cBuf, buf2);
					}
				}
				else
				{
					if (FileBuffer.Length ()-iCurrentByte >= 1)
					{
						sprintf (buf2, "\tSigned: B:%d", (signed char) FileBuffer[iCurrentByte]);
						strcat (cBuf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (cBuf, buf2);
					}
					
					if (FileBuffer.Length ()-iCurrentByte >= 2)
					{
						
						wordval = (FileBuffer[iCurrentByte] << 8) | FileBuffer[iCurrentByte+1];
						sprintf (buf2, ",W:%d", (int) (signed short) wordval);
						strcat (cBuf, buf2);
					}
					if (FileBuffer.Length ()-iCurrentByte >= 4)
					{
						
						longval = (wordval<<16) | (FileBuffer[iCurrentByte+2]<<8) | (FileBuffer[iCurrentByte+3]);
						sprintf (buf2, ",L:%d", (signed int) longval);
						strcat (cBuf, buf2);
					}
				}
			}
			SendMessage (hwndStatusBar, SB_SETTEXT, 0, (LPARAM) cBuf);

			
			switch (iCharacterSet)
			{
			case ANSI_FIXED_FONT:
				sprintf (cBuf, "\tANSI");
				break;

			case OEM_FIXED_FONT:
				sprintf (cBuf, "\tOEM");
				break;
			}

			if( bReadOnly )
			{
				sprintf (buf2, " / READ");
				strcat (cBuf, buf2);
			}
			else if( iInsertMode )
			{
				sprintf (buf2, " / INS");
				strcat (cBuf, buf2);
			}
			else
			{
				sprintf (buf2, " / OVR");
				strcat (cBuf, buf2);
			}
			if (iBinaryMode == LITTLE_ENDIAN)
			{
				sprintf (buf2, " / L"); 
				strcat (cBuf, buf2);
			}
			else if (iBinaryMode == BIG_ENDIAN)
			{
				sprintf (buf2, " / B"); 
				strcat (cBuf, buf2);
			}
			if(iOffsetMode == LOROM)
			{
				sprintf (buf2, " / LOROM"); 
				strcat (cBuf, buf2);
			}
			else if(iOffsetMode == HIROM)
			{
				sprintf (buf2, " / HIROM"); 
				strcat (cBuf, buf2);
			}
			SendMessage (hwndStatusBar, SB_SETTEXT, 1, (LPARAM) cBuf);
			
			
			sprintf (cBuf, "\tSize: %u", FileBuffer.Length ());
			SendMessage (hwndStatusBar, SB_SETTEXT, 2, (LPARAM) cBuf);
		}
	}
	else
	{
		SetWindowText (hwnd, sName.c_str());
		SendMessage (hwndStatusBar, WM_SETTEXT, 0, (LPARAM) "No file loaded");
	}
}

void Translhextion::place_cursor ()
{
	if (bSelected)
	{
		SetCaretPos (-cCharXDim, -cCharYDim);
		return;
	}
	if(bHideHex)
		return;
	int iCaretLine = iCurrentByte / iBytesPerLine,
		iBottomLine = iCurrentLine + cBufferY - 1;

	switch (iTextEntryMode)	
	{
	case CHARS:
		if (iCaretLine >= iCurrentLine && iCaretLine <= iBottomLine && filename[0] != '\0')
		{
			int y = iCaretLine - iCurrentLine;
			int x = 0;
			//if(!bThingyView)
			//{
				x = iOffsetLength+iByteSpace+iBytesPerLine*3+iCharSpace - iHorizontalScrollPos + (iCurrentByte%iBytesPerLine);
			//}
			//else
			//{
			//	x = iOffsetLength+iByteSpace+iBytesPerLine*2 + iMaxCharLine +iCharSpace - iHorizontalScrollPos + (iCurrentByte%iBytesPerLine);
			//}
			SetCaretPos (BORDER_CLIENT_XDIM + x*cCharXDim, BORDER_CLIENT_XDIM + y*cCharYDim);
		}
		else
			SetCaretPos (-cCharXDim, -cCharYDim);
		break;
	case BYTES:
		
		if (iCaretLine >= iCurrentLine && iCaretLine <= iBottomLine && filename[0] != '\0')
		{
			int y = iCaretLine - iCurrentLine;
			int	x = iOffsetLength+iByteSpace + (iCurrentByte%iBytesPerLine)*3 - iHorizontalScrollPos + iCurrentNibble;
			SetCaretPos (BORDER_CLIENT_XDIM + x*cCharXDim, BORDER_CLIENT_XDIM + y*cCharYDim);
		}
		else
			SetCaretPos (-cCharXDim, -cCharYDim);
	}
}

int Translhextion::repaint( int iLine )
{
	HideCaret( hwnd );
	iUpdateLine = iLine;
	InvalidateRect( hwnd, NULL, FALSE );
	UpdateWindow( hwnd );
	ShowCaret( hwnd );
	return 0;
}

void Translhextion::clear_all ()
{
	iOffsetLength = 8;
	iByteSpace = 2;
	iBytesPerLine = 16;
	iCharSpace = 1;
	//if(!bThingyView)
	//{
		iCharsPerLine = iOffsetLength + iByteSpace + iBytesPerLine*3 + iCharSpace + iBytesPerLine;
	//}
	//else
	//{
	//	iCharsPerLine = iOffsetLength + iByteSpace + iBytesPerLine*2 + iMaxCharLine + iCharSpace + iBytesPerLine;
	//}
	FileBuffer.Reset ();
	vChanges = vEmpty;
	vHighlights = vEmpty;
	vDTEs = vEmpty;
	filename[0] = '\0';
	iVerticalScrollMax = 0;
	iVerticalScrollPos = 0;
	iVerticalScrollInc = 0;
	iHorizontalScrollMax = 0;
	iHorizontalScrollPos = 0;
	iHorizontalScrollInc = 0;
	iCurrentLine = 0;
	iCurrentByte = 0;
	iCurrentNibble = 0;
}

void Translhextion::update_vertical_scrollbar ()
{
	iVerticalScrollPos = (int) ((float)iCurrentLine * ((float)iVerticalScrollMax)/(float)(iNumlines-1));
	SetScrollPos (hwndVBar, SB_CTL, iVerticalScrollPos, TRUE);
}

void Translhextion::update_horizontal_scrollbar ()
{
	SetScrollPos (hwndHBar, SB_CTL, iHorizontalScrollPos, TRUE);
}

void Translhextion::invert_char (HDC hdc)
{
	if( bDontMarkCurrentPos )
		return;
	if(bHideHex)
		return;

	if (bSelected)
	{
		SetCaretPos (-cCharXDim, -cCharYDim);
		return;
	}

	int retrieved_device_context = FALSE;
	if (hdc == 0)
	{
		hdc = GetDC (hwnd);
		retrieved_device_context = TRUE;		
	}

	int iCharCoordinate;
	RECT r;
	switch (iTextEntryMode)
	{
	case CHARS:
		iCharCoordinate = iOffsetLength + iByteSpace + (iCurrentByte%iBytesPerLine)*3 - iHorizontalScrollPos;
		r.left = BORDER_CLIENT_XDIM + iCharCoordinate * cCharXDim;
		r.top = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine)*cCharYDim;
		r.right = BORDER_CLIENT_XDIM + r.left + 2*cCharXDim;
		r.bottom = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine+1)*cCharYDim;
		InvertRect (hdc, &r);
		break;
	case BYTES:
		if(!bThingyView)
		{
			iCharCoordinate = iOffsetLength + iByteSpace + iBytesPerLine*3 + iCharSpace
				+ (iCurrentByte % iBytesPerLine) - iHorizontalScrollPos;
			r.left = BORDER_CLIENT_XDIM + iCharCoordinate * cCharXDim;
			r.top = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine)*cCharYDim;
			r.right = BORDER_CLIENT_XDIM + (iCharCoordinate+1)*cCharXDim;
			r.bottom = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine+1)*cCharYDim;
			InvertRect (hdc, &r);
		}
		else
		{//comeback
		//	iCharCoordinate = iOffsetLength + iByteSpace + iBytesPerLine*3 + iCharSpace
		//		+ (iCurrentByte % iBytesPerLine) - iHorizontalScrollPos;
		//	r.left = BORDER_CLIENT_XDIM + iCharCoordinate * cCharXDim;
		//	r.top = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine)*cCharYDim;
		//	r.right = BORDER_CLIENT_XDIM + (iCharCoordinate+1)*cCharXDim;
		//	r.bottom = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine+1)*cCharYDim;
		//	InvertRect (hdc, &r);
		}
		break;
		
	}

	if (retrieved_device_context)
		ReleaseDC (hwnd, hdc);
}

void Translhextion::thingy_view_invert_char (HDC hdc, int iLine)
{
	if( bDontMarkCurrentPos )
		return;
	if(bHideHex)
		return;
	if (bSelected)
	{
		SetCaretPos (-cCharXDim, -cCharYDim);
		return;
	}
	string output;
	if (iLine < iCurrentLine || iLine > iCurrentLine + cBufferY)
		return;
	int iStartPos = iLine * iBytesPerLine, iEndPos, i = 0, m;
	if( iStartPos > FileBuffer.Upper() + 1 )
	{
		return;
	}
	
	if (iStartPos+iBytesPerLine > FileBuffer.Length ())
	{
		iEndPos = FileBuffer.Upper()+1;
	}
	else
	{
		iEndPos = iStartPos+iBytesPerLine-1;
	}
	if( iEndPos < iStartPos )
	{
		iEndPos = iStartPos;
	}
	m = iOffsetLength+iByteSpace; 
	vPLineBuf = vEmpty;
	string highlight;
	for(i = iStartPos;(i + 2 <FileBuffer.Length() && i<=iCurrentByte + 2);i++)
	{
		vPLineBuf.push_back(hex_char(FileBuffer[i]));
	}
	if(vPLineBuf.size() >= 3)
	{
		string output;
		output = "";
		int j = 0;
		bool oldbytemark = bByteMark;
		bByteMark = false;
		bSplitMultiByte = false;
		translate(vPLineBuf);
		bByteMark = oldbytemark;
		bSplitMultiByte = true;
		assert(vPLineBuf.size() > 0);
		vPLineBuf.pop_back();
		assert(vPLineBuf.size() > 0);
		vPLineBuf.pop_back();
		if(vPLineBuf.size() >= 3 && vPLineBuf[vPLineBuf.size() - 1] == "")
		{
			if(vPLineBuf[vPLineBuf.size()-2] == "")
			{
				highlight = vPLineBuf[vPLineBuf.size()-3];
				assert(vPLineBuf.size() > 0);
				vPLineBuf.pop_back();
				assert(vPLineBuf.size() > 0);
				vPLineBuf.pop_back();
				assert(vPLineBuf.size() > 0);
				vPLineBuf.pop_back();
				j++;
				j++;
			}
			else
			{
				highlight = vPLineBuf[vPLineBuf.size()-2];
				assert(vPLineBuf.size() > 0);
				vPLineBuf.pop_back();
				assert(vPLineBuf.size() > 0);
				vPLineBuf.pop_back();
				j++;
			}
		}
		else if(vPLineBuf.size() >= 2 && vPLineBuf[vPLineBuf.size()-1] == "")
		{
			highlight = vPLineBuf[vPLineBuf.size()-2];
			assert(vPLineBuf.size() > 0);
			vPLineBuf.pop_back();
			assert(vPLineBuf.size() > 0);
			vPLineBuf.pop_back();
			j++;
		}
		else
		{
			highlight = vPLineBuf[vPLineBuf.size()-1];
			assert(vPLineBuf.size() > 0);
			vPLineBuf.pop_back();
		}
		if(highlight == "\n")
		{
			highlight = "*";
		}
		else if(highlight.substr(1,3) == "END")
		{
			highlight = "\\";
		}
		else if(highlight.substr(0,1) == sByteOpen && highlight.length() == 4)
		{
			highlight = "#";
		}
		else if(highlight.substr(0,1) == sByteOpen)
		{
			highlight = highlight.substr(1,highlight.length()-2);
		}
		int i = 0;
		for(i = 0;i<vPLineBuf.size();i++)
		{
			if(vPLineBuf[i] == "\n")
			{
				output += "*";
			}
			else if(vPLineBuf[i].length() >= 4 && vPLineBuf[i].substr(1,3) == "END")
			{
				output += "\\";
			}
			else if(vPLineBuf[i].substr(0,1) == sByteOpen && vPLineBuf[i].length() == 4)
			{
				output += "#";
			}
			else
			{
				if(vPLineBuf[i].substr(0,1) == sByteOpen)
				{
					output += vPLineBuf[i].substr(1,vPLineBuf[i].length()-2);
				}
				else
				{	
					if(bJapanese && vPLineBuf[i].length() == 2)
					{
						string hex = hex_char(vPLineBuf[i][0]) + hex_char(vPLineBuf[i][1]);
						long code = dualbyteindex(hex);
						int ascii = -1;
						if(bJPShift)
						{
							ascii = jpfont_shift(code);
						}
						else
						{
							ascii = jpfont_euc(code);
						}
						if(ascii != -1)
						{
							vPLineBuf[i] = "";
							vPLineBuf[i] += (unsigned char) ascii;
							vPLineBuf[i] += (unsigned char) ' ';
							output += vPLineBuf[i];
						}
						else
						{
							output += vPLineBuf[i]; 
						}
						
					}
					else if(bJapanese && vPLineBuf[i].length() == 4)
					{
						string hex = hex_char(vPLineBuf[i][0]) + hex_char(vPLineBuf[i][1]);
						long code1 = dualbyteindex(hex);
						hex = hex_char(vPLineBuf[i][2]) + hex_char(vPLineBuf[i][3]);
						long code2 = dualbyteindex(hex);
						int ascii = -1;
						int ascii2 = -1;
						if(bJPShift)
						{
							ascii = jpfont_shift(code1);
							ascii2 = jpfont_shift(code2);
						}
						else
						{
							ascii = jpfont_euc(code1);
							ascii2 = jpfont_euc(code2);
						}
						if(ascii != -1)
						{
							vPLineBuf[i] = "";
							vPLineBuf[i] += (unsigned char) ascii;
							vPLineBuf[i] += (unsigned char) ' ';
							if(ascii2 != -1)
							{
								vPLineBuf[i] += (unsigned char) ascii2;
								vPLineBuf[i] += (unsigned char) ' ';
							}
							output += vPLineBuf[i];
						}
						else
						{
							output += vPLineBuf[i]; 
						}
						
					}
					else
					{
						output += vPLineBuf[i]; 
					}
				}
			}
		}
		i = i + j;
		int retrieved_device_context = FALSE;
		if (hdc == 0)
		{
			hdc = GetDC (hwnd);
			retrieved_device_context = TRUE;		
		}
	
		int iCharCoordinate;
		RECT r;
		switch (iTextEntryMode)
		{
		case CHARS:
			iCharCoordinate = iOffsetLength + iByteSpace + (iCurrentByte%iBytesPerLine)*3 - iHorizontalScrollPos;
			r.left = BORDER_CLIENT_XDIM + iCharCoordinate * cCharXDim;
			r.top = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine)*cCharYDim;
			r.right = BORDER_CLIENT_XDIM + r.left + 2*cCharXDim;
			r.bottom = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine+1)*cCharYDim;
			InvertRect (hdc, &r);
			break;
		case BYTES:
			iCharCoordinate = iOffsetLength + iByteSpace + iBytesPerLine*3 + iCharSpace
				+ (iCurrentByte % iBytesPerLine) + (output.length()-i) - iHorizontalScrollPos;
			r.left = BORDER_CLIENT_XDIM + iCharCoordinate * cCharXDim;
			r.top = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine)*cCharYDim;
			r.right = BORDER_CLIENT_XDIM + (iCharCoordinate+highlight.length())*cCharXDim;
			r.bottom = BORDER_CLIENT_XDIM + (iCurrentByte/iBytesPerLine-iCurrentLine+1)*cCharYDim;
			InvertRect (hdc, &r);
			break;
		}
	
		if (retrieved_device_context)
			ReleaseDC (hwnd, hdc);
	}
}

void Translhextion::print_line (HDC hdc, int iLine, char* ucLineBuf, HBRUSH hbr )
{
	string output;
	int totallength = 0;
	vector<int> vAsciiLengths;
	if (iLine < iCurrentLine || iLine > iCurrentLine + cBufferY)
		return;
	int iStartPos = iLine * iBytesPerLine, iEndPos, i = 0, m;
	char cBuf[80], c;

	if( iStartPos > FileBuffer.Upper() + 1 )
	{
		return;
	}

	sprintf (cBuf, "%%%d.%dX", iOffsetLength, iOffsetLength);

	for (m=0; m<iByteSpace; m++)
		cBuf[5+m] = ' ';
	cBuf[5+m] = '\0';
	if(iOffsetMode == LOROM)
		sprintf (ucLineBuf, cBuf, toLOROM(iStartPos));
	else if(iOffsetMode == HIROM)
		sprintf (ucLineBuf, cBuf, toHIROM(iStartPos));
	else
		sprintf (ucLineBuf, cBuf, iStartPos);
	if (iStartPos+iBytesPerLine > FileBuffer.Length ())
	{
		iEndPos = FileBuffer.Upper()+1;
	}
	else
	{
		iEndPos = iStartPos+iBytesPerLine-1;
	}
	if( iEndPos < iStartPos )
	{
		iEndPos = iStartPos;
	}
	m = iOffsetLength+iByteSpace; 
	string dtile;
	for (i=iStartPos; i<=iEndPos; i++)
	{
		if (i == FileBuffer.Length())
		{
			ucLineBuf[m++] = '_';
			ucLineBuf[m++] = '_';
			ucLineBuf[m++] = ' ';
		}
		else
		{
			char temp;
			c = (FileBuffer[i] >> 4);
			temp = (char) c;
			_strupr(&temp);
			c = (int) temp;
			if( c < 10 )
				c += '0';
			else
				c = c - 10 + 'A';
			ucLineBuf[m++] = c;
			c = (FileBuffer[i] & 0x0f);
			if( c < 10 )
				c += '0';
			else
				c = c - 10 + 'A';
			ucLineBuf[m++] = c;
			ucLineBuf[m++] = ' ';
			//for(vu = 0;vu<vHighlights.size();vu++)
			//{  
			if(vTableOut[(unsigned char) FileBuffer[i]].length() == 2)
			{
				//if(vChanges[i] != COLORCHANGE)
				//	vChanges[i] = COLORDTE;
				vDTEs[i] = COLORDTE;
			}
			//if(vChanges[i] == COLORHIGHLIGHT)
			//{
			//	vChanges[i] = COLORNORMAL;
			//}
			if(bHighlight)
			{
				dtile = "";
				if(i + 1 < vChanges.size())
				{
					dtile += (unsigned char) FileBuffer[i];
					dtile += (unsigned char) FileBuffer[i+1];
					if(vTwoByteHighlightTable[dualtileindex(dtile)] == "1")
					{
						//if(vChanges[i] != COLORCHANGE)
						//	vChanges[i] = COLORHIGHLIGHT;
						//if(vChanges[i + 1] != COLORCHANGE)
						//	vChanges[i + 1] = COLORHIGHLIGHT;
						vHighlights[i] = COLORHIGHLIGHT;
						vHighlights[i + 1] = COLORHIGHLIGHT;
					}
					else if(vTwoByteHighlightTable[(unsigned char) FileBuffer[i]] == "1")
					{
						vHighlights[i] = COLORHIGHLIGHT;
					}
				}
				else
				{
					if(vTwoByteHighlightTable[(unsigned char) FileBuffer[i]] == "1")
					{
						//if(vChanges[i] != COLORCHANGE)
						//	vChanges[i] = COLORHIGHLIGHT;
						vHighlights[i] = COLORHIGHLIGHT;
					}
				}
			}
			//}
		}
	}
	if (iEndPos-iStartPos < iBytesPerLine-1)
	{
		for (i=0; i<iBytesPerLine-1-(iEndPos-iStartPos); i++)
		{
			ucLineBuf[m++] = ' ';
			ucLineBuf[m++] = ' ';
			ucLineBuf[m++] = ' ';
		}
	}

	for (i=0; i<iCharSpace; i++)
		ucLineBuf[m++] = ' ';
	
	unsigned char * temp;
	int k = 0;
	if(!bThingyView)
	{
		for (i=iStartPos; i<=iEndPos; i++)
		{
			temp = &(FileBuffer[i]);
			if (i == FileBuffer.Length())
			{
				ucLineBuf[m++] = ' ';
			}
			else if (iCharacterSet == OEM_FIXED_FONT && FileBuffer[i]!=0)
			{
				ucLineBuf[m++] = *temp;
			}
			else if ((FileBuffer[i]>=32 && FileBuffer[i]<=126) || (FileBuffer[i]>=160 && FileBuffer[i]<=255) || (FileBuffer[i]>=145 && FileBuffer[i]<=146))
			{
				ucLineBuf[m++] = *temp;
			}
			else
			{
				ucLineBuf[m++] = '.';
			}
		}

	}
	else
	{
		vector<string> dsize(iEndPos - iStartPos + 1);
		vPLineBuf = dsize;
		dsize = vEmpty;
		for (i=iStartPos; i<=iEndPos; i++)
		{
			if (i == FileBuffer.Length())
			{
				;
			}
			else
			{
				vPLineBuf[i - iStartPos] = hex_char(FileBuffer[i]);
			}
		}
		bool oldbytemark = bByteMark;
		bByteMark = false;
		bSplitMultiByte = false;
		translate(vPLineBuf);
		bSplitMultiByte = true;
		bByteMark = oldbytemark;
		if(i == FileBuffer.Length())
		{
			vPLineBuf.push_back(" ");
		}
		output = "";
		vector<int> dsize2(vPLineBuf.size());
		vAsciiLengths = dsize2;
		dsize2 = viEmpty;
		for(i = 0;i<vPLineBuf.size();i++)
		{
			if(vPLineBuf[i] == "\n")
			{
				output += "*";
				vAsciiLengths[i] = 1;
			}
			else if(vPLineBuf[i].length() >= 4 && vPLineBuf[i].substr(1,3) == "END")
			{
				output += "\\";
				vAsciiLengths[i] = 1;
			}
			else if(vPLineBuf[i].substr(0,1) == sByteOpen && vPLineBuf[i].length() == 4) //what about double bytes that translate to {xx}? triples?
			{
				output += "#";
				vAsciiLengths[i] = 1;
			}
			else
			{
				if(vPLineBuf[i].substr(0,1) == sByteOpen)
				{
					output += vPLineBuf[i].substr(1,vPLineBuf[i].length()-2); 
					vAsciiLengths[i] = vPLineBuf[i].length()-2;
				}
				else
				{
					if(bJapanese && vPLineBuf[i].length() == 2)
					{
						string hex = hex_char(vPLineBuf[i][0]) + hex_char(vPLineBuf[i][1]);
						long code = dualbyteindex(hex);
						int ascii = -1;
						if(bJPShift)
						{
							ascii = jpfont_shift(code);
						}
						else
						{
							ascii = jpfont_euc(code);
						}
						if(ascii != -1)
						{
							vPLineBuf[i] = "";
							vPLineBuf[i] += (unsigned char) ascii;
							vPLineBuf[i] += (unsigned char) ' ';
							output += vPLineBuf[i];
						}
						else
						{
							output += vPLineBuf[i]; 
						}
						
					}
					else if(bJapanese && vPLineBuf[i].length() == 4)
					{
						string hex = hex_char(vPLineBuf[i][0]) + hex_char(vPLineBuf[i][1]);
						long code1 = dualbyteindex(hex);
						hex = hex_char(vPLineBuf[i][2]) + hex_char(vPLineBuf[i][3]);
						long code2 = dualbyteindex(hex);
						int ascii = -1;
						int ascii2 = -1;
						if(bJPShift)
						{
							ascii = jpfont_shift(code1);
							ascii2 = jpfont_shift(code2);
						}
						else
						{
							ascii = jpfont_euc(code1);
							ascii2 = jpfont_euc(code2);
						}
						if(ascii != -1)
						{
							vPLineBuf[i] = "";
							vPLineBuf[i] += (unsigned char) ascii;
							vPLineBuf[i] += (unsigned char) ' ';
							if(ascii2 != -1)
							{
								vPLineBuf[i] += (unsigned char) ascii2;
								vPLineBuf[i] += (unsigned char) ' ';
							}
							output += vPLineBuf[i];
						}
						else
						{
							output += vPLineBuf[i]; 
						}
						
					}
					else
					{
						output += vPLineBuf[i]; 
					}
					vAsciiLengths[i] = vPLineBuf[i].length();
				}
			}
		}
		if(output.length() < iMaxCharLine)
		{
			i = output.length();
			if(bHideHex)
				i = i - 15;
			for(i = i; i < iMaxCharLine;i++)
				output += " ";
		}
		else
		{//NOT GREAT BB
			iLastMaxCharLine = iMaxCharLine;
			iMaxCharLine = 2 * output.length();
		}
	}
	if (iEndPos-iStartPos < iBytesPerLine-1)
		for (i=0; i<iBytesPerLine-1-(iEndPos-iStartPos); i++)
			ucLineBuf[m++] = ' ';
	
	
	int iLineLen = m; 

	iBkColor = PALETTERGB (GetRValue(iBkColorValue),GetGValue(iBkColorValue),GetBValue(iBkColorValue));
	iTextColor = PALETTERGB (GetRValue(iTextColorValue),GetGValue(iTextColorValue),GetBValue(iTextColorValue));
	iTextColorE = PALETTERGB (GetRValue(iTextColorEValue),GetGValue(iTextColorEValue),GetBValue(iTextColorEValue));
	iTextColorO = PALETTERGB (GetRValue(iTextColorOValue),GetGValue(iTextColorOValue),GetBValue(iTextColorOValue));
	SetTextColor (hdc, iTextColor);
	SetBkColor (hdc, iBkColor);

	if( iHorizontalScrollPos < iOffsetLength + iByteSpace )
	{
		TextOut( hdc, BORDER_CLIENT_XDIM, BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim, ucLineBuf + iHorizontalScrollPos, iOffsetLength + iByteSpace - iHorizontalScrollPos );
	}

	int iHexStart = iOffsetLength + iByteSpace;
	int iHexXStart = iHexStart * cCharXDim;

	if( iHorizontalScrollPos < iOffsetLength + iByteSpace + iBytesPerLine * iHexWidth )
	{
		TextOut( hdc,
			BORDER_CLIENT_XDIM + ( iHexStart + iBytesPerLine * iHexWidth - iHorizontalScrollPos ) * cCharXDim,
			BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
			ucLineBuf + iHexStart + iBytesPerLine * iHexWidth,
			iCharSpace );
	}

	iSelBkColor = PALETTERGB (GetRValue(iSelBkColorValue),GetGValue(iSelBkColorValue),GetBValue(iSelBkColorValue));
	iSelTextColor = PALETTERGB (GetRValue(iSelTextColorValue),GetGValue(iSelTextColorValue),GetBValue(iSelTextColorValue));
	BOOL bLastTextNorm = TRUE;
	COLORREF crefOldColor = GetTextColor(hdc);
	int p = 0, el = iStartPos + iBytesPerLine - 1 - 1, s, e;
	for( p = iStartPos; p <= el; p++ )
	{
		s = iOffsetLength + iByteSpace + ( p - iStartPos + 1 ) * iHexWidth;
		e = s - iHexWidth;
		if( iHorizontalScrollPos < s && iHorizontalScrollPos + cxBuffer >= e && !bHideHex)
		{
			if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection) )
			{
				if( bLastTextNorm )
				{
					//crefOldColor = iSelTextColor;
					SetTextColor (hdc, iSelTextColor);
					SetBkColor (hdc, iSelBkColor);
					bLastTextNorm = FALSE;
				}
				else
				{
					SetTextColor (hdc, iSelTextColor);
					SetBkColor (hdc, iSelBkColor);
				}
			}
			else
			{
				if(bLastTextNorm)
				{
					
					if(p % 2 == 0)
						SetTextColor(hdc,iTextColorE);
					else if(p % 2 != 0)
						SetTextColor(hdc,iTextColorO);
				}
				else
				{
					if(p % 2 == 0)
						SetTextColor(hdc,iTextColorE);
					else if(p % 2 != 0)
						SetTextColor(hdc,iTextColorO);
					SetBkColor (hdc, iBkColor);
				}
			}
			if(p < vChanges.size())
			{
				if(vChanges[p] == COLORCHANGE)
					SetTextColor(hdc,iChangesColorValue);
				//else if(vChanges[p] == COLORHIGHLIGHT)
				//	SetTextColor(hdc,iHighlightColorValue);
				else if(vHighlights[p] == COLORHIGHLIGHT)
					SetTextColor(hdc,iHighlightColorValue);
			}
			if( bSelected && p == iEndOfSelection)
			{
				TextOut( hdc,
				BORDER_CLIENT_XDIM + ( iHexStart + ( p - iStartPos ) * iHexWidth - iHorizontalScrollPos ) * cCharXDim,
				BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
				ucLineBuf + iHexStart + ( p - iStartPos ) * iHexWidth,
				iHexWidth - 1 );
			}
			else
			{
				TextOut( hdc,
				BORDER_CLIENT_XDIM + ( iHexStart + ( p - iStartPos ) * iHexWidth - iHorizontalScrollPos ) * cCharXDim,
				BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
				ucLineBuf + iHexStart + ( p - iStartPos ) * iHexWidth,
				iHexWidth );
			}
			
			SetTextColor(hdc,crefOldColor);
		}
		if(!bHideHex)
		{
			s = iOffsetLength + iByteSpace + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos + 1);
		}
		else
		{
			s = iOffsetLength + iByteSpace /*+ iBytesPerLine * iHexWidth + iCharSpace*/ + ( p - iStartPos + 1);
		}
		if( iHorizontalScrollPos < s && iHorizontalScrollPos + cxBuffer >= s - 1 )
		{
			if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection) )
			{
				if( bLastTextNorm )
				{
					SetTextColor (hdc, iSelTextColor);
					SetBkColor (hdc, iSelBkColor);
					bLastTextNorm = FALSE;
				}
				else
				{
					SetTextColor (hdc, iSelTextColor);
					SetBkColor (hdc, iSelBkColor);
				}
			}
			else
			{
				if( !bLastTextNorm )
				{
					SetTextColor (hdc, iTextColor);
					SetBkColor (hdc, iBkColor);
					bLastTextNorm = TRUE;
				}
				else
				{
					SetTextColor (hdc, iTextColor);
					SetBkColor (hdc, iBkColor);
				}
			}
			if(p < vChanges.size())
			{
				if(vChanges[p] == COLORCHANGE)
					SetTextColor(hdc,iChangesColorValue);
				//else if(vChanges[p] == COLORHIGHLIGHT)
				else if(vHighlights[p] == COLORHIGHLIGHT)
					SetTextColor(hdc,iHighlightColorValue);
				//else if(vChanges[p] == COLORDTE)
				else if(vDTEs[p] == COLORDTE)
					SetTextColor(hdc,iDTEColorValue);
			}
			if(!bThingyView)
			{
				if(!bHideHex)
				{
					TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos ) - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					ucLineBuf + iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos ),
					1 );
				}
				else
				{
					TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart /*+ iBytesPerLine * iHexWidth + iCharSpace*/ + ( p - iStartPos ) - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					ucLineBuf + iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos ),
					1 );
				}
			}
			else
			{
				if(bJapanese)
				{
					//GetObject(hdc,GetObject( hdc,0,NULL),hfOld); 
					SelectObject( hdc, hfJap);
				}
				if(p-iStartPos < vAsciiLengths.size())
				{
					if(!bHideHex)
					{
						TextOut( hdc,
						BORDER_CLIENT_XDIM + ( totallength + iHexStart + iBytesPerLine * iHexWidth + iCharSpace  - iHorizontalScrollPos ) * cCharXDim,
						BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
						(output.substr(totallength,vAsciiLengths[p-iStartPos])).c_str(),
						vAsciiLengths[p - iStartPos]);
						totallength += vAsciiLengths[p-iStartPos];
					}
					else
					{
						TextOut( hdc,
						BORDER_CLIENT_XDIM + ( totallength + iHexStart /*+ iBytesPerLine * iHexWidth + iCharSpace*/  - iHorizontalScrollPos ) * cCharXDim,
						BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
						(output.substr(totallength,vAsciiLengths[p-iStartPos])).c_str(),
						vAsciiLengths[p - iStartPos]);
						totallength += vAsciiLengths[p-iStartPos];
					}
				}
				if(p + 1 > el)
				{
					if(!bHideHex)
					{
						TextOut( hdc,
						BORDER_CLIENT_XDIM + ( totallength + iHexStart + iBytesPerLine * iHexWidth + iCharSpace  - iHorizontalScrollPos ) * cCharXDim,
						BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
						(output.substr(totallength,output.length()-totallength)).c_str(),
						output.length()-totallength);
					}
					else
					{
						TextOut( hdc,
						BORDER_CLIENT_XDIM + ( totallength + iHexStart /*+ iBytesPerLine * iHexWidth + iCharSpace*/  - iHorizontalScrollPos ) * cCharXDim,
						BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
						(output.substr(totallength,output.length()-totallength)).c_str(),
						output.length()-totallength);
					}
				}
				if(bJapanese)
				{
					//DeleteObject( hdc);
					//SelectObject( hdc, hfOld);			
					make_font();
					SelectObject(hdc, hFont);
				}
			}
		}
	}
	s = iOffsetLength + iByteSpace + ( p - iStartPos + 1 ) * iHexWidth;
	e = s - iHexWidth;
	if( iHorizontalScrollPos < s && iHorizontalScrollPos + cxBuffer >= e && !bHideHex)
	{
		if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection ) )
		{
			if( bLastTextNorm )
			{
				TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart + ( p - iStartPos ) * iHexWidth - iHorizontalScrollPos ) * cCharXDim + 2 * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					" ",
					1 );
				crefOldColor = iSelTextColor;
				SetTextColor (hdc, iSelTextColor);
				SetBkColor (hdc, iSelBkColor);
				if(p < vChanges.size())
				{
					if(vChanges[p] == COLORCHANGE)
						SetTextColor(hdc,iChangesColorValue);
					//else if(vChanges[p] == COLORHIGHLIGHT)
					else if(vHighlights[p] == COLORHIGHLIGHT)
						SetTextColor(hdc,iHighlightColorValue);
				}
				bLastTextNorm = FALSE;
				bLastTextNorm = TRUE;
				TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart + ( p - iStartPos ) * iHexWidth - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					ucLineBuf + iHexStart + ( p - iStartPos ) * iHexWidth,
					iHexWidth - 1 );
			}
			else
			{
				crefOldColor = iSelTextColor;
				SetTextColor (hdc, iSelTextColor);
				SetBkColor (hdc, iSelBkColor);
				if(p < vChanges.size())
				{
					if(vChanges[p] == COLORCHANGE)
						SetTextColor(hdc,iChangesColorValue);
					//else if(vChanges[p] == COLORHIGHLIGHT)
					else if(vHighlights[p] == COLORHIGHLIGHT)
						SetTextColor(hdc,iHighlightColorValue);
				}
				TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart + ( p - iStartPos ) * iHexWidth - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					ucLineBuf + iHexStart + ( p - iStartPos ) * iHexWidth,
					iHexWidth - 1 );

				SetTextColor (hdc, iTextColor);
				crefOldColor = iTextColor;
				SetBkColor (hdc, iBkColor);
				bLastTextNorm = TRUE;
				TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart + ( p - iStartPos ) * iHexWidth - iHorizontalScrollPos ) * cCharXDim + 2 * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					" ",
					1 );
			}
		}
		else
		{
			if( !bLastTextNorm )
			{
				if(p < vChanges.size() && vChanges[p] == COLORCHANGE)
					SetTextColor(hdc,iChangesColorValue);
				else if(p < vChanges.size() && vHighlights[p] == COLORHIGHLIGHT)
					SetTextColor(hdc,iHighlightColorValue);
				else if(p % 2 == 0)
					SetTextColor(hdc,iTextColorE);
				else if(p % 2 != 0)
					SetTextColor(hdc,iTextColorO);
				crefOldColor = iTextColor;
				SetBkColor (hdc, iBkColor);
				bLastTextNorm = TRUE;
			}
			else
			{
				crefOldColor = GetTextColor(hdc);
				if(p < vChanges.size() && vChanges[p] == COLORCHANGE)
					SetTextColor(hdc,iChangesColorValue);
				else if(p < vChanges.size() && vHighlights[p] == COLORHIGHLIGHT)
					SetTextColor(hdc,iHighlightColorValue);
				else if(p % 2 == 0)
					SetTextColor(hdc,iTextColorE);
				else if(p % 2 != 0)
					SetTextColor(hdc,iTextColorO);	
			}
			TextOut( hdc,
				BORDER_CLIENT_XDIM + ( iHexStart + ( p - iStartPos ) * iHexWidth - iHorizontalScrollPos ) * cCharXDim,
				BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
				ucLineBuf + iHexStart + ( p - iStartPos ) * iHexWidth,
				iHexWidth );
			SetTextColor(hdc,crefOldColor);
		}
	}

	if(!bHideHex)//problem here I think
	{
		s = iOffsetLength + iByteSpace + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos + 1);
	}
	else
	{
		s = iOffsetLength + iByteSpace /*+ iBytesPerLine * iHexWidth* + iCharSpace*/ + ( p - iStartPos + 1);
	}
	if( iHorizontalScrollPos < s && iHorizontalScrollPos + cxBuffer >= s - 1 )
	{
		if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection ) )
		{
			if( bLastTextNorm )
			{
				SetTextColor (hdc, iSelTextColor);
				SetBkColor (hdc, iSelBkColor);
			}
			else
			{
				SetTextColor (hdc, iSelTextColor);
				SetBkColor (hdc, iSelBkColor);
			}
		}
		else
		{
			if( !bLastTextNorm )
			{
				SetTextColor (hdc, iTextColor);
				SetBkColor (hdc, iBkColor);
			}
			else
			{
				SetTextColor (hdc, iTextColor);
				SetBkColor (hdc, iBkColor);
			}
		}
		if(p < vChanges.size())
		{
			if(vChanges[p] == COLORCHANGE)
				SetTextColor(hdc,iChangesColorValue);
			//else if(vChanges[p] == COLORHIGHLIGHT)
			else if(vHighlights[p] == COLORHIGHLIGHT)
				SetTextColor(hdc,iHighlightColorValue);
			//else if(vChanges[p] == COLORDTE)
			else if(vDTEs[p] == COLORDTE)
				SetTextColor(hdc,iDTEColorValue);
		}
		if(!bThingyView)
		{
			if(!bHideHex)
			{
				TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos ) - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					ucLineBuf + iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos ),
					1 );
			}
			else
			{
				TextOut( hdc,
					BORDER_CLIENT_XDIM + ( iHexStart /*+ iBytesPerLine * iHexWidth + iCharSpace*/ + ( p - iStartPos ) - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					ucLineBuf + iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - iStartPos ),
					1 );
			}
		}
		else
		{
			if(bJapanese)
			{
				//GetObject(hdc,GetObject( hdc,0,NULL),hfOld); 
				SelectObject( hdc, hfJap);
			}
			if(p-iStartPos < vAsciiLengths.size())
			{
				if(!bHideHex)
				{
					TextOut( hdc,
					BORDER_CLIENT_XDIM + ( totallength + iHexStart + iBytesPerLine * iHexWidth + iCharSpace  - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					(output.substr(totallength,vAsciiLengths[p-iStartPos])).c_str(),
					vAsciiLengths[p - iStartPos]);
					totallength += vAsciiLengths[p-iStartPos];
				}
				else
				{
					TextOut( hdc,
					BORDER_CLIENT_XDIM + ( totallength + iHexStart /*+ iBytesPerLine * iHexWidth + iCharSpace*/  - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					(output.substr(totallength,vAsciiLengths[p-iStartPos])).c_str(),
					vAsciiLengths[p - iStartPos]);
					totallength += vAsciiLengths[p-iStartPos];
				}
			}
			if(p + 1 > el)
			{
				if(!bHideHex)
				{
					TextOut( hdc,
					BORDER_CLIENT_XDIM + ( totallength + iHexStart + iBytesPerLine * iHexWidth + iCharSpace  - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					(output.substr(totallength,output.length()-totallength)).c_str(),
					output.length()-totallength);
				}
				else
				{
					TextOut( hdc,
					BORDER_CLIENT_XDIM + ( totallength + iHexStart /*+ iBytesPerLine * iHexWidth + iCharSpace*/  - iHorizontalScrollPos ) * cCharXDim,
					BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
					(output.substr(totallength,output.length()-totallength)).c_str(),
					output.length()-totallength);
				}
			}
			if(bJapanese)
			{
				//SelectObject( hdc, hfOld);	
				make_font();
				SelectObject(hdc,hFont);
				//DeleteObject( hdc );
			}
			
		}
	}

	if(bHideHex)
	{
		m = (iOffsetLength+iByteSpace)*cCharXDim - cCharXDim/2 + 3*cCharXDim*4*0 - cCharXDim*iHorizontalScrollPos;
		MoveToEx (hdc, BORDER_CLIENT_XDIM+m, BORDER_CLIENT_XDIM+(iLine-iCurrentLine)*cCharYDim, NULL);
		LineTo (hdc, BORDER_CLIENT_XDIM+m, BORDER_CLIENT_XDIM+(iLine-iCurrentLine+1)*cCharYDim);
	}
	else
	{
		for (i = 0; i < (iBytesPerLine / 4) + 1; i++)
		{
			m = (iOffsetLength+iByteSpace)*cCharXDim - cCharXDim/2 + 3*cCharXDim*4*i - cCharXDim*iHorizontalScrollPos;
			MoveToEx (hdc, BORDER_CLIENT_XDIM+m, BORDER_CLIENT_XDIM+(iLine-iCurrentLine)*cCharYDim, NULL);
			LineTo (hdc, BORDER_CLIENT_XDIM+m, BORDER_CLIENT_XDIM+(iLine-iCurrentLine+1)*cCharYDim);
		}
	}

	if(!bHideHex)
	{
		m = (iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace)*cCharXDim - cCharXDim*iHorizontalScrollPos - 2;
		MoveToEx (hdc, BORDER_CLIENT_XDIM+m, BORDER_CLIENT_XDIM+(iLine-iCurrentLine)*cCharYDim, NULL);
		LineTo (hdc, BORDER_CLIENT_XDIM+m, BORDER_CLIENT_XDIM+(iLine-iCurrentLine+1)*cCharYDim);
		MoveToEx (hdc, BORDER_CLIENT_XDIM+m+2, BORDER_CLIENT_XDIM+(iLine-iCurrentLine)*cCharYDim, NULL);
		LineTo (hdc, BORDER_CLIENT_XDIM+m+2, BORDER_CLIENT_XDIM+(iLine-iCurrentLine+1)*cCharYDim);
	}
	el = iStartPos + iBytesPerLine - 1;
	int iCharCoordinate;
	RECT r;
	long bookmark = 0;
	for( i = 0; i < iBookmarkCount && !bHideHex; i++ )
	{
		bookmark = pbmkList[i].offset;
		if(iOffsetMode == LOROM)
			bookmark = fromLOROM(bookmark);
		else if(iOffsetMode == HIROM)
			bookmark = fromHIROM(bookmark);
		if( IN_BOUNDS( bookmark, iStartPos, el ) && bookmark <= FileBuffer.Upper() )
		{
			iCharCoordinate = iOffsetLength + iByteSpace + ( bookmark % iBytesPerLine )*3 - iHorizontalScrollPos;
			r.left = BORDER_CLIENT_XDIM + iCharCoordinate * cCharXDim;
			r.top = BORDER_CLIENT_XDIM + ( bookmark / iBytesPerLine - iCurrentLine ) * cCharYDim;
			r.right = BORDER_CLIENT_XDIM + r.left + 2*cCharXDim;
			r.bottom = BORDER_CLIENT_XDIM + ( bookmark / iBytesPerLine - iCurrentLine + 1 ) * cCharYDim;
			FrameRect( hdc, &r, hbr );

			iCharCoordinate = iOffsetLength + iByteSpace + iBytesPerLine*3 + iCharSpace
				+ ( bookmark % iBytesPerLine ) - iHorizontalScrollPos;
			r.left = BORDER_CLIENT_XDIM + iCharCoordinate * cCharXDim;
			r.top = BORDER_CLIENT_XDIM + ( bookmark / iBytesPerLine - iCurrentLine) * cCharYDim;
			r.right = BORDER_CLIENT_XDIM + ( iCharCoordinate + 1 ) * cCharXDim;
			r.bottom = BORDER_CLIENT_XDIM + ( bookmark / iBytesPerLine - iCurrentLine + 1 ) * cCharYDim;
			if(!bThingyView)
			{
				FrameRect( hdc, &r, hbr );
			}
		}
	}
	if(bHideHex)
	{
		int j = 0;
		string blank;
		for(j = 0;j < iBytesPerLine*4;j++)
			blank += " ";
		TextOut( hdc,
		BORDER_CLIENT_XDIM + ( totallength + iHexStart + iBytesPerLine /** iHexWidth + iCharSpace*/  - iHorizontalScrollPos ) * cCharXDim,
		BORDER_CLIENT_XDIM + ( iLine - iCurrentLine ) * cCharYDim,
		blank.c_str(),
		blank.length());
	}
	return;
}

void Translhextion::adjust_view_for_cursor ()
{
	int iColumn_logical;
	if (iTextEntryMode == BYTES)
		iColumn_logical = (iOffsetLength + iByteSpace + (iCurrentByte % iBytesPerLine) * 3 + iCurrentNibble);
	else
		iColumn_logical = ((iOffsetLength + iByteSpace + iBytesPerLine * 3 + iCharSpace) + (iCurrentByte % iBytesPerLine));

	if (iColumn_logical >= iHorizontalScrollPos+cxBuffer)
		iHorizontalScrollPos = iColumn_logical-(cxBuffer-1);
	else if (iColumn_logical < iHorizontalScrollPos)
		iHorizontalScrollPos = iColumn_logical;
	update_horizontal_scrollbar ();
}

int Translhextion::lbuttonup (int xPos, int yPos)
{
	KillTimer (hwnd, iTimerID);
	bTimerSet = FALSE;
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	ReleaseCapture ();
	bLButtonIsDown = FALSE;
	if (iLBDownX == xPos && iLBDownY == yPos)
	{
		int iColumnOnScreen = xPos / cCharXDim, 
			iRowOnScreen = yPos / cCharYDim,	
			iColumn_logical = iColumnOnScreen + iHorizontalScrollPos,	
			iLine = iCurrentLine + iRowOnScreen; 

		if (iColumn_logical >= iOffsetLength + iByteSpace && iColumn_logical <= iCharsPerLine - 1 - iBytesPerLine - iCharSpace - 1)
		{
			if ((iColumn_logical-iOffsetLength-(iByteSpace-1))%3 != 0)
			{
				int bytenum = (iColumn_logical-iOffsetLength-(iByteSpace-1))/3,
					iNumOfNib = ((iColumn_logical-iOffsetLength-(iByteSpace-1))%3)-1;
				int newpos = iLine * iBytesPerLine + bytenum;
				if( newpos <= (FileBuffer.Upper ()) + 1 )
				{
					iCurrentByte = newpos;
					iCurrentNibble = iNumOfNib;
					iTextEntryMode = BYTES;
					repaint ();
				}
			}
		}
		else if (!bThingyView && iColumn_logical > iCharsPerLine - 1 - iBytesPerLine	&& iColumn_logical < iCharsPerLine)
		{
			int newpos = iLine * iBytesPerLine + (iColumn_logical - (iCharsPerLine - iBytesPerLine));
			if( newpos <= (FileBuffer.Upper ()) + 1 )
			{
				iCurrentByte = newpos;
				iTextEntryMode = CHARS;
				repaint ();
			}
		}
	}
	return 0;
}

int Translhextion::rbuttonup (int xPos, int yPos)
{
	KillTimer (hwnd, iTimerID);
	bTimerSet = FALSE;
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	ReleaseCapture ();
	int iColumnOnScreen = xPos / cCharXDim, 
		iRowOnScreen = yPos / cCharYDim,	
		iColumn_logical = iColumnOnScreen + iHorizontalScrollPos,	
		iLine = iCurrentLine + iRowOnScreen; 
	//HMENU context = LoadMenu(hInstance,MAKEINTRESOURCE(IDR_RIGHTCLICKMENU));
	HMENU context = CreatePopupMenu();
	AppendMenu (context, MF_ENABLED | MF_STRING, 1, "Cu&t");
	if(bThingyView)
	{
		AppendMenu (context, MF_ENABLED | MF_STRING, 2, "Thingy &Copy");
	}
	else
	{
		AppendMenu (context, MF_ENABLED | MF_STRING, 2, "&Copy");
	}
	AppendMenu (context, MF_ENABLED | MF_STRING, 3, "&Paste");
	AppendMenu (context, MF_ENABLED | MF_STRING, 4, "P&aste preview");
	if((bTableLoaded && bTableActive && FileBuffer.Length() > 0 ) ||
		(bTableLoaded && bTableActive && !bReadOnly) || ( FileBuffer.Length() > 0 ))
		AppendMenu (context, MF_ENABLED | MF_SEPARATOR, 0, "appended");
	AppendMenu (context, MF_ENABLED | MF_STRING, 5, "&Dump Script...");
	AppendMenu (context, MF_ENABLED | MF_STRING, 6, "&Replace Script...");
	AppendMenu (context, MF_ENABLED | MF_STRING, 7, "H&exdump...");
	if(( FileBuffer.Length() > 0 ))
		AppendMenu (context, MF_ENABLED | MF_SEPARATOR, 0, "appended");
	AppendMenu (context, MF_ENABLED | MF_STRING, 8, "&Jump To...");
	if(( FileBuffer.Length() > 0 ))
		AppendMenu (context, MF_ENABLED | MF_SEPARATOR, 0, "appended");
	AppendMenu (context, MF_ENABLED | MF_STRING, 9, "Select &all");
	if( ( bSelected || iCurrentByte <= FileBuffer.Upper() ) && !bReadOnly && !bPartialOpen )
		;
	else
		RemoveMenu(context,1,MF_BYCOMMAND); //CUT
	if( bSelected || iCurrentByte <= FileBuffer.Upper() )
		;
	else
		RemoveMenu(context,2,MF_BYCOMMAND); //COPY
	if( !bReadOnly && !bSelected )
	{
		if( OpenClipboard( NULL ) )
		{
			HGLOBAL hClipMemory = GetClipboardData( CF_TEXT );
			if( hClipMemory != NULL )
				;
			else
				RemoveMenu(context,3,MF_BYCOMMAND); //PASTE
			CloseClipboard ();
		}
		else
		{
			// Clipboard could not be opened => can't paste.
			RemoveMenu(context,3,MF_BYCOMMAND); //PASTE
		}
	}
	else
		RemoveMenu(context,3,MF_BYCOMMAND); //PASTE
	if( !bReadOnly && !bSelected )
		;
	else
		RemoveMenu(context,4,MF_BYCOMMAND); //PASTE PREVIEW
	if(bTableLoaded && bTableActive && FileBuffer.Length() > 0 )
		;
	else
		RemoveMenu(context,5,MF_BYCOMMAND);//DUMP SCRIPT
	if(bTableLoaded && bTableActive && !bReadOnly)
		;
	else
		RemoveMenu(context,6,MF_BYCOMMAND);//REPLACE SCRIPT
	if( FileBuffer.Length() > 0 )
		;
	else
		RemoveMenu(context,7,MF_BYCOMMAND); //HEXDUMP
	if( FileBuffer.Length() > 0 )
		;
	else
		RemoveMenu(context,8,MF_BYCOMMAND); //JUMP TO
	if( FileBuffer.Length() > 0 )
		;
	else
		RemoveMenu(context,9,MF_BYCOMMAND); //SELECT ALL
	
	POINT pt;
	pt.x = xPos;
	pt.y = yPos;
	ClientToScreen(hwnd, &pt); 
	//iWindowWidth = wndpl.rcNormalPosition.right - iWindowX;
	//iWindowHeight = wndpl.rcNormalPosition.bottom - iWindowY;
	UINT Msg = TrackPopupMenu(context,TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN,pt.x,pt.y,0,hwnd,0);

	//UINT Msg = TrackPopupMenu(context,TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN,xPos + iWindowX,yPos + iWindowY + 40,0,hwnd,0);
	switch(Msg)
	{
	case 0:
		break;
	case 1:
		command(IDM_EDIT_CUT);
		break;
	case 2:
		command(IDM_EDIT_COPY);
		break;
	case 3:
		command(IDM_EDIT_PASTE);
		break;
	case 4:
		command(IDM_PASTE_WITH_DLG);
		break;
	case 5:
		command(IDM_SCRIPT_DUMP);
		break;
	case 6:
		command(IDM_REPLACE_SCRIPT);
		break;
	case 7:
		command(IDM_COPY_HEXDUMP);
		break;
	case 8:
		command(IDM_GO_TO);
		break;
	case 9:
		command(IDM_SELECT_ALL);
		break;
	default:
		break;
	}
	return true;
}

int Translhextion::mousemove (int xPos, int yPos)
{
	iMouseX = xPos;
	iMouseY = yPos;
	if (bLButtonIsDown == FALSE)
		return 0;
	bSelected = TRUE;
	if (GetCapture() == NULL)
	{
		SetCapture (hwnd);
		SetCursor( LoadCursor( NULL, IDC_IBEAM ) );
	}

	int iColumnOnScreen = xPos / cCharXDim,
		iRowOnScreen = yPos / cCharYDim,
		iColumn_logical = iColumnOnScreen + iHorizontalScrollPos,
		iLine = iCurrentLine + iRowOnScreen;

	if (yPos>cBufferY*cCharYDim || yPos<0)
	{
		if (bTimerSet==FALSE)
		{
			SetTimer (hwnd, iTimerID, iTimerLength, NULL);
			bTimerSet = TRUE;
		}
	}
	else if (iColumn_logical >= iOffsetLength + iByteSpace && iColumn_logical <= iCharsPerLine - 1 - iBytesPerLine - iCharSpace - 1)
	{
		KillTimer (hwnd, iTimerID);
		bTimerSet = FALSE;
		if ((iColumn_logical-iOffsetLength-(iByteSpace-1))%3 != 0)
		{
			int bytenum = (iColumn_logical-iOffsetLength-(iByteSpace-1))/3,
				iNumOfNib = ((iColumn_logical-iOffsetLength-(iByteSpace-1))%3)-1;
			int newpos = iLine * iBytesPerLine + bytenum;
			if (newpos <= (FileBuffer.Upper ()))
			{
				iEndOfSelection = newpos;
				repaint ();
			}
		}
	}
	else if (iColumn_logical > iCharsPerLine - 1 - iBytesPerLine	&& iColumn_logical < iCharsPerLine)
	{
		KillTimer (hwnd, iTimerID);
		bTimerSet = FALSE;
		int newpos = iLine * iBytesPerLine + (iColumn_logical - (iCharsPerLine - iBytesPerLine));
		if (newpos <= (FileBuffer.Upper ()))
		{
			iEndOfSelection = newpos;
			repaint ();
		}
	}
	return 0;
}

int Translhextion::lbuttondown (int xPos, int yPos)
{
	if ((filename[0] == '\0'))
		return 0;
	if(bHideHex)
	{
		return 0;
	}
	int iColumnOnScreen = xPos / cCharXDim,
		iRowOnScreen = yPos / cCharYDim,
		iColumn_logical = iColumnOnScreen + iHorizontalScrollPos,
		iLine = iCurrentLine + iRowOnScreen;
	iLBDownX = xPos;
	iLBDownY = yPos;

	if (iColumn_logical >= iOffsetLength + iByteSpace && iColumn_logical <= iCharsPerLine - 1 - iBytesPerLine - iCharSpace - 1)
	{
		if ((iColumn_logical-iOffsetLength-(iByteSpace-1))%3 != 0)
		{
			bLButtonIsDown = TRUE;
			bSelected = FALSE;
			int bytenum = (iColumn_logical-iOffsetLength-(iByteSpace-1))/3,
				iNumOfNib = ((iColumn_logical-iOffsetLength-(iByteSpace-1))%3)-1;
			int newpos = iLine * iBytesPerLine + bytenum;
			if( newpos <= (FileBuffer.Upper ()) )
			{
				iStartOfSelection = newpos;
				iEndOfSelection = newpos;
			}
			else if( newpos == (FileBuffer.Upper ()) + 1 )
			{
				bLButtonIsDown = FALSE; 
			}
		}
	}
	
	else if (!bThingyView && iColumn_logical > iCharsPerLine - 1 - iBytesPerLine	&& iColumn_logical < iCharsPerLine)
	{
		bLButtonIsDown = TRUE;
		bSelected = FALSE;
		int newpos = iLine * iBytesPerLine + (iColumn_logical - (iCharsPerLine - iBytesPerLine));
		if( newpos <= (FileBuffer.Upper ()) )
		{
			iStartOfSelection = newpos;
			iEndOfSelection = newpos;
		}
		else if( newpos == (FileBuffer.Upper ()) + 1 )
		{
			bLButtonIsDown = FALSE; 
		}
	}
	return 0;
}



void Translhextion::update_size_change ()
{
	RECT r;
	GetClientRect (hwnd, &r);
	resize_window (r.right, r.bottom);
}


int Translhextion::select_with_arrowkeys (int key)
{
	if (filename[0] == '\0' || iCurrentByte>(FileBuffer.Upper ()) || iCurrentByte<0)
		return 0;
	switch (key)
	{
	case IDA_SELECTSTARTOFLINE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			iEndOfSelection = iCurrentByte;
		}
		iEndOfSelection=iEndOfSelection/iBytesPerLine*iBytesPerLine;
		break;

	case IDA_SELECTENDOFLINE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			iEndOfSelection = iCurrentByte;
		}
		iEndOfSelection=(iEndOfSelection/iBytesPerLine+1)*iBytesPerLine-1;
		if (iEndOfSelection>(FileBuffer.Upper ()))
			iEndOfSelection = (FileBuffer.Upper ());
		break;

	case IDA_SELECTSTARTOFFILE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
		}
		iEndOfSelection=0;
		break;

	case IDA_SELECTENDOFFILE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
		}
		iEndOfSelection = (FileBuffer.Upper ());
		break;

	case IDA_SELECTPAGEDOWN:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			iEndOfSelection = iCurrentByte;
		}
		iEndOfSelection += cBufferY*iBytesPerLine;
		if (iEndOfSelection>(FileBuffer.Upper ()))
			iEndOfSelection=(FileBuffer.Upper ());
		break;

	case IDA_SELECTPAGEUP:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			iEndOfSelection = iCurrentByte;
		}
		iEndOfSelection -= cBufferY*iBytesPerLine;
		if (iEndOfSelection<0)
			iEndOfSelection=0;
		break;

	default:
		if (!bSelected)
		{
			
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			iEndOfSelection = iCurrentByte;
			adjust_view_for_selection ();
			repaint ();
			return 1;
		}
		switch (key)
		{
		case IDA_SELECTLEFT:
			if (iEndOfSelection > 0)
				iEndOfSelection--;
			break;

		case IDA_SELECTRIGHT:
			if (iEndOfSelection < (FileBuffer.Upper ()))
				iEndOfSelection++;
			break;

		case IDA_SELECTUP:
			if (iEndOfSelection > iBytesPerLine)
				iEndOfSelection -= iBytesPerLine;
			else
				iEndOfSelection=0;
			break;

		case IDA_SELECTDOWN:
			if (iEndOfSelection+iBytesPerLine < (FileBuffer.Upper ()))
				iEndOfSelection += iBytesPerLine;
			else
				iEndOfSelection = (FileBuffer.Upper ());
			break;

		default:
			break;
		}
	}
	iCurrentByte = iEndOfSelection;
	adjust_view_for_selection ();
	repaint ();
	return 1;
}


void Translhextion::adjust_view_for_selection ()
{
	if( bSelected )
	{
		
		if( iEndOfSelection / iBytesPerLine < iCurrentLine )
			
			iCurrentLine = iEndOfSelection / iBytesPerLine;

		
		else if( iEndOfSelection/iBytesPerLine > iCurrentLine+cBufferY - 1 )
		{
			
			iCurrentLine = iEndOfSelection / iBytesPerLine - cBufferY + 1;
			if( iCurrentLine < 0 )
				iCurrentLine = 0;
		}
		update_vertical_scrollbar();
	}
}


int Translhextion::read_ini_data ()
{
	
	HKEY hkeyKEY;

	char cRegKeyName[64];
	sprintf( cRegKeyName, ("Software\\" + sName + "\\" + sVersion + "\\%d").c_str(), iNumInstances );
	LONG iResourceKey = RegOpenKeyEx( HKEY_CURRENT_USER,
		cRegKeyName,
		0,
		KEY_ALL_ACCESS,
		&hkeyKEY );
	if( iResourceKey == ERROR_SUCCESS )
	{
		
		DWORD datasize = sizeof( int );
		LONG iResourceKey;
		iResourceKey = RegQueryValueEx( hkeyKEY, "iTextColorValue", NULL, NULL, (BYTE*) &iTextColorValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iTextColorEValue", NULL, NULL, (BYTE*) &iTextColorEValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iTextColorOValue", NULL, NULL, (BYTE*) &iTextColorOValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iBkColorValue", NULL, NULL, (BYTE*) &iBkColorValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iSepColorValue", NULL, NULL, (BYTE*) &iSepColorValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iSelTextColorValue", NULL, NULL, (BYTE*) &iSelTextColorValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iSelBkColorValue", NULL, NULL, (BYTE*) &iSelBkColorValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iBookmarkColor", NULL, NULL, (BYTE*) &iBookmarkColor, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iHighlightColorValue", NULL, NULL, (BYTE*) &iHighlightColorValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iChangesColorValue", NULL, NULL, (BYTE*) &iChangesColorValue, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iDTEColorValue", NULL, NULL, (BYTE*) &iDTEColorValue, &datasize );

		iResourceKey = RegQueryValueEx( hkeyKEY, "bAutomaticBPL", NULL, NULL, (BYTE*) &bAutomaticBPL, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iBytesPerLine", NULL, NULL, (BYTE*) &iBytesPerLine, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iOffsetLength", NULL, NULL, (BYTE*) &iOffsetLength, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iCharacterSet", NULL, NULL, (BYTE*) &iCharacterSet, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iFontSize", NULL, NULL, (BYTE*) &iFontSize, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iOffsetMode", NULL, NULL, (BYTE*) &iOffsetMode, &datasize );

		iResourceKey = RegQueryValueEx( hkeyKEY, "bOpenReadOnly", NULL, NULL, (BYTE*) &bOpenReadOnly, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "bDefBytemark", NULL, NULL, (BYTE*) &bDefByteMark, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "bDiscardChangesSave", NULL, NULL, (BYTE*) &bDiscardChangesSave, &datasize );
		cTextColor = iTextColorValue;
		cBkColor = iBkColorValue;
		cTextColorE = iTextColorEValue;
		cTextColorO = iTextColorOValue;
		cSepColor = iSepColorValue;
		cSelBkColor = iSelBkColorValue;
		cSelTextColor = iSelTextColorValue;
		cChangesColor = iChangesColorValue;
		cHighlightColor = iHighlightColorValue;
		cBookmarkColor = iBookmarkColor;
		cDTEColor = iDTEColorValue;
		char szPath[ _MAX_PATH + 1 ];
		datasize = _MAX_PATH + 1;
		iResourceKey = RegQueryValueEx( hkeyKEY, "sTexteditorName", NULL, NULL, (BYTE*) &szPath, &datasize );
		sTexteditorName = szPath;

		datasize = _MAX_PATH + 1;
		char temp[6];
		iResourceKey = RegQueryValueEx( hkeyKEY, "sBrowserName", NULL, NULL, (BYTE*) &szPath, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "LeftByteSep", NULL, NULL, (BYTE*) &temp, &datasize );
		sByteOpen = "";
		sByteOpen += temp[0];
		iResourceKey = RegQueryValueEx( hkeyKEY, "RightByteSep", NULL, NULL, (BYTE*) &temp, &datasize );
		sByteClose = "";
		sByteClose += temp[0];
		sBrowserName = szPath;

		iResourceKey = RegQueryValueEx( hkeyKEY, "iWindowShowCmd", NULL, NULL, (BYTE*) &iWindowShowCmd, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iWindowX", NULL, NULL, (BYTE*) &iWindowX, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iWindowY", NULL, NULL, (BYTE*) &iWindowY, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iWindowWidth", NULL, NULL, (BYTE*) &iWindowWidth, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iWindowHeight", NULL, NULL, (BYTE*) &iWindowHeight, &datasize );
		
		iResourceKey = RegQueryValueEx( hkeyKEY, "iMostRecentCount", NULL, NULL, (BYTE*) &iMostRecentCount, &datasize );
		iResourceKey = RegQueryValueEx( hkeyKEY, "iMostRecentTableCount", NULL, NULL, (BYTE*) &iMostRecentTableCount, &datasize );
		int i;
		char fFileName[64];
		for( i = 1; i <= iMostRecentMaximum; i++ )
		{
			sprintf( fFileName, "MRU_File%d", i );
			datasize = _MAX_PATH + 1;
			iResourceKey = RegQueryValueEx( hkeyKEY, fFileName, NULL, NULL, (BYTE*) &szPath, &datasize );
			strcpy( &( strMostRecent[i-1][0] ), szPath );
		}
		for( i = 1; i <= iMostRecentTableMaximum; i++ )
		{
			sprintf( fFileName, "MRT_File%d", i );
			datasize = _MAX_PATH + 1;
			iResourceKey = RegQueryValueEx( hkeyKEY, fFileName, NULL, NULL, (BYTE*) &szPath, &datasize );
			strcpy( &( strMostRecentTable[i-1][0] ), szPath );
		}
		 if( iResourceKey != ERROR_SUCCESS )
			MessageBox( hwnd, "Some values could not be initialized.", "Registry error:", MB_OK | MB_ICONERROR);

		
		RegCloseKey( hkeyKEY );
	}
	else
	{
		
		MessageBox( hwnd, (sName + " is being started for the first time.\n"
			" It is attempting to write values to the registry.").c_str(), "Notice:", MB_OK | MB_ICONINFORMATION);
		if(CopyFile((sAppPath + "\\" + FONT).c_str(),(sWindowsPath + "\\FONTS\\" + FONT).c_str(),FALSE))
		{
			MessageBox( hwnd,((string) FONT + " has been successfully installed in " + sWindowsPath + "\\FONTS\\.").c_str(),"Notice:",MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			
			MessageBox( hwnd,("Installation of " + (string) FONT + " in " + sWindowsPath + "\\FONTS\\ has failed."
				"  Be sure to install this font in this system's FONTS"
			" directory (probably C:\\WINDOWS\\FONTS\\ or C:\\WINNT\\FONTS\\) so that all of " + sName + "'s features will be enabled.").c_str(),"Notice:",MB_OK | MB_ICONINFORMATION);
		}
		save_ini_data();
	}
	return 0;
}


int Translhextion::save_ini_data ()
{
	HKEY hkeyKEY;
	
	char cRegKeyName[64];
	sprintf( cRegKeyName, ("Software\\" + sName + "\\" + sVersion + "\\%d").c_str(), iNumInstances );
	
	LONG iResourceKey = RegCreateKey( HKEY_CURRENT_USER, cRegKeyName, &hkeyKEY );

	if( iResourceKey == ERROR_SUCCESS )
	{
		iSelBkColorValue = cSelBkColor;
		iSelTextColorValue = cSelTextColor;
		iBookmarkColor = cBookmarkColor;
		iTextColorValue = cTextColor;
		iBkColorValue = cBkColor;
		iTextColorEValue = cTextColorE;
		iTextColorOValue = cTextColorO;
		iSepColorValue = cSepColor;
		iChangesColorValue = cChangesColor;
		iHighlightColorValue = cHighlightColor;
		iDTEColorValue = cDTEColor;
		RegSetValueEx( hkeyKEY, "iTextColorValue", 0, REG_DWORD, (CONST BYTE*) &iTextColorValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iTextColorEValue", 0, REG_DWORD, (CONST BYTE*) &iTextColorEValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iTextColorOValue", 0, REG_DWORD, (CONST BYTE*) &iTextColorOValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iBkColorValue", 0, REG_DWORD, (CONST BYTE*) &iBkColorValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iSepColorValue", 0, REG_DWORD, (CONST BYTE*) &iSepColorValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iSelTextColorValue", 0, REG_DWORD, (CONST BYTE*) &iSelTextColorValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iSelBkColorValue", 0, REG_DWORD, (CONST BYTE*) &iSelBkColorValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iBookmarkColor", 0, REG_DWORD, (CONST BYTE*) &iBookmarkColor, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iChangesColorValue", 0, REG_DWORD, (CONST BYTE*) &iChangesColorValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iDTEColorValue", 0, REG_DWORD, (CONST BYTE*) &iDTEColorValue, sizeof( int ) );

		RegSetValueEx( hkeyKEY, "iHighlightColorValue", 0, REG_DWORD, (CONST BYTE*) &iHighlightColorValue, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "bAutomaticBPL", 0, REG_DWORD, (CONST BYTE*) &bAutomaticBPL, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iBytesPerLine", 0, REG_DWORD, (CONST BYTE*) &iBytesPerLine, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iOffsetLength", 0, REG_DWORD, (CONST BYTE*) &iOffsetLength, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iCharacterSet", 0, REG_DWORD, (CONST BYTE*) &iCharacterSet, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iFontSize", 0, REG_DWORD, (CONST BYTE*) &iFontSize, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iOffsetMode", 0, REG_DWORD, (CONST BYTE*) &iOffsetMode, sizeof( int ) );

		RegSetValueEx( hkeyKEY, "bOpenReadOnly", 0, REG_DWORD, (CONST BYTE*) &bOpenReadOnly, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "bDefByteMark", 0, REG_DWORD, (CONST BYTE*) &bDefByteMark, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "bDiscardChangesSave", 0, REG_DWORD, (CONST BYTE*) &bDiscardChangesSave, sizeof( int ) );
	
		RegSetValueEx( hkeyKEY, "LeftByteSep", 0, REG_SZ, (CONST BYTE*) (char*) sByteOpen.c_str(), 2 );
		RegSetValueEx( hkeyKEY, "RightByteSep", 0, REG_SZ, (CONST BYTE*) (char*) sByteClose.c_str(), 2 );

		RegSetValueEx( hkeyKEY, "sTexteditorName", 0, REG_SZ, (CONST BYTE*) (char*) sTexteditorName.c_str(), sTexteditorName.length() + 1 );
		RegSetValueEx( hkeyKEY, "sBrowserName", 0, REG_SZ, (CONST BYTE*) (char*) sBrowserName.c_str(), sBrowserName.length() + 1 );

		RegSetValueEx( hkeyKEY, "iWindowShowCmd", 0, REG_DWORD, (CONST BYTE*) &iWindowShowCmd, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iWindowX", 0, REG_DWORD, (CONST BYTE*) &iWindowX, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iWindowY", 0, REG_DWORD, (CONST BYTE*) &iWindowY, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iWindowWidth", 0, REG_DWORD, (CONST BYTE*) &iWindowWidth, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iWindowHeight", 0, REG_DWORD, (CONST BYTE*) &iWindowHeight, sizeof( int ) );

		RegSetValueEx( hkeyKEY, "iMostRecentCount", 0, REG_DWORD, (CONST BYTE*) &iMostRecentCount, sizeof( int ) );
		RegSetValueEx( hkeyKEY, "iMostRecentTableCount", 0, REG_DWORD, (CONST BYTE*) &iMostRecentTableCount, sizeof( int ) );
		int i;
		char fFileName[ 64 ];
		for( i = 1; i <= iMostRecentMaximum; i++ )
		{
			sprintf( fFileName, "MRU_File%d", i );
			RegSetValueEx( hkeyKEY, fFileName, 0, REG_SZ, (CONST BYTE*) &(strMostRecent[i-1][0]), strlen( &(strMostRecent[i-1][0]) ) + 1 );
		}
		for( i = 1; i <= iMostRecentTableMaximum; i++ )
		{
			sprintf( fFileName, "MRT_File%d", i );
			RegSetValueEx( hkeyKEY, fFileName, 0, REG_SZ, (CONST BYTE*) &(strMostRecentTable[i-1][0]), strlen( &(strMostRecentTable[i-1][0]) ) + 1 );
		}
		RegCloseKey( hkeyKEY );
	}
	else
	{
		MessageBox( hwnd, "Preferences could not be saved to the registry.", "Registry error:", MB_OK | MB_ICONERROR );
	}
	return 0;
}



void Translhextion::draw_border_client_rect (HDC hdc)
{
	
	int retrieved_device_context = FALSE;
	if (hdc == 0)
	{
		hdc = GetDC (hwnd);
		retrieved_device_context = TRUE;
	}
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = cClientXDim;
	r.bottom = cClientYDim;
	DrawEdge (hdc, &r, EDGE_SUNKEN, BF_RECT);
	if (retrieved_device_context)
	{
		ReleaseDC (hwnd, hdc);
	}
}


int Translhextion::on_deletekey ()
{
	iCutMode = BST_UNCHECKED;
	return edit_cut ();
}


int Translhextion::on_backspace ()
{
	if (iInsertMode && !bReadOnly)
	{
		
		if (iCurrentByte > 0)
		{
			if (FileBuffer.RemoveAt (iCurrentByte-1, 1) == TRUE)
			{
				vChanges.remove(iCurrentByte-1);
				iCurrentByte--;
				update_size_change ();
				iFileChanged = TRUE;
			}
			else
				MessageBox (hwnd, "Byte could not be removed.", "Editing error:", MB_OK | MB_ICONERROR);
			
		}
	}
	else
	{
		
		if (iCurrentByte>0)
			iCurrentByte--;
		repaint ();
	}
	return 1;
}


int Translhextion::make_font ()
{
	if (hFont != NULL)
		DeleteObject (hFont);
	HDC hdc = GetDC (hwnd);
	int nHeight = -MulDiv(iFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC (hwnd, hdc);
	int cset;
	if (iCharacterSet==ANSI_FIXED_FONT)
		cset = ANSI_CHARSET;
	else
		cset = OEM_CHARSET;
	hFont = CreateFont (nHeight,0,0,0,0,0,0,0,cset,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_DONTCARE,0);
	return 1;
}

int Translhextion::make_jfont()
{
	if (hfJap != NULL)
		DeleteObject (hfJap);
	HDC hdc = GetDC (hwnd);
	int nHeight2 = -MulDiv(iFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC (hwnd, hdc);
	int cset;
	if (iCharacterSet==ANSI_FIXED_FONT)
		cset = ANSI_CHARSET;
	else
		cset = OEM_CHARSET;
	hfJap = CreateFont (nHeight2,0,0,0,0,0,0,0,cset,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_DONTCARE,"Januschan JOME Regular");
	return 1;
}


char Translhextion::TranslateAnsiToOem (char c)
{
	char sbuf[2], dbuf[2];
	sbuf[0]=c;
	sbuf[1]=0;
	CharToOemBuff (sbuf, dbuf, 1);
	return dbuf[0];
}


int Translhextion::timer (WPARAM w, LPARAM l)
{
	if (!bSelected || !bTimerSet)
		return 0;

	if (iMouseY > cBufferY*cCharYDim)
	{
		
		if (iCurrentLine < (iNumlines-1))
			iCurrentLine++;
		update_vertical_scrollbar ();
		repaint ();
	}
	else if (iMouseY < 0)
	{
		
		if (iCurrentLine > 0)
			iCurrentLine--;
		update_vertical_scrollbar ();
		repaint ();
	}
	return 1;
}

int Translhextion::dropfiles( HANDLE hDrop )
{
	char lpszFile[ _MAX_PATH+1 ];
	DragQueryFile( (HDROP) hDrop, 0, lpszFile, _MAX_PATH+1 );
	DragFinish( (HDROP) hDrop ); 
	SetForegroundWindow( hwnd );
	if( iFileChanged == TRUE )
	{
		if( MessageBox (hwnd, "You have not saved your changes! Discard them and open anyway?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO )
			return 0;
	}

	char lpszTarget[ MAX_PATH ];
	
	HRESULT hres = ResolveIt( hwnd, lpszFile, lpszTarget );
	if( SUCCEEDED( hres ) )
	{
		
		int ret = MessageBox( hwnd,
			"This is a link file.\n"
			"Choose Yes if you want to open the file it links to.\n"
			"Choose No if you want to open the link file itself.\n"
			"Choose Cancel if you want to cancel.",
			"Notice:", MB_YESNOCANCEL | MB_ICONQUESTION );
		switch( ret )
		{
		case IDYES:
			if( load_file( lpszTarget ) )
			{
				iVerticalScrollMax = 0;
				iVerticalScrollPos = 0;
				iVerticalScrollInc = 0;
				iHorizontalScrollMax = 0;
				iHorizontalScrollPos = 0;
				iHorizontalScrollInc = 0;
				iCurrentLine = 0;
				iCurrentByte = 0;
				iCurrentNibble = 0;
				iFileSize = FileBuffer.Length();
				iFileChanged = FALSE;
				bFilestatusChanged = TRUE;
				update_size_change();
			}					
			break;

		case IDNO:
			if( load_file( lpszFile ) )
			{
				iVerticalScrollMax = 0;
				iVerticalScrollPos = 0;
				iVerticalScrollInc = 0;
				iHorizontalScrollMax = 0;
				iHorizontalScrollPos = 0;
				iHorizontalScrollInc = 0;
				iCurrentLine = 0;
				iCurrentByte = 0;
				iCurrentNibble = 0;
				iFileSize = FileBuffer.Length();
				iFileChanged = FALSE;
				bFilestatusChanged = TRUE;
				update_size_change();
			}					
			break;

		case IDCANCEL:
			break;
		}
	}
	else
	{
		if( load_file( lpszFile ) )
		{
			iVerticalScrollMax = 0;
			iVerticalScrollPos = 0;
			iVerticalScrollInc = 0;
			iHorizontalScrollMax = 0;
			iHorizontalScrollPos = 0;
			iHorizontalScrollInc = 0;
			iCurrentLine = 0;
			iCurrentByte = 0;
			iCurrentNibble = 0;
			iFileSize = FileBuffer.Length();
			iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
			update_size_change();
		}
	}
	return 1;
}

HRESULT ResolveIt( HWND hwnd, LPCSTR lpszLinkFile, LPSTR lpszPath )
{ 
	HRESULT hres; 
	IShellLink* psl; 
	char szGotPath[MAX_PATH]; 
	char szDescription[MAX_PATH]; 
	WIN32_FIND_DATA wfd; 

	*lpszPath = 0; 

	CoInitialize( NULL );
	hres = CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl );
	if (SUCCEEDED(hres))
	{ 
		IPersistFile* ppf; 

		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
		if (SUCCEEDED(hres))
		{ 
			WORD wsz[MAX_PATH]; 

			MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, MAX_PATH); 

			hres = ppf->Load(wsz, STGM_READ); 
			if (SUCCEEDED(hres))
			{ 
				
				hres = psl->Resolve(hwnd, SLR_ANY_MATCH); 
				if (SUCCEEDED(hres))
				{ 
					
					hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA *)&wfd, SLGP_SHORTPATH ); 
					if (!SUCCEEDED(hres) )
					{
						
					}

					hres = psl->GetDescription(szDescription, MAX_PATH); 
					if (!SUCCEEDED(hres)) 
					{
						
					}
					lstrcpy(lpszPath, szGotPath); 
				} 
			} 
		
		ppf->Release(); 
		} 
	
	psl->Release(); 
	}
	else
	{
	}
	CoUninitialize();
	return hres; 
} 

inline int Translhextion::OnWndMsg( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	if( bInsertingHex )
	{
		switch( iMsg )
		{
			case WM_SIZE: case WM_KILLFOCUS: case WM_LBUTTONDOWN:
			case WM_LBUTTONUP: case WM_VSCROLL:
			case WM_HSCROLL: case WM_COMMAND: case WM_DROPFILES:
			case WM_CLOSE: case WM_DESTROY:
				bInsertingHex = FALSE;
				break;

			case WM_KEYDOWN:
				switch( wParam )
				{
				case VK_END:
				case VK_HOME:
				case VK_LEFT:
				case VK_RIGHT:
				case VK_UP:
				case VK_DOWN:
				case VK_PRIOR:
				case VK_NEXT:
					bInsertingHex = FALSE;
					break;

				default:
					break;
				}

			default:
				break;
		}
	}

	switch( iMsg )
	{
	case WM_CREATE:
		on_window_init( hwnd, hMainInstance );
		return 0;

	case WM_SIZE:
		resize_window( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_SETFOCUS:
		set_focus();
		return 0;

	case WM_KILLFOCUS:
		kill_focus();
		return 0;

	case WM_LBUTTONDOWN:
		lbuttondown( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_LBUTTONUP:
		lbuttonup( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_RBUTTONUP:
		rbuttonup( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_MOUSEMOVE:
		mousemove( (int) (short) LOWORD( lParam ), (int) (short) HIWORD( lParam ) );
		return 0;
	
	case WM_KEYDOWN:
		keydown( wParam );
		return 0;

	case WM_CHAR:
		character( wParam );
		return 0;

	case WM_VSCROLL:
		vertical_scroll( LOWORD( wParam ), HIWORD( wParam ) );
		return 0;

	case WM_HSCROLL:
		horizontal_scroll( LOWORD( wParam ), HIWORD( wParam ) );
		return 0;

	case WM_PAINT:
		paint();
		return 0;

	case WM_COMMAND:
		command( LOWORD( wParam ) );
		return 0;

	case WM_INITMENUPOPUP:
		initmenupopup( wParam, lParam );
		return 0;

	case WM_TIMER:
		timer( wParam, lParam );
		return 0;

	case WM_DROPFILES:
		dropfiles( (HANDLE) wParam ); 
		return 0;

	case WM_CLOSE:
		close();
		return 0;

	case WM_DESTROY:
		destroy_window();
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam);
}

int	Translhextion::translate_binary_to_text( string& dest, char* src, int len )
{
	int iDestinationLength = byte_to_bytecode_destinationlength( src, len );
	char * cBuf = new char[iDestinationLength];
	strToReplaceData = (string) cBuf;
	delete [] cBuf;
	if( (char*) strToReplaceData.c_str() != NULL )
	{
		translate_bytes_to_bytecode( (char*) strToReplaceData.c_str(), (unsigned char*) src, len );
		return TRUE;
	}
	else
		return FALSE;
}

int Translhextion::find_and_select_data( string& finddata, int finddir, int do_repaint, char (*cmp) (char) )
{
	char* tofind;
	int iDestinationLength = bytecode_translate( &tofind, 
		(char*) finddata.c_str(), 
		finddata.length(), 
		iCharacterSet, 
		iBinaryMode 
		);

	int i;
	if( finddir == 1 )
	{
		i = find_bytes( (char*) &(FileBuffer[iCurrentByte + 1]),
				FileBuffer.Length() - iCurrentByte - 1,
				tofind,	iDestinationLength, 1, cmp );
		if( i != -1 )
			iCurrentByte += i + 1;
	}
	else
	{
		i = find_bytes( (char*) &(FileBuffer[0]),
					min( iCurrentByte + (iDestinationLength - 1), FileBuffer.Length() ),
					tofind, iDestinationLength, -1, cmp );
		if( i != -1 )
			iCurrentByte = i;
	}

	if( i != -1 )
	{
		bSelected = TRUE;
		iStartOfSelection = iCurrentByte;
		iEndOfSelection = iCurrentByte + iDestinationLength - 1;
		if( do_repaint )
		{
			adjust_view_for_selection();
			repaint();
		}
	}
	else
	{
		if( tofind != NULL )
			delete [] tofind;
		return FALSE;
	}

	if( tofind != NULL )
		delete [] tofind;
	return TRUE;
}

int Translhextion::replace_selected_data( string& replacedata, int do_repaint )
{
	if( bSelected )
	{
		if( replacedata == "")
		{
			if( FileBuffer.Replace( iGetStartOfSelection(),
				iGetEndOfSelection() - iGetStartOfSelection() + 1,
				NULL, 0	) == TRUE )
			{
				long m = iGetStartOfSelection();
				for(m = m;m<iGetEndOfSelection();m++)
					vChanges.remove(m);
				bSelected = FALSE;
				bFilestatusChanged = TRUE;
				iFileChanged = TRUE;
				iCurrentByte = iStartOfSelection;
				if( do_repaint )
				{
					update_size_change();
					repaint();
				}
				return TRUE;
			}
			else
			{
				MessageBox( hwnd, "Selection could not be replaced.", "Replace error:", MB_OK | MB_ICONERROR );
				return FALSE;
			}
		}

		if( iPasteAsText )
		{
			int a = iGetStartOfSelection(), b = iGetEndOfSelection();
			if( FileBuffer.Replace( iGetStartOfSelection(),
				iGetEndOfSelection() - iGetStartOfSelection() + 1,
				(unsigned char*) (char*) replacedata.c_str(), replacedata.length()
				) == TRUE )
			{
				long m = iGetStartOfSelection();
				for(m = m;m<iGetEndOfSelection();m++)
					vChanges[m] = COLORCHANGE;
				iEndOfSelection = iStartOfSelection + replacedata.length() - 1;
				if( do_repaint )
				{
					update_size_change();
					repaint();
				}
				bFilestatusChanged = TRUE;
				iFileChanged = TRUE;
				return TRUE;
			}
			else
			{
				MessageBox( hwnd, "Replace failure.", "Replace error:", MB_OK | MB_ICONERROR );
				return FALSE;
			}
		}
		else
		{
			FileVector<char> out;
			if( translate_text_to_binary( replacedata, out ) == TRUE )
			{
				int a = iGetStartOfSelection(), b = iGetEndOfSelection();
				if( FileBuffer.Replace( iGetStartOfSelection(),
					iGetEndOfSelection() - iGetStartOfSelection() + 1,
					(unsigned char*) (char*) out, out.Length()
					) == TRUE )
				{
					long m = iGetStartOfSelection();
					for(m = m;m<iGetEndOfSelection();m++)
					vChanges[m] = COLORCHANGE;
					bFilestatusChanged = TRUE;
					iFileChanged = TRUE;
					iEndOfSelection = iStartOfSelection + out.Length() - 1;
					if( do_repaint )
					{
						update_size_change();
						repaint();
					}
					return TRUE;
				}
				else
				{
					MessageBox( hwnd, "Replace failure.", "Replace error:", MB_OK | MB_ICONERROR );
					return FALSE;
				}
			}
			else
			{
				MessageBox( hwnd, "Text could not be translated to binary.", "Replace error:", MB_OK | MB_ICONERROR );
				return FALSE;
			}
		}
	}
	else
	{
		MessageBox( hwnd, "Data to replace must be selected.", "Notice:", MB_OK | MB_ICONINFORMATION );
	}
	return FALSE;
}

int Translhextion::translate_text_to_binary( string& in, FileVector<char>& out )
{
	char* pcOut;
	int iDestinationLength = bytecode_translate( &pcOut,
		(char*) in.c_str(),
		in.length(),
		iCharacterSet,
		iBinaryMode
		);

	if( iDestinationLength != 0 )
	{
		out.Adopt( pcOut, iDestinationLength - 1, iDestinationLength );
		return TRUE;
	}
	else
	{
		

	}
	return FALSE;
}

static unsigned char input(const int& index)
{
	return cBuf[index];
}

unsigned char file(const int& index)
{
	unsigned char x;
	_lseek(FWFile, index, SEEK_SET);
	_read(FWFile,&x,1);
	return x;
}
char aa=0;
void strHex2Char()
{
	char* pcTemp;
	aa=0;
	int i,q=strlen(pcFWText);
	if(q%2)
	{
		
		for(i=0;i<q;i++)pcFWText[q+i]=pcFWText[i];
		pcFWText[q*2]=0;aa=1;
	}
	for(i=q=0;pcFWText[i]!='\0';i+=2)
	{
		pcTemp=&pcFWText[i];

		sscanf(pcTemp,"%2x",&cBuf[q]);

		q++;
	}
	buflen=q;
	cBuf[q]='\0';
}

void make_hex_only(HWND hEd)
{
	GetWindowText(hEd,pcFWText,FILL_WITH_MAX);
	int q=0;
	for(int i =0;pcFWText[i]!='\0';i++)
	{
		if(isxdigit(pcFWText[i]))
		{
			pcFWText[q]=pcFWText[i];
			q++;
		}
	}
	pcFWText[q]='\0';
	SetWindowText(hEd,pcFWText);
}

LRESULT CALLBACK HexProc(HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	char a;
	LONG i=0;
	if(iMsg==WM_CHAR)
	{
		a=(TCHAR) wParam;
		if(isxdigit(a)||(a==0x8))return CallWindowProc( (WNDPROC) oldproc, hEdit, iMsg, wParam, lParam);
		else 
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return 0L;
		}
	}
	else if (iMsg==WM_PASTE)
	{
		i= CallWindowProc((WNDPROC)oldproc, hEdit, iMsg, wParam, lParam);
		make_hex_only(hEdit);
	}
	else return CallWindowProc((WNDPROC)oldproc, hEdit, iMsg, wParam, lParam);
	return i;
}

void FillWithInit(HWND hDialog)
{
	if (TypeOfCurrent)
	{
		SetDlgItemText(hDialog,IDC_SI, "???");
		SetDlgItemText(hDialog,IDC_IFS, "???");
		SetDlgItemText(hDialog,IDC_R, "???");
	}
	else{
		char bufff[250];
		if(iOffsetMode == LOROM)
			sprintf(bufff,"%d=0x%X",buflen,toLOROM(buflen));
		else if(iOffsetMode == HIROM)
			sprintf(bufff,"%d=0x%X",buflen,toHIROM(buflen));
		else
			sprintf(bufff,"%d=0x%X",buflen,buflen);
		SetDlgItemText(hDialog,IDC_SI, bufff);
		if(buflen)
		{
			int temp=(1+abs(tmpstart-tmpend))/buflen;
			if(iOffsetMode == LOROM)
				sprintf(bufff,"%d=0x%X",temp,toLOROM(temp));
			else if(iOffsetMode == HIROM)
				sprintf(bufff,"%d=0x%X",temp,toHIROM(temp));
			else
				sprintf(bufff,"%d=0x%X",temp,temp);
			SetDlgItemText(hDialog,IDC_IFS, bufff);
			SendDlgItemMessage(hDialog,IDC_IFS,WM_SETFONT,(WPARAM) hfdef,MAKELPARAM(TRUE, 0));
			temp=(1+abs(tmpstart-tmpend))%buflen;
			if(iOffsetMode == LOROM)
				sprintf(bufff,"%d=0x%X",temp,toLOROM(temp));
			else if(iOffsetMode == HIROM)
				sprintf(bufff,"%d=0x%X",temp,toHIROM(temp));
			else
				sprintf(bufff,"%d=0x%X",temp,temp);
			SetDlgItemText(hDialog,IDC_R, bufff);
		}
		else
		{
			SetDlgItemText(hDialog,IDC_IFS, "\xa5");
			SendDlgItemMessage(hDialog,IDC_IFS,WM_SETFONT,(WPARAM) hfon,MAKELPARAM(TRUE, 0));
			SetDlgItemText(hDialog,IDC_R, "0=0x0");
		}
	}
}


void MessageCopyBox(HWND hWnd, LPTSTR lpText, LPCTSTR lpCaption, UINT uType, HWND hwnd)
{
	int len=strlen(lpText);
	
	strcat(lpText,"\nCopy this information to the clipboard?\n");
	if(IDYES==MessageBox (hWnd, lpText, lpCaption, MB_YESNO | uType))
	{
		
		lpText[len]=0;
		len++;
		HGLOBAL hGlobal = GlobalAlloc (GHND, len); 
		if (hGlobal != NULL)
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			char* pd = (char*) GlobalLock (hGlobal);
			if(pd)
			{
				strcpy(pd,lpText);
				GlobalUnlock (hGlobal);
				if(OpenClipboard(hwnd))
				{
					EmptyClipboard(); 
					SetClipboardData (CF_TEXT, hGlobal);
					CloseClipboard (); 
				}
				else 
					MessageBox (hWnd,"Clipboard failed to open correctly.","Clipboard error:",MB_OK | MB_ICONERROR);
			}
			else
			{
				GlobalFree(hGlobal);
				MessageBox (hWnd,"Clipboard did not lock properly.","Clipboard error:",MB_OK | MB_ICONERROR);
			}
			SetCursor (LoadCursor (NULL, IDC_ARROW));
		}
		else
			MessageBox (hWnd, "Not enough memory for copying to the clipboard.", "Clipboard error:", MB_OK | MB_ICONERROR);
	}
}

int Translhextion::find_byte_pos (char* src, char c)
{
	int i=0;
	while (src[i] != c)
	{
		i++;
	}
	return i;
}

int Translhextion::bytecode_translate (char* dest, char* src, int iLengthgthOfSource, int charmode, int binmode)
{
	int i, di=0, bclen;
	for (i=0; i<iLengthgthOfSource; i++)
	{
		if ((bclen = is_bytecode (&(src[i]), iLengthgthOfSource-i)) > 0) 
		{
			
			translate_bytecode (&(dest[di]), &(src[i]), iLengthgthOfSource-i, binmode);
			di += bclen;
			i += find_byte_pos (&(src[i]), '>');
		}
		else 
		{
			if (src[i] == '\\') 
			{
				if (i+1 < iLengthgthOfSource)
				{
					if (src[i+1] == '<')
					{
						dest[di++] = '<'; 
						i++;
					}
					else if( src[i+1] == '\\' )
					{
						dest[di++] = '\\'; 
						i++;
					}
					else
						dest[di++] = src[i]; 
				}
				else
					dest[di++] = src[i]; 
			}
			else
			{
				
				switch (charmode)
				{
				case ANSI_FIXED_FONT:
					dest[di++] = src[i];
					break;

				case OEM_FIXED_FONT:
					dest[di++] = TranslateAnsiToOem (src[i]);
					break;
				}
			}
		}
	}
	return di;
}

int Translhextion::translate_bytecode (char* dest, char* src, int iLengthgthOfSource, int binmode)
{
	int i, k=0;
	char cBuf[50];
	for (i=4; i<iLengthgthOfSource; i++)
	{
		if (src[i]=='>')
			break;
		else
		{
			cBuf[k++] = src[i];
		}
	}
	cBuf[k] = 0;
	int value;
	float fvalue;
	double dvalue;
	switch (src[2]) 
	{
	case 'd':
		sscanf (cBuf, "%d", &value);
		break;

	case 'h':
		sscanf (cBuf, "%X", &value);
		break;

	case 'l':
		sscanf (cBuf, "%f", &fvalue);
		break;

	case 'o':
		sscanf (cBuf, "%lf", &dvalue);
		break;
	}

	if (binmode == LITTLE_ENDIAN)
	{
		switch (src[1])
		{
		case 'b':
			dest[0] = (char) value;
			break;

		case 'w':
			dest[0] = (value & 0xff);
			dest[1] = (value & 0xff00)>>8;
			break;
		
		case 'l':
			dest[0] = (value & 0xff);
			dest[1] = (value & 0xff00)>>8;
			dest[2] = (value & 0xff0000)>>16;
			dest[3] = (value & 0xff000000)>>24;
			break;

		case 'f':
			*((float*)dest) = fvalue;
			break;

		case 'd':
			*((double*)dest) = dvalue;
			break;
		}
	}
	else 
	{
		switch (src[1])
		{
		case 'b':
			dest[0] = (char) value;
			break;

		case 'w':
			dest[0] = HIBYTE (LOWORD (value));
			dest[1] = LOBYTE (LOWORD (value));
			break;
		
		case 'l':
			dest[0] = HIBYTE (HIWORD (value));
			dest[1] = LOBYTE (HIWORD (value));
			dest[2] = HIBYTE (LOWORD (value));
			dest[3] = LOBYTE (LOWORD (value));
			break;

		case 'f':
			{
				char* p = (char*) &fvalue;
				int i;
				for (i=0; i<4; i++)
				{
					dest[i] = p[3-i];
				}
			}
			break;

		case 'd':
			{
				char* p = (char*) &dvalue;
				int i;
				for (i=0; i<8; i++)
				{
					dest[i] = p[7-i];
				}
			}
			break;
		}
	}
	return value;
}

int Translhextion::calc_bytecode_translation_destinationlength (char* src, int iLengthgthOfSource)
{
	int i, iDestinationLength = 0, l, k;
	for (i=0; i<iLengthgthOfSource; i++)
	{
		if ((l = is_bytecode (&(src[i]), iLengthgthOfSource-i)) == 0)
		{
			if (src[i] == '\\')
			{
				if (i+1 < iLengthgthOfSource)
				{
					if (src[i+1] == '<')
					{
						iDestinationLength++;
						i++;
					}
					else if( src[i+1] == '\\' )
					{
						iDestinationLength++;
						i++;
					}
					else
					{
						iDestinationLength++;
					}
				}
				else
				{
					iDestinationLength++;
				}
			}
			else
			{
				iDestinationLength++;
			}
		}
		else
		{
			iDestinationLength += l;
			for (k=i; i<iLengthgthOfSource; k++)
			{
				if (src[k]=='>')
					break;
			}
			i = k;
		}
	}
	return iDestinationLength;
}

int Translhextion::is_bytecode (char* src, int len)
{
	int i=0;

	if (src[i] == '<')
	{
		if (i+1 < len)
		{
			switch (src[i+1])
			{
			case 'b': case 'w': case 'l': case 'f': case 'd':
				if (i+2 < len)
				{
					switch (src[i+2])
					{
						case 'd': case 'h': case 'l': case 'o':
							if (i+3 < len)
							{
								if (src[i+3] == ':')
								{
									int j,k;
									for (j=4; j < len; j++)
									{
										if (src[i+j] == '>')
											break;
									}
									if (j==4 || j==len)
										return FALSE;
									for (k=4; k<j; k++)
									{
										switch (src[i+2])
										{
										case 'd':
											if ((src[i+k]>='0' && src[i+k]<='9') || src[i+k]=='-')
												continue;
											else
												return FALSE; 
											break;

										case 'h':
											if ((src[i+k]>='0' && src[i+k]<='9') ||
											 (src[i+k]>='a' && src[i+k]<='f'))
												continue;
											else
												return FALSE; 
											break;

										case 'o': case 'l': 
											if ((src[i+k]>='0' && src[i+k]<='9') || src[i+k]=='-' || src[i+k]=='.' || src[i+k]=='e' || src[i+k]=='E')
												continue;
											else
												return FALSE;
											break;
										}
									}
									switch (src[i+1])
									{
									default:
									case 'b': return 1;
									case 'w': return 2;
									case 'l': return 4;
									case 'f': return 4;
									case 'd': return 8;
									}
								}
								else
									return FALSE; 
							}
							else
								return FALSE; 
							break;
						
						default:
							return FALSE; 
					}
				}
				else
					return FALSE; 
				break;
			
			default:
				return FALSE; 
				break;
			}
		}
		else
			return FALSE; 
	}
	else
		return FALSE; 
}

int Translhextion::bytecode_translate (char** ppd, char* src, int iLengthgthOfSource, int charmode, int binmode)
{
	int iDestinationLength = calc_bytecode_translation_destinationlength (src, iLengthgthOfSource);
	if (iDestinationLength > 0)
	{
		*ppd = new char[iDestinationLength];
		bytecode_translate (*ppd, src, iLengthgthOfSource, charmode, binmode);
		return iDestinationLength;
	}
	else
	{
		*ppd = NULL;
		return 0;
	}
}


int Translhextion::translate_bytes_to_bytecode (char* pd, unsigned char* src, int iLengthgthOfSource)
{
	int i, k = 0;
	char cBuf[16];
	for (i=0; i<iLengthgthOfSource; i++)
	{
		if (src[i] == '<')
		{
			pd[k++] = '\\';
			pd[k++] = '<';
		}
		else if( src[i] == '\\' )
		{
			pd[k++] = '\\';
			pd[k++] = '\\';
		}
		else if (src[i] >= 32 && src[i] < 127)
		{
			pd[k++] = src[i];
		}
		else if( src[i]==10 || src[i]==13 )
		{
			pd[k++] = src[i];
		}
		else
		{
			pd[k++] = '<';
			pd[k++] = 'b';
			pd[k++] = 'h';
			pd[k++] = ':';
			sprintf (cBuf, "%2.2x", src[i]);
			pd[k++] = cBuf[0];
			pd[k++] = cBuf[1];
			pd[k++] = '>';
		}
	}
	pd[k] = '\0';
	return k+1;
}

int Translhextion::byte_to_bytecode_destinationlength (char* src, int iLengthgthOfSource)
{
	int i, iDestinationLength = 1;
	for (i=0; i<iLengthgthOfSource; i++)
	{
		if (src[i] == '<')
			iDestinationLength+=2; 
		else if( src[i] == '\\' )
			iDestinationLength+=2; 
		else if (src[i] >= 32 && src[i] < 127)
			iDestinationLength++; 
		else if( src[i]==10 || src[i]==13 )
			iDestinationLength++; 
		else
			iDestinationLength+=7; 
	}
	return iDestinationLength;
}

int Translhextion::command (int cmd)
{
	HMENU hMenu = GetMenu (hwnd);
	switch( cmd )
	{
	case IDA_BACKSPACE:
		on_backspace ();
		break;
	case IDA_DELETEKEY:
		on_deletekey ();
		break;

	case IDA_SELECTPAGEDOWN:
		select_with_arrowkeys (IDA_SELECTPAGEDOWN);
		break;

	case IDA_SELECTPAGEUP:
		select_with_arrowkeys (IDA_SELECTPAGEUP);
		break;

	case IDA_SELECTSTARTOFLINE:
		select_with_arrowkeys (IDA_SELECTSTARTOFLINE);
		break;

	case IDA_SELECTENDOFLINE:
		select_with_arrowkeys (IDA_SELECTENDOFLINE);
		break;

	case IDA_SELECTSTARTOFFILE:
		select_with_arrowkeys (IDA_SELECTSTARTOFFILE);
		break;

	case IDA_SELECTENDOFFILE:
		select_with_arrowkeys (IDA_SELECTENDOFFILE);
		break;

	case IDA_SELECTLEFT:
		select_with_arrowkeys (IDA_SELECTLEFT);
		break;

	case IDA_SELECTRIGHT:
		select_with_arrowkeys (IDA_SELECTRIGHT);
		break;

	case IDA_SELECTUP:
		select_with_arrowkeys (IDA_SELECTUP);
		break;

	case IDA_SELECTDOWN:
		select_with_arrowkeys (IDA_SELECTDOWN);
		break;
	case IDM_IME:
		{
			if(bTableLoaded && bTableActive && !bReadOnly)
			{
				while(!bIMEDone)
				{
					bInsertMode = (bool) iInsertMode;
					DialogBox(hInstance, MAKEINTRESOURCE (IDD_IMEBAR),hwnd,IMEBarDialogProc);
					if(vIMEBytes.size() != 0 && (iInsertMode || vIMEBytes.size() + iCurrentByte > FileBuffer.Length()))
					{
						string bs;
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						int k = 0;
						for(k = 0;k<vIMEBytes.size();k++)
						{
							bs += char_hex(vIMEBytes[k]);
						}
						char* pcPastestring;
						int iDestinationLength = bs.length();
						pcPastestring = new char[ iDestinationLength ];
						memcpy( pcPastestring, bs.c_str(), iDestinationLength );
						if(!FileBuffer.InsertExpandOn(iCurrentByte,(unsigned char*)pcPastestring,0,iDestinationLength))
						{
							delete [] pcPastestring;
							SetCursor (LoadCursor (NULL, IDC_ARROW));
							MessageBox (hwnd, "Not enough memory for insertion.", "Paste error:", MB_OK | MB_ICONERROR);
							update_size_change ();
							return FALSE;
						}
						long m = iCurrentByte;
						for(m = m;m<iCurrentByte+iDestinationLength;m++)
							vChanges.insert(COLORCHANGE,m);
						delete [] pcPastestring;
						iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
					}
					else if (vIMEBytes.size() != 0)
					{
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						int k = iCurrentByte;
						for(k = k;k<iCurrentByte + vIMEBytes.size();k++)
						{
							FileBuffer[k] = char_hex(vIMEBytes[k-iCurrentByte]);
							vChanges[k] = COLORCHANGE;
						}
						iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
					}
					else
					{
						bIMEDone = true;
					}
					iCurrentByte = iCurrentByte + vIMEBytes.size();
					vIMEBytes = vEmpty;
					SetCursor (LoadCursor (NULL, IDC_ARROW));
					adjust_view_for_selection ();
					mainwindow.repaint();
					
				
				}
				bIMEDone = false;
				
			}
			break;
		}
	case IDM_INTERNALSTATUS:
		{
			char cBuf[4000], buf2[300];
			sprintf (cBuf, "");
			sprintf (buf2, "# of bytes in current file: %d\n", FileBuffer.GetSize ());
			strcat (cBuf, buf2);
			sprintf (buf2, "# of lines in current file: %d\n", iNumlines);
			strcat (cBuf, buf2);
			sprintf (buf2, "Current Byte #: %d\n\n", iCurrentByte);
			strcat (cBuf, buf2);

			if(bTableLoaded)
				strcat( cBuf, "Table Loaded: YES\n");
			else
				strcat( cBuf, "Table Loaded: NO\n");
			if(bTableActive)
				strcat( cBuf, "Table Active: YES\n\n");
			else
				strcat( cBuf, "Table Active: NO\n\n");
			sprintf(buf2, "Tokens in table: %d\n\n", iRealTable);
			strcat(cBuf,buf2);
			sprintf (buf2, "# of successful script dumps this run: %d\n", dumps);
			strcat (cBuf, buf2);
			sprintf (buf2, "# of successful script insertions this run: %d\n", inserts);
			strcat (cBuf, buf2);
			sprintf (buf2, "# of successful script optimizations this run: %d\n", optimized);
			strcat (cBuf, buf2);
	
			MessageBox (hwnd, cBuf, (sName + " status:").c_str(), MB_OK);
			break;
		}
	case IDM_CHANGE_MODE:
		character ('\t');
		break;
	//FILE	
	case IDM_EXIT:
		SendMessage (hwnd, WM_CLOSE, 0, 0);
		break;
	case IDM_NEW:
		_New ();
		break;
	case IDM_OPEN:
		open ();
		break;
	case IDM_SAVE:
		save ();
		break;
	case IDM_SAVE_AS:
		save_as ();
		break;
	case IDM_EDIT_READONLYMODE:
		if( bReadOnly == FALSE )
			bReadOnly = TRUE;
		else
			bReadOnly = FALSE;
		update_size_change();
		break;
	case IDM_PARTIAL_OPEN:
		open_partially ();
		break;
	case IDM_REVERT:
		revert();
		break;
	case IDM_DELETEFILE:
		deletefile();
		break;
	case IDM_PROPERTIES:
		properties ();
		break;
	case IDM_APPLYTEMPLATE:
		apply_template();
		break;
	case IDM_OPEN_TEXT:
		summon_text_edit();
		break;
	case IDM_MRU1: case IDM_MRU2: case IDM_MRU3: case IDM_MRU4: case IDM_MRU5:
		case IDM_MRU6: case IDM_MRU7: case IDM_MRU8: case IDM_MRU9:
		most_recent_selection (cmd);
		break;
	//SELECTION
	case IDM_EDIT_CUT:
		iCutMode = BST_CHECKED;
		edit_cut ();
		break;
	case IDM_EDIT_COPY:
		if(bThingyView)
		{
			thingy_copy();
		}
		else
		{
			edit_copy ();
		}
		break;
	case IDM_EDIT_PASTE:
		fast_paste ();
		break;
	case IDM_PASTE_WITH_DLG:
		edit_paste ();
		break;
	case IDM_SELECT_ALL:
		select_all ();
		break;
	case IDM_SELECT_BLOCK:
		select_block ();
		break;
	case IDM_FILL_WITH:
		fill_with();
		break;
	case IDM_COPY_HEXDUMP:
		copy_hexdump ();
		break;
	case IDM_SAVESELAS:
		save_selection_as();
		break;

	//OFFSET
	case IDM_GO_TO:
		_Goto();
		break;
	case IDM_READFLOAT:
		{
			char cBuf[500], buf2[50];
			cBuf[0] = 0;
			float floatval;
			if (FileBuffer.Length()-iCurrentByte >= 4)
			{
				if (iBinaryMode == LITTLE_ENDIAN)
				{
					floatval = *((float*)&(FileBuffer[iCurrentByte]));
				}
				else // BIG_ENDIAN
				{
					char* pf = (char*) &floatval;
					int i;
					for (i=0; i<4; i++)
						pf[i] = FileBuffer[iCurrentByte+3-i];
				}
				sprintf (cBuf, "float size value:\n%f\n", floatval);
			}
			else
				sprintf (cBuf, "Not enough space for float size value.\n");
			double dval;
			if (FileBuffer.Length()-iCurrentByte >= 8)
			{
				if (iBinaryMode == LITTLE_ENDIAN)
				{
					dval = *((double*)&(FileBuffer[iCurrentByte]));
				}
				else // BIG_ENDIAN
				{
					char* pd = (char*) &dval;
					int i;
					for (i=0; i<8; i++)
						pd[i] = FileBuffer[iCurrentByte+7-i];
				}
				sprintf (buf2, "\ndouble size value:\n%g\n", dval);
				strcat (cBuf, buf2);
			}
			else
			{
				sprintf (buf2, "\nNot enough space for double size value.\n");
				strcat (cBuf, buf2);
			}
			
			MessageCopyBox (NULL, cBuf, "Floating point values:", MB_ICONINFORMATION, hwnd);
			break;
		}
	case IDM_EDIT_MANIPULATEBITS:
		manipulate_bits ();
		break;
	case IDM_COMPARE:
		compare ();
		break;
	//INSERT
	case IDA_INSERTMODETOGGLE:
		toggle_insertmode ();
		break;
	case IDM_EDIT_ENTERDECIMALVALUE:
		edit_enterdecimalvalue ();
		break;
	case IDM_EDIT_APPEND:
		edit_append ();
		break;
	case IDM_INSERTFILE:
		insertfile();
		break;
	//SEARCH
	case IDM_FIND:
		find ();
		break;
	case IDM_TABLEFIND:
		find_with_table();
		break;

	case IDM_FINDNEXT:
		{
			if(bLastSearchType == 0)
				findnext();
			if(bLastSearchType == 1)
				findnext_relative();
		}
		break;
	case IDM_FINDPREV:
		{
			if(bLastSearchType == 0)
				findprev();
			if(bLastSearchType == 1)
				findprev_relative();
		}
		break;
	case IDM_REPLACE:
		replace();
		break;
	case IDM_TABLEREPLACE:
		replace_with_table();
		break;
	case IDM_RELATIVEFIND:
		search_relative ();
		break;
	case IDM_RELATIVESCAN:
		scan_relative();
		break;
	case IDM_RELATIVESCANVAL:
		value_scan_relative();
		break;
	//BOOKMARK
	case IDM_ADDBOOKMARK:
		add_bookmark ();
		break;
	case IDM_REMOVE_BKM:
		remove_bookmark ();
		break;
	case IDM_CLEARALL_BMK:
		clear_all_bookmark ();
		break;
	case IDM_BOOKMARK1: case IDM_BOOKMARK2: case IDM_BOOKMARK3: 
		case IDM_BOOKMARK4: case IDM_BOOKMARK5: case IDM_BOOKMARK6: 
		case IDM_BOOKMARK7: case IDM_BOOKMARK8: case IDM_BOOKMARK9: 
			goto_bookmark (cmd);
		break;
	//SCRIPT
	case IDM_OPEN_TABLE:
		open_table();
		break;
	case IDM_MRTABLE1: case IDM_MRTABLE2: case IDM_MRTABLE3: case IDM_MRTABLE4:
		case IDM_MRTABLE5:
			most_recent_table_selection (cmd);
		break;
		case IDM_TABLE_CONVERT:
			convert_table();
			break;
	case IDM_SCRIPT_DUMP:
		script_dump();
		break;
	
	case IDM_REPLACE_SCRIPT:
		replace_script();
		break;

	case IDM_OPTIMIZESCRIPT:
		optimize_script_file();
		break;
	case IDM_SCRIPTMARK1: case IDM_SCRIPTMARK1 + 1: case IDM_SCRIPTMARK1 + 2: 
		case IDM_SCRIPTMARK1 + 3: case IDM_SCRIPTMARK1 + 4: case IDM_SCRIPTMARK1 + 5: 
		case IDM_SCRIPTMARK1 + 6: case IDM_SCRIPTMARK1 + 7: case IDM_SCRIPTMARK1 + 8: 
			iDumpStart = vScriptDumps[cmd - IDM_SCRIPTMARK1].start;
			iDumpEnd = vScriptDumps[cmd - IDM_SCRIPTMARK1].end;
			script_dump();
		break;
	//OPTIONS
	case IDM_VIEW_SETTINGS:
		view_settings ();
		break;
	case IDM_COLOR_SETTINGS:
		color_settings(); 
		break;
	case IDM_BINARYMODE:
		binarymode ();
		break;
	case IDM_OFFSETMODE:
		offsetmode();
		break;
	//SCROLL
	case IDM_HIDE_HEX:
		if(bHideHex)
		{
			bHideHex = false;
			ShowCaret( hwnd );
		}
		else
		{
			bHideHex = true;
			HideCaret( hwnd );
		}
		mainwindow.repaint();
		mainwindow.update_size_change();
		break;

	case IDM_SCROLL_LEFT:
		if (iHorizontalScrollPos > 0)
		{
			iHorizontalScrollPos--;
			update_horizontal_scrollbar ();
			repaint ();
		}
		break;
	
	case IDM_SCROLL_RIGHT:
		if (iHorizontalScrollPos < iHorizontalScrollMax)
		{
			iHorizontalScrollPos++;
			update_horizontal_scrollbar ();
			repaint();
		}
		break;
	case IDM_SCROLL_UP:
		if (iCurrentLine > 0)
		{
			iCurrentLine--;
			update_vertical_scrollbar ();
			repaint ();
		}
		break;

	case IDM_SCROLL_DOWN:
		if (iCurrentLine < iNumlines-1)
		{
			iCurrentLine++;
			update_vertical_scrollbar ();
			repaint ();
		}
		break;
	case IDM_TOP:
		iCurrentByte = iCurrentLine = iCurrentNibble = 0;
		adjust_view_for_cursor ();
		update_vertical_scrollbar ();
		bSelected = FALSE;
		repaint();
		break;
	case IDM_BOTTOM:
	{
		iCurrentByte = FileBuffer.Upper()+1;
		iCurrentNibble = 1;
		iCurrentLine = (iCurrentByte / iBytesPerLine)-(cBufferY-1); 
		if (iCurrentLine < 0)
			iCurrentLine = 0;
		adjust_view_for_cursor ();
		update_vertical_scrollbar ();
		bSelected = FALSE;
		repaint();
		break;
	}
	//HELP
	case IDM_HELP_TOPICS:
		{
  			HINSTANCE hi = ShellExecute( hwnd, "open", (sWindowsPath + "\\HH.EXE").c_str(), (sAppPath + sName + ".chm").c_str(), NULL, SW_SHOWNORMAL );
			if( (int) hi <= 32 )
				MessageBox( hwnd, ("HH.EXE was not found in " + sWindowsPath + ".  HH.EXE is required for HTML help to function.").c_str(), "Help error:", MB_OK | MB_ICONERROR );
		}
		break;
	case IDM_ABOUT:
		DialogBox (hInstance, MAKEINTRESOURCE (IDD_ABOUTDIALOG), hwnd, (DLGPROC) AboutDialogProc);
		break;

	default:
		{
			char cBuf[500];
			sprintf (cBuf, "Unknown COMMAND_ID %d.", cmd);
			MessageBox (hwnd, cBuf, "Command error:", MB_OK);
		}
		break;
	}
	return 0;
}

int Translhextion::initmenupopup( WPARAM w, LPARAM l )
{
	//FILE
	if( l == 0 )
	{
		//NEW
		// "New file" is always possible.
		EnableMenuItem( (HMENU) w, IDM_NEW, MF_ENABLED );

		//OPEN
		// "Open" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_OPEN, MF_ENABLED );

		//OPEN PARTIALLY
		//Open Partially" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_PARTIAL_OPEN, MF_ENABLED );

		//REVERT
		//Revert is allowed when the file has been saved, changed and is not in partial open mode
		EnableMenuItem( (HMENU) w, IDM_REVERT, ((!bFileNeverSaved && iFileChanged && !bPartialOpen)?MF_ENABLED:MF_GRAYED));

		//SAVE
		// "Save" is only possible if the current file has a name.
		if( !bFileNeverSaved )
			EnableMenuItem( (HMENU) w, IDM_SAVE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_SAVE, MF_GRAYED );

		//SAVE AS
		// "Save as" is always possible.
		EnableMenuItem( (HMENU) w, IDM_SAVE_AS, MF_ENABLED );

		//READ-ONLY TOGGLE
		// "Read-only mode" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_EDIT_READONLYMODE, MF_ENABLED );
		if( bReadOnly )
			CheckMenuItem( (HMENU) w, IDM_EDIT_READONLYMODE, MF_CHECKED );
		else
			CheckMenuItem( (HMENU) w, IDM_EDIT_READONLYMODE, MF_UNCHECKED );
		
		//CURRENT FILE
		
			//DELETE FILE
			//Delete file is allowed when the file has been saved and read only mode is off
			EnableMenuItem( GetSubMenu((HMENU) w,CURFPOS), IDM_DELETEFILE, ((!bFileNeverSaved && !bReadOnly)?MF_ENABLED:MF_GRAYED) );

			//PROPERTIES
			// "File properties" is always allowed.
			EnableMenuItem( GetSubMenu((HMENU) w,CURFPOS), IDM_PROPERTIES, MF_ENABLED );

			//APPLY TEMPLATE
			// "Apply template" is allowed if the cursor is on a byte
			// and there is no selection going on.
			if( iCurrentByte <= FileBuffer.Upper() && !bSelected )
				EnableMenuItem( GetSubMenu((HMENU) w,CURFPOS), IDM_APPLYTEMPLATE, MF_ENABLED );
			else
				EnableMenuItem( GetSubMenu((HMENU) w,CURFPOS), IDM_APPLYTEMPLATE, MF_GRAYED );
			
			//TEXT EDIT
			// "Open in text editor" is allowed if file has been saved before.
			if( !bFileNeverSaved )
				EnableMenuItem( GetSubMenu((HMENU) w,CURFPOS), IDM_OPEN_TEXT, MF_ENABLED );
			else
				EnableMenuItem( GetSubMenu((HMENU) w,CURFPOS), IDM_OPEN_TEXT, MF_GRAYED );


		//RECENT FILES
			
			//RECENT FILES
			// Create the MRU list.
			make_most_recent_list( (HMENU) w );
		
		//EXIT
		// "Exit" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_EXIT, MF_ENABLED );

	
	}
	//SELECTION
	else if( l == 1 )
	{
		//CUT
		// "Cut" is allowed if there is a selection or the cursor is on a byte.
		// It is not allowed in read-only and partial-open mode.
		if( ( bSelected || iCurrentByte <= FileBuffer.Upper() ) && !bReadOnly && !bPartialOpen )
			EnableMenuItem( (HMENU) w, IDM_EDIT_CUT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_CUT, MF_GRAYED );

		//COPY
		// "Copy" is allowed if there is a selection or the cursor is on a byte.
		if( bSelected || iCurrentByte <= FileBuffer.Upper() )
			EnableMenuItem( (HMENU) w, IDM_EDIT_COPY, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_COPY, MF_GRAYED );

		//PASTE
		// "Paste" is allowed if the clipboard contains text,
		// there is no selection going on and read-only is disabled.
		if( !bReadOnly && !bSelected )
		{
			if( OpenClipboard( NULL ) )
			{
				HGLOBAL hClipMemory = GetClipboardData( CF_TEXT );
				if( hClipMemory != NULL )
					EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_ENABLED );
				else
					EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_GRAYED );
				CloseClipboard ();
			}
			else
			{
				// Clipboard could not be opened => can't paste.
				EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_GRAYED );
			}
		}
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_GRAYED );

		//PASTE PREVIEW
		// "Paste with dialogue" is allowed if read-only is disabled and.
		// there is no selection going on.
		if( !bReadOnly && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_PASTE_WITH_DLG, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_PASTE_WITH_DLG, MF_GRAYED );

		//DELETE KEY
		// "Delete" is allowed if there is a selection or the cursor is on a byte.
		// It is not allowed in read-only and partial-open mode.
		if( ( bSelected || iCurrentByte <= FileBuffer.Upper() ) && !bReadOnly && !bPartialOpen )
			EnableMenuItem( (HMENU) w, IDA_DELETEKEY, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDA_DELETEKEY, MF_GRAYED );

		//SELECT ALL
		// "Select All" is allowed if file is not empty.
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_SELECT_ALL, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_SELECT_ALL, MF_GRAYED );

		//SELECT BLOCK
		// "Select block" is allowed if file is not empty.
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_SELECT_BLOCK, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_SELECT_BLOCK, MF_GRAYED );

		//FILL SELECTION
		//"Fill with" is allowed if read-only is disabled, there is selection
		//going on and the file is not in partial-open mode
		EnableMenuItem( (HMENU) w, IDM_FILL_WITH, ((!bReadOnly && bSelected && !bPartialOpen)?MF_ENABLED:MF_GRAYED) );

		//HEXDUMP
		// Hexdump is only possible if the file isn't empty.
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_COPY_HEXDUMP, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_COPY_HEXDUMP, MF_GRAYED );

		//SAVE SELECTION
		//Save selection as is allowed when selection is going on
		EnableMenuItem( (HMENU) w, IDM_SAVESELAS, (bSelected?MF_ENABLED:MF_GRAYED) );
	}
	//OFFSET
	else if( l == 2 )
	{
		//JUMP TO
		// "Go to" is allowed if the file isn't empty.
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_GO_TO, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_GO_TO, MF_GRAYED );

		//GET FLOAT VALUE
		// "Get floating point value" is allowed if the cursor is on a byte
		// and there is no selection going on.
		if( iCurrentByte <= FileBuffer.Upper() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_READFLOAT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_READFLOAT, MF_GRAYED );

		//MANIPULATE BITS
		// "Manipulate bits" is allowed if the cursor is on a byte, read-only is disabled
		// and there is no selection going on.
		if( !bReadOnly && iCurrentByte <= FileBuffer.Upper() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_EDIT_MANIPULATEBITS, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_MANIPULATEBITS, MF_GRAYED );

		//COMPARE
		// "Compare from current offset" is allowed if the cursor is on a byte
		// and there is no selection going on.
		if( iCurrentByte <= FileBuffer.Upper() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_COMPARE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_COMPARE, MF_GRAYED );
	}
	//INSERT
	else if( l == 3 )
	{
		//CHAR/HEX ENTERING MODE TOGGLE
		// "Change editing mode" is allowed when not Read-only.
		if( !bReadOnly )
			EnableMenuItem( (HMENU) w, IDM_CHANGE_MODE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_CHANGE_MODE, MF_GRAYED );

		//INSERT/OVERWRITE MODE TOGGLE
		// "Toggle entering mode" is allowed if read-only is disabled.
		if( !bReadOnly )
			EnableMenuItem( (HMENU) w, IDA_INSERTMODETOGGLE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDA_INSERTMODETOGGLE, MF_GRAYED );

		//ENTER DECIMAL VALUE
		// "Enter decimal value" is allowed if read-only is disabled, the file is not empty,
		// the cursor is on a byte and there is no selection going on.
		if( !bReadOnly && FileBuffer.Length() > 0 && iCurrentByte <= FileBuffer.Upper() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_EDIT_ENTERDECIMALVALUE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_ENTERDECIMALVALUE, MF_GRAYED );

		//INSERT BYTES AT END
		// "Append" is allowed if read-only is disabled and file is not
		// in partial-open mode and there is no selection going on.
		if( !bReadOnly && !bPartialOpen && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_EDIT_APPEND, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_APPEND, MF_GRAYED );

		//INSERT FILE
		//Insert file is allowed when read only mode is off & no selection
		EnableMenuItem( (HMENU) w, IDM_INSERTFILE, ((!bReadOnly && !bSelected)?MF_ENABLED:MF_GRAYED));
	}
	//SEARCH
	else if( l == 4 )
	{
		//FIND
		// "Find" is allowed if the file is not empty.
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_FIND, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_FIND, MF_GRAYED );

		//FIND USING TABLE
		// "Find with table" is allowed if file is not empty & if table is loaded and active
		if( FileBuffer.Length() > 0 && bTableLoaded && bTableActive)
			EnableMenuItem( (HMENU) w, IDM_TABLEFIND, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_TABLEFIND, MF_GRAYED );

		//FIND NEXT
		// "Find next" is allowed if the file is not empty,
		// and there is a findstring OR there is a selection
		// (which will be searched for).
		if( FileBuffer.Length() > 0 && ( pcFindDialogBuffer != NULL || bSelected ) )
			EnableMenuItem( (HMENU) w, IDM_FINDNEXT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_FINDNEXT, MF_GRAYED );

		//FIND PREVIOUS
		// "Find previous" is allowed if the file is not empty,
		// and there is a findstring OR there is a selection
		// (which will be searched for).
		if( FileBuffer.Length() > 0 && ( pcFindDialogBuffer != NULL || bSelected ) )
			EnableMenuItem( (HMENU) w, IDM_FINDPREV, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_FINDPREV, MF_GRAYED );

		//REPLACE
		// "Replace" is allowed if the file is not empty and read-only is disabled.
		if( FileBuffer.Length() > 0 && !bReadOnly )
			EnableMenuItem( (HMENU) w, IDM_REPLACE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_REPLACE, MF_GRAYED );

		//REPLACE USING TABLE
		//"Replace with table" is allowed if the file is not empty and read-only is disabled
		// and table is loaded and active
		if( FileBuffer.Length() > 0 && !bReadOnly && bTableLoaded && bTableActive)
			EnableMenuItem( (HMENU) w, IDM_TABLEREPLACE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_TABLEREPLACE, MF_GRAYED );

		//SEARCH RELATIVE
		//"Search relative" is allowed if the file is not empty
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_RELATIVEFIND, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_RELATIVEFIND, MF_GRAYED );

		//SCAN RELATIVE
		//"Scan relative" is allowed if the file is not empty
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_RELATIVESCAN, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_RELATIVESCAN, MF_GRAYED );

		//VALUE SCAN RELATIVE
		//"Scan relative" is allowed if the file is not empty
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_RELATIVESCANVAL, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_RELATIVESCANVAL, MF_GRAYED );

	}
	//BOOKMARKS
	else if( l == 5 )
	{
		//ADD BOOKMARK
		// "Add bookmark" is allowed if the file is not
		// empty and there is no selection going on.
		if( !bSelected && FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_ADDBOOKMARK, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_ADDBOOKMARK, MF_GRAYED );

		//REMOVE BOOKMARK CLEAR BOOKMARK
		// "Remove bookmark" and "Clear all bookmarks" are allowed if there are bookmarks set.
		if( iBookmarkCount > 0 )
		{
			EnableMenuItem( (HMENU) w, IDM_REMOVE_BKM, MF_ENABLED );
			EnableMenuItem( (HMENU) w, IDM_CLEARALL_BMK, MF_ENABLED );
		}
		else
		{
			EnableMenuItem( (HMENU) w, IDM_REMOVE_BKM, MF_GRAYED );
			EnableMenuItem( (HMENU) w, IDM_CLEARALL_BMK, MF_GRAYED );
		}

		//BOOKMARKS
		// Create the bookmark list.
		make_bookmark_list( (HMENU) w );
	}
	//SCRIPT
	else if( l == 6)
	{
		//OPEN TABLE
		//"Open table" is always allowed
		EnableMenuItem( (HMENU) w, IDM_OPEN_TABLE, MF_ENABLED );

		//RECENT TABLES
		
			//RECENT TABLES
			make_most_recent_table_list( (HMENU) w );
		
		
		//CONVERT TABLE
		//"Convert table" is only allowed when a table is loaded and active
		if(bTableLoaded && bTableActive && !bReadOnly)
			EnableMenuItem( (HMENU) w, IDM_TABLE_CONVERT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_TABLE_CONVERT, MF_GRAYED );
			
		//DUMP SCRIPT
		//"Dump Script" is allowed when a table is loaded and active and file is not empty
		if(bTableLoaded && bTableActive && FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_SCRIPT_DUMP, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_SCRIPT_DUMP, MF_GRAYED );
		
		//REPLACE SCRIPT
		//"Replace Script" is allowed when table loaded and active and not read-only mode
		if(bTableLoaded && bTableActive && !bReadOnly)
			EnableMenuItem( (HMENU) w, IDM_REPLACE_SCRIPT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_REPLACE_SCRIPT, MF_GRAYED );

		//OPTIMIZE SCRIPT
		//"Optimize Script" is allowed when table loaded and active
		if(bTableLoaded && bTableActive)
			EnableMenuItem( (HMENU) w, IDM_OPTIMIZESCRIPT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_OPTIMIZESCRIPT, MF_GRAYED );

		//SCRIPTMARKS

			//SCRIPTMARKS
			make_scriptmark_list((HMENU) w);

		//FIND USING TABLE
		// "Find with table" is allowed if file is not empty & if table is loaded and active
		if( FileBuffer.Length() > 0 && bTableLoaded && bTableActive)
			EnableMenuItem( (HMENU) w, IDM_TABLEFIND, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_TABLEFIND, MF_GRAYED );
	
		//REPLACE USING TABLE
		//"Replace with table" is allowed if the file is not empty and read-only is disabled
		// and table is loaded and active
		if( FileBuffer.Length() > 0 && !bReadOnly && bTableLoaded && bTableActive)
			EnableMenuItem( (HMENU) w, IDM_TABLEREPLACE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_TABLEREPLACE, MF_GRAYED );
	}
	//OPTIONS
	else if( l == 7)
	{
		//PREFERENCES
		EnableMenuItem( (HMENU) w, IDM_VIEW_SETTINGS, MF_ENABLED );
		
		//COLOR SETTINGS
		EnableMenuItem( (HMENU) w, IDM_COLOR_SETTINGS, MF_ENABLED );
		
		//BINARY MODE
		EnableMenuItem( (HMENU) w, IDM_BINARYMODE, MF_ENABLED );
		
		EnableMenuItem( (HMENU) w, IDM_OFFSETMODE, MF_ENABLED );	
	}
	//SCROLL
	else if( l == 8)
	{
		//LEFT
		EnableMenuItem( (HMENU) w, IDM_SCROLL_LEFT, MF_ENABLED );
		
		//RIGHT
		EnableMenuItem( (HMENU) w, IDM_SCROLL_RIGHT, MF_ENABLED );
		
		//UP
		EnableMenuItem( (HMENU) w, IDM_SCROLL_UP, MF_ENABLED );
		
		//DOWN
		EnableMenuItem( (HMENU) w, IDM_SCROLL_DOWN, MF_ENABLED );
		
		//TOP
		EnableMenuItem( (HMENU) w, IDM_TOP, MF_ENABLED );
		
		//BOTTOM
		EnableMenuItem( (HMENU) w, IDM_BOTTOM, MF_ENABLED );

		//JUMP TO
		// "Go to" is allowed if the file isn't empty.
		if( FileBuffer.Length() > 0 )
			EnableMenuItem( (HMENU) w, IDM_GO_TO, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_GO_TO, MF_GRAYED );

	}
	//HELP
	else if( l == 9)
	{
		//HELP
		EnableMenuItem( (HMENU) w, IDM_HELP_TOPICS, MF_ENABLED );
		
		//ABOUT
		EnableMenuItem( (HMENU) w, IDM_ABOUT, MF_ENABLED );

	}
	return 0;
}

int Translhextion::update_most_recent ()
{
	int i;
	for (i=0; i<iMostRecentCount; i++)
	{
		if (strcmp (&(strMostRecent[i][0]), filename)==0)  
		{
			
			char temp[_MAX_PATH+1];
			strcpy (temp, &(strMostRecent[0][0])); 
			
			int j;
			for (j=i; j>0; j--)
				strcpy (&(strMostRecent[j][0]), &(strMostRecent[j-1][0]));
			strcpy (&(strMostRecent[0][0]), filename); 
			break;
		}
	}
	if (i==iMostRecentCount) 
	{
		if (iMostRecentCount<iMostRecentMaximum)
		{
			int j;
			iMostRecentCount++;
			for (j=iMostRecentCount-1; j>0; j--)
				strcpy (&(strMostRecent[j][0]), &(strMostRecent[j-1][0]));
			strcpy (&(strMostRecent[0][0]), filename);
		}
		else
		{
			int j;
			for (j=iMostRecentMaximum-1; j>0; j--)
				strcpy (&(strMostRecent[j][0]), &(strMostRecent[j-1][0]));
			strcpy (&(strMostRecent[0][0]), filename);
		}
	}
	save_ini_data ();
	return 1;
}

int Translhextion::make_most_recent_list (HMENU menu)
{
	int i;
	while (RemoveMenu (GetSubMenu(menu,iMostRecentPos), 0, MF_BYPOSITION)==TRUE)
		;
	
	if (iMostRecentCount>0)
	{
		
		char cBuf[_MAX_PATH+1+30];
		for (i=0; i<iMostRecentCount; i++)
		{
			sprintf (cBuf, "&%d %s", i+1, &(strMostRecent[i][0]));
			AppendMenu (GetSubMenu(menu,iMostRecentPos), MF_ENABLED | MF_STRING, IDM_MRU1+i, cBuf);
		}
	}
	return 1;
}

int Translhextion::most_recent_selection (int cmd)
{
	if (cmd-IDM_MRU1+1>iMostRecentCount)
		return 0;

	if (file_can_load (&(strMostRecent[cmd-IDM_MRU1][0])))
	{
		if (iFileChanged == TRUE)
		{
			if( MessageBox (hwnd, "You have not saved your changes! Discard them and open anyway?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO )
				return 0;
		}
		if (load_file (&(strMostRecent[cmd-IDM_MRU1][0])))
		{
			iVerticalScrollMax = 0;
			iVerticalScrollPos = 0;
			iVerticalScrollInc = 0;
			iHorizontalScrollMax = 0;
			iHorizontalScrollPos = 0;
			iHorizontalScrollInc = 0;
			iCurrentLine = 0;
			iCurrentByte = 0;
			iCurrentNibble = 0;
			iFileSize = FileBuffer.Length();
			iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
			bFileNeverSaved = FALSE;
			bSelected = FALSE;
			update_most_recent ();
			update_size_change ();
		}
	}
	else
	{
		MessageBox (hwnd, "This file could not be accessed and\n"
			"will be removed from the list.", "Notice:", MB_OK | MB_ICONINFORMATION);
		int j;
		for (j=cmd-IDM_MRU1; j<iMostRecentCount-1; j++)
			strcpy (&(strMostRecent[j][0]), &(strMostRecent[j+1][0]));
		iMostRecentCount--;
		save_ini_data ();
	}
	return 1;
}

int Translhextion::update_most_recent_table()
{
	int i;
	for (i=0; i<iMostRecentTableCount; i++)
	{
		if (strcmp (&(strMostRecentTable[i][0]), filename2)==0)  
		{
			char temp[_MAX_PATH+1];
			strcpy (temp, &(strMostRecentTable[0][0])); 
			
			int j;
			for (j=i; j>0; j--)
				strcpy (&(strMostRecentTable[j][0]), &(strMostRecentTable[j-1][0]));
			strcpy (&(strMostRecentTable[0][0]), filename2); 
			break;
		}
	}
	if (i==iMostRecentTableCount) 
	{
		if (iMostRecentTableCount<iMostRecentTableMaximum)
		{
			int j;
			iMostRecentTableCount++;
			for (j=iMostRecentTableCount-1; j>0; j--)
				strcpy (&(strMostRecentTable[j][0]), &(strMostRecentTable[j-1][0]));
			strcpy (&(strMostRecentTable[0][0]), filename2);
		}
		else
		{
			int j;
			for (j=iMostRecentTableMaximum-1; j>0; j--)
				strcpy (&(strMostRecentTable[j][0]), &(strMostRecentTable[j-1][0]));
			strcpy (&(strMostRecentTable[0][0]), filename2);
		}
	}
	save_ini_data ();
	return 1;
}

int Translhextion::make_most_recent_table_list (HMENU menu)
{
	int i;
	while (RemoveMenu (GetSubMenu(menu,iMostRecentTablePos), 0, MF_BYPOSITION)==TRUE)
		;
	
	if (iMostRecentTableCount>0)
	{
		//AppendMenu (menu, MF_SEPARATOR, 0, 0);
		char cBuf[_MAX_PATH+1+30];
		for (i=0; i<iMostRecentTableCount; i++)
		{
			sprintf (cBuf, "&%d %s", i+1, &(strMostRecentTable[i][0]));
			AppendMenu (GetSubMenu(menu,iMostRecentTablePos), MF_ENABLED | MF_STRING, IDM_MRTABLE1+i, cBuf);
		}
	}
	return 1;
}

int Translhextion::most_recent_table_selection (int cmd)
{
	if (cmd-IDM_MRTABLE1+1>iMostRecentTableCount)
		return 0;

	if (file_can_load (&(strMostRecentTable[cmd-IDM_MRTABLE1][0])))
	{
		if (bTableLoaded)
		{
			if (MessageBox (hwnd, "A table file is already loaded! Discard loaded table and open this one?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO)
				return 0;
		}
		fstream fs;
		strcpy(filename2,&(strMostRecentTable[cmd-IDM_MRTABLE1][0]));
		fs.open(filename2,ios::in);
		if(!fs.fail())
		{
			wread_table_file(fs);
			fs.close();
			
			if(iRealTable > 0)
			{
				if(bTableLoaded)
				{
					bTableActive = false;
					bThingyView = false;
					CheckDlgButton(hwndTBar,IDC_CHECK1,BST_UNCHECKED);
					CheckDlgButton(hwndTBar,IDC_CHECK2,BST_UNCHECKED);
					if(bJPuncSwitch)
					{
						CheckDlgButton(hwndTBar,IDC_CHECK4, BST_UNCHECKED);
					}
					if(bDumpClean)
					{
						CheckDlgButton(hwndTBar,IDC_CHECK3, BST_CHECKED);
					}
					bTableLoaded = true;
					bTableActive = true;
					sLoadedTable = filename2;
				}
				else
				{
					bTableLoaded = true;
					bTableActive = true;
					hwndTBar = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_TOOLBAR),hwnd,ToolBarDialogProc);
					sLoadedTable = filename2;
				}
				//when using optimizations under >= vc 6.0 then crash issues start here
				// possibly memory is corrupted at some point before this?
				// or maybe STL cannot handle optimizations?
				MessageBox(hwnd,(stringvalue(iRealTable) + " Tokens have been loaded from the table.").c_str(),"Notice:", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				if(bTableLoaded)
				{
					DestroyWindow(hwndTBar);
					bTableLoaded = false;
					bTableActive = false;
					bThingyView = false;
					vTableFileLeft = vEmpty;
					vTableFileRight = vEmpty;
					sLoadedTable = "";
				}
				MessageBox(hwnd,"This file does not seem to be a table.  Investigate the problem or load another table.","Table file error:", MB_OK | MB_ICONERROR);
			}
		}
		else
		{
			fs.close();
			MessageBox(hwnd,"Error occurred while reading from table file.","Table file error:",MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		MessageBox (hwnd, "This file could not be accessed and\n"
			"will be removed from the list.", "Notice:", MB_OK | MB_ICONINFORMATION);
		int j;
		for (j=cmd-IDM_MRTABLE1; j<iMostRecentTableCount-1; j++)
			strcpy (&(strMostRecentTable[j][0]), &(strMostRecentTable[j+1][0]));
		iMostRecentTableCount--;
		save_ini_data ();
	}
	return 1;
}

int Translhextion::make_scriptmark_list (HMENU menu)
{
	int i;
	int sm1place = IDM_SCRIPTMARK1;
	while (RemoveMenu (GetSubMenu(menu,SMPOS), 0, MF_BYPOSITION)==TRUE)
		;

	if (vScriptDumps.size()>0)
	{
		string temp;
		int j = 0;
		for (i=0; i<vScriptDumps.size(); i++)
		{
			if (vScriptDumps[i].name == "")
			{
				temp = "[" + stringvalue(vScriptDumps[i].start) + "-" + stringvalue(vScriptDumps[i].end) + "]";
			}
			else
			{
				temp = vScriptDumps[i].name + "[" + stringvalue(vScriptDumps[i].start) + "-" + stringvalue(vScriptDumps[i].end) + "]";
			}
			if (bTableLoaded && bTableActive && vScriptDumps[i].end <= FileBuffer.Length())
				AppendMenu (GetSubMenu(menu,SMPOS), MF_ENABLED | MF_STRING, IDM_SCRIPTMARK1+i, temp.c_str());
			else
				AppendMenu (GetSubMenu(menu,SMPOS), MF_GRAYED | MF_STRING, IDM_SCRIPTMARK1+i, temp.c_str());
		}
	}
	return 1;
}

int Translhextion::make_bookmark_list (HMENU menu)
{
	int i;
	while (RemoveMenu (menu, iBookmarkPos, MF_BYPOSITION)==TRUE)
		;

	if (iBookmarkCount>0)
	{
		AppendMenu (menu, MF_SEPARATOR, 0, 0);
		char cBuf[128];
		long bookmark = 0;
		for (i=0; i<iBookmarkCount; i++)
		{
			bookmark = pbmkList[i].offset;
			if (pbmkList[i].name == NULL)
				sprintf (cBuf, "&%d 0x%X", i+1, bookmark);
			else
				sprintf (cBuf, "&%d 0x%X:%s", i+1, bookmark, pbmkList[i].name);
			if(iOffsetMode == LOROM)
				bookmark = fromLOROM(bookmark);
			else if(iOffsetMode == HIROM)
				bookmark = fromHIROM(bookmark);
			if (bookmark <= FileBuffer.Length() && !bSelected)
				AppendMenu (menu, MF_ENABLED | MF_STRING, IDM_BOOKMARK1+i, cBuf);
			else
				AppendMenu (menu, MF_GRAYED | MF_STRING, IDM_BOOKMARK1+i, cBuf);
		}
	}
	return 1;
}

//FILE
int Translhextion::_New ()
{
	if (iFileChanged == TRUE)
	{
		if( MessageBox (hwnd, "You have not saved your changes! Discard them and create new file anyway?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO )
			return 0;
	}
	bFileNeverSaved = TRUE;
	bSelected = FALSE;
	bLButtonIsDown = FALSE;
	iFileChanged = FALSE;
	bFilestatusChanged = TRUE;
	iVerticalScrollMax = 0;
	iVerticalScrollPos = 0;
	iVerticalScrollInc = 0;
	iHorizontalScrollMax = 0;
	iHorizontalScrollPos = 0;
	iHorizontalScrollInc = 0;
	iCurrentLine = 0;
	iCurrentByte = 0;
	iCurrentNibble = 0;
	iFileSize = 0;
	bPartialOpen=FALSE;
	FileBuffer.Reset ();
	vChanges = vEmpty;
	vHighlights = vEmpty;
	vDTEs = vEmpty;
	sprintf (filename, "Untitled");
	update_size_change ();
	repaint ();
	return 1;
}

int Translhextion::close ()
{
	if( iFileChanged == TRUE )
	{
		if( MessageBox (hwnd, "You have not saved your changes! Discard them and exit anyway?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO )
			return 0;
	}
	else
	{
		char path[500];
		strcpy (path, _pgmptr);
		int len = strlen (path);
		path[len-3] = 'h';
		path[len-2] = 'l';
		path[len-1] = 'p';
		WinHelp (hwnd, path, HELP_QUIT, 0);
	}

	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof( WINDOWPLACEMENT );
	GetWindowPlacement( hwnd, &wndpl );
	iWindowShowCmd = wndpl.showCmd;
	iWindowX = wndpl.rcNormalPosition.left;
	iWindowY = wndpl.rcNormalPosition.top;
	iWindowWidth = wndpl.rcNormalPosition.right - iWindowX;
	iWindowHeight = wndpl.rcNormalPosition.bottom - iWindowY;
	save_ini_data();

	if(bTableLoaded)
	{
		DestroyWindow(hwndTBar);
	}
		DestroyWindow( hwnd );
	return 0;
}


int Translhextion::open ()
{
	if (iFileChanged == TRUE)
	{
		if( MessageBox (hwnd, "You have not saved your changes! Discard them and open anyway?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO )
			return 0;
	}
	char cFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	cFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		if (load_file (cFileName))
		{
			iVerticalScrollMax = 0;
			iVerticalScrollPos = 0;
			iVerticalScrollInc = 0;
			iHorizontalScrollMax = 0;
			iHorizontalScrollPos = 0;
			iHorizontalScrollInc = 0;
			iCurrentLine = 0;
			iCurrentByte = 0;
			iCurrentNibble = 0;
			bSelected = FALSE;
			iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
			bFileNeverSaved = FALSE;
			iFileSize = FileBuffer.Length();
			RECT r;
			GetClientRect (hwnd, &r);
			SendMessage (hwnd, WM_SIZE, 0, (r.bottom << 16) | r.right);
			InvalidateRect (hwnd, NULL, FALSE);
			UpdateWindow (hwnd);
		}					
	}
	return 1;
}


int Translhextion::open_partially ()
{
	if (iFileChanged == TRUE)
	{
		if( MessageBox (hwnd, "You have not saved your changes! Discard them and open partially anyway?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO )
			return 0;
	}
	char cFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	cFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		int iFileHandle, iFileLength;
		if ((iFileHandle = _open (cFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			iFileLength = _filelength (iFileHandle);
			_close (iFileHandle);
		}
		iStartPL = 0;
		iNumBytesPl = iFileLength;
		iPLFileLength = iFileLength;
		if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_OPEN_PARTIAL_DIALOG), hwnd, (DLGPROC) OpenPartiallyDialogProc))
		{
			if (iStartPL+iNumBytesPl<=iFileLength)
			{
				if ((iFileHandle = _open (cFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
				{
					FileBuffer.Reset ();
					vChanges = vEmpty;
					vHighlights = vEmpty;
					vDTEs = vEmpty;
					if (FileBuffer.SetLength (iNumBytesPl) == TRUE)
					{
						vector<string> temp(iNumBytesPl);
						vChanges = temp;
						vHighlights = temp;
						vDTEs = temp;
						//int m = 0;
						//for(m = 0;m<vChanges.size();m++)
						//	vChanges[m] = COLORNORMAL;
						FileBuffer.SetUpperBound (iNumBytesPl-1);
						_lseek (iFileHandle, iStartPL, 0);
						iPartialOffset = iStartPL;
						if (_read (iFileHandle, FileBuffer, iNumBytesPl) != -1)
						{
							_close (iFileHandle);
							iFileSize = FileBuffer.Length();
							if( bOpenReadOnly )
								bReadOnly = TRUE;
							else
								bReadOnly = FALSE;
							strcpy (filename, cFileName);
							bFileNeverSaved = FALSE;
							iVerticalScrollMax = 0;
							iVerticalScrollPos = 0;
							iVerticalScrollInc = 0;
							iHorizontalScrollMax = 0;
							iHorizontalScrollPos = 0;
							iHorizontalScrollInc = 0;
							iCurrentLine = 0;
							iCurrentByte = 0;
							iCurrentNibble = 0;
							iFileChanged = FALSE;
							bFilestatusChanged = TRUE;
							bFileNeverSaved = FALSE;
							bPartialOpen=TRUE;
							RECT r;
							GetClientRect (hwnd, &r);
							SendMessage (hwnd, WM_SIZE, 0, (r.bottom << 16) | r.right);
							InvalidateRect (hwnd, NULL, FALSE);
							UpdateWindow (hwnd);
							return TRUE;
						}
						else
						{
							_close (iFileHandle);
							MessageBox (hwnd, "Error occurred while reading from file.", "Partial open error:", MB_OK | MB_ICONERROR);
							return FALSE;
						}
					}
					else
					{
						MessageBox (hwnd, "Not enough memory to load file.", "Partial open error:", MB_OK | MB_ICONERROR);
						return FALSE;
					}
				}
				else
				{
					char cBuf[500];
					sprintf (cBuf, "Error code 0x%X occured while opening file %s.", errno, cFileName);
					MessageBox (hwnd, cBuf, "Partial open error:", MB_OK | MB_ICONERROR);
					return FALSE;
				}
			}
			else
			{
				MessageBox (hwnd, "More bytes were specified than there are in the file.", "Partial open error:", MB_OK | MB_ICONERROR);
				return 0;
			}
		}
	}
	return 1;
}

BOOL CALLBACK OpenPartiallyDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[128];
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iStartPL));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iStartPL));
			else
				sprintf (cBuf, "x%X", iStartPL);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			sprintf (cBuf, "Size of file: %d. Load how many bytes:", iPLFileLength);
			SetWindowText (GetDlgItem (hDialog, IDC_STATIC2), cBuf);
			sprintf (cBuf, "%d", iNumBytesPl);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			SetFocus(hDialog);
			CheckDlgButton( hDialog, IDC_RADIO1, BST_CHECKED );
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[128];
				int i=0;

				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 128) != 0)
				{
					if (sscanf (cBuf, "x%X", &iStartPL) == 0)
					{
						if (sscanf (cBuf, "%d", &iStartPL) == 0)
						{
							i = -1;
						}
					}
				}
				if(iOffsetMode == LOROM)
					iStartPL = fromLOROM(iStartPL);
				else if(iOffsetMode == HIROM)
					iStartPL = fromHIROM(iStartPL);
				
				if( i==-1 && IsDlgButtonChecked( hDialog, IDC_RADIO1 ) == BST_CHECKED )
				{
					MessageBox (hDialog, "Starting offset is not recognized.", "Partial open error:", MB_OK | MB_ICONERROR);
					EndDialog (hDialog, 0);
					return 0;
				}
				
				if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 128) != 0)
				{
					if (sscanf (cBuf, "%d", &iNumBytesPl) == 0)
					{
						i = -1;
					}
				}
				
				if (i==-1)
				{
					MessageBox (hDialog, "Number of bytes is not recognized.", "Partial open error:", MB_OK | MB_ICONERROR);
					EndDialog (hDialog, 0);
					return 0;
				}

				if( IsDlgButtonChecked( hDialog, IDC_RADIO2 ) == BST_CHECKED )
				{
					
					iStartPL = iPLFileLength - iNumBytesPl;
					if( iStartPL < 0 )
					{
						MessageBox (hDialog, "Specified number of bytes to load is greater than file size.", "Partial open error:", MB_OK | MB_ICONERROR);
						EndDialog (hDialog, 0);
						return 0;
					}
				}

				EndDialog (hDialog, 1);
				return TRUE;
			}

		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Translhextion::revert()
{
	if (load_file (filename))
	{
		iVerticalScrollMax = iVerticalScrollPos = iVerticalScrollInc = iHorizontalScrollMax = iHorizontalScrollPos = 
		iHorizontalScrollInc = iCurrentLine = iCurrentByte = iCurrentNibble = 0;
		iFileSize = FileBuffer.Length();
		iFileChanged = FALSE;
		bFilestatusChanged = TRUE;
		bFileNeverSaved = FALSE;
		bSelected=FALSE;
		RECT r;
		GetClientRect (hwnd, &r);
		SendMessage (hwnd, WM_SIZE, 0, (r.bottom << 16) | r.right);
		InvalidateRect (hwnd, NULL, FALSE);
		UpdateWindow (hwnd);
	}					
}

int Translhextion::save ()
{
	int choice = IDYES;
	if (bPartialOpen)
	{
		if(iFileSize != FileBuffer.Length())
		{
			long diff = FileBuffer.Length() - iFileSize;
			string change;
			if(diff > 0)
			{
				change = "+" + stringvalue(diff);
			}
			else
			{
				change = stringvalue(diff);
			}
			choice = MessageBox(hwnd,("The file size has changed by " + change + " byte(s).\r\nDo you still want to save?").c_str(),"Notice:", MB_YESNO | MB_ICONINFORMATION);
		}
		if(choice == IDYES)
		{
			int iFileHandle;
			if ((iFileHandle = _open (filename,_O_RDWR|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
			{
				WaitCursor w1;
				if( _lseek( iFileHandle, iPartialOffset, 0 ) == -1 )
				{
					MessageBox( hwnd, "Could not seek to proper location in file.", "Partial save error:", MB_OK | MB_ICONERROR );
					_close( iFileHandle );
					return 0;
				}
				if( _write( iFileHandle, FileBuffer, FileBuffer.Length() ) == -1 )
				{
					MessageBox( hwnd, "Could not write data to file.", "Partial save error:", MB_OK | MB_ICONERROR );
				}
				_close (iFileHandle);
				iFileChanged = FALSE;
				bFilestatusChanged = TRUE;
				if(bDiscardChangesSave)
				{
					vector<string> empty(vHighlights.size());
					vChanges = empty;
					iFileSize = FileBuffer.Length();
					//long r = 0;
					//for(r = 0;r<vChanges.size();r++)
					//{
					//	if(vChanges[r] == COLORCHANGE)
					//	{
					//		vChanges[r] = COLORNORMAL;
					//	}
					//}
				}
			}
			else
			{
				MessageBox (hwnd, "Could not save partially opened file.", "Partial save error:", MB_OK | MB_ICONERROR );
			}
			repaint ();
		}
		return 1;
	}
	if( bFileNeverSaved )
	{
		MessageBox (hwnd, "File is untitled!\nPlease choose \"Save As...\" from menu", "Notice:", MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	if(iFileSize != FileBuffer.Length())
	{
		long diff = FileBuffer.Length() - iFileSize;
		string change;
		if(diff > 0)
		{
			change = "+" + stringvalue(diff);
		}
		else
		{
			change = stringvalue(diff);
		}
		choice = MessageBox(hwnd,("The file size has changed by " + change + " byte(s).\r\nDo you still want to save?").c_str(),"Notice:", MB_YESNO | MB_ICONINFORMATION);
	}
	if(choice == IDYES)
	{
		int iFileHandle;
		if ((iFileHandle = _open (filename,_O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			_write (iFileHandle, FileBuffer, FileBuffer.Length ());
			_close (iFileHandle);
			if(bDiscardChangesSave)
			{
				vector<string> empty(vHighlights.size());
				vChanges = empty;
				iFileSize = FileBuffer.Length();
				//long r = 0;
				//for(r = 0;r<vChanges.size();r++)
				//{
				//	if(vChanges[r] == COLORCHANGE)
				//	{
				//		vChanges[r] = COLORNORMAL;
				//	}
				//}
			}
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
			bPartialOpen=FALSE;
		}
		else
		{
			MessageBox (hwnd, "Could not save file.", "Save error:", MB_OK | MB_ICONERROR);
		}
		repaint ();
	}
	return 1;
}

int Translhextion::save_as ()
{
	char cFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	strcpy (cFileName, filename);
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetSaveFileName (&ofn))
	{
		int iFileHandle;
		if ((iFileHandle = _open (cFileName, _O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			if ((_write (iFileHandle, FileBuffer, FileBuffer.Length ())) != -1)
			{
				strcpy (filename, cFileName);
				iFileChanged = FALSE;
				bFilestatusChanged = TRUE;
				bFileNeverSaved = FALSE;
				bPartialOpen=FALSE;
				if(bDiscardChangesSave)
				{
					vector<string> empty(vHighlights.size());
					vChanges = empty;
					//long r = 0;
					//for(r = 0;r<vChanges.size();r++)
					//{
					//	if(vChanges[r] == COLORCHANGE)
					//	{
					//		vChanges[r] = COLORNORMAL;
					//	}
					//}
				}
				update_most_recent();
			}
			else
				MessageBox (hwnd, "Could not save file.", "Save as error:", MB_OK | MB_ICONERROR);
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			_close (iFileHandle);
		}
		else
			MessageBox (hwnd, "Could not save file.", "Save as error:", MB_OK | MB_ICONERROR);
	}
	repaint ();
	return 1;
}

void Translhextion::deletefile()
{
	if(IDYES==MessageBox(hwnd,"Are you sure you want to delete this file?","Notice:",MB_ICONQUESTION |MB_YESNO))
	{
		if(IDYES==MessageBox(hwnd,"Are you sure that you are sure you want to delete this file?","Notice:",MB_ICONQUESTION|MB_YESNO))
		{
			if(DeleteFile(filename))
			{
				iFileChanged = FALSE;
				_New();
			}
			else MessageBox (hwnd, "Could not delete file.", "Delete file error:", MB_OK | MB_ICONERROR);
		}
	}
}

int Translhextion::properties ()
{
	char cBuf[1000], buf2[500], *pc;
	sprintf (cBuf, "File name and path: ");
	GetFullPathName (filename, 500, buf2, &pc);
	strcat (cBuf, buf2);
	if( bPartialOpen )
	{
		if(iOffsetMode == LOROM)
			sprintf (buf2, "\nPartially opened at offset 0x%X = %d.\n"
			"Number of bytes read: %d = %d kilobytes.\n",
			iPartialOffset, toLOROM(iPartialOffset), FileBuffer.Length(), FileBuffer.Length()/1024);
		else if(iOffsetMode == HIROM)
			sprintf (buf2, "\nPartially opened at offset 0x%X = %d.\n"
			"Number of bytes read: %d = %d kilobytes.\n",
			iPartialOffset, toHIROM(iPartialOffset), FileBuffer.Length(), FileBuffer.Length()/1024);
		else
			sprintf (buf2, "\nPartially opened at offset 0x%X = %d.\n"
			"Number of bytes read: %d = %d kilobytes.\n",
			iPartialOffset, iPartialOffset, FileBuffer.Length(), FileBuffer.Length()/1024);
		strcat (cBuf, buf2);
	}
	else
	{
		sprintf (buf2, "\nFile size: %d bytes = %d kilobytes.\n", FileBuffer.Length(), FileBuffer.Length()/1024);
		strcat (cBuf, buf2);
	}
	sprintf (buf2, "\nNumber of hexdump lines: %d.\n", iNumlines);
	strcat (cBuf, buf2);
	MessageCopyBox (NULL, cBuf, "File properties", MB_ICONINFORMATION, hwnd);
	return 1;
}

int Translhextion::apply_template()
{
	if( FileBuffer.Length() == 0 )
	{
		MessageBox( hwnd, "File is empty.", "Template error:", MB_OK | MB_ICONERROR );
		return FALSE;
	}

	char szTemplateName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	szTemplateName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "Template Files (*.TPL)\0*.TPL\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szTemplateName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Select Template File";
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName( &ofn ) )
	{
		apply_template( szTemplateName );
	}
	return TRUE;
}

int Translhextion::apply_template( char* pcTemplate )
{
	int iFileHandle;
	if( ( iFileHandle = _open( pcTemplate, _O_RDONLY|_O_BINARY, _S_IREAD|_S_IWRITE ) ) != -1 )
	{
		int tpl_filelen = _filelength( iFileHandle );
		if( tpl_filelen > 0 )
		{
			char* pcTpl = new char[ tpl_filelen ];
			if( pcTpl != NULL && _read( iFileHandle, pcTpl, tpl_filelen ) != -1 )
			{
				FileVector<char> TplResult;
				TplResult.SetLength( 1, 100 );
				TplResult.AppendArray( "File: ", 6 );
				TplResult.AppendArray( filename, strlen( filename ) );
				TplResult.AppendArray( "\xd\xa", 2 );
				TplResult.AppendArray( "Template file: ", 15 );
				TplResult.AppendArray( pcTemplate, strlen( pcTemplate ) );
				TplResult.AppendArray( "\xd\xa", 2 );
				TplResult.AppendArray( "Applied at offset: ", 19 );
				char cBuf[16];
				sprintf( cBuf, "%d\xd\xa\xd\xa", iCurrentByte );
				TplResult.AppendArray( cBuf, strlen( cBuf ) );
				apply_template_on_memory( pcTpl, tpl_filelen, TplResult );
				TplResult.Append( '\0' );
				pcTmplText = TplResult;
				DialogBox( hInstance, MAKEINTRESOURCE( IDD_TMPL_RESULT_DIALOG ), hwnd, (DLGPROC) TmplDisplayDialogProc );
			}
			else
				MessageBox( hwnd, "Template file could not be loaded.", "Template load error:", MB_OK | MB_ICONERROR );
			if( pcTpl != NULL )
				delete [] pcTpl;
		}
		else
			MessageBox( hwnd, "Template file is empty.", "Template error:", MB_OK | MB_ICONERROR );
		_close( iFileHandle );
	}
	else
	{
		char cBuf[500];
		sprintf( cBuf, "Error code 0x%X occured while\nopening template file %s.", errno, pcTemplate );
		MessageBox( hwnd, cBuf, "Template error:", MB_OK | MB_ICONERROR );
		return FALSE;
	}
	return TRUE;
}

int Translhextion::apply_template_on_memory( char* pcTpl, int tpl_len, FileVector<char>& ResultArray )
{
	int index = 0, fpos = iCurrentByte;
	while( index < tpl_len )
	{
		if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
		{
			char cmd[ TPL_TYPE_MAXLEN ]; 
			if( read_tpl_token( pcTpl, tpl_len, index, cmd ) == TRUE )
			{
				if( strcmp( cmd, "BYTE" ) == 0 || strcmp( cmd, "char" ) == 0 )
				{
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						if( FileBuffer.Length() - fpos >= 1 )
						{
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							ResultArray.AppendArray( name, strlen(name) );
							
							char cBuf[ TPL_NAME_MAXLEN + 200];
							if( FileBuffer[fpos] != 0 )
								sprintf( cBuf, " = %d (signed) = %u (unsigned) = 0x%X = \'%c\'\xd\xa", (int) (signed char) FileBuffer[fpos], FileBuffer[fpos], FileBuffer[fpos], FileBuffer[fpos] );
							else
								sprintf( cBuf, " = %d (signed) = %u (unsigned) = 0x%X\xd\xa", (int) (signed char) FileBuffer[fpos], FileBuffer[fpos], FileBuffer[fpos] );
							ResultArray.AppendArray( cBuf, strlen(cBuf) );
							
							fpos += 1;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for byte-size datum.", 45 );
							return FALSE;
						}
					}
					else
					{
						
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE;
					}
				}
				else if( strcmp( cmd, "WORD" ) == 0 || strcmp( cmd, "short" ) == 0 )
				{
					
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						
						if( FileBuffer.Length() - fpos >= 2 )
						{
							
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							
							ResultArray.AppendArray( name, strlen(name) );
							WORD wd;
							if( iBinaryMode == LITTLE_ENDIAN )
							{
								wd = *( (WORD*)( &FileBuffer[ fpos ] ) );
							}
							else 
							{
								int i;
								for( i=0; i<2; i++ )
									((char*)&wd)[ i ] = FileBuffer[ fpos + 1 - i ];
							}
							char cBuf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( cBuf, " = %d (signed) = %u (unsigned) = 0x%X\xd\xa", (int) (signed short) wd, wd, wd );
							ResultArray.AppendArray( cBuf, strlen(cBuf) );
							fpos += 2;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for WORD.", 34 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; 
					}
				}
				else if( strcmp( cmd, "DWORD" ) == 0 || strcmp( cmd, "int" ) == 0 ||
					strcmp( cmd, "long" ) == 0 || strcmp( cmd, "LONG" ) == 0 )
				{
					
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						
						if( FileBuffer.Length() - fpos >= 4 )
						{
							
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							
							ResultArray.AppendArray( name, strlen(name) );
							DWORD dw;
							
							if( iBinaryMode == LITTLE_ENDIAN )
							{
								dw = *( (DWORD*)( &FileBuffer[ fpos ] ) );
							}
							else 
							{
								int i;
								for( i=0; i<4; i++ )
									((char*)&dw)[ i ] = FileBuffer[ fpos + 3 - i ];
							}
							char cBuf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( cBuf, " = %d (signed) = %u (unsigned) = 0x%X\xd\xa", (signed long) dw, (unsigned long) dw, dw );
							ResultArray.AppendArray( cBuf, strlen(cBuf) );
							fpos += 4;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for DWORD.", 34 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; 
					}
				}
				else if( strcmp( cmd, "float" ) == 0 )
				{
					
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						
						if( FileBuffer.Length() - fpos >= 4 )
						{
							
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							
							ResultArray.AppendArray( name, strlen(name) );
							float f;
							
							if( iBinaryMode == LITTLE_ENDIAN )
							{
								f = *( (float*)( &FileBuffer[ fpos ] ) );
							}
							else 
							{
								int i;
								for( i=0; i<4; i++ )
									((char*)&f)[ i ] = FileBuffer[ fpos + 3 - i ];
							}
							char cBuf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( cBuf, " = %f = 0x%X\xd\xa", f, (unsigned long) *((int*) &f) );
							ResultArray.AppendArray( cBuf, strlen(cBuf) );
							fpos += 4;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for float.", 34 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; 
					}
				}
				else if( strcmp( cmd, "double" ) == 0 )
				{
					
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						
						if( FileBuffer.Length() - fpos >= 8 )
						{
							
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							
							ResultArray.AppendArray( name, strlen(name) );
							double d;
							
							if( iBinaryMode == LITTLE_ENDIAN )
							{
								d = *( (double*)( &FileBuffer[ fpos ] ) );
							}
							else 
							{
								int i;
								for( i=0; i<8; i++ )
									((char*)&d)[ i ] = FileBuffer[ fpos + 7 - i ];
							}
							char cBuf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( cBuf, " = %g\xd\xa", d );
							ResultArray.AppendArray( cBuf, strlen(cBuf) );
							fpos += 8;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for double.", 35 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; 
					}
				}
				else
				{
					ResultArray.AppendArray( "ERROR: Unknown variable type \"", 30 );
					ResultArray.AppendArray( cmd, strlen( cmd ) );
					ResultArray.Append( '\"' );
					return FALSE;
				}
			}
			else
			{
				
				ResultArray.AppendArray( "ERROR: Missing variable name.", 29 );
				return FALSE;
			}
		}
		else
		{
			
			break;
		}
	}
	
	char cBuf[128];
	sprintf( cBuf, "\xd\xa-> Lengthgth of template = %d bytes.\xd\xa", fpos - iCurrentByte );
	ResultArray.AppendArray( cBuf, strlen( cBuf ) );
	return TRUE;
}

int Translhextion::ignore_non_code( char* pcTpl, int tpl_len, int& index )
{
	while( index < tpl_len )
	{
		
		switch( pcTpl[ index ] )
		{
		case ' ': case '\t': case 0x0d: case 0x0a:
			break;

		default:
			return TRUE;
		}
		index++;
	}
	return FALSE;
}

int Translhextion::read_tpl_token( char* pcTpl, int tpl_len, int& index, char* dest )
{
	int i = 0;
	while( index + i < tpl_len )
	{
		switch( pcTpl[ index + i ] )
		{
		case ' ': case '\t': case 0x0d: case 0x0a:
			dest[i] = '\0';
			index += i;
			return TRUE;

		default:
			dest[i] = pcTpl[ index + i ];
		}
		i++;
	}
	dest[i] = '\0';
	index += i;
	return FALSE;
}

BOOL CALLBACK TmplDisplayDialogProc( HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			SetWindowText( GetDlgItem( hDialog, IDC_EDIT1 ), pcTmplText );
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::summon_text_edit()
{
	if( filename != NULL )
	{
		HINSTANCE hi = ShellExecute( hwnd, "open", sTexteditorName.c_str(), filename, NULL, SW_SHOWNORMAL );
		if( (int) hi <= 32 )
			MessageBox( hwnd, "Error occured when opening text editor.", "Text editing error:", MB_OK | MB_ICONERROR );
	}
	else
	{
		MessageBox( hwnd, "Filename is NULL.", "Text editing error:", MB_OK | MB_ICONERROR );
	}
	return 0;
}

//SELECTION

int Translhextion::edit_cut ()
{
	if (bSelected) 
	{
		if (iEndOfSelection >= iStartOfSelection)
		{
			iCutOffset = iStartOfSelection;
			iCutNumberOfBytes = iEndOfSelection-iStartOfSelection+1;
		}
		else
		{
			iCutOffset = iEndOfSelection;
			iCutNumberOfBytes = iStartOfSelection-iEndOfSelection+1;
		}
	}
	else 
	{
		iCutOffset = iCurrentByte;
		iCutNumberOfBytes = 1;
	}
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_CUTDIALOG), hwnd, (DLGPROC) CutDialogProc))
	{
		if( FileBuffer.Length() - iCutOffset >= iCutNumberOfBytes )
		{
			int newlen = FileBuffer.Length () - iCutNumberOfBytes;
			
			switch (iCutMode)
			{
			case BST_CHECKED:
				{
					
					int iDestinationLength = byte_to_bytecode_destinationlength ((char*) &(FileBuffer[iCutOffset]), iCutNumberOfBytes);
					HGLOBAL hGlobal = GlobalAlloc (GHND, iDestinationLength);
					if (hGlobal != NULL)
					{
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						char* pd = (char*) GlobalLock (hGlobal);
						translate_bytes_to_bytecode (pd, &(FileBuffer[iCutOffset]), iCutNumberOfBytes);
						GlobalUnlock (hGlobal);
						OpenClipboard (hwnd);
						EmptyClipboard ();
						SetClipboardData (CF_TEXT, hGlobal);
						CloseClipboard ();
						SetCursor (LoadCursor (NULL, IDC_ARROW));
					}
					else
					{
						
						MessageBox (hwnd, "Not enough memory for cutting to clipboard.", "Cut error:", MB_OK | MB_ICONERROR);
						return 0;
					}
					break;
				}
				
			default:
				break;
			}
			
			if (FileBuffer.RemoveAt (iCutOffset, iCutNumberOfBytes) == FALSE)
			{
				long m = iCutOffset;
				for(m = m;m<iCutOffset + iCutNumberOfBytes;m++)
					vChanges.remove(m);
				MessageBox (hwnd, "Could not cut the data.", "Cut error:", MB_OK | MB_ICONERROR);
				return FALSE;
			}
			iCurrentByte = iCutOffset;
			if (iCurrentByte > (FileBuffer.Upper ()))
				iCurrentByte = (FileBuffer.Upper ());
			if (iCurrentByte<0)
				iCurrentByte=0;
			iFileChanged = TRUE;
			bFilestatusChanged = TRUE;
			bSelected = FALSE;
			update_size_change ();
			repaint ();
		}
		else
		{
			
			MessageBox (hwnd, "Cannot cut more bytes than are in the file.", "Cut error:", MB_OK | MB_ICONERROR);
		}
	}
	return 0;
}

BOOL CALLBACK CutDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[32];
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCutOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCutOffset));
			else
				sprintf (cBuf, "x%X", iCutOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCutOffset+iCutNumberOfBytes-1));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCutOffset+iCutNumberOfBytes-1));
			else
				sprintf (cBuf, "x%X", iCutOffset+iCutNumberOfBytes-1);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			sprintf (cBuf, "%d", iCutNumberOfBytes);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT3), cBuf);
			SetFocus(hDialog);
			CheckDlgButton (hDialog, IDC_CHECK1, iCutMode);
			CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[64];
				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 64) != 0)
				{
					int i;
					if (sscanf (cBuf, "x%X", &i) == 0)
					{
						if (sscanf (cBuf, "%d", &i) == 0)
						{
							MessageBox (hDialog, "Starting offset is not recognized.", "Cut error:", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
					if (i==-1)
					{
						EndDialog (hDialog, 0);
						return 0;
					}
					if(iOffsetMode == LOROM)
						iCutOffset = fromLOROM(i);
					else if(iOffsetMode == HIROM)
						iCutOffset = fromHIROM(i);
					else
						iCutOffset = i;
				}

				switch (IsDlgButtonChecked (hDialog, IDC_RADIO1))
				{
				case BST_CHECKED: 
					{
						if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 64) != 0)
						{
							int i;
							if (sscanf (cBuf, "x%X", &i) == 0)
							{
								if (sscanf (cBuf, "%d", &i) == 0)
								{
									MessageBox (hDialog, "Ending offset is not recognized.", "Cut error:", MB_OK | MB_ICONERROR);
									i = -1;
								}
							}
							if (i==-1)
							{
								EndDialog (hDialog, 0);
								return 0;
							}
							if(iOffsetMode == LOROM)
								i = fromLOROM(i);
							else if(iOffsetMode == HIROM)
								i = fromHIROM(i);
							iCutNumberOfBytes = i-iCutOffset+1;
						}
					}
					break;

				default: 
					{
						if (GetDlgItemText (hDialog, IDC_EDIT3, cBuf, 64) != 0)
						{
							int i;
							if (sscanf (cBuf, "%d", &i) == 0)
							{
								MessageBox (hDialog, "Number of bytes is not recognized.", "Cut error:", MB_OK | MB_ICONERROR);
								i = -1;
							}
							if (i==-1)
							{
								EndDialog (hDialog, 0);
								return 0;
							}
							iCutNumberOfBytes = i;
						}
					}
				}

				iCutMode = IsDlgButtonChecked (hDialog, IDC_CHECK1);
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::edit_copy ()
{
	if (!bSelected)
	{
		
		iCopyStartOffset = iCurrentByte;
		iCopyEndOffset = iCurrentByte;
	}
	else
	{
		
		if (iEndOfSelection >= iStartOfSelection)
		{
			iCopyStartOffset = iStartOfSelection;
			iCopyEndOffset = iEndOfSelection;
		}
		else
		{
			iCopyStartOffset = iEndOfSelection;
			iCopyEndOffset = iStartOfSelection;
		}
	}

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_COPYDIALOG), hwnd, (DLGPROC) CopyDialogProc) != FALSE)
	{
		
		iStartOfSelection = iCopyStartOffset;
		iEndOfSelection = iCopyEndOffset;
		if (iEndOfSelection >= iStartOfSelection)
		{
			int iDestinationLength = byte_to_bytecode_destinationlength ((char*) &(FileBuffer[iStartOfSelection]), iEndOfSelection-iStartOfSelection+1);
			HGLOBAL hGlobal = GlobalAlloc (GHND, iDestinationLength);
			if (hGlobal != NULL)
			{
				SetCursor (LoadCursor (NULL, IDC_WAIT));
				char* pd = (char*) GlobalLock (hGlobal);
				translate_bytes_to_bytecode (pd, &(FileBuffer[iStartOfSelection]), iEndOfSelection-iStartOfSelection+1);
				GlobalUnlock (hGlobal);
				OpenClipboard (hwnd);
				EmptyClipboard ();
				SetClipboardData (CF_TEXT, hGlobal);
				CloseClipboard ();
				SetCursor (LoadCursor (NULL, IDC_ARROW));
			}
			else
				MessageBox (hwnd, "Not enough memory for copying.", "Copy error:", MB_OK | MB_ICONERROR);
		}
		else
		{
			MessageBox (hwnd, "Ending offset is smaller than starting offset.", "Copy error:", MB_OK | MB_ICONERROR);
		}
	}
	adjust_view_for_selection ();
	return 1;
}

int Translhextion::thingy_copy ()
{
	if (!bSelected)
	{
		
		iCopyStartOffset = iCurrentByte;
		iCopyEndOffset = iCurrentByte;
	}
	else
	{
		
		if (iEndOfSelection >= iStartOfSelection)
		{
			iCopyStartOffset = iStartOfSelection;
			iCopyEndOffset = iEndOfSelection;
		}
		else
		{
			iCopyStartOffset = iEndOfSelection;
			iCopyEndOffset = iStartOfSelection;
		}
	}

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_COPYDIALOG), hwnd, (DLGPROC) CopyDialogProc) != FALSE)
	{
		iStartOfSelection = iCopyStartOffset;
		iEndOfSelection = iCopyEndOffset;
		if (iEndOfSelection >= iStartOfSelection)
		{
			int i = 0;
			iDumpStart = iStartOfSelection;
			iDumpEnd = iEndOfSelection;
			vector<string> vDumpSize((iDumpEnd-iDumpStart)+1);
			vHexdoc = vDumpSize;
			vDumpSize = vEmpty;
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			HWND progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_PROGRESSDIALOG),hwnd,ProgressDialogProc);
			SetFocus(progressdialog);
			SetWindowText(progressdialog,"Script Dump Preparation...");
			SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Queuing bytes...");
			progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
			SetProgressRange(progress1,0,vHexdoc.size());
			if(vHexdoc.size() >= 10000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 1000;
			}
			else if(vHexdoc.size() >= 1000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 100;
			}
			else if(vHexdoc.size() >= 100)
			{
				iIncrementStep = vHexdoc.size() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress1,iIncrementStep);  
			for(i = iDumpStart;i<=iDumpEnd;i++)
			{
				iProgressPos++;
				if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
				{
					StepProgress(progress1);	
				}
				vHexdoc[i - iDumpStart] = hex_char(FileBuffer[i]);
			}
			DestroyWindow(progressdialog);
			iProgressPos = 0;
			iIncrementStep = 0;
			progress1 = NULL;
			long filesize = vHexdoc.size();
			bCleanScript = true;
			progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_2PROGRESSDIALOG),hwnd,ProgressDialogProc);
			SetWindowText(progressdialog,"Script Dump Progress...");
			SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Translating...");
			progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
			progress2 = GetDlgItem(progressdialog,IDC_PROGRESS2);
			SetProgressRange(progress1,0,vHexdoc.size());
			if(vHexdoc.size() >= 10000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 1000;
			}
			else if(vHexdoc.size() >= 1000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 100;
			}
			else if(vHexdoc.size() >= 100)
			{
				iIncrementStep = vHexdoc.size() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress1,iIncrementStep);  
			long numbytes = vHexdoc.size();
			translate(vHexdoc);
			SetProgressPos(progress1,numbytes);
			iProgressPos = 0;
			SetProgressRange(progress2,0,vHexdoc.size());
			if(vHexdoc.size() >= 10000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 1000;
			}
			else if(vHexdoc.size() >= 1000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 100;
			}
			else if(vHexdoc.size() >= 100)
			{
				iIncrementStep = vHexdoc.size() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress2,iIncrementStep);  
			SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS2TITLE),"Formatting...");
			enscript(vHexdoc);
			progress1 = NULL;
			progress2 = NULL;
			iIncrementStep = 0;
			iProgressPos = 0;
			DestroyWindow(progressdialog);
			bCleanScript = false;
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			string clipdata = "";
			clipdata = "";
			int m = 0;
			for(m = 0;m<vHexdoc.size();m++)
			{
				clipdata += vHexdoc[m];
			}
			HGLOBAL hGlobal = GlobalAlloc (GHND, clipdata.length());
			if (hGlobal != NULL)
			{
				SetCursor (LoadCursor (NULL, IDC_WAIT));
				char* pd = (char*) GlobalLock (hGlobal);
				strcpy(pd,clipdata.c_str());
				GlobalUnlock (hGlobal);
				OpenClipboard (hwnd);
				EmptyClipboard ();
				SetClipboardData (CF_TEXT, hGlobal);
				CloseClipboard ();
				SetCursor (LoadCursor (NULL, IDC_ARROW));
			}
			else
				MessageBox (hwnd, "Not enough memory for copying.", "Copy error:", MB_OK | MB_ICONERROR);
		}
		else
		{
			MessageBox (hwnd, "Ending offset is smaller than starting offset.", "Copy error:", MB_OK | MB_ICONERROR);
		}
	}
	adjust_view_for_selection ();
	return 1;
}

BOOL CALLBACK CopyDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[32];
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCopyStartOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCopyStartOffset));
			else
				sprintf (cBuf, "x%X", iCopyStartOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCopyEndOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCopyEndOffset));
			else
				sprintf (cBuf, "x%X", iCopyEndOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			sprintf (cBuf, "%d", iCopyEndOffset-iCopyStartOffset+1);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT3), cBuf);
			CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[64];
				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 64) != 0)
				{
					int i;
					if (sscanf (cBuf, "x%X", &i) == 0)
					{
						if (sscanf (cBuf, "%d", &i) == 0)
						{
							MessageBox (hDialog, "Starting offset is not recognized.", "Copy error:", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
					if (i==-1)
					{
						EndDialog (hDialog, 0);
						return 0;
					}
					if(iOffsetMode == LOROM)
						iCopyStartOffset = fromLOROM(i);
					else if(iOffsetMode == HIROM)
						iCopyStartOffset = fromHIROM(i);
					else
						iCopyStartOffset = i;
				}

				switch (IsDlgButtonChecked (hDialog, IDC_RADIO1))
				{
				case BST_CHECKED: 
					{
						if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 64) != 0)
						{
							int i;
							if (sscanf (cBuf, "x%X", &i) == 0)
							{
								if (sscanf (cBuf, "%d", &i) == 0)
								{
									MessageBox (hDialog, "Ending offset is not recognized.", "Copy error:", MB_OK | MB_ICONERROR);
									i = -1;
								}
							}
							if (i==-1)
							{
								EndDialog (hDialog, 0);
								return 0;
							}
							if(iOffsetMode == LOROM)
								iCopyEndOffset = fromLOROM(i);
							else if(iOffsetMode == HIROM)
								iCopyEndOffset = fromHIROM(i);
							else
								iCopyEndOffset = i;
						}
						break;
					}

				default: 
					{
						if (GetDlgItemText (hDialog, IDC_EDIT3, cBuf, 64) != 0)
						{
							int i;
							if (sscanf (cBuf, "%d", &i) == 0)
							{
								MessageBox (hDialog, "Number of bytes is not recognized.", "Copy error:", MB_OK | MB_ICONERROR);
								i = -1;
							}
							if (i==-1)
							{
								EndDialog (hDialog, 0);
								return 0;
							}
							iCopyEndOffset = iCopyStartOffset + i;
						}
						break;
					}
				}

				EndDialog (hDialog, 1);
				return TRUE;
			}

		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::copy_hexdump ()
{
	if (FileBuffer.Length() <= 0)
	{
		MessageBox (hwnd, "Cannot hexdump an empty file.", "Hexdump error:", MB_OK | MB_ICONERROR);
		return 0;
	}
	if( !bSelected )
	{
		
		iCopyHexdumpDialogStart = 0;
		iCopyHexdumpDialogEnd = ((FileBuffer.Upper())/iBytesPerLine)*iBytesPerLine;
	}
	else
	{
		if( iEndOfSelection < iStartOfSelection )
		{
			int i = iEndOfSelection;
			iEndOfSelection = iStartOfSelection;
			iStartOfSelection = i;
		}
		iCopyHexdumpDialogStart = ( iStartOfSelection / iBytesPerLine ) * iBytesPerLine;
		iCopyHexdumpDialogEnd = ( iEndOfSelection / iBytesPerLine ) * iBytesPerLine;
	}

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_HEXDUMPDIALOG), hwnd, (DLGPROC) CopyHexdumpDialogProc) != FALSE)
	{
		
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		if (iCopyHexdumpDialogStart % iBytesPerLine != 0 || iCopyHexdumpDialogEnd % iBytesPerLine != 0)
		{
			MessageBox (hwnd, "Offsets must be multiples of\nthe number of bytes per iLine.", "Hexdump error:", MB_OK | MB_ICONERROR);
			return 0;
		}
		
		int linecount = (iCopyHexdumpDialogEnd - iCopyHexdumpDialogStart) / iBytesPerLine + 1;
		
		
		int buflen = linecount * (iCharsPerLine+2) + 1;
		
		int a,b,k,m,n,j,l;
		char buf1[128], buf2[128];
		char* pMem = new char[buflen];
		for (n=0; n < buflen; n++)
			pMem[n] = ' ';
		
		b = iCopyHexdumpDialogEnd;
				
		for (k = 0, a = iCopyHexdumpDialogStart; a <= b; a += iBytesPerLine, k += iCharsPerLine + 2)
		{
			
			sprintf (buf1, "%%%d.%dx", iOffsetLength, iOffsetLength);
			for (m = 0; m < iByteSpace; m++)
				strcat (buf1, " ");
			sprintf (buf2, buf1, a); 
			l = 0; 
			n = 0;
			while (buf2[n] != '\0')
				pMem[k + (l++)] = buf2[n++]; 
			
			for (j = 0; j < iBytesPerLine; j++)
			{
				if (a+j > FileBuffer.Upper ())
				{
					
					pMem[k + l + j*3    ] = ' ';
					pMem[k + l + j*3 + 1] = ' ';
					pMem[k + l + j*3 + 2] = ' ';
					
					pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = ' ';
				}
				else
				{
					
					sprintf (buf1, "%2.2x ", FileBuffer[a + j]);
					pMem[k + l + j*3    ] = buf1[0];
					pMem[k + l + j*3 + 1] = buf1[1];
					pMem[k + l + j*3 + 2] = buf1[2];
					
					if( iCharacterSet == OEM_FIXED_FONT && FileBuffer[a + j] != 0 )
					{
						pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = FileBuffer[a + j];
					}
					else if( (FileBuffer[a + j] >= 32 && FileBuffer[a + j] <= 126) || (FileBuffer[a + j]>=160 && FileBuffer[a + j] <= 255) || (FileBuffer[a + j] >= 145 && FileBuffer[a + j] <= 146) )
					{
						pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = FileBuffer[a + j];
					}
					else
					{
						pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = '.';
					}
				}
			}
			pMem[k + iCharsPerLine    ] = '\r';
			pMem[k + iCharsPerLine + 1] = '\n';
		}
		pMem[buflen-1] = '\0';
		
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		if (iCopyHexdumpMode == BST_CHECKED)
		{
			
			HGLOBAL hGlobal = GlobalAlloc (GHND, buflen);
			if (hGlobal != NULL)
			{
				char* pDest = (char*) GlobalLock (hGlobal);
				memcpy (pDest, pMem, buflen);
				GlobalUnlock (hGlobal);
				OpenClipboard (hwnd);
				EmptyClipboard ();
				SetClipboardData (CF_TEXT, hGlobal);
				CloseClipboard ();
			}
			else
				MessageBox (hwnd, "Not enough memory to hexdump to clipboard.", "Hexdump error:", MB_OK | MB_ICONERROR);
		}
		else
		{
			
			char cFileName[_MAX_PATH];
			char szTitleName[_MAX_FNAME + _MAX_EXT];
			strcpy (cFileName, "hexdump.txt");
			OPENFILENAME ofn;
			ofn.lStructSize = sizeof (OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.hInstance = NULL;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
			ofn.lpstrCustomFilter = NULL;
			ofn.nMaxCustFilter = 0;
			ofn.nFilterIndex = 0;
			ofn.lpstrFile = cFileName;
			ofn.nMaxFile = _MAX_PATH;
			ofn.lpstrFileTitle = szTitleName;
			ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrTitle = NULL;
			ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;
			ofn.nFileOffset = 0;
			ofn.nFileExtension = 0;
			ofn.lpstrDefExt = NULL;
			ofn.lCustData = 0L;
			ofn.lpfnHook = NULL;
			ofn.lpTemplateName = NULL;
			if (GetSaveFileName (&ofn))
			{
				int iFileHandle;
				if ((iFileHandle = _open (cFileName, _O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
				{
					
					if ((_write (iFileHandle, pMem, buflen-1)) != -1)
					{
						MessageBox (hwnd, "Hexdump saved.", "Notice:", MB_OK | MB_ICONINFORMATION);
					}
					else
						MessageBox (hwnd, "Could not save Hexdump.", "Hexdump error:", MB_OK | MB_ICONERROR);
					_close (iFileHandle);
				}
				else
					MessageBox (hwnd, "Could not save Hexdump.", "Hexdump error:", MB_OK | MB_ICONERROR);
			}
			repaint ();
		}
		delete [] pMem;
	}
	return 1;
}

BOOL CALLBACK CopyHexdumpDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[16];
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "%X", toLOROM(iCopyHexdumpDialogStart));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "%X", toHIROM(iCopyHexdumpDialogStart));
			else
				sprintf (cBuf, "%X", iCopyHexdumpDialogStart);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "%X", toLOROM(iCopyHexdumpDialogEnd));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "%X", toHIROM(iCopyHexdumpDialogEnd));
			else
				sprintf (cBuf, "%X", iCopyHexdumpDialogEnd);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			if (iCopyHexdumpMode == BST_CHECKED)
				CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
			else
				CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf1[16], buf2[16];
				if (GetDlgItemText (hDialog, IDC_EDIT1, buf1, 16)!=0 && GetDlgItemText (hDialog, IDC_EDIT2, buf2, 16))
				{
					sscanf (buf1, "%X", &iCopyHexdumpDialogStart);
					if(iOffsetMode == LOROM)
						iCopyHexdumpDialogStart = fromLOROM(iCopyHexdumpDialogStart);
					else if(iOffsetMode == HIROM)
						iCopyHexdumpDialogStart = fromHIROM(iCopyHexdumpDialogStart);
					sscanf (buf2, "%X", &iCopyHexdumpDialogEnd);
					if(iOffsetMode == LOROM)
						iCopyHexdumpDialogStart = fromLOROM(iCopyHexdumpDialogEnd);
					else if(iOffsetMode == HIROM)
						iCopyHexdumpDialogStart = fromHIROM(iCopyHexdumpDialogEnd);
					iCopyHexdumpMode = IsDlgButtonChecked (hDialog, IDC_RADIO2);
					EndDialog (hDialog, TRUE);
				}
				else
					EndDialog (hDialog, FALSE);
				return TRUE;
			}
		case IDCANCEL:
			EndDialog (hDialog, FALSE);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::fast_paste ()
{
	if( iInsertMode )
		iPasteMode = 2;
	else
		iPasteMode = 1;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_FASTPASTE_DIALOG), hwnd, (DLGPROC) FastPasteDialogProc))
	{
		switch( iPasteMode )
		{
		case 1: 
			{
				char* pcPastestring;
				
				int iDestinationLength;
				if( iPasteAsText == TRUE )
				{
					iDestinationLength = strlen( pcPasteText );
					pcPastestring = new char[ iDestinationLength ];
					memcpy( pcPastestring, pcPasteText, iDestinationLength );
				}
				else
				{
					iDestinationLength = bytecode_translate (&pcPastestring, pcPasteText, strlen (pcPasteText), iCharacterSet, iBinaryMode);
				}
				if (iDestinationLength > 0)
				{
					
					
					if (FileBuffer.Length()-iCurrentByte >= (iPasteSkip+iDestinationLength)*iPasteTimes)
					{
						
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						int i,k;
						for (k=0; k<iPasteTimes; k++)
						{
							for (i=0; i<iDestinationLength; i++)
							{
								FileBuffer[(iCurrentByte+k*(iPasteSkip+iDestinationLength))+i] = pcPastestring[i];
								vChanges[(iCurrentByte+k*(iPasteSkip+iDestinationLength))+i] = COLORCHANGE;
							}
						}
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
					}
					else
					{
						MessageBox (hwnd, "There is not enough space in the file for overwriting.", "Paste error:", MB_OK | MB_ICONERROR);
					}
					delete [] pcPastestring;
				}
				delete [] pcPasteText;
				repaint ();
				break;
			}

		case 2: 
			{
				char* pcPastestring;
				int iDestinationLength;
				if( iPasteAsText == TRUE )
				{
					iDestinationLength = strlen( pcPasteText );
					pcPastestring = new char[ iDestinationLength ];
					memcpy( pcPastestring, pcPasteText, iDestinationLength );
				}
				else
				{
					iDestinationLength = bytecode_translate (&pcPastestring, pcPasteText, strlen (pcPasteText), iCharacterSet, iBinaryMode);
				}
				if (iDestinationLength > 0)
				{
					
					SetCursor (LoadCursor (NULL, IDC_WAIT));
					int i, k;
					for( k = 0,i=iCurrentByte; k < iPasteTimes; k++ )
					{
						if(!FileBuffer.InsertExpandOn(iCurrentByte,(unsigned char*)pcPastestring,0,iDestinationLength))
						{
							delete [] pcPastestring;
							delete [] pcPasteText;
							SetCursor (LoadCursor (NULL, IDC_ARROW));
							MessageBox (hwnd, "Not enough memory for insertion.", "Paste error:", MB_OK | MB_ICONERROR);
							update_size_change ();
							return FALSE;
						}
						iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
						long m = iCurrentByte;
						for(m = m;m<iCurrentByte+iDestinationLength;m++)
							vChanges.insert(COLORCHANGE,m);
						iCurrentByte+=iDestinationLength+iPasteSkip;
					}
					iCurrentByte=i;
					iFileChanged = TRUE;
					bFilestatusChanged = TRUE;
					update_size_change ();
					delete [] pcPastestring;
					SetCursor (LoadCursor (NULL, IDC_ARROW));
				}
				delete [] pcPasteText;
				repaint ();
				break;
			}

		default:
			break;
		}
	}
	return 0;
}


BOOL CALLBACK FastPasteDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf2[16];
			sprintf (buf2, "%d", iPasteTimes);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), buf2);
			sprintf (buf2, "%d", iPasteSkip);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT3), buf2);
			
			if( iPasteMode == 1 )
				CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			else
				CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
			if( iPasteAsText == TRUE )
				CheckDlgButton (hDialog, IDC_CHECK1, BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				if (IsDlgButtonChecked (hDialog, IDC_CHECK1) == BST_CHECKED)
					iPasteAsText = TRUE;
				else
					iPasteAsText = FALSE;
				if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
					iPasteMode = 1;
				else
					iPasteMode = 2;
				char cBuf[64];
				int i;
				if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 64) != 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Number of times to paste is not recognized.", "Paste error:", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i<=0)
				{
					MessageBox (hDialog, "Number of times to paste must be at least 1.", "Paste error:", MB_OK | MB_ICONERROR);
					EndDialog (hDialog, 0);
					return 0;
				}
				iPasteTimes = i;
				if (GetDlgItemText (hDialog, IDC_EDIT3, cBuf, 64) != 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Number of bytes to skip is not recognized.", "Paste error:", MB_OK | MB_ICONERROR);
						EndDialog (hDialog, 0);
						return 0;
					}
				}
				iPasteSkip = i;
				if (OpenClipboard (NULL))
				{
					HGLOBAL hClipMemory = GetClipboardData (CF_TEXT);
					if (hClipMemory != NULL)
					{
						int gsize = GlobalSize (hClipMemory);
						if (gsize > 0)
						{
							char* pClipMemory = (char*) GlobalLock (hClipMemory);
							pcPasteText = new char[gsize];
							memcpy (pcPasteText, pClipMemory, gsize);
						}
						GlobalUnlock (hClipMemory);
					}
					CloseClipboard ();
				}
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::edit_paste ()
{
	if( iInsertMode )
		iPasteMode = 2;
	else
		iPasteMode = 1;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_PASTEDIALOG), hwnd, (DLGPROC) PasteDialogProc))
	{
		switch( iPasteMode )
		{
		case 1: 
			{
				char* pcPastestring;
				
				int iDestinationLength;
				if( iPasteAsText == TRUE )
				{
					iDestinationLength = strlen( pcPasteText );
					pcPastestring = new char[ iDestinationLength ];
					memcpy( pcPastestring, pcPasteText, iDestinationLength );
				}
				else
				{
					iDestinationLength = bytecode_translate (&pcPastestring, pcPasteText, strlen (pcPasteText), iCharacterSet, iBinaryMode);
				}
				if (iDestinationLength > 0)
				{
					
					if (FileBuffer.Length()-iCurrentByte >= (iPasteSkip+iDestinationLength)*iPasteTimes)
					{
						
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						int i,k;
						for (k=0; k<iPasteTimes; k++)
						{
							for (i=0; i<iDestinationLength; i++)
							{
								FileBuffer[(iCurrentByte+k*(iPasteSkip+iDestinationLength))+i] = pcPastestring[i];
								vChanges[(iCurrentByte+k*(iPasteSkip+iDestinationLength))+i] = COLORCHANGE;
							}
						}
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
					}
					else
					{
						MessageBox (hwnd, "There is not enough space in the file for overwriting.", "Paste error:", MB_OK | MB_ICONERROR);
					}
					delete [] pcPastestring;
				}
				delete [] pcPasteText;
				repaint ();
				break;
			}

		case 2: 
			{
				char* pcPastestring;
				int iDestinationLength;
				if( iPasteAsText == TRUE )
				{
					iDestinationLength = strlen( pcPasteText );
					pcPastestring = new char[ iDestinationLength ];
					memcpy( pcPastestring, pcPasteText, iDestinationLength );
				}
				else
				{
					iDestinationLength = bytecode_translate( &pcPastestring, pcPasteText, strlen( pcPasteText ), iCharacterSet, iBinaryMode );
				}
				if( iDestinationLength > 0 )
				{
					
					SetCursor( LoadCursor( NULL, IDC_WAIT ) );
					int i, k;
					for( k = 0,i=iCurrentByte; k < iPasteTimes; k++ )
					{
						if(!FileBuffer.InsertExpandOn(iCurrentByte,(unsigned char*)pcPastestring,0,iDestinationLength))
						{
							delete [] pcPastestring;
							delete [] pcPasteText;
							SetCursor (LoadCursor (NULL, IDC_ARROW));
							MessageBox (hwnd, "Not enough memory for insertion.", "Paste error:", MB_OK | MB_ICONERROR);
							update_size_change ();
							return FALSE;
						}
						iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
						long m = iCurrentByte;
						for(m = m;m<iCurrentByte+iDestinationLength;m++)
							vChanges.insert(COLORCHANGE,m);
						iCurrentByte+=iDestinationLength+iPasteSkip;
					}
					iCurrentByte=i;
					iFileChanged = TRUE;
					bFilestatusChanged = TRUE;
					update_size_change ();
					delete [] pcPastestring;
					SetCursor (LoadCursor (NULL, IDC_ARROW));
				}
				else
				{
					delete [] pcPasteText;
					MessageBox( hwnd, "Insertion of array of size 0.", "Paste error:", MB_OK | MB_ICONERROR);
					update_size_change ();
					return FALSE;
				}
				
				delete [] pcPasteText;
				repaint ();
				break;
			}

		default:
			break;
		}
	}
	return 0;
}

BOOL CALLBACK PasteDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char *cBuf;
			if (OpenClipboard (NULL))
			{
				HGLOBAL hClipMemory = GetClipboardData (CF_TEXT);
				if (hClipMemory != NULL)
				{
					int gsize = GlobalSize (hClipMemory);
					if (gsize > 0)
					{
						char* pClipMemory = (char*) GlobalLock (hClipMemory);
						cBuf = new char[gsize];
						memcpy (cBuf, pClipMemory, gsize);
						SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
						delete [] cBuf;
					}
					GlobalUnlock (hClipMemory);
				}
				CloseClipboard ();
			}
			char buf2[16];
			sprintf (buf2, "%d", iPasteTimes);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), buf2);
			sprintf (buf2, "%d", iPasteSkip);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT3), buf2);
			if( iPasteMode == 1 )
				CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			else
				CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
			if( iPasteAsText == TRUE )
				CheckDlgButton (hDialog, IDC_CHECK1, BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				if (IsDlgButtonChecked (hDialog, IDC_CHECK1) == BST_CHECKED)
					iPasteAsText = TRUE;
				else
					iPasteAsText = FALSE;
				if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
					iPasteMode = 1;
				else if( IsDlgButtonChecked( hDialog, IDC_RADIO2 ) == BST_CHECKED )
					iPasteMode = 2;
				char cBuf[64];
				int i;
				if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 64) != 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Number of times to paste is not recognized.", "Paste error:", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1 || i==0)
				{
					MessageBox (hDialog, "Number of times to paste must be at least 1.", "Paste error:", MB_OK | MB_ICONERROR);
					EndDialog (hDialog, 0);
					return 0;
				}
				iPasteTimes = i;
				if (GetDlgItemText (hDialog, IDC_EDIT3, cBuf, 64) != 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Number of bytes to skip is not recognized.", "Paste error:", MB_OK | MB_ICONERROR);
						EndDialog (hDialog, 0);
						return 0;
					}
				}
				iPasteSkip = i;
				iPasteMaxTxtLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0) + 1;
				pcPasteText = new char[iPasteMaxTxtLength];
				GetDlgItemText (hDialog, IDC_EDIT1, pcPasteText, iPasteMaxTxtLength);
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::select_all ()
{
	if (FileBuffer.Length() <= 0)
		return 0;
	bSelected = TRUE;
	iStartOfSelection = 0;
	iEndOfSelection = FileBuffer.Upper ();
	adjust_view_for_selection ();
	repaint ();
	return 1;
}

int Translhextion::select_block ()
{
	iEndOfSelSetting = iStartOfSelSetting = iCurrentByte;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_SELECT_BLOCK_DIALOG), hwnd, (DLGPROC) SelectBlockDialogProc))
	{
		if (iStartOfSelSetting<0 || iStartOfSelSetting>(FileBuffer.Upper ()) || iEndOfSelSetting<0 || iEndOfSelSetting>(FileBuffer.Upper ()))
		{
			MessageBox (hwnd, "Specified offset is not valid.", "Select block error:", MB_OK | MB_ICONERROR);
			return 0;
		}
		iStartOfSelection = iStartOfSelSetting;
		iEndOfSelection = iEndOfSelSetting;
		bSelected = TRUE;
		adjust_view_for_selection ();
		repaint ();
	}
	return 0;
}

BOOL CALLBACK SelectBlockDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[128];
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iStartOfSelSetting));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iStartOfSelSetting));
			else
				sprintf (cBuf, "x%X", iStartOfSelSetting);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iEndOfSelSetting));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iEndOfSelSetting));
			else
				sprintf (cBuf, "x%X", iEndOfSelSetting);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			CheckDlgButton(hDialog,IDC_RADIO1,BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[128];
				int i=0;
				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 128) != 0)
				{
					if (sscanf (cBuf, "x%X", &iStartOfSelSetting) == 0)
					{
						if (sscanf (cBuf, "%d", &iStartOfSelSetting) == 0)
						{
							MessageBox (hDialog, "Starting offset is not recognized.", "Select block error:", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
					if(iOffsetMode == LOROM)
						iStartOfSelSetting = fromLOROM(iStartOfSelSetting);
					else if(iOffsetMode == HIROM)
						iStartOfSelSetting = fromHIROM(iStartOfSelSetting);
				}
				if (i==-1)
				{
					EndDialog (hDialog, 0);
					return 0;
				}
				if (IsDlgButtonChecked(hDialog,IDC_RADIO1) && GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 128) != 0)
				{
					if (sscanf (cBuf, "x%X", &iEndOfSelSetting) == 0)
						if (sscanf (cBuf, "%d", &iEndOfSelSetting) == 0)
						{
							MessageBox (hDialog, "Ending offset is not recognized.", "Select block error:", MB_OK | MB_ICONERROR);
							i = -1;
						}
					if(iOffsetMode == LOROM)
						iEndOfSelSetting = fromLOROM(iEndOfSelSetting);
					else if(iOffsetMode == HIROM)
						iEndOfSelSetting = fromHIROM(iEndOfSelSetting);
				}
				else
				{
					if(GetDlgItemText (hDialog, IDC_EDIT3, cBuf, 128) != 0)
					{
						if(sscanf (cBuf, "%d", &iEndOfSelSetting) != 0)
						{
							iEndOfSelSetting = iStartOfSelSetting + iEndOfSelSetting - 1;
						}
						else
						{
							MessageBox (hDialog, "Number of bytes is not recognized.", "Select block error:", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
				}
				if (i==-1)
				{
					EndDialog (hDialog, 0);
					return 0;
				}
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Translhextion::fill_with()
{
	tmpstart=iStartOfSelection;tmpend=iEndOfSelection;
		if (DialogBox(hInstance, MAKEINTRESOURCE (IDD_FILL_WITH), hwnd, (DLGPROC) FillWithDialogProc))
	{
		SetCursor(LoadCursor(NULL,IDC_WAIT));
		unsigned char (*fnc)(const int&);
		long i,q,qmax;
		if (TypeOfCurrent)
		{
			fnc=file;
			qmax = FWFilelen;
		}
		else 
		{
			fnc=input;
			qmax=buflen;
		}
		if (iStartOfSelection>iEndOfSelection)
			sswap(tmpstart,tmpend);
		switch(asstyp)
		{
			case 0:
			{
				for(q=0,i=tmpstart;i<=tmpend;i++)
				{
					FileBuffer[(int)i]=fnc(q);
					vChanges[(int)i] = COLORCHANGE;
					q++;
					q%=qmax;
				}
			}
				break;
			case 1:
			{
				for(q=0,i=tmpstart;i<=tmpend;i++)
				{
					FileBuffer[(int)i]|=fnc(q);
					vChanges[(int)i] = COLORCHANGE;
					q++;
					q%=qmax;
				}
			}
				break;
			case 2:
			{
				for(q=0,i=tmpstart;i<=tmpend;i++)
				{
					FileBuffer[(int)i]&=fnc(q);
					vChanges[(int)i] = COLORCHANGE;
					q++;
					q%=qmax;
				}
			}
			break;
			case 3:
			{
				for(q=0,i=tmpstart;i<=tmpend;i++)
				{
					FileBuffer[(int)i]^=fnc(q);
					vChanges[(int)i] = COLORCHANGE;
					q++;
					q%=qmax;
				}
			}
			break;
		}
		if (TypeOfCurrent) _close(FWFile);
		iFileChanged = TRUE;
		bFilestatusChanged = TRUE;
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		repaint ();
	}
}


BOOL CALLBACK FillWithDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		case WM_INITDIALOG:
			{
			HWND hEditt=GetDlgItem(hDialog, IDC_HEX);
			SendMessage(hEditt,EM_SETLIMITTEXT, (WPARAM) FILL_WITH_MAX,0);
			SetWindowText(hEditt, pcFWText);
			SetFocus(hDialog);
			SetFocus(hEditt);
			EnableWindow(hEditt,!TypeOfCurrent);
			oldproc = GetWindowLong (hEditt, GWL_WNDPROC);
			SetWindowLong(hEditt,GWL_WNDPROC,(LONG)HexProc);
			EnableWindow(GetDlgItem(hDialog, IDC_HEXSTAT),!TypeOfCurrent);

			HWND typ = GetDlgItem(hDialog, IDC_TYPE);
			SendMessage(typ,CB_ADDSTRING ,0,(LPARAM) (LPCTSTR) "Input");
			SendMessage(typ,CB_ADDSTRING ,0,(LPARAM) (LPCTSTR) "File");
			SendMessage(typ,CB_SETCURSEL,(WPARAM)TypeOfCurrent,0);

			HWND fn=GetDlgItem(hDialog, IDC_FN);
			SetWindowText(fn,szFWFileName);
			EnableWindow(fn,TypeOfCurrent);
			EnableWindow(GetDlgItem(hDialog, IDC_BROWSE),TypeOfCurrent);
			EnableWindow(GetDlgItem(hDialog, IDC_FILESTAT),TypeOfCurrent);

			char bufff[250];
			int temp=1+abs(tmpstart-tmpend);
			if(iOffsetMode == LOROM)
				sprintf(bufff,"%d=0x%X",tmpstart,toLOROM(tmpstart));
			else if(iOffsetMode == HIROM)
				sprintf(bufff,"%d=0x%X",tmpstart,toHIROM(tmpstart));
			else
				sprintf(bufff,"%d=0x%X",tmpstart,tmpstart);
			SetDlgItemText(hDialog, IDC_STS,bufff);
			if(iOffsetMode == LOROM)
				sprintf(bufff,"%d=0x%X",tmpend,toLOROM(tmpend));
			else if(iOffsetMode == HIROM)
				sprintf(bufff,"%d=0x%X",tmpend,toHIROM(tmpend));
			else
				sprintf(bufff,"%d=0x%X",tmpend,tmpend);
			SetDlgItemText(hDialog, IDC_ES,bufff);
			if(iOffsetMode == LOROM)
				sprintf(bufff,"%d=0x%X",temp,toLOROM(temp));
			else if(iOffsetMode == HIROM)
				sprintf(bufff,"%d=0x%X",temp,toHIROM(temp));
			else
				sprintf(bufff,"%d=0x%X",temp,temp);
			SetDlgItemText(hDialog, IDC_SS,bufff);
			hfdef = (HFONT) SendDlgItemMessage(hDialog,IDC_R,WM_GETFONT,0,0);
			if((hfon = CreateFont(16,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Symbol"))==NULL)PostQuitMessage(1);
			FillWithInit(hDialog);
			switch(asstyp)
			{
				case 0:CheckDlgButton (hDialog, IDC_EQ, BST_CHECKED);break;
				case 1:CheckDlgButton (hDialog, IDC_OR, BST_CHECKED);break;
				case 2:CheckDlgButton (hDialog, IDC_AND, BST_CHECKED);break;
				case 3:CheckDlgButton (hDialog, IDC_XOR, BST_CHECKED);break;
			}
			return 0;
		}
		break;
		case WM_COMMAND:{
			switch (LOWORD (wParam))
			{
				case IDOK:
					{
					int i;
					if(TypeOfCurrent)
					{
						GetDlgItemText(hDialog,IDC_FN,szFWFileName,_MAX_PATH);
						if((FWFile=_open(szFWFileName,_O_RDONLY|_O_BINARY))==-1)
						{
							MessageBox(hDialog,"Error opening file.","Fill with file error:", MB_OK | MB_ICONERROR);
							return 1;
						}
						if((FWFilelen=_filelength(FWFile))==0)
						{
							MessageBox (hDialog, "Can't fill a selection with a zero-byte file.", "Fill with file error:", MB_OK | MB_ICONERROR);
							_close(FWFile);
							return 1;
						}
						else if (FWFilelen==-1)
						{
							MessageBox(hDialog,"Error opening file.","Fill with file error:", MB_OK | MB_ICONERROR);
							_close(FWFile);
							return 1;
						}
					}
					else
					{
						if (!buflen)
						{
							MessageBox (hDialog, "Can't fill a selection with a string of size 0.", "Fill with string error:", MB_OK | MB_ICONERROR);
							return 1;
						}
						if ((i=(GetDlgItemText (hDialog, IDC_HEX, pcFWText, FILL_WITH_MAX) == 0))||(i==FILL_WITH_MAX-1))
						{
							MessageBox (hDialog, "Unknown error occurred.", "Fill with error:", MB_OK | MB_ICONERROR);
							return 1;
						}
						strHex2Char();
						pcFWText[(aa?buflen:buflen*2)]='\0';

					}
					if(BST_CHECKED == IsDlgButtonChecked(hDialog,IDC_EQ))
						asstyp=0;
					else if(BST_CHECKED == IsDlgButtonChecked(hDialog,IDC_OR))asstyp=1;
					else if(BST_CHECKED == IsDlgButtonChecked(hDialog,IDC_AND))asstyp=2;
					else if(BST_CHECKED == IsDlgButtonChecked(hDialog,IDC_XOR))asstyp=3;
					DeleteObject(hfon);
					DeleteObject(hfdef);
					EndDialog (hDialog, 1);
					return 0;
				}
				break;
				case IDCANCEL:
				{
					DeleteObject(hfon);
					EndDialog (hDialog, 0);
					return 0;
				}
				break;
				case IDC_TYPE:
				{
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						TypeOfCurrent = (char)SendMessage(GetDlgItem(hDialog, IDC_TYPE),CB_GETCURSEL,0,0);
						EnableWindow(GetDlgItem(hDialog, IDC_FN),TypeOfCurrent);
						EnableWindow(GetDlgItem(hDialog, IDC_BROWSE),TypeOfCurrent);
						EnableWindow(GetDlgItem(hDialog, IDC_FILESTAT),TypeOfCurrent);
						TypeOfCurrent=!TypeOfCurrent;
						EnableWindow(GetDlgItem(hDialog, IDC_HEX),TypeOfCurrent);
						EnableWindow(GetDlgItem(hDialog, IDC_HEXSTAT),TypeOfCurrent);
						TypeOfCurrent=!TypeOfCurrent;
						
						FillWithInit(hDialog);
					}
				}
				break;
				case IDC_BROWSE:
				{
					
					szFWFileName[0] = '\0';
					OPENFILENAME ofn;
					ofn.lStructSize = sizeof (OPENFILENAME);
					ofn.hwndOwner = hDialog;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter = 0;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = szFWFileName;
					ofn.nMaxFile = _MAX_PATH;
					ofn.lpstrFileTitle = NULL;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
					ofn.lpstrDefExt = NULL;
					ofn.lCustData = 0L;
					ofn.lpfnHook = NULL;
					ofn.lpTemplateName = NULL;
					
					if (GetOpenFileName(&ofn))SetDlgItemText(hDialog,IDC_FN,ofn.lpstrFile);
					return 0;
				}
				break;
				case IDC_HEX:
				{
					if(HIWORD(wParam)==EN_UPDATE)
					{
						GetWindowText(GetDlgItem(hDialog, IDC_HEX), pcFWText, FILL_WITH_MAX);
						strHex2Char();
						
						FillWithInit(hDialog);
						return 0;
					}
					return 1;
				}
				break;
				default:return 1;
			}
		}
		break;
	}
	return FALSE;
}

void Translhextion::save_selection_as()
{
	char cFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	OPENFILENAME ofn;
	szTitleName[0] =cFileName[0] = '\0';
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Save Selection As";
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetSaveFileName (&ofn))
	{
		int iFileHandle;
		if ((iFileHandle = _open (cFileName, _O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			if(iStartOfSelection>iEndOfSelection)sswap(iStartOfSelection,iEndOfSelection);
			if (!((_write (iFileHandle, &FileBuffer[iStartOfSelection], iEndOfSelection-iStartOfSelection+1)) != -1))
				MessageBox (hwnd, "Could not save selection file.", "Save selection as error:", MB_OK | MB_ICONERROR);
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			_close (iFileHandle);
		}
		else
			MessageBox (NULL, "Could not save selection file.", "Save selection as error:", MB_OK | MB_ICONERROR);
	}
	repaint ();
}

//OFFSET

int Translhextion::_Goto()
{
	if( filename[0] == '\0' )
		return 0;
	if( DialogBox( hInstance, MAKEINTRESOURCE(IDD_GOTODIALOG), hwnd, (DLGPROC) GoToDialogProc ) )
	{
		int offset, i = 0, r = 0;
		if( pcGotoDialogBuffer[0] == '+' || pcGotoDialogBuffer[0] == '-' )
		{
			r = 1;
		}

		if( sscanf( &pcGotoDialogBuffer[r], "x%X", &offset ) == 0 )
		{
			if( sscanf ( &pcGotoDialogBuffer[r], "%d", &offset ) == 0 )
			{
				i = -1;
			}
		}
		if( i==-1 )
		{
			MessageBox (hwnd, "Offset is not recognized.", "Go to error:", MB_OK | MB_ICONERROR);
			return 0;
		}
		

		if( r == 1 )
		{
			
			if ( pcGotoDialogBuffer[0] == '-' )
				r = -1;

			if( iCurrentByte + r * offset >= 0 && iCurrentByte + r * offset <= (FileBuffer.Upper ()) )
			{
				iCurrentByte = iCurrentByte + r * offset;
				iCurrentLine = (iCurrentByte / iBytesPerLine);
				update_vertical_scrollbar();
				repaint();
			}
			else
				MessageBox( hwnd, "Invalid offset.", "Go to error:", MB_OK | MB_ICONERROR );
		}
		else
		{
			if(iOffsetMode == LOROM)
				offset = fromLOROM(offset);
			else if(iOffsetMode == HIROM)
				offset = fromHIROM(offset);
			
			if( offset >= 0 && offset <= (FileBuffer.Upper ()) )
			{
				iCurrentByte = offset;
				iCurrentLine = (iCurrentByte / iBytesPerLine);
				update_vertical_scrollbar();
				repaint();
			}
			else
				MessageBox( hwnd, "Invalid offset.", "Go to error:", MB_OK | MB_ICONERROR );
		}
	}
	return 1;
}

BOOL CALLBACK GoToDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		if( iGotoDialogBufLength == 0 )
		{
			
			iGotoDialogBufLength = SendMessage( GetDlgItem( hDialog, IDC_EDIT1 ), EM_GETLIMITTEXT, 0, 0 );
			
			pcGotoDialogBuffer = new char[iGotoDialogBufLength];
			if( pcGotoDialogBuffer == NULL )
				MessageBox( hDialog, "Could not allocate Goto buffer.", "Go to error:", MB_OK | MB_ICONERROR );
			memset( pcGotoDialogBuffer, 0, iGotoDialogBufLength );
		}
		if( pcGotoDialogBuffer != NULL )
			SetWindowText( GetDlgItem (hDialog, IDC_EDIT1), pcGotoDialogBuffer );
		SetFocus(hDialog);
		
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			
			EndDialog( hDialog, GetDlgItemText( hDialog, IDC_EDIT1, pcGotoDialogBuffer, iGotoDialogBufLength ) );
			return TRUE;

		case IDCANCEL:
			EndDialog( hDialog, 0 );
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::manipulate_bits ()
{
	if (FileBuffer.Length() == 0)
	{
		MessageBox (hwnd, "File is empty.", "Manipulate bits error:", MB_OK | MB_ICONERROR);
		return 0;
	}
	if (iCurrentByte<0 || iCurrentByte>(FileBuffer.Upper ()))
	{
		MessageBox (hwnd, "Must choose a byte in the file.", "Manipulate bits error:", MB_OK | MB_ICONERROR);
		return 0;
	}
	cBitValue = FileBuffer[iCurrentByte];
	iManipPos = iCurrentByte;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_MANIPBITSDIALOG), hwnd, (DLGPROC) BitManipDialogProc))
	{
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		FileBuffer[iCurrentByte] = cBitValue;
		vChanges[iCurrentByte] = COLORCHANGE;
		iFileChanged = TRUE;
		bFilestatusChanged = TRUE;
		repaint ();
	}
	return 1;
}

BOOL CALLBACK BitManipDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[64];
			if(iOffsetMode == LOROM)
				sprintf( cBuf, "Manipulate bits at offset %d=0x%X", iManipPos, toLOROM(iManipPos) );
			else if(iOffsetMode == HIROM)
				sprintf( cBuf, "Manipulate bits at offset %d=0x%X", iManipPos, toHIROM(iManipPos) );
			else
				sprintf( cBuf, "Manipulate bits at offset %d=0x%X", iManipPos, iManipPos );
			SetWindowText( GetDlgItem( hDialog, IDC_STATIC1 ), cBuf );
			
			sprintf( cBuf, "Value: 0x%X , %d signed, %u unsigned.", (unsigned char) cBitValue, (signed char) cBitValue, (unsigned char) cBitValue );
			SetWindowText( GetDlgItem( hDialog, IDC_STATIC2 ), cBuf );

			SetFocus(hDialog);
			if (bitval (&cBitValue,0) != 0)
				CheckDlgButton (hDialog, IDC_CHECK1, BST_CHECKED);
			if (bitval (&cBitValue,1) != 0)
				CheckDlgButton (hDialog, IDC_CHECK2, BST_CHECKED);
			if (bitval (&cBitValue,2) != 0)
				CheckDlgButton (hDialog, IDC_CHECK3, BST_CHECKED);
			if (bitval (&cBitValue,3) != 0)
				CheckDlgButton (hDialog, IDC_CHECK4, BST_CHECKED);
			if (bitval (&cBitValue,4) != 0)
				CheckDlgButton (hDialog, IDC_CHECK5, BST_CHECKED);
			if (bitval (&cBitValue,5) != 0)
				CheckDlgButton (hDialog, IDC_CHECK6, BST_CHECKED);
			if (bitval (&cBitValue,6) != 0)
				CheckDlgButton (hDialog, IDC_CHECK7, BST_CHECKED);
			if (bitval (&cBitValue,7) != 0)
				CheckDlgButton (hDialog, IDC_CHECK8, BST_CHECKED);
			return FALSE;
		}

	case WM_COMMAND:
		{
			cBitValue = 0;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK8) == BST_CHECKED)
				cBitValue += 128;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK7) == BST_CHECKED)
				cBitValue += 64;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK6) == BST_CHECKED)
				cBitValue += 32;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK5) == BST_CHECKED)
				cBitValue += 16;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK4) == BST_CHECKED)
				cBitValue += 8;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK3) == BST_CHECKED)
				cBitValue += 4;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK2) == BST_CHECKED)
				cBitValue += 2;
			if (IsDlgButtonChecked (hDialog, IDC_CHECK1) == BST_CHECKED)
				cBitValue += 1;
			switch (LOWORD (wParam))
			{
			case IDC_CHECK1: case IDC_CHECK2: case IDC_CHECK3: case IDC_CHECK4:
			case IDC_CHECK5: case IDC_CHECK6: case IDC_CHECK7: case IDC_CHECK8:
				{
					char cBuf[64];
					sprintf( cBuf, "Value: 0x%X , %d signed, %u unsigned.", (unsigned char) cBitValue, (signed char) cBitValue, (unsigned char) cBitValue );
					SetWindowText( GetDlgItem( hDialog, IDC_STATIC2 ), cBuf );
				}
				break;

			case IDOK:
				{
					EndDialog (hDialog, 1);
					return TRUE;
				}
		
			case IDCANCEL:
				EndDialog (hDialog, 0);
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

int Translhextion::compare_arrays (char* ps, int sl, char* pd, int dl)
{
	int i, diff=0, type=1;
	for (i=0; i<sl && i<dl; i++)
	{
		switch (type)
		{
		case 0:
			if (ps[i]==pd[i])
			{
				diff++;
				type = 1;
			}
			break;

		case 1:
			if (ps[i]!=pd[i])
			{
				type = 0;
			}
			break;
		}
	}
	if (type == 0)
		diff++;
	return diff;
}

int Translhextion::get_diffs (char* ps, int sl, char* pd, int dl, intpair* pdiff)
{
	int i, diff=0, type=1;
	
	for (i=0; i<sl && i<dl; i++)
	{
		switch (type)
		{
		case 0:
			
			if (ps[i]==pd[i])
			{
				
				pdiff[diff].y = i-1; 
				diff++;
				type = 1;
			}
			
			break;

		case 1:
			
			if (ps[i]!=pd[i])
			{
				
				pdiff[diff].x = i; 
				type = 0;
			}
			
			break;
		}
	}
	if (type == 0) 
	{
		pdiff[diff].y = i-1;
		diff++;
	}
	return diff;
}

int Translhextion::compare ()
{
	if (FileBuffer.Length() <= 0)
	{
		MessageBox (hwnd, "Current file is empty.", "Compare error:", MB_OK | MB_ICONERROR);
		return 0;
	}
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Open File to Compare";
	ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		int iFileHandle;
		if ((iFileHandle = _open (cFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			int iFileLength = _filelength (iFileHandle);
			iDestFileLength = iFileLength;
			iSrcFileLength = FileBuffer.Length()-iCurrentByte;
			char* cmpdata = new char[iFileLength];
			
			if (_read (iFileHandle, cmpdata, iFileLength) != -1)
			{
				int diff;
				if ((diff = compare_arrays ((char*) &(FileBuffer[iCurrentByte]), FileBuffer.Length()-iCurrentByte, cmpdata, iFileLength)) == 0)
				{
					
					MessageBox (hwnd, "Data matches exactly.", "Notice:", MB_OK | MB_ICONINFORMATION);
				}
				else
				{
					
					intpair* pdiff = new intpair[diff];
					get_diffs ((char*) &(FileBuffer[iCurrentByte]), FileBuffer.Length()-iCurrentByte, cmpdata, iFileLength, pdiff);
					pdiffChoice = pdiff;
					iDiffNum = diff;
					if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_CHOOSEDIFFDIALOG), hwnd, (DLGPROC) ChooseDiffDialogProc))
					{
						iStartOfSelection = iCurrentByte+pdiff[iDiffNum].x;
						iEndOfSelection = iCurrentByte+pdiff[iDiffNum].y;
						bSelected = TRUE;
						iCurrentByte = iCurrentByte+pdiff[iDiffNum].x;
						adjust_view_for_selection ();
						repaint ();
					}
					delete [] pdiff;
				}
				_close (iFileHandle);
				delete [] cmpdata;
				return TRUE;
			}
			else
			{
				delete [] cmpdata;
				_close (iFileHandle);
				MessageBox (hwnd, "Error while reading from file.", "Compare file error:", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
	}
	return 1;
}

BOOL CALLBACK ChooseDiffDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[100];
			int i;
			sprintf (cBuf, "%d areas of difference found.", iDiffNum);
			SetWindowText (GetDlgItem (hDialog, IDC_STATIC1), cBuf);
			sprintf (cBuf, "Remaining loaded data size: %d, size of file on disk: %d.", iSrcFileLength, iDestFileLength);
			SetWindowText (GetDlgItem (hDialog, IDC_STATIC2), cBuf);
			HWND hwndList = GetDlgItem (hDialog, IDC_LIST1);
			for (i=0; i<iDiffNum; i++)
			{
				if(iOffsetMode == LOROM)
					sprintf (cBuf, "%d) %d=0x%X to %d=0x%X (%d bytes)", i+1, pdiffChoice[i].x, toLOROM(pdiffChoice[i].x),
					pdiffChoice[i].y, toLOROM(pdiffChoice[i].y), pdiffChoice[i].y-pdiffChoice[i].x+1);
				else if(iOffsetMode == HIROM)
					sprintf (cBuf, "%d) %d=0x%X to %d=0x%X (%d bytes)", i+1, pdiffChoice[i].x, toHIROM(pdiffChoice[i].x),
					pdiffChoice[i].y, toHIROM(pdiffChoice[i].y), pdiffChoice[i].y-pdiffChoice[i].x+1);
				else
					sprintf (cBuf, "%d) %d=0x%X to %d=0x%X (%d bytes)", i+1, pdiffChoice[i].x, pdiffChoice[i].x,
					pdiffChoice[i].y, pdiffChoice[i].y, pdiffChoice[i].y-pdiffChoice[i].x+1);
				SendMessage (hwndList, LB_ADDSTRING, 0, (LPARAM) cBuf);
			}
			SendMessage (hwndList, LB_SETCURSEL, 0, 0);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
			case IDCOPY:
				{
					int sumlen=1;
					int len=0;
					char*cBuf=(char*)malloc(1);
					cBuf[0]=0;
					char*bt=NULL;
					HWND hwndList = GetDlgItem (hDialog, IDC_LIST1);
					int num = SendMessage(hwndList,LB_GETCOUNT,0,0);
					for(int i=0;i<num;i++)
					{	
						len=SendMessage(hwndList,LB_GETTEXTLEN,i,0)+2;
						sumlen+=len;
						bt = (char*)realloc(cBuf,sumlen);
						if(bt!=NULL)
							cBuf=bt;
						else
						break;
					
					SendMessage(hwndList,LB_GETTEXT,i,(LPARAM)&cBuf[sumlen-len-1]);
					strcat(cBuf,"\r\n");
					}
					TextToClipboard(cBuf);
					free(cBuf);
					return TRUE;
				}
				break;


		case IDOK:
			{
				HWND hwndList = GetDlgItem (hDialog, IDC_LIST1);
				iDiffNum = SendMessage (hwndList, LB_GETCURSEL, 0, 0);
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//INSERT

int Translhextion::toggle_insertmode ()
{
	iInsertMode = (iInsertMode) ? FALSE : TRUE;
	update_window_status();
	return 1;
}

int Translhextion::edit_enterdecimalvalue ()
{
	iDecValDialogOffset = iCurrentByte;
	if (iCurrentByte <= (FileBuffer.Upper ()) && iCurrentByte >= 0)
		iDecValDialogValue = (int) FileBuffer[iCurrentByte];
	else
		iDecValDialogValue = 0;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_DECIMALDIALOG), hwnd, (DLGPROC) EnterDecimalValueDialogProc))
	{
		if (iDecValDialogOffset<0 || iDecValDialogOffset>(FileBuffer.Upper ()))
		{
			MessageBox (hwnd, "Invalid starting offset.", "Enter decimal value error:", MB_OK | MB_ICONERROR);
			return 0;
		}
		if (iDecValDialogOffset+iDecValDialogSize*iDecValDialogTimes > (FileBuffer.Upper ()))
		{
			MessageBox (hwnd, "Not enough space in file for writing decimal values.", "Enter decimal value error:", MB_OK | MB_ICONERROR);
			return 0;
		}
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		int i, k = 0;
		for (i = 0; i < iDecValDialogTimes; i++)
		{
			if (iDecValDialogOffset + k > FileBuffer.Upper ())
			{
				MessageBox (hwnd, "Reached end of file prematurely.", "Enter decimal value error:", MB_OK | MB_ICONERROR);
				break;
			}

			if (iBinaryMode == LITTLE_ENDIAN)
			{
				switch (iDecValDialogSize)
				{
				case 1:
					FileBuffer[iDecValDialogOffset + k] = iDecValDialogValue;
					vChanges[iDecValDialogOffset + k] = COLORCHANGE;
					break;

				case 2:
					FileBuffer[iDecValDialogOffset + k] = (char) (iDecValDialogValue & 0xff);
					FileBuffer[iDecValDialogOffset + k+1] = (char) ((iDecValDialogValue & 0xff00) >> 8);
					vChanges[iDecValDialogOffset + k] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k+1] = COLORCHANGE;
					break;

				case 4:
					FileBuffer[iDecValDialogOffset + k  ] = (char) (iDecValDialogValue & 0xff);
					FileBuffer[iDecValDialogOffset + k+1] = (char) ((iDecValDialogValue & 0xff00) >> 8);
					FileBuffer[iDecValDialogOffset + k+2] = (char) ((iDecValDialogValue & 0xff0000) >> 16);
					FileBuffer[iDecValDialogOffset + k+3] = (char) ((iDecValDialogValue & 0xff000000) >> 24);
					vChanges[iDecValDialogOffset + k] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k+1] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k+2] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k+3] = COLORCHANGE;
					break;
				}
			}
			else
			{
				switch (iDecValDialogSize)
				{
				case 1:
					FileBuffer[iDecValDialogOffset + k] = iDecValDialogValue;
					vChanges[iDecValDialogOffset + k] = COLORCHANGE;
					break;

				case 2:
					FileBuffer[iDecValDialogOffset + k+1] = (char) (iDecValDialogValue & 0xff);
					FileBuffer[iDecValDialogOffset + k] = (char) ((iDecValDialogValue & 0xff00) >> 8);
					vChanges[iDecValDialogOffset + k+1] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k] = COLORCHANGE;
					break;

				case 4:
					FileBuffer[iDecValDialogOffset + k+3] = (char) (iDecValDialogValue & 0xff);
					FileBuffer[iDecValDialogOffset + k+2] = (char) ((iDecValDialogValue & 0xff00) >> 8);
					FileBuffer[iDecValDialogOffset + k+1] = (char) ((iDecValDialogValue & 0xff0000) >> 16);
					FileBuffer[iDecValDialogOffset + k+0] = (char) ((iDecValDialogValue & 0xff000000) >> 24);
					vChanges[iDecValDialogOffset + k+3] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k+2] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k+1] = COLORCHANGE;
					vChanges[iDecValDialogOffset + k] = COLORCHANGE;
					break;
				}
			}
			k += iDecValDialogSize;
		}
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		iFileChanged = TRUE;
		bFilestatusChanged = TRUE;
		repaint ();
	}
	return 1;
}

BOOL CALLBACK EnterDecimalValueDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[16];
			sprintf (cBuf, "%d", iDecValDialogValue);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iDecValDialogOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iDecValDialogOffset));
			else
				sprintf (cBuf, "x%X", iDecValDialogOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			sprintf (cBuf, "1");
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT3), cBuf);
			CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
			iDecValDialogSize = 1;
		else if (IsDlgButtonChecked (hDialog, IDC_RADIO2) == BST_CHECKED)
			iDecValDialogSize = 2;
		else if (IsDlgButtonChecked (hDialog, IDC_RADIO3) == BST_CHECKED)
			iDecValDialogSize = 4;
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[16];
				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 16) != 0)
					sscanf (cBuf, "%d", &iDecValDialogValue);
				if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 16) != 0)
					sscanf (cBuf, "%X", &iDecValDialogOffset);
				int i;
				if (sscanf (cBuf, "x%X", &i) == 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						i = -1;
					}
				}
				if(iOffsetMode == LOROM)
					iDecValDialogOffset = fromLOROM(i);
				else if(iOffsetMode == HIROM)
					iDecValDialogOffset = fromHIROM(i);
				else
					iDecValDialogOffset = i;
				if (GetDlgItemText (hDialog, IDC_EDIT3, cBuf, 16) != 0)
					sscanf (cBuf, "%d", &iDecValDialogTimes);
				EndDialog (hDialog, 1);
				return TRUE;
			}

		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::edit_append ()
{
	iAppendbytes = 1;
	iManipPos = iCurrentByte;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_APPENDDIALOG), hwnd, (DLGPROC) AppendDialogProc))
	{
		int i, oldupbound = FileBuffer.Length();
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		if (FileBuffer.SetLength (FileBuffer.GetSize()+iAppendbytes) == FALSE)
		{
			MessageBox (hwnd, "Not enough memory for appending.", "Append error:", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		int m = 0;
		for(m = 0;m<iAppendbytes;m++)
			vChanges.push_back(COLORCHANGE);
		FileBuffer.SetUpperBound(FileBuffer.Upper()+iAppendbytes);
		for (i=0; i<iAppendbytes; i++)
		{
			FileBuffer[oldupbound+i] = 0;
			vChanges[oldupbound+i] = COLORCHANGE;
		}
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		iFileChanged = TRUE;
		bFilestatusChanged = TRUE;
		update_size_change ();
		repaint ();
	}
	return 1;
}

BOOL CALLBACK AppendDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[32];
			sprintf (cBuf, "%d", iAppendbytes);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[64];
				int i=-1;
				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 64) != 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Number of bytes to append is not recognized.", "Append error:", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDialog, 0);
					return 0;
				}
				iAppendbytes = i;
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Translhextion::insertfile()
{
	char cFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	cFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Open File to Insert";
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		SetCursor( LoadCursor( NULL, IDC_WAIT ) );
		int fhandle;
		if((fhandle=_open(cFileName,_O_RDONLY|_O_BINARY))!=-1)
		{
			int inslen;
			if((inslen=_filelength (fhandle))!= -1)
			{
				if(inslen){
					unsigned char* insbuf=new unsigned char[inslen];
					if(insbuf!=NULL){
						if (_read (fhandle, insbuf, inslen)!=-1)
						{
							if(!FileBuffer.InsertExpandOn(iCurrentByte,insbuf,0,inslen))
							{
								SetCursor( LoadCursor( NULL, IDC_ARROW ) );
								MessageBox(hwnd,"Could not insert data","Insert file error:",MB_OK | MB_ICONERROR);
							}
							else
							{
								long m = iCurrentByte;
								for(m = m;m<iCurrentByte + inslen;m++)
									vChanges.insert(COLORCHANGE,m);
								iFileChanged = true; bFilestatusChanged = true; 
									SetCursor( LoadCursor( NULL, IDC_ARROW ) );repaint();
							}
						}
						else 
						{
								SetCursor( LoadCursor( NULL, IDC_ARROW ) );
							MessageBox(hwnd,"Could not read data from file","Insert file error:",MB_OK | MB_ICONERROR);
						}
						delete []insbuf;
					}
					else 
					{
							SetCursor( LoadCursor( NULL, IDC_ARROW ) );
						MessageBox(hwnd,"Not enough memory to read data from file","Insert file error:",MB_OK | MB_ICONERROR);
					}
				}
			}
			else MessageBox(hwnd,"Error checking file size.","Insert file error:",MB_OK | MB_ICONERROR);
			{
					SetCursor( LoadCursor( NULL, IDC_ARROW ) );
				_close (fhandle);
			}
		}
		else MessageBox(hwnd,"Error opening file.","Insert file error:",MB_OK | MB_ICONERROR);
		{
			SetCursor( LoadCursor( NULL, IDC_ARROW ) );
		}
	}
}
//SEARCH

int find_bytes (char* ps, int ls, char* pb, int lb, int mode, char (*cmp) (char))
{
	int start, inc, end, i;
	if (mode == 1)
	{
		start = 0;
		inc = 1;
		end = ls - lb + 1;
	}
	else
	{
		start = ls - lb;
		inc = -1;
		end = 1;
	}

	for (; mode * start < end; start += inc)
	{
		for (i = start; i < start + lb; i++)
		{
			if (cmp (ps[i]) != cmp (pb[i - start]))
				break;
		}
		if (i == start + lb)
			return start;
	}

	return -1;
}

int find_bytes_relatively (char* ps, int ls, vector<relation>& offsets, int findstringlength, int mode)
{

	int start, inc, end;
	int match = 0;
	if (mode == 1)
	{
		start = 0;
		inc = 1;
		end = ls - findstringlength + 1;
	}
	else
	{
		start = ls - findstringlength;
		inc = -1;
		end = 1;
	}
	for (; mode * start < end; start += inc)
	{
		int k = 0;
		match = 0;
		for(k = 0;k<offsets.size();k++)
		{
			iProgressPos++;
			if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
			{
				StepProgress(progress1);
			}
			if(offsets[k].op == "=" && (ps[start + offsets[k].related] - ps[start + offsets[k].relate]) == offsets[k].offset)
			{
				match++;
			}
			else if(offsets[k].op == "<" && 
				(ps[start + offsets[k].relate] < ps[start + offsets[k].related] - offsets[k].offset
				|| ps[start + offsets[k].relate] > ps[start + offsets[k].related] + (25 - offsets[k].offset)))
			{
				match++;
			}
			else
			{
				break;
			}
		}
		if (match == offsets.size())
			return start;
	}
	return -1;
}

string Translhextion::create_tablefind_string(string x)
{
	vector<string> phrase;
	int m = 0;
	FileVector<unsigned char> bcphrase;
	phrase.push_back(x);
	if(bOptimizeBytes)
	{
		bool oldbmark = bByteMark;
		bByteMark = false;
		descript(phrase);
		bByteMark = oldbmark;
	}
	else
	{
		bool oldbmark = bByteMark;
		bByteMark = true;
		descript(phrase);
		bByteMark = oldbmark;
	}
	detranslate(phrase);
	x = "";
	for(m = 0;m<phrase.size();m++)
	{
		x += char_hex(phrase[m]);
	}
	bcphrase.SetLength(phrase.size());
	bcphrase.SetUpperBound(phrase.size()-1);
	for(m =0;m<phrase.size();m++)
		bcphrase[m] = char_hex(phrase[m]);
	int buflen = byte_to_bytecode_destinationlength((char*)&bcphrase[0], phrase.size() );
	char* cBuf = new char[buflen];
	translate_bytes_to_bytecode( cBuf, &bcphrase[0], phrase.size() );
	return (string) cBuf;
}

int Translhextion::find ()
{
	if (filename[0] == '\0')
		return 0;

	if( bSelected )
	{
		int sel_start, select_len;
		if( iEndOfSelection < iStartOfSelection )
		{
			sel_start = iEndOfSelection;
			select_len = iStartOfSelection - iEndOfSelection + 1;
		}
		else
		{
			sel_start = iStartOfSelection;
			select_len = iEndOfSelection - iStartOfSelection + 1;
		}
		iFindDialogBufLength = byte_to_bytecode_destinationlength( (char*) &FileBuffer[sel_start], select_len );
		iFindDialogLastLength = iFindDialogBufLength;
		if( pcFindDialogBuffer != NULL )
			delete [] pcFindDialogBuffer;
		pcFindDialogBuffer = new char[iFindDialogBufLength];
		translate_bytes_to_bytecode( pcFindDialogBuffer, &FileBuffer[sel_start], select_len );
	}

	int iLengthgthOfSource = DialogBox (hInstance, MAKEINTRESOURCE (IDD_FINDDIALOG), hwnd, (DLGPROC) FindDialogProc);
	char* pcFindstring;
	if (iLengthgthOfSource != 0)
	{
		int iDestinationLength = bytecode_translate (&pcFindstring, pcFindDialogBuffer, iLengthgthOfSource, iCharacterSet, iBinaryMode);
		if (iDestinationLength > 0)
		{
			int i;
			char (*cmp) (char);

			if (iFindDialogMatchCase == BST_CHECKED)
				cmp = equal;
			else
				cmp = lower_case;

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			if (iFindDialogDirection == 1)
			{
				i = find_bytes ((char*) &(FileBuffer[iCurrentByte + 1]), FileBuffer.Length() - iCurrentByte - 1, pcFindstring, iDestinationLength, 1, cmp);
				if (i != -1)
					iCurrentByte += i + 1;
			}
			else
			{
				i = find_bytes( (char*) &(FileBuffer[0]),
					min( iCurrentByte + (iDestinationLength - 1), FileBuffer.Length() ),
					pcFindstring, iDestinationLength, -1, cmp );
				if (i != -1)
					iCurrentByte = i;
			}
			SetCursor (LoadCursor (NULL, IDC_ARROW));

			if (i != -1)
			{
				bSelected = TRUE;
				iStartOfSelection = iCurrentByte;
				iEndOfSelection = iCurrentByte + iDestinationLength - 1;
				adjust_view_for_selection();

				repaint();
			}
			else
				MessageBox (hwnd, "String not found.", "Notice:", MB_OK | MB_ICONINFORMATION);
			iFindDialogLastLength = iLengthgthOfSource;
			delete [] pcFindstring;
			bLastSearchType = 0;
		}
		else
			MessageBox (hwnd, "Findstring is zero bytes long.", "Find error:", MB_OK | MB_ICONERROR);
	}
	return 1;
}

BOOL CALLBACK FindDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		
		if (iFindDialogBufLength == 0)
		{
			
			iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
			
			if(pcFindDialogBuffer != NULL)
				delete [] pcFindDialogBuffer;
			pcFindDialogBuffer = new char[iFindDialogBufLength];
		}
		SetFocus(hDialog);
		if(bSearchHex)
		{
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), sHexSearchString.c_str());
		}
		else if (iFindDialogLastLength != 0)
		{
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), pcFindDialogBuffer);
		}
		CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
		if(bSearchHex)
		{
			CheckDlgButton(hDialog,IDC_RADIO5, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(hDialog,IDC_RADIO4, BST_CHECKED);
		}
		return FALSE;

	case WM_COMMAND:
		iFindDialogMatchCase = IsDlgButtonChecked (hDialog, IDC_CHECK1);
		if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
			iFindDialogDirection = -1;
		else
			iFindDialogDirection = 1;
		switch (LOWORD (wParam))
		{
		case IDOK:
			
			{	
				if(IsDlgButtonChecked (hDialog, IDC_RADIO4) == BST_CHECKED)
				{
					bSearchHex = false;
				}
				else
				{
					bSearchHex = true;
				}
				iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
				if(pcFindDialogBuffer != NULL)
					delete [] pcFindDialogBuffer;
				pcFindDialogBuffer = new char[iFindDialogBufLength];
				int result = GetDlgItemText (hDialog, IDC_EDIT1, pcFindDialogBuffer, iFindDialogBufLength);
				if(bSearchHex)
				{
					string temp = (string) pcFindDialogBuffer;
					if(temp.length() % 2 != 0)
					{
						MessageBox(hDialog,"Hex string is not recognized.", "Find error:", MB_OK | MB_ICONERROR);
						return TRUE;
					}
					else
					{
						string temp2;
						if(!is_hex(temp))
						{
							MessageBox(hDialog,"Hex string is not recognized.", "Find error:", MB_OK | MB_ICONERROR);
							return TRUE;
						}
						int i = 0;
						for(i = 0;i<temp.length();i+=2)
						{
							temp2 += char_hex(temp.substr(i,2));
						}
						result = temp2.length();
						strcpy(pcFindDialogBuffer,temp2.c_str());
						sHexSearchString = temp;
						EndDialog (hDialog,result);
						return TRUE;
					}
				}
				EndDialog (hDialog,result);
				return TRUE;
				
			}
		case IDCANCEL:
			
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::find_with_table()
{
	if (filename[0] == '\0')
		return 0;

	int iLengthgthOfSource = DialogBox (hInstance, MAKEINTRESOURCE (IDD_TABLEFINDDIALOG), hwnd, (DLGPROC) FindWithTableDialogProc);
	if (iLengthgthOfSource != 0)
	{
		
		sLastTableSearch = (string) pcFindDialogBuffer;
		string temp = create_tablefind_string((string) pcFindDialogBuffer);
		strcpy(pcFindDialogBuffer,temp.c_str());
		iFindDialogLastLength = iLengthgthOfSource;
		bLastSearchType = 0;
		if(iFindDialogDirection == -1)
		{
			bFirstSearch = true;
			findprev();
			bFirstSearch = false;
		}
		else if(iFindDialogDirection == 1)
		{
			bFirstSearch = true;
			findnext();		
			bFirstSearch = false;
		}
	}
	return 1;
}


BOOL CALLBACK FindWithTableDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		
		if(sLastTableSearch != "")
		{
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), sLastTableSearch.c_str());
		}
		bOptimizeBytes = true;
		if(bOptimizeBytes)
		{
			CheckDlgButton(hDialog,IDC_CHECK4,BST_CHECKED);
		}
		if (iFindDialogBufLength == 0)
		{
			
			iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
			
			if(pcFindDialogBuffer != NULL)
				delete [] pcFindDialogBuffer;
			pcFindDialogBuffer = new char[iFindDialogBufLength];
		}
		SetFocus(hDialog);
		CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
		return FALSE;

	case WM_COMMAND:
		if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
			iFindDialogDirection = -1;
		else
			iFindDialogDirection = 1;

		switch (LOWORD (wParam))
		{
		case IDOK:
			{
			
			if(IsDlgButtonChecked(hDialog,IDC_CHECK4))
			{
				bOptimizeBytes = true;
			}
			else
			{
				bOptimizeBytes = false;
			}
			iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
			if(pcFindDialogBuffer != NULL)
				delete [] pcFindDialogBuffer;
			pcFindDialogBuffer = new char[iFindDialogBufLength];
			int result = GetDlgItemText (hDialog, IDC_EDIT1, pcFindDialogBuffer, iFindDialogBufLength);
			EndDialog (hDialog,result);
			return TRUE;
			}
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::findnext()
{
	if( pcFindDialogBuffer != NULL )
	{
		char* pcFindstring;
		int iLengthgthOfSource = strlen( pcFindDialogBuffer );
		int iDestinationLength = bytecode_translate( &pcFindstring, pcFindDialogBuffer, iLengthgthOfSource, iCharacterSet, iBinaryMode );
		if( iDestinationLength > 0 )
		{
			int i;
			char (*cmp) (char);

			if( iFindDialogMatchCase == BST_CHECKED )
				cmp = equal;
			else
				cmp = lower_case;

			SetCursor( LoadCursor( NULL, IDC_WAIT ) );

			i = find_bytes( (char*) &(FileBuffer[iCurrentByte + 1]), FileBuffer.Length () - iCurrentByte - 1, pcFindstring, iDestinationLength, 1, cmp );
			if( i != -1 )
				iCurrentByte += i + 1;

			SetCursor( LoadCursor( NULL, IDC_ARROW ) );
			if( i != -1 )
			{
				bSelected = TRUE;
				iStartOfSelection = iCurrentByte;
				iEndOfSelection = iCurrentByte + iDestinationLength - 1;
				adjust_view_for_selection();
				repaint();
			}
			else
			{
				if(bFirstSearch)
				{
					MessageBox (hwnd, "String not found.", "Notice:", MB_OK | MB_ICONINFORMATION);
					bFirstSearch = false;
				}
				else
				{
					MessageBox( hwnd, "No more occurrences found.", "Notice:", MB_OK | MB_ICONINFORMATION );
				}
			}
			delete [] pcFindstring;
		}
		else
			MessageBox( hwnd, "Findstring is zero bytes long.", "Find next error:", MB_OK | MB_ICONERROR );
	}
	else
	{
		MessageBox( hwnd, "String to find not specified.", "Find next error:", MB_OK | MB_ICONERROR );
	}
	return 0;
}

int Translhextion::findprev()
{
	if( pcFindDialogBuffer != NULL )
	{
		char* pcFindstring;
		int iLengthgthOfSource = strlen( pcFindDialogBuffer );
		int iDestinationLength = bytecode_translate( &pcFindstring, pcFindDialogBuffer, iLengthgthOfSource, iCharacterSet, iBinaryMode );
		if( iDestinationLength > 0 )
		{
			int i;
			char (*cmp) (char);

			if( iFindDialogMatchCase == BST_CHECKED )
				cmp = equal;
			else
				cmp = lower_case;

			SetCursor( LoadCursor( NULL, IDC_WAIT ) );
			{
				i = find_bytes( (char*) &(FileBuffer[0]),
					min( iCurrentByte + (iDestinationLength - 1), FileBuffer.Length() ),
					pcFindstring, iDestinationLength, -1, cmp );
				if (i != -1)
					iCurrentByte = i;
			}
			SetCursor( LoadCursor( NULL, IDC_ARROW ) );
			if( i != -1 )
			{
				bSelected = TRUE;
				iStartOfSelection = iCurrentByte;
				iEndOfSelection = iCurrentByte + iDestinationLength - 1;
				adjust_view_for_selection();
				repaint();
			}
			else
			{
				if(bFirstSearch)
				{
					MessageBox (hwnd, "String not found.", "Notice:", MB_OK | MB_ICONINFORMATION);
					bFirstSearch = false;
				}
				else
				{
					MessageBox( hwnd, "No more occurrences found.", "Notice:", MB_OK | MB_ICONINFORMATION );
				}
			}
			delete [] pcFindstring;
		}
		else
			MessageBox( hwnd, "Findstring is zero bytes long.", "Find previous error:", MB_OK | MB_ICONERROR );
	}
	else
	{
		MessageBox( hwnd, "String to find not specified.", "Find previous error:", MB_OK | MB_ICONERROR );
	}
	return 0;
}

int Translhextion::findnext_relative()
{
	if( vRelativeOffsets.size() > 0 )
	{
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		int i = find_bytes_relatively ((char*) &(FileBuffer[iCurrentByte + 1]), FileBuffer.Length() - iCurrentByte - 1, vRelativeOffsets,sRelSearchString.length(), 1);
		if (i != -1)
			iCurrentByte += i + 1;
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		if (i != -1)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			iEndOfSelection = iCurrentByte + sRelSearchString.length() - 1;
			adjust_view_for_selection();
			repaint();
		}
		else
		{
			if(!bFileRelativeResults)
			{
				MessageBox (hwnd, "No more relative occurrences found.", "Notice:", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				bRelativeSearchDone = true;
			}
		}
	}
	else
	{
		if(bFileRelativeResults)
		{
			bRelativeSearchDone = true;
		}
		else
		{
			MessageBox( hwnd, "String to find not specified.", "Find next relatively error:", MB_OK | MB_ICONERROR );
		}
	}
	return 0;
}

int Translhextion::findprev_relative()
{
	if( vRelativeOffsets.size() > 0 )
	{
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		int i = find_bytes_relatively( (char*) &(FileBuffer[0]),
			min( iCurrentByte + (iFindDialogLastLength - 1), FileBuffer.Length() ),
			vRelativeOffsets,sRelSearchString.length(), -1);
		if (i != -1)
			iCurrentByte = i;
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		if (i != -1)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			iEndOfSelection = iCurrentByte + sRelSearchString.length() - 1;
			adjust_view_for_selection();
			repaint();
		}
		else
		{
			if(!bFileRelativeResults)
			{
				MessageBox (hwnd, "No more relative occurrences found.", "Notice:", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				bRelativeSearchDone = true;
			}
		}
	}
	else
	{
		if(bFileRelativeResults)
		{
			bRelativeSearchDone = true;
		}
		else
		{
			MessageBox( hwnd, "String to find not specified.", "Find previous relatively:", MB_OK | MB_ICONERROR );
		}
	}
	return 0;
}

void Translhextion::replace()
{
	if( bSelected )
	{
		int sel_start, select_len;
		if( iEndOfSelection < iStartOfSelection )
		{
			sel_start = iEndOfSelection;
			select_len = iStartOfSelection - iEndOfSelection + 1;
		}
		else
		{
			sel_start = iStartOfSelection;
			select_len = iEndOfSelection - iStartOfSelection + 1;
		}

		if( translate_binary_to_text( strToReplaceData, (char*) &FileBuffer[sel_start], select_len ) )
		{
		}
		else
		{
			MessageBox( hwnd, "Could not use selection as replace target.", "Notice:", MB_OK | MB_ICONINFORMATION);
			return;
		}
	}

	bDontMarkCurrentPos = TRUE;
	if( DialogBox( hInstance, MAKEINTRESOURCE(IDD_REPLACEDIALOG), hwnd, (DLGPROC) ReplaceDialogProc ) )
	{
		if(bReplaceFileChange)
		{
			iFileChanged = true;
			bReplaceFileChange = false;
		}
	}
	bDontMarkCurrentPos = FALSE;
}

BOOL CALLBACK ReplaceDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		if( iPasteAsText )
			CheckDlgButton (hDialog, IDC_USETRANSLATION_CHECK, BST_UNCHECKED);
		else
			CheckDlgButton (hDialog, IDC_USETRANSLATION_CHECK, BST_CHECKED);
		if( strToReplaceData.length() != 0 )
			SetWindowText( GetDlgItem ( hDialog, IDC_TO_REPLACE_EDIT ), strToReplaceData.c_str() );
		SetFocus(hDialog);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			EndDialog (hDialog, 0);
			return TRUE;

		case IDCANCEL:
			{
				if( IsDlgButtonChecked( hDialog, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;
				EndDialog (hDialog, 0);
				return TRUE;
			}

		case IDC_FINDPREVIOUS_BUTTON:
			{
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDialog, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				strToReplaceData = cBuf;
				if( ! mainwindow.find_and_select_data( strToReplaceData, -1, TRUE, cmp ) )
					MessageBox( hDialog, "String not found.", "Notice:", MB_OK | MB_ICONINFORMATION );
				delete [] cBuf;
			}
			break;

		case IDC_FINDNEXT_BUTTON:
			{
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDialog, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				strToReplaceData = cBuf;
				if( ! mainwindow.find_and_select_data( strToReplaceData, 1, TRUE, cmp ) )
				{
					MessageBox( hDialog, "String not found.", "Notice:", MB_OK | MB_ICONINFORMATION );
					break;
				}
				delete [] cBuf;
			}
			break;

		case IDC_FOLLOCC_BUTTON:
			{
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDialog, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				
				int occ_num = 0;

				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace.", "Replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strToReplaceData = cBuf;
				delete [] cBuf;

				buflen = SendMessage( GetDlgItem( hDialog, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
				cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_REPLACEWITH_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace with.", "Replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strReplaceWithData = cBuf;
				delete [] cBuf;

				if( IsDlgButtonChecked( hDialog, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;

				SetCursor( LoadCursor( NULL, IDC_WAIT ) );
				do
				{
					if( ! mainwindow.find_and_select_data( strToReplaceData, 1, FALSE, cmp ) )
					{
						break;
					}
					occ_num++;
					mainwindow.replace_selected_data( strReplaceWithData, FALSE );
				} while( TRUE );
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );

				char tbuf[32];
				sprintf( tbuf, "%d occurrences replaced.", occ_num );
				MessageBox( hDialog, tbuf, "Notice:", MB_OK | MB_ICONINFORMATION );
				bReplaceFileChange = true;
				mainwindow.update_window_status();
				mainwindow.adjust_view_for_selection();
				mainwindow.update_size_change();
				mainwindow.repaint();
			}
			break;

		case IDC_PREVOCC_BUTTON:
			{
				
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDialog, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				int occ_num = 0;

				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace.", "Replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strToReplaceData = cBuf;
				delete [] cBuf;

				buflen = SendMessage( GetDlgItem( hDialog, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
				cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_REPLACEWITH_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace with.", "Replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strReplaceWithData = cBuf;
				delete [] cBuf;

				if( IsDlgButtonChecked( hDialog, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;

				SetCursor( LoadCursor( NULL, IDC_WAIT ) );
				do
				{
					if( ! mainwindow.find_and_select_data( strToReplaceData, -1, FALSE, cmp ) )
					{
						break;
					}
					occ_num++;
					mainwindow.replace_selected_data( strReplaceWithData, FALSE );
				} while( TRUE );
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );

				char tbuf[32];
				sprintf( tbuf, "%d occurrences replaced.", occ_num );
				MessageBox( hDialog, tbuf, "Notice:", MB_OK | MB_ICONINFORMATION );
				bReplaceFileChange = true;
				mainwindow.update_window_status();
				mainwindow.adjust_view_for_selection();
				mainwindow.update_size_change();
				mainwindow.repaint();
			}
			break;

		case IDC_REPLACE_BUTTON:
			{
				if( IsDlgButtonChecked( hDialog, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;
				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				if( cBuf != NULL )
				{
					if( GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen ) != 0)
					{
						strToReplaceData = cBuf;
						delete [] cBuf;
						if( (char*) strToReplaceData.c_str() != NULL )
						{
							buflen = SendMessage( GetDlgItem( hDialog, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
							cBuf = new char[ buflen ];
							GetDlgItemText( hDialog, IDC_REPLACEWITH_EDIT, cBuf, buflen );
							strReplaceWithData = cBuf;
							delete [] cBuf;
							mainwindow.replace_selected_data( strReplaceWithData );
							bReplaceFileChange = true;
						}
						else
						{
							MessageBox( hDialog, "Could not store data to replace.", "Replace error:", MB_OK | MB_ICONERROR );
						}
						break;
					}
					else
					{
						delete [] cBuf;
						MessageBox( hDialog, "Could not read out data to replace.", "Replace error:", MB_OK | MB_ICONERROR );
					}
				}
				else
				{
					MessageBox( hDialog, "Could not allocate memory for data to replace.", "Replace error:", MB_OK | MB_ICONERROR );
				}
			}
			break;
		}
		break;
	}
	return FALSE;
}

void Translhextion::replace_with_table()
{
	bDontMarkCurrentPos = TRUE;
	if( DialogBox( hInstance, MAKEINTRESOURCE(IDD_TABLEREPLACEDIALOG), hwnd, (DLGPROC) ReplaceWithTableDialogProc ) )
	{
		if(bReplaceFileChange)
		{
			iFileChanged = true;
			bReplaceFileChange = false;
		}
	}
	bDontMarkCurrentPos = FALSE;
}


BOOL CALLBACK ReplaceWithTableDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		bOptimizeBytes = true;
		if(bOptimizeBytes)
		{
			CheckDlgButton(hDialog,IDC_CHECK4,BST_CHECKED);
		}
		if(sLastTableReplace != "" )
		{
			SetWindowText( GetDlgItem ( hDialog, IDC_TO_REPLACE_EDIT ), sLastTableReplace.c_str() );
		}
		SetFocus(hDialog);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				
			EndDialog (hDialog, 0);
			return TRUE;
			}
		case IDCANCEL:
			{
				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				sLastTableReplace = (string) cBuf;
				delete [] cBuf;
				EndDialog (hDialog, 0);
				return TRUE;
			}

		case IDC_FINDPREVIOUS_BUTTON:
			{
				if(IsDlgButtonChecked(hDialog,IDC_CHECK4))
				{
					bOptimizeBytes = true;
				}
				else
				{
					bOptimizeBytes = false;
				}
				char (*cmp) (char);
				cmp = equal;
				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				strToReplaceData = cBuf;
				
				string temp = mainwindow.create_tablefind_string((string) strToReplaceData);
				strToReplaceData = temp;
				if( ! mainwindow.find_and_select_data( strToReplaceData, -1, TRUE, cmp ) )
					MessageBox( hDialog, "String not found.", "Notice:", MB_OK | MB_ICONINFORMATION );
				delete [] cBuf;
			}
			break;

		case IDC_FINDNEXT_BUTTON:
			{
				if(IsDlgButtonChecked(hDialog,IDC_CHECK4))
				{
					bOptimizeBytes = true;
				}
				else
				{
					bOptimizeBytes = false;
				}
				char (*cmp) (char);
				cmp = equal;
				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				strToReplaceData = cBuf;
				
				string temp = mainwindow.create_tablefind_string((string) strToReplaceData);
				strToReplaceData = temp.c_str();
				if( ! mainwindow.find_and_select_data( strToReplaceData, 1, TRUE, cmp ) )
				{
					MessageBox( hDialog, "String not found.", "Notice:", MB_OK | MB_ICONINFORMATION );
					break;
				}
				delete [] cBuf;
			}
			break;

		case IDC_FOLLOCC_BUTTON:
			{
				char (*cmp) (char);
				cmp = equal;
				
				int occ_num = 0;

				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace.", "Table replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strToReplaceData = cBuf;
				string temp = mainwindow.create_tablefind_string((string) strToReplaceData);
				strToReplaceData = temp.c_str();
				delete [] cBuf;

				buflen = SendMessage( GetDlgItem( hDialog, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
				cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_REPLACEWITH_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace with.", "Table replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strReplaceWithData = cBuf;
				temp = mainwindow.create_tablefind_string((string) strReplaceWithData);
				strReplaceWithData = temp.c_str();
				delete [] cBuf;

				iPasteAsText = FALSE;
				SetCursor( LoadCursor( NULL, IDC_WAIT ) );
				do
				{
					if( ! mainwindow.find_and_select_data( strToReplaceData, 1, FALSE, cmp ) )
					{
						break;
					}
					occ_num++;
					mainwindow.replace_selected_data( strReplaceWithData, FALSE );
				} while( TRUE );
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );

				char tbuf[32];
				sprintf( tbuf, "%d occurrences replaced.", occ_num );
				MessageBox( hDialog, tbuf, "Notice:", MB_OK | MB_ICONINFORMATION );
				bReplaceFileChange = TRUE;
				mainwindow.update_window_status();
				mainwindow.adjust_view_for_selection();
				mainwindow.update_size_change();
				mainwindow.repaint();
			}
			break;

		case IDC_PREVOCC_BUTTON:
			{
				
				char (*cmp) (char);
				cmp = equal;
				int occ_num = 0;

				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace.", "Table replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strToReplaceData = cBuf;
				string temp = mainwindow.create_tablefind_string((string) strToReplaceData);
				strToReplaceData = temp;
				delete [] cBuf;

				buflen = SendMessage( GetDlgItem( hDialog, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
				cBuf = new char[ buflen ];
				if( cBuf != NULL )
					GetDlgItemText( hDialog, IDC_REPLACEWITH_EDIT, cBuf, buflen );
				else
				{
					MessageBox( hDialog, "Could not store data to replace with.", "Table replace all error:", MB_OK | MB_ICONERROR );
					break;
				}
				strReplaceWithData = cBuf;
				temp = mainwindow.create_tablefind_string((string) strReplaceWithData);
				strReplaceWithData = temp;
				delete [] cBuf;

				iPasteAsText = FALSE;

				SetCursor( LoadCursor( NULL, IDC_WAIT ) );
				do
				{
					if( ! mainwindow.find_and_select_data( strToReplaceData, -1, FALSE, cmp ) )
					{
						break;
					}
					occ_num++;
					mainwindow.replace_selected_data( strReplaceWithData, FALSE );
				} while( TRUE );
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );

				char tbuf[32];
				sprintf( tbuf, "%d occurrences replaced.", occ_num );
				MessageBox( hDialog, tbuf, "Notice:", MB_OK | MB_ICONINFORMATION );
				bReplaceFileChange = TRUE;
				mainwindow.update_window_status();
				mainwindow.adjust_view_for_selection();
				mainwindow.update_size_change();
				mainwindow.repaint();
			}
			break;

		case IDC_REPLACE_BUTTON:
			{
				iPasteAsText = FALSE;
				int buflen = SendMessage( GetDlgItem (hDialog, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* cBuf = new char[ buflen ];
				if( cBuf != NULL )
				{
					if( GetDlgItemText( hDialog, IDC_TO_REPLACE_EDIT, cBuf, buflen ) != 0)
					{
						strToReplaceData = cBuf;
						string temp = mainwindow.create_tablefind_string((string) strToReplaceData);
						strToReplaceData = temp;
						delete [] cBuf;
						if( (char*) strToReplaceData.c_str() != NULL )
						{
							buflen = SendMessage( GetDlgItem( hDialog, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
							cBuf = new char[ buflen ];
							GetDlgItemText( hDialog, IDC_REPLACEWITH_EDIT, cBuf, buflen );
							strReplaceWithData = cBuf;
							string temp = mainwindow.create_tablefind_string((string) strReplaceWithData);
							strReplaceWithData = temp;
							delete [] cBuf;
							mainwindow.replace_selected_data( strReplaceWithData );
							bReplaceFileChange = TRUE;
						}
						else
						{
							MessageBox( hDialog, "Could not store data to replace.", "Table replace error:", MB_OK | MB_ICONERROR );
						}
						break;
					}
					else
					{
						delete [] cBuf;
						MessageBox( hDialog, "Could not read out data to replace.", "Table replace error:", MB_OK | MB_ICONERROR );
					}
				}
				else
				{
					MessageBox( hDialog, "Could not allocate memory for data to replace.", "Table replace error:", MB_OK | MB_ICONERROR );
				}
			}
			break;
		}
		break;
	}
	return FALSE;
}
void create_relative_search_string()
{
	
	string temp = (string) pcFindDialogBuffer;
	sRelSearchString = temp;
	vector<relation> emp;
	vRelativeOffsets = emp;
	relation a;
	vector<int> caps;
	vector<int> nums;
	vector<int> lows;
	SetCursor (LoadCursor (NULL, IDC_WAIT));
	int i = 0;
	for(i = 0;i<temp.length();i++)
	{
		if(temp[i] >= 'A' && temp[i] <= 'Z')
			caps.push_back(i);
		else if(temp[i] >= '1' && temp[i] <= '9')
			nums.push_back(i);
		else if(temp[i] >= 'a' && temp[i] <= 'z')
			lows.push_back(i);
	}
	for(i = 0;i<caps.size()-1;i++)
	{
		a.relate = caps[i];
		a.offset = temp[caps[i+1]] - temp[caps[i]];
		a.op = "=";
		a.related = caps[i+1];
		vRelativeOffsets.push_back(a);
	}
	for(i = 0;i<lows.size()-1;i++)
	{
		a.relate = lows[i];
		a.offset = temp[lows[i+1]] - temp[lows[i]];
		a.op = "=";
		a.related = lows[i+1];
		vRelativeOffsets.push_back(a);
	}
	for(i = 0;i<nums.size()-1;i++)
	{
		a.relate = nums[i];
		a.offset = temp[nums[i+1]] - temp[nums[i]];
		a.op = "=";
		a.related = nums[i+1];
		vRelativeOffsets.push_back(a);
	}
	if(lows.size() > 0 && caps.size() > 0)
	{
		a.relate = caps[0];
		a.offset = temp[lows[0]] - 'a';
		a.op = "<";
		a.related = lows[0];
		vRelativeOffsets.push_back(a);
	}
	if(lows.size() > 0 && nums.size() > 0)
	{
		a.relate = nums[0];
		a.offset = temp[lows[0]] - 'a';
		a.op = "<";
		a.related = lows[0];
		vRelativeOffsets.push_back(a);
	}
	if(caps.size() > 0 && nums.size() > 0)
	{
		a.relate = nums[0];
		a.offset = temp[caps[0]] - 'A';
		a.op = "<";
		a.related = caps[0];
		vRelativeOffsets.push_back(a);
	}
	caps = viEmpty;
	lows = viEmpty;
	caps = viEmpty;
}

relativescandata Translhextion::guess_table_values(long start)
{
	int i = 0;
	int cap = -1;
	int low = -1;
	int num = -1;
	RelativeOffsets result;
	result.bA = false;
	result.ba = false;
	result.bn = false;
	string sResult;
	relativescandata result2;
	result2.start = start;
	result2.end = start + sRelSearchString.length();
	result2.search = sRelSearchString;
	for(i = 0;i<sRelSearchString.length();i++)
	{
		if(sRelSearchString[i] >= 'A' && sRelSearchString[i] <= 'Z')
		{	
			cap = i;
			result.A = FileBuffer[start + cap] - (sRelSearchString[i] - 'A');
			result.bA = true;
			break;
		}
	}
	for(i = 0;i<sRelSearchString.length();i++)
	{
		if(sRelSearchString[i] >= '1' && sRelSearchString[i] <= '9')
		{	
			num = i;
			result.n = FileBuffer[start + num] - (sRelSearchString[i] - '1');
			result.bn = true;
			break;
		}
	}
	for(i = 0;i<sRelSearchString.length();i++)
	{
		if(sRelSearchString[i] >= 'a' && sRelSearchString[i] <= 'z')
		{	
			low = i;
			result.a = FileBuffer[start + low] - (sRelSearchString[i] - 'a');
			result.ba = true;
			break;
		}
	}
	sResult += "A=";
	if(result.bA)
	{
		result2.A = hex_char(result.A);
		sResult += hex_char(result.A) + " ";
	}
	else
	{
		result2.A = "??";
		sResult += "?? ";
	}
	sResult += "a=";
	if(result.ba)
	{
		result2.a = hex_char(result.a);
		sResult += hex_char(result.a) + " ";
	}
	else
	{
		result2.a = "??";
		sResult += "?? ";
	}
	sResult += "1=";
	if(result.bn)
	{
		result2.n = hex_char(result.n);
		sResult += hex_char(result.n);
	}
	else
	{
		result2.n = "??";
		sResult += "??";
	}
	result2.listitem = sResult;
	return result2;
}


int Translhextion::search_relative()
{
	if (filename[0] == '\0')
		return 0;
	int i = 0;
	int iLengthgthOfSource = DialogBox (hInstance, MAKEINTRESOURCE (IDD_SEARCHRELATIVEDIALOG), hwnd, (DLGPROC) SearchRelativeDialogProc);
	if (iLengthgthOfSource > 1)
	{
		create_relative_search_string();
		
		if (iFindDialogDirection == 1)
		{
			i = find_bytes_relatively ((char*) &(FileBuffer[iCurrentByte + 1]), FileBuffer.Length() - iCurrentByte - 1, vRelativeOffsets,sRelSearchString.length(), 1);
			if (i != -1)
				iCurrentByte += i + 1;
		}
		
		else
		{
			i = find_bytes_relatively( (char*) &(FileBuffer[0]),
				min( iCurrentByte + (iLengthgthOfSource - 1), FileBuffer.Length() ),
				vRelativeOffsets,sRelSearchString.length(), -1);
			if (i != -1)
				iCurrentByte = i;
		}
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		if (i != -1)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurrentByte;
			
			iEndOfSelection = iCurrentByte + sRelSearchString.length() - 1;
			adjust_view_for_selection();
			repaint();
		}
		else
		{
			MessageBox (hwnd, "Relative string not found.", "Notice:", MB_OK | MB_ICONINFORMATION);
		}
		iFindDialogLastLength = iLengthgthOfSource;
		bLastSearchType = 1;
		
	}
	else if(iLengthgthOfSource == 1)
	{
		MessageBox(hwnd,"Relative search string must be at least 2 characters long!","Notice:",MB_OK | MB_ICONINFORMATION);
	}
	else if(iLengthgthOfSource == -5)
	{
		;
	}
	else
	{
		
		MessageBox( hwnd, "String to find not specified.", "Search Relative error:", MB_OK | MB_ICONERROR );
	}
	return 1;
}

BOOL CALLBACK SearchRelativeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		
		if (iFindDialogBufLength == 0)
		{
			
			iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
			
			if(pcFindDialogBuffer != NULL)
				delete [] pcFindDialogBuffer;
			pcFindDialogBuffer = new char[iFindDialogBufLength];
		}
		SetFocus(hDialog);
		
		if (iFindDialogLastLength != 0 && bLastSearchType == 1)
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), pcFindDialogBuffer);
		CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
		return FALSE;

	case WM_COMMAND:
		if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
			iFindDialogDirection = -1;
		else
			iFindDialogDirection = 1;

		switch (LOWORD (wParam))
		{
		case IDOK:
			{
			
			if(IsDlgButtonChecked(hDialog,IDC_CHECK1))
				bFileRelativeResults = true;
			iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
			if(pcFindDialogBuffer != NULL)
				delete [] pcFindDialogBuffer;
			pcFindDialogBuffer = new char[iFindDialogBufLength];
			int result = GetDlgItemText (hDialog, IDC_EDIT1, pcFindDialogBuffer, iFindDialogBufLength);
			EndDialog (hDialog,result);
			return TRUE;
			}
			break;
		case IDCANCEL:
			EndDialog (hDialog, -5);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

long Translhextion::scan_relative_helper(int iLengthgthOfSource)
{
	
	int i = 0;
	if (iFindDialogDirection == 1)
	{
		if(progress1 != NULL)
		{
			SetProgressRange(progress1,iCurrentByte - 1,FileBuffer.Length());
			if(FileBuffer.Length() >= 10000)
			{
				iIncrementStep = FileBuffer.Length() / 1000;
			}
			else if(FileBuffer.Length() >= 1000)//this could be better
			{
				iIncrementStep = FileBuffer.Length() / 100;
			}
			else if(FileBuffer.Length() >= 100)
			{
				iIncrementStep = FileBuffer.Length() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress1,iIncrementStep);
		}
		i = find_bytes_relatively ((char*) &(FileBuffer[iCurrentByte + 1]), FileBuffer.Length() - iCurrentByte - 1, vRelativeOffsets,sRelSearchString.length(),1);
		if (i != -1)
			iCurrentByte += i + 1;
	}
	else
	{
		if(progress1 != NULL)
		{
			SetProgressRange(progress1,0,min( iCurrentByte + (iLengthgthOfSource - 1), FileBuffer.Length()));
			if(FileBuffer.Length() >= 10000)
			{
				iIncrementStep = FileBuffer.Length() / 1000;
			}
			else if(FileBuffer.Length() >= 1000)//this could be better
			{
				iIncrementStep = FileBuffer.Length() / 100;
			}
			else if(FileBuffer.Length() >= 100)
			{
				iIncrementStep = FileBuffer.Length() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress1,iIncrementStep);
		}
		i = find_bytes_relatively( (char*) &(FileBuffer[0]),
			min( iCurrentByte + (iLengthgthOfSource - 1), FileBuffer.Length() ),
			vRelativeOffsets,sRelSearchString.length(), -1);
		if (i != -1)
			iCurrentByte = i;
	}
	if (i != -1)
	{
		bSelected = TRUE;
		iStartOfSelection = iCurrentByte;
		iEndOfSelection = iCurrentByte + sRelSearchString.length() - 1;
		mainwindow.adjust_view_for_selection();
		mainwindow.repaint();
	}
	else
	{
		bRelativeSearchDone = true;
	}
	bLastSearchType = 1;
	return iCurrentByte;
}

void Translhextion::save_relative_results(fstream& fs)
{
	char cBuf[512];
	string output;
	int index = 0;
	for(index = 0; index < vScanData.size();index++)
	{
		if(iOffsetMode == LOROM)
			sprintf (cBuf, "%d=0x%X\t", vScanData[index].start, toLOROM(vScanData[index].start));
		else if(iOffsetMode == HIROM)
			sprintf (cBuf, "%d=0x%X\t", vScanData[index].start, toHIROM(vScanData[index].start));
		else
			sprintf (cBuf, "%d=0x%X\t", vScanData[index].start, vScanData[index].start);
		fs << "Offset: \t\t" << (string) cBuf << endl;
		int k = 0;
		for(k = 0;k<sRelSearchString.length();k++)
		{
			output += hex_char(FileBuffer[vScanData[index].start + k]) + " ";
		}
		fs << "Hex Data: \t" << output << endl;
		output = "";
		for(k = 0;k<sRelSearchString.length();k++)
		{
			output += FileBuffer[vScanData[index].start + k];
		}
		fs << "ASCII Data: \t" << output << endl;
		fs << "Relative Offsets: \t" << vScanData[index].listitem << endl << endl;
		output = "";
	}
}

int Translhextion::scan_relative()
{
	if (filename[0] == '\0')
		return 0;
	int i = 0;
	int iLengthgthOfSource = DialogBox (hInstance, MAKEINTRESOURCE (IDD_SCANRELATIVEDIALOG), hwnd, (DLGPROC) ScanRelativeDialogProc);
	if (iLengthgthOfSource != 0)
	{
		
	}
	return 1;
}

BOOL CALLBACK ScanRelativeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		if (iFindDialogBufLength == 0)
		{
			
			iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
			
			if(pcFindDialogBuffer != NULL)
				delete [] pcFindDialogBuffer;
			pcFindDialogBuffer = new char[iFindDialogBufLength];
		}
		SetFocus(GetDlgItem (hDialog, IDC_EDIT1));
		
		if (iFindDialogLastLength != 0 && bLastSearchType == 1)
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), pcFindDialogBuffer);
		CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDSCAN:
			{
				
				
				if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
				{
					iFindDialogDirection = -1;
				}
				else
				{
					iFindDialogDirection = 1;
				}
				progress1 = GetDlgItem(hDialog,IDC_SCANPROGRESS);
				bFileRelativeResults = true;
				SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_RESETCONTENT, 0, 0);
				vector<relativescandata> emp;
				vScanData = emp;
				iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
				if(pcFindDialogBuffer != NULL)
					delete [] pcFindDialogBuffer;
				pcFindDialogBuffer = new char[iFindDialogBufLength];
				int result = GetDlgItemText (hDialog, IDC_EDIT1, pcFindDialogBuffer, iFindDialogBufLength);
				if(result == 1)
				{
					MessageBox(hDialog,"Relative search string must be at least 2 characters long!","Notice:",MB_OK | MB_ICONINFORMATION);
					bFileRelativeResults = false;
					bRelativeSearchDone = false;
					
				}
				else if(result < 1)
				{
					
					MessageBox( hDialog, "String to find not specified.", "Search Relative error:", MB_OK | MB_ICONERROR );
					bFileRelativeResults = false;
					bRelativeSearchDone = false;
					
				}
				else
				{
					create_relative_search_string();
					SetCursor (LoadCursor (NULL, IDC_WAIT));
					iFindDialogLastLength = result;
					mainwindow.scan_relative_helper(result);
					bLastSearchType = 1;
					if(bRelativeSearchDone)
					{
						MessageBox (hDialog, "Relative string not found.", "Notice:", MB_OK | MB_ICONINFORMATION);
						bFileRelativeResults = false;
						bRelativeSearchDone = false;
						bLastSearchType = 1;
					}
					else
					{
						vScanData.push_back(mainwindow.guess_table_values(mainwindow.iCurrentByte));
						int k = 0;
						int l = 20;
						SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM) (makelength(vScanData[k].search,l) + makelength(stringvalue(vScanData[k].start),l) + vScanData[k].listitem).c_str());
						SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_SETCURSEL, k, 0);
						while(!bRelativeSearchDone)
						{
							if (iFindDialogDirection == 1)
							{
								mainwindow.findnext_relative();
							}
							else
							{
								mainwindow.findprev_relative();
							}
							vScanData.push_back(mainwindow.guess_table_values(mainwindow.iCurrentByte));
							k = vScanData.size() - 1;
							SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM) (makelength(vScanData[k].search,l) + makelength(stringvalue(vScanData[k].start),l) + vScanData[k].listitem).c_str());
							SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_SETCURSEL, k, 0);
						}
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						mainwindow.adjust_view_for_selection();
						mainwindow.repaint();
						
						SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_SETCURSEL, 0, 0);
						bFileRelativeResults = false;
						bRelativeSearchDone = false;
						bLastSearchType = 1;
					}
				}
				ResetProgress(progress1);
				iIncrementStep = 0;
				iProgressPos = 0;
				progress1 = NULL;
			}
			break;
		case IDC_GTABLE:
			{
				if(vScanData.size() > 0)
				{
					int index = SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_GETCURSEL, 0, 0);
					char cFileName[_MAX_PATH];
					char szTitleName[_MAX_FNAME + _MAX_EXT];
					OPENFILENAME ofn;
					szTitleName[0] =cFileName[0] = '\0';
					ofn.lStructSize = sizeof (OPENFILENAME);
					ofn.hwndOwner = hDialog;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = "Thingy Table Files (*.TBL)\0*.TBL\0\0";
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter = 0;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = cFileName;
					ofn.nMaxFile = _MAX_PATH;
					ofn.lpstrFileTitle = szTitleName;
					ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = "Save Table As";
					ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
					ofn.nFileOffset = 0;
					ofn.nFileExtension = 0;
					ofn.lpstrDefExt = NULL;
					ofn.lCustData = 0L;
					ofn.lpfnHook = NULL;
					ofn.lpTemplateName = NULL;
					if (GetSaveFileName (&ofn))
					{
						fstream fs;
						int cur = strlen(cFileName) - 4;
						if(uppercase(((string) cFileName).substr(((string) cFileName).length()-4,4)) == ".TBL")
						{
							sFileName = (string) cFileName;
						}
						else
						{
							sFileName = (string) strcat(cFileName,".tbl");
						}
						fs.open(sFileName.c_str(),ios::out);
						if(!fs.fail())
						{
							if(vScanData[index].A != "??")
							{
								int left = char_hex(vScanData[index].A);
								int right = 'A';
								int k = 0;
								for(k = 0;k<26;k++)
								{
									fs << hex_char(left) << "=" << (unsigned char) right << endl;
									left++;
									right++;
								}
							}
							if(vScanData[index].a != "??")
							{
								int left = char_hex(vScanData[index].a);
								int right = 'a';
								int k = 0;
								for(k = 0;k<26;k++)
								{
									fs << hex_char(left) << "=" << (unsigned char) right << endl;
									left++;
									right++;
								}
							}
							if(vScanData[index].n != "??")
							{
								int left = char_hex(vScanData[index].n);
								int right = '1';
								int k = 0;
								for(k = 0;k<9;k++)
								{
									fs << hex_char(left) << "=" << (unsigned char) right << endl;
									left++;
									right++;
								}
							}
							fs.close();
							MessageBox (hDialog,(sFileName + " sucessfully generated.").c_str(), "Table Generation Complete", MB_OK);
						}
						else
						{
							MessageBox (hDialog, "Could not save file.", "Relative Scan error:", MB_OK | MB_ICONERROR);
							fs.close();
						}
					}
				}
				else
				{
					MessageBox(hDialog,"No results to create table from!","Notice:",MB_OK | MB_ICONINFORMATION);
				}
			}
			break;
		case IDC_SAVE:
			{
				if(vScanData.size() > 0)
				{
					char cFileName[_MAX_PATH];
					char szTitleName[_MAX_FNAME + _MAX_EXT];
					OPENFILENAME ofn;
					szTitleName[0] =cFileName[0] = '\0';
					ofn.lStructSize = sizeof (OPENFILENAME);
					ofn.hwndOwner = hDialog;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = "Text Files (*.TXT)\0*.TXT\0\0";
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter = 0;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = cFileName;
					ofn.nMaxFile = _MAX_PATH;
					ofn.lpstrFileTitle = szTitleName;
					ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = "Save Search Results As";
					ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
					ofn.nFileOffset = 0;
					ofn.nFileExtension = 0;
					ofn.lpstrDefExt = NULL;
					ofn.lCustData = 0L;
					ofn.lpfnHook = NULL;
					ofn.lpTemplateName = NULL;
					if (GetSaveFileName (&ofn))
					{
						fstream fs;
						int cur = strlen(cFileName) - 4;
						if(uppercase(((string) cFileName).substr(((string) cFileName).length()-4,4)) == ".TXT")
						{
							sFileName = (string) cFileName;
						}
						else
						{
							sFileName = (string) strcat(cFileName,".txt");
						}
						fs.open(sFileName.c_str(),ios::out);
						if(!fs.fail())
						{
							mainwindow.save_relative_results(fs);
							fs.close();
							MessageBox (hDialog, ("Results saved sucessfully to " + sFileName).c_str(), "Notice:", MB_OK | MB_ICONINFORMATION);
						}
						else
						{
							MessageBox (hDialog, "Could not save file.", "Relative Scan error:", MB_OK | MB_ICONERROR);
							fs.close();
						}
					}
				}
				else
				{
					MessageBox(hDialog,"No results to save to file!","Notice:",MB_OK | MB_ICONINFORMATION);
				}
			}	
			break;
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int Translhextion::value_scan_relative()
{
	if (filename[0] == '\0')
		return 0;
	int i = 0;
	int iLengthgthOfSource = DialogBox (hInstance, MAKEINTRESOURCE (IDD_VALSCANRELATIVEDIALOG), hwnd, (DLGPROC) ValScanRelativeDialogProc);
	if (iLengthgthOfSource != 0)
	{
		
	}
	return 1;
}


BOOL CALLBACK ValScanRelativeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
		
		if (iFindDialogBufLength == 0)
		{
			
			iFindDialogBufLength = SendMessage (GetDlgItem (hDialog, IDC_EDIT2), EM_GETLIMITTEXT, 0, 0);
			
			if(pcFindDialogBuffer != NULL)
				delete [] pcFindDialogBuffer;
			pcFindDialogBuffer = new char[iFindDialogBufLength];
		}
		CheckDlgButton (hDialog, IDC_RADIO3, BST_CHECKED);
		SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_LIMITTEXT, 3, 0);
		vValueList = viEmpty;
		iLastRelativeValue = -1;
		SendMessage (GetDlgItem (hDialog, IDC_COMBO1), CB_INSERTSTRING, 0, (LPARAM) ((string)"Up").c_str());
		SendMessage (GetDlgItem (hDialog, IDC_COMBO1), CB_INSERTSTRING, 0, (LPARAM) ((string)"Down").c_str());
		SendMessage (GetDlgItem (hDialog, IDC_COMBO1), CB_SETCURSEL, 0,0);
		SetFocus(GetDlgItem (hDialog, IDC_RADIO3));
		return FALSE;
		}
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDC_ADD:
			{
			int valuemode = -1;
			int value = -1;
			int change = 0;
			string sign;
			relation a;
			int buflen = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0) + 1;
			char *cBuf = new char[buflen];
			int result = GetDlgItemText (hDialog, IDC_EDIT1, cBuf, buflen);
			if(IsDlgButtonChecked(hDialog,IDC_RADIO3))
			{
				valuemode = 0;
				if(intvalue((string)cBuf) > 255 || intvalue((string)cBuf) < 0)
				{
					MessageBox(hDialog,"Decimal value out of range [0-255]!","Notice:",MB_OK | MB_ICONINFORMATION);
					break;
				}
				value = intvalue((string) cBuf);
			}
			else if(IsDlgButtonChecked(hDialog,IDC_RADIO4))
			{
				valuemode = 1;
				value = char_hex((string) cBuf);
			}
			else
			{
				valuemode = 2;
				value = (unsigned char) cBuf[0];
			}
			if(vValueList.size() == 0)
			{
				iLastRelativeValue = value;
				SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM) (stringvalue(value)).c_str());
			}
			else
			{
				change = value - iLastRelativeValue;
				iLastRelativeValue = value;
				if(change < 0)
				{
					sign = "-";
				}
				else if(change > 0)
				{
					sign = "+";
				}
				else
				{
					sign = "";
				}
				SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)(stringvalue(value) + " (" + sign + stringvalue(change) + ")").c_str());
			}
			vValueList.push_back(value);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_SETSEL, 0, -1);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), WM_CLEAR, 0, 0);
			delete [] cBuf;
			}
			break;
		case IDC_REMOVE:
			{
			int selected = SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_GETCURSEL, 0, 0);
			if(selected == LB_ERR)
			{
				MessageBox(hDialog,"You must select a value to remove!","Notice:",MB_OK | MB_ICONINFORMATION);
				break;
			}	
			vValueList.remove(selected);
			SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_DELETESTRING, selected, 0);
			}
			break;
		case IDC_SKIP:
			{
				if(vValueList.size() == 0)
				{
					MessageBox(hDialog,"You cannot skip the first byte!","Notice:",MB_OK | MB_ICONINFORMATION);
				}
				else
				{
					vValueList.push_back(-1);
					SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM) "Skipped Byte");
				}
			}
			break;
		case IDC_RESET:
			{
			vValueList = viEmpty;
			SendMessage (GetDlgItem (hDialog, IDC_LIST1), LB_RESETCONTENT, 0, 0);
			SendMessage (GetDlgItem (hDialog, IDC_LIST2), LB_RESETCONTENT, 0, 0);
			}
			break;
		case IDSCAN:
			{
				
				if(vValueList.size() < 2)
				{
					MessageBox(hDialog,"You need at least 2 values!","Notice:",MB_OK | MB_ICONINFORMATION);
					break;
				}
				if(vValueList[vValueList.size()-1] == -1)
				{
					MessageBox(hDialog,"You cannot skip the last byte!","Notice:",MB_OK | MB_ICONINFORMATION);
					break;
				}
				relation a;
				if(SendMessage (GetDlgItem (hDialog, IDC_COMBO1), CB_GETCURSEL, 0, 0) == 0)
				{
					iFindDialogDirection = 1;
				}
				else
				{
					iFindDialogDirection = -1;
				}
				vector<relation> emp2;
				vRelativeOffsets = emp2;
				int k = 0;
				int m = 0;
				for(k = 0;k<vValueList.size()-1;k++)
				{
					if(vValueList[k+1] == -1)
					{
					}
					else
					{
						a.relate = m;
						a.related = k + 1;
						a.offset = vValueList[k+1] - vValueList[m];
						a.op = "=";
						vRelativeOffsets.push_back(a);
						m = k;
						m++;
					}
				}
				progress1 = GetDlgItem(hDialog,IDC_SCANPROGRESS);
				bFileRelativeResults = true;
				SendMessage (GetDlgItem (hDialog, IDC_LIST2), LB_RESETCONTENT, 0, 0);
				vector<relativescandata> emp;
				vScanData = emp;
				int result = vValueList.size();
				sRelSearchString = "";
				for(k = 0;k<vValueList.size();k++)
				{
					sRelSearchString += "*";
				}
				mainwindow.scan_relative_helper(result);
				bLastSearchType = 1;
				if(bRelativeSearchDone)
				{
					MessageBox (hDialog, "Relative string not found.", "Notice:", MB_OK | MB_ICONINFORMATION);
					bFileRelativeResults = false;
					bRelativeSearchDone = false;
					bLastSearchType = 1;
				}
				else
				{
					vScanData.push_back(mainwindow.guess_table_values(mainwindow.iCurrentByte));
					int k = 0;
					SendMessage (GetDlgItem (hDialog, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM) stringvalue(vScanData[k].start).c_str());
					SendMessage (GetDlgItem (hDialog, IDC_LIST2), LB_SETCURSEL, k, 0);
					while(!bRelativeSearchDone)
					{
						if (iFindDialogDirection == 1)
						{
							mainwindow.findnext_relative();
						}
						else
						{
							mainwindow.findprev_relative();
						}
						vScanData.push_back(mainwindow.guess_table_values(mainwindow.iCurrentByte));
						k = vScanData.size() - 1;
						SendMessage (GetDlgItem (hDialog, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM) stringvalue(vScanData[k].start).c_str());
						SendMessage (GetDlgItem (hDialog, IDC_LIST2), LB_SETCURSEL, k, 0);
					}
					SetCursor (LoadCursor (NULL, IDC_ARROW));
					mainwindow.adjust_view_for_selection();
					mainwindow.repaint();
					SendMessage (GetDlgItem (hDialog, IDC_LIST2), LB_SETCURSEL, 0, 0);
					
					bFileRelativeResults = false;
					bRelativeSearchDone = false;
					bLastSearchType = 1;
				}
				ResetProgress(progress1);
				iIncrementStep = 0;
				iProgressPos = 0;
				progress1 = NULL;
			}
			break;
		case IDC_SAVE:
			{
				if(vScanData.size() > 0)
				{
					char cFileName[_MAX_PATH];
					char szTitleName[_MAX_FNAME + _MAX_EXT];
					OPENFILENAME ofn;
					szTitleName[0] =cFileName[0] = '\0';
					ofn.lStructSize = sizeof (OPENFILENAME);
					ofn.hwndOwner = hDialog;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = "Text Files (*.TXT)\0*.TXT\0\0";
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter = 0;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = cFileName;
					ofn.nMaxFile = _MAX_PATH;
					ofn.lpstrFileTitle = szTitleName;
					ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = "Save Search Results As";
					ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
					ofn.nFileOffset = 0;
					ofn.nFileExtension = 0;
					ofn.lpstrDefExt = NULL;
					ofn.lCustData = 0L;
					ofn.lpfnHook = NULL;
					ofn.lpTemplateName = NULL;
					if (GetSaveFileName (&ofn))
					{
						fstream fs;
						int cur = strlen(cFileName) - 4;
						if(uppercase(((string) cFileName).substr(((string) cFileName).length()-4,4)) == ".TXT")
						{
							sFileName = (string) cFileName;
						}
						else
						{
							sFileName = (string) strcat(cFileName,".txt");
						}
						fs.open(sFileName.c_str(),ios::out);
						if(!fs.fail())
						{
							mainwindow.save_relative_results(fs);
							fs.close();
							MessageBox (hDialog, ("Results saved sucessfully to " + sFileName).c_str(), "Notice:", MB_OK | MB_ICONINFORMATION);
						}
						else
						{
							MessageBox (hDialog, "Could not save file.", "Relative Scan error:", MB_OK | MB_ICONERROR);
							fs.close();
						}
					}
				}
				else
				{
					MessageBox(hDialog,"No results to save to file!","Notice:",MB_OK | MB_ICONINFORMATION);
				}
			}	
			break;
		case IDC_RADIO3:
			{
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_SETSEL, 0, -1);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), WM_CLEAR, 0, 0);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_LIMITTEXT, 3, 0);
			SetFocus(GetDlgItem(hDialog,IDC_EDIT1));
			}
			break;
		case IDC_RADIO4:
			{
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_SETSEL, 0, -1);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), WM_CLEAR, 0, 0);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_LIMITTEXT, 2, 0);
			SetFocus(GetDlgItem(hDialog,IDC_EDIT1));
			}
			break;
		case IDC_RADIO5:
			{
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_SETSEL, 0, -1);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), WM_CLEAR, 0, 0);
			SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_LIMITTEXT, 1, 0);
			SetFocus(GetDlgItem(hDialog,IDC_EDIT1));
			}
			break;
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


//BOOKMARKS

int Translhextion::add_bookmark ()
{
	if (FileBuffer.Length()<=0)
	{
		MessageBox (hwnd, "Can not set bookmark in empty file.", "Add bookmark error:", MB_OK | MB_ICONERROR);
		return 0;
	}
	if (iBookmarkCount<iBookmarkMax)
	{
		iBookmarkOffset = iCurrentByte;
		pcBookmarkTxt[0]=0;
		if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_ADDBMK_DIALOG), hwnd, (DLGPROC) AddBookmarkDialogProc))
		{
			if (iBookmarkOffset>=0 && iBookmarkOffset<=FileBuffer.Length())
			{
				if(iOffsetMode == LOROM)
					iBookmarkOffset = toLOROM(iCurrentByte);
				else if(iOffsetMode == HIROM)
					iBookmarkOffset = toHIROM(iCurrentByte);
				else
					iBookmarkOffset = iCurrentByte;
				
				int i;
				for( i = 0; i < iBookmarkCount; i++ )
				{
					if( pbmkList[ i ].offset == iBookmarkOffset )
					{
						MessageBox (hwnd, "There already is a bookmark on that position.", "Add bookmark error:", MB_OK | MB_ICONERROR);
						return 0;
					}
				}
				
				if(sLoadedTable != "")
				{
					string totable;
					char cBuf[512];
					sprintf (cBuf, "%X", iBookmarkOffset);
					totable = "(" + (string) cBuf + "h)" + pcBookmarkTxt;
					vTableFileLeft.push_back(totable);
					vTableFileRight.push_back("");
					fstream fs;
					fs.open(sLoadedTable.c_str(),ios::out);
					
					save_table_file(fs);
					fs.close();
				}

				pbmkList[iBookmarkCount].offset = iBookmarkOffset;
				if (strlen(pcBookmarkTxt)>0)
				{
					pbmkList[iBookmarkCount].name = new char[strlen(pcBookmarkTxt)+1];
					strcpy (pbmkList[iBookmarkCount].name, pcBookmarkTxt);
				}
				else
					pbmkList[iBookmarkCount].name = NULL;
				iBookmarkCount++;
				repaint();
			}
			else
			{
				MessageBox (hwnd, "Can not set bookmark at that position.", "Add bookmark error:", MB_OK | MB_ICONERROR);
				return 0;
			}
		}
	}
	else
	{
		MessageBox (hwnd, "Can not set any more bookmarks.", "Notice:", MB_OK | MB_ICONINFORMATION);
	}
	return 0;
}


BOOL CALLBACK AddBookmarkDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int i = -1;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		char cBuf[32];
		sprintf (cBuf, "x%X", iBookmarkOffset);
		SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
		SetFocus(hDialog);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			char cBuf[16];
			if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 16) != 0)
			{
				if (sscanf (cBuf, "x%X", &i) == 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Starting offset is not recognized.", "Add bookmark error:", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDialog, 0);
					return 0;
				}
				iBookmarkOffset = i;
			}
			GetDlgItemText (hDialog, IDC_EDIT2, pcBookmarkTxt, iBookmarkMaxChars);
			EndDialog (hDialog, 1);
			return TRUE;

		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


int Translhextion::remove_bookmark ()
{
	if (iBookmarkCount==0)
	{
		MessageBox (hwnd, "No bookmarks to remove.", "Remove bookmark error:", MB_OK | MB_ICONERROR);
		return 0;
	}
	pbmkRemove = pbmkList;
	iRemBookmark = iBookmarkCount;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_REMOVEBMK_DIALOG), hwnd, (DLGPROC) RemoveBookmarkDialogProc))
	{
		if(sLoadedTable != "")
		{
			string totable;
			char cBuf[512];
			if(iOffsetMode == LOROM)
				sprintf (cBuf,"%X",toLOROM(pbmkList[iRemBookmark].offset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf,"%X",toHIROM(pbmkList[iRemBookmark].offset));
			else
				sprintf (cBuf,"%X",pbmkList[iRemBookmark].offset);
			totable = "(" + (string) cBuf;
			int k = 0;
			for(k = 0;k < vTableFileLeft.size();k++)
			{
				if(vTableFileLeft[k].substr(0,totable.length()) == totable)
				{
					vTableFileLeft.remove(k);
					vTableFileRight.remove(k);
					break;
				}
			}
			fstream fs;
			fs.open(sLoadedTable.c_str(),ios::out);
			save_table_file(fs);
			fs.close();
		}
		if (pbmkList[iRemBookmark].name != NULL)
			delete [] (pbmkList[iRemBookmark].name);
		int i;
		for (i=iRemBookmark; i<iBookmarkCount-1; i++)
			pbmkList[i] = pbmkList[i+1];
		iBookmarkCount--;
		repaint();
	}
	return 1;
}


BOOL CALLBACK RemoveBookmarkDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[128];
			int i;
			HWND hwndList = GetDlgItem (hDialog, IDC_LIST1);
			for (i=0; i<iRemBookmark; i++)
			{
				if (pbmkRemove[i].name == NULL)
				{
					if(iOffsetMode == LOROM)
						sprintf (cBuf, "%d) 0x%X", i+1, toLOROM(pbmkRemove[i].offset));
					else if(iOffsetMode == HIROM)
						sprintf (cBuf, "%d) 0x%X", i+1, toHIROM(pbmkRemove[i].offset));
					else
						sprintf (cBuf, "%d) 0x%X", i+1, pbmkRemove[i].offset);
				}
				else
				{
					if(iOffsetMode == LOROM)
						sprintf (cBuf, "%d) 0x%X:%s", i+1, toLOROM(pbmkRemove[i].offset), pbmkRemove[i].name);
					else if(iOffsetMode == HIROM)
						sprintf (cBuf, "%d) 0x%X:%s", i+1, toHIROM(pbmkRemove[i].offset), pbmkRemove[i].name);
					else
						sprintf (cBuf, "%d) 0x%X:%s", i+1, pbmkRemove[i].offset, pbmkRemove[i].name);
				}
				SendMessage (hwndList, LB_ADDSTRING, 0, (LPARAM) cBuf);
			}
			SendMessage (hwndList, LB_SETCURSEL, 0, 0);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				HWND hwndList = GetDlgItem (hDialog, IDC_LIST1);
				iRemBookmark = SendMessage (hwndList, LB_GETCURSEL, 0, 0);
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


int Translhextion::clear_all_bookmark ()
{
	if(bReadingTable || MessageBox( hwnd, "Are you sure you want to clear all bookmarks?", "Notice:", MB_YESNO | MB_ICONQUESTION ) == IDYES )
	{
		int i;
		for (i=0; i<iBookmarkCount; i++)
			if (pbmkList[i].name != NULL)
				delete [] (pbmkList[i].name);
		iBookmarkCount = 0;
		if(!bReadingTable && sLoadedTable != "")
		{
			int k = 0;
			for(k = 0;k < vTableFileLeft.size();k++)
			{
				if(vTableFileLeft[k].substr(0,1) == "(")
				{
					vTableFileLeft.remove(k);
					vTableFileRight.remove(k);
					k--;
				}
			}
			fstream fs;
			fs.open(sLoadedTable.c_str(),ios::out);
			save_table_file(fs);
			fs.close();
		}
		return 1;
	}
	return 1;
}


int Translhextion::goto_bookmark( int cmd )
{
	long offset = 0;
	if(iOffsetMode == LOROM)
		offset = fromLOROM(pbmkList[ cmd - IDM_BOOKMARK1 ].offset);
	else if(iOffsetMode == HIROM)
		offset = fromHIROM(pbmkList[ cmd - IDM_BOOKMARK1 ].offset);
	else
		offset = pbmkList[ cmd - IDM_BOOKMARK1 ].offset;
	if( offset >= 0 && offset <= FileBuffer.Length() )
	{
		iCurrentByte = offset;
		update_size_change();
		update_vertical_scrollbar();
		repaint();
	}
	else
	{
		MessageBox (hwnd, "Bookmark points to invalid position.", "Go to bookmark error:", MB_OK | MB_ICONERROR);
	}
	return 0;
}

//SCRIPT

int Translhextion::open_table ()
{
	if (bTableLoaded)
	{
		if (MessageBox (hwnd, "A table file is already loaded! Discard loaded table and open this one?", "Notice:", MB_YESNO | MB_ICONQUESTION) == IDNO)
			return 0;
	}
	char cFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	cFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "Thingy Table Files (*.TBL)\0*.TBL\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Open Thingy Table File";
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		if (file_can_load (cFileName))
		{
			fstream fs;
			sFileName = cFileName;
			fs.open(sFileName.c_str(),ios::in|ios::nocreate);
			if (!fs.fail())
			{
				wread_table_file(fs);
				fs.close();
				strcpy(filename2,sFileName.c_str());
				update_most_recent_table();
				SetCursor (LoadCursor (NULL, IDC_ARROW));
				if(iRealTable > 0)
				{
					if(bTableLoaded)
					{
						bTableActive = false;
						bThingyView = false;
						CheckDlgButton(hwndTBar,IDC_CHECK1,BST_UNCHECKED);
						CheckDlgButton(hwndTBar,IDC_CHECK2,BST_UNCHECKED);
						bTableLoaded = true;
						bTableActive = true;
						sLoadedTable = sFileName;
					}
					else
					{
						bTableLoaded = true;
						bTableActive = true;
						hwndTBar = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_TOOLBAR),hwnd,ToolBarDialogProc);
						sLoadedTable = sFileName;
					}
					//another optimization problem as commented further above in this file
					MessageBox(hwnd,(stringvalue(iRealTable) + " Tokens have been loaded from the table.").c_str(),"Notice:", MB_OK | MB_ICONINFORMATION);
				}
				else
				{
					if(bTableLoaded)
					{
						DestroyWindow(hwndTBar);
						bTableLoaded = false;
						bTableActive = false;
						bThingyView = false;
						sLoadedTable = "";
						vTableFileLeft = vEmpty;
						vTableFileRight = vEmpty;
					}
					MessageBox(hwnd,"This file does not seem to be a table.  Investigate the problem or load another table.","Table file error:", MB_OK | MB_ICONERROR);
				}

			}
			else
			{
				char cBuf[500];
				sprintf (cBuf, "Error code 0x%X occured while opening file %s.", errno, cFileName);
				MessageBox (hwnd, cBuf, "Table file error:", MB_OK | MB_ICONERROR);
				return FALSE;
			}
			
		}					
	}
	repaint();
	return 1;
}

int Translhextion::convert_table()
{
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_TABLECONVERTDIALOG), hwnd, (DLGPROC) TableConvertDialogProc))
	{
		fstream fs;
		fs.open(sLoadedTable.c_str(),ios::in);
		if(!fs.fail())
		{
			wread_table_file(fs);
			fs.close();
			mainwindow.repaint();
			mainwindow.update_size_change();
			if(!(iRealTable > 0))
			{
				if(bTableLoaded)
				{
					DestroyWindow(hwndTBar);
					bTableLoaded = false;
					bTableActive = false;
					bThingyView = false;
					vTableFileLeft = vEmpty;
					vTableFileRight = vEmpty;
					sLoadedTable = "";
				}
				MessageBox(hwnd,"This file does not seem to be a table.  Investigate the problem or load another table.","Table file error:", MB_OK | MB_ICONERROR);
			}
		}
	}
	return 1;
}

BOOL CALLBACK TableConvertDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			int i = 0;
			for(i = 0;i<vTableFileRight.size();i++)
			{
				if(vTableFileRight[i].length() > 0 && 
					(vTableFileRight[i].substr(0,1) == (string) (char) 164 || 
					vTableFileRight[i].substr(0,1) == (string) (char) 165 ||
					vTableFileRight[i].substr(0,1) == (string) (char) 161))
				{
					CheckDlgButton(hDialog,IDC_RADIO2,BST_CHECKED);
					CheckDlgButton(hDialog,IDC_RADIO4,BST_CHECKED);
					CheckDlgButton(hDialog,IDC_RADIO1,BST_UNCHECKED);
					CheckDlgButton(hDialog,IDC_RADIO3,BST_UNCHECKED);
					CheckDlgButton(hDialog,IDC_RADIO5,BST_UNCHECKED);
					CheckDlgButton(hDialog,IDC_RADIO6,BST_UNCHECKED);
					break;
				}
				if(vTableFileRight[i].length() > 0 && 
					(vTableFileRight[i].substr(0,1) == (string) (char) 130 || 
					vTableFileRight[i].substr(0,1) == (string) (char) 131))
				{
					CheckDlgButton(hDialog,IDC_RADIO1,BST_CHECKED);
					CheckDlgButton(hDialog,IDC_RADIO5,BST_CHECKED);
					CheckDlgButton(hDialog,IDC_RADIO2,BST_UNCHECKED);
					CheckDlgButton(hDialog,IDC_RADIO3,BST_UNCHECKED);
					CheckDlgButton(hDialog,IDC_RADIO4,BST_UNCHECKED);
					CheckDlgButton(hDialog,IDC_RADIO6,BST_UNCHECKED);
					break;
				}
			}
			if(i >= vTableFileRight.size())
			{
				CheckDlgButton(hDialog,IDC_RADIO3,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO4,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO1,BST_UNCHECKED);
				CheckDlgButton(hDialog,IDC_RADIO2,BST_UNCHECKED);
				CheckDlgButton(hDialog,IDC_RADIO5,BST_UNCHECKED);
				CheckDlgButton(hDialog,IDC_RADIO6,BST_UNCHECKED);
			}
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				int from = -1;
				if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
				{
					if (IsDlgButtonChecked (hDialog, IDC_RADIO5) == BST_CHECKED)
					{
						convert_table_from_to(vShiftEnc,vEucEnc);
					}
					else if (IsDlgButtonChecked (hDialog, IDC_RADIO6) == BST_CHECKED)
					{
						convert_table_from_to(vShiftEnc,vRomajiEnc);
					}
					else
					{
						MessageBox(hDialog,"Tables cannot be converted between the same encoding!","Notice:",MB_OK | MB_ICONINFORMATION);
					}
				}
				else if (IsDlgButtonChecked (hDialog, IDC_RADIO2) == BST_CHECKED)
				{
					if (IsDlgButtonChecked (hDialog, IDC_RADIO4) == BST_CHECKED)
					{
						convert_table_from_to(vEucEnc,vShiftEnc);
					}
					else if (IsDlgButtonChecked (hDialog, IDC_RADIO6) == BST_CHECKED)
					{
						convert_table_from_to(vEucEnc,vRomajiEnc);
					}
					else
					{
						MessageBox(hDialog,"Tables cannot be converted between the same encoding!","Notice:",MB_OK | MB_ICONINFORMATION);
					}	
				}
				else
				{
					if (IsDlgButtonChecked (hDialog, IDC_RADIO4) == BST_CHECKED)
					{
						convert_table_from_to(vRomajiEnc,vShiftEnc);
					}
					else if (IsDlgButtonChecked (hDialog, IDC_RADIO5) == BST_CHECKED)
					{
						convert_table_from_to(vRomajiEnc,vEucEnc);
					}
					else
					{
						MessageBox(hDialog,"Tables cannot be converted between the same encoding!","Notice:",MB_OK | MB_ICONINFORMATION);
					}
				}
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		case IDC_RADIO1:
			if(IsDlgButtonChecked (hDialog, IDC_RADIO4) == BST_CHECKED)
			{
				CheckDlgButton(hDialog,IDC_RADIO5,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO4,BST_UNCHECKED);
			}
			return TRUE;
			break;
		case IDC_RADIO2:
			if(IsDlgButtonChecked (hDialog, IDC_RADIO5) == BST_CHECKED)
			{
				CheckDlgButton(hDialog,IDC_RADIO4,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO5,BST_UNCHECKED);
			}
			return TRUE;
			break;
		case IDC_RADIO3:
			if(IsDlgButtonChecked (hDialog, IDC_RADIO6) == BST_CHECKED)
			{
				CheckDlgButton(hDialog,IDC_RADIO4,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO6,BST_UNCHECKED);
			}
			return TRUE;
			break;
		case IDC_RADIO4:
			if(IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
			{
				CheckDlgButton(hDialog,IDC_RADIO2,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO1,BST_UNCHECKED);
			}
			return TRUE;
			break;
		case IDC_RADIO5:
			if(IsDlgButtonChecked (hDialog, IDC_RADIO2) == BST_CHECKED)
			{
				CheckDlgButton(hDialog,IDC_RADIO1,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO2,BST_UNCHECKED);
			}
			return TRUE;
			break;
		case IDC_RADIO6:
			if(IsDlgButtonChecked (hDialog, IDC_RADIO3) == BST_CHECKED)
			{
				CheckDlgButton(hDialog,IDC_RADIO1,BST_CHECKED);
				CheckDlgButton(hDialog,IDC_RADIO3,BST_UNCHECKED);
			}
			return TRUE;
			break;
		}
		break;
	}
	return FALSE;
}

int Translhextion::script_dump ()
{
	long numbytes = 0;
	int difference = 0;
	if (!bSelected)
	{
		iCopyStartOffset = iCurrentByte;
		iCopyEndOffset = iCurrentByte;
	}
	else
	{
		if (iEndOfSelection >= iStartOfSelection)
		{
			iCopyStartOffset = iStartOfSelection;
			iCopyEndOffset = iEndOfSelection;
		}
		else
		{
			iCopyStartOffset = iEndOfSelection;
			iCopyEndOffset = iStartOfSelection;
		}
	}
	if(iDumpEnd != 0)
	{
		iCopyStartOffset = iDumpStart;
		iCopyEndOffset = iDumpEnd;
	}

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_SCRIPTDUMPDIALOG), hwnd, (DLGPROC) ScriptDumpDialogProc) != FALSE)
	{
		iStartOfSelection = iCopyStartOffset;
		iEndOfSelection = iCopyEndOffset;
		if (iEndOfSelection >= iStartOfSelection)
		{
			int i = 0;
			iDumpStart = iStartOfSelection;
			iDumpEnd = iEndOfSelection;
			vector<string> vDumpSize((iDumpEnd-iDumpStart)+1);
			vHexdoc = vDumpSize;
			vDumpSize = vEmpty;
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			HWND progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_PROGRESSDIALOG),hwnd,ProgressDialogProc);
			SetWindowText(progressdialog,"Script Dump Preparation...");
			SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Queuing bytes...");
			progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
			SetProgressRange(progress1,0,vHexdoc.size());
			if(vHexdoc.size() >= 10000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 1000;
			}
			else if(vHexdoc.size() >= 1000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 100;
			}
			else if(vHexdoc.size() >= 100)
			{
				iIncrementStep = vHexdoc.size() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress1,iIncrementStep);  
			for(i = iDumpStart;i<=iDumpEnd;i++)
			{
				iProgressPos++;
				if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
				{
					StepProgress(progress1);	
				}
				vHexdoc[i - iDumpStart] = hex_char(FileBuffer[i]);
			}
			DestroyWindow(progressdialog);
			iProgressPos = 0;
			iIncrementStep = 0;
			progress1 = NULL;
			long filesize = vHexdoc.size();
			numbytes = vHexdoc.size();
			if(bDumpClean)
			{
				bCleanScript = true;
			}
			else
			{
				bCleanScript = false;
			}
			progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_2PROGRESSDIALOG),hwnd,ProgressDialogProc);
			SetWindowText(progressdialog,"Script Dump Progress...");
			SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Translating...");
			progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
			progress2 = GetDlgItem(progressdialog,IDC_PROGRESS2);
			SetProgressRange(progress1,0,vHexdoc.size());
			if(vHexdoc.size() >= 10000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 1000;
			}
			else if(vHexdoc.size() >= 1000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 100;
			}
			else if(vHexdoc.size() >= 100)
			{
				iIncrementStep = vHexdoc.size() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress1,iIncrementStep);  
			translate(vHexdoc);
			SetProgressPos(progress1,numbytes);
			iProgressPos = 0;
			SetProgressRange(progress2,0,vHexdoc.size());
			if(vHexdoc.size() >= 10000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 1000;
			}
			else if(vHexdoc.size() >= 1000)//this could be better
			{
				iIncrementStep = vHexdoc.size() / 100;
			}
			else if(vHexdoc.size() >= 100)
			{
				iIncrementStep = vHexdoc.size() / 10;
			}
			else
			{
				iIncrementStep = 2; 
			}
			SetProgressStep(progress2,iIncrementStep);  
			SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS2TITLE),"Formatting...");
			enscript(vHexdoc);
			progress1 = NULL;
			progress2 = NULL;
			iIncrementStep = 0;
			iProgressPos = 0;
			DestroyWindow(progressdialog);
			bCleanScript = false;
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			if(bDumpToFile)
			{
				fstream fs;
				fs.open(sFileName.c_str(),ios::out);
				if (!fs.fail())
				{
					
					if(bSeemsJ)
					{
						if(MessageBox(hwnd,((string) sName + " has detected that this may be a Japanese script.  You" +
						" have the option to combine characters and the punctuation characters that modify them into single characters." +
						"  Would you like to merge modified characters with their punctuation?").c_str(),"Notice:", MB_YESNO | MB_ICONQUESTION) == IDYES)
						{
							SetCursor (LoadCursor (NULL, IDC_WAIT));
							pwrite_script_file(fs,vHexdoc);
						}
						else
						{
							SetCursor (LoadCursor (NULL, IDC_WAIT));
							write_script_file(fs,vHexdoc);
						}
						bSeemsJ = false;
					}
					else
					{
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						write_script_file(fs,vHexdoc);
					}
					SetCursor (LoadCursor (NULL, IDC_ARROW));
					fs.close();
					MessageBox (hwnd, ("Script dumped sucessfully to " + sFileName + "\n\n" + stringvalue(filesize) + " tile-bytes have been dumped.").c_str(), "Script Dump Complete", MB_OK);
					dumps++;
				}
				else
				{
					MessageBox (hwnd, "Could not save file.", "Script dump error:", MB_OK | MB_ICONERROR);
					fs.close();
				}
				
			}
			else
			{
				if(bDumpClean)
				{
					MessageBox(hwnd,"'#' is defined in your table file so dump to screen is not allowed!","Notice:",MB_OK | MB_ICONINFORMATION);
				}
				bool oldbmark = bByteMark;
				if (!bDumpClean && DialogBox (hInstance, MAKEINTRESOURCE (IDD_VIEWSCRIPTDIALOG), hwnd, (DLGPROC) ViewScriptDialogProc) != FALSE)
				{
					if(bReadOnly)
					{
						MessageBox(hwnd,"File cannot be changed in read-only mode!","Script reinsertion error:", MB_OK | MB_ICONERROR);
						return 0;
					}
					SetCursor (LoadCursor (NULL, IDC_WAIT));
					HWND progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_2PROGRESSDIALOG),hwnd,ProgressDialogProc);
					SetWindowText(progressdialog,"Script Replacement Progress...");
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Removing Formatting...");
					progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
					progress2 = GetDlgItem(progressdialog,IDC_PROGRESS2);
					int numbytes2 = vHexdoc.size();
					SetProgressRange(progress1,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress1,iIncrementStep);  
					descript(vHexdoc);
					SetProgressPos(progress1,numbytes2);
					iProgressPos = 0;
					SetProgressRange(progress2,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress2,iIncrementStep);  
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS2TITLE),"Translating...");
					detranslate(vHexdoc);
					progress1 = NULL;
					progress2 = NULL;
					iIncrementStep = 0;
					iProgressPos = 0;
					DestroyWindow(progressdialog);
					int tblock = -1;
					if(vHexdoc.size() > numbytes)
					{
						tblock = MessageBox(hwnd,((string) "The number of bytes you are inserting exceeds the amount you dumped by " +
							stringvalue(vHexdoc.size() - numbytes) + " bytes." + 
							"\nThe extra bytes will be written AFTER the originally selected dump overwriting existing bytes!" +
							"\nAre you sure you want to do this?").c_str(),"WARNING!",MB_OKCANCEL | MB_ICONWARNING);
						switch(tblock)
						{
						case IDOK:
							{
								if(iDumpStart + vHexdoc.size() > FileBuffer.Length())
								{
									MessageBox(hwnd,((string) "The number of bytes you are replacing exceeds the end of the file!" +
										"\r\nThe replacement will stop when the end of the file is reached.").c_str(),"Notice:",MB_OK | MB_ICONINFORMATION);
								}
								progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_PROGRESSDIALOG),hwnd,ProgressDialogProc);
								SetWindowText(progressdialog,"Script Replacement Progress...");
								SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Replacing bytes...");
								progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
								SetProgressRange(progress1,0,vHexdoc.size());
								if(vHexdoc.size() >= 10000)//this could be better
								{
									iIncrementStep = vHexdoc.size() / 1000;
								}
								else if(vHexdoc.size() >= 1000)//this could be better
								{
									iIncrementStep = vHexdoc.size() / 100;
								}
								else if(vHexdoc.size() >= 100)
								{
									iIncrementStep = vHexdoc.size() / 10;
								}
								else
								{
									iIncrementStep = 2; 
								}
								SetProgressStep(progress1,iIncrementStep);  
								for(i = iDumpStart;i < FileBuffer.Length() && i<iDumpStart + vHexdoc.size();i++)
								{
									iProgressPos++;
									if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
									{
										StepProgress(progress1);
									}
									FileBuffer[i] = char_hex(vHexdoc[i - iDumpStart]);
									vChanges[i] = COLORCHANGE;
								}
								DestroyWindow(progressdialog);
								iProgressPos = 0;
								iIncrementStep = 0;
								progress1 = NULL;
								inserts++;
								iFileChanged = TRUE;
								break;
							}
						case IDCANCEL:
							{
								MessageBox(hwnd,"No bytes have been changed.","Notice:",MB_ICONINFORMATION);
								break;
							}
						}
					}
					else if(vHexdoc.size() < numbytes)
					{
						tblock = MessageBox(hwnd,((string) "The number of bytes you are inserting is less than the amount you dumped by " +
							stringvalue(numbytes - vHexdoc.size()) + " bytes." + 
							"\nDo you wish to overwrite the remainder of the originally selected dump with '00'?" +
							"\nIf you select no, the remaining bytes will be left as is.").c_str(),"Notice:",MB_YESNO | MB_ICONWARNING);
						switch(tblock)
						{
						case IDYES:
							{
								progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_PROGRESSDIALOG),hwnd,ProgressDialogProc);
								SetWindowText(progressdialog,"Script Replacement Progress...");
								SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Replacing bytes...");
								progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
								SetProgressRange(progress1,0,numbytes);
								if(vHexdoc.size() >= 10000)//this could be better
								{
									iIncrementStep = vHexdoc.size() / 1000;
								}
								else if(vHexdoc.size() >= 1000)//this could be better
								{
									iIncrementStep = vHexdoc.size() / 100;
								}
								else if(vHexdoc.size() >= 100)
								{
									iIncrementStep = vHexdoc.size() / 10;
								}
								else
								{
									iIncrementStep = 2; 
								}
								SetProgressStep(progress1,iIncrementStep);  
								for(i = 0;i<vHexdoc.size();i++)
								{
									iProgressPos++;
									if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
									{
										StepProgress(progress1);
									}
									FileBuffer[iDumpStart + i] = char_hex(vHexdoc[i]);
									vChanges[iDumpStart + i] = COLORCHANGE;
								}
								difference = iDumpStart + i;
								for(i = 0;i <(numbytes - vHexdoc.size());i++)
								{
									iProgressPos++;
									if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
									{
										StepProgress(progress1);
									}
									FileBuffer[difference + i] = char_hex("00");
									vChanges[difference + i] = COLORCHANGE;
								}
								DestroyWindow(progressdialog);
								iProgressPos = 0;
								iIncrementStep = 0;
								progress1 = NULL;
								inserts++;
								iFileChanged = TRUE;
								break;
							}
						case IDNO:
							{
								progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_PROGRESSDIALOG),hwnd,ProgressDialogProc);
								SetWindowText(progressdialog,"Script Replacement Progress...");
								SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Replacing bytes...");
								progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
								SetProgressRange(progress1,0,vHexdoc.size());
								if(vHexdoc.size() >= 10000)//this could be better
								{
									iIncrementStep = vHexdoc.size() / 1000;
								}
								else if(vHexdoc.size() >= 1000)//this could be better
								{
									iIncrementStep = vHexdoc.size() / 100;
								}
								else if(vHexdoc.size() >= 100)
								{
									iIncrementStep = vHexdoc.size() / 10;
								}
								else
								{
									iIncrementStep = 2; 
								}
								SetProgressStep(progress1,iIncrementStep);  
								for(i = 0;i<vHexdoc.size();i++)
								{
									iProgressPos++;
									if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
									{
										StepProgress(progress1);
									}
									FileBuffer[iDumpStart + i] = char_hex(vHexdoc[i]);
									vChanges[iDumpStart + i] = COLORCHANGE;
								}
								DestroyWindow(progressdialog);
								iProgressPos = 0;
								iIncrementStep = 0;
								progress1 = NULL;
								inserts++;
								iFileChanged = TRUE;
								break;
							}
						}
						
					}
					else
					{
						progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_PROGRESSDIALOG),hwnd,ProgressDialogProc);
						SetWindowText(progressdialog,"Script Replacement Progress...");
						SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Replacing bytes...");
						progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
						SetProgressRange(progress1,0,vHexdoc.size());
						if(vHexdoc.size() >= 10000)//this could be better
						{
							iIncrementStep = vHexdoc.size() / 1000;
						}
						else if(vHexdoc.size() >= 1000)//this could be better
						{
							iIncrementStep = vHexdoc.size() / 100;
						}
						else if(vHexdoc.size() >= 100)
						{
							iIncrementStep = vHexdoc.size() / 10;
						}
						else
						{
							iIncrementStep = 2; 
						}
						SetProgressStep(progress1,iIncrementStep);  
						for(i = 0;i<vHexdoc.size();i++)
						{
							iProgressPos++;
							if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
							{
								StepProgress(progress1);
							}
							FileBuffer[iDumpStart + i] = char_hex(vHexdoc[i]);
							vChanges[iDumpStart + i] = COLORCHANGE;
						}
						DestroyWindow(progressdialog);
						iProgressPos = 0;
						iIncrementStep = 0;
						progress1 = NULL;
						inserts++;
						iFileChanged = TRUE;
					}
					bByteMark = oldbmark;
					vHexdoc = vEmpty;
					sScriptDump = "";
					iDumpStart = 0;
					iDumpEnd = 0;
					SetCursor (LoadCursor (NULL, IDC_ARROW));
					repaint();
				}

			}
		}
		else
		{
			MessageBox (hwnd, "Ending offset is smaller than starting offset.", "Script dump error:", MB_OK | MB_ICONERROR);
		}

			
	}
	iDumpStart = 0;
	iDumpEnd = 0;
	adjust_view_for_selection ();
	return 1;
}

BOOL CALLBACK ProgressDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
			default:
			return FALSE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK InfoDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		case WM_INITDIALOG:
		{
			long filesize = ppair.x;
			long filesize2 = ppair.y;
			progress1 = GetDlgItem(hDialog,IDC_PROGRESS1);
			progress2 = GetDlgItem(hDialog,IDC_PROGRESS2);
			SetProgressColor(progress1,RGB(170,55,255));
			SetProgressColor(progress2,RGB(170,55,255));
			SetProgressRange(progress1,0,filesize);
			SetProgressRange(progress2,0,filesize);
			SetProgressPos(progress1,filesize + 1);
			SetProgressPos(progress2,filesize2);
			string cresults;
			long percent = 100;
			if(filesize != 0)
			{
				percent = (long) ((100.0 / filesize) * filesize2);
			}
			cresults += "The input script had:";
			cresults += "\r\n\r\n";
			cresults += "     " + stringvalue(filesize) + " tile-bytes";
			cresults += "\r\n\r\n";
			cresults += "The output script has been crunched to:";
			cresults += "\r\n\r\n";
			cresults += "     " + stringvalue(filesize2) + " tile-bytes";
			cresults += "\r\n\r\n\r\n";
			cresults += "Compression ratio:  " + stringvalue(percent) + "%";
			SetWindowText(GetDlgItem(hDialog,IDC_CRESULTS),cresults.c_str());
			
			SetFocus(hDialog);
			return FALSE;
		}

		case WM_COMMAND:
		{
			switch (LOWORD (wParam))
			{
				case IDOK:
				{
					EndDialog(hDialog,0);
					return FALSE;
					
				}
				break;
			}
		}
	}
	return FALSE;
}

BOOL CALLBACK ScriptDumpDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[32];
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCopyStartOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCopyStartOffset));
			else
				sprintf (cBuf, "x%X", iCopyStartOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCopyEndOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCopyEndOffset));
			else
				sprintf (cBuf, "x%X", iCopyEndOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			sprintf (cBuf, "%d", iCopyEndOffset-iCopyStartOffset+1);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT3), cBuf);
			CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			if(iLastDumpTarget == 0)
				CheckDlgButton(hDialog,IDC_RADIO3,BST_CHECKED);
			else
				CheckDlgButton(hDialog,IDC_RADIO4,BST_CHECKED);
			if(bDefByteMark)
				CheckDlgButton(hDialog,IDC_CHECK1,BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[64];
				if(IsDlgButtonChecked(hDialog,IDC_CHECK1))
				{
					bByteMark = true;
				}
				else
				{
					bByteMark = false;
				}
				if(IsDlgButtonChecked(hDialog,IDC_RADIO3))
				{
					bDumpToFile = true;
					iLastDumpTarget = 0;
				}
				else
				{
					bDumpToFile = false;
					iLastDumpTarget = 1;
				}
				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 64) != 0)
				{
					int i;
					if (sscanf (cBuf, "x%X", &i) == 0)
					{
						if (sscanf (cBuf, "%d", &i) == 0)
						{
							MessageBox (hDialog, "Starting offset is not recognized.", "Script dump error:", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
					if (i==-1)
					{
						EndDialog (hDialog, 0);
						return 0;
					}
					if(iOffsetMode == LOROM)
						iCopyStartOffset = fromLOROM(i);
					else if(iOffsetMode == HIROM)
						iCopyStartOffset = fromHIROM(i);
					else
						iCopyStartOffset = i;
				}
				switch (IsDlgButtonChecked (hDialog, IDC_RADIO1))
				{
				case BST_CHECKED: 
					{
						if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 64) != 0)
						{
							int i;
							if (sscanf (cBuf, "x%X", &i) == 0)
							{
								if (sscanf (cBuf, "%d", &i) == 0)
								{
									MessageBox (hDialog, "Ending offset is not recognized.", "Script dump error:", MB_OK | MB_ICONERROR);
									i = -1;
								}
							}
							if (i==-1)
							{
								EndDialog (hDialog, 0);
								return 0;
							}
							if(iOffsetMode == LOROM)
								iCopyEndOffset = fromLOROM(i);
							else if(iOffsetMode == HIROM)
								iCopyEndOffset = fromHIROM(i);
							else
								iCopyEndOffset = i;
						}
						break;
					}

				default: 
					{
						if (GetDlgItemText (hDialog, IDC_EDIT3, cBuf, 64) != 0)
						{
							int i;
							if (sscanf (cBuf, "%d", &i) == 0)
							{
								MessageBox (hDialog, "Number of bytes is not recognized.", "Script dump error:", MB_OK | MB_ICONERROR);
								i = -1;
							}
							if (i==-1)
							{
								EndDialog (hDialog, 0);
								return 0;
							}
							iCopyEndOffset = iCopyStartOffset + i;
						}
						break;
					}
				}
				if(bDumpToFile)
				{
					char cFileName[_MAX_PATH];
					char szTitleName[_MAX_FNAME + _MAX_EXT];
					OPENFILENAME ofn;
					szTitleName[0] =cFileName[0] = '\0';
					ofn.lStructSize = sizeof (OPENFILENAME);
					ofn.hwndOwner = hDialog;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = "Text Files (*.TXT)\0*.TXT\0Script Files (*.SCP)\0*.SCP\0\0";
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter = 0;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = cFileName;
					ofn.nMaxFile = _MAX_PATH;
					ofn.lpstrFileTitle = szTitleName;
					ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = "Save Script As";
					ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
					ofn.nFileOffset = 0;
					ofn.nFileExtension = 0;
					ofn.lpstrDefExt = "SCP";
					ofn.lCustData = 0L;
					ofn.lpfnHook = NULL;
					ofn.lpTemplateName = NULL;
					if (GetSaveFileName (&ofn))
					{
						fstream fs;
						int cur = strlen(cFileName) - 4;
						if(uppercase(((string) cFileName).substr(((string) cFileName).length()-4,4)) == ".TXT")
						{
							sFileName = (string) cFileName;
						}
						else
						{
							sFileName = (string) strcat(cFileName,".txt");
						}
						EndDialog (hDialog, 1);
						return TRUE;
					}
					else
						//EndDialog (hDialog, 0);
					break;
				}
				else
				{
				
					EndDialog (hDialog, 1);
					return TRUE;
				}
				//return TRUE;
			}

		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK ViewScriptDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			int i = 0;
			assert(vHexdoc.size() >= 0);
			for(i = 0;i<vHexdoc.size();i++)
			{
				if(vHexdoc[i].substr(vHexdoc[i].length()-1,1) == "\n")
					sScriptDump += vHexdoc[i].substr(0,vHexdoc[i].length()-1) + "\r\n";
				else
					sScriptDump += vHexdoc[i].substr(0,vHexdoc[i].length());
				
			}
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			SetWindowText( GetDlgItem ( hDialog, IDC_SCRIPTEDIT), sScriptDump.c_str());
			SetWindowText( GetDlgItem ( hDialog, IDC_NUMBYTES), ("Total Bytes: " + stringvalue(iDumpEnd+1 - iDumpStart)).c_str());
			SetWindowText( GetDlgItem ( hDialog, IDC_STARTBYTE), stringvalue(iDumpStart).c_str());
			SetWindowText( GetDlgItem ( hDialog, IDC_ENDBYTE), stringvalue(iDumpEnd).c_str());
			if(!bByteMark)
			{
				CheckDlgButton(hDialog,IDC_CHECK1,BST_CHECKED);
			}
			SetFocus(hDialog);
			dumps++;
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				sScriptDump = "";
				SetWindowText( GetDlgItem ( hDialog, IDC_SCRIPTEDIT), sScriptDump.c_str());
				EndDialog(hDialog,0);
				return FALSE;
			}

		case IDC_REINSERT:
			{
				if(IsDlgButtonChecked(hDialog,IDC_CHECK1))
				{
					bByteMark = false;
				}
				else
				{	
					bByteMark = true;
				}	
				SetCursor (LoadCursor (NULL, IDC_WAIT));
				int i = 0;
				int j = 0;
				int numlines = SendMessage(GetDlgItem (hDialog, IDC_SCRIPTEDIT),EM_GETLINECOUNT,0,0);
				int linelength = 0;
				for(i = 0;i<numlines;i++)
				{
					linelength += 2 + SendMessage(GetDlgItem (hDialog, IDC_SCRIPTEDIT),EM_LINELENGTH,SendMessage(GetDlgItem (hDialog, IDC_SCRIPTEDIT),EM_LINEINDEX,i,0),0);
				}
				linelength = linelength - 1;
				char* cBuf = new char[ linelength ];
					GetDlgItemText( hDialog, IDC_SCRIPTEDIT, cBuf, linelength );
				//else
				//{
				///	MessageBox( hDialog, "Could not store reinsertion data.", "Reinsertion Failed!", MB_OK | MB_ICONERROR );
				//	break;
				//}
				int	scriptsize = sScriptDump.length();
				sScriptDump = "";
				sScriptDump = (string) cBuf;
				string temp;
				vHexdoc = vEmpty;
				for(i = 0;i<sScriptDump.length();i++)
				{
					if(sScriptDump.substr(i,6) == sByteOpen + "END" + sByteClose + "\r")
					{
						vHexdoc.push_back(temp + sByteOpen + "END" + sByteClose + "\n");
						temp = "";
						i = i + 6;
					}
					else if(sScriptDump.substr(i,1) == "\r")
					{
						vHexdoc.push_back(temp + "\n");
						temp = "";
						i++;
					}
					else
					{
						temp += sScriptDump.substr(i,1);
					}
				}
				if(temp != "")
					vHexdoc.push_back(temp);
				delete [] cBuf;
				sScriptDump = "";
				SetWindowText( GetDlgItem ( hDialog, IDC_SCRIPTEDIT), sScriptDump.c_str());
				SetCursor (LoadCursor (NULL, IDC_ARROW));
				EndDialog(hDialog,1);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

int Translhextion::replace_script ()
{
	long numbytes = 0;
	long difference = 0;
	long actualreplace = 0;
	if (!bSelected)
	{
		iCopyStartOffset = iCurrentByte;
		iCopyEndOffset = iCurrentByte;
	}
	else
	{
		if (iEndOfSelection >= iStartOfSelection)
		{
			iCopyStartOffset = iStartOfSelection;
			iCopyEndOffset = iEndOfSelection;
		}
		else
		{
			iCopyStartOffset = iEndOfSelection;
			iCopyEndOffset = iStartOfSelection;
		}
	}

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_REPLACESCRIPTDIALOG), hwnd, (DLGPROC) ScriptReplaceDialogProc) != FALSE)
	{
		
		iStartOfSelection = iCopyStartOffset;
		iEndOfSelection = iCopyEndOffset;
		if (iEndOfSelection >= iStartOfSelection)
		{
			fstream fs;
			fs.open(sFileName.c_str(),ios::in);
			vHexdoc = vEmpty;
			if(!fs.fail())
			{
				int i = 0;
				read_script_file(fs,vHexdoc);
				fs.close();
				iDumpStart = iStartOfSelection;
				iDumpEnd = iEndOfSelection;
				SetCursor (LoadCursor (NULL, IDC_WAIT));
				HWND progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_2PROGRESSDIALOG),hwnd,ProgressDialogProc);
				SetWindowText(progressdialog,"Script Replacement Progress...");
				SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Removing Formatting...");
				progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
				progress2 = GetDlgItem(progressdialog,IDC_PROGRESS2);
				numbytes = vHexdoc.size();
				SetProgressRange(progress1,0,vHexdoc.size());
				if(vHexdoc.size() >= 10000)//this could be better
				{
					iIncrementStep = vHexdoc.size() / 1000;
				}
				else if(vHexdoc.size() >= 1000)//this could be better
				{
					iIncrementStep = vHexdoc.size() / 100;
				}
				else if(vHexdoc.size() >= 100)
				{
					iIncrementStep = vHexdoc.size() / 10;
				}
				else
				{
					iIncrementStep = 2; 
				}
				SetProgressStep(progress1,iIncrementStep);  
				descript(vHexdoc);
				SetProgressPos(progress1,numbytes);
				iProgressPos = 0;
				SetProgressRange(progress2,0,vHexdoc.size());
				if(vHexdoc.size() >= 10000)//this could be better
				{
					iIncrementStep = vHexdoc.size() / 1000;
				}
				else if(vHexdoc.size() >= 1000)//this could be better
				{
					iIncrementStep = vHexdoc.size() / 100;
				}
				else if(vHexdoc.size() >= 100)
				{
					iIncrementStep = vHexdoc.size() / 10;
				}
				else
				{
					iIncrementStep = 2; 
				}
				SetProgressStep(progress2,iIncrementStep);  
				SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS2TITLE),"Translating...");
				detranslate(vHexdoc);
				progress1 = NULL;
				progress2 = NULL;
				iIncrementStep = 0;
				iProgressPos = 0;
				DestroyWindow(progressdialog);
				long filesize = vHexdoc.size();
				actualreplace = filesize;
				numbytes = vHexdoc.size();
				progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_PROGRESSDIALOG),hwnd,ProgressDialogProc);
				SetWindowText(progressdialog,"Script Replacement Progress...");
				SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Replacing bytes...");
				progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
				if(bReplaceToOffset && bPadBytes)
				{
					SetProgressRange(progress1,0,(iDumpEnd - iDumpStart)+1);
				}
				else if(bReplaceToOffset && !bPadBytes)
				{
					SetProgressRange(progress1,0,vHexdoc.size() < ((iDumpEnd - iDumpStart)+1) ? vHexdoc.size() : ((iDumpEnd - iDumpStart)+1));
				}
				else
				{
					SetProgressRange(progress1,0,vHexdoc.size());
				}
				if(vHexdoc.size() >= 10000)//this could be better
				{
					iIncrementStep = vHexdoc.size() / 1000;
				}
				else if(vHexdoc.size() >= 1000)//this could be better
				{
					iIncrementStep = vHexdoc.size() / 100;
				}
				else if(vHexdoc.size() >= 100)
				{
					iIncrementStep = vHexdoc.size() / 10;
				}
				else
				{
					iIncrementStep = 2; 
				}
				SetProgressStep(progress1,iIncrementStep);  
				if(bReplaceToOffset)
				{
					if(bPadBytes)
					{
						for(i = 0;((i + iDumpStart)<= iDumpEnd) && (i<vHexdoc.size()) && ((i + iDumpStart) < FileBuffer.Length());i++)
						{
							iProgressPos++;
							if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
							{
								StepProgress(progress1);
							}
							FileBuffer[iDumpStart + i] = char_hex(vHexdoc[i]);
							vChanges[iDumpStart + i] = COLORCHANGE;
						}
						difference = iDumpStart + i;
						for(i = 0;((difference + i) <= iDumpEnd) && ((difference + i) < FileBuffer.Length());i++)
						{
							iProgressPos++;
							if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
							{
								StepProgress(progress1);
							}
							FileBuffer[difference + i] = char_hex("00");
							vChanges[difference + i] = COLORCHANGE;
						}
						if(iDumpEnd - iDumpStart < filesize)
						{
							filesize = iDumpEnd - iDumpStart + 1;
						}
					}
					else
					{
						for(i = 0;((i + iDumpStart) <= iDumpEnd) && (i<vHexdoc.size()) && ((i + iDumpStart) < FileBuffer.Length());i++)
						{
							iProgressPos++;
							if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
							{
								StepProgress(progress1);
							}
							FileBuffer[iDumpStart + i] = char_hex(vHexdoc[i]);
							vChanges[iDumpStart + i] = COLORCHANGE;
						}
						if(iDumpEnd - iDumpStart < filesize)
						{
							filesize = iDumpEnd - iDumpStart + 1;
						}
					}
				}
				else
				{
					if(iDumpStart + vHexdoc.size() > FileBuffer.Length())
					{
						MessageBox(hwnd,((string) "The number of bytes you are replacing exceeds the end of the file!" +
							"\r\nThe replacement will stop when the end of the file is reached.").c_str(),"Notice:",MB_OK | MB_ICONINFORMATION);
					}
					for(i = 0;(i<vHexdoc.size()) && ((i + iDumpStart) < FileBuffer.Length());i++)
					{
						iProgressPos++;
						if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
						{
							StepProgress(progress1);
						}
						FileBuffer[iDumpStart + i] = char_hex(vHexdoc[i]);
						vChanges[iDumpStart + i] = COLORCHANGE;
					}
				}
				DestroyWindow(progressdialog);
				iProgressPos = 0;
				iIncrementStep = 0;
				progress1 = NULL;
				inserts++;
				SetCursor (LoadCursor (NULL, IDC_ARROW));
				bReplaceToOffset = false;
				bPadBytes = false;
				vHexdoc = vEmpty;
				iDumpStart = 0;
				iDumpEnd = 0;
				iFileChanged = TRUE;
				repaint();
				MessageBox(hwnd,(stringvalue(filesize) + " tile-bytes have been replaced.").c_str(),"Notice:",MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				fs.close();
				MessageBox(hwnd,"Error occurred while reading from script file.","Script replace error:",MB_OK | MB_ICONERROR);
			}		
		}
		else
		{
			MessageBox (hwnd, "Ending offset is smaller than starting offset.", "Script replace error:", MB_OK | MB_ICONERROR);
		}	
	}
	iDumpStart = 0;
	iDumpEnd = 0;
	adjust_view_for_selection ();
	return 1;
}

BOOL CALLBACK ScriptReplaceDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char cBuf[32];
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCopyStartOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCopyStartOffset));
			else
				sprintf (cBuf, "x%X", iCopyStartOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			if(iOffsetMode == LOROM)
				sprintf (cBuf, "x%X", toLOROM(iCopyEndOffset));
			else if(iOffsetMode == HIROM)
				sprintf (cBuf, "x%X", toHIROM(iCopyEndOffset));
			else
				sprintf (cBuf, "x%X", iCopyEndOffset);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			if(bDefByteMark)
				CheckDlgButton(hDialog,IDC_CHECK6, BST_CHECKED);
			SetFocus(hDialog);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char cBuf[64];
				// Read start offset.
				if(IsDlgButtonChecked(hDialog,IDC_CHECK6))
				{
					bByteMark = true;
				}
				else
				{
					bByteMark = false;
				}
				if(IsDlgButtonChecked(hDialog,IDC_CHECK5))
				{
					bPadBytes = true;
				}
				else
				{
					bPadBytes = false;
				}
				if(IsDlgButtonChecked(hDialog,IDC_RADIO1))
				{
					bReplaceToOffset = false;
				}
				else
				{
					bReplaceToOffset = true;
				}

				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 64) != 0)
				{
					int i;
					if (sscanf (cBuf, "x%X", &i) == 0)
					{
						if (sscanf (cBuf, "%d", &i) == 0)
						{
							MessageBox (hDialog, "Starting offset is not recognized.", "Script replace error:", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
					if (i==-1)
					{
						EndDialog (hDialog, 0);
						return 0;
					}
					if(iOffsetMode == LOROM)
						iCopyStartOffset = fromLOROM(i);
					else if(iOffsetMode == HIROM)
						iCopyStartOffset = fromHIROM(i);
					else
						iCopyStartOffset = i;
				}
				char cFileName[_MAX_PATH];
				char szTitleName[_MAX_FNAME + _MAX_EXT];
				cFileName[0] = '\0';
				OPENFILENAME ofn;
				ofn.lStructSize = sizeof (OPENFILENAME);
				ofn.hwndOwner = hDialog;
				ofn.hInstance = NULL;
				ofn.lpstrFilter = "Text Files (*.TXT)\0*.TXT\0Script Files (*.SCP)\0*.SCP\0All Files (*.*)\0*.*\0\0";
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = 0;
				ofn.nFilterIndex = 0;
				ofn.lpstrFile = cFileName;
				ofn.nMaxFile = _MAX_PATH;
				ofn.lpstrFileTitle = szTitleName;
				ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrTitle = NULL;
				ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
				ofn.nFileOffset = 0;
				ofn.nFileExtension = 0;
				ofn.lpstrDefExt = NULL;
				ofn.lCustData = 0L;
				ofn.lpfnHook = NULL;
				ofn.lpTemplateName = NULL;
				if (GetOpenFileName (&ofn))
				{
					fstream fs;
					int cur = strlen(cFileName) - 4;
					if(uppercase(((string) cFileName).substr(((string) cFileName).length()-4,4)) == ".TXT")
					{
						sFileName = (string) cFileName;
					}
					else
					{
						sFileName = (string) strcat(cFileName,".txt");
					}
					EndDialog (hDialog, 1);
					return TRUE;
				}
				else
				{
					//EndDialog (hDialog, 0);
				}
				break;
			}

		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Translhextion::optimize_script_file()
{
	string question;
	long numbytes = 0;
	char cFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	cFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "Text Files (*.TXT)\0*.TXT\0Script Files (*.SCP)\0*.SCP\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = cFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Open Script File to Optimize";
	ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		sFileName = (string) cFileName;
		fstream fs;
		fs.open(sFileName.c_str(),ios::in);
		if(!fs.fail())
		{
			bool oldbmark = bByteMark;
			vHexdoc = vEmpty;
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			read_script_file(fs,vHexdoc);
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			fs.close();
			OPENFILENAME ofn2;
			szTitleName[0] =cFileName[0] = '\0';
			ofn2.lStructSize = sizeof (OPENFILENAME);
			ofn2.hwndOwner = hwnd;
			ofn2.hInstance = NULL;
			ofn2.lpstrFilter = "Text Files (*.TXT)\0*.TXT\0Script Files (*.SCP)\0*.SCP\0All Files (*.*)\0*.*\0\0";
			ofn2.lpstrCustomFilter = NULL;
			ofn2.nMaxCustFilter = 0;
			ofn2.nFilterIndex = 0;
			ofn2.lpstrFile = cFileName;
			ofn2.nMaxFile = _MAX_PATH;
			ofn2.lpstrFileTitle = szTitleName;
			ofn2.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
			ofn2.lpstrInitialDir = NULL;
			ofn2.lpstrTitle = "Save Optimized Script As";
			ofn2.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;
			ofn2.nFileOffset = 0;
			ofn2.nFileExtension = 0;
			ofn2.lpstrDefExt = NULL;
			ofn2.lCustData = 0L;
			ofn2.lpfnHook = NULL;
			ofn2.lpTemplateName = NULL;
			if (GetSaveFileName (&ofn2))
			{
				int cur = strlen(cFileName) - 4;
				if(uppercase(((string) cFileName).substr(((string) cFileName).length()-4,4)) == ".TXT")
				{
					sFileName = (string) cFileName;
				}
				else
				{
					sFileName = (string) strcat(cFileName,".txt");
				}
				fs.open(sFileName.c_str(),ios::out);
				if(!fs.fail())
				{
					SetCursor (LoadCursor (NULL, IDC_WAIT));
					HWND progressdialog = CreateDialog(hInstance, MAKEINTRESOURCE (IDD_6PROGRESSDIALOG),hwnd,ProgressDialogProc);
					SetWindowText(progressdialog,"Script Optimization Progress...");
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS1TITLE),"Removing Formatting...");
					progress1 = GetDlgItem(progressdialog,IDC_PROGRESS1);
					progress2 = GetDlgItem(progressdialog,IDC_PROGRESS2);
					SetProgressColor(progress1,RGB(255,0,0));
					SetProgressColor(progress2,RGB(255,128,0));
					numbytes = vHexdoc.size();
					SetProgressRange(progress1,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress1,iIncrementStep);  
					bByteMark = true;
					descript(vHexdoc);
					SetProgressPos(progress1,numbytes);
					iProgressPos = 0;
					numbytes = vHexdoc.size();
					SetProgressRange(progress2,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress2,iIncrementStep);  
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS2TITLE),"Translating...");
					detranslate(vHexdoc);
					SetProgressPos(progress2,numbytes);
					progress1 = NULL;
					progress2 = NULL;
					iIncrementStep = 0;
					iProgressPos = 0;
					long filesize = vHexdoc.size();
					bByteMark = false;
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS3TITLE),"Phase 1...");
					progress1 = GetDlgItem(progressdialog,IDC_PROGRESS3);
					progress2 = GetDlgItem(progressdialog,IDC_PROGRESS4);
					SetProgressColor(progress1,RGB(255,255,0));
					SetProgressColor(progress2,RGB(0,255,0));
					numbytes = vHexdoc.size();
					SetProgressRange(progress1,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress1,iIncrementStep);  
					translate(vHexdoc);
					SetProgressPos(progress1,numbytes);
					iProgressPos = 0;
					numbytes = vHexdoc.size();
					SetProgressRange(progress2,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress2,iIncrementStep);  
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS4TITLE),"Phase 2...");
					enscript(vHexdoc);
					SetProgressPos(progress2,numbytes);
					progress1 = NULL;
					progress2 = NULL;
					iIncrementStep = 0;
					iProgressPos = 0;
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS5TITLE),"Phase 3...");
					progress1 = GetDlgItem(progressdialog,IDC_PROGRESS5);
					progress2 = GetDlgItem(progressdialog,IDC_PROGRESS6);
					SetProgressColor(progress1,RGB(0,255,255));
					SetProgressColor(progress2,RGB(128,0,255));
					numbytes = vHexdoc.size();
					SetProgressRange(progress1,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress1,iIncrementStep);  
					descript(vHexdoc);
					SetProgressPos(progress1,numbytes);
					iProgressPos = 0;
					numbytes = vHexdoc.size();
					SetProgressRange(progress2,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress2,iIncrementStep);  
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS6TITLE),"Phase 4...");
					detranslate(vHexdoc);
					SetProgressPos(progress2,numbytes);
					progress1 = NULL;
					progress2 = NULL;
					iIncrementStep = 0;
					iProgressPos = 0;
					long filesize2 = vHexdoc.size();
					bByteMark = true;
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS7TITLE),"Translating...");
					progress1 = GetDlgItem(progressdialog,IDC_PROGRESS7);
					progress2 = GetDlgItem(progressdialog,IDC_PROGRESS8);
					numbytes = vHexdoc.size();
					SetProgressRange(progress1,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress1,iIncrementStep);  
					translate(vHexdoc);
					SetProgressPos(progress1,numbytes);
					iProgressPos = 0;
					numbytes = vHexdoc.size();
					SetProgressRange(progress2,0,vHexdoc.size());
					if(vHexdoc.size() >= 10000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 1000;
					}
					else if(vHexdoc.size() >= 1000)//this could be better
					{
						iIncrementStep = vHexdoc.size() / 100;
					}
					else if(vHexdoc.size() >= 100)
					{
						iIncrementStep = vHexdoc.size() / 10;
					}
					else
					{
						iIncrementStep = 2; 
					}
					SetProgressStep(progress2,iIncrementStep);  
					SetWindowText(GetDlgItem(progressdialog,IDC_PROGRESS8TITLE),"Formatting...");
					enscript(vHexdoc);
					SetProgressPos(progress2,numbytes);
					progress1 = NULL;
					progress2 = NULL;
					iIncrementStep = 0;
					iProgressPos = 0;
					bByteMark = oldbmark;
					DestroyWindow(progressdialog);
					write_script_file(fs,vHexdoc);
					SetCursor (LoadCursor (NULL, IDC_ARROW));
					fs.close();
					optimized++;
					ppair.x = filesize;
					ppair.y = filesize2;
					DialogBox (hInstance, MAKEINTRESOURCE (IDD_OPTIMIZERESULTSDIALOG), hwnd, (DLGPROC) InfoDialogProc);
					//SetFocus(progressdialog);
					//SetWindowText(progressdialog,"Script Optimization Progress...");
					//MessageBox(hwnd,("The input script file had " + stringvalue(filesize) + " tile-bytes." +
					//	"\nThe output script file has been crunched to " + stringvalue(filesize2) + " tile-bytes.").c_str(),"Notice:",MB_OK | MB_ICONINFORMATION);
					progress1 = NULL;
					progress2 = NULL;
					iIncrementStep = 0;
					iProgressPos = 0;
				}
				else
				{
					MessageBox(hwnd,"Error occurred while creating output script file.","Optimize script error:",MB_OK | MB_ICONWARNING);
					fs.close();
					return;
				}	
			}
			
		}
		else
		{
			MessageBox(hwnd,"Error occurred while reading input script file.","Optimize script error:",MB_OK | MB_ICONWARNING);
			fs.close();
			return;
		}
	}
}

//OPTIONS
int Translhextion::view_settings()
{
	TxtEditName = sTexteditorName;
	iBPLSetting = iBytesPerLine;
	bAutomaticXAdjust = bAutomaticBPL;
	iOffsetLengthSetting = iOffsetLength;
	bUnsignedViewSetting = bUnsignedView;
	iOpenReadOnlySetting = bOpenReadOnly;

	iCharacterSetting = iCharacterSet;
	iFontSizeSetting = iFontSize;

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_VIEWSETTINGSDIALOG), hwnd, (DLGPROC) ViewSettingsDialogProc))
	{
		sTexteditorName = TxtEditName;
		if (iOffsetLength>0 && iOffsetLength<=8)
			iOffsetLength = iOffsetLengthSetting;
		iBytesPerLine = iBPLSetting;
		bOldAutomaticBPL = false;
		if(iBytesPerLine > 1 && iBytesPerLine % 2 != 0)
			iBytesPerLine--;
		bAutomaticBPL = bAutomaticXAdjust;
		bUnsignedView = bUnsignedViewSetting;
		bOpenReadOnly = iOpenReadOnlySetting;
		
		iCharacterSet = iCharacterSetting;
		iFontSize = iFontSizeSetting;
		save_ini_data ();
		update_size_change ();
		kill_focus ();
		set_focus ();
	}
	return 1;
}

BOOL CALLBACK ViewSettingsDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			HKEY hkeyKEY;
				LONG iResourceKey = RegOpenKeyEx( HKEY_CLASSES_ROOT,
				("*\\shell\\Open in " + sName + "\\command").c_str(),
				0,
				KEY_ALL_ACCESS,
				&hkeyKEY );
			if( iResourceKey == ERROR_SUCCESS )
			{
				CheckDlgButton(hDialog,IDC_CHECK6,TRUE);
			}
			RegCloseKey( hkeyKEY );
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT6), sByteOpen.c_str());
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT7), sByteClose.c_str());
			if(bDefByteMark)
			{
				CheckDlgButton(hDialog,IDC_CHECK7,TRUE);	
			}
			bContextMenu = (bool) IsDlgButtonChecked(hDialog,IDC_CHECK6);
			char cBuf[32];
			sprintf (cBuf, "%d", iBPLSetting);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), cBuf);
			sprintf (cBuf, "%d", iOffsetLengthSetting);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT2), cBuf);
			CheckDlgButton (hDialog, IDC_RADIO5, bAutomaticXAdjust);
			CheckDlgButton (hDialog, IDC_RADIO4, !bAutomaticXAdjust);
			SetFocus(hDialog);
			if( bUnsignedViewSetting )
				CheckDlgButton( hDialog, IDC_RADIO1, BST_CHECKED );
			else
				CheckDlgButton( hDialog, IDC_RADIO2, BST_CHECKED );
			
			if( iOpenReadOnlySetting )
				CheckDlgButton( hDialog, IDC_CHECK5, BST_CHECKED );
			else
				CheckDlgButton( hDialog, IDC_CHECK5, BST_UNCHECKED );
			if( bDiscardChangesSave )
				CheckDlgButton( hDialog, IDC_CHECK1, BST_CHECKED );
			else
				CheckDlgButton( hDialog, IDC_CHECK1, BST_UNCHECKED );
			SetWindowText( GetDlgItem( hDialog, IDC_EDIT3 ), TxtEditName.c_str() );
			SetWindowText( GetDlgItem( hDialog, IDC_EDIT4 ), sBrowserName.c_str() );

			char buf2[16];
			sprintf (buf2, "%d", iFontSizeSetting);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT5), buf2);
			switch (iCharacterSetting)
			{
			case ANSI_FIXED_FONT:
				CheckDlgButton (hDialog, IDC_RADIO9, BST_CHECKED);
				break;

			case OEM_FIXED_FONT:
				CheckDlgButton (hDialog, IDC_RADIO10, BST_CHECKED);
				break;
			}
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
			case IDOK:
			{
				char sbuf[2];
				if(GetDlgItemText(hDialog,IDC_EDIT6,sbuf,2))
				{
					sByteOpen = (string) sbuf;
				}
				if(GetDlgItemText(hDialog,IDC_EDIT7,sbuf,2))
				{
					sByteClose = (string) sbuf;
				}
				if(IsDlgButtonChecked(hDialog,IDC_CHECK7))
				{
					bDefByteMark = true;
				}
				else
				{
					bDefByteMark = false;
				}
				char cBuf[512];
				int i=-1;
				if (GetDlgItemText (hDialog, IDC_EDIT1, cBuf, 512) != 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Number of bytes is not recognized.", "Settings error:", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDialog, 0);
					return 0;
				}
				iBPLSetting = i;
				if (GetDlgItemText (hDialog, IDC_EDIT2, cBuf, 512) != 0)
				{
					if (sscanf (cBuf, "%d", &i) == 0)
					{
						MessageBox (hDialog, "Lengthgth of offset is not recognized.", "Settings error:", MB_OK | MB_ICONERROR);						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDialog, 0);
					return 0;
				}
				iOffsetLengthSetting = i;
				if( GetDlgItemText( hDialog, IDC_EDIT3, cBuf, 512 ) != 0 )
				{
					TxtEditName = (string) cBuf;
				}
				else
				{
					MessageBox( hDialog, "Text editor name was not specified: default name not changed.", "Notice:", MB_OK | MB_ICONINFORMATION );
				}
				if( GetDlgItemText( hDialog, IDC_EDIT4, cBuf, 512 ) != 0 )
				{
					sBrowserName = cBuf;
				}
				else
				{
					MessageBox( hDialog, "Internet Browser name was not specified: default name not changed.", "Notice:", MB_OK | MB_ICONINFORMATION );
				}
				if (iBPLSetting < 1)
					iBPLSetting = 1;
				bAutomaticXAdjust = (bool) IsDlgButtonChecked (hDialog, IDC_RADIO5);
				if (IsDlgButtonChecked (hDialog, IDC_RADIO1))
					bUnsignedViewSetting = TRUE;
				else
					bUnsignedViewSetting = FALSE;
				
				if( IsDlgButtonChecked( hDialog, IDC_CHECK5 ) )
					iOpenReadOnlySetting = TRUE;
				else
					iOpenReadOnlySetting = FALSE;
				if(IsDlgButtonChecked( hDialog, IDC_CHECK1 ) )
					bDiscardChangesSave = true;
				else
					bDiscardChangesSave = false;
				int j;
				char buf2[16];
				if (GetDlgItemText (hDialog, IDC_EDIT5, buf2, 16) != 0)
				{
					if (sscanf (buf2, "%d", &j) == 0)
					{
						MessageBox (hDialog, "Font size is not recognized.", "Settings error:", MB_OK | MB_ICONERROR);
						j = -1;
					}
				}
				if (j != -1)
					iFontSizeSetting = j;
				if (IsDlgButtonChecked (hDialog, IDC_RADIO9) == BST_CHECKED)
					iCharacterSetting = ANSI_FIXED_FONT;
				else
					iCharacterSetting = OEM_FIXED_FONT;

				if(bContextMenu == (bool) IsDlgButtonChecked(hDialog,IDC_CHECK6))
				{
					;
				}
				else
				{
					HKEY hkeyKEY;
						LONG iResourceKey = RegOpenKeyEx( HKEY_CLASSES_ROOT,
						("*\\shell\\Open in " + sName + "\\command").c_str(),
						0,
						KEY_ALL_ACCESS,
						&hkeyKEY );
					if( iResourceKey == ERROR_SUCCESS )
					{
						RegCloseKey( hkeyKEY );
						if( MessageBox( hDialog,
							(sName + " is installed in the Windows Explorer context menu.\n"
							"Are you sure you want to remove it from there?").c_str(),
							"Notice:",
								MB_ICONQUESTION | MB_YESNO ) == IDYES )
							{
								if( RegDeleteKey( HKEY_CLASSES_ROOT, ("*\\shell\\Open in " + sName + "\\command").c_str() ) ==  ERROR_SUCCESS
									&& RegDeleteKey( HKEY_CLASSES_ROOT, ("*\\shell\\Open in " + sName).c_str()) ==  ERROR_SUCCESS )
								{
									MessageBox( hDialog, (sName + " was removed from the Windows Explorer context menu.").c_str(), "Notice:", MB_OK | MB_ICONINFORMATION );
								}
								else
								{
									MessageBox( hDialog, (sName + " could not be removed from the Windows Explorer context menu.").c_str(), "Context menu error:", MB_OK | MB_ICONERROR );
								}
							}
						else
							CheckDlgButton(hDialog,IDC_CHECK6,TRUE);
					}
					else
					{
									
						if( MessageBox( hDialog,
							("This will install an \"Open in " + sName + "\" command in the Windows Explorer context menu.  \nAre you sure you want to do this?").c_str(),
							"Notice:", MB_ICONQUESTION | MB_YESNO ) == IDYES )
						{
							HKEY hkeyKEY;
							LONG iResourceKey = RegCreateKey( HKEY_CLASSES_ROOT,
								("*\\shell\\Open in " + sName + "\\command").c_str(),
								&hkeyKEY );
							if( iResourceKey == ERROR_SUCCESS )
							{
								char exepath[ _MAX_PATH ];
								strcpy( exepath, _pgmptr );
								string str1 = exepath;
								str1 += " %1";
								RegSetValue( hkeyKEY, NULL, REG_SZ, str1.c_str(), str1.length() + 1 );
								MessageBox( hDialog, (sName + " is now installed in the Explorer context menu.\n"
									"To remove " + sName + " from the context menu choose this menu item again.").c_str(),
									"Install in context menu", MB_OK | MB_ICONINFORMATION );
								}
							else
							{
								MessageBox( hDialog, (sName + " could not be installed in the Explorer context menu").c_str(), "Context menu error:", MB_OK | MB_ICONERROR );
							}
						}
						else
						CheckDlgButton(hDialog,IDC_CHECK6,FALSE);
					}
				}
				EndDialog (hDialog, 1);
				return TRUE;
			}
			case IDCANCEL:
			{
				EndDialog (hDialog, 0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

int Translhextion::color_settings ()
{
 	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_COLORSETTINGSDIALOG), hwnd, (DLGPROC) ColorSettingsDialogProc))
	{
		iSelBkColorValue = cSelBkColor;
		iSelTextColorValue = cSelTextColor;
		iBookmarkColor = cBookmarkColor;
		iTextColorValue = cTextColor;
		iBkColorValue = cBkColor;
		iTextColorEValue = cTextColorE;
		iTextColorOValue = cTextColorO;
		iSepColorValue = cSepColor;
		iChangesColorValue = cChangesColor;
		iDTEColorValue = cDTEColor;
		iHighlightColorValue = cHighlightColor;
		save_ini_data ();
		update_size_change ();
		repaint ();
	}
	return 0;
}

BOOL CALLBACK ColorSettingsDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC (hDialog);
	HBRUSH brush;
	RECT swatch;
	int topx = 15;
	int topy = 15;
	int offset = 22;
	int space = 14;
	int radiochoice = 0;
	COLORREF outline = RGB(0,0,0);

	SetRect(&swatch, topx, topy, topx + offset, topy + offset); 
	brush = CreateSolidBrush (cTextColorO);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);
	
	SetRect(&swatch, topx + 10 * space, topy, topx + offset + 10 * space, topy + offset); 
	brush = CreateSolidBrush (cTextColorE);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx, topy + offset + space, topx + offset, topy + 2 * offset + space); 
	brush = CreateSolidBrush (cTextColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx + 10 * space, topy + offset + space, topx + offset + 10 * space, topy + 2 * offset + space); 
	brush = CreateSolidBrush (cBkColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx, topy + 2 * offset + 2 * space, topx + offset, topy + 3 * offset + 2 * space); 
	brush = CreateSolidBrush (cSelTextColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx + 10 * space, topy + 2 * offset + 2 * space, topx + offset + 10 * space, topy + 3 * offset + 2 * space); 
	brush = CreateSolidBrush (cSelBkColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx, topy + 3 * offset + 3 * space, topx + offset, topy + 4 * offset + 3 * space); 
	brush = CreateSolidBrush (cChangesColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx + 10 * space, topy + 3 * offset + 3 * space, topx + offset + 10 * space, topy + 4 * offset + 3 * space); 
	brush = CreateSolidBrush (cHighlightColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx, topy + 4 * offset + 4 * space, topx + offset, topy + 5 * offset + 4 * space); 
	brush = CreateSolidBrush (cBookmarkColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx + 10 * space, topy + 4 * offset + 4 * space, topx + offset + 10 * space, topy + 5 * offset + 4 * space); 
	brush = CreateSolidBrush (cSepColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	SetRect(&swatch, topx, topy + 5 * offset + 5 * space, topx + offset, topy + 6 * offset + 5 * space); 
	brush = CreateSolidBrush (cDTEColor);      
	FillRect(hdc,&swatch,brush);
	DeleteObject(brush);
	brush = CreateSolidBrush (outline);      
	DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
	DeleteObject(brush);

	ReleaseDC (hDialog, hdc);
	switch (iMsg)
	{
		case WM_INITDIALOG:
		{
			CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			SetFocus(hDialog);
			return TRUE;
		}
		case WM_COMMAND:
		{
			if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
				radiochoice = 0;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO2) == BST_CHECKED)
				radiochoice = 1;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO3) == BST_CHECKED)
				radiochoice = 2;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO4) == BST_CHECKED)
				radiochoice = 3;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO5) == BST_CHECKED)
				radiochoice = 4;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO6) == BST_CHECKED)
				radiochoice = 5;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO7) == BST_CHECKED)
				radiochoice = 6;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO8) == BST_CHECKED)
				radiochoice = 7;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO9) == BST_CHECKED)
				radiochoice = 8;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO10) == BST_CHECKED)
				radiochoice = 9;
			else if (IsDlgButtonChecked (hDialog, IDC_RADIO11) == BST_CHECKED)
				radiochoice = 10;
			else
			{
				CheckDlgButton(hDialog, IDC_RADIO1, BST_CHECKED);
				radiochoice = 0;
			}
			switch (LOWORD (wParam))
			{
				case IDOK:
				{
					EndDialog (hDialog, 1);
					return TRUE;
				}
				break;
				case IDC_CHANGE:
				{
					CHOOSECOLOR cc;
					COLORREF crCustColors[16];
					cc.lStructSize = sizeof (CHOOSECOLOR);
					cc.hwndOwner = hDialog;
					cc.hInstance = NULL;
					if(radiochoice == 0)
						cc.rgbResult = cTextColorO;
					else if(radiochoice == 1)
						cc.rgbResult = cTextColorE;
					else if(radiochoice == 2)
						cc.rgbResult = cTextColor;
					else if(radiochoice == 3)
						cc.rgbResult = cBkColor;
					else if(radiochoice == 4)
						cc.rgbResult = cSelTextColor;
					else if(radiochoice == 5)
						cc.rgbResult = cSelBkColor;
					else if(radiochoice == 6)
						cc.rgbResult = cChangesColor;
					else if(radiochoice == 7)
						cc.rgbResult = cHighlightColor;
					else if(radiochoice == 8)
						cc.rgbResult = cBookmarkColor;
					else if(radiochoice == 9)
						cc.rgbResult = cSepColor;
					else if(radiochoice == 10)
						cc.rgbResult = cDTEColor;
					cc.lpCustColors = crCustColors;
					cc.Flags = CC_FULLOPEN | CC_RGBINIT;
					cc.lCustData = 0L;
					cc.lpfnHook;
					cc.lpTemplateName = NULL;
					if (ChooseColor (&cc))
					{
						if(radiochoice == 0)
							cTextColorO = cc.rgbResult;
						else if(radiochoice == 1)
							cTextColorE = cc.rgbResult;
						else if(radiochoice == 2)
							cTextColor = cc.rgbResult;
						else if(radiochoice == 3)
							cBkColor = cc.rgbResult;
						else if(radiochoice == 4)
							cSelTextColor = cc.rgbResult;
						else if(radiochoice == 5)
							cSelBkColor = cc.rgbResult;
						else if(radiochoice == 6)
							cChangesColor = cc.rgbResult;
						else if(radiochoice == 7)
							cHighlightColor = cc.rgbResult;
						else if(radiochoice == 8)
							cBookmarkColor = cc.rgbResult;
						else if(radiochoice == 9)
							cSepColor = cc.rgbResult;
						else if(radiochoice == 10)
							cDTEColor = cc.rgbResult;
					
							HDC hdc = GetDC (hDialog);
							
							SetRect(&swatch, topx, topy, topx + offset, topy + offset); 
							brush = CreateSolidBrush (cTextColorO);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx + 10 * space, topy, topx + offset + 10 * space, topy + offset); 
							brush = CreateSolidBrush (cTextColorE);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx, topy + offset + space, topx + offset, topy + 2 * offset + space); 
							brush = CreateSolidBrush (cTextColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx + 10 * space, topy + offset + space, topx + offset + 10 * space, topy + 2 * offset + space); 
							brush = CreateSolidBrush (cBkColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx, topy + 2 * offset + 2 * space, topx + offset, topy + 3 * offset + 2 * space); 
							brush = CreateSolidBrush (cSelTextColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx + 10 * space, topy + 2 * offset + 2 * space, topx + offset + 10 * space, topy + 3 * offset + 2 * space); 
							brush = CreateSolidBrush (cSelBkColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx, topy + 3 * offset + 3 * space, topx + offset, topy + 4 * offset + 3 * space); 
							brush = CreateSolidBrush (cChangesColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx + 10 * space, topy + 3 * offset + 3 * space, topx + offset + 10 * space, topy + 4 * offset + 3 * space); 
							brush = CreateSolidBrush (cHighlightColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx, topy + 4 * offset + 4 * space, topx + offset, topy + 5 * offset + 4 * space); 
							brush = CreateSolidBrush (cBookmarkColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx + 10 * space, topy + 4 * offset + 4 * space, topx + offset + 10 * space, topy + 5 * offset + 4 * space); 
							brush = CreateSolidBrush (cSepColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							SetRect(&swatch, topx, topy + 5 * offset + 5 * space, topx + offset, topy + 6 * offset + 5 * space); 
							brush = CreateSolidBrush (cDTEColor);      
							FillRect(hdc,&swatch,brush);
							DeleteObject(brush);
							brush = CreateSolidBrush (outline);      
							DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
							DeleteObject(brush);

							ReleaseDC(hDialog,hdc);
					}
				}
				break;
				case IDC_DEFAULT:
				{
					if( MessageBox( hDialog, "Are you sure you want to reset all colors to their default values?", "Notice:", MB_YESNO | MB_ICONQUESTION ) == IDYES )
					{
						cTextColor = RGB (0,0,0);
						cBkColor = RGB (255,255,255);
						cTextColorE = RGB(0,0,255);
						cTextColorO = RGB(255,0,0);
						cSepColor = RGB (192,192,192);
						cSelBkColor = RGB( 255, 255, 0 );
						cSelTextColor = RGB( 0, 0, 0 );
						cBookmarkColor = RGB( 255, 0, 0 );
						cHighlightColor = RGB(180,0,150);
						cChangesColor = RGB(255,150,0);
						cDTEColor = RGB(120,0,215);

						HDC hdc = GetDC (hDialog);

						SetRect(&swatch, topx, topy, topx + offset, topy + offset); 
						brush = CreateSolidBrush (cTextColorO);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx + 10 * space, topy, topx + offset + 10 * space, topy + offset); 
						brush = CreateSolidBrush (cTextColorE);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx, topy + offset + space, topx + offset, topy + 2 * offset + space); 
						brush = CreateSolidBrush (cTextColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx + 10 * space, topy + offset + space, topx + offset + 10 * space, topy + 2 * offset + space); 
						brush = CreateSolidBrush (cBkColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx, topy + 2 * offset + 2 * space, topx + offset, topy + 3 * offset + 2 * space); 
						brush = CreateSolidBrush (cSelTextColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx + 10 * space, topy + 2 * offset + 2 * space, topx + offset + 10 * space, topy + 3 * offset + 2 * space); 
						brush = CreateSolidBrush (cSelBkColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx, topy + 3 * offset + 3 * space, topx + offset, topy + 4 * offset + 3 * space); 
						brush = CreateSolidBrush (cChangesColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx + 10 * space, topy + 3 * offset + 3 * space, topx + offset + 10 * space, topy + 4 * offset + 3 * space); 
						brush = CreateSolidBrush (cHighlightColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx, topy + 4 * offset + 4 * space, topx + offset, topy + 5 * offset + 4 * space); 
						brush = CreateSolidBrush (cBookmarkColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx + 10 * space, topy + 4 * offset + 4 * space, topx + offset + 10 * space, topy + 5 * offset + 4 * space); 
						brush = CreateSolidBrush (cSepColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						SetRect(&swatch, topx, topy + 5 * offset + 5 * space, topx + offset, topy + 6 * offset + 5 * space); 
						brush = CreateSolidBrush (cDTEColor);      
						FillRect(hdc,&swatch,brush);
						DeleteObject(brush);
						brush = CreateSolidBrush (outline);      
						DrawEdge(hdc,&swatch,BDR_RAISEDOUTER,BF_RECT);
						DeleteObject(brush);

						ReleaseDC(hDialog,hdc);
					}
				}
				break;
				default:return 1;
			}
		}
		break;
	}
	return FALSE;
}

int Translhextion::binarymode ()
{
	iBinaryModeSetting = iBinaryMode;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_BINARYMODEDIALOG), hwnd, (DLGPROC) BinaryModeDialogProc))
	{
		iBinaryMode = iBinaryModeSetting;
		repaint ();
	}
	return 1;
}


BOOL CALLBACK BinaryModeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			if (iBinaryModeSetting == LITTLE_ENDIAN)
				CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			else
				CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
			SetFocus(hDialog);
			
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
					iBinaryModeSetting = LITTLE_ENDIAN;
				else
					iBinaryModeSetting = BIG_ENDIAN;
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


int Translhextion::offsetmode ()
{
	
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_OFFSETMODEDIALOG), hwnd, (DLGPROC) OffsetModeDialogProc))
	{
		
		repaint ();
	}
	return 1;
}


BOOL CALLBACK OffsetModeDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			if (iOffsetMode == STANDARDHEX)
				CheckDlgButton (hDialog, IDC_RADIO1, BST_CHECKED);
			else if(iOffsetMode == LOROM)
				CheckDlgButton (hDialog, IDC_RADIO2, BST_CHECKED);
			else
				CheckDlgButton (hDialog, IDC_RADIO3, BST_CHECKED);
			SetWindowText (GetDlgItem (hDialog, IDC_EDIT1), stringvalue(SNESHEADER).c_str());
			SetFocus(hDialog);
			
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				if (IsDlgButtonChecked (hDialog, IDC_RADIO1) == BST_CHECKED)
					iOffsetMode = STANDARDHEX;
				else if(IsDlgButtonChecked (hDialog, IDC_RADIO2) == BST_CHECKED)
					iOffsetMode = LOROM;
				else
					iOffsetMode = HIROM;
				int len = SendMessage (GetDlgItem (hDialog, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
				char * num = new char[len];
				int result = GetDlgItemText (hDialog, IDC_EDIT1, num, len);
				string x = (string) num;
				delete [] num;
				num = NULL;
				SNESHEADER = intvalue(x);
				EndDialog (hDialog, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
//HELP
BOOL CALLBACK AboutDialogProc (HWND hDialog, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			SetFocus(hDialog);
			SetWindowText( GetDlgItem( hDialog, IDC_EDIT1 ),
				sEmail.c_str());
			SetWindowText( GetDlgItem( hDialog, IDC_BUTTON1 ), sHomepage.c_str() );
			return TRUE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog (hDialog, 0);
			return TRUE;
		
		case IDC_BUTTON1:
			{
				HINSTANCE hi = ShellExecute( hMainWnd, "open", sBrowserName.c_str(), sHomepage.c_str(), NULL, SW_SHOWNORMAL );
				if( (int) hi <= 32 )
					MessageBox( hMainWnd, "Error occurred while trying open web browser.", "Go to homepage error:", MB_OK | MB_ICONERROR );
			}
		}
		break;
	}
	return FALSE;
}

