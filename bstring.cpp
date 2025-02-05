/****************************************************************************
** COPYRIGHT (C):    2000 Brian Bennewitz. All Rights Reserved.
** PROJECT:          Utility Library
** FILE:             bstring.cpp
** PURPOSE:          a subset of the ANSI string class with bounds checking
** Version           1.0
** PROGRAMMER:      Brian Bennewitz
****************************************************************************/

/* IMPORT ******************************************************************/

#define EXPORT

#include "bstring.h"

/* TYPES *******************************************************************/

#ifndef EXPORT

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

#endif
  // EXPORT


/*.........................................................................*/

string::string(size_t tlen, const char s[], size_t slen)
/* RECEIVES:      tlen - total length of the string
        s - character string to initialize first bytes
        slen - length of s
   REMARKS:       (1) The string gets a 0 at position len
        (2) This constructor is private, and its interface is made
          slightly obscure on purpose to avoid accidental invocation
          by class users. Originally we had a string(size_t),
          and under some circumstances the compiler complained about
          ambiguities with String(char)
*/
:  _str(StringHeader::alloc(tlen, s, slen))
{}

/*...........................................................................*/

string::string(const string& b)
/* REMARKS:    Inlining this function doesn't work with Borland 4.5
*/
:  _str(b._str)
{  if (_str) StringHeader::header(_str)->incref(); }

/*.........................................................................*/

string::~string()
/* REMARKS:    Inlining this function doesn't work with Borland 4.5
*/
{  detach();
}

/*...........................................................................*/

string::string()
/* REMARKS:    Constructs an empty string
               Inlining this function doesn't work with Borland 4.5
*/
:  _str(0)
{}

/*...........................................................................*/

string::string(const char s[])
/* RECEIVES:   s - C character array
*/
:  _str(StringHeader::alloc(s))
{}

/*...........................................................................*/

string::string(char ch)
/* RECEIVES:   ch - single character
*/
:  _str(StringHeader::alloc(1, &ch, 1))
{}

/*...........................................................................*/

char string::operator[](int i) const
/* RETURNS:       the ith character in a string (0 = first character)
   REMARKS:       fails if i out of bounds
*/
{  CHECK_BOUNDS(i);
   return _str[i];
}

/*...........................................................................*/

char& string::operator[](int i)
/* RETURNS:       a reference to the ith character in a string (0 = first character)
   REMARKS:       fails if i out of bounds
                  It does NOT grow the string
*/
{  CHECK_BOUNDS(i);
   unique();
   return _str[i];
}

/*.........................................................................*/

const string& string::operator=(const string& b)
{  if (b._str) StringHeader::header(b._str)->incref();
   detach();
   _str = b._str;
   return (*this);
}

//............................................................................

int string::compare(const string& b) const
/* PURPOSE:    lexicographically compares two strings
   RECEIVES:   b - string to compare with (*this)
   RETURNS:    < 0 if (*this) before b
         0 if (*this) equals b
        > 0 if (*this) after b
*/
{  return strcmp(c_str(), b.c_str());
}

//............................................................................

int string::compare(const char s[]) const
/* PURPOSE:    lexicographically compares two strings
   RECEIVES:   b - string to compare with (*this)
   RETURNS:    < 0 if (*this) before b
         0 if (*this) equals b
         > 0 if (*this) after b
*/
{  return strcmp(c_str(), s ? s : "");
}

/*...........................................................................*/

string string::operator+(const string& b) const
/* PURPOSE:    concatenate two strings
   RECEIVES:   b - the string to concatenate with (*this)
   RETURNS:    the concatenated string
   REMARKS:    (*this) is not affected
*/
{  int len = length();
   string r(len + b.length(), _str, len);
   if (b._str) strcpy(r._str + len, b._str);
   return r;
}

/*...........................................................................*/

