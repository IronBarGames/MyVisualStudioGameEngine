// EasyBake game engine, created with C++/SDL2/DirectX
// EasyBake engine is a custom game engine created for easy of game creation using C++ with visual studio, along with SDL2 and DirectX
// currently lightweight, the engine is packed into this C++ header file for ease of access within existing projects
// the engine will continue to be expanded upon over time and will only get better with each release
// the engine is fully portable on windows, not requiring SDL2/DX9 as those are safely baked into the engine using windows specific commands to make sure they are so you dont need to
#include "AdviLib.h"

// import and/or convert things from AdviLib to EasyBake engine
// should figure out a way to clean up this file
// do a complete redo of other header files to better support ease of use

#define ENGINE_VERSION "EasyBake 1.1.5"
#define REN WM.Ren()
#define DumpTextboxes(src, des) for (int i = src.size(); i--;) { des.push_back(src.back()); src.pop_back(); src.shrink_to_fit(); }
#define DumpTextures(src, des) for (int i = src.size(); i--;) { des.push_back(src.back()); src.pop_back(); src.shrink_to_fit(); }
#define VecErase(vec, val) vec.erase(std::next(vec.begin(), val))
#define GPU_RENDERING true, false, true // input is grabbed, vsync not enabled, gpu rendering enabled

// window variables
const char* wTitle{ "EasyBake" };
int xRes(600), yRes(600), xWin(100), yWin(100);
WindowManager WM{ wTitle, xWin, yWin, xRes, yRes, GPU_RENDERING };

// engine variables
bool quit(false);
SDL_Event e{};
int MAX_ALPHA(255), HALFALPHA(128);
SDL_Point M{}; SDL_FPoint MouseF{};
KBM_OP Keys;

class FrameController
{
private:
	FpsCounter fpsCounter{};
	float frame1{}, frame2{}, frameTime{};
public:
	int FrameCap{ 120 };
	inline FrameController() { fpsCounter.capped = false; }

	inline const int& FPS()
	{
		return fpsCounter.get();
	}

	inline const int& FRAME_COUNT()
	{
		return fpsCounter.GetFrames();
	}

	inline void Start()
	{
		fpsCounter.Start();
	}

	inline const float& deltaTime()
	{
		return frameTime;
	}

	inline void BeginFrame()
	{
		fpsCounter.equate();
		frame2 = SDL_GetTicks(); frameTime = (frame2 - frame1); frame1 = SDL_GetTicks();
	}

	inline void ToggleFrameCap()
	{
		fpsCounter.capped = !fpsCounter.capped;
		if (fpsCounter.capped) fpsCounter.SetLimit(FrameCap);
	}

	inline void DrawFrame()
	{
		fpsCounter.RenderFrame(REN);
	}
}; FrameController frameController;

void InitEngine()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP);
	TTF_Init();
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 640);
	WM.LoadWindow(); WM.put_cursor(xRes / 2, yRes / 2);
	SDL_SetRenderDrawBlendMode(REN, SDL_BLENDMODE_BLEND);
	frameController.Start();
}

void QuitEngine(std::vector<SDL_Texture*>& t)
{
	while (t.size() > 0) { SDL_DestroyTexture(t.back()); t.pop_back(); t.shrink_to_fit(); } std::cout << "QuitSDL(): textures cleared!\n";
	WM.destroy();
	Mix_CloseAudio(); std::cout << "QuitSDL(): audio closed!\n";
	Mix_Quit(); std::cout << "QuitSDL(): Mix_Quit() passed!\n";
	TTF_Quit(); std::cout << "QuitSDL(): TTF_Quit() passed!\n";
	IMG_Quit(); std::cout << "QuitSDL(): IMG_Quit() passed!\n";
	SDL_Quit(); std::cout << "QuitSDL(): SDL_Quit() passed!\n";
}

inline const SDL_Color& GetRandomColor()
{
	return { static_cast<Uint8>(random(255)), static_cast<Uint8>(random(255)), static_cast<Uint8>(random(255)), static_cast<Uint8>(random(255)) };
}

inline void DrawLine(const int& x1, const int& y1, const int& x2, const int& y2)
{
	SDL_RenderDrawLine(REN, x1, y1, x2, y2);
}

inline void DrawLineF(const float& x1, const float& y1, const float& x2, const float& y2)
{
	SDL_RenderDrawLineF(REN, x1, y1, x2, y2);
}

