#ifndef FCPP_LIBSDL_H
#define FCPP_LIBSDL_H

#include "OtherExtensions/SDL/include/SDL.h"
#include "OtherExtensions/SDL_TTF/include/SDL_ttf.h"
#include "OtherExtensions/SDL_Image/include/SDL_image.h"
#include "OtherExtensions/SDL_Mixer/include/SDL_mixer.h"
#include <vector>
#include <iostream>
#include <d3d9.h>
#include <tchar.h>
#include "StringFunctions.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "GameEngine/GameEngine/library/OtherExtensions/SDL/lib/x86/SDL2.lib")
#pragma comment(lib, "GameEngine/GameEngine/library/OtherExtensions/SDL/lib/x86/SDL2main.lib")
#pragma comment(lib, "GameEngine/GameEngine/library/OtherExtensions/SDL_TTF/lib/x86/SDL2_ttf.lib")
#pragma comment(lib, "GameEngine/GameEngine/library/OtherExtensions/SDL_Mixer/lib/x86/SDL2_mixer.lib")
#pragma comment(lib, "GameEngine/GameEngine/library/OtherExtensions/SDL_Image/lib/x86/SDL2_image.lib")

typedef std::string string;

/***************************************************************************************************************************************************************************************************************************************************************************************/
inline const SDL_Point& SDL_GetMousePos(SDL_Event& e)
{
	SDL_Point p{ 0, 0 };
	p.x += e.button.x;
	p.y += e.button.y;
	return p;
}

class List
{
private:
	std::fstream file{};
	string name{};
	std::vector<string>_list{};

	void dump()
	{
		file.open(name.c_str());
		if (!Empty()) _list.clear();
		while (file)
		{
			string temp{};
			std::getline(file, temp);
			_list.push_back(temp);
		}
		_list.pop_back(); _list.shrink_to_fit(); // remove empty space at the end
		file.close();
	}

	void pop()
	{
		_list.pop_back();
		_list.shrink_to_fit();
	}

	void Erase(int element)
	{
		_list.erase(std::next(_list.begin(), element));
	}

	std::vector<string>& list()
	{
		return _list;
	}

public:
	List()
	{
	}

	explicit List(string strname)
	{
		name = strname;
		if (FileSize(name.c_str()) > 0) dump();
	}

	const char* Name()
	{
		return name.c_str();
	}

	const bool& Empty()
	{
		return _list.empty();
	}

	const size_t& size()
	{
		return _list.size();
	}

	void attach_new_list(string strname)
	{
		name = strname;
		if (FileSize(name.c_str()) > 0) dump();
	}

	void clear()
	{
		_list.clear();
		file.open(name.c_str(), std::fstream::out | std::fstream::trunc); std::cout << ""; file.close();
	}

	int& compare(const char* item)
	{
		int num = -1;
		for (unsigned int i = 0; i < _list.size(); i++)
		{
			if (strfind(_list[i], item))
			{
				num = i;
				break;
			}
		}
		return num;
	}

	void overwrite()
	{
		file.open(name.c_str(), std::fstream::out | std::fstream::trunc); file << ""; file.close();
		for (unsigned int i = 0; i < _list.size(); i++)
		{
			file.open(name.c_str(), std::fstream::app);
			file << _list[i] << std::endl;
			file.close();
		}
	}

	void operator+=(string s) { _list.push_back(s); }
	void operator-=(int i) { Erase(i); }
	void operator--() { pop(); }
	string& operator()(int index) { return _list[index]; }
};

class WindowManager
{
private:
	SDL_Window* win{};
	SDL_Renderer* ren{};
	SDL_Rect WinRect{};
	string name{};
	bool Vsync{};
	bool GpuRender{}; // true means gpu render, false means cpu render
	bool grabbed{};

	void SetRen()
	{
		if (GpuRender) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
		else if (!GpuRender) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
		SDL_RenderSetVSync(ren, Vsync);
	}
public:
	/// <summary>
	/// everything a basic SDL window needs and more in a single class
	/// </summary>
	/// <param name="wName">the window name</param>
	/// <param name="x">x pos of top left corner</param>
	/// <param name="y">y pos of top left corner</param>
	/// <param name="w">width of window</param>
	/// <param name="h">height of window</param>
	/// <param name="grabbed">is input grabbed?</param>
	/// <param name="vsync">vsync anyone?</param>
	/// <param name="gpu_render">sometimes graphics rendering is better</param>
	inline explicit WindowManager(const char* wName, int x, int y, int w, int h, bool grabbed = false, bool vsync = false, bool gpu_render = false)
	{
		name = wName;
		WinRect = { x,y,w,h };
		Vsync = vsync;
		GpuRender = gpu_render;
		this->grabbed = grabbed;
	}

	inline WindowManager() {}

	inline void DeploySetupInformation(const char* wName, int x, int y, int w, int h, bool grabbed = false, bool vsync = false, bool gpu_render = false)
	{
		name = wName;
		WinRect = { x,y,w,h };
		Vsync = vsync;
		GpuRender = gpu_render;
		this->grabbed = grabbed;
	}

