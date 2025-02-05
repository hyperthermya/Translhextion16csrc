#ifndef BVECT_H
#define BVECT_H

/****************************************************************************
** COPYRIGHT (C):    2000 Brian Bennewitz. All Rights Reserved.
** PROJECT:          Utility Library
** FILE:             bvect.h
** PURPOSE:          A subset of the ANSI vector template with bounds checking
** Version           1.0
** PROGRAMMER:      Brian Bennewitz
****************************************************************************/

#line 14 "bvect.cpp"

//   #ifdef ANSI
      #include <stddef.h>
      #include <string.h>
      #include <assert.h>
      #include <stdlib.h>
      #include <malloc.h>
//   #else
//      #include <cstddef>
//      #include <cstring>
//      #include <cassert>
//   #endif

#line 30 "bvect.cpp"


class ArrayHeader
/* PURPOSE:    The (invisible) header stored with each smart array
               It contains the reference count, current block size and
               growth amount.
   REMARKS:    This is stored below the array itself and hidden on purpose
               so the array values can be inspected easily in the debugger
*/
{
public:
   enum { GROWBY = 20, };

   bool unique() { return _rc == 1; }
   void incref() { ++_rc; }
   int decref() { return --_rc; }

   static ArrayHeader* alloc(int xsize, int size, int growby );

   static ArrayHeader* header(void*);

   void free();

   int block_size() const { return _block_size; }
   int growby() const { return _growby; }
   void* start() const;

private:
   unsigned _rc;                       // reference count
   int _block_size;
   int _growby;
   /* 
		do not remove this comment block!!      


   */
   int _filler; 
};

/*-------------------------------------------------------------------------*/

template<class X> class vector
/* PURPOSE:    smart array class template
   RECEIVES:   X - any type with copy construction and assignment
*/
{
public:
   vector()
   :  _elements(0),
      _size(0)
   {}

   vector(int hi)
   {  init(hi);
   }

   void push_back(X t);
   void pop_back();
   void push_front(X t);
   void pop_front();
   void insert(X t, int i);
   void remove(int i);
   int size() const { return _size; }

   X& operator[](int i)
   {  assert(0 <= i && i < _size);
      unique();
      return _elements[i];
   }

   X const& operator[](int i) const
   {  assert(0 <= i && i < _size);
      return _elements[i];
   }

   vector(const vector& b)
   :  _elements(b._elements),
      _size(b._size)
   {  if (_elements) ArrayHeader::header(_elements)->incref(); }
   
   const vector& operator=(const vector& b)
   {  if (b._elements) ArrayHeader::header(b._elements)->incref();
      detach();
      _elements = b._elements;
      _size = b._size;
      return *this;
   }

   ~vector() { detach(); }

private:
   void init(int hi);
   void ins(int ins1, int keep1, int ins2);
   void rem(int rem1, int keep1, int rem2, int keep2);
   void resize(int to, int place, int left);
   static void construct_block(X* p, int size);
   static void copy_block(X* p, const X* from, int size);
   static void destroy_block(X* p, int size);
   static void move_block(X* tt, const X* ff, int size);
   static const X& defval();

   void detach();
   void unique();

   X* _elements;
   int _size;
};


/* FUNCTIONS ***************************************************************/

template< class X >
void vector<X>::detach()
/* PURPOSE:    decrement reference count and free associated object if
          necessary
*/
{  if (_elements && ArrayHeader::header(_elements)->decref() == 0)
   {  ArrayHeader* ph = ArrayHeader::header(_elements);
      destroy_block(_elements, ph->block_size());
      ph->free();
   }
   _elements = 0;
   _size = 0;
}

/*.........................................................................*/

template< class X >
void vector<X>::unique()
/* PURPOSE:    clone array rep if ref count > 1
*/
{  if (_elements && !ArrayHeader::header(_elements)->unique())
   {  ArrayHeader* ph = ArrayHeader::header(_elements);
      ph->decref();
      ArrayHeader* nh = ArrayHeader::alloc(sizeof(X), ph->block_size(), ph->growby());
      if (nh != 0)
      {  // initialize all array entries
         X* pp = _elements;
         _elements = (X*)nh->start();
         copy_block(_elements, pp, ph->block_size());
         construct_block(_elements + ph->block_size(), nh->block_size() - ph->block_size());
      }
   }
}

/*.........................................................................*/

template< class X >
void vector<X>::resize(int newsize, int place, int left)
/* PURPOSE:    resize array
   RECEIVES:   newsize - the new desired size
*/
{  if (_elements)
   {  
	  if(place == -1)
	  {
		 if (newsize > _size)
		   ins(newsize - _size, _size, 0);
         else if (newsize > 0)
           rem( _size - newsize, newsize, 0, 0);
         else
           detach();
	  }
	  else if(place == 0)
	  {
		 if (newsize > _size)
		   ins(0, left, newsize - _size);
         else if (newsize > 0)
           rem( 0, left, _size - newsize, newsize - left);
         else
           detach();
	  }
	  else
	  {
         if (newsize > _size)
		   ins(0, _size, newsize - _size);
         else if (newsize > 0)
           rem( 0, newsize, _size - newsize, 0);
         else
           detach();
	  }
   }
   else if (newsize > 0)
      init(newsize);
}