inline void PixelScale(float px, float py)
{
	SDL_RenderSetScale(REN, px, py);
}

inline void DrawColor(const SDL_Color & color, const int& alpha)
{
	SDL_SetRenderDrawColor(REN, color.r, color.g, color.b, alpha);
}

inline void DrawColorRand()
{
	DrawColor(GetRandomColor(), MAX_ALPHA);
}

inline const int& ImportNumber(List& list, int line)
{
	return std::stoi(list(line));
}

inline const string& PreciseDecimalOutput(const float& f, int per)
{
	string format("%." + std::to_string(per) + "f"), r; char buf[256];
	sprintf_s(buf, format.c_str(), f);
	return buf;
}

class Rect
{
private:
	SDL_Rect rect{};
	SDL_Color rect_color;
	int alpha{};
public:
	inline explicit Rect(int x, int y, int w, int h)
	{
		rect = { x, y, w , h };
	}

	inline int& X() { return rect.x; }
	inline int& Y() { return rect.y; }
	inline int& W() { return rect.w; }
	inline int& H() { return rect.h; }

	inline void recolor(SDL_Color color, int alpha = 255)
	{
		rect_color = color;
		this->alpha = alpha;
	}

	inline void draw()
	{
		DrawColor(rect_color, alpha);
		SDL_RenderDrawRect(REN, &rect);
	}

	inline void fill()
	{
		DrawColor(rect_color, alpha);
		SDL_RenderFillRect(REN, &rect);
	}

	inline const SDL_Rect& GET()
	{
		return rect;
	}

	inline const bool& CursorInside()
	{
		return SDL_PointInRect(&M, &rect);
	}
};

class RectF
{
private:
	SDL_FRect rect{};
	SDL_Color rect_color{};
	int alpha{};

public:
	inline explicit RectF(float x, float y, float w, float h)
	{
		rect = { x, y, w, h };
	}

	inline float& X() { return rect.x; }
	inline float& Y() { return rect.y; }
	inline float& W() { return rect.w; }
	inline float& H() { return rect.h; }

	inline void recolor(SDL_Color color, int alpha = 255)
	{
		rect_color = color;
		this->alpha = alpha;
	}

	inline void draw()
	{
		DrawColor(rect_color, alpha);
		SDL_RenderDrawRectF(REN, &rect);
	}

	inline void fill()
	{
		DrawColor(rect_color, alpha);
		SDL_RenderFillRectF(REN, &rect);
	}

	inline const SDL_FRect& GET()
	{
		return rect;
	}
};

class PointF
{
private:
	SDL_FPoint point{};
	SDL_Color point_color{};
	int alpha;
public:
	inline explicit PointF(float x, float y)
	{
		point = { x, y };
	}

	inline float& X() { return point.x; }
	inline float& Y() { return point.y; }

	inline void recolor(SDL_Color color, int alpha = 255)
	{
		point_color = color;
		this->alpha = alpha;
	}

	inline void draw()
	{
		DrawColor(point_color, alpha);
		SDL_RenderDrawPointF(REN, point.x, point.y);
	}

	inline const SDL_FPoint& GET()
	{
		return point;
	}
};

class Point
{
private:
	SDL_Point point{};
	SDL_Color point_color;
	int alpha;
public:
	
	inline explicit Point(int x, int y)
	{
		point = { x, y };
	}

	inline int& X() { return point.x; }
	inline int& Y() { return point.y; }

	inline void recolor(SDL_Color color, int alpha = 255)
	{
		point_color = color;
		this->alpha = alpha;
	}

	inline void draw()
	{
		DrawColor(point_color, alpha);
		SDL_RenderDrawPoint(REN, point.x, point.y);
	}

	inline const SDL_Point& GET()
	{
		return point;
	}
};

inline const bool& PointInRect(Point& P, Rect& R)
{
	if (P.X() < R.X() || P.X() > R.X() + R.W()) return false;
	if (P.Y() < R.Y() || P.Y() > R.Y() + R.H()) return false;
	return true;
}

class GridMap
{
private:
	List map_file{};
	int mapsize_x, mapsize_y;
	std::vector<string>Container{};
	int cell_size{};
public:
	explicit GridMap(string file, const int& map_size_x, const int& map_size_y, const int& cell_size)
	{
		this->cell_size = cell_size;
		map_file.attach_new_list(file);
		mapsize_x = map_size_x; mapsize_y = map_size_y;
		Container.resize(map_size_y);
		for (int improw = map_size_y; improw--;)
		{
			Container[improw] = map_file(improw);
		}
	}

