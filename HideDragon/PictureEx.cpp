
#include "stdafx.h"
#include "PictureEx.h"
#include <tchar.h>


#define szShareDumpData			L"HTMLManager"
void dbgPrintf(const TCHAR* fmt, ...)
{
	va_list marker;
	TCHAR szBuf[1024];

	va_start(marker, fmt);
	wvsprintf(szBuf, fmt, marker);
	va_end(marker);

	HANDLE hMap = OpenFileMapping( FILE_MAP_ALL_ACCESS,FALSE, szShareDumpData );
	LPWSTR pString = (LPWSTR)MapViewOfFile(hMap,FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);

	if(pString && wcslen(szBuf) )
	{
		/*lstrcpy*/wcscpy( pString, szBuf );	
		UnmapViewOfFile((LPCVOID)pString);
	} 
	CloseHandle(hMap);
	Sleep(100);
	
}

LRESULT CALLBACK CPictureEx::WindowProc(
										  HWND hwnd,
										  UINT uMsg, 
										  WPARAM wParam, 
										  LPARAM lParam 
									   )
{
     CPictureEx * pMyThis = (CPictureEx *)hwnd;
     switch(uMsg) 
	 {
	 case WM_SETCURSOR:	//设置鼠标为手型
		 SetCursor(LoadCursor(NULL, IDC_HAND));
		 return 0;
	 case WM_LBUTTONDOWN:
		 return 0;
	 case WM_PAINT:
		pMyThis->OnPaint();
		return DefWindowProc(pMyThis->m_hWnd,uMsg,wParam,lParam);


	 case WM_DESTROY:
		 pMyThis->OnDestroy();
		 return DefWindowProc(pMyThis->m_hWnd,uMsg,wParam,lParam);


	 }
	 return CallWindowProc(m_pOldWindowProc,pMyThis->m_hWnd,uMsg,wParam,lParam);
}

WNDPROC   CPictureEx::m_pOldWindowProc = NULL;
//////////////////////////////////////////////////////////////////////
// Nested structures member functions
//////////////////////////////////////////////////////////////////////

inline int CPictureEx::TGIFControlExt::GetPackedValue(enum ControlExtValues Value)
{
	int nRet = (int) m_cPacked;
	switch(Value)
	{
    	case GCX_PACKED_DISPOSAL:
			nRet = (nRet & 28) >> 2;
			break;

    	case GCX_PACKED_USERINPUT:
			nRet = (nRet & 2) >> 1;
			break;

    	case GCX_PACKED_TRANSPCOLOR:
			nRet &= 1;
			break;
	};

	return nRet;
}

inline int CPictureEx::TGIFLSDescriptor::GetPackedValue(enum LSDPackedValues Value)
{
	int nRet = (int) m_cPacked;

	switch(Value)
	{
    	case LSD_PACKED_GLOBALCT:
			nRet = nRet >> 7;
			break;

    	case LSD_PACKED_CRESOLUTION:
			nRet = ((nRet & 0x70) >> 4) + 1;
			break;

    	case LSD_PACKED_SORT:
			nRet = (nRet & 8) >> 3;
			break;

    	case LSD_PACKED_GLOBALCTSIZE:
			nRet &= 7;
			break;
	};

	return nRet;
}

inline int CPictureEx::TGIFImageDescriptor::GetPackedValue(enum IDPackedValues Value)
{
	int nRet = (int) m_cPacked;

	switch(Value)
	{
    	case ID_PACKED_LOCALCT:
			nRet >>= 7;
			break;

    	case ID_PACKED_INTERLACE:
			nRet = ((nRet & 0x40) >> 6);
			break;

    	case ID_PACKED_SORT:
			nRet = (nRet & 0x20) >> 5;
			break;

    	case ID_PACKED_LOCALCTSIZE:
			nRet &= 7;
			break;
	};

	return nRet;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPictureEx::CPictureEx()
{
	// check structures size
	assert(sizeof(TGIFImageDescriptor) == 10);
	assert(sizeof(TGIFAppExtension) == 14);
	assert(sizeof(TGIFPlainTextExt) == 15);
	assert(sizeof(TGIFLSDescriptor) == 7);
	assert(sizeof(TGIFControlExt) == 8);
	assert(sizeof(TGIFCommentExt) == 2);
	assert(sizeof(TGIFHeader) == 6);

	m_pGIFLSDescriptor = NULL;
	m_pGIFHeader = NULL;
	m_pPicture = NULL;
	m_pRawData = NULL;
	m_hThread = NULL;
	m_hBitmap = NULL;
	m_hMemDC = NULL;

	m_hDispMemDC = NULL;
	m_hDispMemBM = NULL;
	m_hDispOldBM = NULL;

	m_bIsInitialized = FALSE;
	m_bExitThread = FALSE;
	m_bIsPlaying = FALSE;
	m_bIsGIF = FALSE;
	m_clrBackground = RGB(255, 255, 255); // white by default
	m_nGlobalCTSize = 0;
	m_nCurrOffset = 0;
	m_nCurrFrame = 0;
	m_nDataSize = 0;
	m_PictureSize.cx = m_PictureSize.cy = 0;
	SetRect(&m_PaintRect, 0, 0, 0, 0);

	m_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	m_thunk.Init(WindowProc, this);

}


BOOL CPictureEx::Attach(HWND hWnd)
{
	m_hWnd = hWnd;
	WNDPROC pProc = (WNDPROC)&(m_thunk.thunk);
	CPictureEx::m_pOldWindowProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)pProc);
	
	
	dbgPrintf(L"CPictureEx(HWND hWnd = %x\n)",m_hWnd);





	return TRUE;
}


