#include "d3dxcreate.h"

ID3DXMesh *xCylinder, *xSphere, *xTorus, *xTeapot;

bool Setup()
{
	D3DXCreateCylinder(pD3DDEV, 1.0f, 2.0f, 3.0f, 20, 15, &xCylinder, 0);
	D3DXCreateSphere(pD3DDEV, 2.0f, 20, 20, &xSphere, 0);
	D3DXCreateTorus(pD3DDEV, 1.0f, 2.0f, 20, 25, &xTorus, 0);
	D3DXCreateTeapot(pD3DDEV, &xTeapot, 0);


	//Setup Camera
	D3DXVECTOR3 EyeAt(0.0f, 0.0f, -7.0f);
	D3DXVECTOR3 LootAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX cam;
	D3DXMatrixLookAtLH(&cam, &EyeAt, &LootAt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &cam);

	//Set Projection
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2.0f,
							   800.0f / 600.0f,
							   1.0f, 1000.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

	pD3DDEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return TRUE;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		D3DXMATRIX rotate;
		static float y = 0.0f;
		D3DXMatrixRotationY(&rotate, y);
		y += DeltaTime;
		if( y >= D3DX_PI * 2.0f )
			y = 0.0f;

		D3DXMATRIX transCylinder;
		D3DXMatrixTranslation(&transCylinder, -3.0f, 3.0f, 0.0f);
		transCylinder = rotate * transCylinder;

		D3DXMATRIX transSphere;
		D3DXMatrixTranslation(&transSphere, 3.0f, 3.0f, 0.0f);
		transSphere = rotate * transSphere;

		D3DXMATRIX transTorus;
		D3DXMatrixTranslation(&transTorus, -3.0f, -3.0f, 0.0f);
		transTorus = rotate * transTorus;

		D3DXMATRIX transTeapot;
		D3DXMatrixTranslation(&transTeapot, 3.0f, -3.0f, 0.0f);
		D3DXMATRIX scaleTeapot;
		D3DXMatrixScaling(&scaleTeapot, 2.0f, 2.0f, 2.0f);
		transTeapot = scaleTeapot * rotate * transTeapot;

		pD3DDEV->SetTransform(D3DTS_WORLD, &transCylinder);
		xCylinder->DrawSubset(0);

		pD3DDEV->SetTransform(D3DTS_WORLD, &transSphere);
		xSphere->DrawSubset(0);

		pD3DDEV->SetTransform(D3DTS_WORLD, &transTeapot);
		xTeapot->DrawSubset(0);
		
		pD3DDEV->SetTransform(D3DTS_WORLD, &transTorus);
		xTorus->DrawSubset(0);
		pD3DDEV->EndScene();
	}

	pD3DDEV->Present(0, 0, 0, 0);
	return TRUE;
}

void Cleanup()
{
}