
namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		WINMODE			eWinMode; //! ��� â��带 ����� ���ΰ�. WINMODE_FULL ��üȭ��, WINMODE_WIN â ȭ��
		HWND			hWnd; //! ��� â�ΰ�
		unsigned int	iBackBufferSizeX; //! ����ۿ��� �� â�� ���λ�����
		unsigned int	iBackBufferSizeY; //! ����ۿ��� �� â�� ���λ�����

	}GRAPHIC_DESC;



	typedef struct tagPassDesc
	{
		ID3D11VertexShader* VertexShader = { nullptr };
		ID3D11PixelShader* PixelShader = { nullptr };
		ID3D11InputLayout* layout = { nullptr };
	}PASS_DESC;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC   Elements[iNumElements];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;
}


