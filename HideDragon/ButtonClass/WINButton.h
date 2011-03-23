// WINButton.h: interface for the CWINButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINBUTTON_H__9103A627_0574_40A5_B5B6_060572A121B3__INCLUDED_)
#define AFX_WINBUTTON_H__9103A627_0574_40A5_B5B6_060572A121B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define MAX_TEXTLEN 256

class  CWINButton  
{
public:
	//初始化按钮(这是第一步!)
	BOOL GetItemhWnd(HWND hWnd);
	//还原按钮区域设置
	BOOL Restore();
	//设置按钮是否可以拖动
	BOOL SetDrag(BOOL Enable);
	//设置按钮图标
	BOOL SetIcon(HICON icon);
	//设置按钮文字
	BOOL SetText(wchar_t *text, HFONT font);
	BOOL SetText(wchar_t *text);
	BOOL SetText(wchar_t *text, COLORREF color);
	//设置按钮有效区域
	BOOL SetupRegion(COLORREF TransColor);
	LRESULT OnPaint(HDC hdc);
	//设置按钮无效时的图片
	BOOL SetDisablePic(HBITMAP bmp);
	//设置按钮按下时的图片
	BOOL SetPressPic(HBITMAP bmp);
	//设置悬停按钮时的图片
	BOOL SetHoverPic(HBITMAP bmp);
	//设置按钮背景图片，第二个参数是是否根据图片调整按钮大小
	BOOL SetBackPic(HBITMAP bmp, BOOL bReSize);
	//设置按钮的提示消息
	BOOL SetToolTip(wchar_t *text);
	BOOL ClearToolTip();

	CWINButton();
	virtual ~CWINButton();

private:
	static LRESULT WINAPI stdProc(HWND hWnd,UINT uMsg,UINT wParam,LONG lParam);
	WNDPROC GetThunk();
	WNDPROC CreateThunk();
	LRESULT CALLBACK WINProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL DrawInsideBorder(HDC dc, RECT *rect);
	BOOL DrawFlat(HDC dc, RECT *rect);
	BOOL DrawDefault(HDC dc);

	HWND m_ToolTip;
	HWND m_hWnd;
	HWND m_hWndParent;
	LONG m_OldProc;
	WNDPROC m_thunk;
	TOOLINFO ti;
	HICON m_icon;
	HBITMAP m_Back;		//按钮背景图片
	HBITMAP m_Hove;		//鼠标悬停时按钮背景图片
	HBITMAP m_Press;	//鼠标按下时按钮背景图片
	HBITMAP m_Disable;  //按钮无效时背景图片
	BITMAP bm;
	COLORREF m_textcolor;	//按钮文字的颜色
	BOOL m_bMouseTracking;	//判断鼠标是否在窗口内
	BOOL m_bPress;	//判断鼠标是否按下
	BOOL m_Enable; //控件是否有效
	BOOL m_bFocus;	//按钮是否处于输入焦点
	BOOL m_bOwnerDraw; //判断是否用户自己贴图
	BOOL m_bDrag;		//是否处于拖动状态
	BOOL m_bDragEnable; //是否允许拖动
	wchar_t m_text[MAX_TEXTLEN];		//按钮文字
	wchar_t m_tiptext[MAX_TEXTLEN];  //按钮提示文字
	HFONT m_font;					//按钮文字字体
	HCURSOR m_OldCursor;
	RECT m_ParentRt;
	RECT m_BeginRt;
	RECT m_CurrentRt;
	POINT m_BeginPt;
	POINT m_CurrentPt;
	int m_CaptionHeight;
	int m_BorderWidth;
	int m_EdgeWidth;

protected:
	//按钮的外边框
	HPEN m_BoundryPen;
	
	//鼠标指针置于按钮之上时按钮的内边框
	HPEN m_InsideBoundryPenLeft;
	HPEN m_InsideBoundryPenRight;
	HPEN m_InsideBoundryPenTop;
	HPEN m_InsideBoundryPenBottom;
	
	//按钮获得焦点时按钮的内边框
	HPEN m_InsideBoundryPenLeftSel;
	HPEN m_InsideBoundryPenRightSel;
	HPEN m_InsideBoundryPenTopSel;
	HPEN m_InsideBoundryPenBottomSel;
	
	//按钮的底色，包括有效和无效两种状态
	HBRUSH m_FillActive;
	HBRUSH m_FillInactive;

};


#endif // !defined(AFX_WINBUTTON_H__9103A627_0574_40A5_B5B6_060572A121B3__INCLUDED_)
