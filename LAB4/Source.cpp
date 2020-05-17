#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#define ID_TIMER2   2

const int x = 1200;
const int y = 800;
HWND hWnd;
DWORD idThread;
HANDLE hThread1;
BOOL bFin = true;
TCHAR szClassName[] = _T("FirstWindow");
TCHAR szTitel[] = _T("Лаб.4");
WNDCLASS WndClass;
MSG Msg;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HPEN MyPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 200));
HBRUSH MyBrush = CreateSolidBrush(RGB(0, 0, 200));
long start = 0;
long current = 0;
INT cvet = 0;
//INT fff = 1;
INT z = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = szClassName;
	start = GetCurrentTime();
	if (!RegisterClass(&WndClass))
	{
		MessageBox(NULL, _T("Cannot register class"), _T("Error"), MB_OK);
		return 0;
	}

	hWnd = CreateWindow(szClassName, szTitel, WS_OVERLAPPEDWINDOW, 10, 10, x, y, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Cannot create window"), _T("Error"), MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

DWORD WINAPI Thread1(LPVOID)
{
	while (bFin)
	{
		InvalidateRect(hWnd, NULL, TRUE);
		Sleep(1000);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT Rect;
	WCHAR szBuff[15];
	WCHAR szBuff1[1];
	POINT My_Massiv[3];
	HFONT hFont;
	INT lengthOfSec = 0;
	INT i = 0;
	INT l = 0;
	INT k = 0;
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, ID_TIMER2, 2000, NULL);
		hThread1 = CreateThread(NULL, 0, Thread1, NULL, 0, &idThread);
	case WM_TIMER:
		if (wParam == ID_TIMER2)
		{
			InvalidateRect(hWnd, NULL, TRUE);
			z = 1;
		}
		break;
	case WM_PAINT:
		WaitForSingleObject(hThread1, 1);
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &Rect);
		My_Massiv[0].x = Rect.right - 300; My_Massiv[0].y = Rect.top + 120;
		My_Massiv[1].x = Rect.right - 100; My_Massiv[1].y = Rect.top + 600;
		My_Massiv[2].x = Rect.right - 500; My_Massiv[2].y = Rect.top + 600;
		SetTextColor(hdc, RGB(20, 255, 20));
		//SetBkColor(hdc, RGB(250, 200, 250));
		hFont = CreateFont(60, 40, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
		SelectObject(hdc, hFont);
		if (hThread1 /*&& z == 0*/)//вывод количества секунд, прошедших с запуска системы
		{
			current = ((GetCurrentTime()-start) / 1000);//////////////!!!!!!!!!!!!!!!!!!!!!!!!
			WCHAR a[4] = { ' ',' ',' ',' ' };
			wsprintf(a, TEXT("%d"),current);
			SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
			TextOut(hdc, Rect.left + 200, Rect.bottom - 100, a, 4);
			while (szBuff[i] != NULL)
			{
				lengthOfSec++;
				i++;
			}
			while (k < 50 * lengthOfSec && k != -1)//вывод цифр разными цветами
			{
				if (l == 0)
					SetTextColor(hdc, RGB(255, 0, 0));
				else if (l == 1)
					SetTextColor(hdc, RGB(255, 165, 0));
				else if (l == 2)
					SetTextColor(hdc, RGB(0, 255, 0));
				else if (l == 3)
					SetTextColor(hdc, RGB(0, 165, 255));
				else if (l == 4)
					SetTextColor(hdc, RGB(128, 0, 128));
				else SetTextColor(hdc, RGB(rand() % 255+0, rand() % 255 + 0, rand() % 255 + 0));
				szBuff1[0] = szBuff[l];
				//TextOut(hdc, Rect.left + 50 + k, Rect.top + 300, szBuff1, 1);
				k = k + 50;
				l++;
				if (k == 50 * lengthOfSec) k = -1;
			}
		}
		if (z == 1)
		{
			if (cvet == 0)
			{
				MyBrush = CreateSolidBrush(RGB(0, 240, 0));
				MyPen = CreatePen(PS_SOLID, 4, RGB(0, 240, 0));
				cvet = 1;
			}
			else
			{
				MyBrush = CreateSolidBrush(RGB(0, 0, 240));
				MyPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 240));
				cvet = 0;
			}
			SelectPen(hdc, MyPen);
			SelectBrush(hdc, MyBrush);
			Polygon(hdc, My_Massiv, 3);//вывод треугольника
		}
		else
		{
			SelectPen(hdc, MyPen);
			SelectBrush(hdc, MyBrush);
			Polygon(hdc, My_Massiv, 3);
		}
		z = 0;
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN://приостановка потоков
		SuspendThread(hThread1);
		SetTimer(hWnd, ID_TIMER2, INFINITE, NULL);
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &Rect);
		FillRect(hdc, &Rect, (HBRUSH)CreateSolidBrush(RGB(190, 0, 190)));
		ReleaseDC(hWnd, hdc);
		MessageBox(hWnd, TEXT("Потоки приостановлены!!!\nВозобновить по нажатию правой кнопки мыши"), TEXT("Остановка потоков"), MB_ICONINFORMATION);
		break;
	case WM_RBUTTONDOWN://возобновление работы потоков
		ResumeThread(hThread1);
		SetTimer(hWnd, ID_TIMER2, 2000, NULL);
		MessageBox(hWnd, TEXT("Потоки снова запущены!!!"), TEXT("Возобновление работы потоков"), MB_ICONINFORMATION);
		break;
	case WM_DESTROY:
		bFin = FALSE;
		TerminateThread(hThread1, 0);
		DeleteObject(hThread1);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
