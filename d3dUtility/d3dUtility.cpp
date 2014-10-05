#define DLL_IMPLEMENT

#include "d3dUtility.h"

namespace d3d
{
	//Functions
	//D3DInit create a windows and initializes Direct3D environment in the user space of the windows
	bool D3DInit(HINSTANCE hInstance,
				 int width,
				 int height,
				 bool windowed,
				 D3DDEVTYPE devtype,
				 LPCWSTR AppName,
				 IDirect3DDevice9 **dev)
	{
		//Step 0: Create Application Windows
		WNDCLASS wc;

		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hbrBackground	= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.hCursor			= ::LoadCursor(0, IDI_APPLICATION);
		wc.hIcon			= ::LoadIcon(0, IDC_ARROW);
		wc.hInstance		= hInstance;
		wc.lpfnWndProc		= D3DWndProc;
		wc.lpszClassName	= AppName;
		wc.lpszMenuName		= 0;
		wc.style			= CS_HREDRAW | CS_VREDRAW;

		if( !RegisterClass(&wc) )
		{
			::MessageBox(0, TEXT("RegisterClass Failure"), TEXT("Error"), MB_OK);
			return false;
		}

		HWND hWndMain = 0;
		hWndMain = ::CreateWindow(AppName, AppName,
								  WS_OVERLAPPEDWINDOW,
								  0, 0, width, height,
								  NULL, NULL, hInstance, 0);

		if( !hWndMain )
		{
			::MessageBox(0, TEXT("CreateWindow Failure"), TEXT("Error"), MB_OK);
			return false;
		}

		::ShowWindow(hWndMain, SW_SHOW);
		::UpdateWindow(hWndMain);

		/*Init D3D Environment*/

		HRESULT hr = 0;

	
		//Step 1. Create IDirect3D9 Object
		IDirect3D9 *pD3D9 = 0;
		pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

		if( !pD3D9 )
		{
			::MessageBox(0, TEXT("Direc3DCreate9 Failure"), TEXT("ERROR"), MB_OK);
			return false;
		}


		//Step 2. Check Device Caps, here we just process the hardware TnL
		D3DCAPS9 devcaps;
		pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, devtype, &devcaps);

		int hwTnL = 0;
		if( devcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
			hwTnL = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			hwTnL = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


		//Step 3. Fill the D3DPRESENT_PARAMETERS struct
		D3DPRESENT_PARAMETERS d3dpp;
		d3dpp.BackBufferCount			= 0;
		d3dpp.BackBufferWidth			= width;
		d3dpp.BackBufferHeight			= height;
		d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8;
		d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality		= 0;
		d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow				= hWndMain;
		d3dpp.Windowed					= windowed;
		d3dpp.EnableAutoDepthStencil	= TRUE;
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
		d3dpp.Flags						= 0;
		d3dpp.FullScreen_RefreshRateInHz= D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

		
		//Step 4. Create Device
		hr = pD3D9->CreateDevice(D3DADAPTER_DEFAULT,
								 devtype,
								 hWndMain,
								 hwTnL,
								 &d3dpp,
								 dev);

		if( FAILED(hr) )
		{
			pD3D9->Release();
			::MessageBox(0, TEXT("CreateDevice"), TEXT("Error"), MB_OK);
			return false;
		}

		//Release IDirect3D9 Object
		pD3D9->Release();

		return true;
	}