	void LoadWindow()
	{
		win = SDL_CreateWindow(name.c_str(), WinRect.x, WinRect.y, WinRect.w, WinRect.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		SetRen();
		SDL_RaiseWindow(this->win);
		if (grabbed)
		{
			SDL_SetWindowGrab(this->win, SDL_TRUE);
		}
	}

	inline void ToggleVsync()
	{
		Vsync = !Vsync;
		SDL_RenderSetVSync(ren, Vsync);
	}

	inline void resize(int w, int h)
	{
		SDL_SetWindowSize(win, w, h);
	}

	inline void put_cursor(int x, int y)
	{
		int new_x(0), new_y(0);
		if (x > WinRect.x && x < (WinRect.x + WinRect.w)) new_x = WinRect.x + x;
		if (y > WinRect.y && y < (WinRect.y + WinRect.h)) new_y = WinRect.y + y;
		SetCursorPos(new_x, new_y);
	}

	inline void SetGrab(bool grab)
	{
		grabbed = grab;
		if (grabbed) SDL_SetWindowGrab(win, SDL_TRUE);
		else { SDL_SetWindowGrab(win, SDL_FALSE); }
	}

	inline const bool& isGrabbed()
	{
		return SDL_GetWindowGrab(win);
	}

	inline void raise()
	{
		SDL_RaiseWindow(win);
	}

	inline const SDL_Point& Size() { return { WinRect.w, WinRect.h }; }

	inline SDL_Renderer* Ren() { return ren; }

	inline void SetTitle(const char* name)
	{
		this->name = name;
		SDL_SetWindowTitle(win, name);
	}

	inline void destroy()
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
	}

	inline SDL_Window* operator()()
	{
		return win;
	}
};

#undef __T
namespace kbInput
{
	bool __A(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_a) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_a) return false; } }
	bool __B(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_b) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_b) return false; } }
	bool __C(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_c) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_c) return false; } }
	bool __D(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_d) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_d) return false; } }
	bool __E(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_e) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_e) return false; } }
	bool __F(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_f) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_f) return false; } }
	bool __G(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_g) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_g) return false; } }
	bool __H(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_h) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_h) return false; } }
	bool __I(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_i) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_i) return false; } }
	bool __N(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_n) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_n) return false; } }
	bool __O(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_o) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_o) return false; } }
	bool __P(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_p) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_p) return false; } }
	bool __Q(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_q) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_q) return false; } }
	bool __R(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_r) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_r) return false; } }
	bool __S(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_s) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_s) return false; } }
	bool __T(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_t) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_t) return false; } }
	bool __V(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_v) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_v) return false; } }
	bool __W(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_w) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_w) return false; } }
	bool __X(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_x) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_x) return false; } }
	bool _UP(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKUP) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKUP) return false; } }
	bool DWN(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKDN) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKDN) return false; } }
	bool LFT(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKLF) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKLF) return false; } }
	bool RGT(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKRT) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKRT) return false; } }
	bool ENT(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKEN) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKEN) return false; } }
	bool ESC(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKES) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKES) return false; } }
	bool LSH(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKLS) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKLS) return false; } }
	bool RSH(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKRS) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKRS) return false; } }
	bool SPC(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKSP) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKSP) return false; } }
	bool __0(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_0) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_0) return false; } }
	bool __1(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_1) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_1) return false; } }
	bool __2(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_2) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_2) return false; } }
	bool __3(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_3) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_3) return false; } }
	bool __4(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_4) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_4) return false; } }
	bool __5(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_5) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_5) return false; } }
	bool __6(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_6) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_6) return false; } }
	bool __7(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_7) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_7) return false; } }
	bool __8(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_8) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_8) return false; } }
	bool __9(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLK_9) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLK_9) return false; } }
	bool BCK(SDL_Event e) { if (e.type == SDL_KEYDOWN) { if (e.key.keysym.sym == SDLKBS) return true; } else if (e.type == SDL_KEYUP) { if (e.key.keysym.sym == SDLKBS) return false; } }
}

// still not sure if this is better or worse than simple pixel scaling
void SDL_ScaleResolution(SDL_Renderer* ren, int w, int h)
{
	SDL_RenderSetLogicalSize(ren, w, h);
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");
}

namespace ColorCollection
{
	SDL_Color Pink{ 255, 192, 203 }, DeepPink{ 255, 20, 147 }, PaleVioletRed{ 219, 112, 147 }, HotPink{ 255, 105, 180 }, MediumVioletRed{ 199, 21, 133 }; // pink shades
	SDL_Color Blue{ 0, 0, 255 }, Cyan{ 0, 255, 255 }, Azure{ 0, 128, 255 }, SkyBlue{ 135, 206, 250 }, RoyalBlue{ 65, 105, 225 }, Navy{ 0, 0, 128 }; // blue shades
	SDL_Color Lime{ 0, 255, 0 }, Green{ 0, 128, 0 }, ForestGreen{ 74, 103, 65 }, Olive{ 128, 128, 0 }, SpringGreen{ 0, 255, 127 }, SeaGreen{ 46, 139, 87 }; // green shades
	SDL_Color Magenta{ 255, 0, 255 }, Violet{ 238, 130, 238 }, Purple{ 128, 0, 128 }, Indigo{ 75, 0, 130 }; // purple shades
	SDL_Color Red{ 255, 0, 0 }, Orange{ 255, 127, 0 }, Brown{ 139, 69, 19 }, Crimson{ 220, 20, 60 }, Salmon{ 250, 128, 114 }, Maroon{ 128, 0, 0 }; // red shades
	SDL_Color Black{ 0, 0, 0 }, White{ 255, 255, 255 }, Gray{ 128, 128, 128 }, Yellow{ 255, 255, 0 };
}


#endif