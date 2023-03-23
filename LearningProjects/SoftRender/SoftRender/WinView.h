#pragma once
#ifndef _WIN_VIEW_H
#define _WIN_VIEW_H
//#include "simple3D.h"
#include <Windows.h>

namespace Simple3D {
	class WinView
	{
	public:
		WinView();
		~WinView();

		void update();

		bool mainLoop();

		bool createWin(const char* win_name);

		void swapFrameBuff(void*& buff);

		void setWinPosition(float x, float y)
		{
			m_posX = x;			m_posY = y;
		};

		void setWinSize(float widht, float height)
		{
			m_width = widht;	m_height = height;
		};

		static bool checkIsLoop() { return m_IsLoop; };

		static LRESULT win_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		float			m_posX;
		float			m_posY;
		float			m_width;
		float			m_height;
		static bool		m_IsLoop;

		HDC				mHdc;				// 配套的 HDC
		HDC				mCompatibleDC;		// 配套的 内存DC
		HWND			mHwnd;				// 主窗口 HWND
		HBITMAP			mDibsection;		// DIB
		void* mFrameBuff;			// frame buffer
	};
}

#endif

