#if !defined(CPICTUREEX__INCLUDED_)
#define CPICTUREEX__INCLUDED_

#include "ocidl.h"
#include <vector>
#include <process.h> 
#include "assert.h"
#include "Winbase.h"
#include "olectl.h"

class CPictureEx 
{
public:

	class WndProcThunk
	{
	public:

		#pragma pack(push,1)
		struct _WndProcThunk
		{
			DWORD   m_mov;          
			DWORD   m_this;
			BYTE    m_jmp;          
			DWORD   m_relproc;     
		};
		#pragma pack(pop)
		
		_WndProcThunk thunk;

		void Init(WNDPROC proc, void* pThis)
		{
			thunk.m_mov = 0x042444C7;  
			thunk.m_this = (DWORD)pThis;
			thunk.m_jmp = 0xe9;
			thunk.m_relproc = (int)proc - ((int)this+sizeof(_WndProcThunk));

			::FlushInstructionCache(GetCurrentProcess(), &thunk, sizeof(thunk));
		}
	};
    struct TFrame    // structure that keeps a single frame info
	{
		IPicture*	m_pPicture;  // pointer to the interface used for drawing
		SIZE		m_frameSize;
		SIZE		m_frameOffset;
		UINT		m_nDelay;     // delay (in 1/100s of a second)
		UINT		m_nDisposal;  // disposal method
	};

#pragma pack(1)   // turn byte alignment on

	enum GIFBlockTypes
	{
		BLOCK_UNKNOWN,
		BLOCK_APPEXT,
		BLOCK_COMMEXT,
		BLOCK_CONTROLEXT,
		BLOCK_PLAINTEXT,
		BLOCK_IMAGE,
		BLOCK_TRAILER
	};

	enum ControlExtValues // graphic control extension packed field values

	{
		GCX_PACKED_DISPOSAL,  // disposal method
		GCX_PACKED_USERINPUT,
		GCX_PACKED_TRANSPCOLOR
	};

	enum LSDPackedValues  // logical screen descriptor packed field values

	{
		LSD_PACKED_GLOBALCT,
		LSD_PACKED_CRESOLUTION,
		LSD_PACKED_SORT,
		LSD_PACKED_GLOBALCTSIZE
	};

	enum IDPackedValues   // image descriptor packed field values

	{
		ID_PACKED_LOCALCT,
		ID_PACKED_INTERLACE,
		ID_PACKED_SORT,
		ID_PACKED_LOCALCTSIZE
	};

	struct TGIFHeader     // GIF header
	{
		char	m_cSignature[3]; // Signature - Identifies the GIF Data Stream
		// This field contains the fixed value 'GIF'
		char	m_cVersion[3]; // Version number. May be one of the following:
		// "87a" or "89a"
	};

	struct TGIFLSDescriptor // Logical Screen Descriptor
	{
		WORD			m_wWidth;  // 2 bytes. Logical screen width
		WORD			m_wHeight; // 2 bytes. Logical screen height

		unsigned char	m_cPacked;	// packed field 

		unsigned char	m_cBkIndex;     // 1 byte. Background color index
		unsigned char	m_cPixelAspect; // 1 byte. Pixel aspect ratio
		inline int		GetPackedValue(enum LSDPackedValues Value);
	};

	struct TGIFAppExtension // application extension block
	{
		unsigned char	m_cExtIntroducer; // extension introducer (0x21)
		unsigned char	m_cExtLabel; // app. extension label (0xFF)
		unsigned char	m_cBlockSize; // fixed value of 11
		char			m_cAppIdentifier[8];   // application identifier
		char			m_cAppAuth[3];  // application authentication code
	};

	struct TGIFControlExt // graphic control extension block
	{
		unsigned char	m_cExtIntroducer; // extension introducer (0x21)
		unsigned char	m_cControlLabel;  // control extension label (0xF9)
		unsigned char	m_cBlockSize; // fixed value of 4
		unsigned char	m_cPacked;  // packed field
		WORD			m_wDelayTime;  // delay time
		unsigned char	m_cTColorIndex; // transparent color index
		unsigned char	m_cBlockTerm;   // block terminator (0x00)
    public:
		inline int		GetPackedValue(enum ControlExtValues Value);
	};

	struct TGIFCommentExt  // comment extension block
	{
		unsigned char	m_cExtIntroducer; // extension introducer (0x21)
		unsigned char	m_cCommentLabel;  // comment extension label (0xFE)
	};

	struct TGIFPlainTextExt // plain text extension block
	{
		unsigned char	m_cExtIntroducer;  // extension introducer (0x21)
		unsigned char	m_cPlainTextLabel; // text extension label (0x01)
		unsigned char	m_cBlockSize; // fixed value of 12
		WORD			m_wLeftPos;  // text grid left position
		WORD			m_wTopPos;   // text grid top position
		WORD			m_wGridWidth;  // text grid width
		WORD			m_wGridHeight; // text grid height
		unsigned char	m_cCellWidth;  // character cell width
		unsigned char	m_cCellHeight; // character cell height
		unsigned char	m_cFgColor; // text foreground color index
		unsigned char	m_cBkColor; // text background color index
	};