	void Overwrite()
	{
		for (int exprow = mapsize_y; exprow--;)
		{
			map_file(exprow) = Container[exprow];
		}
		map_file.overwrite();
	}

	inline void SetCell(const int& x, const int& y, const char& c) { Container[y][x] = c; }

	inline const char& cell(const int& x, const int& y)
	{
		return Container[y][x];
	}

	inline const bool& EmptyCell(const int& x, const int& y) { return (Container[y][x] == '0'); }

	inline const int& GRID_X() { return mapsize_x; }
	inline const int& GRID_Y() { return mapsize_y; }

	inline const bool& xIsGridCell(const int& num) { return (num >= 0 && num < mapsize_x); }
	inline const bool& yIsGridCell(const int& num) { return (num >= 0 && num < mapsize_y); }

	inline const int& Cells() { return (mapsize_x * mapsize_y); }

	inline const int& CELL_SIZE()
	{
		return cell_size;
	}

	inline const bool& ValidCell(Point& CELL)
	{
		bool _x(CELL.X() >= 0 && CELL.X() < mapsize_x);
		bool _y(CELL.Y() >= 0 && CELL.Y() < mapsize_y);
		return (_x && _y);
	}

	inline const int& Normalized(const int& num)
	{
		return (num / cell_size) * cell_size;
	}

	inline void Clear()
	{
		for (int i = (mapsize_x * mapsize_y); i--;)
		{
			int mx(floor(i / mapsize_y));
			int my(i % mapsize_x);
			if (Container[my][mx] != '0') Container[my][mx] = '0';
		}
	}

	inline const bool& isPlayerCell(const int& x, const int& y, PointF& Player)
	{
		bool _x(x == floor(Player.X() / cell_size));
		bool _y(y == floor(Player.Y() / cell_size));
		return (_x && _y);
	}

	const SDL_Point& GetRandomEmptyCell()
	{
		int sx(mapsize_x - 1);
		while (true)
		{
			int py = random(100);
			int px = random(sx);
			if (Container[py][px] == '0') return { px, py };
		}
	}

	const bool& MapIdExists(char c)
	{
		string mc{}; mc += c;
		for (int i = mapsize_y; i--;)
		{
			if (strfind(Container[i], mc.c_str())) return true;
		}
		return false;
	}
};

void PutPlayerInCell(PointF& player, const int& x, const int& y, GridMap& grid)
{
	Point cell(x, y);
	if (!grid.EmptyCell(x, y) || !grid.ValidCell(cell)) return;
	player.X() = x * grid.CELL_SIZE();
	player.Y() = y * grid.CELL_SIZE();
}

class AudioController
{
public:
	bool _switch;
	AudioController() : _switch(true) {}

	inline void PlayAudio(AudioClip& aud, const bool& seq = true)
	{
		if (_switch) aud.Play(seq);
	}
};

/// <summary>
/// calculates step in positive or negative direction based on input equation
/// </summary>
/// <param name="value">this part is very touchy, so be exact down to the position of each value in the equation input</param>
/// <returns>-1 or 1</returns>
inline const int& StepRelativeDirection(const int& value)
{
	if (value == 0) return value;
	int result(value / abs(value));
	return result;
}

