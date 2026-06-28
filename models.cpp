#include "modeldata.h"
#include <set>

// Changed positions tracking
std::set<__int64> g_changedPositions;
__int64 g_changedRangeStart = -1;
__int64 g_changedRangeEnd = -1;

// Change tracking functions
void markPositionChanged(__int64 offset)
{
	g_changedPositions.insert(offset);
}

void clearChangedPositions()
{
	g_changedPositions.clear();
	g_changedRangeStart = -1;
	g_changedRangeEnd = -1;
}

void setChangedRange(__int64 start, __int64 end)
{
	g_changedRangeStart = start;
	g_changedRangeEnd = end;
	for (__int64 i = start; i <= end; ++i)
	{
		g_changedPositions.insert(i);
	}
}

// Other global variables
std::vector<DocTableEntry> g_docTables;
int g_activeTableIndex = -1;
std::vector<__int64> g_alignmentOffsets;
std::vector<std::pair<__int64, string>> g_originalBytes;
__int64 g_originalFileSize = -1;