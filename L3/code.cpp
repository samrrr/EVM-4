#undef _DEBUG

#define _USE_MATH_DEFINES
#include <cmath>
#include "usefull.h"
#include <stdlib.h>



#define _DEBUG

#define EL_K 0
#define EL_H 1
#define EL_R 2
#define EL_I 3
#define EL_U 4
#define EL_L 5
#define EL_C 6

#define SIGN_T 0
#define SIGN_V 1
#define SIGN_Z 100

//using namespace itl;
using namespace std;

string sympy_sim(string funct);
string sympy_lap(string funct);
string sympy_eva(string f1, string f2, string f3);
string sympy_dif(string f1, string f2);

#define cout std::cout














struct W_MOUSE
{
	int x, y;
	int mchl, mchr, mdol, mdor;
};


#define LOWORD(f) ((f)&0xffff) 
#define HIWORD(f) (((f)>>16)&0xffff) 

class OPENGL_WINDOW
{
private:

	HDC			hDC = NULL;		// Private GDI Device Context
	HGLRC		hRC = NULL;		// Permanent Rendering Context
	HWND		hWnd = NULL;		// Holds Our Window Handle
	HINSTANCE	hInstance;		// Holds The Instance Of The Application

	bool	keys[256];			// Array Used For The Keyboard Routine
	bool	active = true;		// Window Active Flag Set To true By Default
	bool	fullscreen = true;	// Fullscreen Flag Set To Fullscreen Mode By Default

	bool enabled;

	bool initialise;

	int razokx;
	int razoky;
	float pers_angle;
	W_MOUSE mouse;

	//static LRESULT CALLBACK WinMessage(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam);	
	//LRESULT CALLBACK WinMessage(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam);

