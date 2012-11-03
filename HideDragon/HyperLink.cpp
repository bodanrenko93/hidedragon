// HyperLink.cpp: implementation of the CHyperLink class.
//
//////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------
//
// CHyperLink 1.0
//
// ����: Javen
// ��Դ: http://blog.csdn.net/jun_01
//
// ���������޸Ĵ��ļ��Լ��������ļ�������ע��������
//
// ʹ�ô�����԰�ĳ���ؼ���ɳ����ӣ���������������STATIC,BUTTON
//
// ʹ�÷�����
// 1,�����Ĵ���������CHyperLink��ʵ��
// 2,����Ҫ��ɳ�����ʱ����InitDlgʱ������CHyperLink.Attach()��
// 3,������Ҫ������ʱ���Ե���CHyperLink.Detach()ȡ���󶨣��ؼ��ָ�Ĭ��״̬��
// 4,���κ�ʱ����Ե���CHyperLink.SetLink()�����ó����ӵ��ı���Ŀ��Url�����û��ָ������ȡ�ؼ���Text������Ϊ�����ӵ�Text��Url
//
//-------------------------------------------------------------------


#include "stdafx.h"
#include "HyperLink.h"
#include "shellapi.h"
#include <commctrl.h>
#include <windowsx.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern HINSTANCE hInst;





CHyperLink::CHyperLink()
:m_bMouseOn(FALSE), m_hWnd(NULL), m_hFont(NULL), m_pOldProc(NULL)
{
	*m_szText = L'\0';
	*m_szUrl = L'\0';
}

CHyperLink::~CHyperLink()
{
}


HWND hStatusToolTip = NULL;


//�Ϳؼ���
BOOL CHyperLink::Attach(HWND hWnd)
{
	if(hWnd == NULL)	//��������
		return FALSE;

	if(m_hWnd != NULL)	//�����ظ���
		return FALSE;

	//�ÿؼ�����Ӧ��꣬Ĭ�ϵ�STATIC�ָ��������
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	dwStyle |= SS_NOTIFY;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);

	//���໯��ע�⣬����ʹ����GWL_USERDATA����������������ʹ��
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);

	m_pOldProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)NewProc);
	HFONT hFont = (HFONT)SendMessage(GetParent(hWnd), WM_GETFONT, 0, 0);
	

	m_hWnd = hWnd;

	LOGFONT lf = {0};
	GetObject(hFont, sizeof(lf), &lf);
	lf.lfUnderline = 1;
	m_hFont = CreateFontIndirect(&lf);













	return TRUE;
}

//�Ϳؼ����벢�ͷ���Դ
HWND CHyperLink::Detach()
{
	if(m_hWnd == NULL)
		return NULL;

	HWND hWnd = m_hWnd;
	/*HFONT hFont = m_hFont;*/

	m_hWnd = NULL;
	/*m_hFont = NULL;*/

	if(hWnd != NULL)
	{
		SetWindowLong(hWnd, GWL_USERDATA, 0);
		SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_pOldProc);
	}
	//if(hFont != NULL)
		DeleteObject(m_hFont);

	m_bMouseOn = FALSE;
	m_pOldProc = NULL;

	*m_szText = L'\0';
	*m_szUrl = L'\0';

	return hWnd;
}

HWND CHyperLink::GetHwnd()
{
	return m_hWnd;
}

