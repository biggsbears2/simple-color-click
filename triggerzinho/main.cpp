#include <iostream>
#include <Windows.h>

//add formas de configurar


void ActivateScript() {
    int key_hold = VK_MENU; // Tecla para o trigger funcionar
    int key_exit = VK_END;
    int key_to_simulate = 'P';
    double pixel_box = 2.2555555555;
    int pixel_sens = 60;
    COLORREF pixel_color = RGB(235, 105, 254); // cor roxa valorant, mudar para amarelo depois
    int sleep_time = 0.1;

    int leftbound = GetSystemMetrics(SM_CXSCREEN) / 2 - pixel_box;
    int rightbound = GetSystemMetrics(SM_CXSCREEN) / 2 + pixel_box;
    int topbound = GetSystemMetrics(SM_CYSCREEN) / 2 - pixel_box;
    int bottombound = GetSystemMetrics(SM_CYSCREEN) / 2 + pixel_box;

    while (true) {
        if (GetAsyncKeyState(key_hold) & 0x8000) {
            HDC hdcScreen = GetDC(NULL);
            HDC hdcMem = CreateCompatibleDC(hdcScreen);
            int width = rightbound - leftbound;
            int height = bottombound - topbound;
            BITMAPINFOHEADER bmpInfo = { 0 };
            bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
            bmpInfo.biPlanes = 1;
            bmpInfo.biBitCount = 32;
            bmpInfo.biWidth = width;
            bmpInfo.biHeight = -height;
            bmpInfo.biCompression = BI_RGB;
            LPBYTE lpBits = NULL;
            HBITMAP hBitmap = CreateDIBSection(hdcScreen, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, (LPVOID*)&lpBits, NULL, 0);

            if (hBitmap != NULL) {
                SelectObject(hdcMem, hBitmap);
                BitBlt(hdcMem, 0, 0, width, height, hdcScreen, leftbound, topbound, SRCCOPY);

                for (int i = 0; i < width * height; i++) {
                    int r = lpBits[i * 4 + 2];
                    int g = lpBits[i * 4 + 1];
                    int b = lpBits[i * 4];

                    if (abs(r - GetRValue(pixel_color)) < pixel_sens &&
                        abs(g - GetGValue(pixel_color)) < pixel_sens &&
                        abs(b - GetBValue(pixel_color)) < pixel_sens)
                    {
                        
                            INPUT input = {};
                            input.type = INPUT_KEYBOARD;
                            input.ki.wVk = VkKeyScan('P');
                            input.ki.dwFlags = 0;
                            SendInput(1, &input, sizeof(INPUT));
                            Sleep(sleep_time);
                            input.ki.dwFlags = KEYEVENTF_KEYUP; 
                            SendInput(1, &input, sizeof(INPUT));
                        
                        break;
                    }
                }

                DeleteObject(hBitmap);
            }

            DeleteDC(hdcMem);
            ReleaseDC(NULL, hdcScreen);
        }

        if (GetAsyncKeyState(key_exit) & 0x8000) {
            Sleep(400);
            break;
        }

        Sleep(1);
    }
}

int main() {
    ActivateScript();
    return 0;
}
