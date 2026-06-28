#ifndef _POINTERRENDER_H
#define _POINTERRENDER_H

#include "pointerlistmodel.h"
#include "modeldata.h"
#include "hex.h"
#include "filevect.h"

// Pointer rendering support
// Based on RTHextion pointer highlighting system

// Check if a byte at the given offset is part of a pointer
inline bool isByteInPointer(__int64 offset, int pointerSize = 4)
{
	for (size_t i = 0; i < g_pointerList.size(); ++i)
	{
		__int64 ptrStart = g_pointerList[i].offset;
		__int64 ptrEnd = ptrStart + g_pointerList[i].size;
		if (offset >= ptrStart && offset < ptrEnd)
		{
			return true;
		}
	}
	return false;
}

// Get the pointer entry that contains the given offset (if any)
inline PointerEntry* getPointerAtOffset(__int64 offset)
{
	for (size_t i = 0; i < g_pointerList.size(); ++i)
	{
		__int64 ptrStart = g_pointerList[i].offset;
		__int64 ptrEnd = ptrStart + g_pointerList[i].size;
		if (offset >= ptrStart && offset < ptrEnd)
		{
			return &g_pointerList[i];
		}
	}
	return nullptr;
}

// Render pointer colors in hex view
inline COLORREF getPointerRenderColor(__int64 offset)
{
	PointerEntry* entry = getPointerAtOffset(offset);
	if (entry)
	{
		return RGB(255, 150, 0); // Use changes color for pointers
	}
	return RGB(255, 255, 255); // Default
}

// Get target data for a pointer (declared inline in header for use by mainwindow).
// FileBuffer is a private member of class Translhextion, not a global, so the
// caller (a Translhextion member function) must pass its own FileBuffer/iFileSize in.
inline string getPointerTargetData(__int64 ptrOffset, FileVector<unsigned char>& fileBuffer, __int64 fileSize, int maxBytes = 48)
{
	for (size_t i = 0; i < g_pointerList.size(); ++i)
	{
		if (g_pointerList[i].offset == ptrOffset)
		{
			__int64 target = g_pointerList[i].target;
			int size = g_pointerList[i].size;
			if (target >= 0 && target + size <= fileSize)
			{
				string result;
				for (int j = 0; j < size && j < maxBytes; ++j)
				{
					result += hex_char((unsigned char)fileBuffer[target + j]);
				}
				return result;
			}
		}
	}
	return "";
}

#endif // _POINTERRENDER_H