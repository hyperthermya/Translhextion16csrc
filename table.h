#ifndef _TABLE_H
#define _TABLE_H

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

#include "hex.h"
#include "progress.h"

inline long dualtileindex(string dualtile)
{
	return (unsigned char) dualtile[0] * 256 + (unsigned char) dualtile[1];
}

inline long dualbyteindex(string dualbyte)
{
	return (int) char_hex(dualbyte.substr(0,2)) * 256 + (int) char_hex(dualbyte.substr(2,2));
}

// dualbyteindex from 4 raw characters — zero string allocations
inline long dualbyteindex_fast(char h0, char l0, char h1, char l1) {
	return (long)char_hex_fast(h0,l0) * 256 + (long)char_hex_fast(h1,l1);
}

inline long toHIROM(long offset)
{
	return (offset - SNESHEADER) + 12582912;
}

inline long fromHIROM(long offset)
{
	return (offset - 12582912) + SNESHEADER;
}

inline long toLOROM(long offset)
{
	return (offset - SNESHEADER) + 32768;
}

inline long fromLOROM(long offset)
{
	return (offset - 32768) + SNESHEADER;
}

inline void insert_longest_to_shortest(vector<tablevalue>& x,tablevalue& y)
{
	if(x.size() == 0)
	{
		x.push_back(y);
		return;
	}
	if(x.size() == 1)
	{
		if(y.ascii.length() < x[0].ascii.length())
		{
			x.push_back(y);
		}
		else
		{
			x.push_front(y);
		}
		return;
	}
	else
	{
		int k = 0;
		for(k = 0;k<x.size();k++)
		{
			if(!(y.ascii.length() < x[k].ascii.length()))
			{
				break;
			}
		}
		if(k == x.size())
		{
			x.push_back(y);

		}
		else
		{
			x.insert(y,k);
		}
		return;
	}
}