/*.........................................................................*/

template< class X >
void vector<X>::push_back(X t)
/* PURPOSE:    append an element at the end of the array
   RECEIVES:   t - the element to insert
   REMARKS:    if the array is empty, the element is placed at index 0
*/
{  resize(_size + 1,1,0);
   // changes _size
   _elements[_size - 1] = t;
}

/*.........................................................................*/

template< class X >
void vector<X>::pop_back()
/* PURPOSE:    removes an element from the end of the array
   REMARKS:    if the array is empty, it is not changed
*/
{  if (_size > 0)
      resize(_size - 1,1,0);
}

/*.........................................................................*/

template< class X >
void vector<X>::push_front(X t)
/* PURPOSE:    append an element at the front of the array
   RECEIVES:   t - the element to insert
   REMARKS:    if the array is empty, the element is placed at index 0
*/
{  resize(_size + 1,-1,0);
   // changes _size
   _elements[0] = t;
}

/*.........................................................................*/

template< class X >
void vector<X>::pop_front()
/* PURPOSE:    removes an element from the front of the array
   REMARKS:    if the array is empty, it is not changed
*/
{  if (_size > 0)
      resize(_size - 1,-1,0);
}

/*.........................................................................*/

template< class X >
void vector<X>::insert(X t, int i)
/* PURPOSE:    inserts element into middle of array
   RECEIVES:   t - the element to insert , i - the index to insert at
				(moves current values starting at i up 1)
   REMARKS:    if the array is empty, the element is placed at index 0
*/
{  resize(_size + 1,0,i);
   // changes _size
   _elements[i] = t;
}


/*.........................................................................*/

template< class X >
void vector<X>::remove(int i)
/* PURPOSE:    deletes element from middle of array
   RECEIVES:   i - the index to remove
   REMARKS:    if the array is empty, then nothing happens
*/
{  if (_size > 0)
      resize(_size - 1,0,i);
}


/*.........................................................................*/

template< class X >
void vector<X>::init(int newsize)

/* PURPOSE:    Allocate array storage from the heap
          and initialize it with the default constructor
   RECEIVES:   lo, hi - array range
*/
{  // allocate storage

   if (0 >= newsize)
   {  _elements = 0;
      _size = 0;
      return;
   }

   _size = newsize;
   ArrayHeader* ph = ArrayHeader::alloc(sizeof(X), _size, ArrayHeader::GROWBY);
   if (ph == 0) return;

   // initialize all array entries

   _elements = (X*)ph->start();
   construct_block(_elements, ph->block_size());
}

/*.........................................................................*/

template< class X >
void vector<X>::construct_block(X* p, int size)
/* PURPOSE:    run the default constructor on a block of elements
   RECEIVES:   p - start of block
          size - number of elements
*/
{  for (int i = 0; i < size; i++)
      new(p + i, (ArrayHeader*)0) X;
}

/*.........................................................................*/

template< class X >
void vector<X>::copy_block(X* p, const X* from, int size)
/* PURPOSE:    run the copy constructor to make a copy of a block of elements
   RECEIVES:   p - start of target block
          from - start of source block
          size - number of elements
*/
{  for (int i = 0; i < size; i++)
      new(p + i, (ArrayHeader*)0) X(from[i]);
}

/*.........................................................................*/

template< class X >
void vector<X>::destroy_block(X* p, int size)
/* PURPOSE:    run the destructor on a block of elements
   RECEIVES:   p - start of block
               size - number of elements
   REMARKS:    it is legal to run the "destructor" on a non-class type
               (e.g. X = int) but some compilers do not support this.
               Borland 3.1 and 4.0 give a warning "p not used" if X has no
               destructor. For that reason, we make a spurious usage of p.
*/
{  for (int i = size - 1; i >= 0; i--)
#ifndef DONT_CALL_DESTRUCTORS
#ifndef DONT_CALL_STATIC_DESTRUCTORS
      (p + i)->X::~X()
#else
      (p + i)->~X()
#endif
#endif
      ;

#ifdef AVOID_UNUSED_ARG_WARNING
   static void* avoid_unused_arg_warning = p;
#endif
}

/*.........................................................................*/

