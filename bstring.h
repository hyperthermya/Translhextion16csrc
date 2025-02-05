#ifndef BSTRING_H
#define BSTRING_H

/****************************************************************************
** COPYRIGHT (C):    2000 Brian Bennewitz. All Rights Reserved.
** PROJECT:          Utility Library
** FILE:             bstring.h
** PURPOSE:          a subset of the ANSI string class with bounds checking
** Version           1.0
** PROGRAMMER:      Brian Bennewitz
****************************************************************************/

#line 14 "bstring.cpp"

/*MFC Includes*/
//#include "afx.h"
/*MFC*/

/*ANSI Includes*/
#include <iostream.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
/*ANSI*/

/*misc includes ?*/
////#include <cstring>
////#include <cctype>
////#include <cassert>

#line 37 "bstring.cpp"


#define CHECK_BOUNDS(I) (check_bounds(__FILE__, __LINE__, I))

class string
{
public:
   string();                       // constructs an empty string
   string(const char s[]);
   string(char ch);

   int length() const;
   string substr(int from, int n) const;

   char& operator[](int i);
   char operator[](int i) const;

   const string& operator+=(const string& s);
   const string& operator+=(const char s[]);
   const string& operator+=(char ch);

   string operator+(const string&) const;
   string operator+(const char b[]) const;
   string operator+(char) const;

   friend string operator+(const char a[], const string& b);
   friend string operator+(char ch, const string& b);

   friend ostream& operator<<(ostream& os, const string& s);
   friend istream& operator>>(istream& os, string& s);

   const char* c_str() const { return _str ? _str : ""; }
   int compare(const string&) const;
   int compare(const char s[]) const;

   /*added 3/9/00*/
   friend double floatvalue(string s);
   friend int intvalue(string s);
   /*end of add*/

   string(const string&);
   const string& operator=(const string&);
   ~string();

private:
   void check_bounds(const char* f, int l, int i) const;

   string(size_t len, const char s[], size_t slen);
   string(int);
   /* left undefined--to avoid accidental conversion int -> char -> string
   */
   void detach();
   void unique();

   char* _str;
};

/*-------------------------------------------------------------------------*/

class StringHeader
/* PURPOSE:    The header block with reference count and available space
   REMARKS:    An instance of this structure is BELOW each string
               that is allocated on the collectible heap
*/
{
public:
   int unique() { return _rc == 1; }
   void incref() { ++_rc; }
   int decref() { return --_rc; }

   size_t length() const { return _length; }

   static char* alloc(const char[]);
   static char* alloc(int, const char[], int);
   static void free(char*);

   static StringHeader* header(const char* p) { return (StringHeader*)p - 1; }

private:
   char* start() const { return (char*)(this+1); }

   unsigned _rc;                       // reference count
   size_t _length;

friend class string;
};

/* FUNCTIONS ***************************************************************/

inline int string::length() const
/* RETURNS:       the length of the string
*/
{  return _str ? StringHeader::header(_str)->_length : 0;
}

/*...........................................................................*/

inline int operator==(const string& a, const string& b)
{ return a.compare(b) == 0; }
inline int operator!=(const string& a, const string& b)
{ return a.compare(b) != 0; }
inline int operator<(const string& a, const string& b)
{ return a.compare(b) < 0; }
inline int operator<=(const string& a, const string& b)
{ return a.compare(b) <= 0; }
inline int operator>(const string& a, const string& b)
{ return a.compare(b) > 0; }
inline int operator>=(const string& a, const string& b)
{ return a.compare(b) >= 0; }
 
inline int operator==(const string& a, const char b[])
{ return a.compare(b) == 0; }
inline int operator!=(const string& a, const char b[])
{ return a.compare(b) != 0; }
inline int operator<(const string& a, const char b[])
{ return a.compare(b) < 0; }
inline int operator<=(const string& a, const char b[])
{ return a.compare(b) <= 0; }
inline int operator>(const string& a, const char b[])
{ return a.compare(b) > 0; }
inline int operator>=(const string& a, const char b[])
 { return a.compare(b) >= 0; }

inline int operator==(const char a[], const string& b)
{ return b.compare(a) == 0; }
inline int operator!=(const char a[], const string& b)
{ return b.compare(a) != 0; }
inline int operator<(const char a[], const string& b)
 { return b.compare(a) > 0; }
inline int operator<=(const char a[], const string& b)
 { return b.compare(a) >= 0; }
inline int operator>(const char a[], const string& b)
 { return b.compare(a) < 0; }
inline int operator>=(const char a[], const string& b)
{ return b.compare(a) <= 0; }


#line 362 "bstring.cpp"
extern string operator+(const char a[], const string& b);

#line 376 "bstring.cpp"
extern string operator+(char ch, const string& b);

#line 388 "bstring.cpp"
extern ostream& operator<<(ostream& os, const string& s);

#line 400 "bstring.cpp"
extern istream& operator>>(istream& is, string& s);


/*added 3/9/00*/

#line 523 "bstring.cpp"
extern string lowercase(string s);

#line 536 "bstring.cpp"
extern string uppercase(string s);

#line 549 "bstring.cpp"
extern int intvalue(string s);

#line 558 "bstring.cpp"
extern double floatvalue(string s);

#line 567 "bstring.cpp"
extern string stringvalue(int n);

#line 578 "bstring.cpp"
extern string stringvalue(long n);

#line 589 "bstring.cpp"
extern string stringvalue(double d);

#line 600 "bstring.cpp"
extern void getline(istream& is, string& r);

#line 631 "bstring.cpp"
extern string get_char(istream& is);

#line 642 "bstring.cpp"
extern void putback_char(istream& is, string s);
/*end of add*/
/*
#line 648 "bstring.cpp"
extern string stringvalue(CString cs);

#line 662 "bstring.cpp"
extern CString CStringvalue(string s);

#line 676 "bstring.cpp"
extern int intvalue(CString cs);

#line 686 "bstring.cpp"
extern double floatvalue(CString cs);

#line 694 "bstring.cpp"
extern CString CStringvalue(int n);

#line 705 "bstring.cpp"
extern CString CStringvalue(long n);

#line 716 "bstring.cpp"
extern CString CStringvalue(double d);
*/
#endif
