
// mazeGeneratorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "mazeGenerator.h"
#include "mazeGeneratorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CmazeGeneratorDlg ��ȭ ����



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


// CmazeGeneratorDlg �޽��� ó����

BOOL CmazeGeneratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CSpinButtonCtrl *spinBox = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_CELLSIZE);
	spinBox->SetRange(MIN_CELLSIZE, 500);
	m_checkBtnPath.SetCheck(true);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CmazeGeneratorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();	
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CmazeGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmazeGeneratorDlg::OnBnClickedButtonGenerate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// pictureControl �ʱ�ȭ
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

	// ��� �簢������ �����.
	CBrush whitePaint;
	whitePaint.CreateSolidBrush(RGB(255, 255, 255));
	mPicCtrl->SelectObject(&whitePaint);
	CPen *oldPen, pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = mPicCtrl->SelectObject(&pen);
	mPicCtrl->Rectangle(0, 0, m_picCtrlWidth, m_picCtrlHeight);
	mPicCtrl->SelectObject(oldPen);

	// cellSize�� ���� Picture Control �� ������ ���� �� �ʺ�, ���̸� ����Ѵ�.
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
	
	// ���� ��ġ�� ���� 4��Ʈ (1111b) �� ��Ÿ����.
	// 1111 �� ���� ��/��/��/�츦 �ǹ���.

	// ���� ��
	if((wallBit&8) == 8){
		mPicCtrl->MoveTo((col+1)*cellSize, (row+1)*cellSize);
		mPicCtrl->LineTo((col+2)*cellSize, (row+1)*cellSize);
	}
	// ������ ��
	if((wallBit&4) == 4){
		mPicCtrl->MoveTo((col+2)*cellSize, (row+1)*cellSize);
		mPicCtrl->LineTo((col+2)*cellSize, (row+2)*cellSize);
	}
	// �Ʒ��� ��
	if((wallBit&2) == 2){
		mPicCtrl->MoveTo((col+1)*cellSize, (row+2)*cellSize);
		mPicCtrl->LineTo((col+2)*cellSize, (row+2)*cellSize);
	}
	// ���� ��
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
	// left-up �� right-bottom �� �ٱ����� ���� ����. (�������� ������)
	m_mazeGrid[0][0] &= 7;	// ���� ����
	m_mazeGrid[m_mazeHeight-1][m_mazeWidth-1] &= 13; // �Ʒ��� ����

	return true;
}

void CmazeGeneratorDlg::makeMaze(bool* visit[MAX_MAZESIZE], int y, int x, int cellSize)
{
	if(y < 0 || y >= m_mazeHeight || x < 0 || x >= m_mazeWidth) return;
	visit[y][x] = true;

	static const int dirt[4][2]={-1, 0, 0, 1, 1, 0, 0, -1};
	// 2014-10-06 18:37
	// ���� Ƚ���� ���� (���� Ȯ�� ����)
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// �̷ΰ� �������� �ʾҴٸ� ����
	if(!m_mazeWidth || !m_mazeHeight || !m_cellSize) return;

	// ������ �ε�� �̷� ũ��� �ٸ��� ���� (2014-10-06 18:47 ����)
	if( m_cellSize !=  GetDlgItemInt(IDC_EDIT_CELLSIZE) ) return;

	// Path/BFS �� �����ֱ� �� �ʱ�ȭ (2014-10-08 16:55 ����)
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

	// ��� ���� ��Ȱ��ȭ
	BtnShowStatus();
	// �ʱ�ȭ ��ư Ȱ��ȭ
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(true);
}


void CmazeGeneratorDlg::pathTracing(int sy, int sx, int ey, int ex)
{
	mPicCtrl->LineTo((ex+1)*m_cellSize+m_cellSize/2, (ey+1)*m_cellSize+m_cellSize/2);
	mPicCtrl->MoveTo((ex+1)*m_cellSize+m_cellSize/2, (ey+1)*m_cellSize+m_cellSize/2);

	if(sy == ey && sx == ex) return;
	// ���������κ��� ���������� ������ trace�� ���������� ã�ư���.
	// �߰��� ��ΰ� �̷��� ������ ��� ���� ����.
	
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
		// ���� ���� ������
		if((wallBit&8) == 0 && cy-1 >= 0 && visit[cy-1][cx] == false
			&& /* ������� ���� ���� ���� */ (cy != 0 || cx != 0)){
			q.push(Point(cx, cy-1, cd+1));
			visit[cy-1][cx] = true;
			m_mazeBack[cy-1][cx] = DirectionIndex::DOWN;
		}
		// ������ ���� ������
		if((wallBit&4) == 0 && cx+1 < m_mazeWidth && visit[cy][cx+1] == false){
			q.push(Point(cx+1, cy, cd+1));
			visit[cy][cx+1] = true;
			m_mazeBack[cy][cx+1] = DirectionIndex::LEFT;
		}
		// �Ʒ��� ���� ������
		if((wallBit&2) == 0 && cy+1 < m_mazeHeight && visit[cy+1][cx] == false
			&& /* �������� �Ʒ��� ���� ���� */ (cy != m_mazeHeight-1 || cx != m_mazeWidth-1)){
			q.push(Point(cx, cy+1, cd+1));
			visit[cy+1][cx] = true;
			m_mazeBack[cy+1][cx] = DirectionIndex::UP;
		}
		// ���� ���� ������
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(drawMaze(m_cellSize) == false) return;

	// ��� ���� ��Ȱ��ȭ
	BtnShowStatus();
	// �ʱ�ȭ ��ư Ȱ��ȭ
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(false);
}


void CmazeGeneratorDlg::OnEnKillfocusEditCellsize()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	//Invalidate( false );
	if(IsWindow(m_picControl.m_hWnd))	//��ó��Ʈ�� ���� Ȯ��
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// ��� ���� (from v.1.4.0)
	/*
	static bool isRunning = true;
	if ( isRunning && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {      
        if (GetDlgItem(IDC_EDIT_CELLSIZE) == GetFocus())
        {
			isRunning = false;
			// ���͸� ������
			// ����Ʈ�ڽ��� �Է°� �����ħ
			OnEnKillfocusEditCellsize();
			// �̷� ���� ��ư�� ���� ȿ��
            OnBnClickedButtonGenerate();
        }
    }
	isRunning = true;
	*/
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CmazeGeneratorDlg::OnBnClickedCheckPath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_checkViewPath = !m_checkViewPath;
	BtnShowStatus();
}


void CmazeGeneratorDlg::OnBnClickedCheckBfs()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// ������ ��� �߰� v.1.4.1
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