//takes buffer
//translates hex in buffer to chars
// pProc: optional char array marking already-processed positions (for parallel use)
// iFrom/iTo: range to process; iTo=-1 means data.size()
// doCompact: run the empty-string compaction at the end
void translate(vector<string>& data, char* pProc = 0, int iFrom = 0, int iTo = -1, bool doCompact = true)
{
	int i = 0;
	int j = 0;
	int index = -1;
	bool done = false;
	bool puncswitched = false;
	bool oldbjpuncswitch = bJPuncSwitch;
	if(iTo < 0) iTo = (int)data.size();
	if(tvHandakuten.hex.length() == 0 && tvDakuten.hex.length() == 0)
	{
		bJPuncSwitch = false;
	}
	for(i = iFrom;i < iTo;i++)
	{
		if(pProc && pProc[i]) continue;
		if(bCancelDump) break;
		done = false;
		if(!pProc) {  // UI updates only on the main thread (pProc==NULL means sequential call)
			iProgressPos++;
			if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
			{
				StepProgress(progress1);
				MSG _msg;
				while(PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE)) {
					TranslateMessage(&_msg);
					DispatchMessage(&_msg);
				}
			}
		}
		if(vLinked.size() > 0 && i + 1 < iTo)
		{
			int k = 0;
			int l = 0;
			for(k = 0;k<vLinked.size();k++)
			{
				if(data[i] == vLinked[k].hex && i + vLinked[k].links < iTo)
				{
					data[i] = sByteOpen + data[i] + sByteClose;
					for(l = 0;l<vLinked[k].links;l++)
					{
						data[i+1+l] = sByteOpen + data[i+1+l] + sByteClose;
					}
					i = i + l;
					k = vLinked.size();
					done = true;
				}
			}
		}
		if(!done && bJPuncSwitch && !puncswitched && i+(tvDakuten.hex.length()/2) < iTo)
		{
			int k = 0;
			string temp;
			int numbytes = tvDakuten.hex.length()/2;
			if(numbytes == 1)
			{
				temp = data[i];
			}
			else if(numbytes == 2)
			{
				temp = data[i];
				temp += data[i+1];
			}
			else if(numbytes == 3)
			{
				temp = data[i];
				temp += data[i+1];
				temp += data[i+2];
			}
			if(temp == tvDakuten.hex)
			{
				for(k = 0;k<numbytes;k++)
				{
					temp = data[i];
					data[i] = data[i+1+k];
					data[i+1+k] = temp;
				}
				puncswitched = true;
				bSeemsJ = true;
			}
		}
		if(!done && bJPuncSwitch && !puncswitched && i+(tvHandakuten.hex.length()/2) < iTo)
		{
			int k = 0;
			string temp;
			int numbytes = tvHandakuten.hex.length()/2;
			if(numbytes == 1)
			{
				temp = data[i];
			}
			else if(numbytes == 2)
			{
				temp = data[i];
				temp += data[i+1];
			}
			else if(numbytes == 3)
			{
				temp = data[i];
				temp += data[i+1];
				temp += data[i+2];
			}
			if(temp == tvHandakuten.hex)
			{
				for(k = 0;k<numbytes;k++)
				{
					temp = data[i];
					data[i] = data[i+1+k];
					data[i+1+k] = temp;
				}
				puncswitched = true;
				bSeemsJ = true;
			}
		}
		for(j = 0;!done && (i+2< iTo) && (j < vMultiByteTableOut.size());j++)
		{
			if(data[i] + data[i+1] + data[i+2] == vMultiByteTableOut[j].hex)
			{	
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i+2] = "";
					if(bCleanScript || !bByteMark)
					{
						data[i] = vMultiByteTableOut[j].ascii;
					}
					else
					{
						data[i] = sByteOpen + vMultiByteTableOut[j].ascii + sByteClose;
					}
					i += 2;
				}
				else
				{
					data[i+1] = "";
					data[i+2] = "";
					if(bCleanScript || !bByteMark)
					{
						data[i] = vMultiByteTableOut[j].ascii;
					}
					else
					{
						data[i] = sByteOpen + vMultiByteTableOut[j].ascii + sByteClose;
					}
					i = i + 2;
				}
				done = true;
			    break;
			}
		}
		if(data[i] == "")
		{
			done = true;
		}//problems next iLine
		if(!done && (i+1<iTo) && data[i].length()==2 && data[i+1].length()==2)
		{
			long dbIdx = dualbyteindex_fast(data[i][0],data[i][1],data[i+1][0],data[i+1][1]);
			if(vDualByteTableOut[dbIdx] != "")
			{
				if(bCleanScript || !bByteMark)
				{
					data[i] = vDualByteTableOut[dbIdx];
				}
				else
				{
					data[i] = sByteOpen + vDualByteTableOut[dbIdx] + sByteClose;
				}
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					i += 1;
				}
				else
				{
					data[i+1] = "";
					i = i + 1;
				}
				done = true;
			}
		}
		if(!done)
		{
			if(sNewLine.length() > 4 && (i+2 < iTo) && data[i] + data[i+1] + data[i+2] == sNewLine)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = "\n";
					i += 2;
				}
				else
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = "\n";
					i = i + 2;
				}
				done = true;
			}
			else if(sNewLine.length() > 2 && (i+1 < iTo) && data[i] + data[i+1] == sNewLine)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i] = "\n";
					i += 1;
				}
				else
				{
					data[i+1] = "";
					data[i] = "\n";
					i = i + 1;
				}
				done = true;
			}
			else if(data[i].substr(0,2) == sNewLine)
			{
				data[i] = "\n";
				done = true;
			}
			if(!done && sNull.length() > 4 && (i+2 < iTo) && data[i] + data[i+1] + data[i+2] == sNull)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = sByteOpen + "END" + sByteClose;
					i += 2;
				}
				else
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = sByteOpen + "END" + sByteClose;
					i = i + 2;
				}
				done = true;
			}
			else if(!done && sNull.length() > 2 && (i+1 < iTo) && data[i] + data[i+1] == sNull)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i] = sByteOpen + "END" + sByteClose;
					i += 1;
				}
				else
				{
					data[i+1] = "";
					data[i] = sByteOpen + "END" + sByteClose;
					i = i + 1;
				}
				done = true;
			}
			else if(!done && data[i].substr(0,2) == sNull)
			{
				data[i] = sByteOpen + "END" + sByteClose;
				done = true;
			}
			if(!done && tvDakuten.hex.length() > 4 && (i+2 < iTo) && data[i] + data[i+1] + data[i+2] == tvDakuten.hex)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = tvDakuten.ascii;
					i += 2;
				}
				else
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = tvDakuten.ascii;
					i = i + 2;
				}
				puncswitched = false;
				bSeemsJ = true;
				done = true;
			}
			else if(!done && tvDakuten.hex.length() > 2 && (i+1 < iTo) && data[i] + data[i+1] == tvDakuten.hex)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i] = tvDakuten.ascii;
					i += 1;
				}
				else
				{
					data[i+1] = "";
					data[i] = tvDakuten.ascii;
					i = i + 1;
				}
				puncswitched = false;
				bSeemsJ = true;
				done = true;
			}
			else if(!done && data[i].substr(0,2) == tvDakuten.hex)
			{
				data[i] = tvDakuten.ascii;
				puncswitched = false;
				bSeemsJ = true;
				done = true;
			}
			if(!done && tvHandakuten.hex.length() > 4 && (i+2 < iTo) && data[i] + data[i+1] + data[i+2] == tvHandakuten.hex)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = tvHandakuten.ascii;
					i += 2;
				}
				else
				{
					data[i+1] = "";
					data[i+2] = "";
					data[i] = tvHandakuten.ascii;
					i = i + 2;
				}
				puncswitched = false;
				bSeemsJ = true;
				done = true;
			}
			else if(!done && tvHandakuten.hex.length() > 2 && (i+1 < iTo) && data[i] + data[i+1] == tvHandakuten.hex)
			{
				if(bSplitMultiByte)
				{
					data[i+1] = "";
					data[i] = tvHandakuten.ascii;
					i += 1;
				}
				else
				{
					data[i+1] = "";
					data[i] = tvHandakuten.ascii;
					i = i + 1;
				}
				puncswitched = false;
				bSeemsJ = true;
				done = true;
			}
			else if(!done && data[i].substr(0,2) == tvHandakuten.hex)
			{
				data[i] = tvHandakuten.ascii;
				puncswitched = false;
				bSeemsJ = true;
				done = true;
			}
			if(!done && data[i].length()==2)
			{
				index = (int) char_hex_fast(data[i][0], data[i][1]);
				if(vTableOut[index].length() > 2)
				{
					if(bCleanScript || !bByteMark)
					{
						data[i] = vTableOut[index];
					}
					else
					{
						data[i] = sByteOpen + vTableOut[index] + sByteClose;
					}
					done = true;
				}
				if(!done && vTableOut[index].length() == 2)
				{
					if(bByteMark)
					{
						if(bCleanScript)
						{	
							data[i] = vTableOut[index];
						}
						else
						{
							data[i] = sByteOpen + vTableOut[index] + sByteClose;
						}
					}
					else
					{
						data[i] = vTableOut[index];
					}
					done = true;
				}
				if(!done && vTableOut[index] != "")
				{
					data[i] = vTableOut[index];
					done = true;
				}
				if(!done)
				{
					data[i] = sByteOpen + data[i] + sByteClose;
					done = true;
				}
			}
		}
		if(pProc) pProc[i] = 1;
	}
	if(tvHandakuten.hex.length() == 0 && tvDakuten.hex.length() == 0)
	{
		bJPuncSwitch = oldbjpuncswitch;
	}
	if(doCompact && bSplitMultiByte) {
		int w = 0;
		for(int k = 0; k < (int)data.size(); k++) {
			if(data[k].length() > 0) {
				if(w != k) data[w] = data[k];
				w++;
			}
		}
		while((int)data.size() > w) data.pop_back();
	}
}