	static LRESULT CALLBACK WndProcS(HWND _window, unsigned int _message, WPARAM _wParam, LPARAM _lParam)
	{
		OPENGL_WINDOW* application = 0;

		if (_message == WM_NCCREATE)
			application = (OPENGL_WINDOW*)(_lParam);
		else
			application = (OPENGL_WINDOW*)GetWindowLongPtr(_window, GWLP_USERDATA);

		//application = (OPENGL_WINDOW*)((long)application & 0x00000000ffffffff);

		return (application)->WndProc(_window, _message, _wParam, _lParam);
	}
	LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
		UINT	uMsg,			// Message For This Window
		WPARAM	wParam,			// Additional Message Information
		LPARAM	lParam)			// Additional Message Information
	{
		if (this)
		switch (uMsg)									// Check For Windows Messages
		{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active = true;						// Program Is Active
			}
			else
			{
				//disable();
				//enable();
				active = false;						// Program Is No Longer Active
			}

			return 0;
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
			case SC_SCREENSAVE:					// Screensaver Trying To Start?
			case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				//rly this your problem....
				return 0;
			}
			break;
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			disable();
			return 0;
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			if (wParam >= 0 && wParam < 256)
			keys[wParam] = true;					// If So, Mark It As true
			if (wParam == 910)
			{
				//disable();
				//enable();
			}
			return 0;

		}
		case WM_ACTIVATEAPP:
		{
			break;
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As false
			return 0;
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			int g = lParam;
			razokx = LOWORD(lParam);
			razoky = HIWORD(lParam);
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
			break;
		}

		case WM_MOUSEMOVE:
			mouse.x = LOWORD(lParam);
			mouse.y = HIWORD(lParam);
			break;
		case WM_LBUTTONDOWN:
			mouse.mdol = 1;
			mouse.mchl = 1;
			break;
		case WM_LBUTTONUP:
			mouse.mchl = 0;
			break;
		case WM_RBUTTONDOWN:
			mouse.mdor = 1;
			mouse.mchr = 1;
			break;
		case WM_RBUTTONUP:
			mouse.mchr = 0;
			break;

		}

		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
	{
		if (height == 0)										// Prevent A Divide By Zero By
		{
			height = 1;										// Making Height Equal One
		}

		glViewport(0, 0, width, height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(90.0f, (GLfloat)width / (GLfloat)height, 0.1, 10000.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

	int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
	{
		for (int i = 0; i < 256;i++)
			keys[i] = 0;

		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(1.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glAlphaFunc(GL_GREATER, 0.99f);
		glDisable(GL_ALPHA_TEST);

		return true;										// Initialization Went OK
	}

	int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
	{

		return true;										// Everything Went OK
	}

	GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
	{
		if (fullscreen)										// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
			ShowCursor(true);								// Show Mouse Pointer
		}

		if (hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			}
			hRC = NULL;										// Set RC To NULL
		}

		if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			hDC = NULL;										// Set DC To NULL
		}

		if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			hWnd = NULL;										// Set hWnd To NULL
		}

		if (!UnregisterClass(L"OpenGL", hInstance))			// Are We Able To Unregister Class
		{
			MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			hInstance = NULL;									// Set hInstance To NULL
		}
	}

	/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
	*	title			- Title To Appear At The Top Of The Window				*
	*	width			- Width Of The GL Window Or Fullscreen Mode				*
	*	height			- Height Of The GL Window Or Fullscreen Mode			*
	*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
	*	fullscreenflag	- Use Fullscreen Mode (true) Or Windowed Mode (false)	*/

	BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
	{
		GLuint		PixelFormat;			// Holds The Results After Searching For A Match
		WNDCLASS	wc;						// Windows Class Structure
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left = (long)0;			// Set Left Value To 0
		WindowRect.right = (long)width;		// Set Right Value To Requested Width
		WindowRect.top = (long)0;				// Set Top Value To 0
		WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

		fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

		hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc = (WNDPROC)WndProcS;					// WndProc Handles Messages
		wc.cbClsExtra = 0;									// No Extra Window Data
		wc.cbWndExtra = 0;									// No Extra Window Data
		wc.hInstance = hInstance;							// Set The Instance
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground = NULL;									// No Background Required For GL
		wc.lpszMenuName = NULL;									// We Don't Want A Menu
		wc.lpszClassName = L"OpenGL";								// Set The Class Name

		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			MessageBox(NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;											// Return false
		}

		if (fullscreen)												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
			dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
			dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
				if (MessageBox(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				{
					fullscreen = false;		// Windowed Mode Selected.  Fullscreen = false
				}
				else
				{
					// Pop Up A Message Box Letting User Know The Program Is Closing.
					MessageBox(NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
					return false;									// Return false
				}
			}
		}

		if (fullscreen)												// Are We Still In Fullscreen Mode?
		{
			dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
			dwStyle = WS_POPUP;										// Windows Style
			ShowCursor(false);										// Hide Mouse Pointer
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);		// Adjust Window To true Requested Size

		// Create The Window
		if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
			L"OpenGL",							// Class Name
			L"title not work",								// Window Title
			dwStyle |							// Defined Window Style
			WS_CLIPSIBLINGS |					// Required Window Style
			WS_CLIPCHILDREN,					// Required Window Style
			0, 0,								// Window Position
			WindowRect.right - WindowRect.left,	// Calculate Window Width
			WindowRect.bottom - WindowRect.top,	// Calculate Window Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			hInstance,							// Instance
			this)))
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (long long)this);

		static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		ShowWindow(hWnd, SW_SHOW);						// Show The Window
		SetForegroundWindow(hWnd);						// Slightly Higher Priority
		SetFocus(hWnd);									// Sets Keyboard Focus To The Window
		ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

		if (!InitGL())									// Initialize Our Newly Created GL Window
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL, L"Initialization Failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return false
		}

		return true;									// Success
	}




public:
	OPENGL_WINDOW()
	{
		enabled = 0;
	}
	~OPENGL_WINDOW()
	{
		disable();
	}

	void draw()
	{
		DrawGLScene();
		SwapBuffers(hDC);
	}
	void upd()
	{
		mouse.mdol = 0;
		mouse.mdor = 0;
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool enable()
	{
		if (enabled)
			return 0;

		enabled = 1;

		hDC = NULL;
		hRC = NULL;
		hWnd = NULL;

		active = true;

		fullscreen = false;
		CreateGLWindow("NeHe's OpenGL Framework", 640, 480, 16, fullscreen);
		razokx = 640; razoky = 480;

		//fullscreen = true;
		//CreateGLWindow("NeHe's OpenGL Framework", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 16, fullscreen);


		return 1;
	}
	bool is_enabled()
	{
		return enabled;
	}
	bool disable()
	{
		if (!enabled)
			return 0;
		enabled = 0;
		KillGLWindow();
		return 1;
	}
	int get_wx()
	{
		return razokx;
	}
	int get_wy()
	{
		return razoky;
	}
	float get_angle()
	{
		return 60;
	}
	const bool* get_keys()
	{
		return keys;
	}
	W_MOUSE get_mouse()
	{
		return mouse;
	}

};

