#include "filevect.h"

template<class T> FileVector<T>::FileVector()
{
	m_pT = NULL;
	m_nSize = 0;
	m_nUpperBound = EMPTY_STATE;
	m_nExpandBy = 1;
}

template<class T> FileVector<T>::FileVector(long iNewSize, long nExpandBy)
: m_nExpandBy(nExpandBy), m_nUpperBound(EMPTY_STATE), m_nSize(iNewSize)
{
	m_pT = new T[m_nSize];
}

template<class T> FileVector<T>::FileVector(T* ptArray, long upbound, long size)
: m_nExpandBy(1), m_pT(ptArray), m_nUpperBound(upbound), m_nSize(size)
{
#ifdef _DEBUG
	if(m_pT == NULL || m_nUpperBound<0 || m_nSize<=0 || m_nUpperBound >= m_nSize)
	{
		m_nSize = 0;
		m_nUpperBound = EMPTY_STATE;
	}
#endif
}

template<class T> FileVector<T>::FileVector(FileVector& spaArg)
	: m_nSize(spaArg.m_nSize), m_nUpperBound(spaArg.m_nUpperBound),
	m_nExpandBy(spaArg.m_nExpandBy)
{
	m_pT = new T[m_nSize];
	long k;
	for(k = 0; k <= m_nUpperBound; k++)
		m_pT[k] = spaArg.m_pT[k];
}

template<class T> FileVector<T>::~FileVector()
{
	if(m_pT != NULL) delete [] m_pT;
}

template<class T> long FileVector<T>::InsertExpandOn(long iIndex, T elem, long iCount)
{
	if(iIndex<0 || iCount<1)
		return FALSE;
	long i;
	if(iIndex > m_nUpperBound)
	{
		for(i = 0; i < iCount; i++)
			SetAtExpand(iIndex + i, elem);
		return TRUE;
	}
	else
	{
		if(m_nSize < m_nUpperBound + 1 + iCount)
		{
			if (AddSpace(iCount) == FALSE)
				return FALSE;
		}
		for(i = m_nUpperBound + iCount; i > iIndex; i--)
		{
			m_pT[i] = m_pT[i-iCount];
		}
		for(i = 0; i < iCount; i++)
			m_pT[iIndex + i] = elem;
		m_nUpperBound += iCount;
		return TRUE;
	}
}

template<class T> long FileVector<T>::InsertExpandOn (long iIndex, T* pT, long iSrcIndex, long iCount)
{
	if(iIndex<0 || iCount<1)
		return FALSE;
	long i;
	if(iIndex > m_nUpperBound)
	{
		for(i = 0; i < iCount; i++)
		{
			if (SetAtExpandRef(iIndex + i, pT[iSrcIndex+i]) == FALSE)
				return FALSE;
		}
		return TRUE;
	}
	else
	{
		if(m_nSize < m_nUpperBound + 1 + iCount)
		{
			if (AddSpace(iCount) == FALSE)
				return FALSE;
		}
		for(i = m_nUpperBound + iCount; i > iIndex; i--)
		{
			m_pT[i] = m_pT[i-iCount];
		}
		for(i = 0; i < iCount; i++)
			m_pT[iIndex + i] = pT[iSrcIndex+i];
		m_nUpperBound += iCount;
		return TRUE;
	}
}

template<class T> void FileVector<T>::InsertAtExpandRef(long iIndex, T& elem, long iCount)
{
	if(iIndex<0 || iCount<1) return;
	long i;
	if(iIndex > m_nUpperBound)
	{
		for(i = 0; i < iCount; i++)
			SetAtExpandRef(iIndex + i, elem);
		return;
	}
	else
	{
		if(m_nSize < m_nUpperBound + 1 + iCount)
			AddSpace(iCount);
		for(i = m_nUpperBound + iCount; i > iIndex; i--)
		{
			m_pT[i] = m_pT[i-iCount];
		}
		for(i = 0; i < iCount; i++)
			m_pT[iIndex + i] = elem;
		m_nUpperBound += iCount;
	}
}