//takes buffer
//translates chars to hex in buffer
void detranslate(vector<string>& data)
{
	string temp;
	vector<string> result;
	int i = 0;
	int j = 0;
	int k = 0;
	bool done = false;
	for(i = 0;i < data.size();i++)
	{
		temp = data[i];
		done = false;
		iProgressPos++;
		if(progress2 != NULL && iProgressPos % iIncrementStep == 0)
		{
			StepProgress(progress2);
		}
		if(temp.substr(0,1) == sByteOpen)
		{
			for(j = 0;j<vMultiByteTableIn.size();j++)
			{
				if(temp.substr(1,temp.length()-2) == vMultiByteTableIn[j].ascii)
				{
					temp = vMultiByteTableIn[j].hex;
					for(k = 0;k<temp.length();k = k+2)
					{
						result.push_back(temp.substr(k,2));
					}
					temp = "";
					done = true;
					break;
				}
			}
		}
		if(!done)
		{
			if(tvDakuten.hex.length() > 4 && temp == tvDakuten.ascii)
			{
				result.push_back(tvDakuten.hex.substr(0,2));
				result.push_back(tvDakuten.hex.substr(2,2));
				result.push_back(tvDakuten.hex.substr(4,2));
			}
			else if(tvDakuten.hex.length() > 2 && temp == tvDakuten.ascii)
			{
				result.push_back(tvDakuten.hex.substr(0,2));
				result.push_back(tvDakuten.hex.substr(2,2));
			}
			else if(temp == tvDakuten.ascii)
			{
				result.push_back(tvDakuten.hex);
			}
			else if(tvHandakuten.hex.length() > 4 && temp == tvHandakuten.ascii)
			{
				result.push_back(tvHandakuten.hex.substr(0,2));
				result.push_back(tvHandakuten.hex.substr(2,2));
				result.push_back(tvHandakuten.hex.substr(4,2));
			}
			else if(tvHandakuten.hex.length() > 2 && temp == tvHandakuten.ascii)
			{
				result.push_back(tvHandakuten.hex.substr(0,2));
				result.push_back(tvHandakuten.hex.substr(2,2));
			}
			else if(temp == tvHandakuten.ascii)
			{
				result.push_back(tvHandakuten.hex);
			}
			else if(sNewLine.length() > 4 && temp == "\n")
			{
				result.push_back(sNewLine.substr(0,2));
				result.push_back(sNewLine.substr(2,2));
				result.push_back(sNewLine.substr(4,2));
			}
			else if(sNewLine.length() > 2 && temp == "\n")
			{
				result.push_back(sNewLine.substr(0,2));
				result.push_back(sNewLine.substr(2,2));
			}
			else if(temp == "\n")
			{
				result.push_back(sNewLine);
			}
			else if(sNull.length() > 4 && temp.substr(1,3) == "END")
			{
				result.push_back(sNull.substr(0,2));
				result.push_back(sNull.substr(2,2));
				result.push_back(sNull.substr(4,2));
				i++;
			}
			else if(sNull.length() > 2 && temp.substr(1,3) == "END")
			{
				result.push_back(sNull.substr(0,2));
				result.push_back(sNull.substr(2,2));
				i++;
			}
			else if(temp.substr(1,3) == "END")
			{
				result.push_back(sNull);
				i++;
			}
			else if(temp.substr(0,1) == sByteOpen)
			{
				if(temp.substr(1,temp.length()-2).length() >= 2 && vDualTileTableIn[dualtileindex(temp.substr(1,temp.length()-2))] != "")
				{
					result.push_back(vDualTileTableIn[dualtileindex(temp.substr(1,temp.length()-2))]);
					done = true;
				}
				if(!done)
				{
					if((temp.substr(1,temp.length()-2)).length() > 1)
					{
						result.push_back(temp.substr(1,temp.length()-2));
						done = true;
					}
					else
					{
						string test = temp;
						if(vTableIn[(int) (unsigned char) (temp.substr(1,1))[0]] == "")
						{
							string message;
							message += "You have attempted to reinsert '" + (temp.substr(1,1)) + "', a value with no table equivalent.  It has been ignored.";
							message += "\r\n";
							message += "This is most likely because you have a table value ";
							message += "$XX=AA where one of the \"A\"s is the same as one of the separated-byte enclosures.";
							message += "\r\n";
							message += "It is recommended that you DO NOT SAVE the changes that have taken place.";
							message += "\r\n";
							message += "Change the separated-byte enclosures or remove the problematic table value and try this replacement again.";
							MessageBox(NULL,(message).c_str(),"Script Replace error:",MB_OK | MB_ICONERROR);
						}
						else
						{
							result.push_back(vTableIn[(int) (unsigned char) (temp.substr(1,1))[0]]);
						}
						done = true;
					}
				}
			}
			else
			{
				if(temp.substr(0,2).length() == 2 && vDualTileTableIn[dualtileindex(temp.substr(0,2))] != "")
				{
					result.push_back(vDualTileTableIn[dualtileindex(temp.substr(0,2))]);
					done = true;
				}
				if(!done)
				{
					if(vTableIn[(int)  (unsigned char) (temp.substr(0,1))[0]] == "")
					{
						if(!bDoingIME)
							MessageBox(NULL,("You have attempted to reinsert '" + (temp.substr(0,1)) + "', a value with no table equivalent.  It has been ignored.").c_str(),"Script Replace error:",MB_OK | MB_ICONERROR);
						else
						{
							//result.push_back("00");
						}
					}
					else
					{
						result.push_back(vTableIn[(int)  (unsigned char) (temp.substr(0,1))[0]]);
					}
					done = true;
				}
			}
		}
	}
	data = result;
}