CPictureEx::~CPictureEx()
{
	dbgPrintf(L"~CPictureEx()\n");
	UnLoad();
	CloseHandle(m_hExitEvent);
}

BOOL CPictureEx::Load(HGLOBAL hGlobal, DWORD dwSize)
{
	
	dbgPrintf(L"Load(HGLOBAL hGlobal, DWORD dwSize)\n");
	IStream* pStream = NULL;
	UnLoad();

	if(!(m_pRawData = reinterpret_cast<unsigned char*>(GlobalLock(hGlobal))))
	{
		OutputDebugString(L"Load: Error locking memory\n");
		return FALSE;
	};

	m_nDataSize = dwSize;
	m_pGIFHeader = reinterpret_cast<TGIFHeader*>(m_pRawData);

	if((memcmp(&m_pGIFHeader->m_cSignature, "GIF", 3) != 0)
    && ((memcmp(&m_pGIFHeader->m_cVersion, "87a", 3) != 0)
     || (memcmp(&m_pGIFHeader->m_cVersion, "89a", 3) != 0)))
	{
		// it's neither GIF87a nor GIF89a
		// do the default processing

		// clear GIF variables
		m_pRawData = NULL;
		GlobalUnlock(hGlobal);

		// don't delete memory on object's release
		if(CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) != S_OK)
			return FALSE;

		if(OleLoadPicture(pStream,
                          dwSize,
                          FALSE,
                          IID_IPicture,
                          reinterpret_cast<LPVOID*>(&m_pPicture)) != S_OK)
		{
			pStream->Release();
			return FALSE;
		};
		pStream->Release();

		// store picture's size

		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		HDC hDC = ::GetDC(m_hWnd);
		m_PictureSize.cx = MulDiv(hmWidth, GetDeviceCaps(hDC, LOGPIXELSX), 2540);
		m_PictureSize.cy = MulDiv(hmHeight, GetDeviceCaps(hDC, LOGPIXELSY), 2540);
		::ReleaseDC(m_hWnd, hDC);
	}
	else
	{
		// it's a GIF
		m_bIsGIF = TRUE;
		m_pGIFLSDescriptor = reinterpret_cast<TGIFLSDescriptor*>(m_pRawData + sizeof(TGIFHeader));
		if(m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCT) == 1)
		{
			// calculate the globat color table size
			m_nGlobalCTSize = static_cast<int>(3 * (1
			                                     << (m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCTSIZE)
			                                       + 1)));
			// get the background color if GCT is present
			unsigned char * pBkClr = m_pRawData
			                       + sizeof(TGIFHeader) + sizeof(TGIFLSDescriptor)
			                       + 3 * m_pGIFLSDescriptor->m_cBkIndex;
			m_clrBackground = RGB(pBkClr[0], pBkClr[1], pBkClr[2]);
		};

		// store the picture's size
		m_PictureSize.cx = m_pGIFLSDescriptor->m_wWidth;
		m_PictureSize.cy = m_pGIFLSDescriptor->m_wHeight;

		// determine frame count for this picture
		UINT nFrameCount = 0;
		ResetDataPointer();
		while(SkipNextGraphicBlock())
			nFrameCount++;

#ifdef GIF_TRACING
		OutputDebugString(_T(" -= GIF encountered\n"
		         "Logical Screen dimensions = %dx%d\n"
		         "Global color table = %d\n"
		         "Color depth = %d\n"
		         "Sort flag = %d\n"
		         "Size of Global Color Table = %d\n"
		         "Background color index = %d\n"
		         "Pixel aspect ratio = %d\n"
		         "Frame count = %d\n"
		         "Background color = %06Xh\n\n"),
		      m_pGIFLSDescriptor->m_wWidth,
		      m_pGIFLSDescriptor->m_wHeight,
		      m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCT),
		      m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_CRESOLUTION),
		      m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_SORT),
		      m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCTSIZE),
		      m_pGIFLSDescriptor->m_cBkIndex,
		      m_pGIFLSDescriptor->m_cPixelAspect,
		      nFrameCount,
		      m_clrBackground);
		EnumGIFBlocks();
