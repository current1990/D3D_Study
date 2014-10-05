#include "render.h"

using namespace d3d;

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z)
	{
		_x = x, _y = y, _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ;

bool Setup()
{
	//Create both Vertex buffer & index buffer
	pD3DDEV->CreateVertexBuffer(9 * sizeof(Vertex),
							D3DUSAGE_WRITEONLY,
							Vertex::FVF,
							D3DPOOL_MANAGED,
							&pVB,
							NULL);

	pD3DDEV->CreateIndexBuffer(36 * sizeof(short),
						   D3DUSAGE_WRITEONLY,
						   D3DFMT_INDEX16,
						   D3DPOOL_MANAGED,
						   &pIB,
						   NULL);

	//Fill Vertex Buffer
	Vertex *vertices;
	pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(1.0f, 1.0f, 1.0f);
	vertices[1] = Vertex(2.0f, -2.0f, 2.0f);
	vertices[2] = Vertex(2.0f, -2.0f, 0.0f);
	vertices[3] = Vertex(0.0f, -2.0f, 2.0f);
	vertices[4] = Vertex(0.0f, -2.0f, 0.0f);
	vertices[5] = Vertex(2.0f, 0.0f, 2.0f);
	vertices[6] = Vertex(2.0f, 0.0f, 0.0f);
	vertices[7] = Vertex(0.0f, 0.0f, 0.0f);
	vertices[8] = Vertex(0.0f, 0.0f, 2.0f);

	pVB->Unlock();

	//Define the cube via Index

	WORD *indices;
	pIB->Lock(0, 0, (void**)&indices, 0);

	//front
	indices[0] = 2; indices[1] = 3; indices[2] = 1;
	indices[3] = 2; indices[4] = 4; indices[5] = 3;

	//back
	indices[6] = 5; indices[7] = 8; indices[8] = 7;
	indices[9] = 5; indices[10] = 7; indices[11] = 6;

	//left
	indices[12] = 4; indices[13] = 8; indices[14] = 3;
	indices[15] = 4; indices[16] = 7; indices[17] = 8;

	//right
	indices[18] = 1; indices[19] = 6; indices[20] = 2;
	indices[21] = 1; indices[22] = 5; indices[23] = 6;

	//top
	indices[24] = 1; indices[25] = 3; indices[26] = 8;
	indices[27] = 1; indices[28] = 8; indices[29] = 5;

	//bottom
	indices[30] = 2; indices[31] = 7; indices[32] = 4;
	indices[33] = 2; indices[34] = 6; indices[35] = 7;

	//Setup camera, both position and aiming
	D3DXVECTOR3 campos(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 camtgt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	/*Set Variable transforms*/

	//View Space Transform
	D3DXMATRIX ViewSpace;
	D3DXMatrixLookAtLH(&ViewSpace, &campos, &camtgt, &up);
	pD3DDEV->SetTransform(D3DTS_VIEW, &ViewSpace);

	//Projection Transform
	D3DXMATRIX Projection;
	D3DXMatrixPerspectiveFovLH(&Projection,
							   D3DX_PI * 0.5f, //pi/2 -> 90
							   800.0f / 600.0f,
							   1.0f,
							   1000.0f);
	pD3DDEV->SetTransform(D3DTS_PROJECTION, &Projection);

	//Set Wire-frame mode
	pD3DDEV->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pD3DDEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return true;
}
void CleanUp()
{
	Release<IDirect3DVertexBuffer9>(pVB);
	Release<IDirect3DIndexBuffer9>(pIB);
}

bool Render(float DeltaTime)
{
	//Spin Matrix
	D3DXMATRIX rotatex, rotatey;
	D3DXMATRIX mov_center;

	//Move the cube to space center
	D3DXMatrixTranslation(&mov_center, -1.0f, 1.0f, -1.0f);

	//pi/6 rad on X axis
	D3DXMatrixRotationX(&rotatex, D3DX_PI / 6.0f);


	//increment on Y axis
	static float ry = 0.0f;
	D3DXMatrixRotationY(&rotatey, ry);
	ry += DeltaTime;
	if( ry >= 6.28f )
		ry = 0.0f;

	D3DXMATRIX rotate = mov_center * rotatex * rotatey;
	pD3DDEV->SetTransform(D3DTS_WORLD, &rotate);

	pD3DDEV->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( SUCCEEDED(pD3DDEV->BeginScene()) )
	{
		pD3DDEV->SetStreamSource(0, pVB, 0, sizeof(Vertex));
		pD3DDEV->SetIndices(pIB);
		pD3DDEV->SetFVF(Vertex::FVF);
		pD3DDEV->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 1, 8, 0, 12);
		//rendering
		pD3DDEV->EndScene();
	}

	pD3DDEV->Present(NULL, NULL, NULL, NULL);

	return true;
}