template<class T> long FileVector<T>::InsertAt( long iIndex, T elem, long iCount )
{
	
	if( iIndex < 0 || iIndex > m_nUpperBound )
		return FALSE;

	long i;
	
	if( ( m_nSize - 1 ) - m_nUpperBound >= iCount )
	{
		for( i = m_nUpperBound + iCount; i >= iIndex + iCount; i-- )
			m_pT[ i ] = m_pT[ i - iCount ];
		for( i = 0; i < iCount; i++ )
			m_pT[ iIndex + i ] = elem;
		m_nUpperBound += iCount;
	}
	else
	{
		for (i = m_nSize - 1; i >= iIndex + iCount; i--)
			m_pT[i] = m_pT[i - iCount];
		if (m_nSize - iIndex < iCount)
			iCount = m_nSize - iIndex;
		for (i = 0; i < iCount; i++)
			m_pT[iIndex + i] = elem;
		m_nUpperBound = m_nSize - 1;		
	}
	return TRUE;
}

template<class T> void FileVector<T>::InsertAtRef(long iIndex, T& elem, long iCount)
{
	if(iIndex < 0 || iIndex > m_nUpperBound)
		return;
	long i;
	if ((m_nSize - 1) - m_nUpperBound >= iCount) 
	{
		for(i = m_nUpperBound + iCount; i >= iIndex + iCount; i--)
			m_pT[i] = m_pT[i - iCount];
		for(i = 0; i < iCount; i++)
			m_pT[iIndex + i] = elem;
		m_nUpperBound += iCount;
	}
	else
	{
		for (i = m_nSize - 1; i >= iIndex + iCount; i--)
			m_pT[i] = m_pT[i - iCount];
		if (m_nSize - iIndex < iCount)
			iCount = m_nSize - iIndex;
		for (i = 0; i < iCount; i++)
			m_pT[iIndex + i] = elem;
		m_nUpperBound = m_nSize - 1;		
	}
}

template<class T> long FileVector<T>::RemoveAt(long iIndex, long iCount)
{
	if(iIndex < 0 || iIndex > m_nUpperBound || iCount < 1) return FALSE;
	if(iCount > m_nUpperBound - iIndex)
	{
		m_nUpperBound = iIndex - 1;
		return TRUE;
	}
	long i;
	for(i = iIndex; i <= m_nUpperBound - iCount; i++)
	{
		m_pT[i] = m_pT[i + iCount];
	}
	m_nUpperBound -= iCount;
	return TRUE;
}

template<class T> void FileVector<T>::SetAtExpand(long iIndex, T elem)
{
	if(iIndex < 0) return;
	if(iIndex > m_nSize - 1)
		AddSpace(iIndex - m_nSize + 1);
	m_pT[iIndex] = elem;
	if(iIndex > m_nUpperBound) m_nUpperBound = iIndex;
}

template<class T> long FileVector<T>::SetAtExpandRef(long iIndex, T& elem)
{
	if(iIndex < 0)
		return FALSE;
	if(iIndex > m_nSize - 1)
	{
		if (AddSpace(iIndex - m_nSize + 1) == FALSE)
			return FALSE;
	}
	m_pT[iIndex] = elem;
	if(iIndex > m_nUpperBound)
		m_nUpperBound = iIndex;
	return TRUE;
}

template<class T> void FileVector<T>::SetAt(long iIndex, T elem)
{
	if(iIndex >= 0 && iIndex < m_nSize)
	{
		m_pT[iIndex] = elem;
		if(iIndex > m_nUpperBound)
			m_nUpperBound = iIndex;
	}
	else
		return;
}

template<class T> void FileVector<T>::SetAtRef(long iIndex, T& elem)
{
	if(iIndex >= 0 && iIndex < m_nSize)
	{
		m_pT[iIndex] = elem;
		if(iIndex > m_nUpperBound)
			m_nUpperBound = iIndex;
	}
	else
		return;
}

template<class T> T FileVector<T>::GetAt(long iIndex)
{
	return m_pT[iIndex];
}