//takes buffer
//puts chars into readable lines in buffer
void enscript(vector<string>& data)
{
	int n = (int)data.size();

	// Pre-calculate total characters so we can do one allocation.
	// bstring::operator+= reallocates+copies on every call, so building
	// a line via temp+=token is O(L^2) per line.  Writing into a flat
	// buffer with memcpy() reduces this to O(total_chars).
	int totalChars = n * 2; // +2 per element covers worst-case \n padding
	for(int k = 0; k < n; k++)
		totalChars += (int)data[k].length();

	char* outbuf   = new char[totalChars + 4];
	int   outpos   = 0;
	int   lineStart = 0;
	char  byteOpenChar = sByteOpen.length() > 0 ? sByteOpen[0] : '{';

	vector<string> result;
	for(int i = 0; i < n; i++)
	{
		if(bCancelDump) break;
		iProgressPos++;
		if(progress2 != NULL && iProgressPos % iIncrementStep == 0)
		{
			StepProgress(progress2);
			MSG _msg;
			while(PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&_msg);
				DispatchMessage(&_msg);
			}
		}
		const char* s    = data[i].c_str();
		int         slen = (int)data[i].length();

		if(slen == 1 && s[0] == '\n')
		{
			outbuf[outpos++] = '\n';
			outbuf[outpos]   = 0;
			result.push_back(string(outbuf + lineStart));
			lineStart = outpos;
		}
		else if(slen >= 4 && s[1] == 'E' && s[2] == 'N' && s[3] == 'D')
		{
			if(bCleanScript)
			{
				outbuf[outpos++] = '\n';
				outbuf[outpos]   = 0;
				result.push_back(string(outbuf + lineStart));
				result.push_back(string("\n"));
			}
			else
			{
				memcpy(outbuf + outpos, s, slen); outpos += slen;
				outbuf[outpos++] = '\n';
				outbuf[outpos]   = 0;
				result.push_back(string(outbuf + lineStart));
				result.push_back(string("\n"));
			}
			lineStart = outpos;
		}
		else
		{
			if(bCleanScript && slen > 0 && s[0] == byteOpenChar)
			{
				; // skip byte markers in clean-script mode
			}
			else
			{
				memcpy(outbuf + outpos, s, slen);
				outpos += slen;
			}
		}
	}
	if(outpos > lineStart)
	{
		outbuf[outpos] = 0;
		result.push_back(string(outbuf + lineStart));
	}
	delete[] outbuf;
	data = result;
}

//takes buffer
//puts readable lines into chars in buffer
void descript(vector<string>& data)
{
	string temp;
	string temp2;
	bool done = false;
	string multichar;
	vector<string> result;
	int i = 0;
	int j = 0;
	int k = 0;
	for(i = 0;i<data.size();i++)
	{
		temp = data[i];
		iProgressPos++;
		if(progress1 != NULL && iProgressPos % iIncrementStep == 0)
		{
			StepProgress(progress1);
		}
		for(j=0;j<temp.length();j++)
		{
			done = false;
			if(temp.substr(j,1) == sByteOpen)
			{
				if(temp.substr(j+1,3) == "END")
				{
					result.push_back(sByteOpen + "END" + sByteClose);
					result.push_back(data[i+1]);
					i++;
					j = j + 6;
				}
				else
				{
					while(multichar.substr(multichar.length()-1,1) != sByteClose)
					{
						multichar += temp.substr(j,1);
						j++;
					}
					j--;
					result.push_back(multichar);
					multichar = "";
				}
			}
			else
			{
				if(tvDakuten.hex != "" || tvHandakuten.hex != "")
				{
					if(temp.substr(j,tvDakuten.ascii.length()) == tvDakuten.ascii)
					{
						temp2 = temp.substr(j,tvDakuten.ascii.length());
						j = j + temp2.length()-1;
						result.push_back(temp2);
						if(bJPuncSwitch)
						{
							stringswap(result,result.size()-1,result.size()-2);
						}
						done = true;
					}
					else if(temp.substr(j,tvHandakuten.ascii.length()) == tvHandakuten.ascii)
					{
						temp2 = temp.substr(j,tvHandakuten.ascii.length());
						j = j + temp2.length()-1;
						result.push_back(temp2);
						if(bJPuncSwitch)
						{
							stringswap(result,result.size()-1,result.size()-2);
						}
						done = true;
					}
				}
				if(!done && !bByteMark)
				{
					int k = 0;
					for(k = 0;k<vMultiByteTableIn.size();k++)
					{
						if(temp.length() - j >= vMultiByteTableIn[k].ascii.length() && temp.substr(j,vMultiByteTableIn[k].ascii.length()) == vMultiByteTableIn[k].ascii)
						{
							temp2 = vMultiByteTableIn[k].ascii;
							j = j + temp2.length()-1;
							result.push_back(sByteOpen + temp2 + sByteClose);
							done = true;
						}
					}
					if(!done && temp.substr(j,2).length() == 2 && vDualTileTableIn[dualtileindex(temp.substr(j,2))] != "")
					{
						temp2 = temp.substr(j,2);
						j = j + temp2.length()-1;
						result.push_back(temp2);
						done = true;
					}
				}
				if(!done)
				{
					result.push_back(temp.substr(j,1));
				}
			}
		}
	}	
	data = result;
}

