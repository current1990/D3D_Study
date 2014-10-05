#include "movingspot.h"

using namespace d3d;

bool Setup()
{
	//Setup Geometry
	D3DXCreateTeapot(pD3DDEV, &pMsh[0], 0);
	D3DXCreateTeapot(pD3DDEV, &pMsh[1], 0);
	D3DXCreateTeapot(pD3DDEV, &pMsh[2], 0);
	D3DXCreateTeapot(pD3DDEV, &pMsh[3], 0);

	//Set Material
	pD3DDEV->SetMaterial(&WHITE_MTRL);

	//Set camera
	D3DXMATRIX cam;
	D3DXVECTOR3 EyeAt(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &EyeAt, &LookAt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &cam);

	//Set Projection
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2.0f,
							   800.0f / 600.0f,
							   1.0f, 100.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

	//Set Render State
	pD3DDEV->SetRenderState(D3DRS_LIGHTING, true);
	pD3DDEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DDEV->SetRenderState(D3DRS_SPECULARENABLE, true);

	return true;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		D3DXMATRIX rotate;
		static float y = 0.0f;
		D3DXMatrixRotationY(&rotate, y);
		y += DeltaTime;
		if( y >= D3DX_PI * 2 )
			y = 0.0f;

		pD3DDEV->SetTransform(D3DTS_WORLD, &rotate);
		//Set Light
		D3DLIGHT9 light;
		static float posx = 0.0f;
		static float posy = 0.0f;
		D3DXVECTOR3 dir = D3DXVECTOR3(posx, posy, 1.0f);

		light.Type = D3DLIGHT_SPOT;
		light.Ambient = WHITE * 0.3f;
		light.Diffuse = RED;
		light.Specular = WHITE * 0.7f;
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Position = D3DXVECTOR3(0.0f, 0.0f, -3.0f);
		light.Falloff = 5.0f;
		light.Range = 100.0f;
		light.Phi = D3DX_PI / 2.5;
		light.Theta = D3DX_PI / 45;
		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			posx -= 0.5f * DeltaTime;
		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			posx += 0.5f * DeltaTime;
		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			posy += 0.5f * DeltaTime;
		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			posy -= 0.5f * DeltaTime;
		light.Direction = dir;

		pD3DDEV->SetLight(0, &light);
		pD3DDEV->LightEnable(0, true);

		D3DXMATRIX mv;
		D3DXMatrixTranslation(&mv, 2.0f, 0.0f, 0.0f);
		pD3DDEV->SetTransform(D3DTS_WORLD, &mv);
		pMsh[0]->DrawSubset(0);

		D3DXMatrixTranslation(&mv, -2.0f, 0.0f, 0.0f);
		pD3DDEV->SetTransform(D3DTS_WORLD, &mv);
		pMsh[1]->DrawSubset(0);

		D3DXMatrixTranslation(&mv, 0.0f, -2.0f, 0.0f);
		pD3DDEV->SetTransform(D3DTS_WORLD, &mv);
		pMsh[2]->DrawSubset(0);

		D3DXMatrixTranslation(&mv, 0.0f, 2.0f, 0.0f);
		pD3DDEV->SetTransform(D3DTS_WORLD, &mv);
		pMsh[3]->DrawSubset(0);

		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);
	return true;
}

void Cleanup()
{
	Release<IDirect3DDevice9>(pD3DDEV);
	for( int i = 0; i < 4; ++i )
	{
		Release<ID3DXMesh>(pMsh[i]);
	}
}