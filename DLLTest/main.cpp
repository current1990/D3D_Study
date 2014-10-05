#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")
using namespace d3d;

LPDIRECT3DDEVICE9 pD3DDEV;

bool Setup() { return true; }
bool Render(float DeltaTime) { return true; }
void CleanUp() {}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int iCmdShow)
{
	D3DCOLOR test = WHITE;
	D3DMATERIAL9 test2 = WHITE_MTRL;
	D3DMATERIAL9 test3 = D3DInitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);
}