#endif

		if(nFrameCount == 0) // it's an empty GIF!
		{
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);
			return FALSE;
		};

		// now check the frame count
		// if there's only one frame, no need to animate this GIF
		// therefore, treat it like any other pic

		if(nFrameCount == 1)
		{
			// clear GIF variables
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			// don't delete memory on object's release
			if(CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) != S_OK)
				return FALSE;

			if(OleLoadPicture(pStream, dwSize, FALSE, IID_IPicture, (LPVOID *) &m_pPicture) != S_OK)
			{
				pStream->Release();
				return FALSE;
			};

			pStream->Release();
		}
		else
		{
			// if, on the contrary, there are several frames
			// then store separate frames in an array

			TFrame frame;
			UINT nBlockLen;
			HGLOBAL hFrameData;
			UINT nCurFrame = 0;

			ResetDataPointer();
			while(hFrameData = GetNextGraphicBlock(&nBlockLen,
                                                   &frame.m_nDelay,
                                                   &frame.m_frameSize,
                                                   &frame.m_frameOffset,
                                                   &frame.m_nDisposal))
			{
#ifdef GIF_TRACING
				//////////////////////////////////////////////
				// uncomment the following strings if you want 
				// to write separate frames on disk
				//
				//  CString szName;
				//  szName.Format(_T("%.4d.gif"),nCurFrame);
				//  WriteDataOnDisk(szName,hFrameData,nBlockLen);
				//  nCurFrame++;
#endif // GIF_TRACING

				IStream* pStream = NULL;

				// delete memory on object's release
				if(CreateStreamOnHGlobal(hFrameData, TRUE, &pStream) != S_OK)
				{
					GlobalFree(hFrameData);
					continue;
				};

				if(OleLoadPicture(pStream,
                                  nBlockLen,
                                  FALSE,
                                  IID_IPicture,
                                  reinterpret_cast<LPVOID*>(&frame.m_pPicture)) != S_OK)
				{
					pStream->Release();
					continue;
				};
				pStream->Release();

				// everything went well, add this frame
				m_arrFrames.push_back(frame);
			};

			// clean after ourselves
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			if(m_arrFrames.empty()) // couldn't load any frames
				return FALSE;
		};
	}; // if (!IsGIF...

	return PrepareDC(m_PictureSize.cx, m_PictureSize.cy);
}


BOOL CPictureEx::Load(HINSTANCE hinst,LPCTSTR szResourceName, LPCTSTR szResourceType)
{
	assert(szResourceName);
	assert(szResourceType);

	HRSRC hPicture = FindResource(hinst,szResourceName,szResourceType);
	HGLOBAL hResData;
	if (!hPicture || !(hResData = LoadResource(hinst,hPicture)))
	{
		dbgPrintf(L"Load (resource): Error loading resource %s\n",szResourceName);
		return FALSE;
	};
	DWORD dwSize = SizeofResource(hinst,hPicture);

	// hResData is not the real HGLOBAL (we can't lock it)
	// let's make it real

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
	if (!hGlobal)
	{
		dbgPrintf(L"Load (resource): Error allocating memory\n");
		FreeResource(hResData);
		return FALSE;
	};
	
	char *pDest = reinterpret_cast<char *> (GlobalLock(hGlobal));
	char *pSrc = reinterpret_cast<char *> (LockResource(hResData));
	if (!pSrc || !pDest)
	{
		dbgPrintf(L"Load (resource): Error locking memory\n");
		GlobalFree(hGlobal);
		FreeResource(hResData);
		return FALSE;
	};
	CopyMemory(pDest,pSrc,dwSize);
	FreeResource(hResData);
	GlobalUnlock(hGlobal);

	BOOL bRetValue = Load(hGlobal,dwSize);
	GlobalFree(hGlobal);
	return bRetValue;
}




BOOL CPictureEx::Invalidate()
{
	OnPaint();



	return TRUE;
}






void CPictureEx::UnLoad()
{
	dbgPrintf(L"CPictureEx::UnLoad()\n");
	Stop();
	if(m_pPicture)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	};

	std::vector<TFrame>::iterator it;
	for(it = m_arrFrames.begin();it < m_arrFrames.end();it++)
		(*it).m_pPicture->Release();
	m_arrFrames.clear();

	if(m_hMemDC)
	{
		SelectObject(m_hMemDC, m_hOldBitmap);
		::DeleteDC(m_hMemDC);
		::DeleteObject(m_hBitmap);
		m_hMemDC = NULL;
		m_hBitmap = NULL;
	};

	if(m_hDispMemDC)
	{
		SelectObject(m_hDispMemDC, m_hDispOldBM);
		::DeleteDC(m_hDispMemDC);
		::DeleteObject(m_hDispMemBM);
		m_hDispMemDC = NULL;
		m_hDispMemBM = NULL;
	};

	SetRect(&m_PaintRect, 0, 0, 0, 0);
	m_pGIFLSDescriptor = NULL;
	m_pGIFHeader = NULL;
	m_pRawData = NULL;
	m_hThread = NULL;
	m_bIsInitialized = FALSE;
	m_bExitThread = FALSE;
	m_bIsGIF = FALSE;
	m_clrBackground = RGB(255, 255, 255); // white by default
	m_nGlobalCTSize = 0;
	m_nCurrOffset = 0;
	m_nCurrFrame = 0;
	m_nDataSize = 0;
}

