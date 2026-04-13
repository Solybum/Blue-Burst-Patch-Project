#include "d3d8_hook.h"
#include <atomic>
#include <stdio.h>
#include <thread>
#include <windows.h>

#ifndef __MSABI_LONG
#define __MSABI_LONG(x) x
#endif

#include "../include/d3d8.h"

static bool g_bDebugLogs = false;

void Log(const char *fmt, ...) {
  if (!g_bDebugLogs)
    return;
  FILE *f = nullptr;
  if (fopen_s(&f, "d3d8_hook.log", "a") == 0 && f) {
    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fclose(f);
  }
}

typedef IDirect3D8* (WINAPI *Direct3DCreate8_Proto)(UINT SDKVersion);

typedef HRESULT (__stdcall *Present_Proto)(
    IDirect3DDevice8* pThis, const RECT* pSourceRect, const RECT* pDestRect,
    HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
Present_Proto oPresent = nullptr;

std::atomic<bool> g_apiHooked{false};
bool g_bInterpEnabled = false;
DWORD g_targetHz = 60;

void ReadFrameGenSettings() {
  HKEY hKey;
  if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\SonicTeam\\PSOBB", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
    DWORD size = sizeof(DWORD);
    DWORD enabled = 0;
    if (RegQueryValueExA(hKey, "FrameGenEnabled", 0, 0, (LPBYTE)&enabled, &size) == ERROR_SUCCESS) {
      g_bInterpEnabled = (enabled != 0);
    }
    DWORD hz = 60;
    if (RegQueryValueExA(hKey, "TargetHz", 0, 0, (LPBYTE)&hz, &size) == ERROR_SUCCESS) {
      g_targetHz = hz;
    }
    DWORD debugLogs = 0;
    if (RegQueryValueExA(hKey, "DebugLogsEnabled", 0, 0, (LPBYTE)&debugLogs, &size) == ERROR_SUCCESS) {
      g_bDebugLogs = (debugLogs != 0);
    }
    RegCloseKey(hKey);

    if (g_bDebugLogs) {
      Log("[BBPP] FrameGen Settings: Enabled=%d, TargetHz=%d, DebugLogs=%d\n", g_bInterpEnabled, g_targetHz, g_bDebugLogs);
    }
  }
}

HRESULT __stdcall MyPresent(IDirect3DDevice8* pThis, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) {
  int repeats = 1;
  if (g_bInterpEnabled && g_targetHz > 30) {
    if (g_targetHz >= 240) repeats = 8;
    else if (g_targetHz >= 165) repeats = 6;
    else if (g_targetHz >= 144) repeats = 5;
    else if (g_targetHz >= 120) repeats = 4;
    else if (g_targetHz >= 90) repeats = 3;
    else if (g_targetHz >= 75) repeats = 3;
    else if (g_targetHz >= 60) repeats = 2;
  }
  
  HRESULT hr = S_OK;
  for (int i = 0; i < repeats; i++) {
    hr = oPresent(pThis, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
  }
  return hr;
}

void InstallD3D8Hook() {
  if (g_apiHooked)
    return;
  g_apiHooked = true;

  HMODULE hD3d8 = GetModuleHandleA("d3d8.dll");
  if (!hD3d8) hD3d8 = LoadLibraryA("d3d8.dll");

  if (!hD3d8) {
    Log("[BBPP] ERROR: d3d8.dll not found!\n");
    return;
  }

  Direct3DCreate8_Proto pCreate8 = (Direct3DCreate8_Proto)GetProcAddress(hD3d8, "Direct3DCreate8");
  if (!pCreate8) {
    Log("[BBPP] ERROR: Direct3DCreate8 not found!\n");
    return;
  }

  IDirect3D8* pD3D = pCreate8(D3D_SDK_VERSION);
  if (!pD3D) {
    Log("[BBPP] ERROR: Direct3DCreate8 failed!\n");
    return;
  }

  HWND dummyWindow = CreateWindowA("STATIC", "dummy", 0, 0, 0, 1, 1, NULL, NULL, NULL, NULL);
  if (!dummyWindow) {
    Log("[BBPP] ERROR: Failed to create dummy window!\n");
    pD3D->Release();
    return;
  }

  D3DPRESENT_PARAMETERS d3dpp = {0};
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.hDeviceWindow = dummyWindow;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  d3dpp.BackBufferCount = 1;

  IDirect3DDevice8* pDummyDevice = nullptr;
  HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dummyWindow,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &d3dpp, &pDummyDevice);
  
  if (SUCCEEDED(hr) && pDummyDevice) {
    void** vtable = *(void***)pDummyDevice;
    if (vtable[15] != MyPresent) {
      oPresent = (Present_Proto)vtable[15];
      Log("[BBPP] Hooking D3D8 Present (15). Original: %p\n", oPresent);
      DWORD oldProtect;
      VirtualProtect(&vtable[15], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);
      vtable[15] = (void*)MyPresent;
      VirtualProtect(&vtable[15], sizeof(void*), oldProtect, &oldProtect);
      Log("[BBPP] D3D8 Global Hook securely initialized via dummy device!\n");
    }
    pDummyDevice->Release();
  } else {
    Log("[BBPP] Dummy CreateDevice failed: %X\n", hr);
  }

  DestroyWindow(dummyWindow);
  pD3D->Release();
}

void HookThread() {
  InstallD3D8Hook();
}

void SetupD3D8Hook() {
  ReadFrameGenSettings();
  Log("[BBPP] SetupD3D8Hook called. FrameGen=%d, TargetHz=%d\n", g_bInterpEnabled, g_targetHz);
  std::thread(HookThread).detach();
}