	struct TGIFImageDescriptor // image descriptor block
	{
		unsigned char		m_cImageSeparator; // image separator byte (0x2C)
		WORD				m_wLeftPos; // image left position
		WORD				m_wTopPos;  // image top position
		WORD				m_wWidth;   // image width
		WORD				m_wHeight;  // image height
		unsigned char		m_cPacked; // packed field
		inline int			GetPackedValue(enum IDPackedValues Value);
	};

#pragma pack() // turn byte alignment off

public:

	//add by Jason
	HWND m_hWnd;
	WndProcThunk	m_thunk;
	BOOL CPictureEx::Attach(HWND hWnd);

    //////////////////////////////////////////////////////////////////////////

	BOOL				GetPaintRect(RECT* lpRect);
	BOOL				SetPaintRect(const RECT* lpRect);
						CPictureEx();
	virtual				~CPictureEx();
	void				Stop();   // stops animation
	void				UnLoad(); // stops animation plus releases all resources

	BOOL				IsGIF() const;
	BOOL				IsPlaying() const;
	BOOL				IsAnimatedGIF() const;
	SIZE				GetSize() const;
	int					GetFrameCount() const;
	COLORREF			GetBkColor() const;
	void				SetBkColor(COLORREF clr);

	// draws the picture (starts an animation thread if needed)
	// if an animation was previously stopped by Stop(),
	// continues it from the last displayed frame
	BOOL				Draw();

	BOOL                Invalidate();

	// loads a picture from a file
	// i.e. Load(_T("mypic.gif"));
	BOOL				Load(LPCTSTR szFileName);

	// loads a picture from a global memory block (allocated by GlobalAlloc)
	// Warning: this function DOES NOT free the global memory, pointed to by hGlobal
	BOOL				Load(HGLOBAL hGlobal, DWORD dwSize);

	// loads a picture from a program resource
	// i.e. Load(MAKEINTRESOURCE(IDR_MYPIC),_T("GIFTYPE"));
    BOOL                Load(HINSTANCE hinst,LPCTSTR szResourceName, LPCTSTR szResourceType);
protected:

#ifdef GIF_TRACING
	void				EnumGIFBlocks();
	void				WriteDataOnDisk(CString szFileName, HGLOBAL hData, DWORD dwSize);
#endif // GIF_TRACING

	RECT				m_PaintRect;
	SIZE				m_PictureSize;
	COLORREF			m_clrBackground;
	UINT				m_nCurrFrame;
	UINT				m_nDataSize;
	UINT				m_nCurrOffset;
	UINT				m_nGlobalCTSize;
	BOOL				m_bIsGIF;
	BOOL				m_bIsPlaying;
	volatile BOOL		m_bExitThread;
	BOOL				m_bIsInitialized;
	HDC					m_hMemDC;

	HDC					m_hDispMemDC;
	HBITMAP				m_hDispMemBM;
	HBITMAP				m_hDispOldBM;

	HBITMAP				m_hBitmap;
	HBITMAP				m_hOldBitmap;
	HANDLE				m_hThread;
	HANDLE				m_hExitEvent;
	IPicture*			m_pPicture;
	TGIFHeader*			m_pGIFHeader;
	unsigned char*		m_pRawData;
	TGIFLSDescriptor*	m_pGIFLSDescriptor;
	std::vector<TFrame>	m_arrFrames;

	void				ThreadAnimation();
	static unsigned __stdcall  _ThreadAnimation(LPVOID pParam);
	
	//add by jason////////////////////////////////////////////////////////////
	static LRESULT CALLBACK WindowProc(   HWND  hwnd,
										  UINT uMsg, 
										  WPARAM wParam, 
										  LPARAM lParam 
										);
    static WNDPROC      m_pOldWindowProc;

    //////////////////////////////////////////////////////////////////////////
    
	int					GetNextBlockLen() const;
	BOOL				SkipNextBlock();
	BOOL				SkipNextGraphicBlock();
	BOOL				PrepareDC(int nWidth, int nHeight);
	void				ResetDataPointer();
	enum GIFBlockTypes	GetNextBlock() const;
	UINT				GetSubBlocksLen(UINT nStartingOffset) const;
	HGLOBAL				GetNextGraphicBlock(UINT* pBlockLen,
	                                        UINT* pDelay,
	                                        SIZE* pBlockSize,
	                                        SIZE* pBlockOffset,
	                                        UINT* pDisposal);

	

	//add by jason
	void OnDestroy();
	void OnPaint();
	//////////////////////////////////////////////////////////////////////////
	

};

#endif
