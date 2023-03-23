/*#include "WinView.h"
#include "DrawUtils.h"
#include "Transform3DUtils.h"
using namespace Simple3D;

bool WinView::m_IsLoop = true;

DrawUtils* drawHandle = DrawUtils::getInstance();

Transform3DUtils* tranform3D = Transform3DUtils::getInstance();

LRESULT WinView::win_proc(HWND hWnd, ::UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			m_IsLoop = false;
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		m_IsLoop = false;
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

WinView::WinView()
	:m_posX(200), m_posY(200),
	m_width(400), m_height(400),
	mHdc(NULL), mDibsection(NULL),
	mCompatibleDC(NULL), mHwnd(NULL)
{
}

WinView::~WinView()
{
	if (mDibsection) {
		DeleteObject(mDibsection);
		mDibsection = NULL;
	}

	if (mHdc)
	{
		ReleaseDC(mHwnd, mHdc);
		mHdc = NULL;
	}

	if (mCompatibleDC)
	{
		DeleteDC(mCompatibleDC);
		mCompatibleDC = NULL;
	}
}


void WinView::swapFrameBuff(void*& buff)
{
	buff = mFrameBuff;
}


bool WinView::createWin(const char* win_name)
{
	WNDCLASSA wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_PEN);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.lpfnWndProc = (WNDPROC)win_proc;
	wndclass.lpszClassName = win_name;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = m_width;
	bmpInfo.bmiHeader.biHeight = m_height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = m_width * m_height * 4;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;

	// 注册一个窗口
	if (!RegisterClassA(&wndclass))
		return false;
	//创建一个窗口
	mHwnd = CreateWindowA(win_name, win_name,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wndclass.hInstance, NULL);

	if (mHwnd == NULL)
		return false;

	mHdc = GetDC(mHwnd);

	// 获取一个内存dc，用来创建一个画板
	mCompatibleDC = CreateCompatibleDC(mHdc);
	// 创建一个画板
	mDibsection = CreateDIBSection(mCompatibleDC, &bmpInfo, DIB_RGB_COLORS, &mFrameBuff, 0, 0);
	SelectObject(mCompatibleDC, mDibsection);
	if (mDibsection == NULL)
		return false;

	RECT rect = { 0, 0, m_width, m_height };
	int wx, wy, sx, sy;


	AdjustWindowRect(&rect, GetWindowLong(mHwnd, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(mHwnd, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(mHwnd);

	ShowWindow(mHwnd, SW_SHOWNORMAL);

	// 更新窗口  
	UpdateWindow(mHwnd);

	memset(mFrameBuff, 0, m_width * m_height * 4);

	// 初始化画布器
	drawHandle->Init((RGBA*)mFrameBuff, m_width, m_height);

	// 初始化3D变化器
	tranform3D->setViewPort(0, 0, m_width, m_height);

	return true;
}


void WinView::update()
{
	drawHandle->clear();
	tranform3D->clear();
	
	//UCHAR* tempRgb = (UCHAR*)mFrameBuff;
	//int pitch = m_width * 4;

	//memset(tempRgb + pitch * 25, 255, pitch*3);
	


	Vector2D begin(0, 490);
	// for (int i = 0; i < m_width; i++)
	// {
	// 	for (int j = 0; j < m_height; j++)
	// 	{
	// 		begin.x = i;
	// 		begin.y = j;
	//
	// 		UCHAR r = rand() % 255;、
	// 		UCHAR g = rand() % 255;
	// 		UCHAR b = rand() % 255;
	// 		drawHandle.drawPixel(begin, MAKE_RGBA(r, g, b));
	// 	}
	// }

	//drawHandle.drawFillRect(begin, 100, 100);

	SImage image1;
	image1.createImage("bg.png");

	SImage image2;
	image2.setPosition(Vector2D(300, 100));
	image2.setScale(2.2f);
	//image2.alphaTest(true, 125);
	//image2.setOpacity(155);
	image2.createImage("book.png");



	// 利用投影画一个三角形

	Vec3 posArr[3] = {
		Vec3(2.0f, 0.0f, -4.0f),
		Vec3(0.0f, 0.0f, -4.0f),
		Vec3(0.0f, 2.0f, -4.0f),
	};
	const int len = 8;
	Vec3 posXX[len][3] = {
		{ Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f) },
		{ Vec3(1.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f) },
		{ Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, -1.0f) },
		{ Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, -1.0f), Vec3(1.0f, 1.0f, -1.0f) },
		{ Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, -1.0f), Vec3(1.0f, 1.0f, -1.0f) },
		{ Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, -1.0f) },
		{ Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f) },
		{ Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, -1.0f) },
	};

	Vec3 eye(0, 10, 5);
	Vec3 at(0, 0, -1);
	//tranform3D->Translate(2, 0, 0);
	tranform3D->Scale(3.0f, 3.0f, 3.0f);
	//tranform3D->RotateZ(90);
	tranform3D->LookAt(eye, at);
	tranform3D->Perspective(60, m_width / m_height, 1.0f, 500.0f);

	Vertex vertexTest[len][3] = {
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
		{ Vertex{ 200, 400, S_RED, 0.0f, 0.0f }, Vertex{ 400, 400, S_WHITE, 1.0f, 0.0f }, Vertex{ 300, 500, S_WHITE, 0.5f, 1.0f }, },
	};


	for (int n = 0; n < len; n++)
	{
		bool IsInCVV = true;
		for (int m = 0; m < 3; m++)
		{
			Vec4 tempPP(posXX[n][m].x, posXX[n][m].y, posXX[n][m].z, 1);

			Matrix4 mvp = tranform3D->MVP();

			Vec4 temPos = mvp * tempPP;
			if (!tranform3D->TransformCheckInCVV(temPos))
			{
				IsInCVV = false;
			}

			Vec3 screenPos;
			tranform3D->TransformUI(temPos, screenPos);
			vertexTest[n][m].x = screenPos.x;
			vertexTest[n][m].y = screenPos.y;

			//printf("vec[%d] x = %f, y = %f,z = %f\n", m, screenPos.x, screenPos.y, screenPos.z);
		}
		if (IsInCVV)
			drawHandle->drawTriangle(vertexTest[n]);

	}



}

bool WinView::mainLoop()
{
	MSG msg;
	while (true) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			break;

		if (!GetMessage(&msg, NULL, 0, 0))
			break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	update();
	BitBlt(mHdc, 0, 0, m_width, m_height, mCompatibleDC, 0, 0, SRCCOPY);
	return true;
}*/