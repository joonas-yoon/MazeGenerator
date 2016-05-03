
// mazeGeneratorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "mazeGenerator.h"
#include "mazeGeneratorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmazeGeneratorDlg 대화 상자



CmazeGeneratorDlg::CmazeGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmazeGeneratorDlg::IDD, pParent)
	, m_picCtrlWidth(0)
	, m_picCtrlHeight(0)
	, m_mazeWidth(0)
	, m_mazeHeight(0)
	, mPicCtrl(NULL)
	, m_cellSize(20)
	, m_checkViewPath(true)
	, m_checkViewBFS(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmazeGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CELLSIZE, m_cellSize);
	DDV_MinMaxInt(pDX, m_cellSize, MIN_CELLSIZE, 500);
	DDX_Control(pDX, IDC_PICTURE_CTRL, m_picControl);
	DDX_Control(pDX, IDC_CHECK_PATH, m_checkBtnPath);
}

BEGIN_MESSAGE_MAP(CmazeGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GEN, &CmazeGeneratorDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_PATHFIND, &CmazeGeneratorDlg::OnBnClickedButtonPathfind)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CmazeGeneratorDlg::OnBnClickedButtonReset)
	ON_EN_KILLFOCUS(IDC_EDIT_CELLSIZE, &CmazeGeneratorDlg::OnEnKillfocusEditCellsize)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CHECK_PATH, &CmazeGeneratorDlg::OnBnClickedCheckPath)
	ON_BN_CLICKED(IDC_CHECK_BFS, &CmazeGeneratorDlg::OnBnClickedCheckBfs)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CmazeGeneratorDlg::OnBnClickedButtonPrint)
END_MESSAGE_MAP()


// CmazeGeneratorDlg 메시지 처리기

BOOL CmazeGeneratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	CSpinButtonCtrl *spinBox = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_CELLSIZE);
	spinBox->SetRange(MIN_CELLSIZE, 500);
	m_checkBtnPath.SetCheck(true);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CmazeGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CmazeGeneratorDlg::OnPaint()
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
		CDialogEx::OnPaint();	
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CmazeGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmazeGeneratorDlg::OnBnClickedButtonGenerate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// pictureControl 초기화
	pictureCtrlInit();

	BtnShowStatus();
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);

	if(mazeGenerator(m_cellSize)){
		drawMaze(m_cellSize);
	}
}


bool CmazeGeneratorDlg::pictureCtrlInit()
{
	mPathDC = mPicCtrl = GetDlgItem(IDC_PICTURE_CTRL)->GetDC();
	CStatic *mPicCtrl_static = (CStatic*)GetDlgItem(IDC_PICTURE_CTRL);
	CRect mPicCtrlSize;
	mPicCtrl_static->GetWindowRect(mPicCtrlSize);

	this->m_picCtrlWidth = mPicCtrlSize.Width();
	this->m_picCtrlHeight = mPicCtrlSize.Height();

	// 흰색 사각형으로 지운다.
	CBrush whitePaint;
	whitePaint.CreateSolidBrush(RGB(255, 255, 255));
	mPicCtrl->SelectObject(&whitePaint);
	CPen *oldPen, pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = mPicCtrl->SelectObject(&pen);
	mPicCtrl->Rectangle(0, 0, m_picCtrlWidth, m_picCtrlHeight);
	mPicCtrl->SelectObject(oldPen);

	// cellSize에 맞춰 Picture Control 을 적당히 나눈 후 너비, 높이를 계산한다.
	m_mazeWidth  = (m_picCtrlWidth-2*m_cellSize)/m_cellSize;
	m_mazeHeight = (m_picCtrlHeight-2*m_cellSize)/m_cellSize;

	if(m_mazeHeight < 2
		|| m_mazeWidth < 2
		|| m_mazeHeight >= MAX_MAZESIZE
		|| m_mazeWidth >= MAX_MAZESIZE)
		return false;

	return true;
}