struct COLOR
{
	float r, g, b, a;
};







class EASY_TEX
{
private:
	vector<byte> data;
	GLuint texID;
	int init;
	int x, y;
public:
	EASY_TEX()
	{
		init = 0;
		x = 0; y = 0; texID = 0;
	}
	GLuint ID()
	{
		return texID;
	}
	int gx()
	{
		return x;
	}
	int gy()
	{
		return y;
	}
	void resize(int _x, int _y)
	{
		if (_x > 100000)_x = 100000;
		if (_y > 100000)_y = 100000;
		int xx = 1;
		int yy = 1;
		while (xx<_x)
			xx = xx << 1;
		while (yy < _y)
			yy = yy << 1;
		x = xx;
		y = yy;

		data.resize(x * y * 3);

		for (int i = 0; i < data.size(); i++)
		{
			data[i] = ((i/3+i/3/x)%2)<<7;//шахматное поле
		}
	}
	void setpixel(int _x, int _y, byte _r, byte _g, byte _b)
	{
		if (_x < 0 || _y < 0 || _x >= x || _y >= y)
		{
			//cout << "draw error ";
		}
		else
		{
			data[(_x + _y*x) * 3+0] = _r;
			data[(_x + _y*x) * 3+1] = _g;
			data[(_x + _y*x) * 3+2] = _b;
			stringstream ss;
			int a = data[(_x + _y*x) * 3 + 0];
			int b= data[(_x + _y*x) * 3 + 1];
			int c = data[(_x + _y*x) * 3 +2];

		}
	}

	void numbers(int px, int py, string s)
	{
		vector<int> n_d[12][5];

		int i;

		i = 0;
		n_d[i][0] = { 0, 1, 0 };
		n_d[i][1] = { 1, 0, 1 };
		n_d[i][2] = { 1, 0, 1 };
		n_d[i][3] = { 1, 0, 1 };
		n_d[i][4] = { 0, 1, 0 };

		i++;
		n_d[i][0] = { 0, 1, 0 };
		n_d[i][1] = { 1, 1, 0 };
		n_d[i][2] = { 0, 1, 0 };
		n_d[i][3] = { 0, 1, 0 };
		n_d[i][4] = { 1, 1, 1 };

		i++;
		n_d[i][0] = { 1, 1, 0 };
		n_d[i][1] = { 0, 0, 1 };
		n_d[i][2] = { 0, 0, 1 };
		n_d[i][3] = { 0, 1, 0 };
		n_d[i][4] = { 1, 1, 1 };

		i++;
		n_d[i][0] = { 1, 1, 0 };
		n_d[i][1] = { 0, 0, 1 };
		n_d[i][2] = { 0, 1, 0 };
		n_d[i][3] = { 0, 0, 1 };
		n_d[i][4] = { 1, 1, 0 };

		i++;
		n_d[i][0] = { 1, 0, 1 };
		n_d[i][1] = { 1, 0, 1 };
		n_d[i][2] = { 1, 1, 1 };
		n_d[i][3] = { 0, 0, 1 };
		n_d[i][4] = { 0, 0, 1 };

		i++;
		n_d[i][0] = { 1, 1, 1 };
		n_d[i][1] = { 1, 0, 0 };
		n_d[i][2] = { 1, 1, 0 };
		n_d[i][3] = { 0, 0, 1 };
		n_d[i][4] = { 1, 1, 0 };

		i++;
		n_d[i][0] = { 0, 1, 0 };
		n_d[i][1] = { 1, 0, 0 };
		n_d[i][2] = { 1, 1, 0 };
		n_d[i][3] = { 1, 0, 1 };
		n_d[i][4] = { 0, 1, 0 };

		i++;
		n_d[i][0] = { 1, 1, 1 };
		n_d[i][1] = { 0, 0, 1 };
		n_d[i][2] = { 0, 0, 1 };
		n_d[i][3] = { 0, 1, 0 };
		n_d[i][4] = { 0, 1, 0 };

		i++;
		n_d[i][0] = { 0, 1, 0 };
		n_d[i][1] = { 1, 0, 1 };
		n_d[i][2] = { 0, 1, 0 };
		n_d[i][3] = { 1, 0, 1 };
		n_d[i][4] = { 0, 1, 0 };

		i++;
		n_d[i][0] = { 0, 1, 0 };
		n_d[i][1] = { 1, 0, 1 };
		n_d[i][2] = { 0, 1, 1 };
		n_d[i][3] = { 0, 0, 1 };
		n_d[i][4] = { 0, 1, 0 };

		i++;
		n_d[i][0] = { 0, 0, 0 };
		n_d[i][1] = { 0, 0, 0 };
		n_d[i][2] = { 0, 0, 0 };
		n_d[i][3] = { 0, 0, 0 };
		n_d[i][4] = { 0, 0, 1 };

		i++;
		n_d[i][0] = { 0, 0, 0 };
		n_d[i][1] = { 0, 0, 0 };
		n_d[i][2] = { 1, 1, 1 };
		n_d[i][3] = { 0, 0, 0 };
		n_d[i][4] = { 0, 0, 0 };

		int offs = 0;

		for (int i = 0; i < s.length(); i++)
		{
			int id = s[i]-'0';
			if (id < 0 || id >= 10)
			{
				id = -1;
				if (s[i] == '.' || s[i] == ',')
					id = 10,offs+=2;
				if (s[i] == '-')
					id = 11;
			}
			if (id>=0)
			for (int r = 0; r < 10; r++)
				for (int o = 0; o < 6; o++)
					if (n_d[id][r/2][o/2])
					{
						setpixel(px+i*4*2+o-offs*2,py-r,255,255,255);
					}
		}


	}

