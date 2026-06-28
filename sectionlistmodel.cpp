#include "sectionlistmodel.h"
#include "modeldata.h"
#include <vector>

// Global variables for section system
std::vector<Section> g_sectionList;
std::vector<Section> g_sectionSnapshot;

// Global section model instance
SectionListModel g_sectionModel;

SectionListModel::SectionListModel() {}

void SectionListModel::addSection(const Section& s)
{
	g_sectionList.push_back(s);
}

void SectionListModel::removeSection(int index)
{
	if (index < 0 || index >= (int)g_sectionList.size())
		return;

	g_sectionList.erase(g_sectionList.begin() + index);

	// Remove children (sections that had this as parent)
	for (int i = g_sectionList.size() - 1; i >= 0; --i)
	{
		if (g_sectionList[i].parentIndex == index || g_sectionList[i].parentIndex > index)
		{
			g_sectionList.erase(g_sectionList.begin() + i);
		}
	}

	// Fix parent indices for sections after removed ones
	for (int i = 0; i < (int)g_sectionList.size(); ++i)
	{
		if (g_sectionList[i].parentIndex > index)
		{
			g_sectionList[i].parentIndex--;
		}
	}
}

void SectionListModel::renameSection(int index, const string& name)
{
	if (index >= 0 && index < (int)g_sectionList.size())
	{
		g_sectionList[index].name = name;
	}
}

void SectionListModel::recolorSection(int index, COLORREF color)
{
	if (index >= 0 && index < (int)g_sectionList.size())
	{
		g_sectionList[index].color = color;
	}
}

void SectionListModel::updateSection(int index, const Section& s)
{
	if (index >= 0 && index < (int)g_sectionList.size())
	{
		g_sectionList[index] = s;
	}
}

void SectionListModel::clear()
{
	g_sectionList.clear();
}

COLORREF SectionListModel::colorAtOffset(__int64 offset) const
{
	for (size_t i = 0; i < g_sectionList.size(); ++i)
	{
		if (offset >= g_sectionList[i].startOffset && offset < g_sectionList[i].endOffset)
		{
			return g_sectionList[i].color;
		}
	}
	return RGB(255, 255, 255); // White/default
}

void SectionListModel::setSections(const std::vector<Section>& sections)
{
	g_sectionList = sections;
}

const std::vector<Section>& SectionListModel::sections() const
{
	return g_sectionList;
}

COLORREF SectionListModel::randomPastelColor()
{
	int r = 128 + rand() % 128;
	int g = 128 + rand() % 128;
	int b = 128 + rand() % 128;
	return RGB(r, g, b);
}