#include "matrices.h"

using namespace d3d;

//define Vertex format
struct Vertex
{
	Vertex(float x, float y, float z, DWORD color)
	{
		_x = x; _y = y; _z = z;
		_color = color;
	}
	float _x, _y, _z;
	DWORD _color;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

/*Setup
 * 1. Vertex Buffer
 * 2. Index Buffer
 * 3. Transform Matrices
 * 4. Render Stats */

bool Setup()
{
	//Create Vertex Buffer and Index Buffer
	pD3DDEV->CreateVertexBuffer(3 * sizeof(Vertex),
								D3DUSAGE_WRITEONLY,
								Vertex::FVF,
								D3DPOOL_MANAGED,
								&pVB,
								0);
	
	pD3DDEV->CreateIndexBuffer(3 * sizeof(DWORD),
							   D3DUSAGE_WRITEONLY,
							   D3DFMT_INDEX16,
							   D3DPOOL_MANAGED,
							   &pIB,
							   0);
	
	//fill the vertex buffer
	Vertex *vertices;
	pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(0.0f, 1.73f, 0.0f, 0xffff0000);
	vertices[1] = Vertex(-1.0f, 0.0f, 0.0f, 0xff00ff00);
	vertices[2] = Vertex(1.0f, 0.0f, 0.0f, 0xff0000ff);

	pVB->Unlock();

	//fill the index buffer

	WORD *indices = 0;
//	WORD indices[3];
	pIB->Lock(0, 0, (void**)&indices, 0);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	pIB->Unlock();

	//Setup Camera and View transform
	D3DXMATRIX cam;
	D3DXVECTOR3 EyeAt(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&cam, &EyeAt, &LookAt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &cam);

	//Set Projection Transform
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
							   D3DX_PI * 0.5f,
							   800.0f/ 600.0f,
							   1.0f,
							   100.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &proj);

	//Disable Back-culling
	pD3DDEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDEV->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}

//Cleanup
void CleanUp()
{
	Release<IDirect3DIndexBuffer9>(pIB);
	Release<IDirect3DVertexBuffer9>(pVB);
}

//Render
bool Render(float DeltaTime)
{
	//Set World Transform, since the triangle rotates every frame, we calculate the world matrix in Render()
	D3DXMATRIX ry;

	static float y = 0.0f;
	D3DXMatrixRotationY(&ry, y);
	y += DeltaTime;

	if( y >= 6.28f )
		y = 0.0f;

	pD3DDEV->SetTransform(D3DTS_WORLD, &ry);

	pD3DDEV->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		pD3DDEV->SetStreamSource(0, pVB, 0, sizeof(Vertex));
		pD3DDEV->SetIndices(pIB);
		pD3DDEV->SetFVF(Vertex::FVF);
		pD3DDEV->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);

		pD3DDEV->EndScene();
		pD3DDEV->Present(0, 0, 0, 0);
	}

	return true;
}