	void line(double x1, double y1, double x2, double y2, int _r=255, int _g=255, int _b=255)
	{
		if ((y1 - y2) * 0 != 0 || (x1 - x2) * 0 != 0)
			return;

		if (abs(x1 - x2) > abs(y1 - y2))//горизонтальная линия
		{
			double a = (y2-y1)/(x2-x1);
			double b = y1-a*x1;



			//y=ax+b

			if (x1 < 0)
			{
				x1 = 0;
				y1 = b;
			}
			if (x1 > x)
			{
				x1 = x;
				y1 = a*x+b;
			}


			if (y1 < 0 && y2 < 0 || y1 > y && y2 > y)
				return;
			int maxi = abs(x1 - x2)+1;
			for (int i = 0; i <= maxi; i++)
			{
				setpixel(x1*i / maxi + x2*(maxi - i) / maxi, y1*i / maxi + y2*(maxi - i) / maxi, _r, _g, _b);
			}

		}
		else
		{
			double a = (x2 - x1) / (y2 - y1);
			double b = x1 - a*y1;



			//x=ay+b

			if (y1 < 0)
			{
				y1 = 0;
				x1 = b;
			}
			if (y1 > y)
			{
				y1 = y;
				x1 = a*y + b;
			}


			if (x1 < 0 && x2 < 0 || x1 > x && x2 > x)
				return;
			int maxi = abs(y1 - y2)+1;
			for (int i = 0; i <= maxi; i++)
			{
				setpixel(x1*i / maxi + x2*(maxi - i) / maxi, y1*i / maxi + y2*(maxi - i) / maxi, _r,_g,_b);
			}

		}
	}

	void gen_broken_tex()
	{
		init = 0;
		gentex();
	}
	void gentex()
	{
		if (init)
			return;
		init = 1;
		if (texID)
		{
			glDeleteTextures(1, &texID);
		}
		// Build A Texture From The Data
		glGenTextures(1, &texID);						// Generate OpenGL texture IDs
		int fff = GL_LINEAR;
		glBindTexture(GL_TEXTURE_2D, texID);				// Bind Our Texture
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, fff);	// Linear Filtered
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, fff);	// Linear Filtered


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

	}


};

mutex mtx;

class OUTPUT_IMAGES
{
public:
	vector < EASY_TEX > a;
	void add(EASY_TEX& tt)
	{
		a.push_back(tt);
	}
};



float pow1(double x, double y)
{
	return exp(y* log(x));
}
double pow10(int y)
{
	if (y >= 0)
	{
		double b = 1;
		for (int i = 0; i < y; i++)
			b = b * 10;
		return b;
	}
	else
	{
		return 1 / pow10(-y);
	}
}


