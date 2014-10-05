#include "Teapot.h"

bool Setup()
{
	//Create a teapot
	if(FAILED(D3DXCreateTeapot(pD3DDEV, &pMesh, 0)))
	{
		::MessageBox(0, TEXT("D3DXCreate Failure"), TEXT("ERROR"), MB_OK);
		return false;
	}

	//Setup Camera
	D3DXMATRIX cam;
	D3DXVECTOR3 EyeAt(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &EyeAt, &LookAt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &cam);

	//Set Projection
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4.0f,
							   800.0f / 600.0f,
							   1.0f, 100.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

	//Set render state
	pD3DDEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}

bool Render(float DeltaTime)
{
	//Set World Transform
	D3DXMATRIX Ry;
	static float y = 0.0f;

	D3DXMatrixRotationY(&Ry, y);

	y += DeltaTime;
	if( y >= D3DX_PI * 2.0f )
		y = 0.0f;

	pD3DDEV->SetTransform(D3DTS_WORLD, &Ry);

	pD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		pMesh->DrawSubset(0);
		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);

	return true;
}

void Cleanup()
{
	Release<ID3DXMesh>(pMesh);
	Release<IDirect3DDevice9>(pD3DDEV);

}