	/* D3DMsgLoop manages the message loop, Since directx applications are usually
	 * not event driven, so the usual windows message mechanism cant meet our needs.
	 * This function intercepts messages if there is any, otherwise, it calls Renderer
	 * to render a frame */
	int D3DMsgLoop(bool(RenderPtr)(float DeltaTime))
	{
		MSG msg;
		::ZeroMemory(&msg, sizeof(MSG));

		static float lastTime = (float)timeGetTime();

		while( msg.message != WM_QUIT )
		{
			if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				float currTime = (float)timeGetTime();
				float DeltaTime = (currTime - lastTime) * 0.001f;

				RenderPtr(DeltaTime);

				lastTime = currTime;
			}
		}
		return msg.wParam;
	}

	/* D3DWndProc is the windows process function registered in D3DInit
	 * D3DMsgLoop intercepts messages and dispatch them, D3DWndProc would
	 * take suck messages and process them appropriately */
	LRESULT CALLBACK D3DWndProc(HWND hWnd,
								UINT msg,
								WPARAM wParam,
								LPARAM lParam)
	{
		switch( msg )
		{
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				::DestroyWindow(hWnd);
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}


	//D3DInitMtrl initialize a D3DMATERIAL9 struct which defines the material for an object
	D3DMATERIAL9 D3DInitMtrl(D3DXCOLOR ambient,
							 D3DXCOLOR diffuse,
							 D3DXCOLOR specular,
							 D3DXCOLOR emissive,
							 float power)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = ambient;
		mtrl.Diffuse = diffuse;
		mtrl.Specular = specular;
		mtrl.Emissive = emissive;
		mtrl.Power = power;
		return mtrl;
	}
	//Initialize a PointLight
	DLL_API D3DLIGHT9 D3DInitPointLight(D3DXVECTOR3 *pos,
										D3DXCOLOR *color)
	{
		D3DLIGHT9 light;
		::ZeroMemory(&light, sizeof(light));
		light.Type		= D3DLIGHT_POINT;
		light.Position	= *pos;
		light.Ambient	= *color * 0.4f;
		light.Diffuse	= *color;
		light.Specular	= *color * 0.6f;
		return light;
	}
	
	//Initialize a DirectionalLight
	DLL_API D3DLIGHT9 D3DInitDirectionalLight(D3DXVECTOR3 *direction,
											  D3DXCOLOR *color)
	{
		D3DLIGHT9 light;
		::ZeroMemory(&light, sizeof(light));
		light.Type		= D3DLIGHT_DIRECTIONAL;
		light.Direction	= *direction;
		light.Ambient	= *color * 0.4f;
		light.Diffuse	= *color;
		light.Specular	= *color * 0.6f;
		return light;
	}

	//Initialize a SpotLight
	DLL_API D3DLIGHT9 D3DInitSpotLight(D3DXVECTOR3 *pos,
									   D3DXVECTOR3 *direction,
									   D3DXCOLOR *color)
	{
		D3DLIGHT9 light;
		::ZeroMemory(&light, sizeof(light));
		light.Type		= D3DLIGHT_SPOT;
		light.Position	= *pos;
		light.Direction	= *direction;
		light.Ambient	= *color * 0.4f;
		light.Diffuse	= *color;
		light.Specular	= *color * 0.6f;
		return light;
	}

	//Constants
	//Common colors
	const D3DXCOLOR	WHITE	(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR	BLACK	(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR	RED		(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR	GREEN	(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR	BLUE	(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR	YELLOW	(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR	CYAN	(D3DCOLOR_XRGB(0, 255, 255));	//ÇàÉ«
	const D3DXCOLOR	MAGENTA	(D3DCOLOR_XRGB(255, 0, 255));	//Æ·ºì
	
	//Common Materials
	const D3DMATERIAL9	WHITE_MTRL	= {WHITE, WHITE, WHITE, BLACK, 8.0f};
	const D3DMATERIAL9	RED_MTRL	= {RED, RED, RED, BLACK, 8.0f};
	const D3DMATERIAL9	GREEN_MTRL	= {GREEN, GREEN, GREEN, BLACK, 8.0f};
	const D3DMATERIAL9	BLUE_MTRL	= {BLUE, BLUE, BLUE, BLACK, 8.0f};
	const D3DMATERIAL9	YELLOW_MTRL	= {YELLOW, YELLOW, YELLOW, BLACK, 8.0f};
}