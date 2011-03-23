// WINButton.cpp: implementation of the CWINButton class.
//
//////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT  0x0500

#include "../stdafx.h"
#include <commctrl.h>
#include <stdio.h>
#include "WINButton.h"
#include <winuser.h>


#pragma comment(lib, "comctl32.lib")

#pragma pack(push,1) //该结构必须以字节对齐
struct Thunk {
 BYTE    Call;
 int    Offset;
 WNDPROC   Proc;
 BYTE    Code[5];
 CWINButton*   Window;
 BYTE    Jmp;
 BYTE    ECX; 
};
#pragma pack(pop)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWINButton::CWINButton()
{
//	OutputDebugString("CWINButton");
	m_ToolTip = NULL;
	m_hWnd = NULL;
	m_hWndParent =NULL;
	wcscpy(m_text, L"\0");
	wcscpy(m_tiptext, L"\0");
	m_icon = NULL;
	m_Back = NULL;
	m_Hove = NULL;
	m_Disable = NULL;
	m_Press = NULL;
	m_bMouseTracking = FALSE;
	m_bPress = FALSE;
	m_Enable = TRUE;
	m_textcolor = RGB(0, 0, 0);
	m_font = NULL;
	m_bFocus = FALSE;
	m_bOwnerDraw = FALSE;
	m_bDrag = FALSE;
	m_bDragEnable = FALSE;
	m_CaptionHeight = 0;
	m_BorderWidth = 0;
	m_EdgeWidth = 0;
	m_BoundryPen = CreatePen(PS_INSIDEFRAME | PS_SOLID, 1, RGB(255, 255, 255));
	m_InsideBoundryPenLeft = CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(250, 196, 88)); 
	m_InsideBoundryPenRight = CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(251, 202, 106));
	m_InsideBoundryPenTop = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(252, 210, 121));
	m_InsideBoundryPenBottom = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(229, 151, 0));
	
	m_FillActive = CreateSolidBrush(RGB(223, 222, 236));
	m_FillInactive = CreateSolidBrush(RGB(222, 223, 236));
	
	m_InsideBoundryPenLeftSel = CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(153, 198, 252)); 
	m_InsideBoundryPenTopSel = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));
	m_InsideBoundryPenRightSel = CreatePen(PS_INSIDEFRAME | PS_SOLID, 3, RGB(162, 189, 252));
	m_InsideBoundryPenBottomSel = CreatePen(PS_INSIDEFRAME | PS_SOLID, 2, RGB(162, 201, 255));

}

CWINButton::~CWINButton()
{
//	OutputDebugString("~CWINButton");
	//重新赋值为以前的回调函数
	SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_OldProc);
	if(m_ToolTip)
	{
		DestroyWindow(m_ToolTip);
	}
	if(m_font)
		DeleteObject(m_font);
	DeleteObject(m_BoundryPen);
	DeleteObject(m_InsideBoundryPenLeft);
	DeleteObject(m_InsideBoundryPenRight);
	DeleteObject(m_InsideBoundryPenTop);
	DeleteObject(m_InsideBoundryPenBottom);
	DeleteObject(m_FillActive);
	DeleteObject(m_FillInactive);
	DeleteObject(m_InsideBoundryPenLeftSel);
	DeleteObject(m_InsideBoundryPenTopSel);
	DeleteObject(m_InsideBoundryPenRightSel);
	DeleteObject(m_InsideBoundryPenBottomSel);

}