void CmazeGeneratorDlg::drawCell(int row, int col, char wallBit, int cellSize)
{
	CPen *oldPen, pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = mPicCtrl->SelectObject(&pen);
	
	// 벽의 위치에 대해 4비트 (1111b) 로 나타낸다.
	// 1111 은 각각 상/하/좌/우를 의미함.

	// 위쪽 벽
	if((wallBit&8) == 8){
		mPicCtrl->MoveTo((col+1)*cellSize, (row+1)*cellSize);
		mPicCtrl->LineTo((col+2)*cellSize, (row+1)*cellSize);
	}
	// 오른쪽 벽
	if((wallBit&4) == 4){
		mPicCtrl->MoveTo((col+2)*cellSize, (row+1)*cellSize);
		mPicCtrl->LineTo((col+2)*cellSize, (row+2)*cellSize);
	}
	// 아래쪽 벽
	if((wallBit&2) == 2){
		mPicCtrl->MoveTo((col+1)*cellSize, (row+2)*cellSize);
		mPicCtrl->LineTo((col+2)*cellSize, (row+2)*cellSize);
	}
	// 왼쪽 벽
	if((wallBit&1) == 1){
		mPicCtrl->MoveTo((col+1)*cellSize, (row+1)*cellSize);
		mPicCtrl->LineTo((col+1)*cellSize, (row+2)*cellSize);
	}

	mPicCtrl->SelectObject(oldPen);
}


bool CmazeGeneratorDlg::mazeGenerator(int cellSize)
{
	if(mazeInit(cellSize) == false) return false;

	bool** visit = new bool*[m_mazeHeight];
	for(int i=0; i < m_mazeHeight; ++i){
		visit[i] = new bool[m_mazeWidth];
		for(int j=0; j < m_mazeWidth; ++j) visit[i][j] = false;
	}

	makeMaze(visit, 0, 0, cellSize);

	return true;
}


bool CmazeGeneratorDlg::mazeInit(int cellSize)
{
	if(!m_picCtrlWidth || pictureCtrlInit() == false) return false;

	for(int i=0; i < m_mazeHeight; ++i){
		for(int j=0; j < m_mazeWidth; ++j){
			m_mazeGrid[i][j] = 15;
			m_mazeBack[i][j] = DirectionIndex::NONE;
		}
	}
	// left-up 과 right-bottom 은 바깥으로 벽을 뚫음. (시작점과 도착점)
	m_mazeGrid[0][0] &= 7;	// 위로 뚫음
	m_mazeGrid[m_mazeHeight-1][m_mazeWidth-1] &= 13; // 아래로 뚫음

	return true;
}

void CmazeGeneratorDlg::makeMaze(bool* visit[MAX_MAZESIZE], int y, int x, int cellSize)
{
	if(y < 0 || y >= m_mazeHeight || x < 0 || x >= m_mazeWidth) return;
	visit[y][x] = true;

	static const int dirt[4][2]={-1, 0, 0, 1, 1, 0, 0, -1};
	// 2014-10-06 18:37
	// 셔플 횟수를 높임 (랜덤 확률 보정)
	DirectionIndex order[4]={UP, RIGHT, DOWN, LEFT};
	arrayShuffle(order, 4, 8);
	for(int i=0; i < 4; ++i){
		DirectionIndex d = order[i];
		int dy = y + dirt[d][0];
		int dx = x + dirt[d][1];
		if(dy < 0 || dy >= m_mazeHeight || dx < 0 || dx >= m_mazeWidth || visit[dy][dx]) continue;
		visit[dy][dx] = true;

		if(d == DirectionIndex::UP){
			m_mazeGrid[y][x] &= 7;
			m_mazeGrid[dy][dx] &= 13;
		}
		if(d == DirectionIndex::RIGHT){
			m_mazeGrid[y][x] &= 11;
			m_mazeGrid[dy][dx] &= 14;
		}
		if(d == DirectionIndex::DOWN){
			m_mazeGrid[y][x] &= 13;
			m_mazeGrid[dy][dx] &= 7;
		}
		if(d == DirectionIndex::LEFT){
			m_mazeGrid[y][x] &= 14;
			m_mazeGrid[dy][dx] &= 11;
		}
		makeMaze(visit, dy, dx, cellSize);
	}
}


void CmazeGeneratorDlg::arrayShuffle(DirectionIndex* ar, int size, int seed)
{
	for(int i=0; i < seed; ++i){
		int r1 = WellRand()%size, r2 = WellRand()%size;
		DirectionIndex t = ar[r1];
		ar[r1] = ar[r2];
		ar[r2] = t;
	}
}