class GridMapRaycaster
{
private:
	Point Hit{0, 0};
	void GridMapRaycast(const float& ray_angle, PointF& ray, GridMap& grid)
	{
		PointF RayDirection{ Cos(ray_angle), Sin(ray_angle) }, DistH(0, 0), DistV(0, 0), DistE(0, 0), SqrLen(0, 0);
		Point RayMap(0, 0), FixedRay(0, 0);
		bool hit(false);
		while (!hit)
		{
			RayMap.X() = (ray.X() / grid.CELL_SIZE());
			RayMap.Y() = (ray.Y() / grid.CELL_SIZE());
			Hit = RayMap;
			FixedRay.X() = grid.Normalized(ray.X());
			FixedRay.Y() = grid.Normalized(ray.Y());
			if (ray.X() == FixedRay.X() && RayDirection.X() < 0) Hit.X()--;
			if (ray.Y() == FixedRay.Y() && RayDirection.Y() < 0) Hit.Y()--;
			if (!grid.ValidCell(Hit)) hit = true;
			else if (!grid.EmptyCell(Hit.X(), Hit.Y())) hit = true;
			else
			{
				if (ray.Y() == FixedRay.Y()) DistH.Y() = grid.CELL_SIZE();
				else
				{
					if (RayDirection.Y() < 0) DistH.Y() = ray.Y() - FixedRay.Y();
					else DistH.Y() = (RayMap.Y() + 1) * grid.CELL_SIZE() - ray.Y();
				}

				DistH.X() = DistH.Y() / Tan(ray_angle);

				if (RayDirection.Y() < 0)
				{
					InvertF(DistH.X());
					InvertF(DistH.Y());
				}

				if (ray.X() == FixedRay.X()) DistV.X() = grid.CELL_SIZE();
				else
				{
					if (RayDirection.X() < 0) DistV.X() = ray.X() - FixedRay.X();
					else DistV.X() = (RayMap.X() + 1) * grid.CELL_SIZE() - ray.X();
				}

				DistV.Y() = DistV.X() * Tan(ray_angle);

				if (RayDirection.X() < 0)
				{
					InvertF(DistV.X());
					InvertF(DistV.Y());
				}

				SqrLen.X() = DistH.X() * DistH.X() + DistH.Y() * DistH.Y();
				SqrLen.Y() = DistV.X() * DistV.X() + DistV.Y() * DistV.Y();
				DistE.X() = SqrLen.X() < SqrLen.Y() ? DistH.X() : DistV.X();
				DistE.Y() = SqrLen.X() < SqrLen.Y() ? DistH.Y() : DistV.Y();

				ray.X() += DistE.X();
				ray.Y() += DistE.Y();
			}
		}
	}

public:
	inline const char& ReadRayHit(PointF& _ray, PointF& RayStart, const float& _ray_angle, GridMap& grid)
	{
		_ray = RayStart;
		GridMapRaycast(_ray_angle, _ray, grid);
		if (!grid.ValidCell(Hit)) return '0';
		else return grid.cell(Hit.X(), Hit.Y());
	}

	inline const SDL_Point& GetHit() { return Hit.GET(); }
	inline const int& HitX() { return Hit.X(); }
	inline const int& HitY() { return Hit.Y(); }
};

class Circle
{
private:
	SDL_Color color{};
	int alpha{};
	Point Center{ 0, 0 };
	int radius;
public:
	inline explicit Circle(const int& cx, const int& cy, const int& r)
	{
		Center.X() = cx;
		Center.Y() = cy;
		radius = r;
	}

	inline void recolor(const SDL_Color& color, const int& alpha = 255)
	{
		this->color = color;
		this->alpha = alpha;
	}

	inline const SDL_Point& CENTER() { return Center.GET(); }
	inline int& CenterX() { return Center.X(); }
	inline int& CenterY() { return Center.Y(); }
	inline int& Radius() { return radius; }

	void draw()
	{
		DrawColor(color, alpha);
		int x(0);
		int y(radius);
		int d(1 - radius);
		int deltaE(3);
		int deltaSE(-2 * radius + 5);
		while (y >= x)
		{
			SDL_RenderDrawPoint(REN, Center.X() + x, Center.Y() + y);
			SDL_RenderDrawPoint(REN, Center.X() - x, Center.Y() + y);
			SDL_RenderDrawPoint(REN, Center.X() + x, Center.Y() - y);
			SDL_RenderDrawPoint(REN, Center.X() - x, Center.Y() - y);
			SDL_RenderDrawPoint(REN, Center.X() + y, Center.Y() + x);
			SDL_RenderDrawPoint(REN, Center.X() - y, Center.Y() + x);
			SDL_RenderDrawPoint(REN, Center.X() + y, Center.Y() - x);
			SDL_RenderDrawPoint(REN, Center.X() - y, Center.Y() - x);

			if (d < 0)
			{
				d += deltaE;
				deltaE += 2;
				deltaSE += 2;
			}
			else
			{
				d += deltaSE;
				deltaE += 2;
				deltaSE += 4;
				--y;
			}
			++x;
		}
	}