template<class T> long FileVector<T>::AddSpace(long nExtend)
{
	long newsize = m_nSize + (((nExtend-1) / m_nExpandBy) + 1) * m_nExpandBy;
	T* pT = new T[newsize];
	if (pT != NULL)
	{
		long i;
		for(i = 0; i < m_nSize; i++)
			pT[i] = m_pT[i];
		if(m_pT != NULL)
			delete [] m_pT;
		m_pT = pT;
		m_nSize = newsize;
		return TRUE;
	}
	else
		return FALSE;
}

template<class T> long FileVector<T>::GetExpandBy()
{
	return m_nExpandBy;
}

template<class T> long FileVector<T>::GetSize()
{
	return m_nSize;
}

template<class T> long FileVector<T>::Upper()
{
	return m_nUpperBound;
}

template<class T> long FileVector<T>::Length()
{
	return m_nUpperBound+1;
}

template<class T> long FileVector<T>::SetLength(long iNewSize, long nExpandBy )
{
	if(iNewSize < 0)
		return FALSE;
	if (iNewSize == m_nSize)
		return TRUE;
	if( iNewSize == 0 )
	{
		Reset();
		return TRUE;
	}

	T* pT = new T[iNewSize];
	if (pT == NULL)
		return FALSE;
	long i;
	if(m_nUpperBound < iNewSize)
	{
		for(i = 0; i <= m_nUpperBound; i++)
			pT[i] = m_pT[i];
	}
	else
	{
		for(i = 0; i < iNewSize; i++)
			pT[i] = m_pT[i];
		m_nUpperBound = iNewSize - 1;
	}

	if(m_pT != NULL)
		delete [] m_pT;
	m_pT = pT;
	m_nSize = iNewSize;
	if(nExpandBy > 0)
		m_nExpandBy = nExpandBy;
	return TRUE;
}

template<class T> void FileVector<T>::SetExpandBy(long nExpandBy)
{
	if(nExpandBy > 0) m_nExpandBy = nExpandBy;
}

template<class T> void FileVector<T>::Reset()
{
	if(m_pT != NULL)
		delete [] m_pT;
	m_pT = NULL;
	m_nSize = 0;
	m_nUpperBound = EMPTY_STATE;
}

template<class T> long FileVector<T>::blContainsRef(T& elem)
{
	long i;
	for(i = 0; i <= m_nUpperBound; i++)
		if(elem == m_pT[i])
			return TRUE;
	return FALSE;
}

template<class T> long FileVector<T>::blContains(T elem)
{
	long i;
	for(i = 0; i <= m_nUpperBound; i++)
		if(elem == m_pT[i])
			return TRUE;
	return FALSE;
}

template<class T> long FileVector<T>::nContainsAt(T elem)
{
	long i;
	for(i = 0; i <= m_nUpperBound; i++)
		if(elem == m_pT[i])
			return i;
	return EMPTY_STATE;
}

template<class T> FileVector<T>& FileVector<T>::operator=( FileVector<T>& v )
{
	if( &v != this )
	{
		Reset();
		m_pT = new T[ m_nSize ];
		if( m_pT != NULL )
		{
			m_nUpperBound = v.m_nUpperBound;
			m_nSize = m_nUpperBound + 1;
			m_nExpandBy = v.m_nExpandBy;
			long k;
			for(k = 0; k <= m_nUpperBound; k++)
				m_pT[k] = v.m_pT[k];
		}
	}
	return *this;
}

template<class T> void FileVector<T>::AppendRef(T& elem)
{
	SetAt(m_nUpperBound+1, elem);
}

template<class T> void FileVector<T>::Append(T elem)
{
	SetAtExpand(m_nUpperBound+1, elem);
}

template<class T> T& FileVector<T>::GetRefAt(long iIndex)
{
	return m_pT[iIndex];
}

template<class T> long FileVector<T>::Compare(FileVector<T>& v)
{
	if(m_nUpperBound != v.Upper() ) return FALSE;
	long k;
	for(k = 0; k <= m_nUpperBound; k++)
	{
		if(m_pT[k] != v[k]) return FALSE;
	}
	return TRUE;
}