unsigned long CmazeGeneratorDlg::WellRand()
{
	static unsigned long state[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	srand(time(NULL));
	static unsigned int index = 0;
	unsigned long a, b, c, d;
	for(int i=0; i < 8; ++i){
		int r1 = rand()%16, r2 = rand()%16;
		int t = state[r1];
		state[r1] = state[r2];
		state[r2] = t;
	}
	a = state[index];
	c = state[(index+13)&15];
	b = a^c^(a<<16)^(c<<15);
	c = state[(index+9)&15];
	c ^= (c>>11);
	a = state[index] = b^c;
	d = a^((a<<5)&0xDA442D20UL);
	index = (index+15)&15;
	state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
	return state[index];
}


void CmazeGeneratorDlg::OnBnClickedButtonPathfind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 미로가 생성되지 않았다면 종료
	if(!m_mazeWidth || !m_mazeHeight || !m_cellSize) return;

	// 이전에 로드된 미로 크기와 다르면 종료 (2014-10-06 18:47 수정)
	if( m_cellSize !=  GetDlgItemInt(IDC_EDIT_CELLSIZE) ) return;

	// Path/BFS 를 보여주기 전 초기화 (2014-10-08 16:55 수정)
	OnBnClickedButtonReset();

	pathFinder(0, 0, m_mazeHeight-1, m_mazeWidth-1);
	if(m_checkViewPath)
	{
		CPen redPen;
		redPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		mPicCtrl->SelectObject(&redPen);
		mPicCtrl->MoveTo(m_mazeWidth*m_cellSize+m_cellSize/2, (m_mazeHeight+1)*m_cellSize+m_cellSize/2);
		mPicCtrl->LineTo(m_mazeWidth*m_cellSize+m_cellSize/2, m_mazeHeight*m_cellSize+m_cellSize/2);

		pathTracing(0, 0, m_mazeHeight-1, m_mazeWidth-1);

		mPicCtrl->LineTo(m_cellSize*1.5, m_cellSize/2);
	}

	// 경로 생성 비활성화
	BtnShowStatus();
	// 초기화 버튼 활성화
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);
}


void CmazeGeneratorDlg::pathTracing(int sy, int sx, int ey, int ex)
{
	mPicCtrl->LineTo((ex+1)*m_cellSize+m_cellSize/2, (ey+1)*m_cellSize+m_cellSize/2);
	mPicCtrl->MoveTo((ex+1)*m_cellSize+m_cellSize/2, (ey+1)*m_cellSize+m_cellSize/2);

	if(sy == ey && sx == ex) return;
	// 시작점으로부터 도착점까지 남겨진 trace를 도착점부터 찾아간다.
	// 중간에 경로가 미로의 범위를 벗어날 일은 없음.
	
	static const int dirt[4][2]={-1, 0, 0, 1, 1, 0, 0, -1};
	char d = m_mazeBack[ey][ex];
	if(d == DirectionIndex::NONE) return;

	pathTracing(sy, sx, ey+dirt[d][0], ex+dirt[d][1]);
}


