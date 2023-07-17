#ifndef FCPP_SYSFUNC_H
#define FCPP_SYSFUNC_H

#include <iostream>
#include <string>
#include <Windows.h>
#include <d3d9.h>
#include <dxgi.h>
#include <Psapi.h>

typedef std::string string;

class CmdTools
{
private:
	bool show_cmd{};
	TCHAR title[MAX_PATH]{};
	buf b{};
	int x, y, w, h;

public:
	CmdTools()
	{
		x = 100; y = 100;
		w = 800; h = 600;
		GetConsoleTitle(title, MAX_PATH);
		sprintf_s(b, "%ws", title);
		show_cmd = true;
	}

	void repos(int x, int y)
	{
		this->x = x; this->y = y;
		HWND Cmd = GetConsoleWindow();
		if (!Cmd) return;
		SetWindowPos(Cmd, HWND_NOTOPMOST, x, y, w, h, SWP_SHOWWINDOW);
		ShowWindow(Cmd, SW_NORMAL);
	}

	void focus()
	{
		HWND Cmd = GetConsoleWindow();
		if (Cmd)
		{
			SetWindowPos(Cmd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
			ShowWindow(Cmd, SW_NORMAL);
		}
	}

	void unfocus()
	{
		HWND Cmd = GetConsoleWindow();
		if (Cmd)
		{
			SetWindowPos(Cmd, HWND_BOTTOM, x, y, w, h, SWP_SHOWWINDOW);
			ShowWindow(Cmd, SW_NORMAL);
		}
	}

	CmdTools(bool showcmd)
	{
		x = 100; y = 100;
		w = 800; h = 600;
		GetConsoleTitle(title, MAX_PATH);
		sprintf_s(b, "%ws", title);
		show_cmd = showcmd;
		if (show_cmd) ShowWindow(GetConsoleWindow(), SW_SHOW);
		else if (!show_cmd) ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	const string get_name()
	{
		return b;
	}

	void set_name(string title)
	{
		SetConsoleTitleA(title.c_str());
		GetConsoleTitle(this->title, MAX_PATH);
		sprintf_s(b, "%ws", this->title);
	}

	void toggle()
	{
		show_cmd = !show_cmd;
		if (show_cmd) ShowWindow(GetConsoleWindow(), SW_SHOW);
		else if (!show_cmd) ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	void cls()
	{
		system("cls");
	}

	bool showing()
	{
		return IsWindowVisible(GetConsoleWindow());
	}
};

// keyboard and mouse operations
struct KBM_OP
{
	bool __0{ false }; // 0 key
	bool __1{ false }; // 1 key
	bool __2{ false }; // 2 key
	bool __3{ false }; // 3 key
	bool __4{ false }; // 4 key
	bool __5{ false }; // 5 key
	bool __6{ false }; // 6 key
	bool __7{ false }; // 7 key
	bool __8{ false }; // 8 key
	bool __9{ false }; // 9 key
	bool __a{ false }; // a key
	bool __b{ false }; // b key
	bool __c{ false }; // c key
	bool __d{ false }; // d key
	bool __e{ false }; // e key
	bool __f{ false }; // f key
	bool __g{ false }; // g key
	bool __h{ false }; // h key
	bool __i{ false }; // i key
	bool __n{ false }; // n key
	bool __o{ false }; // o key
	bool __p{ false }; // p key
	bool __q{ false }; // q key
	bool __r{ false }; // r key
	bool __s{ false }; // s key
	bool __t{ false }; // t key
	bool __v{ false }; // v key
	bool __w{ false }; // w key
	bool __x{ false }; // x key
	bool lsh{ false }; // left shift
	bool rsh{ false }; // right shift
	bool spc{ false }; // space
	bool lct{ false }; // left ctrl
	bool rct{ false }; // right ctrl
	bool ent{ false }; // enter
	bool _up{ false }; // up arrow
	bool dwn{ false }; // down arrow
	bool lft{ false }; // left arrow
	bool rgt{ false }; // right arrow
	bool esc{ false }; // escape
	bool mbl{ false }; // left mouse
	bool mbr{ false }; // right mouse
	bool mbm{ false }; // middle mouse
	bool mwu{ false }; // mouse wheel up
	bool mwd{ false }; // mouse wheel down
	bool bck{ false }; // backspace
};

class SysMessageBox
{
private:
	LPCSTR Text{}, Caption;
	UINT MessageBoxType{}, MessageBoxIcon{};
	int lastInput{};

public:
	SysMessageBox(LPCSTR Txt = "Message Box Text", LPCSTR Cap = "Message Box Caption", UINT Type = MB_OK, UINT Icon = MB_ICONWARNING)
	{
		Text = Txt;
		Caption = Cap;
		MessageBoxType = Type;
		MessageBoxIcon = Icon;
	}

	void display()
	{
		lastInput = MessageBoxA(NULL, Text, Caption, MessageBoxType | MessageBoxIcon);
	}

	// checks if value of Inp(IN ALL CAPS) was returned from last message box input(IE "ABORT" for IDABORT)
	const bool SYSMB_NO() { return lastInput == IDNO; }
	const bool SYSMB_OK() { return lastInput == IDOK; }
	const bool SYSMB_YES() { return lastInput == IDYES; }
	const bool SYSMB_RETRY() { return lastInput == IDRETRY; }
	const bool SYSMB_ABORT() { return lastInput == IDABORT; }
	const bool SYSMB_CANCEL() { return lastInput == IDCANCEL; }
	const bool SYSMB_IGNORE() { return lastInput == IDIGNORE; }
	const bool SYSMB_CONTINUE() { return lastInput == IDCONTINUE; }
	const bool SYSMB_TRYAGAIN() { return lastInput == IDTRYAGAIN; }
};


#endif