BOOL CHyperLink::SetLink(LPCTSTR lpszText, LPCTSTR lpszUrl, BOOL bRedraw)
{
	if(wcslen(lpszText) >= MAX_TEXT_LEN || wcslen(lpszUrl) >= MAX_URL_LEN)
		return FALSE;

	wcscpy(m_szText, lpszText);
	wcscpy(m_szUrl, lpszUrl);
	if(bRedraw && m_hWnd != NULL)	//����static��ˢ�±�������������ˢ�¸�����ʵ�֡�
	{
		RECT rcWnd = {0};
		RECT rcParent = {0};
		HWND hParent = GetParent(m_hWnd);

		::GetWindowRect(m_hWnd, &rcWnd);
		::ScreenToClient(hParent, (LPPOINT)(&rcWnd));
		::ScreenToClient(hParent, ((LPPOINT)(&rcWnd))+1);
		//����ϸ�ĵ��жϴ���WS_EX_LAYOUTRTL�����ã�SwapLeftRight();

		InvalidateRect(hParent, &rcWnd, TRUE);
	}

	return TRUE;
}
HWND hwndFrom;
HRESULT CHyperLink::NewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HFONT hOldFont = NULL;

	CHyperLink *pHLink = (CHyperLink *)GetWindowLong(hWnd, GWL_USERDATA);
	if (pHLink == NULL || pHLink->m_hWnd == NULL)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	switch(uMsg) {
	case WM_LBUTTONDOWN:
		return 0;
	case WM_LBUTTONUP:	//���up���������ӣ����ϱߵ�downҲ���ԣ������˰��á�
		{
			TCHAR szUrl[MAX_URL_LEN] = {0};
			LPCTSTR lpszUrl;
			if(*pHLink->m_szUrl == L'\0')	//δָ��urlʱ���ÿؼ�����text����url��
			{
				GetWindowText(hWnd, szUrl, MAX_TEXT_LEN);
				lpszUrl = szUrl;
			}
			else
				lpszUrl = pHLink->m_szUrl;
			ShellExecute(NULL, L"open", lpszUrl, NULL, NULL, SW_SHOWNORMAL);
		}
		return 0;
	case WM_PAINT:		//���ƿؼ�Ϊ��������ʽ
		{
			COLORREF crText;
			if (pHLink->m_bMouseOn) {
				crText = RGB(255, 0, 0);
			}
			else{
				crText = RGB(0, 0, 255);
			}
			LPCTSTR lpszText;
			TCHAR szText[MAX_TEXT_LEN] = {0};

			if(*pHLink->m_szText == L'\0')	//δָ��textʱ���ÿؼ������ġ�
			{
				GetWindowText(hWnd, szText, MAX_TEXT_LEN);
				lpszText = szText;
			}
			else
				lpszText = pHLink->m_szText;

			RECT rcDraw = {0};
			GetClientRect(pHLink->m_hWnd, &rcDraw);

			PAINTSTRUCT ps = {0};
			BeginPaint(hWnd, &ps);

			hOldFont = (HFONT)SelectObject(ps.hdc, pHLink->m_hFont);
			SetTextColor(ps.hdc, crText);
			SetBkMode(ps.hdc, TRANSPARENT);
			DrawText(ps.hdc, lpszText, -1, &rcDraw, DT_VCENTER | DT_SINGLELINE);	//����Ϊ��͵�����������ͻ�����rect���˴������Ż�
			SelectObject(ps.hdc, hOldFont);
			EndPaint(hWnd, &ps);
		}
		return TRUE;
	case WM_SETCURSOR:	//�������Ϊ����
		SetCursor(LoadCursor(NULL, IDC_HAND));
		return TRUE;
	case WM_MOUSEMOVE:	
		pHLink->OnMouseMove(wParam, lParam);
		return 0;
	case WM_CAPTURECHANGED: //��겶�����������Ϣ��Ҫ
		pHLink->m_bMouseOn = FALSE;
		return 0;
	case WM_DESTROY:
		pHLink->Detach();
		break;
	default:
		break;
	}
	return CallWindowProc(pHLink->m_pOldProc, hWnd, uMsg, wParam, lParam);
}

void CHyperLink::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	int xPos = 0,yPos = 0;
	if (!m_bMouseOn) {		//��һ������
		SetCapture(m_hWnd);
		m_bMouseOn = TRUE;
		InvalidateRect(m_hWnd, NULL, FALSE);
	}
	else{					//���뿪
		POINT pt;
		GetCursorPos(&pt);
		RECT rect;
		GetWindowRect(m_hWnd, &rect);
		if (pt.x < rect.left || pt.x > rect.right || pt.y < rect.top || pt.y > rect.bottom) {
			m_bMouseOn = FALSE;
			ReleaseCapture();
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}
}