#include "windows.h"
#include "string"

struct {
    HWND hWnd;
    HDC device_context, context;
    int width, height;
} window;

struct client {
    float x, y;
    int width;
    int height;
};

int camera = 3;
int shelfNum = 1;
int fridgeNum = 1;
int foodSize = 200;
HBITMAP ramen1;
HBITMAP ramen2;
HBITMAP ramen3;
HBITMAP ramen4;
HBITMAP shelf;
HBITMAP fridge;
HBITMAP ball;//заглушка для всей еды

HBITMAP loadImage(const char* name)
{
    return (HBITMAP)LoadImageA(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void setText() {
	SetTextColor(window.context, RGB(0, 0, 0));
	SetBkMode(window.context, TRANSPARENT);
	auto hFont = CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 10, 0, L"CALIBRI");
	auto hTmp = (HFONT)SelectObject(window.context, hFont);
}

void InitGame()
{
    //в этой секции загружаем спрайты с помощью функций gdi
    //пути относительные - файлы должны лежать рядом с .exe 
    //результат работы LoadImageA сохраняет в хэндлах битмапов, рисование спрайтов будет произовдиться с помощью этих хэндлов
    ramen1 = loadImage("ramen1.bmp");
    ramen2 = loadImage("ramen2.bmp");
    ramen3 = loadImage("ramen3.bmp");
    ramen4 = loadImage("ramen4.bmp");
    shelf = loadImage("shelf.bmp");
    fridge = loadImage("fridge.bmp");
	ball = loadImage("ball.bmp");
}

void InitWindow() {
	SetProcessDPIAware();
	window.hWnd = CreateWindow(L"edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

	RECT r;
	GetClientRect(window.hWnd, &r);
	window.device_context = GetDC(window.hWnd);
	window.width = r.right - r.left;
	window.height = r.bottom - r.top;
	window.context = CreateCompatibleDC(window.device_context);
	SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));
	GetClientRect(window.hWnd, &r);
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false)
{
	HBITMAP hbm, hOldbm;
	HDC hMemDC;
	BITMAP bm;

	hMemDC = CreateCompatibleDC(hDC);
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);

	if (hOldbm) {
		GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm);
		StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		SelectObject(hMemDC, hOldbm);
	}
	DeleteDC(hMemDC);
}

struct Pos {
	int x;
	int y;
};

POINT p;
void mouseInput() {
	GetCursorPos(&p);
	ScreenToClient(window.hWnd, &p);
	//GetAsyncKeyState(VK_LBUTTON);
	/*struct Mouse {
		Pos pos;

		void Input()
		{
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(window.hWnd, &p);
			pos = { p.x, p.y };
		}
	};*/
}

void LocOne()
{
	ShowBitmap(window.context, 0, 0, window.width, window.height, ramen1);//задний фон
	TextOutA(window.context, window.width / 4 * 3, window.height - 100, "utility", 7);
} // kitchen

void LocTwo() {
	ShowBitmap(window.context, 0, 0, window.width, window.height, ramen2);//задний фон
} // man's room

void LocThree() {
	ShowBitmap(window.context, 0,0, window.width, window.height, ramen3);//задний фон
} // coridor

void LocFour() {
	ShowBitmap(window.context, 0, 0, window.width, window.height, ramen4);//задний фон
} // closet: x (1100 - ww)

void LocFive() {
	ShowBitmap(window.context, 0, 0, window.width, window.height, shelf);
	if (shelfNum == 1) {
		ShowBitmap(window.context, window.width / 4, window.height / 5, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 2, window.height / 5, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 4 * 3, window.height / 5, foodSize, foodSize, ball);
	}
	else if (shelfNum == 2) {
		ShowBitmap(window.context, window.width / 4, window.height / 2, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 2, window.height / 2, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 4 * 3, window.height / 2, foodSize, foodSize, ball);
	}
	else if (shelfNum == 3) {
		ShowBitmap(window.context, window.width / 4, window.height / 5 * 4, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 2, window.height / 5 * 4, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 4 * 3, window.height / 5 * 4, foodSize, foodSize, ball);
	}
} // brokenstove

void LocSix() {
	ShowBitmap(window.context, 0, 0, window.width, window.height, fridge);
	if (fridgeNum == 1) {
		ShowBitmap(window.context, window.width / 4, window.height / 5, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 2, window.height / 5, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 4 * 3, window.height / 5, foodSize, foodSize, ball);
	}
	else if (fridgeNum == 2) {
		ShowBitmap(window.context, window.width / 4, window.height / 2, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 2, window.height / 2, foodSize, foodSize, ball);
		ShowBitmap(window.context, window.width / 4 * 3, window.height / 2, foodSize, foodSize, ball);
	}
} // utility room

void switchCamera() {
	for (int i = 49; i < 55; i++) {
		if (GetAsyncKeyState(i)) {
			camera = i - 48;
		}
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	InitWindow();
	InitGame();
	setText();
	ShowCursor(true);
	while (!(GetAsyncKeyState(VK_ESCAPE))) {
		switch (camera) {
		case 1: {
			LocOne();
			break;
		}
		case 2: {
			LocTwo();
			break;
		}
		case 3: {
			LocThree();
			break;
		}
		case 4: {
			LocFour();
			break;
		}
		case 5: {
			LocFive();
			break;
		}
		case 6: {
			LocSix();
			break;
		}
		}
		switchCamera();
		BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
		Sleep(16);//ждем 16 милисекунд (1/количество кадров в секунду)
	}
}