BOOL CWINButton::GetItemhWnd(HWND hWnd)
{
	if(IsWindow(hWnd))
	{
		m_hWnd = hWnd;
		m_hWndParent = GetParent(m_hWnd);
		
		if(SetProp(m_hWnd, L"CWINBUTTON", (HANDLE)this) == 0)
		{
			OutputDebugString(L"SetProp ERROR");
			return FALSE;
		}
		if(m_hWndParent!=NULL)
		{
			if(GetWindowLong(m_hWndParent, GWL_STYLE) & WS_CAPTION)
			{
				m_CaptionHeight = GetSystemMetrics(SM_CYCAPTION);
			}
			if(GetWindowLong(m_hWndParent, GWL_STYLE) & WS_BORDER)
			{
				m_BorderWidth = GetSystemMetrics(SM_CXBORDER);
			}
			if(GetWindowLong(m_hWndParent, GWL_STYLE) & WS_THICKFRAME)
			{
				m_EdgeWidth = GetSystemMetrics(SM_CXEDGE);
			}
		}
//		if(SetProp(m_hWndParent, "CWINPARENT", (HANDLE)this) == 0)
//		{
//			OutputDebugString("SetProp ERROR");
//			return FALSE;
//		}
		SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | BS_OWNERDRAW);
		RECT rt;
		GetClientRect(m_hWnd, &rt);
		HRGN wndRgn = CreateRoundRectRgn(rt.left, rt.top, rt.right, rt.bottom, 5, 5);
		SetWindowRgn(m_hWnd, wndRgn, TRUE); 
		DeleteObject(wndRgn);
//		CreateThunk();
//		m_OldProc  = SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_thunk);
		m_OldProc = SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)stdProc);
//		m_OldParentProc = SetWindowLong(m_hWndParent,GWL_WNDPROC,(LONG)stdParentProc);
		return TRUE;
	}

	return FALSE;

}

LRESULT CALLBACK CWINButton::WINProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//	return CallWindowProc((WNDPROC)m_OldProc, m_hWnd, message, wParam, lParam);
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	HRGN rgn;
	static POINT oldpt = {0,0};
	int indexDC;
	//wchar_t info[256];
