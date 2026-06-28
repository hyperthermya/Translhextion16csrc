#include "pointerlistmodel.h"
#include "modeldata.h"
#include <vector>
#include <map>

// Global variables for pointer system
std::map<__int64, __int64> g_pointerMap;
std::multimap<__int64, __int64> g_offsetMultiMap;
std::vector<PointerEntry> g_pointerList;
__int64 g_pointerOffset = 0;
int g_pointerSize = 4;
std::vector<__int64> g_pointerSnapshot;

// Global pointer model instance
PointerListModel g_pointerModel;

PointerListModel::PointerListModel() {}

bool PointerListModel::addPointer(__int64 ptrOffset, __int64 targetOffset, int ptrSize)
{
	if (g_pointerMap.find(ptrOffset) != g_pointerMap.end())
		return false;

	__int64 encoded = encodePtrValue(targetOffset, ptrSize);
	g_pointerMap[ptrOffset] = encoded;
	g_offsetMultiMap.insert(std::make_pair(targetOffset, ptrOffset));

	PointerEntry entry;
	entry.offset = ptrOffset;
	entry.target = targetOffset;
	entry.size = ptrSize;
	g_pointerList.push_back(entry);

	return true;
}

bool PointerListModel::dropPointer(__int64 ptrOffset)
{
	std::map<__int64, __int64>::iterator it = g_pointerMap.find(ptrOffset);
	if (it == g_pointerMap.end())
		return false;

	__int64 target = decodePtrTarget(it->second);
	g_pointerMap.erase(it);
	g_offsetMultiMap.erase(target);

	for (size_t i = 0; i < g_pointerList.size(); ++i)
	{
		if (g_pointerList[i].offset == ptrOffset)
		{
			g_pointerList.erase(g_pointerList.begin() + i);
			break;
		}
	}
	return true;
}

__int64 PointerListModel::getOffset(__int64 ptrOffset) const
{
	std::map<__int64, __int64>::const_iterator it = g_pointerMap.find(ptrOffset);
	if (it == g_pointerMap.end())
		return -1;
	return decodePtrTarget(it->second);
}

std::vector<__int64> PointerListModel::getPointers(__int64 dataOffset) const
{
	std::vector<__int64> result;
	std::pair<std::multimap<__int64, __int64>::const_iterator, 
	          std::multimap<__int64, __int64>::const_iterator> range = 
		g_offsetMultiMap.equal_range(dataOffset);
	for (std::multimap<__int64, __int64>::const_iterator it = range.first; 
	     it != range.second; ++it)
	{
		result.push_back(it->second);
	}
	return result;
}

bool PointerListModel::isPointer(__int64 offset) const
{
	return g_pointerMap.find(offset) != g_pointerMap.end();
}

int PointerListModel::getPointerSize(__int64 ptrOffset) const
{
	std::map<__int64, __int64>::const_iterator it = g_pointerMap.find(ptrOffset);
	if (it == g_pointerMap.end())
		return 4;
	return decodePtrSize(it->second);
}

void PointerListModel::clear()
{
	g_pointerMap.clear();
	g_offsetMultiMap.clear();
	g_pointerList.clear();
}

void PointerListModel::snapshotPointers()
{
	g_pointerSnapshot.clear();
	for (size_t i = 0; i < g_pointerList.size(); ++i)
	{
		g_pointerSnapshot.push_back(g_pointerList[i].offset);
		g_pointerSnapshot.push_back(g_pointerList[i].target);
	}
}

void PointerListModel::restorePointers()
{
	clear();
	for (size_t i = 0; i + 1 < g_pointerSnapshot.size(); i += 2)
	{
		addPointer(g_pointerSnapshot[i], g_pointerSnapshot[i + 1]);
	}
}

const std::vector<PointerEntry>& PointerListModel::getPointerList() const
{
	return g_pointerList;
}