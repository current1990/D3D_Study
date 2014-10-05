#include "lighting.h"

using namespace d3d;

//define vertex format
struct Vertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

bool Setup()
{
	//Setup Geometry
	pD3DDEV->CreateVertexBuffer(50 * 2 * sizeof(Vertex),
								D3DUSAGE_WRITEONLY,
								Vertex::FVF,
								D3DPOOL_MANAGED,
								&pVB,
								0);
	Vertex *vertices;
	pVB->Lock(0, 0, (void**)&vertices, 0);

	for( int i = 0; i < 50; ++i )
	{
		float theta = (D3DX_PI * 2 * i / 49);

		vertices[2 * i + 0].pos = D3DXVECTOR3(sinf(theta), -1.0, cosf(theta));
		vertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0, cosf(theta));
		vertices[2 * i + 1].pos = D3DXVECTOR3(sinf(theta), 1.0, cosf(theta));
		vertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0, cosf(theta));
	}
	pVB->Unlock();

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
	light.Direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	light.Range = 1000.0f;

	pD3DDEV->SetLight(0, &light);
	pD3DDEV->LightEnable(0, true);

	//Set Render States
	pD3DDEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDEV->SetRenderState(D3DRS_ZENABLE, true);
	pD3DDEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DDEV->SetRenderState(D3DRS_SPECULARENABLE, true);
	pD3DDEV->SetRenderState(D3DRS_LIGHTING, true);
	pD3DDEV->SetRenderState(D3DRS_AMBIENT, 0x00030303);

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
		D3DXMatrixRotationX(&rotate, 2.5f * x);
		x += DeltaTime;
		if( x >= D3DX_PI * 2 )
			x = 0.0f;

		pD3DDEV->SetTransform(D3DTS_WORLD, &rotate);
		//Set source
		pD3DDEV->SetStreamSource(0, pVB, 0, sizeof(Vertex));
		pD3DDEV->SetFVF(Vertex::FVF);
		pD3DDEV->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 100);
		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);

	return true;
}

void Cleanup()
{
	Release<IDirect3DDevice9>(pD3DDEV);
	Release<IDirect3DVertexBuffer9>(pVB);
}