//	sprintf(info,"Msg is:%x", message);
//	OutputDebugString(info);
	switch(message)
    { 
		case WM_DRAWITEM:
			OutputDebugString(L"WM_DRAWITEM");
			break;

		case WM_CLOSE:
			OutputDebugString(L"WM_CHAR");
			break;
		case WM_CHAR: 
			OutputDebugString(L"WM_CHAR");
         break;
		case WM_SYSKEYDOWN:
//			sprintf(info,"wParam  is:%x Param is:%x", wParam, lParam);
//			OutputDebugString(info);
//			if((int)wParam == VK_ACCEPT)
//			{
//				OutputDebugString(L"Press Enter");
//			}
			OutputDebugString(L"WM_SYSKEYDOWN");
			break;
		case WM_KEYDOWN:
//			sprintf(info,"wParam  is:%x Param is:%x", wParam, lParam);
//			OutputDebugString(info);
//			if(lParam == VK_RETURN)
//			{
//				OutputDebugString(L"Press Enter");
//			}
//			OutputDebugString(L"WM_KEYDOWN");
			break;
//		case WM_NCPAINT:
		case WM_PAINT:
//			OutputDebugString("WM_PAINT");

			hdc = BeginPaint(m_hWnd, &ps);
			indexDC = SaveDC(hdc);
			rgn = CreateRectRgn(0, 0, 0, 0);
			GetWindowRgn(m_hWnd, rgn);
			if(SelectClipRgn(hdc, rgn) == ERROR)
			{
				OutputDebugString(L"SelectClipRgn ERROR");
			}
			OnPaint(hdc);
			RestoreDC(hdc, indexDC);
			EndPaint(m_hWnd, &ps);
			DeleteObject(rgn);
			//return TRUE;
			return 0;
			break;
		case WM_ERASEBKGND:
//			OutputDebugString("WM_ERASEBKGND");
//			GetClientRect(m_hWnd, &rt);
//			FillRect((HDC)wParam, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
//			SelectObject((HDC) wParam, wndRgn);
			return 1;
		case WM_MOUSEMOVE:
//			OutputDebugString("WM_MOUSEMOVE");
			if(m_bDrag && (m_hWndParent!=NULL) && m_bDragEnable)
			{
//				OutputDebugString("Move Item");
//				sprintf(info, "Begin x:%d, y:%d", m_BeginPt.x, m_BeginPt.y);
//				OutputDebugString(info);
//				sprintf(info, "Current x:%d, y:%d", m_CurrentPt.x, m_CurrentPt.y);
//				OutputDebugString(info);
//				sprintf(info, "Current left:%d, top:%d",
//				m_CurrentRt.left, m_CurrentRt.top);
//				OutputDebugString(info);
//				m_OldCursor = SetCursor(LoadCursor(NULL, IDC_CROSS));
				GetCursorPos(&m_CurrentPt);
				GetWindowRect(m_hWndParent, &m_ParentRt);
				m_CurrentRt.left = m_BeginRt.left + m_CurrentPt.x - m_BeginPt.x - m_ParentRt.left - \
							m_EdgeWidth-m_BorderWidth -1;
				m_CurrentRt.top = m_BeginRt.top + m_CurrentPt.y - m_BeginPt.y - \
									m_ParentRt.top-m_CaptionHeight-\
									m_BorderWidth -1; 
				
				MoveWindow(m_hWnd, m_CurrentRt.left, m_CurrentRt.top,
							m_BeginRt.right-m_BeginRt.left, 
							m_BeginRt.bottom-m_BeginRt.top, TRUE); 

			}
			if(m_ToolTip)
			{
				MSG msg; 
                msg.lParam = lParam; 
                msg.wParam = wParam; 
                msg.message = message; 
                msg.hwnd = m_hWnd; 
				SendMessage(m_ToolTip, TTM_RELAYEVENT, 0, 
                    (LPARAM) (LPMSG) &msg); 

			}
			if(!m_bMouseTracking)
			{
				m_bMouseTracking = TRUE;
//				UpdateWindow(m_hWnd);
				GetClientRect(m_hWnd, &rt);
				InvalidateRect(m_hWnd, &rt, TRUE);
//				InvalidateRgn(m_hWnd, wndRgn, TRUE);
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				//监控鼠标离开
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = m_hWnd;
				TrackMouseEvent(&tme);
			}
			break;

		case WM_SETFOCUS:
//			OutputDebugString("WM_SETFOCUS");
//			m_bFocus = TRUE;
//			m_OldCursor = SetCursor(LoadCursor(NULL, IDC_CROSS));
//			GetClientRect(m_hWnd, &rt);
//			InvalidateRect(m_hWnd, &rt, TRUE);
			break;
//			return TRUE;
		case WM_MOUSELEAVE:
//			OutputDebugString("WM_MOUSELEAVE");
			m_bMouseTracking = FALSE;
			m_bDrag = FALSE;
//			SetCursor(m_OldCursor);
//			UpdateWindow(m_hWnd);
			GetClientRect(m_hWnd, &rt);
			InvalidateRect(m_hWnd, &rt, TRUE);
//			InvalidateRgn(m_hWnd, wndRgn, TRUE);
			break;
		case WM_KILLFOCUS:
//			OutputDebugString("WM_KILLFOCUS");
//			m_bFocus = FALSE;
//			GetClientRect(m_hWnd, &rt);
//			InvalidateRect(m_hWnd, &rt, TRUE);
			break;			
//		case WM_NCHITTEST:
//			if(m_bDrag)
//				return HTCAPTION;
//			OutputDebugString("WM_NCHITTEST");
////			GetClientRect(m_hWnd, &rt);
////			InvalidateRect(m_hWnd, &rt, TRUE);
//			break;
		case WM_RBUTTONDOWN:
			m_bDrag = TRUE;
//			m_OldCursor = SetCursor(LoadCursor(NULL, IDC_CROSS));
			GetCursorPos(&m_BeginPt);
			GetWindowRect(m_hWnd, &m_BeginRt);
			GetWindowRect(m_hWnd, &m_CurrentRt);
//			sprintf(info, "Current left:%d, top:%d", m_CurrentRt.left, m_CurrentRt.top);
//			OutputDebugString(info);

			break;
		case WM_RBUTTONUP:
			m_bDrag = FALSE;
//			SetCursor(m_OldCursor);
			break;
		case WM_LBUTTONDOWN:
//			OutputDebugString("WM_LBUTTONDOWN");
			m_bPress = TRUE;
			oldpt.x = LOWORD(lParam);
			oldpt.y = HIWORD(lParam);
//			UpdateWindow(m_hWnd);
			GetClientRect(m_hWnd, &rt);
			InvalidateRect(m_hWnd, &rt, TRUE);
//			InvalidateRgn(m_hWnd, wndRgn, TRUE);
			break;
		case WM_LBUTTONUP:
//			OutputDebugString("WM_LBUTTONUP");
			m_bPress = FALSE;
//			UpdateWindow(m_hWnd);
			GetClientRect(m_hWnd, &rt);
			InvalidateRect(m_hWnd, &rt, TRUE);
//			InvalidateRgn(m_hWnd, wndRgn, TRUE);
			break;
		case WM_ENABLE:
//			OutputDebugString("WM_ENABLE");
			m_Enable = (BOOL) wParam;
//			UpdateWindow(m_hWnd);
			GetClientRect(m_hWnd, &rt);
			InvalidateRect(m_hWnd, &rt, TRUE);
//			InvalidateRgn(m_hWnd, wndRgn, TRUE);
			break;

	}

	return CallWindowProc((WNDPROC)m_OldProc, m_hWnd, message, wParam, lParam); 
}