void read_table_file(fstream& fs, HWND hwnd, bookmark *pbmkList, int &iBookmarkCount)
{
	SetCursor (LoadCursor (NULL, IDC_WAIT));
	string cBuf;
	string temp;
	scriptdump a;
	//change this
	tablevalue b;
	linkedvalue c;
	//change this
	vTableIn = vEmptyOB;
	vTableOut = vEmptyOB;
	vDualTileTableIn = vEmptyTB;
	vDualByteTableOut = vEmptyTB;
	vMultiByteTableIn = vEmptyTable;
	vMultiByteTableOut = vEmptyTable;
	//change this
	vTableFileLeft = vEmpty;
	vTableFileRight = vEmpty;
	vTwoByteHighlightTable = vEmptyTB;
	vector<scriptdump> d;
	vScriptDumps = d;
	vector<linkedvalue> e;
	vLinked = e;
	int y = 0;
	bJPuncSwitch = true;
	bDumpClean = false;
	bHighlight = false;
	iRealTable = 0;
	tvDakuten.ascii = "�J";
	tvHandakuten.ascii = "�K";
	bReadingTable = true;
	bReadingTable = false;
	vector<string> empty(vChanges.size());
	vHighlights = empty;
	vDTEs = empty;
	while(!fs.eof() && !fs.fail())
	{
		getline(fs,cBuf);
		if(cBuf.length() != 0)
		{
			for(y = 0;y<cBuf.length();y++)
			{
				if(cBuf.substr(y,1) == "=")
					break;
			}
			if(y >= cBuf.length())
			{
				vTableFileLeft.push_back(cBuf);
				vTableFileRight.push_back("");
			}
			else
			{
				vTableFileLeft.push_back(cBuf.substr(0,y));
				vTableFileRight.push_back(cBuf.substr(y+1,cBuf.length()-y+1));
			}
		}
		if(cBuf.substr(0,1) == ">")
		{
			bJPuncSwitch = false;
			iRealTable++;
		}
		if(cBuf.substr(0,1) == "#")
		{
			bDumpClean = true;
			iRealTable++;
		}
		else if(cBuf.substr(0,1) == "*")
		{
			sNewLine = cBuf.substr(1,cBuf.length()-1);
			iRealTable++;
		}
		else if(cBuf.substr(0,1) == "/")
		{
			sNull = cBuf.substr(1,cBuf.length()-1);
			iRealTable++;
		}
		else if(cBuf.substr(0,1) == "!")
		{
            tvDakuten.hex = cBuf.substr(1,cBuf.length()-1);
			iRealTable++;
			if(tvDakuten.hex.length() == 2)
			{
				int index = (int) char_hex(tvDakuten.hex);
				if(vTableOut[index] != "")
				{
					tvDakuten.ascii = vTableOut[index];
					vTableOut[index] = "";
					iRealTable--;
				}
			}
			else if(tvDakuten.hex.length() == 4)
			{
				int index = dualbyteindex(tvDakuten.hex);
				if(vDualByteTableOut[index] != "")
				{
					tvDakuten.ascii = vDualByteTableOut[index];
					vDualByteTableOut[index] = "";
					iRealTable--;
				}
			}
			else if(tvDakuten.hex.length() == 6)
			{
				int i = 0;
				for(i = 0;i<vMultiByteTableOut.size();i++)
				{
					if(tvDakuten.hex == vMultiByteTableOut[i].hex)
					{
						tvDakuten.ascii = vMultiByteTableOut[i].ascii;
						vMultiByteTableOut.remove(i);
						iRealTable--;
						i = vMultiByteTableOut.size();
					}
				}
			}
		}
		else if(cBuf.substr(0,1) == "@")
		{
			tvHandakuten.hex = cBuf.substr(1,cBuf.length()-1);
			iRealTable++;
			if(tvHandakuten.hex.length() == 2)
			{
				int index = (int) char_hex(tvHandakuten.hex);
				if(vTableOut[index] != "")
				{
					tvHandakuten.ascii = vTableOut[index];
					vTableOut[index] = "";
					iRealTable--;
				}
			}
			else if(tvHandakuten.hex.length() == 4)
			{
				int index = dualbyteindex(tvHandakuten.hex);
				if(vDualByteTableOut[index] != "")
				{
					tvHandakuten.ascii = vDualByteTableOut[index];
					vDualByteTableOut[index] = "";
					iRealTable--;
				}
			}
			else if(tvHandakuten.hex.length() == 6)
			{
				int i = 0;
				for(i = 0;i<vMultiByteTableOut.size();i++)
				{
					if(tvHandakuten.hex == vMultiByteTableOut[i].hex)
					{
						tvHandakuten.ascii = vMultiByteTableOut[i].ascii;
						vMultiByteTableOut.remove(i);
						iRealTable--;
						i = vMultiByteTableOut.size();
					}
				}
			}
		}
		else if(cBuf.substr(0,1) == "$")
		{
			if(cBuf.length() >= 5)
			{
				c.hex = cBuf.substr(1,2);
				c.links = intvalue(cBuf.substr(4,cBuf.length()-4));
				vLinked.push_back(c);
				iRealTable++;
			}
		}
		else if(cBuf.substr(0,1) == "(")
		{
			int i = 0;
			int j = 0;
			for(i = 0;i<cBuf.length();i++)
			{
				if(cBuf.substr(i,1) == ")")
					break;
			}
			temp = "x" + cBuf.substr(1,i-2);
			j = -1;
			if (sscanf (temp.c_str(), "x%X", &j) == 0)
			{
				if (sscanf (temp.c_str(), "%d", &j) == 0)
				{
					MessageBox (hwnd, "Starting offset of bookmark is not recognized.", "Table has a bookmark error:", MB_OK | MB_ICONERROR);
					i = -1;
				}
			}
			if(j == -1)
				;
			else
			{
				if(iOffsetMode == LOROM)
					iBookmarkOffset = fromLOROM(j);
				else if(iOffsetMode == HIROM)
					iBookmarkOffset = fromHIROM(j);
				else
					iBookmarkOffset = j;
				temp = cBuf.substr(i,cBuf.length()-i);
				strcpy(pcBookmarkTxt,(cBuf.substr(i+1,cBuf.length()-(i+1)).c_str()));
				temp = "";
				pbmkList[iBookmarkCount].offset = iBookmarkOffset;
				if (strlen(pcBookmarkTxt)>0)
				{
					pbmkList[iBookmarkCount].name = new char[strlen(pcBookmarkTxt)+1];
					strcpy (pbmkList[iBookmarkCount].name, pcBookmarkTxt);
				}
				else
					pbmkList[iBookmarkCount].name = NULL;
				iBookmarkCount++;
				iRealTable++;
			}
		}
		else if(cBuf.substr(0,1) == "[")
		{
			int i = 0;
			int j = 0;
			int k = 0;
			for(i = 0;i<cBuf.length();i++)
			{
				if(cBuf.substr(i,1) == "-")
				break;
			}
			temp = "x" + cBuf.substr(1,i-2);
			j = -1;
			if (sscanf (temp.c_str(), "x%X", &j) == 0)
			{
				if (sscanf (temp.c_str(), "%d", &j) == 0)
				{
					MessageBox (hwnd, "Starting offset of script dump bookmark is not recognized.", "Table has a script dump bookmark error:", MB_OK | MB_ICONERROR);
					i = -1;
				}
			}
			if(j == -1)
				;
			else
			{
				if(iOffsetMode == LOROM)
					a.start = fromLOROM(j);
				else if(iOffsetMode == HIROM)
					a.start = fromHIROM(j);
				else
					a.start = j;
			}
			k = i;
			for(i = i;i<cBuf.length();i++)
			{
				if(cBuf.substr(i,1) == "]")
					break;
			}
			temp = "x" + cBuf.substr(k + 1,(i-(k+1))-1);
			j = -1;
			if (sscanf (temp.c_str(), "x%X", &j) == 0)
			{
				if (sscanf (temp.c_str(), "%d", &j) == 0)
				{
					MessageBox (hwnd, "Ending offset of script dump bookmark is not recognized.", "Table has a script dump bookmark error:", MB_OK | MB_ICONERROR);
					i = -1;
				}
			}
			if(j == -1)
				;
			else
			{
				temp = cBuf.substr(i+1,cBuf.length()-(i+1));
				if(iOffsetMode == LOROM)
					a.end = fromLOROM(j);
				else if(iOffsetMode == HIROM)
					a.end = fromHIROM(j);
				else
					a.end = j;
				a.name = temp;
				iBookmarkOffset = a.start;
			    strcpy(pcBookmarkTxt,a.name.c_str());
				temp = "";
				pbmkList[iBookmarkCount].offset = iBookmarkOffset;
				if (strlen(pcBookmarkTxt)>0)
				{
					pbmkList[iBookmarkCount].name = new char[strlen(pcBookmarkTxt)+1];
					strcpy (pbmkList[iBookmarkCount].name, pcBookmarkTxt);
				}
				else
					pbmkList[iBookmarkCount].name = NULL;
				iBookmarkCount++;
				iRealTable++;
				vScriptDumps.push_back(a);
			}
		}
		else if(cBuf.substr(0,1) == "^")
		{
			if(cBuf.length() == 3)
			{
				assert(vTwoByteHighlightTable.size() > (int) char_hex(cBuf.substr(1,2)));
				vTwoByteHighlightTable[(int) char_hex(cBuf.substr(1,2))] = "1";
				iRealTable++;
			}
			else if(cBuf.length() == 5)
			{
				assert(vTwoByteHighlightTable.size() > dualbyteindex(cBuf.substr(1,4)));
				vTwoByteHighlightTable[dualbyteindex(cBuf.substr(1,4))] = "1";
				iRealTable++;
			}
			bHighlight = true;
		}
		else if(hex_value(cBuf.substr(0,2)) == -1)
			;
		else
		{
			if(tvDakuten.hex.length() > 0 && cBuf.length() > tvDakuten.hex.length() && cBuf.substr(0,tvDakuten.hex.length()) == tvDakuten.hex)
			{
				tvDakuten.ascii = cBuf.substr(tvDakuten.hex.length()+1,cBuf.length()-tvDakuten.hex.length()+1);
			}
			else if(tvHandakuten.hex.length() > 0 && cBuf.length() > tvHandakuten.hex.length() && cBuf.substr(0,tvHandakuten.hex.length()) == tvHandakuten.hex)
			{
				tvHandakuten.ascii = cBuf.substr(tvHandakuten.hex.length()+1,cBuf.length()-tvHandakuten.hex.length()+1);
			}
		    else if(cBuf.substr(2,1) == "=")
			{
 				if((cBuf.substr(3,cBuf.length()-3)).length() == 2)
				{
					assert(vTableOut.size() > (int) char_hex(cBuf.substr(0,2)));
					vTableOut[(int) char_hex(cBuf.substr(0,2))] = cBuf.substr(3,cBuf.length()-3);
					assert(vDualTileTableIn.size() > dualtileindex(cBuf.substr(3,cBuf.length()-3)));
					vDualTileTableIn[dualtileindex(cBuf.substr(3,cBuf.length()-3))] = cBuf.substr(0,2);
					iRealTable++;
				}
				else if((cBuf.substr(3,cBuf.length()-3)).length() == 1)
				{
					assert(vTableOut.size() > (int) char_hex(cBuf.substr(0,2)));
					vTableOut[(int) char_hex(cBuf.substr(0,2))] = cBuf.substr(3,cBuf.length()-3);
					assert(vTableIn.size() > (int)  (unsigned char) cBuf.substr(3,cBuf.length()-3)[0]);				
					vTableIn[(int) (unsigned char) cBuf.substr(3,cBuf.length()-3)[0]] = cBuf.substr(0,2);
					iRealTable++;
				}
				else
				{
					assert(vTableOut.size() > (int) char_hex(cBuf.substr(0,2)));
					vTableOut[(int) char_hex(cBuf.substr(0,2))] = cBuf.substr(3,cBuf.length()-3);
					b.hex = cBuf.substr(0,2);
					b.ascii = cBuf.substr(3,cBuf.length()-3);
					insert_longest_to_shortest(vMultiByteTableIn,b);
					iRealTable++;
				}
			}
			else
			{
				int j = 0;
				for(j = 0;j<cBuf.length();j++)
				{
					if(cBuf.substr(j,1) == "=")
						break;
				}
				if(j / 2 == 2)
				{
					assert(vDualByteTableOut.size() -1 >= dualbyteindex(cBuf.substr(0,j)));
					vDualByteTableOut[dualbyteindex(cBuf.substr(0,j))] = cBuf.substr(j+1,cBuf.length()-(j+1));
					b.hex = cBuf.substr(0,j);
					b.ascii = cBuf.substr(j+1,cBuf.length()-(j+1));
					insert_longest_to_shortest(vMultiByteTableIn,b);
					iRealTable++;
				}
				else
				{
					b.hex = cBuf.substr(0,j);
					b.ascii = cBuf.substr(j+1,cBuf.length()-(j+1));
					vMultiByteTableOut.push_back(b);
					insert_longest_to_shortest(vMultiByteTableIn,b);
					iRealTable++;
				}
			}
		}
	}
	long i = 0;
	for(i = 0;i<vTableFileRight.size();i++)
	{
		if(vTableFileRight[i].length() > 0 &&
			(vTableFileRight[i].substr(0,1) == (string) (char) 164 || 
			vTableFileRight[i].substr(0,1) == (string) (char) 165 ||
			vTableFileRight[i].substr(0,1) == (string) (char) 161))
		{
			tvDakuten.ascii = "��";
			tvHandakuten.ascii = "��";
			break;
		}
		if(vTableFileRight[i].length() > 0 && 
			(vTableFileRight[i].substr(0,1) == (string) (char) 130 || 
			vTableFileRight[i].substr(0,1) == (string) (char) 131))
		{
			tvDakuten.ascii = "�J";
			tvHandakuten.ascii = "�K";
			break;
		}
	}
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
				//else if(vTableOut[(unsigned char) FileBuffer[k]].length() == 2)
				//{
				//	vChanges[k] = COLORDTE;
				//}
			}
			if(vTwoByteHighlightTable[(unsigned char) FileBuffer[k]] == "1")
			{
				vChanges[k] = COLORHIGHLIGHT;
			}
			//else if(vTableOut[(unsigned char) FileBuffer[k]].length() == 2)
			//{
			//	vChanges[k] = COLORDTE;
			//}
		}
	}*/
		SetCursor (LoadCursor (NULL, IDC_ARROW));
}