template<class T> long FileVector<T>::operator==(FileVector<T>& v)
{
	return Compare(v);
}

template<class T> long FileVector<T>::operator!=(FileVector<T>& v)
{
	return !Compare(v);
}

template<class T> long FileVector<T>::IsEmpty()
{
	return (Upper() < 0) ? TRUE : FALSE;
}

template<class T> void FileVector<T>::Exchange(long iIndex1, long iIndex2)
{
	T temp(GetRefAt(iIndex2));
	GetRefAt(iIndex2) = GetRefAt(iIndex1);
	GetRefAt(iIndex1) = temp;
}

template<class T> long FileVector<T>::Adopt(T* ptArray, long upbound, long size)
{
#ifdef _DEBUG
	if(ptArray == NULL || upbound<0 || size<=0 || upbound >= size)
		return FALSE;
#endif
	if(m_pT!=NULL)
		delete [] m_pT;
	m_pT = ptArray;
	m_nSize = size;
	m_nUpperBound = upbound;
	return TRUE;
}

template<class T> void FileVector<T>::SetUpperBound(long upbnd)
{
	if(upbnd < m_nSize)
		m_nUpperBound = upbnd;
}

template<class T> long FileVector<T>::AppendArray( T* pSrc, long iLengthgthOfSource )
{
	if( iLengthgthOfSource <= 0 )
		return FALSE;

	if( m_nUpperBound + 1 + iLengthgthOfSource > m_nSize )
	{
		if( !AddSpace( iLengthgthOfSource ) )
			return FALSE;
	}
	long i;
	for( i=0; i<iLengthgthOfSource; i++ )
	{
		m_pT[ m_nUpperBound + 1 + i ] = pSrc[i];
	}
	m_nUpperBound += iLengthgthOfSource;
	return TRUE;
}

template<class T> long FileVector<T>::ExpandToSize()
{
	m_nUpperBound = m_nSize - 1;
	return TRUE;
}

template<class T> long FileVector<T>::CopyFrom( long index, T* pSrc, long iLengthgthOfSource )
{
	if( m_nSize - index >= iLengthgthOfSource )
	{
		long i;
		for( i = 0; i < iLengthgthOfSource; i++ )
			m_pT[ index + i ] = pSrc[ i ];
		if( index + iLengthgthOfSource - 1 > m_nUpperBound )
			m_nUpperBound = index + iLengthgthOfSource - 1;
		return TRUE;
	}
	else
		return FALSE;
}

template<class T> long FileVector<T>::Replace( long ToReplaceIndex, long ToReplaceLengthgth, T* pReplaceWith, long ReplaceWithLengthgth )
{
	if( m_pT != NULL && ToReplaceLengthgth > 0 )
	{
		if( m_nUpperBound - ToReplaceIndex + 1 >= ToReplaceLengthgth )
		{
			if( ToReplaceLengthgth < ReplaceWithLengthgth )
			{
				long i;
				T dummy;

				dummy = 0;

				InsertExpandOn( ToReplaceIndex, dummy, ReplaceWithLengthgth - ToReplaceLengthgth );
				for( i = 0; i < ReplaceWithLengthgth; i++ )
				{
					m_pT[ ToReplaceIndex + i ] = pReplaceWith[ i ];
				}

				return TRUE;
			}
			else if( ToReplaceLengthgth == ReplaceWithLengthgth )
			{
				long i;
				for( i = 0; i < ReplaceWithLengthgth; i++ )
				{
					m_pT[ ToReplaceIndex + i ] = pReplaceWith[ i ];
				}
				return TRUE;
			}
			else 
			{
				long i;
				for( i = 0; i < ReplaceWithLengthgth; i++ )
				{
					m_pT[ ToReplaceIndex + i ] = pReplaceWith[ i ];
				}

				RemoveAt( ToReplaceIndex + ReplaceWithLengthgth, ToReplaceLengthgth - ReplaceWithLengthgth );
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}
