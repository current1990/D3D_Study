#ifndef _D3DUTILITY_H_
#define _D3DUTILITY_H_

#ifdef DLL_IMPLEMENT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


#define IN
#define OUT

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

namespace d3d
{
	//Functions
	//D3DInit creates an IDirect3DDevice9 object and returns via pointer
	DLL_API bool D3DInit(HINSTANCE hInstance,
						 int width,
						 int height,
						 bool windowed,
						 D3DDEVTYPE devtype,
						 LPCWSTR AppName,
						 IDirect3DDevice9 **dev);


	//Message Loop, pass a render function pointer in order to render
	DLL_API int D3DMsgLoop(bool(*RenderPtr)(float deltaTime));

	//We'll take control of the messages
	DLL_API LRESULT CALLBACK D3DWndProc(HWND hWnd,
										UINT msg,
										WPARAM wParam,
										LPARAM lParam);

	//Initialize a material9 struct and return
	DLL_API D3DMATERIAL9 D3DInitMtrl(D3DXCOLOR ambient,
									 D3DXCOLOR diffuse,
									 D3DXCOLOR specular,
									 D3DXCOLOR emissive,
									 float power);
	
	//Initialization for three types of simple lights
	//Initialize a PointLight
	DLL_API D3DLIGHT9 D3DInitPointLight(D3DXVECTOR3 *pos,
										D3DXCOLOR *color);
	
	//Initialize a DirectionalLight
	DLL_API D3DLIGHT9 D3DInitDirectionalLight(D3DXVECTOR3 *direction,
											  D3DXCOLOR *color);

	//Initialize a SpotLight
	DLL_API D3DLIGHT9 D3DInitSpotLight(D3DXVECTOR3 *pos,
									   D3DXVECTOR3 *direction,
									   D3DXCOLOR *color);

	//Release a COM Object, function templates are not exported to dll
	template <class T> void Release(T *t)
	{
		if( t )
		{
			t->Release();
			t = 0;
		}
	}

	template <class T> void Delete(T *t)
	{
		if( t )
		{
			delete t;
		}
	}

	//constants
	/* Color constants defines the most often used colors in form of D3DCOLOR structure*/
	DLL_API extern const D3DXCOLOR WHITE;
	DLL_API extern const D3DXCOLOR BLACK;
	DLL_API extern const D3DXCOLOR RED;
	DLL_API extern const D3DXCOLOR GREEN;
	DLL_API extern const D3DXCOLOR BLUE;
	DLL_API extern const D3DXCOLOR YELLOW;
	DLL_API extern const D3DXCOLOR CYAN;
	DLL_API extern const D3DXCOLOR MAGENTA;

	/* Material constants defines some often used materials in form of D3DMATERIAL9 structure*/
	DLL_API extern const D3DMATERIAL9 WHITE_MTRL;
	DLL_API extern const D3DMATERIAL9 RED_MTRL;
	DLL_API extern const D3DMATERIAL9 GREEN_MTRL;
	DLL_API extern const D3DMATERIAL9 BLUE_MTRL;
	DLL_API extern const D3DMATERIAL9 YELLOW_MTRL;
}

#endif /*D3DUtility*/