string string::operator+(const char b[]) const
/* PURPOSE:    concatenate two strings
   RECEIVES:   b - the string to concatenate with (*this)
   RETURNS:    the concatenated string
   REMARKS:    (*this) is not affected
*/
{  int len = length();
   int blen = b ? strlen(b) : 0;
   string r(len + blen, _str, len);
   if (blen) strcpy(r._str + len, b);
   return r;
}

/*...........................................................................*/

string string::operator+(char ch) const
/* PURPOSE:    concatenate a string and a character
   RECEIVES:   ch - the character to concatenate with (*this)
   RETURNS:    the concatenated string
   REMARKS:    (*this) is not affected
*/
{  int len = length();
   string r(len + 1, _str, len);
   r._str[ len ] = ch;
   return r;
}

//............................................................................

string string::substr(int from, int n) const
/* PURPOSE:       Get a substring from a string
   RECEIVES:      from - the starting index (0 = beginning of the string)
                  n - the number of characters to copy into substring
   RETURNS:       the substring
   REMARKS:       if from + n >= string length, takes characters until end of string
*/
{  int len = length();
   if (from < 0) from = 0;
   if (from + n >= len) n = len - from;
   return string(n, _str + from, n);
}

/*.........................................................................*/

void string::detach()
/* PURPOSE:    decrement reference count and free associated object if
         necessary
*/
{  if (_str && StringHeader::header(_str)->decref() == 0)
	StringHeader::free(_str);
}

/*.........................................................................*/

void string::unique()
/* PURPOSE:    makes copy of representation if reference count > 1
*/
{  if (_str && !StringHeader::header(_str)->unique())
   {  StringHeader::header(_str)->decref();
    int len = length();
    _str = StringHeader::alloc(len, _str, len);
   }
}

/*.........................................................................*/

void string::check_bounds(const char* f, int l, int i) const
/* PURPOSE:    check for bounds error
   RECEIVES:   f - file name
               l - iLine number
               i - actual index
*/
{  int s = length();
   if (0 <= i && i < s) return;
   cerr << "String index error index = " << i << " size = " << s;
   cerr << "\nFile " << f << " iLine " << l << "\n";
   exit(1);
}

/*.........................................................................*/

EXPORT string operator+(const char a[], const string& b)
/* PURPOSE:    concatenate two strings
   RECEIVES:   a, b - the strings to concatenate
   RETURNS:    the concatenated string
*/

{  int alen = a ? strlen(a) : 0;
   string r(alen + b.length(), a, alen);
   if (b._str) strcpy(r._str + alen, b._str);
   return r;
}

/*.........................................................................*/

EXPORT string operator+(char ch, const string& b)
/* PURPOSE:    concatenate a character and a string
   RECEIVES:   ch, b - the character and string to concatenate
   RETURNS:    the concatenated string
*/
{  string r(1 + b.length(), &ch, 1);
   if (b._str) strcpy(r._str + 1, b._str);
   return r;
}

/*.........................................................................*/

EXPORT ostream& operator<<(ostream& os, const string& s)
/* PURPOSE:    Print a string on a stream
   RECEIVES:   os - the stream
         s - the string to print
   RETURNS:    os
*/
{     return os << s.c_str();
}


/*.........................................................................*/

EXPORT istream& operator>>(istream& is, string& s)
/* PURPOSE:    Read a string from a stream
   RECEIVES:   is - the stream
         s (OUT) - the string that is read
   RETURNS:    is
*/
{  const int BUFLEN = 80;
   char buffer[BUFLEN];
   ostream* os = is.tie(); // CSH added 3-14-97
   if (os != NULL) os->flush(); // CSH added 3-14-97
   ws(is); // skip whitespace
   if (!is) return is;
   s = "";
   do
   {  is.width(sizeof(buffer));
      is >> buffer;
      if (is) s += buffer;
   }
   while (is && !isspace(is.peek()));
   if (s.length() > 0 && !is) is.clear(); // CSH 1998-01-21
   return is;
}

/*...........................................................................*/

const string& string::operator+=(const string& b)
/* PURPOSE:    append a string to (*this)
   RECEIVES:   b - the string to append
   RETURNS:    (*this)
*/
{  int len = length();
   string r(len + b.length(), _str, len);
   if (b._str) strcpy(r._str + length(), b._str);
   (*this) = r;
   return (*this);
}

