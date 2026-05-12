// DrawCircleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DrawCircle.h"
#include "DrawCircleDlg.h"
#include "afxdialogex.h"
#include "math.h"
#include <mutex>
#include <condition_variable>

#ifdef _DEBUG
#define new DEBUG_NEW
#include <iostream>
#endif
#include <thread>

using namespace std;

#ifdef _DEBUG
inline void debugPrint(CString str)
{
	wcout << (LPCTSTR)str << endl;
}
#else
#define debugPrint(str)
#endif

mutex g_mutex; // 전역 뮤텍스 객체 선언
condition_variable g_condVar; // 전역 조건 변수 객체 선언

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDrawCircleDlg 대화 상자



CDrawCircleDlg::CDrawCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWCIRCLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_SIZE, m_nCircleWidth);
	DDX_Text(pDX, IDC_EDIT_POINT_WIDTH, m_nPointerRadius);
}

BEGIN_MESSAGE_MAP(CDrawCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DRAW_CIRCLE, &CDrawCircleDlg::OnBnClickedBtnDrawCircle)
	ON_EN_CHANGE(IDC_EDIT_CIRCLE_SIZE, &CDrawCircleDlg::OnEnChangeEditCircleSize)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDrawCircleDlg::OnBnClickedBtnClear)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT_POINT_WIDTH, &CDrawCircleDlg::OnEnChangeEditPointWidth)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDrawCircleDlg 메시지 처리기

BOOL CDrawCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	initialzieImage();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDrawCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDrawCircleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		updateImage(); // 이미지 업데이트
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDrawCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CDrawCircleDlg::drawCircle(double x, double y, double radius, BYTE color, int width, bool fill) // 원을 그리는 멤버 함수 선언
{
#ifdef _DEBUG
	CString msg;
	msg.Format(_T("drawCircle: x=%lf, y=%lf, radius=%lf, color=%u, width=%d, fill=%s"),
		x, y, radius, static_cast<unsigned int>(color), width, fill ? _T("true") : _T("false"));
	debugPrint(msg);
#endif

	BYTE* pBits = (BYTE*)m_image.GetBits(); // 이미지의 비트 데이터에 대한 포인터를 가져옵니다.
	int nPitch = m_image.GetPitch();

	for (int i = 0; i < m_nHeight; ++i)
	{
		for (int j = 0; j < m_nWidth; ++j)
		{
			double dx = j - x; // 현재 픽셀의 x 좌표와 원의 중심 x 좌표의 차이 계산
			double dy = i - y; // 현재 픽셀의 y 좌표와 원의 중심 y 좌표의 차이 계산
			double d2xy = dx * dx + dy * dy;
			if (fill == true)
			{
				if (d2xy <= (radius + width) * (radius + width)) // 원의 방정식을 이용하여 픽셀이 원 내부에 있는지 확인
				{
					pBits[i * nPitch + j] = color;
				}
			}
			else if (d2xy >= (radius - width) * (radius - width) && d2xy <= (radius + width) * (radius + width))
			{
				pBits[i * nPitch + j] = color;
			}
		}
	}
}

void CDrawCircleDlg::OnBnClickedBtnDrawCircle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nPointCount > 2)
	{
		m_bRunThread = true; // 스레드 실행 상태를 true로 설정합니다.
		thread t(&CDrawCircleDlg::drawRandomCircleInThread, this, 10);
		t.detach();
	}
}

void CDrawCircleDlg::OnEnChangeEditCircleSize()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nCircleWidth = GetDlgItemInt(IDC_EDIT_CIRCLE_SIZE);
	m_circle.setWidth(m_nCircleWidth);
	refreshCircle(); // 원을 다시 그립니다.
}

void CDrawCircleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bDrag = false; // 드래그 상태로 전환
	CDialogEx::OnLButtonDown(nFlags, point);
	if (validatePoint(point)) // 클릭한 위치가 이미지 범위 내에 있는지 확인
	{
		if (m_nPointCount < 3) // 세 점의 개수가 3보다 작은 경우에만 점을 추가
		{
			m_aPoints[m_nPointCount] = point;
			drawCircle(point.x, point.y, m_nPointerRadius, m_nPointerColor, m_nPointerWidth, true); // 클릭한 위치를 중심으로 반지름이 10인 원을 그립니다.
			updateImage();
		}
		if (m_nPointCount == 2) // 세 점이 모두 추가된 경우에 원을 그립니다.
		{
			m_circle = Circle::fromPoints(m_aPoints[0], m_aPoints[1], m_aPoints[2], m_nCircleColor, m_nCircleWidth, false); // 세 점을 이용해 Circle 객체를 생성합니다.
			refreshCircle(); // 원을 다시 그립니다.
		}
		if (m_nPointCount >= 3) 
		{
			for (int i = 0; i < 3; i++)
			{
				double dx = point.x - m_aPoints[i].x; // 클릭한 위치와 점의 x 좌표의 차이 계산
				double dy = point.y - m_aPoints[i].y; // 클릭한 위치와 점의 y 좌표의 차이 계산
				double d2xy = dx * dx + dy * dy; // 클릭한 위치와 점 사이의 거리 계산
				if (d2xy <= m_nPointerRadius * m_nPointerRadius) // 클릭한 위치가 점의 반지름 내에 있는지 확인
				{
					m_nDragPointIndex = i;
					m_bDrag = true; // 드래그 상태로 전환
					break; // 드래그할 점을 찾았으므로 루프를 종료합니다.
				}
			}
		}
		else
			m_nPointCount++;
	}
}

void CDrawCircleDlg::initialzieImage()
{
	m_image.Create(m_nWidth, m_nHeight, m_nBpp); // CImage 객체를 초기화합니다.
	if (m_nBpp == 8) // 8비트 팔레트 모드인 경우
	{
		RGBQUAD palette[256];
		memset(palette, 0, sizeof(palette)); // 팔레트 배열을 초기화합니다.
		for (int i = 0; i < 256; ++i)
		{
			palette[i].rgbRed = i;   // 빨간색 값 설정
			palette[i].rgbGreen = i; // 녹색 값 설정
			palette[i].rgbBlue = i;  // 파란색 값 설정
			palette[i].rgbReserved = 0; // 예약된 필드 초기화
		}
		m_image.SetColorTable(0, 256, palette); // 팔레트를 설정합니다.
	}

	int nPitch = m_image.GetPitch(); // 이미지의 피치(한 줄의 바이트 수)를 가져옵니다.
	BYTE* pBits = (BYTE*)m_image.GetBits(); // 이미지의 비트 데이터에 대한 포인터를 가져옵니다.
	for (int i = 0; i < m_nHeight; ++i)
	{
		for (int j = 0; j < m_nWidth; ++j)
		{
			pBits[i * nPitch + j] = 255; // 이미지의 모든 픽셀을 흰색으로 초기화합니다.
		}
	}
}
void CDrawCircleDlg::updateImage()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0); // CImage 객체를 대화 상자에 그립니다.
}
void CDrawCircleDlg::clearImage()
{
	if (m_image.IsNull() == false)
	{
		int nPitch = m_image.GetPitch(); // 이미지의 피치(한 줄의 바이트 수)를 가져옵니다.
		BYTE* pBits = (BYTE*)m_image.GetBits(); // 이미지의 비트 데이터에 대한 포인터를 가져옵니다.
		for (int i = 0; i < m_nHeight; ++i)
		{
			for (int j = 0; j < m_nWidth; ++j)
			{
				pBits[i * nPitch + j] = 255; // 이미지의 모든 픽셀을 흰색으로 초기화합니다.
			}
		}
	}
}
void CDrawCircleDlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	clearImage();
	m_nPointCount = 0; // 세 점의 개수를 초기화합니다.
	updateImage();
}

void CDrawCircleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrag)
	{
		if (validatePoint(point)) // 클릭한 위치가 이미지 범위 내에 있는지 확인
		{
			m_aPoints[m_nDragPointIndex] = point; // 드래그한 위치로 점의 좌표를 업데이트
			m_circle = Circle::fromPoints(m_aPoints[0], m_aPoints[1], m_aPoints[2], m_nCircleColor, m_nCircleWidth, false); // 세 점을 이용해 Circle 객체를 생성합니다.
			refreshCircle(); // 원을 다시 그립니다.
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CDrawCircleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrag)
	{
		m_bDrag = false; // 드래그 상태가 아님
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDrawCircleDlg::OnEnChangeEditPointWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nPointerRadius = GetDlgItemInt(IDC_EDIT_POINT_WIDTH);
	refreshCircle(); // 원을 다시 그립니다.
}

BOOL CDrawCircleDlg::validatePoint(CPoint point)
{
	CRect rect(0, 0, m_nWidth, m_nHeight);
	if (rect.PtInRect(point))
	{
		return true;
	}
	return false;
}

void CDrawCircleDlg::refreshCircle()
{
	clearImage();
	drawCircle(m_circle);
	for (int i = 0; i < 3; i++)	//3점 그리기
	{
		drawCircle(m_aPoints[i].x, m_aPoints[i].y, m_nPointerRadius, m_nPointerColor, m_nPointerWidth, true);
	}
	drawCircle(m_circle.getX(), m_circle.getY(), m_nPointerRadius, m_nPointerColor, m_nPointerWidth, true); // 원의 중심을 점으로 표시
	CString strText;
	strText.Format(_T("(%d, %d)"), static_cast<int>(m_circle.getX()), static_cast<int>(m_circle.getY()));
	drawText(strText, static_cast<int>(m_circle.getX() + m_nPointerRadius + m_nPointerWidth), static_cast<int>(m_circle.getY() + m_nPointerRadius + m_nPointerWidth)); // 원의 중심 좌표를 텍스트로 표시
	updateImage();
}

void CDrawCircleDlg::drawCircle(Circle circle)
{
	if (circle.isValid())
	{
		drawCircle(circle.getX(), circle.getY(), circle.getRadius(), circle.getColor(), circle.getWidth(), circle.isFill());
	}
}

void CDrawCircleDlg::drawText(const CString& text, int x, int y)
{
	CDC* pDC = CDC::FromHandle(m_image.GetDC());
	int prevMode = pDC->SetBkMode(TRANSPARENT);      // 배경 투명
	pDC->TextOut(x, y, text); // 지정된 위치에 텍스트 출력
	pDC->SetBkMode(prevMode); // 이전 배경 모드로 복원
	m_image.ReleaseDC();
}
void CDrawCircleDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	unique_lock<mutex> lock(g_mutex); // 뮤텍스를 잠급니다.
	g_condVar.wait_until(lock, chrono::steady_clock::now() + chrono::seconds(5), [this]() { return m_bRunThread == false; }); // 스레드가 종료될 때까지 대기합니다.
}
void CDrawCircleDlg::drawRandomCircleInThread(int nCount)
{
	lock_guard<mutex> lock(g_mutex);
	for (int n = 0; n < nCount; n++)
	{
		CPoint* pPoints = getPoints();
		for (int i = 0; i < 3; i++)
		{
			pPoints[i].x = rand() % getWidht(); // 0부터 m_nWidth-1 사이의 랜덤한 x 좌표 생성
			pPoints[i].y = rand() % getHeight(); // 0부터 m_nHeight-1 사이의 랜덤한 y 좌표 생성
		}
		setCircle(Circle::fromPoints(pPoints[0], pPoints[1], pPoints[2], getCircleColor(), getCircleWidth(), false));
		refreshCircle();
		setPointerCount(3);
		Sleep(500);
	}
	m_bRunThread = false; // 스레드 실행 상태를 false로 설정하여 스레드가 종료되도록 합니다.
	g_condVar.notify_all(); // 조건 변수를 사용하여 스레드가 종료되었음을 알립니다.
}