void save_table_file(fstream& fs)
{
	int k = 0;
	if(!fs.fail())
	{
		for(k = 0;k<vTableFileLeft.size();k++)
		{
			if(vTableFileRight[k] != "")
			{
				fs << vTableFileLeft[k] << "=" << vTableFileRight[k] << endl;
			}
			else
			{
				fs << vTableFileLeft[k] << endl;
			}
		}
	}
}

//takes filestream and buffer
//reads a script into the buffer
void read_script_file(fstream & fs, vector<string> & data)
{
	string temp;
	while(!fs.fail() && !fs.eof())
	{
		getline(fs,temp);
		if(temp.substr(temp.length()-1,1) != "\n")
			temp += "\n";
		data.push_back(temp);
	}
	int last = data.size()-1;
	char lc = (unsigned char)(data[last].substr(data[last].length()-1,1))[0];
	if(!(lc >= 33 && lc <= 172))
	{
		if(data[last].length() <= 1)
			data.pop_back();
		else
			data[last] = data[last].substr(0,data[last].length()-1);
	}
}

//takes a filestream and a buffer
//writes a script into a file
void write_script_file(fstream & fs, vector<string> & data)
{
	// Batch writes into 64KB chunks to reduce syscall overhead.
	// Avoids thousands of individual operator<< calls for large dumps.
	static char wbuf[65536];
	int wpos = 0;
	for(int i = 0; i < (int)data.size(); i++)
	{
		const char* p = data[i].c_str();
		int len = (int)data[i].length();
		if(wpos + len > (int)sizeof(wbuf))
		{
			if(wpos > 0) { fs.write(wbuf, wpos); wpos = 0; }
			if(len >= (int)sizeof(wbuf)) { fs.write(p, len); continue; }
		}
		memcpy(wbuf + wpos, p, len);
		wpos += len;
	}
	if(wpos > 0) fs.write(wbuf, wpos);
}

