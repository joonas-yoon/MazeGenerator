
// mazeGeneratorDlg.h : 헤더 파일
//

#pragma once

#define MAX_MAZESIZE 100
#define MIN_CELLSIZE 5

#include <queue>
#include "afxwin.h"

// CmazeGeneratorDlg 대화 상자
class CmazeGeneratorDlg : public CDialogEx
{
// 생성입니다.
public:
	CmazeGeneratorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAZEGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	typedef enum DirectionIndex { NONE=-1, UP, RIGHT, DOWN, LEFT };
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
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonPathfind();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnEnKillfocusEditCellsize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonPrint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	int m_picCtrlWidth;
	int m_picCtrlHeight;
	char m_mazeGrid[MAX_MAZESIZE][MAX_MAZESIZE];
	int m_mazeWidth;
	int m_mazeHeight;
	int m_cellSize;
	DirectionIndex m_mazeBack[MAX_MAZESIZE][MAX_MAZESIZE];
public:
	bool pictureCtrlInit();
	void drawCell(int row, int col, char wallBit, int cellSize);

	bool mazeInit(int cellSize);
	bool mazeGenerator(int cellSize=20);
	void makeMaze(bool* visit[MAX_MAZESIZE], int y, int x, int cellSize);

	void pathFinder(int sy, int sx, int ey, int ex);
	void pathTracing(int sy, int sx, int ey, int ex);

	bool drawMaze(int cellSize);
private:
	void arrayShuffle(DirectionIndex* ar, int size, int seed);

	unsigned long WellRand();
	CStatic m_picControl;
	CDC *mPicCtrl;
	CDC* mPathDC;

public:
	afx_msg void OnBnClickedCheckPath();
	afx_msg void OnBnClickedCheckBfs();
private:
	bool m_checkViewPath;
	bool m_checkViewBFS;
	CButton m_checkBtnPath;
	void BtnShowStatus();
};
