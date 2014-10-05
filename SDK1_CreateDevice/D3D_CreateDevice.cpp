#include <windows.h>
#include <d3d9.h>
#include <strsafe.h>

#pragma comment(lib, "d3d9.lib")


LPDIRECT3D9			g_pD3D9;
LPDIRECT3DDEVICE9	g_pD3D9dev;

HWND hMain;

bool initWinApp(HINSTANCE hInstance, int iCmdShow);
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT D3DInit(HWND hWnd);
void D3DRender();
void D3DCleanup();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int iCmdShow)
{
	if( !initWinApp(hInstance, iCmdShow) )
	{
		::MessageBox(0, TEXT("initWinApp Failed!"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	if( SUCCEEDED(D3DInit(hMain)) )
	{
		::ShowWindow(hMain, iCmdShow);
		::UpdateWindow(hMain);

		MSG msg;
		while( PeekMessage(&msg, 0, 0, 0, 0 ) )
		{
			if( msg.message == WM_QUIT )
				break;
			msg.message = WM_PAINT;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	D3DCleanup();
	UnregisterClass(L"D3DInit", hInstance);
	return 0;


}

bool initWinApp(HINSTANCE hInstance, int iCmdshow)
{
	WNDCLASS wc;
	wc.style		= CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc	= MsgProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInstance;
	wc.hIcon		= ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor		= ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground= static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName	= 0;
	wc.lpszClassName= TEXT("D3DInit");

	if( !RegisterClass(&wc) )
	{
		::MessageBox(0, TEXT("RegisterClass Failed!"), TEXT("ERROR"), MB_OK);
		return false;
	}

	if( (hMain = CreateWindow(TEXT("D3DInit"), TEXT("D3DInit"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL)) == NULL)
	{
		::MessageBox(0, TEXT("CreateWindow Failed!"), TEXT("ERROR"), MB_OK);
		return false;
	}

	return true;
}

HRESULT D3DInit(HWND hWnd)
{
	//acquire pointer to IDirect3D9
	if( (g_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION)) == NULL )
	{
		::MessageBox(0, TEXT("Direct3DCreate Failed!"), TEXT("ERROR"), MB_OK);
		return E_FAIL;
	}

	//Check Device Capabilities, here we check the hardware TnL support
	D3DCAPS9 caps;
	g_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	int hardwareTnL = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		hardwareTnL = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		hardwareTnL = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//setup D3DPRESENT_PARAMETERS which defines the behavior of the D3D app 
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth			= 800;
	d3dpp.BackBufferHeight			= 600;
	d3dpp.BackBufferCount			= 2;
	d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow				= hMain;
	d3dpp.Windowed					= 1;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
	d3dpp.Flags						= 0;
	d3dpp.FullScreen_RefreshRateInHz= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

	//Create D3D device
	if( FAILED(g_pD3D9->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									hardwareTnL,
									&d3dpp,
									&g_pD3D9dev)) )
	{
		MessageBox(0, TEXT("CreateDevice Failed"), TEXT("ERROR"), MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		D3DRender();
		ValidateRect(hWnd, NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void D3DRender()
{
	//Set BackBuffer
	g_pD3D9dev->Clear(0, NULL, D3DCLEAR_TARGET,D3DCOLOR_XRGB( 0, 255, 0 ), 1.0f, 0 );

	//Render in this section
	if( SUCCEEDED(g_pD3D9dev->BeginScene()) )
	{
		g_pD3D9dev->EndScene();
	}

	//Present BackBuffer
	g_pD3D9dev->Present(NULL, NULL, NULL, NULL);
}

void D3DCleanup()
{
	if( g_pD3D9dev != NULL )
	{
		g_pD3D9dev->Release();
	}
	if( g_pD3D9 != NULL )
	{
		g_pD3D9->Release();
	}
}