void pwrite_script_file(fstream & fs, vector<string> & data)
{
	string dak = tvDakuten.ascii;
	string hand = tvHandakuten.ascii;
	string temp;
	int i = 0;
	int j = 0;
	for(i = 0;i<data.size();i++)
	{
		temp = data[i];
		for(j = 0;j<temp.length();j++)
		{
			if(temp.length() >= dak.length()+j && temp.substr(j,dak.length()) == dak)
			{
				if(temp.substr(j-1,1) == sByteClose)
				{
					fs << temp.substr(0,j-2) << (unsigned char) (((int) (unsigned char) temp[j-2]) + 1) << sByteClose;
					temp = temp.substr(j+dak.length(),temp.length()-j+dak.length());
					j = 0;
				}
				else
				{
					fs << temp.substr(0,j-1) << (unsigned char) (((int) (unsigned char) temp[j-1]) + 1);
					temp = temp.substr(j+dak.length(),temp.length()-j+dak.length());
					j = 0;
				}
			}
			else if(temp.length() >= hand.length()+j && temp.substr(j,hand.length()) == hand)
			{
				if(temp.substr(j-1,1) == sByteClose)
				{
					fs << temp.substr(0,j-2) << (unsigned char) (((int) (unsigned char) temp[j-2]) + 2) << sByteClose;
					temp = temp.substr(j+hand.length(),temp.length()-j+hand.length());
					j = 0;
				}
				else
				{
					fs << temp.substr(0,j-1) << (unsigned char) (((int) (unsigned char) temp[j-1]) + 2);
					temp = temp.substr(j+hand.length(),temp.length()-j+hand.length());
					j = 0;
				}
			}
			/*
			else if(temp.length() >= 2+j && (temp.substr(j,2) == "��" || temp.substr(j,2) == "�J"))
			{
				if(temp.substr(j-1,1) == sByteClose)
				{
					fs << temp.substr(0,j-2) << (unsigned char) (((int) (unsigned char) temp[j-2]) + 1) << sByteClose;
					temp = temp.substr(j+2,temp.length()-j+2);
					j = 0;
				}
				else
				{
					fs << temp.substr(0,j-1) << (unsigned char) (((int) (unsigned char) temp[j-1]) + 1);
					temp = temp.substr(j+2,temp.length()-j+2);
					j = 0;
				}
			}
			else if(temp.substr(j,2) == "�K" || temp.substr(j,2) == "��" )//|| temp.substr(j,2) == "��")
			{
				if(temp.substr(j-1,1) == sByteClose)
				{
					fs << temp.substr(0,j-2) << (unsigned char) (((int) (unsigned char) temp[j-2]) + 2) << sByteClose;
					temp = temp.substr(j+2,temp.length()-j+2);
					j = 0;
				}
				else
				{
					fs << temp.substr(0,j-1) << (unsigned char) (((int) (unsigned char) temp[j-1]) + 2);
					temp = temp.substr(j+2,temp.length()-j+2);
					j = 0;
				}
			}*/
		}
		fs << temp;
	}
}


