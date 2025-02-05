//=========================================================
// File: filevect.h

#if !defined(__FILEVECTOR_H)
#define __FILEVECTOR_H

#define EMPTY_STATE -1

#include <string.h>

template<class T> class FileVector
{
public:
	FileVector();
	FileVector(T* ptArray, long upbound, long size);
	FileVector(long iNewSize, long nExpandBy = 1);
	virtual ~FileVector();
	FileVector(FileVector& spaArg);
	operator T*()
	{
		return m_pT;
	}
	long InsertAt(long iIndex, T elem, long iCount = 1);
	void InsertAtRef(long iIndex, T& elem, long iCount = 1);
	long InsertExpandOn(long iIndex, T elem, long iCount = 1);
	void InsertAtExpandRef(long iIndex, T& elem, long iCount = 1);
	long InsertExpandOn (long iIndex, T* pT, long iSrcIndex, long iCount);
	long RemoveAt(long iIndex, long iCount = 1);
	void SetAtExpand(long iIndex, T elem);
	long SetAtExpandRef(long iIndex, T& elem);
	void SetAt(long iIndex, T elem);
	void SetAtRef(long iIndex, T& elem);
	T GetAt(long iIndex);
	T& GetRefAt(long iIndex);
	long GetSize();
	long Upper();
	long Length();
	long GetExpandBy();
	long SetLength(long iNewSize, long nExpandBy = 0);
	void SetExpandBy(long nExpandBy);
	T& operator[](long iIndex) {return m_pT[iIndex];}
	T& operator[](int iIndex) {return m_pT[iIndex];}
	FileVector<T>& operator=(FileVector<T>& v);
	void Reset();
	long blContainsRef(T& elem);
	long blContains(T elem);
	long nContainsAt(T elem);
	long IsEmpty();
	void AppendRef(T& elem);
	void Append(T elem);
	void Exchange(long iIndex1, long iIndex2);
	long Compare(FileVector<T>& v);
	long operator==(FileVector<T>& v);
	long operator!=(FileVector<T>& v);
	long Adopt(T* ptArray, long upbound, long size);
	void SetUpperBound(long upbnd);
	long AppendArray( T* pSrc, long iLengthgthOfSource );
	long ExpandToSize();
	long CopyFrom( long index, T* pSrc, long iLengthgthOfSource );
	long Replace( long ToReplaceIndex, long ToReplaceLengthgth, T* pReplaceWith, long ReplaceWithLengthgth );

protected:
	long AddSpace (long nExtend);
	T* m_pT;
	long m_nSize;
	long m_nUpperBound;
	long m_nExpandBy;
};

#endif // #if !defined(__FILEVECTOR_H)
