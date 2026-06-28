#ifndef _SECTIONLISTMODEL_H
#define _SECTIONLISTMODEL_H

#include <windows.h>
#include "bstring.h"
#include "bvect.h"
#include "structs.h"
#include <vector>

// SectionListModel: manages file sections with color lookup
// Based on RTHextion implementation
class SectionListModel
{
public:
	SectionListModel();

	// Add a section
	void addSection(const Section& s);

	// Remove a section (and its children)
	void removeSection(int index);

	// Rename a section
	void renameSection(int index, const string& name);

	// Recolor a section
	void recolorSection(int index, COLORREF color);

	// Update section data
	void updateSection(int index, const Section& s);

	// Clear all sections
	void clear();

	// Get color for an offset
	COLORREF colorAtOffset(__int64 offset) const;

	// Set sections from serialized data
	void setSections(const std::vector<Section>& sections);

	const std::vector<Section>& sections() const;

	// Generate random pastel color
	static COLORREF randomPastelColor();
};

// Global section model instance
extern SectionListModel g_sectionModel;

#endif // _SECTIONLISTMODEL_H