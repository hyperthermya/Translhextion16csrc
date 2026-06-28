#ifndef _STRUCTS_H
#define _STRUCTS_H

typedef struct
{
	int x;
	int y;
} intpair;

typedef struct
{
	long offset;
	char* name;
} bookmark;

struct tablevalue
{
	string hex;
	string ascii;
};

struct scriptdump
{
	long start;
	long end;
	string name;
};

struct longpair
{
	long x;
	long y;
};

struct relation
{
	int relate;
	long offset;
	string op;
	int related;
};

struct relativescandata
{
	string listitem;
	string search;
	long start;
	long end;
	string A;
	string a;
	string n;
};

struct linkedvalue
{
	string hex;
	int links;
};

struct RelativeOffsets
{
	unsigned char a;
	bool ba;
	unsigned char A;
	bool bA;
	unsigned char n;
	bool bn;
};

// Extended pointer entry with size information (2/4 bytes)
// Based on RTHextion pointer system
struct PointerEntry
{
	__int64 offset;        // Position where pointer is stored
	__int64 target;        // Target data offset
	int     size;          // 2 or 4 bytes
	bool operator==(const PointerEntry& other) const {
		return offset == other.offset && target == other.target && size == other.size;
	}
};

// Section for file organization (hierarchical with colors)
// Based on RTHextion Section system
struct Section
{
	string name;
	__int64 startOffset;
	__int64 endOffset;     // exclusive (one past last byte)
	COLORREF color;
	int parentIndex;      // -1 = root section; >=0 = parent index
	bool operator==(const Section& other) const {
		return startOffset == other.startOffset && endOffset == other.endOffset && name == other.name;
	}
};

// Named translation table entry for multi-table support
struct DocTableEntry
{
	string name;
	void* table;        // Will point to translation table data
	bool isOriginal;      // true if original encoding table
};

#endif