void CmazeGeneratorDlg::pathFinder(int sy, int sx, int ey, int ex)
{
	if(!m_mazeWidth || !m_mazeHeight) return;

	typedef struct Point {
		int x, y, d;
		Point(){}
		Point(int _x, int _y, int _d) :x(_x), y(_y), d(_d){}
	} Point;

	bool **visit = new bool*[m_mazeHeight];
	for(int i=0; i < m_mazeHeight; ++i){
		visit[i] = new bool[m_mazeWidth];
		for(int j=0; j < m_mazeWidth; ++j) visit[i][j] = false;
	}

	static const int dirt[4][2]={-1, 0, 0, 1, 1, 0, 0, -1};

	std::queue<Point> q;
	q.push(Point(sx, sy, 0));
	while(!q.empty()){
		int cy = q.front().y;
		int cx = q.front().x;
		int cd = q.front().d;
		q.pop();

		if(cy == ey && cx == ex) break;
		visit[cy][cx] = true;

		char wallBit = m_mazeGrid[cy][cx];
		// 위쪽 벽이 없으면
		if((wallBit&8) == 0 && cy-1 >= 0 && visit[cy-1][cx] == false
			&& /* 출발점은 위쪽 벽이 없음 */ (cy != 0 || cx != 0)){
			q.push(Point(cx, cy-1, cd+1));
			visit[cy-1][cx] = true;
			m_mazeBack[cy-1][cx] = DirectionIndex::DOWN;
		}
		// 오른쪽 벽이 없으면
		if((wallBit&4) == 0 && cx+1 < m_mazeWidth && visit[cy][cx+1] == false){
			q.push(Point(cx+1, cy, cd+1));
			visit[cy][cx+1] = true;
			m_mazeBack[cy][cx+1] = DirectionIndex::LEFT;
		}
		// 아래쪽 벽이 없으면
		if((wallBit&2) == 0 && cy+1 < m_mazeHeight && visit[cy+1][cx] == false
			&& /* 도착점은 아래쪽 벽이 없음 */ (cy != m_mazeHeight-1 || cx != m_mazeWidth-1)){
			q.push(Point(cx, cy+1, cd+1));
			visit[cy+1][cx] = true;
			m_mazeBack[cy+1][cx] = DirectionIndex::UP;
		}
		// 왼쪽 벽이 없으면
		if((wallBit&1) == 0 && cx-1 >= 0 && visit[cy][cx-1] == false){
			q.push(Point(cx-1, cy, cd+1));
			visit[cy][cx-1] = true;
			m_mazeBack[cy][cx-1] = DirectionIndex::RIGHT;
		}

		// BFS Debug
		if(m_checkViewBFS)
		{
			CPen redPen;
			redPen.CreatePen(PS_SOLID, 2, RGB(cd*25, cd*15, cd*25));
			mPicCtrl->SelectObject(&redPen);
			mPicCtrl->Rectangle(
				(cx+1)*m_cellSize+m_cellSize/3*1,
				(cy+1)*m_cellSize+m_cellSize/3*1,
				(cx+1)*m_cellSize+m_cellSize/3*2,
				(cy+1)*m_cellSize+m_cellSize/3*2
			);
		}
	}
}


bool CmazeGeneratorDlg::drawMaze(int cellSize)
{
	if(pictureCtrlInit() == false) return false;
	for(int h=0; h < m_mazeHeight; ++h){
		for(int w=0; w < m_mazeWidth; ++w){
			drawCell(h, w, m_mazeGrid[h][w], cellSize);
		}
	}
	return true;
}


void CmazeGeneratorDlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(drawMaze(m_cellSize) == false) return;

	// 경로 생성 비활성화
	BtnShowStatus();
	// 초기화 버튼 활성화
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);
}


void CmazeGeneratorDlg::OnEnKillfocusEditCellsize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cellSize = GetDlgItemInt(IDC_EDIT_CELLSIZE);
	if( m_cellSize < MIN_CELLSIZE )
	{
		m_cellSize = MIN_CELLSIZE;
		SetDlgItemInt(IDC_EDIT_CELLSIZE, m_cellSize);
	}
}


void CmazeGeneratorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//Invalidate( false );
	if(IsWindow(m_picControl.m_hWnd))	//피처컨트롤 유무 확인
	{
		//CStatic *mPicCtrl_static = (CStatic*)GetDlgItem(IDC_PICTURE_CTRL);
		CStatic *rightMenuGroup_static = (CStatic*)GetDlgItem(IDC_RIGHT_MENU_GROUP);
		CRect mPicCtrlSize;
		CRect rightMenuGroup;
		//mPicCtrl_static->GetWindowRect(mPicCtrlSize);
		
		GetWindowRect(mPicCtrlSize);
		rightMenuGroup_static->GetWindowRect(rightMenuGroup);

		const int rMenuWidth = rightMenuGroup.Width()+5;

		m_picCtrlWidth = mPicCtrlSize.Width()-rMenuWidth-50;
		m_picCtrlHeight = mPicCtrlSize.Height()-75;
		
		m_mazeWidth  = (m_picCtrlWidth-2*m_cellSize)/m_cellSize;
		m_mazeHeight = (m_picCtrlHeight-2*m_cellSize)/m_cellSize;

		m_picControl.MoveWindow(15+rMenuWidth, 15, m_picCtrlWidth, m_picCtrlHeight);

		pictureCtrlInit();
		if( m_mazeHeight && m_mazeWidth && m_cellSize ) drawMaze(m_cellSize);
	}
}


