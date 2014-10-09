#ifndef _MTRLBLEND_HPP_
#define _MTRLBLEND_HPP_

#include "d3dUtility.h"

#pragma comment(lib, "d3dUtility.lib")

extern IDirect3DDevice9 *pD3DDEV;
ID3DXMesh *pMsh;
IDirect3DVertexBuffer9 *pBG;
IDirect3DTexture9 *pTex;
D3DMATERIAL9 teapotmtrl;
D3DMATERIAL9 bgmtrl;

struct Vertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	Vertex(float x, float y, float z,
		   float nx, float ny, float nz,
		   float u, float v) :
		   _x(x), _y(y), _z(z),
		   _nx(nx), _ny(ny), _nz(nz),
		   _u(u), _v(v) {}
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

bool Setup()
{
	//Set background & teapot
	D3DXCreateTeapot(pD3DDEV, &pMsh, 0);
	pD3DDEV->CreateVertexBuffer(6 * sizeof(Vertex), 0,
								Vertex::FVF,
								D3DPOOL_MANAGED,
								&pBG, nullptr);
	Vertex *vertices;
	pBG->Lock(0, 0, (void**)&vertices, 0);
	vertices[0] = Vertex(5.0f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[1] = Vertex(5.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	vertices[2] = Vertex(-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);


	vertices[3] = Vertex(5.0f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[4] = Vertex(-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[5] = Vertex(-5.0f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	pBG->Unlock();

	teapotmtrl = d3d::RED_MTRL;
	teapotmtrl.Diffuse.a = 0.5f;

	bgmtrl = d3d::WHITE_MTRL;

	//Set Light
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = d3d::WHITE * 0.6f;
	light.Diffuse = d3d::WHITE;
	light.Specular = d3d::WHITE * 0.2f;
	light.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);

	pD3DDEV->SetLight(0, &light);
	pD3DDEV->LightEnable(0, true);

	pD3DDEV->SetRenderState(D3DRS_LIGHTING, true);
	pD3DDEV->SetRenderState(D3DRS_SPECULARENABLE, true);
	pD3DDEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	//load texture
	D3DXCreateTextureFromFile(pD3DDEV, TEXT("crate.dds"), &pTex);

	pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//Enable Alpha
	pD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pD3DDEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//Set blending factor
	pD3DDEV->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDEV->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Set Camera
	D3DXMATRIX cam;
	D3DXVECTOR3 EyeAt(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &EyeAt, &LookAt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &cam);

	//Set Projection
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2,
							   800.0f / 600.0f,
							   1.0f, 100.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		if( ::GetAsyncKeyState('W') & 0x8000f )
			teapotmtrl.Diffuse.a += 0.01f;
		if( ::GetAsyncKeyState('S') & 0x8000f )
			teapotmtrl.Diffuse.a -= 0.01f;

		if( teapotmtrl.Diffuse.a >= 1.0f )
			teapotmtrl.Diffuse.a = 1.0f;
		if( teapotmtrl.Diffuse.a <= 0.0f )
			teapotmtrl.Diffuse.a = 0.0f;

		//Move the teapot
		D3DXMATRIX mov;
		::ZeroMemory(&mov, sizeof(mov));
		static float x = 0.0f;
		static float y = 0.0f;
		if( GetAsyncKeyState(VK_UP) & 0x8000f )
			y += DeltaTime;
		if( GetAsyncKeyState(VK_DOWN) & 0x8000f )
			y -= DeltaTime;
		if( GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			x += DeltaTime;
		if( GetAsyncKeyState(VK_LEFT) & 0x8000f )
			x -= DeltaTime;
		D3DXMatrixTranslation(&mov, x, y, 0.0f);

		//Draw BackGround
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		pD3DDEV->SetTransform(D3DTS_WORLD, &world);
		pD3DDEV->SetStreamSource(0, pBG, 0, sizeof(Vertex));
		pD3DDEV->SetFVF(Vertex::FVF);
		pD3DDEV->SetMaterial(&bgmtrl);
		pD3DDEV->SetTexture(0, pTex);
		pD3DDEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		//Draw Teapot
		pD3DDEV->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DXMatrixScaling(&world, 2.0f, 2.0f, 2.0f);
		world = world * mov;
		pD3DDEV->SetTransform(D3DTS_WORLD, &world);
		pD3DDEV->SetMaterial(&teapotmtrl);
		pD3DDEV->SetTexture(0, NULL);
		pMsh->DrawSubset(0);

		pD3DDEV->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DDevice9>(pD3DDEV);
}

#endif