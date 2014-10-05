#include "lighting.h"

using namespace d3d;

//define vertex format
struct Vertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	static const DWORD FVF;
	Vertex(D3DXVECTOR3 &pos, D3DXVECTOR3 &normal);
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

bool Setup()
{
	//Setup Geometry
	D3DXCreateTorus(pD3DDEV, 0.5f, 2.1f, 25, 50, &pMsh1, NULL);
	D3DXCreateTorus(pD3DDEV, 0.5f, 2.1f, 25, 50, &pMsh2, NULL);

	//Setup Material
	pD3DDEV->SetMaterial(&WHITE_MTRL);
	
	//Setup Camera
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

	//Setup Light
	D3DLIGHT9 light;
	light.Ambient = WHITE * 0.3f;
	light.Diffuse = RED;
	light.Specular = WHITE * 0.7f;
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(0.0f, 1.0f, 1.0f);

	pD3DDEV->SetLight(0, &light);
	pD3DDEV->LightEnable(0, true);

	//Set Render States
	pD3DDEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DDEV->SetRenderState(D3DRS_SPECULARENABLE, true);
	pD3DDEV->SetRenderState(D3DRS_LIGHTING, true);

	return true;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 1);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		//TODO: Rendering
		//Rotation matrix
		D3DXMATRIX rotate;
		static float x = 0.0f;
		D3DXMatrixRotationX(&rotate, x);
		x += DeltaTime;
		if( x >= D3DX_PI * 2 )
			x = 0.0f;
		//move pMsh1 to left
		D3DXMATRIX mesh1;
		D3DXMatrixTranslation(&mesh1, -1.0f, 0.0f, 0.0f);
		mesh1 = rotate * mesh1;
		
		pD3DDEV->SetTransform(D3DTS_WORLD, &mesh1);
		pMsh1->DrawSubset(0);

		//move pMsh2 to right
		D3DXMATRIX mesh2;
		D3DXMatrixTranslation(&mesh2, 1.0f, 0.0f, 0.0f);
		D3DXMATRIX mesh2r;
		D3DXMatrixRotationX(&mesh2r, D3DX_PI / 2.0f);
		mesh2 = mesh2 * mesh2r * rotate;
		pD3DDEV->SetTransform(D3DTS_WORLD, &mesh2);
		pMsh2->DrawSubset(0);

		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);

	return true;
}

void Cleanup()
{
	Release<IDirect3DDevice9>(pD3DDEV);
	Release<ID3DXMesh>(pMsh1);
	Release<ID3DXMesh>(pMsh2);
}