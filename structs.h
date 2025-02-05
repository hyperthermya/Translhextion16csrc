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

#endif