	void fill()
	{
		DrawColor(color, alpha);
		int x(0);
		int y(radius);
		int d(1 - radius);
		int deltaE(3);
		int deltaSE(-2 * radius + 5);
		while (y >= x)
		{
			SDL_RenderDrawLine(REN, CenterX() + x, CenterY() + y, CenterX() - x, CenterY() + y);
			SDL_RenderDrawLine(REN, CenterX() + x, CenterY() - y, CenterX() - x, CenterY() - y);
			SDL_RenderDrawLine(REN, CenterX() + y, CenterY() + x, CenterX() - y, CenterY() + x);
			SDL_RenderDrawLine(REN, CenterX() + y, CenterY() - x, CenterX() - y, CenterY() - x);

			if (d < 0)
			{
				d += deltaE;
				deltaE += 2;
				deltaSE += 2;
			}
			else
			{
				d += deltaSE;
				deltaE += 2;
				deltaSE += 4;
				--y;
			}
			++x;
		}
	}
};

/// <summary>
/// gets a screenshot of selected area of window
/// </summary>
/// <param name="name">the name of the image file with extension included</param>
/// <param name="portion">the part of the screen to get</param>
/// <param name="FORMAT">the type of image, must be in all caps: support "BMP", "PNG", or "JPG"</param>
void MakeScreenshot(const char* name, Rect& portion, string FORMAT)
{
	SDL_Surface* sur(SDL_CreateRGBSurfaceWithFormat(0, portion.GET().w, portion.GET().h, 32, SDL_PIXELFORMAT_ARGB8888));
	SDL_RenderReadPixels(REN, &portion.GET(), SDL_PIXELFORMAT_ARGB8888, sur->pixels, sur->pitch);
	if (FORMAT == "BMP") SDL_SaveBMP(sur, name);
	else if (FORMAT == "PNG") IMG_SavePNG(sur, name);
	else if (FORMAT == "JPG") IMG_SaveJPG(sur, name, 100);
	SDL_FreeSurface(sur);
}

const char* GetGpuName()
{
	D3DADAPTER_IDENTIFIER9 ID{};
	IDirect3D9* id(Direct3DCreate9(D3D_SDK_VERSION));
	id->GetAdapterIdentifier(0, 0, &ID);
	id->Release();
	return ID.Description;
}

const char* GetCpuName()
{
	int CpuInfo[4]{ -1 };
	unsigned nExIds, i = 0;
	char CpuBrandString[0x40]{};
	__cpuid(CpuInfo, 0x80000000);
	nExIds = CpuInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CpuInfo, i);
		if (i == 0x80000002) memcpy(CpuBrandString, CpuInfo, sizeof(CpuInfo));
		else if (i == 0x80000003) memcpy(CpuBrandString + 16, CpuInfo, sizeof(CpuInfo));
		else if (i == 0x80000004) memcpy(CpuBrandString + 32, CpuInfo, sizeof(CpuInfo));
	}
	return CpuBrandString;
}

unsigned const long long& SysRamUsage()
{
	MEMORYSTATUSEX mem; // should look more into this
	mem.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&mem);
	DWORDLONG UsedRam = mem.ullTotalPhys - mem.ullAvailPhys;
	unsigned long long t(mem.ullTotalPhys / (1024 * 1024));
	unsigned long long a(mem.ullAvailPhys / (1024 * 1024));
	return (t - a);
}

const int& GetTotalSystemMemory()
{
	return SDL_GetSystemRAM();
}

unsigned const long& AppRamUsage()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T memused = pmc.WorkingSetSize;
	return (memused / (1024 * 1024));
}

inline const float& SDL_GetRelativeAngle(PointF& AngleOrigin, PointF& RelativeTo)
{
	float a = 180 + atan2(AngleOrigin.Y() - RelativeTo.Y(), AngleOrigin.X() - RelativeTo.X()) * (180 / M_PI);
	a >= 0 ? a : 360 + a; return a;
}

void DrawPercentageBar(const float& num, const float& tot, const SDL_Color& bar, Rect& rect, const float& mul = 1, const int& alpha = 255, const bool& vertical = false)
{
	Rect Bar = rect;
	if (vertical)
	{
		Bar.Y() = (rect.Y() + rect.H() - (Percent(num, tot) * mul));
		Bar.H() = Percent(num, tot) * mul;
	}
	else Bar.W() = Percent(num, tot) * mul;
	rect.draw(); // this rect should have already been recolored
	Bar.recolor(bar, alpha); Bar.fill();
}
namespace MouseInput
{
	const bool& MBL() { static bool value(false); if (e.type == SDL_MOUSEBUTTONDOWN) { if (e.button.button == SDLMBL) value = true; } else if (e.type == SDL_MOUSEBUTTONUP) { if (e.button.button == SDLMBL) value = false; } return value; }
	const bool& MBR() { static bool value(false); if (e.type == SDL_MOUSEBUTTONDOWN) { if (e.button.button == SDLMBR) value = true; } else if (e.type == SDL_MOUSEBUTTONUP) { if (e.button.button == SDLMBR) value = false; } return value; }
	const bool& MBM() { static bool value(false); if (e.type == SDL_MOUSEBUTTONDOWN) { if (e.button.button == SDLMBM) value = true; } else if (e.type == SDL_MOUSEBUTTONUP) { if (e.button.button == SDLMBM) value = false; } return value; }
};