/*...........................................................................*/

const string& string::operator+=(const char b[])
/* PURPOSE:    append a string to (*this)
   RECEIVES:   b - the string to append
   RETURNS:    (*this)
*/
{  if (b == 0) return (*this);
   int len = length();
   string r(len + (int)strlen(b), _str, len);
   if (r._str) strcpy(r._str + len, b);
   (*this) = r;
   return (*this);
}

/*...........................................................................*/

const string& string::operator+=(char ch)
/* PURPOSE:    append a character to (*this)
   RECEIVES:   ch - the character to append
   RETURNS:    (*this)
   REMARKS:    this is slow since the entire string must be copied.
        Do not use this function to build up a string. Instead,
         use grow() to desired size and set() when possible
*/
{  int len = length();
   string r(len + 1, _str, len);
   r._str[ len ] = ch;
   (*this) = r;
   return (*this);
}

/*---------------------------------------------------------------------------*/

char* StringHeader::alloc(const char s[])
/* PURPOSE:    allocate space for a header and a string
   RECEIVES:   s - the string to copy above the header
   RETURNS:    the start of the string
*/
{  if (s == 0) return 0;
   int len = strlen(s);
   if (len == 0) return 0;
   StringHeader* ph = (StringHeader*)::malloc(len + 1 + sizeof(StringHeader));
   assert(ph != 0);
   ph->_length = len;
   ph->_rc = 1;
   char* t = (char*)(ph+1);
   strcpy(t, s);
   return t;
}

/*...........................................................................*/

char* StringHeader::alloc(int len, const char s[], int ncopy)
/* PURPOSE:    allocate space for a header and a string
   RECEIVES:   len - the total number of characters to allocate
         s - the string to copy above the header
         ncopy - the number of characters in s to copy
   RETURNS:    the start of the string
*/
{  if (len == 0) return 0;
   StringHeader* ph = (StringHeader*)::malloc(len + 1 + sizeof(StringHeader));
   assert(ph != 0);
   ph->_length = len;
   ph->_rc = 1;
   char* t = (char*)(ph+1);
   assert(len >= ncopy);
   if (s && ncopy) strncpy(t, s, ncopy);
   t[ len ] = 0;
   return t;
}

/*...........................................................................*/

void StringHeader::free(char* s)
/* PURPOSE:    free a block containing a header and string
   RECEIVES:   s - the start of the string
*/
{  StringHeader* ph = header(s);
   assert(ph->_rc == 0);
   assert(ph->_length > 0);
   ::free(ph);
}

//............................................................................

EXPORT string lowercase(string s)
/* PURPOSE:    Convert to lowercase
   RETURNS:    a string identical to s, except all characters are lowercase
*/
{  string r = s;
   for (int i = 0; i < s.length(); i++)
      r[i] = (char)tolower(s[i]);   /* CSH 1997/08/21 */

   return r;
}

//............................................................................

EXPORT string uppercase(string s)
/* PURPOSE:    Convert to upperercase
   RETURNS:    a string identical to s, except all characters are uppercase
*/
{  string r = s;
   for (int i = 0; i < s.length(); i++)
      r[i] = (char)toupper(s[i]);   /* CSH 1997/08/21 */

   return r;
}

/*...........................................................................*/

EXPORT int intvalue(string s)
/* PURPOSE:    Converts a string to a long
   RETURNS:    The equivalent long (0 if none found).
*/
{  return s._str ? atoi(s._str) : 0;
}

/*...........................................................................*/

EXPORT double floatvalue(string s)
/* PURPOSE:    Converts a string to floating point
   RETURNS:    The equivalent floating point
*/
{  return s._str ? atof(s._str) : 0;
}

/*...........................................................................*/

EXPORT string stringvalue(int n)
/* PURPOSE:    Converts a long to a string.
   RECEIVES:   The long to convert.
*/
{  char cBuf[3*sizeof(int)];
   sprintf(cBuf, "%d", n);
   return string(cBuf);
}

