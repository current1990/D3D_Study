#include "rotateLight.h"

using namespace d3d;

bool Setup()
{
	//Set Teapot
	D3DXCreateTeapot(pD3DDEV, &pMsh, 0);

	//Set Camera
	D3DXMATRIX cam;
	D3DXVECTOR3 EyeAt(0.0f, 2.0f, -3.0f);
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

	//Set Material
	pD3DDEV->SetMaterial(&WHITE_MTRL);

	//SetRenderState
	pD3DDEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDEV->SetRenderState(D3DRS_LIGHTING, true);
	pD3DDEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DDEV->SetRenderState(D3DRS_SPECULARENABLE, true);

	return true;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		//Set Light position
		float posinit = 0.0f;
		D3DXMATRIX position = (&posinit);
		position._11 = 3.0f;
		position._22 = 3.0f;
		position._33 = 3.0f;
		D3DXMATRIX rotate;

		static float y = 0.0f;
		D3DXMatrixRotationY(&rotate, y);
		y += DeltaTime;

	//	if( y >= D3DX_PI * 2.0f )
	//		y = 0.0f;


		//Set Light
		D3DLIGHT9 lt;
		::ZeroMemory(&lt, sizeof(lt));
		lt.Type = D3DLIGHT_POINT;
		lt.Ambient = WHITE * 0.3f;
		lt.Diffuse = RED;
		lt.Specular = WHITE * 0.7f;
		lt.Range = 100.0f;
		lt.Attenuation0 = 1.0f;
		lt.Attenuation1 = 0.0f;
		lt.Attenuation2 = 0.0f;

		lt.Position = D3DXVECTOR3(position._11 * sinf(y), position._22 , position._33* cosf(y));

		pD3DDEV->SetLight(0, &lt);
		pD3DDEV->LightEnable(0, true);
		pMsh->DrawSubset(0);

		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);
	return true;
}

void Cleanup()
{
	Release<IDirect3DDevice9>(pD3DDEV);
	Release<ID3DXMesh>(pMsh);
}