BOOL CmazeGeneratorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 사용 안함 (from v.1.4.0)
	/*
	static bool isRunning = true;
	if ( isRunning && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {      
        if (GetDlgItem(IDC_EDIT_CELLSIZE) == GetFocus())
        {
			isRunning = false;
			// 엔터를 누르면
			// 에디트박스의 입력값 새고로침
			OnEnKillfocusEditCellsize();
			// 미로 생성 버튼을 누른 효과
            OnBnClickedButtonGenerate();
        }
    }
	isRunning = true;
	*/
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CmazeGeneratorDlg::OnBnClickedCheckPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_checkViewPath = !m_checkViewPath;
	BtnShowStatus();
}


void CmazeGeneratorDlg::OnBnClickedCheckBfs()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_checkViewBFS = !m_checkViewBFS;
	BtnShowStatus();
}


void CmazeGeneratorDlg::BtnShowStatus()
{
	CString str = _T("Show ...");
	if(m_checkViewBFS) str = _T("Show BFS");
	if(m_checkViewPath) str = _T("Show Path");
	if(m_checkViewPath && m_checkViewBFS) str = _T("Show Both");
	GetDlgItem(IDC_BUTTON_PATHFIND)->SetWindowTextW(str);
	GetDlgItem(IDC_BUTTON_PATHFIND)->EnableWindow(m_mazeHeight && (m_checkViewPath || m_checkViewBFS));
}


void CmazeGeneratorDlg::OnBnClickedButtonPrint()
{
	// 프린팅 기능 추가 v.1.4.1
	CWnd* pictureControl = GetDlgItem(IDC_PICTURE_CTRL);
	CClientDC dc(pictureControl);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	CRect rect;
	pictureControl->GetClientRect(rect);
	const int rHeight = rect.Width();
	const int rWidth = rect.Height();

	CBitmap BMP;
	BMP.CreateCompatibleBitmap(&dc, rWidth, rHeight);
	CBitmap* pOldBitmap = MemDC.SelectObject(&BMP);
	MemDC.StretchBlt(0, 0, rWidth, rHeight, &dc, 0, 0, rWidth, rHeight, SRCCOPY);

	/*
	SECJpeg* jpg = new SECJpeg();   
	jpg->CreateFromBitmap(&MemDC,&BMP);     
	jpg->SaveImage("Test.jpg");
	*/

	HANDLE hDib;
	LPSTR pDib;
	LPBITMAPINFO lpBitInfo;
	HANDLE hlpBitInfo;

	hDib = GlobalAlloc(GHND, rWidth * rHeight * 3);
	pDib = (LPSTR)GlobalLock(hDib);
	hlpBitInfo = GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+sizeof(BITMAPINFO));
	lpBitInfo = (LPBITMAPINFO)GlobalLock(hlpBitInfo);
	//BITMAPINFO
	lpBitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBitInfo->bmiHeader.biWidth = rWidth;
	lpBitInfo->bmiHeader.biHeight = rHeight;

	lpBitInfo->bmiHeader.biPlanes = 1;
	lpBitInfo->bmiHeader.biBitCount = 24;
	lpBitInfo->bmiHeader.biCompression = BI_RGB;
	lpBitInfo->bmiHeader.biSizeImage = rWidth * rHeight * 3;
	lpBitInfo->bmiHeader.biXPelsPerMeter = 0;
	lpBitInfo->bmiHeader.biYPelsPerMeter = 0;
	lpBitInfo->bmiHeader.biClrUsed = 0;
	lpBitInfo->bmiHeader.biClrImportant = 0;

	////BITMAPINFO 
	HDC hdc = ::GetDC(m_hWnd);
	GetDIBits(hdc, (HBITMAP)BMP, 0, rHeight, pDib, lpBitInfo, DIB_RGB_COLORS);
	::ReleaseDC(m_hWnd, hdc);

	static DOCINFO docinfo = { sizeof(DOCINFO), _T("IMAGE"), NULL };

	CPrintDialog dlg(FALSE);
	if (dlg.DoModal() == IDCANCEL) return;

	HDC hpdc = dlg.GetPrinterDC();

	int cy = GetDeviceCaps(hpdc, VERTRES);
	int cx = GetDeviceCaps(hpdc, HORZRES);

	if (StartDocW(hpdc, &docinfo)){
		if (StartPage(hpdc)){
			StretchDIBits(hpdc, 0, 0, cx, cy, 0, 0, rWidth, rHeight, pDib, lpBitInfo, DIB_RGB_COLORS, SRCCOPY);
			EndPage(hpdc);
		}
		EndDoc(hpdc);
	}
	::RestoreDC(hpdc, -1);
}