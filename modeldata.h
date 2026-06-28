#ifndef _MODELDATA_H
#define _MODELDATA_H

// Extern declarations for the pointer/section/table model globals.
// Kept out of globals.h (which is a single-inclusion definitions file
// for mainwindow.cpp) so model .cpp files can include just this header
// without pulling in -- and redefining -- every legacy global.

#include <windows.h>
#include "bstring.h"
#include "bvect.h"
#include "structs.h"
#include <map>
#include <set>
#include <utility>
#include <vector>

// Enhanced pointer system - declarations (definitions in pointerlistmodel.cpp)
extern std::map<__int64, __int64> g_pointerMap;
extern std::multimap<__int64, __int64> g_offsetMultiMap;
extern std::vector<PointerEntry> g_pointerList;
extern __int64 g_pointerOffset;
extern int g_pointerSize;
extern std::vector<__int64> g_pointerSnapshot;

// Enhanced section system - declarations (definitions in sectionlistmodel.cpp)
extern std::vector<Section> g_sectionList;
extern std::vector<Section> g_sectionSnapshot;

// Multi-table support (definitions in models.cpp)
extern std::vector<DocTableEntry> g_docTables;
extern int g_activeTableIndex;

// Virtual formatting (definitions in models.cpp)
extern std::vector<__int64> g_alignmentOffsets;

// Original data preservation (definitions in models.cpp)
extern std::vector<std::pair<__int64, string>> g_originalBytes;
extern __int64 g_originalFileSize;

// Changed positions tracking (definitions in models.cpp)
extern std::set<__int64> g_changedPositions;
extern __int64 g_changedRangeStart;
extern __int64 g_changedRangeEnd;

#endif