/*...........................................................................*/

EXPORT string stringvalue(long n)
/* PURPOSE:    Converts a long to a string.
   RECEIVES:   The long to convert.
*/
{  char cBuf[3*sizeof(long)];
   sprintf(cBuf, "%ld", n);
   return string(cBuf);
}

/*...........................................................................*/

EXPORT string stringvalue(double d)
/* PURPOSE:    Converts a double to a string (in general format, with 6 digits)
   RECEIVES:   The double to convert.
*/
{  char cBuf[2 * DBL_DIG];
   sprintf(cBuf, "%g", d);
   return string(cBuf);
}

/*...........................................................................*/

EXPORT void getline(istream& is, string& r)
/* PURPOSE:    read a iLine until newline or end of file
   RECEIVES:   r - the iLine to fill
               is - the input stream
   RETURNS:    the iLine
   REMARKS:    the terminating newline is not part of the string
*/
{  const int BUFLEN = 80;
   char buffer[BUFLEN];
   bool more = true;
   r = "";
   while (more)
   {  is.get(buffer, BUFLEN - 1);
      if (is.fail()) more = false;
      else
      {  int blen = strlen(buffer);
         char ch = (char)is.get(); /* CSH 1997-11-07 */
         if (!is.fail())
         {  if (ch == '\n') more = false;
            else
            {  buffer[blen] = ch;
               buffer[blen + 1] = 0;
            }
         }
         r += buffer;
      }
   }
}

//............................................................................

EXPORT string get_char(istream& is)
{  char ch;
   if (!is.fail())
   {  is.get(ch);
      if (!is.fail()) return string(ch);
   }
   return "";
}

//............................................................................

EXPORT void putback_char(istream& is, string s)
{  if (s != "") is.putback(s[0]); /* CSH 11 Aug 1997 */
}


/*
EXPORT string stringvalue(CString cs)
// PURPOSE:    Converts a MS CString to a string
//   RECEIVES:   The MS CString to convert.
//
{
   string s;
   int i = 0;
   for(i = 0; i < cs.Length(); i++)
	   s += cs[i];
   return s;
}

//............................................................................

EXPORT CString CStringvalue(string s)
// PURPOSE:    Converts a string to a MS CString
//   RECEIVES:   The string to convert.
//
{
   CString cs;
   int i = 0;
   for(i = 0; i < s.length(); i++)
	   cs += s[i];
   return cs;
}
///
//............................................................................

EXPORT int intvalue(CString cs)
// PURPOSE:    Converts a string to a long
//   RETURNS:    The equivalent long (0 if none found).
//
{   string s = stringvalue(cs);
	return intvalue(s);//s._str ? atoi(s._str) : 0;
}

//...........................................................................//

EXPORT double floatvalue(CString cs)
// PURPOSE:    Converts a string to floating point
//   RETURNS:    The equivalent floating point
//
{   string s = stringvalue(cs);
	return floatvalue(s);//s._str ? atof(s._str) : 0;
}

EXPORT CString CStringvalue(int n)
// PURPOSE:    Converts a long to a string.
//   RECEIVES:   The long to convert.
//
{  char cBuf[3*sizeof(int)];
   sprintf(cBuf, "%d", n);
   return CStringvalue(string(cBuf));
}

//...........................................................................//

EXPORT CString CStringvalue(long n)
// PURPOSE:    Converts a long to a string.
//   RECEIVES:   The long to convert.
//
{  char cBuf[3*sizeof(long)];
   sprintf(cBuf, "%ld", n);
   return CStringvalue(string(cBuf));
}

//...........................................................................//

EXPORT CString CStringvalue(double d)
// PURPOSE:    Converts a double to a string (in general format, with 6 digits)
//   RECEIVES:   The double to convert.
//
{  char cBuf[2 * DBL_DIG];
   sprintf(cBuf, "%g", d);
   return CStringvalue(string(cBuf));
}
*/