template< class X >
void vector<X>::move_block(X* tt, const X* ff, int size)
/* PURPOSE:    run the copy constructor and destructor to move a block of elements
   RECEIVES:   tt - start of target block
          from - start of source block
          size - number of elements
   REMARKS:    source and target may overlap
*/
{  if (size == 0) return;
   if (tt == ff) return;
   if (tt > ff) // move from high to low address to avoid overwrite
   {  tt += size;
      ff += size;
      for (int i = size; i > 0; i--)
      {  tt--;
         ff--;
         new(tt, (ArrayHeader*)0) X(*ff);
#ifndef DONT_CALL_DESTRUCTORS
#ifndef DONT_CALL_STATIC_DESTRUCTORS
         ff->X::~X();
#else
         ff->~X();
#endif
#endif
      }
   }
   else
   {  for (int i = 0; i < size; i++)
      {  new(tt, (ArrayHeader*)0) X(*ff);
#ifndef DONT_CALL_DESTRUCTORS
#ifndef DONT_CALL_STATIC_DESTRUCTORS
         ff->X::~X();
#else
         ff->~X();
#endif
#endif
         tt++;
         ff++;
      }
   }
}

/*.........................................................................*/

template< class X >
void vector<X>::ins(int ins1, int keep1, int ins2)
/* PURPOSE:    Expand the array
   RECEIVES:   ins1 - the #new entries to insert at the bottom of the array
               keep1 - the #entries to keep after that
               ins2 - the #new entries to insert after that
   RETURNS:    the new address
   REMARKS:    For grow, keep2 is 0
               For insert, ins1 is 0
*/
{  int keep2 = _size - keep1;
   _size += ins1 + ins2;

   int new_size = ins1 + keep1 + ins2 + keep2;
   ArrayHeader* ph = ArrayHeader::header(_elements);
   if (new_size <= ph->block_size() && ins1 == 0 && keep2 == 0) return;
   bool uniq = ph->unique();
    // true iff pp is the only reference to that information

   if (uniq && new_size <= ph->block_size()) // shift entries
   {  if (ins1 + ins2 > 0)
      {  // start on top since shifting up
         destroy_block(_elements + keep1 + keep2, ins1 + ins2);
         move_block(_elements + ins1 + keep1 + ins2, _elements + keep1, keep2);
         if (ins2 > 0)
            construct_block(_elements + ins1 + keep1, ins2);
         if (ins1 > 0)
         {  move_block(_elements + ins1, _elements, keep1);
            construct_block(_elements, ins1);
         }
      }
   }
   else // allocate new block
   {  ph->decref();
      ArrayHeader* nh = ArrayHeader::alloc(sizeof(X), new_size, ph->growby());
      if (nh == 0) return;

      X* pp = _elements;
      _elements = (X*)nh->start();

      // initialize all new array entries

      if (ins1) construct_block(_elements, ins1);
      copy_block(_elements + ins1, pp, keep1);
      construct_block(_elements + ins1 + keep1, ins2);
      if (keep2)  copy_block(_elements + ins1 + keep1 + ins2, pp + keep1, keep2);
      construct_block(_elements + new_size, nh->block_size() - new_size);

      if (uniq)
      {  destroy_block(pp, ph->block_size());
         ph->free();
      }
   }
}

/*.........................................................................*/

template< class X >
void vector<X>::rem(int rem1, int keep1, int rem2, int keep2)
/* PURPOSE:    remove elements from the array
   RECEIVES:   rem1 - #elements to remove at bottom
               keep1 - #elements to keep after that
               rem2 - #elements to remove after that
               keep2 - #elements to keep after that
*/
{  _size -= rem1 + rem2;
   int new_size = keep1 + keep2;
   if (new_size == 0) { detach(); return; }
   ArrayHeader* ph = ArrayHeader::header(_elements);
   if (rem1 == 0 && rem2 == 0 && new_size > ph->block_size() - ph->growby() )
      return;

   bool uniq = ph->unique();
      // true iff pp is the only reference to that information

   if (uniq && new_size > ph->block_size() - ph->growby()) // shift entries
   {  if (rem1 > 0)
      {  destroy_block(_elements, rem1);
         move_block(_elements, _elements + rem1, keep1);
      }
      if (rem2 > 0)
         destroy_block(_elements + rem1 + keep1, rem2);
      move_block(_elements + keep1, _elements + rem1 + keep1 + rem2, keep2);
      construct_block(_elements + keep1 + keep2, rem1 + rem2);
   }
   else // allocate new block
   {  ph->decref();
      ArrayHeader* nh = ArrayHeader::alloc(sizeof(X), new_size, ph->growby());
      if (nh == 0) return;

      X* pp = _elements;
      _elements = (X*)nh->start();

      copy_block(_elements, pp + rem1, keep1);
      copy_block(_elements + keep1, pp + rem1 + keep1 + rem2, keep2);
      construct_block(_elements + new_size, nh->block_size() - new_size);
      if (uniq)
      {  destroy_block(pp, ph->block_size());
         ph->free();
      }
   }
}

//............................................................................

template< class X >
const X& vector<X>::defval()
/* PURPOSE:    return a default value of type X
*/
{  static X d;
   return d;
}


#line 534 "bvect.cpp"
extern void* operator new(size_t, void* p, ArrayHeader*);

#endif
