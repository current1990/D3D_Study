#include "d3dxcreate.h"

using namespace d3d;

IDirect3DDevice9 *pD3DDEV;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int iCmdShow)
{
	if (!D3DInit(hInstance, 800, 600, TRUE, D3DDEVTYPE_HAL, TEXT("D3DXCreate"), &pD3DDEV))
	{
		::MessageBox(0, TEXT("D3DInit Failure"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, TEXT("Setup Failure"), TEXT("ERROR"), MB_OK);
		return 0;
	}

	D3DMsgLoop(Render);

	Cleanup();

	return 0;
}