BOOL CPictureEx::Draw()
{
	if(!m_bIsInitialized)
	{
		OutputDebugString(L"Call one of the CPictureEx::Load() member functions before calling Draw()\n");
		return FALSE;
	};

	if(IsAnimatedGIF())
	{
		// the picture needs animation
		// we'll start the thread that will handle it for us

		unsigned int nDummy;
		m_hThread = (HANDLE) _beginthreadex(NULL,
		                                    0,
		                                    _ThreadAnimation,
		                                    (void *)this,
		                                    CREATE_SUSPENDED,
		                                    &nDummy);
		if(!m_hThread)
		{
			OutputDebugString(L"Draw: Couldn't start a GIF animation thread\n");
			return FALSE;
		}
		else
			ResumeThread(m_hThread);
	}
	else
	{
		if(m_pPicture)
		{
			long hmWidth;
			long hmHeight;
			m_pPicture->get_Width(&hmWidth);
			m_pPicture->get_Height(&hmHeight);
			if(m_pPicture->Render(m_hMemDC,
                                  0,
                                  0,
                                  m_PictureSize.cx,
                                  m_PictureSize.cy,
                                  0,
                                  hmHeight,
                                  hmWidth,
                                  -hmHeight,
                                  NULL) == S_OK)
			{
				InvalidateRect(m_hWnd,NULL,FALSE);
				return TRUE;
			};
		};
	};

	return FALSE;
}

SIZE CPictureEx::GetSize() const
{
	return m_PictureSize;
}

BOOL CPictureEx::Load(LPCTSTR szFileName)
{
	assert(szFileName);
    
	FILE * file;
	HGLOBAL hGlobal;
	DWORD dwSize;
    file = _tfopen(szFileName, _T("rb"));
	if(!file)
	{
		OutputDebugString(_T("Load (file): Error opening file\n"));
		return FALSE;
	};

	fseek(file,0,SEEK_END);
	dwSize = ftell(file);
	fseek(file,0,SEEK_SET);


	hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, dwSize);
	if(!hGlobal)
	{
		OutputDebugString(_T("Load (file): Error allocating memory\n"));
		return FALSE;
	};

	char* pData = reinterpret_cast<char*>(GlobalLock(hGlobal));
	if(!pData)
	{
		OutputDebugString(_T("Load (file): Error locking memory\n"));
		GlobalFree(hGlobal);
		return FALSE;
	};

	if (fread(pData,1,dwSize,file) != dwSize)
	{
		dbgPrintf(_T("Load (file): Error reading the file %s\n"),
		            	szFileName);
		GlobalFree(hGlobal);
		fclose(file);
		return FALSE;
	};

	GlobalUnlock(hGlobal);
	fclose(file);

	BOOL bRetValue = Load(hGlobal, dwSize);
	GlobalFree(hGlobal);
	return bRetValue;
}

void CPictureEx::ResetDataPointer()
{
	// skip header and logical screen descriptor
	m_nCurrOffset = sizeof(TGIFHeader) + sizeof(TGIFLSDescriptor) + m_nGlobalCTSize;
}

