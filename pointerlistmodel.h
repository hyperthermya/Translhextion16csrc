#ifndef _POINTERLISTMODEL_H
#define _POINTERLISTMODEL_H

#include <windows.h>
#include "bstring.h"
#include "bvect.h"
#include "structs.h"
#include <vector>

// Pointer size encoding (bits 61-62 of stored value)
// 00 = 4 bytes, 01 = 3 bytes, 10 = 2 bytes
const __int64 kPtrSizeBitMask = ((__int64)3) << 61;

inline __int64 encodePtrValue(__int64 target, int ptrSize)
{
	int code = (ptrSize == 2) ? 2 : (ptrSize == 3) ? 1 : 0;
	return (target & ~kPtrSizeBitMask) | ((__int64)code << 61);
}

inline __int64 decodePtrTarget(__int64 stored)
{
	return stored & ~kPtrSizeBitMask;
}

inline int decodePtrSize(__int64 stored)
{
	int code = (int)((stored >> 61) & 3);
	return (code == 2) ? 2 : (code == 1) ? 3 : 4;
}

// PointerListModel: manages pointer list with bidirectional lookup
// Based on RTHextion implementation
class PointerListModel
{
public:
	PointerListModel();

	// Add a pointer (return false if already exists)
	bool addPointer(__int64 ptrOffset, __int64 targetOffset, int ptrSize = 4);

	// Remove a pointer
	bool dropPointer(__int64 ptrOffset);

	// Get target for a pointer offset
	__int64 getOffset(__int64 ptrOffset) const;

	// Get all pointers pointing to a target offset
	std::vector<__int64> getPointers(__int64 dataOffset) const;

	// Check if offset is a pointer
	bool isPointer(__int64 offset) const;

	// Get pointer byte size for an offset
	int getPointerSize(__int64 ptrOffset) const;

	// Clear all pointers
	void clear();

	// Snapshot/restore for project persistence
	void snapshotPointers();
	void restorePointers();

	const std::vector<PointerEntry>& getPointerList() const;
};

// Global pointer model instance
extern PointerListModel g_pointerModel;

#endif // _POINTERLISTMODEL_H