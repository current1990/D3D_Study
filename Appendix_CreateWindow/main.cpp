#include <windows.h>

//Main Window handler, which we are going to create in the application
HWND hMain = NULL;

//Initialize a windows application
bool InitWindowsApp(HINSTANCE hInstance, int iShow);

//Window process which handles event the windows receives
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//The message loop
int run();

//Entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int iCmdShow)
{
	//if initialization fails, pop a msgbox showing error msg and then exit
	if (!InitWindowsApp(hInstance, iCmdShow))
	{
		::MessageBox(0, TEXT("Init Failed"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	//Enter message loop
	//the program flow is passed to function run, tricky move in game engines
	return run();
}

bool InitWindowsApp(HINSTANCE hInstance, int iCmdShow)
{
	//wndclass contains characteristics of the new window
	WNDCLASS wc;

	wc.style		= CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc	= WndProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInstance;
	wc.hIcon		= ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor		= ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground= static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName	= 0;
	wc.lpszClassName= TEXT("Hello");

	//Register the window class so we can create windows with suck characteristics
	if( !RegisterClass(&wc) )
	{
		::MessageBox(0, TEXT("RegisterClass Failed!"), TEXT("Error"), MB_OK);
		return false;
	}

	//We have our windows class registered, now we can create our window via CreateWindow(),
	//The function returns handler of the new window, in Init of the new winApp, we store it in
	//MainWindowHandler, after that, we can reference the main windows via the handler

	hMain = ::CreateWindow(TEXT("Hello"),
						   TEXT("Hello"),
						   WS_OVERLAPPEDWINDOW,
						   CW_USEDEFAULT,
						   CW_USEDEFAULT,
						   CW_USEDEFAULT,
						   CW_USEDEFAULT,
						   NULL,
						   NULL,
						   hInstance,
						   NULL);
	if( hMain == NULL )
	{
		::MessageBox(NULL, TEXT("CreateWindow Failed"), TEXT("ERROR"), MB_OK);
		return false;
	}

	//Finally the new windows is created, we can now call ShowWindow and UpdateWindow
	//to Show and Update the window via passing the window handler to the functions
	::ShowWindow(hMain, iCmdShow); 
	::UpdateWindow(hMain);

	//Init finished
	return true;
}

//msg loop
/*
int run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	//GetMessage returns 0 on WM_QUIT
	while( ::GetMessage(&msg, 0, 0, 0) )
	{
		//Translate the msg and dispatch it to WndProc
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}
*/

int run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while( true )
	{
		if( PeekMessage(&msg, 0, 0, 0, 0) )
		{
			if( msg.message == WM_QUIT )
			{
				break;
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		::MessageBox(0, TEXT("Outside of msg"), TEXT("Test"), MB_OK);
		//TODO: Game Code
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		::MessageBox(0, TEXT("Hello, World!"), TEXT("Hello"), MB_OK);
		return 0;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
		{
			::DestroyWindow(hMain);
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}