void convert_table_from_to(vector<string>& x, vector<string>& y)
{
	int i = 0;
	int j = 0;\
    SetCursor (LoadCursor (NULL, IDC_WAIT));
	for(i = 0;i<vTableFileRight.size();i++)
	{
		for(j = 0;j<x.size();j++)
		{
			if(vTableFileRight[i] == x[j])
			{
				vTableFileRight[i] = y[j];
				j = x.size();
			}
		}
	}
	fstream fs;
	fs.open(sLoadedTable.c_str(),ios::out);
	save_table_file(fs);
	fs.close();
	SetCursor (LoadCursor (NULL, IDC_ARROW));
}

// ---- Parallel translate ----

struct TranslateJob {
	vector<string>* pData;
	char*           pProc;
	int             iStart;
	int             iEnd;
};

static DWORD WINAPI TranslateJobThread(LPVOID lpParam) {
	TranslateJob* j = (TranslateJob*)lpParam;
	translate(*j->pData, j->pProc, j->iStart, j->iEnd, false);
	return 0;
}

void translate_parallel(vector<string>& data) {
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	int nCores = (int)si.dwNumberOfProcessors;
	int total  = (int)data.size();
	if(nCores < 2 || total < 20000) {
		translate(data);
		return;
	}
	if(nCores > 8) nCores = 8;

	const int MARGIN = 3;
	int chunkSize = total / nCores;

	// processed[] marks elements already handled by a thread
	char* processed = new char[total];
	memset(processed, 0, total);

	TranslateJob* jobs    = new TranslateJob[nCores];
	HANDLE*       threads = new HANDLE[nCores];
	int           nThreads = 0;

	for(int t = 0; t < nCores; t++) {
		int iStart = t * chunkSize;
		int iEnd   = (t == nCores-1) ? total : (t+1)*chunkSize - MARGIN;
		if(t > 0) iStart += MARGIN;
		if(iEnd < iStart) iEnd = iStart;
		jobs[t].pData  = &data;
		jobs[t].pProc  = processed;
		jobs[t].iStart = iStart;
		jobs[t].iEnd   = iEnd;
		HANDLE h = CreateThread(NULL, 0, TranslateJobThread, &jobs[t], 0, NULL);
		if(h) threads[nThreads++] = h;
	}

	// Wait with message pumping so the UI stays responsive during parallel translate
	if(nThreads > 0) {
		DWORD r;
		do {
			r = MsgWaitForMultipleObjects((DWORD)nThreads, threads, TRUE, 100, QS_ALLINPUT);
			if(r == WAIT_OBJECT_0 + (DWORD)nThreads) {
				// Message available — pump it so the Cancel button and progress dialog work
				MSG _msg;
				while(PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE)) {
					TranslateMessage(&_msg);
					DispatchMessage(&_msg);
				}
			} else if(r == WAIT_TIMEOUT) {
				// Periodic pump + progress step
				if(progress1 != NULL) StepProgress(progress1);
				MSG _msg;
				while(PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE)) {
					TranslateMessage(&_msg);
					DispatchMessage(&_msg);
				}
			}
		} while(r != WAIT_OBJECT_0);
	}
	for(int t = 0; t < nThreads; t++) CloseHandle(threads[t]);

	// Sequential cleanup for the MARGIN-sized gaps at each chunk boundary
	for(int t = 0; t < nCores-1; t++) {
		int bStart = (t+1)*chunkSize - MARGIN;
		int bEnd   = (t+1)*chunkSize + MARGIN;
		if(bEnd > total) bEnd = total;
		translate(data, processed, bStart, bEnd, false);
	}

	delete[] threads;
	delete[] jobs;
	delete[] processed;

	// Restore bJPuncSwitch state (translate() does this per-call; redo here for safety)
	if(tvHandakuten.hex.length() == 0 && tvDakuten.hex.length() == 0)
		bJPuncSwitch = bJPuncSwitch; // no-op; each thread already restored it

	// Single compaction over the whole array
	if(bSplitMultiByte) {
		int w = 0;
		for(int k = 0; k < total; k++) {
			if(data[k].length() > 0) {
				if(w != k) data[w] = data[k];
				w++;
			}
		}
		while((int)data.size() > w) data.pop_back();
	}
}

#endif
