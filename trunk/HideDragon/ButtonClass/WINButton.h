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
	//��ʼ����ť(���ǵ�һ��!)
	BOOL GetItemhWnd(HWND hWnd);
	//��ԭ��ť��������
	BOOL Restore();
	//���ð�ť�Ƿ�����϶�
	BOOL SetDrag(BOOL Enable);
	//���ð�ťͼ��
	BOOL SetIcon(HICON icon);
	//���ð�ť����
	BOOL SetText(wchar_t *text, HFONT font);
	BOOL SetText(wchar_t *text);
	BOOL SetText(wchar_t *text, COLORREF color);
	//���ð�ť��Ч����
	BOOL SetupRegion(COLORREF TransColor);
	LRESULT OnPaint(HDC hdc);
	//���ð�ť��Чʱ��ͼƬ
	BOOL SetDisablePic(HBITMAP bmp);
	//���ð�ť����ʱ��ͼƬ
	BOOL SetPressPic(HBITMAP bmp);
	//������ͣ��ťʱ��ͼƬ
	BOOL SetHoverPic(HBITMAP bmp);
	//���ð�ť����ͼƬ���ڶ����������Ƿ����ͼƬ������ť��С
	BOOL SetBackPic(HBITMAP bmp, BOOL bReSize);
	//���ð�ť����ʾ��Ϣ
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
	HBITMAP m_Back;		//��ť����ͼƬ
	HBITMAP m_Hove;		//�����ͣʱ��ť����ͼƬ
	HBITMAP m_Press;	//��갴��ʱ��ť����ͼƬ
	HBITMAP m_Disable;  //��ť��Чʱ����ͼƬ
	BITMAP bm;
	COLORREF m_textcolor;	//��ť���ֵ���ɫ
	BOOL m_bMouseTracking;	//�ж�����Ƿ��ڴ�����
	BOOL m_bPress;	//�ж�����Ƿ���
	BOOL m_Enable; //�ؼ��Ƿ���Ч
	BOOL m_bFocus;	//��ť�Ƿ������뽹��
	BOOL m_bOwnerDraw; //�ж��Ƿ��û��Լ���ͼ
	BOOL m_bDrag;		//�Ƿ����϶�״̬
	BOOL m_bDragEnable; //�Ƿ������϶�
	wchar_t m_text[MAX_TEXTLEN];		//��ť����
	wchar_t m_tiptext[MAX_TEXTLEN];  //��ť��ʾ����
	HFONT m_font;					//��ť��������
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
	//��ť����߿�
	HPEN m_BoundryPen;
	
	//���ָ�����ڰ�ť֮��ʱ��ť���ڱ߿�
	HPEN m_InsideBoundryPenLeft;
	HPEN m_InsideBoundryPenRight;
	HPEN m_InsideBoundryPenTop;
	HPEN m_InsideBoundryPenBottom;
	
	//��ť��ý���ʱ��ť���ڱ߿�
	HPEN m_InsideBoundryPenLeftSel;
	HPEN m_InsideBoundryPenRightSel;
	HPEN m_InsideBoundryPenTopSel;
	HPEN m_InsideBoundryPenBottomSel;
	
	//��ť�ĵ�ɫ��������Ч����Ч����״̬
	HBRUSH m_FillActive;
	HBRUSH m_FillInactive;

};


#endif // !defined(AFX_WINBUTTON_H__9103A627_0574_40A5_B5B6_060572A121B3__INCLUDED_)