WNDPROC CWINButton::CreateThunk()
{
	Thunk*  thunk = new Thunk;

	///////////////////////////////////////////////
	//
	//系统调用m_thunk时的堆栈：
	//ret HWND MSG WPARAM LPARAM
	//-------------------------------------------
	//栈顶                                             栈底
	///////////////////////////////////////////////

	//call Offset
	//调用code[0]，call执行时会把下一条指令压栈，即把Proc压栈
	thunk->Call = 0xE8;        // call [rel]32
	thunk->Offset = (size_t)&(((Thunk*)0)->Code)-(size_t)&(((Thunk*)0)->Proc);  // 偏移量，跳过Proc到Code[0]
	thunk->Proc = CWINButton::stdProc;  //静态窗口过程

	//pop ecx，Proc已压栈，弹出Proc到ecx 
	thunk->Code[0] = 0x59;  //pop ecx

	//mov dword ptr [esp+0x4],this
	//Proc已弹出，栈顶是返回地址，紧接着就是HWND了。
	//[esp+0x4]就是HWND
	thunk->Code[1] = 0xC7;  // mov
	thunk->Code[2] = 0x44;  // dword ptr
	thunk->Code[3] = 0x24;  // disp8[esp]
	thunk->Code[4] = 0x04;  // +4
	thunk->Window = this;

	//偷梁换柱成功！跳转到Proc
	//jmp [ecx]
	thunk->Jmp = 0xFF;     // jmp [r/m]32
	thunk->ECX = 0x21;     // [ecx]

	m_thunk = (WNDPROC)thunk;
	return m_thunk;


}

WNDPROC CWINButton::GetThunk()
{
	return m_thunk;
}

LRESULT WINAPI CWINButton::stdProc(HWND hWnd, UINT uMsg, UINT wParam, LONG lParam)
{
//	CWINButton* w = (CWINButton*)hWnd; 
	CWINButton* w = (CWINButton*)GetProp(hWnd, L"CWINBUTTON");
	return w->WINProc(uMsg,wParam,lParam);
}

BOOL CWINButton::SetText(wchar_t *text, COLORREF color)
{
	if(text!=NULL)
	{
		wcscpy(m_text, text);
		m_textcolor = color;
		return TRUE;
	}
	return FALSE;
}

BOOL CWINButton::SetBackPic(HBITMAP bmp, BOOL bReSize)
{
	DWORD size = 0;
	RECT rt;
	if(m_Enable == FALSE)
		return FALSE;
	if(bmp != NULL)
	{
		m_Back = bmp;
		if(bReSize)
		{
//			int nCaption = GetSystemMetrics(SM_CYCAPTION); // 系统Title高度
//			int nXEdge = GetSystemMetrics(SM_CXEDGE);  
//			int nYEdge = GetSystemMetrics(SM_CYEDGE);
			GetObject(m_Back, sizeof(bm), &bm);
			GetWindowRect(m_hWnd, &rt);
			SetWindowPos(m_hWnd, 0, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE|SWP_NOREPOSITION);
		}
		GetClientRect(m_hWnd, &rt);
		HRGN wndRgn = CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);
		SetWindowRgn(m_hWnd, wndRgn, TRUE); 
		DeleteObject(wndRgn);
		InvalidateRect(m_hWnd, &rt, TRUE);
		m_bOwnerDraw = TRUE;
		return TRUE;
	}
	return FALSE;
}

