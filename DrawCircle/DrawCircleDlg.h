
// DrawCircleDlg.h: 헤더 파일
//

#pragma once
#include "Circle.h"
#include <thread>

using namespace std;
// CDrawCircleDlg 대화 상자
class CDrawCircleDlg : public CDialogEx
{
private:
	CImage m_image; // CImage 객체를 멤버 변수로 선언
	int m_nCircleWidth = 1; // 원의 선 두께를 저장하는 멤버 변수 선언
	int m_nWidth = 1280; // 이미지의 너비를 저장하는 멤버 변수 선언
	int m_nHeight = 1280; // 이미지의 높이를 저장하는 멤버 변수 선언
	int m_nBpp = 8; // 이미지의 비트 깊이를 저장하는 멤버 변수 선언
	CPoint m_aPoints[3] = { 0, }; // 세 점의 좌표를 저장하는 멤버 변수 선언
	int m_nPointCount = 0; // 세 점의 개수를 저장하는 멤버 변수 선언
	bool m_bDrag = FALSE; // 드래그 상태를 저장하는 멤버 변수 선언
	int m_nDragPointIndex = 0;
	BYTE m_nPointerColor = 80; // 점의 색상을 저장하는 멤버 변수 선언
	BYTE m_nCircleColor = 0; // 원의 색상을 저장하는 멤버 변수 선언
	int m_nPointerRadius = 3; // 점의 반지름 저장하는 멤버 변수 선언
	int m_nPointerWidth = 0;// 점의 선 두께를 저장하는 멤버 변수 선언
	Circle m_circle; // Circle 객체를 멤버 변수로 선언
	bool m_bRunThread = false; // 원을 그리는 작업을 수행하는 스레드의 실행 상태를 저장하는 멤버 변수 선언
// 생성입니다.
public:
	CDrawCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWCIRCLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDrawCircle();
	afx_msg void OnEnChangeEditCircleSize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeEditPointWidth();
	inline int getWidht(){	return m_nWidth; }
	inline int getHeight() { return m_nHeight; }
	inline int getBpp() { return m_nBpp; }
	inline int getCircleWidth() { return m_nCircleWidth; }
	inline int getPointerWidth() { return m_nPointerWidth; }
	inline int getPointerRadius() { return m_nPointerRadius; }
	inline int getPointerColor() { return m_nPointerColor; }
	inline int getCircleColor() { return m_nCircleColor; }
	inline CPoint* getPoints() { return m_aPoints; }
	inline Circle getCircle() { return m_circle; }
	inline void setCircle(Circle circle) { m_circle = circle; }
	inline void setPointerCount(int count) { m_nPointCount = count; }
	BOOL validatePoint(CPoint point);
	void refreshCircle();
	void drawText(const CString& text, int x, int y); // 텍스트를 그리는 멤버 함수 선언
	void drawCircle(Circle circle); // Circle 객체를 이용해 원을 그리는 멤버 함수 선언
	void drawCircle(double x, double y, double radius, BYTE color = 0, int width = 0, bool fill = false); // 원을 그리는 멤버 함수 선언
	void saveImage(const CString& filePath); // 이미지를 저장하는 멤버 함수 선언
	void loadImage(const CString& filePath); // 이미지를 불러오는 멤버 함수 선언
	void initialzieImage();
	void updateImage();
	void clearImage();
	void drawRandomCircleInThread(int nCount);
	afx_msg void OnDestroy();
};
