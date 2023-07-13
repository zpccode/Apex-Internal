#pragma once
#include"../defines/defines.hpp"
#include"../../../sdk/hook/hook.hpp"

class DX11
{
public:
	bool InitWindow();
	bool DeleteWindow();
	bool Initialize();
}; DX11* pDX11 = new DX11();

bool DX11::InitWindow()
{
	pD3D11->WindowClass.cbSize = sizeof(WNDCLASSEX);
	pD3D11->WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	pD3D11->WindowClass.lpfnWndProc = DefWindowProc;
	pD3D11->WindowClass.cbClsExtra = 0;
	pD3D11->WindowClass.cbWndExtra = 0;
	pD3D11->WindowClass.hInstance = spoof_call::GetModuleBase(NULL);
	pD3D11->WindowClass.hIcon = NULL;
	pD3D11->WindowClass.hCursor = NULL;
	pD3D11->WindowClass.hbrBackground = NULL;
	pD3D11->WindowClass.lpszMenuName = NULL;
	pD3D11->WindowClass.lpszClassName = L"OK";
	pD3D11->WindowClass.hIconSm = NULL;
	spoof_call::RegisterWndClas(&pD3D11->WindowClass);
	pD3D11->WindowHwnd = spoof_call::NtCreateWindow(NULL, pD3D11->WindowClass.lpszClassName, L"Apex Window", 
		WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, pD3D11->WindowClass.hInstance, NULL);
	if (pD3D11->WindowHwnd == NULL) { return false; }
	return true;
}

bool DX11::DeleteWindow()
{
	spoof_call::_DeleteWindow(pD3D11->WindowHwnd);
	spoof_call::UnregisterWndClass(pD3D11->WindowClass.lpszClassName, pD3D11->WindowClass.hInstance);
	if (pD3D11->WindowHwnd != NULL) { return false; }
	return true;
}

bool DX11::Initialize()
{
	if (this->InitWindow() == false) { this->DeleteWindow(); return false; }

	HINSTANCE hModule = spoof_call::GetModuleBase(skCrypt("d3d11.dll").decrypt());
	void* D3D11CreateDeviceSwapChain = spoof_call::GetModuleProcAddr(hModule, skCrypt("D3D11CreateDeviceAndSwapChain").decrypt());
	
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	DXGI_RATIONAL refreshRate;
	refreshRate.Numerator = 60;
	refreshRate.Denominator = 1;

	DXGI_MODE_DESC bufferDesc;
	bufferDesc.Width = 100;
	bufferDesc.Height = 100;
	bufferDesc.RefreshRate = refreshRate;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = pD3D11->WindowHwnd;
	swapChainDesc.Windowed = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	if (((long(__stdcall*)(
		IDXGIAdapter*,
		D3D_DRIVER_TYPE,
		HMODULE,
		UINT,
		const D3D_FEATURE_LEVEL*,
		UINT,
		UINT,
		const DXGI_SWAP_CHAIN_DESC*,
		IDXGISwapChain**,
		ID3D11Device**,
		D3D_FEATURE_LEVEL*,
		ID3D11DeviceContext**))(D3D11CreateDeviceSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
	{
		this->DeleteWindow();
		return false;
	}

	MethodsTable = (uintx_t*)::spoof_call::NtCalloc(205, sizeof(uintx_t));
	spoof_call::SafeCopy(MethodsTable, *(uintx_t**)swapChain, 18 * sizeof(uintx_t));
	spoof_call::SafeCopy(MethodsTable + 18, *(uintx_t**)device, 43 * sizeof(uintx_t));
	spoof_call::SafeCopy(MethodsTable + 18 + 43, *(uintx_t**)context, 144 * sizeof(uintx_t));

	NTMakeHook::NTInitializeHook();

	swapChain->Release();
	swapChain = NULL;

	device->Release();
	device = NULL;

	context->Release();
	context = NULL;

	this->DeleteWindow();
	return true;
}