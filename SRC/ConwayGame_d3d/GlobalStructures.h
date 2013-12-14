//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
#include <xnamath.h>

struct SimpleVertex
{
  SimpleVertex();
  SimpleVertex(XMFLOAT3 Pos, XMFLOAT4 Color)
  {
    this->Pos   = Pos;
    this->Color = Color;
  };

  XMFLOAT3 Pos;
  XMFLOAT4 Color;
};

struct ConstantBuffer
{
  XMMATRIX mWorld;
  XMMATRIX mView;
  XMMATRIX mProjection;
};