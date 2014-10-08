#include "texquad.h"

using namespace d3d;

struct Vertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	Vertex(float x, float y, float z,
		   float nx, float ny, float nz,
		   float u, float v) :
		   _x(x), _y(y), _z(z), _nx(nx), _ny(ny), _nz(nz), _u(u), _v(v)
	{
	}
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

bool Setup()
{
	::MessageBox(0, TEXT("Press \'CBMW\' for sampler address modes. Press \'ALP\' for sampling modes. Press G for grid texture, Press for DirectX Logo"),
				 TEXT("Tips"), MB_OK);
	//Set Vertex Buffer
	pD3DDEV->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &pVB, 0);
	Vertex *v;
	pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(1.5f, 1.5f, 1.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f);
	v[1] = Vertex(1.5f, -1.5f, 1.0f, 0.0f, 0.0f, 1.0f, 3.0f, 3.0f);
	v[2] = Vertex(-1.5f, -1.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 3.0f);

	v[3] = Vertex(1.5f, 1.5f, 1.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f);
	v[4] = Vertex(-1.5f, -1.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 3.0f);
	v[5] = Vertex(-1.5f, 1.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

	pVB->Unlock();

	//Set Address mode

	pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	//Load Texture
	D3DXCreateTextureFromFile(pD3DDEV, TEXT("grid.bmp"), &pTex);

	pD3DDEV->SetTexture(0, pTex);
	pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//Shut down light
	pD3DDEV->SetRenderState(D3DRS_LIGHTING, false);

	//Set Camera
	D3DXMATRIX cam;
	D3DXVECTOR3 EyeAt(0.0f, 0.0f, -2.0f);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &EyeAt, &LookAt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &cam);

	//Set Projection
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2,
							   800.0f / 600.0f,
							   1.0f, 1000.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool Render(float DeltaTime)
{
	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		//Get Sampler address mode
		if( ::GetAsyncKeyState('C') & 0x8000f )
		{
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}
		if( ::GetAsyncKeyState('B') & 0x8000f )
		{
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
		}
		if( ::GetAsyncKeyState('W') & 0x8000f )
		{
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}
		if( ::GetAsyncKeyState('M') & 0x8000f )
		{
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			pD3DDEV->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		}

		//Get Sampling mode
		if( ::GetAsyncKeyState('A') & 0x8000f )
		{
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
		}
		if( ::GetAsyncKeyState('L') & 0x8000f )
		{
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
		if( ::GetAsyncKeyState('P') & 0x8000f )
		{
			pD3DDEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			pD3DDEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		}

		//Set Texture
		if( ::GetAsyncKeyState('G') & 0x8000f )
		{
			Release<IDirect3DTexture9>(pTex);
			D3DXCreateTextureFromFile(pD3DDEV, TEXT("grid.bmp"), &pTex);
			pD3DDEV->SetTexture(0, pTex);
		}
		if( ::GetAsyncKeyState('D') & 0x8000f )
		{
			Release<IDirect3DTexture9>(pTex);
			D3DXCreateTextureFromFile(pD3DDEV, TEXT("dx5_logo.bmp"), &pTex);
			pD3DDEV->SetTexture(0, pTex);
		}

		pD3DDEV->SetStreamSource(0, pVB, 0, sizeof(Vertex));
		pD3DDEV->SetFVF(Vertex::FVF);
		pD3DDEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		pD3DDEV->EndScene();
	}
	pD3DDEV->Present(0, 0, 0, 0);

	return true;
}

void Cleanup()
{
	Release<IDirect3DDevice9>(pD3DDEV);
	Release<IDirect3DTexture9>(pTex);
	Release<IDirect3DVertexBuffer9>(pVB);
}