class Textbox
{
private:
	TTF_Font* font{ nullptr };
	SDL_Texture* tex{ nullptr };
	SDL_Color color{};
	string Text{};
	Rect rect{ 0, 0, 0, 0 };
	int size{};
public:
	Textbox() {}
	inline explicit Textbox(const char* font, const int& textSize, Rect rect, const SDL_Color& color)
	{
		this->font = TTF_OpenFont(font, textSize);
		this->rect = rect;
		this->color = color;
		size = textSize;
	}

	inline void setup(const char* font, const int& textSize, Rect rect, const SDL_Color& color)
	{
		this->font = TTF_OpenFont(font, textSize);
		this->rect = rect;
		this->color = color;
		size = textSize;
	}

	inline const int& length()
	{
		int TEXT_LENGTH(0);
		TTF_MeasureText(font, Text.c_str(), rect.W(), &TEXT_LENGTH, nullptr);
		return TEXT_LENGTH;
	}

	inline const int& CharacterCount()
	{
		int CHARLEN(0);
		TTF_MeasureText(font, Text.c_str(), rect.W(), nullptr, &CHARLEN);
		return CHARLEN;
	}

	inline void recolor(const SDL_Color& c)
	{
		color = c;
	}

	inline const int& TextLength() { return rect.W(); }

	inline void set_text(const char* t)
	{
		Text = t;
		tex = SDL_CreateTextureFromSurface(REN, TTF_RenderText_Blended(this->font, Text.c_str(), color));
		SDL_QueryTexture(tex, NULL, NULL, &rect.W(), &rect.H());
	}

	inline const bool& Hover(const SDL_Point& M, const SDL_Color& c)
	{
		if (!SDL_PointInRect(&M, &rect.GET())) return false;
		rect.recolor(c); rect.draw();
		return true;
	}

	inline const string& GET()
	{
		return Text;
	}

	inline void destroy()
	{
		TTF_CloseFont(font);
		SDL_DestroyTexture(tex);
		tex = nullptr;
		font = nullptr;
	}

	void operator()() { SDL_RenderCopy(REN, tex, NULL, &rect.GET()); }
};

inline void DelayedRateDynamicText(Textbox& tb, const string& target, MyClock& clock)
{
	if (!clock.Reached()) return;
	if (tb.GET() != target) { tb.set_text(target.c_str()); clock.Reset(); }
}

inline void FixedRateDynamicText(Textbox& tb, const string& target)
{
	if (tb.GET() != target) { tb.set_text(target.c_str()); }
}

inline void DrawTextboxes(std::vector<Textbox>& tbs)
{
	for (int i = tbs.size(); i--;) tbs[i]();
}

bool motion(false);
void ReadInput(bool& quitcase)
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) quitcase = !quitcase;
		if (motion = e.type == SDL_MOUSEMOTION) { M = SDL_GetMousePos(e); }
		Keys.esc = kbInput::ESC(e); Keys.__q = kbInput::__Q(e); Keys.__e = kbInput::__E(e); Keys.__w = kbInput::__W(e); Keys.__a = kbInput::__A(e); Keys.__s = kbInput::__S(e);
		Keys.__d = kbInput::__D(e); Keys.mbl = MouseInput::MBL(); Keys.mbr = MouseInput::MBR(); Keys.mbm = MouseInput::MBM(); Keys._up = kbInput::_UP(e); Keys.dwn = kbInput::DWN(e);
		Keys.ent = kbInput::ENT(e); Keys.spc = kbInput::SPC(e); Keys.__c = kbInput::__C(e);
	}
}

inline void RenderClear(const SDL_Color& C)
{
	DrawColor(C, MAX_ALPHA);
	SDL_RenderClear(REN);
}