EASY_TEX create_plot(int tx, int ty, vector<double> vx, vector<double> vy, double min_x, double max_x, double min_y, double max_y)
{


	if (min_x == min_y)
	{
		max_x = vx[0];
		min_x = vx[0];
		max_y = vy[0];
		min_y = vy[0];
		for (int i = 0; i < vx.size(); i++)
		{
			if (vx[i] < min_x)
				min_x = vx[i];
			if (vx[i] > max_x)
				max_x = vx[i];

			if (vy[i] < min_y)
				min_y = vy[i];
			if (vy[i] > max_y)
				max_y = vy[i];
		}

		min_x -= (max_x - min_x)*0.1;
		min_y -= (max_y - min_y)*0.1;
		max_x += (max_x - min_x)*2/tx;
		max_y += (max_y - min_y)*2/ty;

	}


	EASY_TEX tex;
	tex.resize(tx, ty);

	for (int i = 0; i < tx; i++)
		for (int r = 0; r < ty; r++)
			tex.setpixel(i, r, 30, 30, 30);


	//теперь выведем циферки и оси

	double sx, sy;
	int porx,pory;
	sx = abs(max_x - min_x);
	sy = abs(max_y - min_y);

	porx = 10;//10^10
	while (sx/pow1((double)10, porx) < 3 && porx < 100)
		porx--;
	pory = 10;//10^10
	while (sy/pow1((double)10, pory) < 3 && pory < 100)
		pory--;



	float mnox = pow10(porx);
	float mnoy = pow10(pory);

	if (mnox != 0)
	for (int i = min_x / mnox; i < max_x / mnox; i++)
	{
		tex.line((i * mnox - min_x) / sx * tx, 40, (i * mnox - min_x) / sx * tx, ty, 100, 100, 100);

		if ((i * mnox - min_x) / sx * tx>20 && (i * mnox - min_x) / sx * tx < tx - 30)
			tex.numbers((i * mnox - min_x) / sx * tx - 3, 20, ftos(i*mnox));
	}
	if (mnoy!=0)
	for (int i = min_y / mnoy; i < max_y / mnoy; i++)
	{
		tex.line(20, (i * mnoy - min_y) / sy * ty, tx, (i * mnoy - min_y) / sy * ty, 100, 100, 100);

		if ((i * mnoy - min_y) / sy * ty>20 && (i * mnoy - min_y) / sy * ty < ty - 30)
			tex.numbers(4, (i * mnoy - min_y) / sy * ty + 5, ftos(i*mnoy));
	}
	//tex.numbers(30, 20, "111111");


	for (int i = 1; i <vx.size(); i++)
	{
		double rx[2], ry[2];
		double px[2], py[2];

		for (int r = 0; r < 2; r++)
		{
			rx[r] = vx[i - 1 + r];
			ry[r] = vy[i - 1 + r];
			px[r] = (rx[r] - min_x) / (max_x - min_x)*tx;
			py[r] = (ry[r] - min_y) / (max_y - min_y)*ty;
		}
		tex.line(px[0], py[0], px[1], py[1]);
		//tex.numbers(px[0], py[0],ftos(py[0]));
	}

	return tex;
}


int main()
{
	setlocale(0, "RU");

	OPENGL_WINDOW w;

	vector<double> vx(1000);
	vector<double> vy(1000);

	double a, b;

	a = M_PI / 2;
	b = M_PI * 5;

	for (int i = 0; i < vx.size(); i++)
	{
		vx[i] = a + (b - a)* i / (vx.size() - 1);
		vy[i] = sin(vx[i])*sin(vx[i]) + cos(vx[i])*cos(vx[i])*cos(vx[i]);
	}

	w.enable();
	auto tex = create_plot(512, 512, vx, vy,-1,4,-1,5);

	tex.gentex();

	while (1)
	{

		glColor3f(0.3, 0.3, 0.3);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glClearColor(0, 0, 0, 0);


		glColor3f(1, 1, 1);


		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex.ID());


		int wx = w.get_wx(), wy = w.get_wy();

		int tx, ty;
		tx = tex.gx();
		ty = tex.gy();

		glBegin(GL_POLYGON);
		glTexCoord2d(0, 1); glVertex3f(-wx + 000000, wy - 000000, -wy);
		glTexCoord2d(1, 1); glVertex3f(-wx + tx * 2, wy - 000000, -wy);
		glTexCoord2d(1, 0); glVertex3f(-wx + tx * 2, wy - ty * 2, -wy);
		glTexCoord2d(0, 0); glVertex3f(-wx + 000000, wy - ty * 2, -wy);
		glEnd();


		glDisable(GL_TEXTURE_2D);


		Sleep(10);
		w.draw();
		w.upd();
	}

	return 0;
}