BOOL CPictureEx::SkipNextGraphicBlock()
{
	if(!m_pRawData)
		return FALSE;

	// GIF header + LSDescriptor [+ GCT] [+ Control block] + Data

	enum GIFBlockTypes nBlock;

	nBlock = GetNextBlock();

	while((nBlock != BLOCK_CONTROLEXT)
       && (nBlock != BLOCK_IMAGE)
       && (nBlock != BLOCK_PLAINTEXT)
       && (nBlock != BLOCK_UNKNOWN)
       && (nBlock != BLOCK_TRAILER))
	{
		if(!SkipNextBlock())
			return NULL;
		nBlock = GetNextBlock();
	};

	if((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
		return FALSE;

	// it's either a control ext.block, an image or a plain text

	if(GetNextBlockLen() <= 0)
		return FALSE;

	if(nBlock == BLOCK_CONTROLEXT)
	{
		if(!SkipNextBlock())
			return FALSE;
		nBlock = GetNextBlock();

		// skip everything until we meet an image block or a plain-text block
		while((nBlock != BLOCK_IMAGE)
           && (nBlock != BLOCK_PLAINTEXT)
           && (nBlock != BLOCK_UNKNOWN)
           && (nBlock != BLOCK_TRAILER))
		{
			if(!SkipNextBlock())
				return NULL;
			nBlock = GetNextBlock();
		};

		if((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
			return FALSE;
	};

	// skip the found data block (image or plain-text)
	if(!SkipNextBlock())
		return FALSE;

	return TRUE;
}

UINT CPictureEx::GetSubBlocksLen(UINT nStartingOffset) const
{
	UINT nRet = 0;
	UINT nCurOffset = nStartingOffset;

	while(m_pRawData[nCurOffset] != 0 )
	{
		nRet += m_pRawData[nCurOffset] + 1;
		nCurOffset += m_pRawData[nCurOffset] + 1;
	};

	return nRet + 1;
}

enum CPictureEx::GIFBlockTypes CPictureEx::GetNextBlock() const
{
	switch(m_pRawData[m_nCurrOffset])
	{
    	case 0x21:
			// extension block
			switch(m_pRawData[m_nCurrOffset + 1])
			{
            	case 0x01:
					// plain text extension
					return BLOCK_PLAINTEXT;
					break;

            	case 0xF9:
					// graphic control extension
					return BLOCK_CONTROLEXT;
					break;

            	case 0xFE:
					// comment extension
					return BLOCK_COMMEXT;
					break;

            	case 0xFF:
					// application extension
					return BLOCK_APPEXT;
					break;
			};
			break;

    	case 0x3B:
			// trailer
			return BLOCK_TRAILER;
			break;

    	case 0x2C:
			// image data
			return BLOCK_IMAGE;
			break;
	};

	return BLOCK_UNKNOWN;
}

BOOL CPictureEx::SkipNextBlock()
{
	if(!m_pRawData)
		return FALSE;

	int nLen = GetNextBlockLen();
	if((nLen <= 0) || ((m_nCurrOffset + nLen) > m_nDataSize))
		return FALSE;

	m_nCurrOffset += nLen;
	return TRUE;
}

int CPictureEx::GetNextBlockLen() const
{
	GIFBlockTypes nBlock = GetNextBlock();

	int nTmp;

	switch(nBlock)
	{
    	case BLOCK_UNKNOWN:
			return -1;
			break;

    	case BLOCK_TRAILER:
			return 1;
			break;

    	case BLOCK_APPEXT:
			nTmp = GetSubBlocksLen(m_nCurrOffset + sizeof(TGIFAppExtension));
			if(nTmp > 0)
				return sizeof(TGIFAppExtension) + nTmp;
			break;

    	case BLOCK_COMMEXT:
			nTmp = GetSubBlocksLen(m_nCurrOffset + sizeof(TGIFCommentExt));
			if(nTmp > 0)
				return sizeof(TGIFCommentExt) + nTmp;
			break;

    	case BLOCK_CONTROLEXT:
			return sizeof(TGIFControlExt);
			break;

    	case BLOCK_PLAINTEXT:
			nTmp = GetSubBlocksLen(m_nCurrOffset + sizeof(TGIFPlainTextExt));
			if(nTmp > 0)
				return sizeof(TGIFPlainTextExt) + nTmp;
			break;

    	case BLOCK_IMAGE:
			TGIFImageDescriptor* pIDescr = reinterpret_cast<TGIFImageDescriptor*>(&m_pRawData[m_nCurrOffset]);
			int nLCTSize = (int)
			               (pIDescr->GetPackedValue(ID_PACKED_LOCALCT) * 3 * (1
			                                                               << (pIDescr->GetPackedValue(ID_PACKED_LOCALCTSIZE)
			                                                                 + 1)));

			int nTmp = GetSubBlocksLen(m_nCurrOffset + sizeof(TGIFImageDescriptor) + nLCTSize + 1);
			if(nTmp > 0)
				return sizeof(TGIFImageDescriptor) + nLCTSize + 1 + nTmp;
			break;
	};

	return 0;
}

UINT __stdcall CPictureEx::_ThreadAnimation(LPVOID pParam)
{
	assert(pParam);
	CPictureEx* pPic = reinterpret_cast<CPictureEx*>(pParam);

	pPic->m_bIsPlaying = TRUE;
	pPic->ThreadAnimation();
	pPic->m_bIsPlaying = FALSE;

	// this thread has finished its work so we close the handle
	CloseHandle(pPic->m_hThread); 
	// and init the handle to zero (so that Stop() doesn't Wait on it)
	pPic->m_hThread = 0;
	return 0;
}

void CPictureEx::ThreadAnimation()
{
	// first, restore background (for stop/draw support)
	// disposal method #2
	if(m_arrFrames[m_nCurrFrame].m_nDisposal == 2)
	{
		HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
		if(hBrush)
		{
			RECT rect =
			{
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx
              + m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy
              + m_arrFrames[m_nCurrFrame].m_frameSize.cy
			};
			FillRect(m_hMemDC, &rect, hBrush);
			DeleteObject(hBrush);
		};
	}
	else
	// disposal method #3
	if(m_hDispMemDC && (m_arrFrames[m_nCurrFrame].m_nDisposal == 3))
	{
		// put it back
		BitBlt(m_hMemDC,
		       m_arrFrames[m_nCurrFrame].m_frameOffset.cx+0,
		       m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
		       m_arrFrames[m_nCurrFrame].m_frameSize.cx,
		       m_arrFrames[m_nCurrFrame].m_frameSize.cy,
		       m_hDispMemDC,
		       0,
		       0,
		       SRCCOPY);
		// init variables
		SelectObject(m_hDispMemDC, m_hDispOldBM);
		DeleteDC(m_hDispMemDC); m_hDispMemDC = NULL;
		DeleteObject(m_hDispMemBM); m_hDispMemBM = NULL;
	};

	while(!m_bExitThread)
	{
		if(m_arrFrames[m_nCurrFrame].m_pPicture)
		{
			///////////////////////////////////////////////////////
			// Before rendering a frame we should take care of what's 
			// behind that frame. TFrame::m_nDisposal will be our guide:
			//   0 - no disposal specified (do nothing)
			//   1 - do not dispose (again, do nothing)
			//   2 - restore to background color (m_clrBackground)
			//   3 - restore to previous

			//////// disposal method #3
			if(m_arrFrames[m_nCurrFrame].m_nDisposal == 3)
			{
				// prepare a memory DC and store the background in it
				m_hDispMemDC = CreateCompatibleDC(m_hMemDC);
				m_hDispMemBM = CreateCompatibleBitmap(m_hMemDC,
				                                      m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				                                      m_arrFrames[m_nCurrFrame].m_frameSize.cy);

				if(m_hDispMemDC && m_hDispMemBM)
				{
					m_hDispOldBM = reinterpret_cast<HBITMAP>(SelectObject(m_hDispMemDC, m_hDispMemBM));
					BitBlt(m_hDispMemDC,
					       0,
					       0,
					       m_arrFrames[m_nCurrFrame].m_frameSize.cx,
					       m_arrFrames[m_nCurrFrame].m_frameSize.cy,
					       m_hMemDC,
					       m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
					       m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
					       SRCCOPY);
				};
			};
			///////////////////////

			long hmWidth;
			long hmHeight;
			m_arrFrames[m_nCurrFrame].m_pPicture->get_Width(&hmWidth);
			m_arrFrames[m_nCurrFrame].m_pPicture->get_Height(&hmHeight);

			if(m_arrFrames[m_nCurrFrame].m_pPicture->Render(m_hMemDC,
                                                            m_arrFrames[m_nCurrFrame].m_frameOffset.cx+0,
                                                            m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
                                                            m_arrFrames[m_nCurrFrame].m_frameSize.cx,
                                                            m_arrFrames[m_nCurrFrame].m_frameSize.cy,
                                                            0,
                                                            hmHeight,
                                                            hmWidth,
                                                           -hmHeight,
                                                            NULL)
             == S_OK)
			{

				InvalidateRect(m_hWnd,NULL,FALSE);

			}

			if(m_bExitThread)
				break;

			// if the delay time is too short (like in old GIFs), wait for 100ms
			if(m_arrFrames[m_nCurrFrame].m_nDelay < 5)
				WaitForSingleObject(m_hExitEvent, 100);
			else
				WaitForSingleObject(m_hExitEvent, 10 * m_arrFrames[m_nCurrFrame].m_nDelay);

			if(m_bExitThread)
				break;

			// disposal method #2
			if(m_arrFrames[m_nCurrFrame].m_nDisposal == 2)
			{
				HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
				if(hBrush)
				{
					RECT rect =
					{
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx
                      + m_arrFrames[m_nCurrFrame].m_frameSize.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy
                      + m_arrFrames[m_nCurrFrame].m_frameSize.cy
					};
					FillRect(m_hMemDC, &rect, hBrush);
					DeleteObject(hBrush);
				};
			}
			else if(m_hDispMemDC && (m_arrFrames[m_nCurrFrame].m_nDisposal == 3))
			{
				// put it back
				BitBlt(m_hMemDC,
				       m_arrFrames[m_nCurrFrame].m_frameOffset.cx+0,
				       m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				       m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				       m_arrFrames[m_nCurrFrame].m_frameSize.cy,
				       m_hDispMemDC,
				       0,
				       0,
				       SRCCOPY);
				// init variables
				SelectObject(m_hDispMemDC, m_hDispOldBM);
				DeleteDC(m_hDispMemDC); m_hDispMemDC = NULL;
				DeleteObject(m_hDispMemBM); m_hDispMemBM = NULL;
			};
		};
		m_nCurrFrame++;
		if(m_nCurrFrame == m_arrFrames.size())
		{
			m_nCurrFrame = 0; 
			// init the screen for the first frame,
			HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
			if(hBrush)
			{
				RECT rect =
				{
					0, 0, m_PictureSize.cx, m_PictureSize.cy
				};
				FillRect(m_hMemDC, &rect, hBrush);
				DeleteObject(hBrush);
			};
		};
	};
}

void CPictureEx::Stop()
{
	dbgPrintf(L"Stop()");
	m_bIsPlaying = FALSE;
	m_bExitThread = TRUE;
	SetEvent(m_hExitEvent);
	if(m_hThread)
	{
		// we'll wait for 5 seconds then continue execution
		WaitForSingleObject(m_hThread, 5000);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	// make it possible to Draw() again
	ResetEvent(m_hExitEvent);
	m_bExitThread = FALSE;
}

HGLOBAL CPictureEx::GetNextGraphicBlock(UINT* pBlockLen,
                                        UINT* pDelay,
                                        SIZE* pBlockSize,
                                        SIZE* pBlockOffset,
                                        UINT* pDisposal)
{
	if(!m_pRawData)
		return NULL;

	// GIF header + LSDescriptor [+ GCT] [+ Control block] + Data

	*pDisposal = 0;
	enum GIFBlockTypes nBlock;
	nBlock = GetNextBlock();

	while((nBlock != BLOCK_CONTROLEXT)
       && (nBlock != BLOCK_IMAGE)
       && (nBlock != BLOCK_PLAINTEXT)
       && (nBlock != BLOCK_UNKNOWN)
       && (nBlock != BLOCK_TRAILER))
	{
		if(!SkipNextBlock())
			return NULL;
		nBlock = GetNextBlock();
	};

	if((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
		return NULL;

	// it's either a control ext.block, an image or a plain text

	int nStart = m_nCurrOffset;
	int nBlockLen = GetNextBlockLen();

	if(nBlockLen <= 0)
		return NULL;

	if(nBlock == BLOCK_CONTROLEXT)
	{
		// get the following data
		TGIFControlExt* pControl = reinterpret_cast<TGIFControlExt*>(&m_pRawData[m_nCurrOffset]);
		// store delay time
		*pDelay = pControl->m_wDelayTime;
		// store disposal method
		*pDisposal = pControl->GetPackedValue(GCX_PACKED_DISPOSAL);

		if(!SkipNextBlock())
			return NULL;
		nBlock = GetNextBlock();

		// skip everything until we find data to display 
		// (image block or plain-text block)

		while((nBlock != BLOCK_IMAGE)
           && (nBlock != BLOCK_PLAINTEXT)
           && (nBlock != BLOCK_UNKNOWN)
           && (nBlock != BLOCK_TRAILER))
		{
			if(!SkipNextBlock())
				return NULL;
			nBlock = GetNextBlock();
			nBlockLen += GetNextBlockLen();
		};

		if((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))
			return NULL;
		nBlockLen += GetNextBlockLen();
	}
	else
		*pDelay = -1; // to indicate that there was no delay value

	if(nBlock == BLOCK_IMAGE)
	{
		// store size and offsets
		TGIFImageDescriptor* pImage = reinterpret_cast<TGIFImageDescriptor*>(&m_pRawData[m_nCurrOffset]);
		pBlockSize->cx = pImage->m_wWidth;
		pBlockSize->cy = pImage->m_wHeight;
		pBlockOffset->cx = pImage->m_wLeftPos;
		pBlockOffset->cy = pImage->m_wTopPos;
	};

	if(!SkipNextBlock())
		return NULL;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED,
	                              sizeof(TGIFHeader)
	                            + sizeof(TGIFLSDescriptor) + m_nGlobalCTSize
	                            + nBlockLen
	                            + 1);  // for the trailer

	if(!hGlobal)
		return NULL;

	int nOffset = 0; 

	// GMEM_FIXED means we get a pointer
	unsigned char * pGlobal = reinterpret_cast<unsigned char*>(hGlobal);

	CopyMemory(pGlobal, m_pRawData, sizeof(TGIFHeader) + sizeof(TGIFLSDescriptor) + m_nGlobalCTSize);
	nOffset += sizeof(TGIFHeader) + sizeof(TGIFLSDescriptor) + m_nGlobalCTSize;

	CopyMemory(pGlobal + nOffset, &m_pRawData[nStart], nBlockLen);
	nOffset += nBlockLen;

	pGlobal[nOffset] = 0x3B; // trailer
	nOffset++;

	*pBlockLen = nOffset;

	return hGlobal;
}

BOOL CPictureEx::IsGIF() const
{
	return m_bIsGIF;
}

BOOL CPictureEx::IsAnimatedGIF() const
{
	return (m_bIsGIF && (m_arrFrames.size() > 1));
}

BOOL CPictureEx::IsPlaying() const
{
	return m_bIsPlaying;
}

int CPictureEx::GetFrameCount() const
{
	if(!IsAnimatedGIF())
		return 0;

	return m_arrFrames.size();
}

COLORREF CPictureEx::GetBkColor() const
{
	return m_clrBackground;
}

void CPictureEx::OnPaint()
{
	HDC m_hDC  = GetDC(m_hWnd);

	LONG nPaintWidth = m_PaintRect.right - m_PaintRect.left;

	RECT rtHwnd;
	GetWindowRect(m_hWnd,&rtHwnd);
	
	int len = rtHwnd.right - rtHwnd.left;




	if(nPaintWidth > 0)
	{
		LONG nPaintHeight = m_PaintRect.bottom - m_PaintRect.top;
		::BitBlt(m_hDC,
		         (len-510)/2,
		         0,
		         nPaintWidth,
		         nPaintHeight,
		         m_hMemDC,
		         m_PaintRect.left,
		         m_PaintRect.top,
		         SRCCOPY);
	}
	else
		::BitBlt(m_hDC, (len-510)/2, 0, m_PictureSize.cx, m_PictureSize.cy, m_hMemDC, 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd,m_hDC);



	InvalidateRect(m_hWnd,NULL,TRUE);
}

BOOL CPictureEx::PrepareDC(int nWidth, int nHeight)
{
	::SetWindowPos(m_hWnd,NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER);

	HDC hWinDC = ::GetDC(m_hWnd);
	if(!hWinDC)
		return FALSE;

	m_hMemDC = CreateCompatibleDC(hWinDC);
	if(!m_hMemDC)
	{
		::ReleaseDC(m_hWnd, hWinDC);
		return FALSE;
	};

	m_hBitmap = CreateCompatibleBitmap(hWinDC, nWidth, nHeight);
	if(!m_hBitmap)
	{
		::ReleaseDC(m_hWnd, hWinDC);
		::DeleteDC(m_hMemDC);
		return FALSE;
	};

	m_hOldBitmap = reinterpret_cast<HBITMAP>(SelectObject(m_hMemDC, m_hBitmap));

	// fill the background
	m_clrBackground = GetSysColor(COLOR_3DFACE);
	RECT rect =
	{
		0, 0, nWidth, nHeight
	};
	FillRect(m_hMemDC, &rect, (HBRUSH) (COLOR_WINDOW));

	::ReleaseDC(m_hWnd, hWinDC);
	m_bIsInitialized = TRUE;
	return TRUE;
}

void CPictureEx::OnDestroy()
{
	Stop(); 
}

void CPictureEx::SetBkColor(COLORREF clr)
{
	if(!m_bIsInitialized)
		return;

	m_clrBackground = clr;

	HBRUSH hBrush = CreateSolidBrush(clr);
	if(hBrush)
	{
		RECT rect =
		{
			0, 0, m_PictureSize.cx, m_PictureSize.cy
		};
		FillRect(m_hMemDC, &rect, hBrush);
		DeleteObject(hBrush);
	};
}

#ifdef GIF_TRACING
void CPictureEx::WriteDataOnDisk(CString szFileName, HGLOBAL hData, DWORD dwSize)
{
	CFile file;

	if(!file.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))
	{
		OutputDebugString(_T("WriteData: Error creating file %s\n"), szFileName);
		return;
	};

	char* pData = reinterpret_cast<char*>(GlobalLock(hData));
	if(!pData)
	{
		OutputDebugString(_T("WriteData: Error locking memory\n"));
		return;
	};

	TRY
	{
		file.Write(pData, dwSize);
	}
	CATCH(CFileException, e);
	{
		OutputDebugString(_T("WriteData: An exception occured while writing to the file %s\n"), szFileName);
		e->Delete();
		GlobalUnlock(hData);
		file.Close();
		return;
	}
	END_CATCH GlobalUnlock(hData);
	file.Close();
}

void CPictureEx::EnumGIFBlocks()
{
	enum GIFBlockTypes nBlock;

	ResetDataPointer();
	while(m_nCurrOffset < m_nDataSize)
	{
		nBlock = GetNextBlock();
		switch(nBlock)
		{
        	case BLOCK_UNKNOWN:
				OutputDebugString(_T("- Unknown block\n"));
				return;
				break;

        	case BLOCK_TRAILER:
				OutputDebugString(_T("- Trailer block\n"));
				break;

        	case BLOCK_APPEXT:
				OutputDebugString(_T("- Application extension block\n"));
				break;

        	case BLOCK_COMMEXT:
				OutputDebugString(_T("- Comment extension block\n"));
				break;

        	case BLOCK_CONTROLEXT:
				{
					TGIFControlExt* pControl = reinterpret_cast<TGIFControlExt*>(&m_pRawData[m_nCurrOffset]);
					OutputDebugString(_T("- Graphic control extension block (delay %d, disposal %d)\n"),
					      pControl->m_wDelayTime,
					      pControl->GetPackedValue(GCX_PACKED_DISPOSAL));
				};
				break;

        	case BLOCK_PLAINTEXT:
				OutputDebugString(_T("- Plain text extension block\n"));
				break;

        	case BLOCK_IMAGE:
				TGIFImageDescriptor* pIDescr = reinterpret_cast<TGIFImageDescriptor*>(&m_pRawData[m_nCurrOffset]);
				OutputDebugString(_T("- Image data block (%dx%d  %d,%d)\n"),
				      pIDescr->m_wWidth,
				      pIDescr->m_wHeight,
				      pIDescr->m_wLeftPos,
				      pIDescr->m_wTopPos);
				break;
		};

		SkipNextBlock();
	};

	OutputDebugString(_T("\n"));
}
#endif // GIF_TRACING

BOOL CPictureEx::SetPaintRect(const RECT* lpRect)
{
	return CopyRect(&m_PaintRect, lpRect);
}

BOOL CPictureEx::GetPaintRect(RECT* lpRect)
{
	return CopyRect(lpRect, &m_PaintRect);
}