class Line
{
private:
	int x1, x2, y1, y2, alpha;
	SDL_Color color;
public:
	inline explicit Line(const int& x1, const int& y1, const int& x2, const int& y2): x1(x1), y1(y1), x2(x2), y2(y2), alpha(255), color(ColorCollection::White) {}

	inline int& X1() { return x1; } inline int& X2() { return x2; }
	inline int& Y1() { return y1; } inline int& Y2() { return y2; }

	inline void recolor(const SDL_Color& c, const int& alpha = 255)
	{
		color = c;
		this->alpha = alpha;
	}

	inline void draw()
	{
		DrawColor(color, alpha);
		SDL_RenderDrawLine(REN, x1, y1, x2, y2);
	}

	inline void operator()(const int& x1, const int& y1, const int& x2, const int& y2)
	{
		this->x1 = x1; this->y1 = y1;
		this->x2 = x2; this->y2 = y2;
	}
};

inline void OutputIntegerList(const std::vector<int>& list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		std::cout << list[i] << " ";
	}
}

inline const std::vector<int>& VectorCopy(const std::vector<int>& p2)
{
	std::vector<int>clone; clone.resize(p2.size());
	for (int i = p2.size(); i--;)
	{
		clone[i] = p2[i];
	}
	return clone;
}

namespace Sorters
{
	inline void LowToHighFloatSort(std::vector<float>& container) { for (int i = 0; i < container.size() - 1; ++i) { for (int u = i + 1; u < container.size(); ++u) { if (container[i] > container[u]) { std::swap(container[i], container[u]); } } } }
	inline void HighToLowFloatSort(std::vector<float>& container) { for (int i = 0; i < container.size() - 1; ++i) { for (int u = i + 1; u < container.size(); ++u) { if (container[i] < container[u]) { std::swap(container[i], container[u]); } } } }
	inline void LowToHighIntegerSort(std::vector<int>& container) { for (int i = 0; i < container.size() - 1; ++i) { for (int u = i + 1; u < container.size(); ++u) { if (container[i] > container[u]) { std::swap(container[i], container[u]); } } } }
	inline void HighToLowIntegerSort(std::vector<int>& container) { for (int i = 0; i < container.size() - 1; ++i) { for (int u = i + 1; u < container.size(); ++u) { if (container[i] < container[u]) { std::swap(container[i], container[u]); } } } }
	inline void LowToHighCharacterSort(string& container) { for (int i = 0; i < container.size() - 1; ++i) { for (int u = i + 1; u < container.size(); ++u) { if ((int)container[i] > (int)container[u]) { std::swap(container[i], container[u]); } } } }
	inline void HighToLowCharacterSort(string& container) { for (int i = 0; i < container.size() - 1; ++i) { for (int u = i + 1; u < container.size(); ++u) { if ((int)container[i] < (int)container[u]) { std::swap(container[i], container[u]); } } } }
}

namespace Shufflers
{
	inline void RandomCharacterShuffle(string& container) { for (int i = 0; i < container.size(); ++i) { int n(random(container.size() - 1, 0)); std::swap(container[i], container[n]); } }
	inline void LinearCharacterShuffle(string& container) { for (int i = 0; i < container.size() - 1; ++i) { int n(random(container.size() - 1, i + 1)); std::swap(container[i], container[n]); } }
	inline void RandomIntegerShuffle(std::vector<int>& container) { for (int i = 0; i < container.size(); ++i) { int n(random(container.size() - 1, 0)); std::swap(container[i], container[n]); } }
	inline void RandomFloatShuffle(std::vector<float>& container) { for (int i = 0; i < container.size(); ++i) { int n(random(container.size() - 1, 0)); std::swap(container[i], container[n]); } }
	inline void LinearFloatShuffle(std::vector<float>& container) { for (int i = 0; i < container.size() - 1; ++i) { int n(random(container.size() - 1, i + 1)); std::swap(container[i], container[n]); } }
	inline void LinearIntegerShuffle(std::vector<int>& container) { for (int i = 0; i < container.size() - 1; ++i) { int n(random(container.size() - 1, i + 1)); std::swap(container[i], container[n]); } }
};

// these functions(at least currently) provide no real use to the engine and are simply here because i wanted to write them
namespace UselessFunAlgos
{
	inline const int& GetPixel(const int& x, const int& y, const int& width) { return (y * width + x); }
};