BOOL CWINButton::SetHoverPic(HBITMAP bmp)
{
	if(bmp != NULL)
	{
		m_Hove = bmp;
		return TRUE;
	}
	return FALSE;
}

BOOL CWINButton::SetPressPic(HBITMAP bmp)
{
	if(bmp != NULL)
	{
		m_Press = bmp;
		return TRUE;
	}
	return FALSE;

}

BOOL CWINButton::SetDisablePic(HBITMAP bmp)
{
	if(bmp != NULL)
	{
		m_Disable = bmp;
		return TRUE;
	}
	return FALSE;
}

LRESULT CWINButton::OnPaint(HDC hdc)
{
	HBITMAP bmp,hOrgBmp = NULL,hOrgMemBmp = NULL; 
	int width, height;
	RECT rt;
	HDC hdcMem, hdcBMP;
	HFONT prefont = NULL;
	wchar_t text[MAX_TEXTLEN];
	SetBkMode(hdc, TRANSPARENT);
	// TODO: Add any drawing code here...
	GetClientRect(m_hWnd, &rt);
	width = rt.right-rt.left;
	height = rt.bottom-rt.top;
	hdcMem = CreateCompatibleDC(hdc);
	if(hdcMem == NULL)
	{
		OutputDebugString(L"Create MemDc ERROR");
	}
	hdcBMP = CreateCompatibleDC(hdc);
	if(hdcBMP == NULL)
	{
		OutputDebugString(L"Create MemDc ERROR");
	}

	int indexMemDC = SaveDC(hdcMem);

	SetBkMode(hdcMem, TRANSPARENT);

	bmp = CreateCompatibleBitmap(hdc, width, height);
	hOrgMemBmp = (HBITMAP)SelectObject(hdcMem, bmp);

	if(!m_bOwnerDraw)
	{
		DrawDefault(hdcMem);
	}
	else
	{
		//控件有效
		if(m_Enable)
		{
			//填充跟系统背景色一样的颜色
			if(m_Back != NULL)
			{
				hOrgBmp = (HBITMAP)SelectObject(hdcBMP, m_Back);
			}
			
			//鼠标悬停
			if(m_bMouseTracking || m_bFocus)
			{
				if(m_Hove != NULL)
				{
					hOrgBmp = (HBITMAP)SelectObject(hdcBMP, m_Hove);
				}
			}
			//鼠标按下
			if(m_bPress)
			{
				if(m_Press)
				{
					hOrgBmp = (HBITMAP)SelectObject(hdcBMP, m_Press);
				}
			}
			
			BitBlt(hdcMem, 0, 0, width, height, hdcBMP, 0, 0, SRCCOPY);
			
//			if(hOrgBmp != NULL)
//			{
//				SelectObject(hdcBMP, hOrgBmp);
//			}
			DeleteDC(hdcBMP);
			//写文字
			if(GetWindowText(m_hWnd, text, MAX_TEXTLEN) != 0)
			{
				wcscpy(m_text, text);
			}
			if(wcslen(m_text) != 0)
			{
		//		OutputDebugString(m_text);
				COLORREF precolor = SetTextColor(hdcMem, m_textcolor);
				if(m_font != NULL)
				{
					prefont = (HFONT)SelectObject(hdcMem, m_font);
				}
				DrawText(hdcMem, m_text, -1, &rt, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				SetTextColor(hdcMem, precolor);
				if(prefont != NULL)
				{
					SelectObject(hdcMem, prefont);
				}
			}
		}
		//控件无效的状态
		else
		{
			if((m_Disable!=NULL))
			{
				SelectObject(hdcBMP, m_Disable);
			}
			BitBlt(hdcMem, 0, 0, width, height, hdcBMP, 0, 0, SRCCOPY);
			DeleteDC(hdcBMP);
			if(GetWindowText(m_hWnd, text, MAX_TEXTLEN) != 0)
			{
				wcscpy(m_text, text);
			}
			if(wcslen(m_text) != 0)
			{
				COLORREF precolor = SetTextColor(hdcMem, GetSysColor(COLOR_GRAYTEXT));
				if(m_font != NULL)
				{
					prefont = (HFONT)SelectObject(hdcMem, m_font);
				}

				DrawText(hdcMem, m_text, -1, &rt, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				SetTextColor(hdcMem, precolor);
				SelectObject(hdcMem, prefont);
			}	
		}
	
	}


	BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
	
	RestoreDC(hdcMem, indexMemDC);
	SelectObject(hdcMem, hOrgMemBmp);
	if(DeleteDC(hdcMem) ==FALSE)
	{
		OutputDebugString(L"DeleteDC(hdcMem) ERROR");
	}
	if(DeleteObject(bmp) == FALSE)
	{
		OutputDebugString(L"DeleteObject(bmp) ERROR");
	}
	return TRUE;

}

BOOL CWINButton::SetText(wchar_t *text)
{
	if(text!=NULL)
	{
		wcscpy(m_text, text);
		m_textcolor = RGB(0, 0, 0);
		return TRUE;
	}
	return FALSE;
}

BOOL CWINButton::SetupRegion(COLORREF TransColor)
{
	int bRet;
	HDC memDC;
	//创建与传入DC兼容的临时DC

	if(m_Back==NULL)
	{
		return FALSE;
	}

	SetBackPic(m_Back, TRUE);
	HDC dc = GetDC(m_hWnd);
	SetBkMode(dc, TRANSPARENT);

	memDC = CreateCompatibleDC(dc);
	SetBkMode(memDC, TRANSPARENT);
	HRGN wndRgn = CreateRectRgn(0, 0, 0, 0);

	//将位图选入临时DC
	SelectObject(memDC, m_Back);
	BITMAP bit; 
	GetObject(m_Back, sizeof(bit), &bit);
	//创建总的窗体区域，初始region为0
	if(wndRgn ==NULL)
	{
		OutputDebugString(L"CreateRectRgn wndRgn ERROR");
	}

	int y;
	for(y=0;y<=bit.bmHeight ;y++)
	{
		HRGN rgnTemp; //保存临时region
		int iX = 0;
		do
		{
			//跳过透明色找到下一个非透明色的点.
			while (iX <= bit.bmWidth && GetPixel(memDC, iX, y) == TransColor)
			iX++;
			//记住这个起始点
			int iLeftX = iX;
			//寻找下个透明色的点
			while (iX <= bit.bmWidth && GetPixel(memDC, iX, y) != TransColor)
			++iX;
			//创建一个包含起点与重点间高为1像素的临时“region”
			rgnTemp = CreateRectRgn(iLeftX, y, iX, y+1);
			if(rgnTemp == NULL)
			{
				OutputDebugString(L"CreateRectRgn temp ERROR");
			}
			//合并到主"region".
			bRet = CombineRgn(wndRgn, wndRgn, rgnTemp, RGN_OR);
			if(bRet == ERROR)
			{
				OutputDebugString(L"CombineRgn ERROR");
			}
			//删除临时"region",否则下次创建时和出错
			DeleteObject(rgnTemp);
		}
		while(iX<= bit.bmWidth);
	}
	
	bRet = SetWindowRgn(m_hWnd, wndRgn, TRUE); 
	DeleteObject(wndRgn);
//	SelectObject(dc, wndRgn);
	DeleteDC(memDC);
	ReleaseDC(m_hWnd, dc);
	return TRUE;
}

BOOL CWINButton::SetText(wchar_t *text, HFONT font)
{
	if(text!=NULL)
	{
		wcscpy(m_text, text);
		m_font = font;
		return TRUE;
	}
	return FALSE;

}

BOOL CWINButton::DrawDefault(HDC dc)
{
	HPEN hOldPen;
	HBRUSH hOldBrush;
	RECT rt;
	RECT rect;
	GetClientRect(m_hWnd, &rt);
	
	int x = GetSystemMetrics(SM_CXEDGE);
	int y = GetSystemMetrics(SM_CYEDGE);
	//画按钮的外边框，它是一个半径为5的圆角矩形
	hOldPen = (HPEN)SelectObject(dc, m_BoundryPen);
	RoundRect(dc, rt.left, rt.top, rt.right, rt.bottom, 5, 5);	
	SelectObject(dc, hOldPen);
	
	SetRect(&rect, rt.left+x, rt.top+y, rt.right-x, rt.bottom-y);
	
	//根据按钮的状态填充按钮的底色
	if (m_bMouseTracking)
	{
		hOldBrush = (HBRUSH)SelectObject(dc, m_FillActive);
		DrawFlat(dc, &rect);
	}
	else
	{
		hOldBrush = (HBRUSH)SelectObject(dc, m_FillInactive);
		DrawFlat(dc, &rect);
	}
	
	//根据按钮的状态绘制内边框
	if (m_bMouseTracking || m_bFocus)
		DrawInsideBorder(dc, &rect);
				
	SelectObject(dc, hOldBrush);
	
	if(m_icon)
	{
		ICONINFO ico;
		GetIconInfo(m_icon, &ico);
		if(ico.fIcon)
		{
			if(!m_bPress)
				DrawIcon(dc, rt.left+4, rt.bottom /2 - ico.yHotspot, m_icon);
			else
				DrawIcon(dc, rt.left+5, rt.bottom /2 - ico.yHotspot + 1, m_icon);	
		}
	}
	//显示按钮的文本
	TCHAR text[MAX_TEXTLEN+1];
	if(GetWindowText(m_hWnd, text, MAX_TEXTLEN) != 0)
	{
		wcscpy(m_text, text);
	}
	if(wcslen(m_text) != 0)
	{
//		OutputDebugString(m_text);
		HFONT prefont;
		COLORREF precolor;
		if(m_Enable)
		{
			precolor = SetTextColor(dc, m_textcolor);
		}
		else
		{
			precolor = SetTextColor(dc, GetSysColor(COLOR_GRAYTEXT));
		}
		if(m_font != NULL)
		{
			prefont = (HFONT)SelectObject(dc, m_font);
		}
		if(m_bPress)
		{
			SetRect(&rect, rt.left+2, rt.top+2, rt.right, rt.bottom);
		}
		DrawText(dc, m_text, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		SetTextColor(dc, precolor);
		SelectObject(dc, prefont);
	}
	
	return TRUE;
}

BOOL CWINButton::DrawFlat(HDC dc, RECT *rect)
{
	HBRUSH brBk[64];
	int nWidth = rect->right - rect->left;	
	int nHeight = rect->bottom - rect->top;
	RECT rct;
	
	for (int i = 0; i < 64; i ++)
	{
		if (m_bMouseTracking)
		{
			if (m_bFocus)
				brBk[i] = CreateSolidBrush(RGB(255 - (i / 4), 255 - (i / 4), 255 - (i / 3)));
			else
				brBk[i] = CreateSolidBrush(RGB(255 - (i / 4), 255 - (i / 4), 255 - (i / 5)));
		}
		else
		{
			if (m_bFocus)
				brBk[i] = CreateSolidBrush(RGB(255 - (i / 3), 255 - (i / 3), 255 - (i / 4)));
			else
				brBk[i] = CreateSolidBrush(RGB(255 - (i / 3), 255 - (i / 3), 255 - (i / 5)));
		}
	}
	
	for (i = rect->top; i <= nHeight + 2; i ++) 
	{
		SetRect(&rct, rect->left, i, nWidth + 2, i + 1);
		FillRect(dc, &rct, brBk[((i * 63) / nHeight)]);
	}
	
	for (i = 0; i < 64; i ++)
		DeleteObject(brBk[i]);
	return TRUE;
}

BOOL CWINButton::DrawInsideBorder(HDC dc, RECT *rect)
{
	HPEN pLeft, pRight, pTop, pBottom;
	
	if (m_bFocus && !m_bMouseTracking)
	{
		pLeft = m_InsideBoundryPenLeftSel;
		pRight = m_InsideBoundryPenRightSel;
		pTop = m_InsideBoundryPenTopSel;
		pBottom = m_InsideBoundryPenBottomSel;
	}
	else
	{
		pLeft = m_InsideBoundryPenLeft;
		pRight = m_InsideBoundryPenRight;
		pTop = m_InsideBoundryPenTop;
		pBottom = m_InsideBoundryPenBottom;
	}
	
	POINT oldPoint;
	MoveToEx(dc, rect->left, rect->bottom - 1, &oldPoint);
	HPEN pOldPen = (HPEN)SelectObject(dc, pLeft);
	LineTo(dc, rect->left, rect->top + 1);
	SelectObject(dc, pRight);
	MoveToEx(dc, rect->right - 1, rect->bottom - 1, NULL);
	LineTo(dc, rect->right - 1, rect->top);
	SelectObject(dc, pTop);
	MoveToEx(dc, rect->left - 1, rect->top, NULL);
	LineTo(dc, rect->right - 1, rect->top);
	SelectObject(dc, pBottom);
	MoveToEx(dc, rect->left, rect->bottom, NULL);
	LineTo(dc, rect->right - 1, rect->bottom);
	SelectObject(dc, pOldPen);
	MoveToEx(dc, oldPoint.x, oldPoint.y, NULL);

	if (m_bFocus && !m_bMouseTracking)
		DrawFocusRect(dc, rect);
	return TRUE;
}

BOOL CWINButton::SetIcon(HICON icon)
{
	if(icon != NULL)
	{
		m_icon = icon;
		return TRUE;
	}
	return FALSE;
}

BOOL CWINButton::SetDrag(BOOL Enable)
{
	m_bDragEnable = Enable;
	return TRUE;
}

BOOL CWINButton::Restore()
{
	if(m_hWnd==NULL)
		return FALSE;
	RECT rt;
	GetClientRect(m_hWnd, &rt);
	HRGN rgn = CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);
	SetWindowRgn(m_hWnd, rgn, TRUE);
	DeleteObject(rgn);
	return TRUE;
}

BOOL CWINButton::SetToolTip(wchar_t *text)
{
	if(!m_ToolTip)
	{
		InitCommonControls(); 

		m_ToolTip = CreateWindow(TOOLTIPS_CLASS, L"", TTS_ALWAYSTIP, 
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
					NULL, (HMENU) NULL, NULL, NULL);
		if(!m_ToolTip)
			return FALSE;
	}
	if(text!=NULL)
		wcscpy(m_tiptext, text);

	ti.cbSize = sizeof(TOOLINFO); 
	ti.uFlags = TTF_IDISHWND; 
	ti.hwnd = m_hWnd; 
	ti.hinst = NULL; 
	ti.uId = (UINT) m_hWnd; 
//	int i = SendMessage(m_ToolTip, TTM_GETTOOLCOUNT, 0, 0);
	if(SendMessage(m_ToolTip, TTM_GETTOOLCOUNT, 0, 0) > 0)
	{
		SendMessage(m_ToolTip, TTM_DELTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
		OutputDebugString(L"Set ToolTip Already");
	}
	ti.lpszText = m_tiptext;
	if (!SendMessage(m_ToolTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti)) 
		return FALSE; 
	
	return TRUE;
}



BOOL CWINButton::ClearToolTip()
{
	if(!m_ToolTip)
	{
		return FALSE;
	}


	ti.cbSize = sizeof(TOOLINFO); 
	ti.uFlags = TTF_IDISHWND; 
	ti.hwnd = m_hWnd; 
	ti.hinst = NULL; 
	ti.uId = (UINT) m_hWnd; 

	SendMessage(m_ToolTip, TTM_DELTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	OutputDebugString(L"Clear ToolTip Already");

	
	return TRUE;
}

