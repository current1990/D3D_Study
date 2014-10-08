#ifndef _CUBE_HPP_
#define _CUBE_HPP_

#include "d3dUtility.h"
#include "vertex.h"

#pragma comment(lib, "d3dUtility.lib")

class cube
{
private:
	IDirect3DDevice9 *_dev;
	IDirect3DVertexBuffer9 *_pVB;
	IDirect3DIndexBuffer9 *_pIB;
public:
	cube(IDirect3DDevice9 *dev);
	~cube();

	void Draw(D3DXMATRIX *World, D3DMATERIAL9 *mtrl, IDirect3DTexture9 *Tex);
};

cube::cube(IDirect3DDevice9 *dev) : _dev(dev)
{
	_dev->CreateVertexBuffer(24 * sizeof(Vertex),
							 D3DUSAGE_WRITEONLY,
							 Vertex::FVF,
							 D3DPOOL_MANAGED,
							 &_pVB,
							 NULL);

	Vertex *vertices;
	_pVB->Lock(0, 0, (void**)&vertices, 0);
	
	// fill in the front face vertex data
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	// fill in the back face vertex data
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[5] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// fill in the top face vertex data
	vertices[8]  = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[9]  = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[10] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	vertices[11] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the bottom face vertex data
	vertices[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[13] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[14] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	vertices[15] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the left face vertex data
	vertices[16] = Vertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[17] = Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[18] = Vertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// fill in the right face vertex data
	vertices[20] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[21] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[22] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertices[23] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	_pVB->Unlock();
	
	dev->CreateIndexBuffer(36 * sizeof(WORD),
						   D3DUSAGE_WRITEONLY,
						   D3DFMT_INDEX16,
						   D3DPOOL_MANAGED,
						   &_pIB,
						   NULL);

	WORD *indeces;
	_pIB->Lock(0, 0, (void**)&indeces, 0);

	// fill in the front face index data
	indeces[0] = 0; indeces[1] = 1; indeces[2] = 2;
	indeces[3] = 0; indeces[4] = 2; indeces[5] = 3;

	// fill in the back face index data
	indeces[6] = 4; indeces[7]  = 5; indeces[8]  = 6;
	indeces[9] = 4; indeces[10] = 6; indeces[11] = 7;

	// fill in the top face index data
	indeces[12] = 8; indeces[13] =  9; indeces[14] = 10;
	indeces[15] = 8; indeces[16] = 10; indeces[17] = 11;

	// fill in the bottom face index data
	indeces[18] = 12; indeces[19] = 13; indeces[20] = 14;
	indeces[21] = 12; indeces[22] = 14; indeces[23] = 15;

	// fill in the left face index data
	indeces[24] = 16; indeces[25] = 17; indeces[26] = 18;
	indeces[27] = 16; indeces[28] = 18; indeces[29] = 19;

	// fill in the right face index data
	indeces[30] = 20; indeces[31] = 21; indeces[32] = 22;
	indeces[33] = 20; indeces[34] = 22; indeces[35] = 23;

	_pIB->Unlock();
}

cube::~cube()
{
	if( _pVB ) { _pVB->Release(); _pVB = NULL; }
	if( _pIB ) { _pIB->Release(); _pIB = NULL; }
}

void cube::Draw(D3DXMATRIX *World, D3DMATERIAL9 *mtrl, IDirect3DTexture9 *Tex)
{
	if( World )
		_dev->SetTransform(D3DTS_WORLD, World);
	if( mtrl )
		_dev->SetMaterial(mtrl);
	if( Tex )
		_dev->SetTexture(0, Tex);

	_dev->SetStreamSource(0, _pVB, 0, sizeof(Vertex));
	_dev->SetFVF(Vertex::FVF);
	_dev->SetIndices(_pIB);
	_dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}

#endif