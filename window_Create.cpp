#include "window_Create.h"
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")//导入一个库文件，以使程序可以调用相应的动态链接库。

static HWND target_hwnd;//注意这个static是在cpp文件里面声明的，而不是.h,因为进程也是一个窗口，所以得找到这个窗口的窗口句柄
static HWND Wind_hwnd;//我们绘制 框 的 进程 的窗口指针
WND::Wind::Wind(std::string_view window_name, std::string_view class_name)
{
	if (window_name.empty() || class_name.empty())throw std::exception("找不到该进程");//先来一个特判，确定能找到这个进程

	/*流程：窗口注册RegisterClass()
	->窗口创建CreateWindow()和createWindowEx()
	->窗口显示
	*/
	WNDCLASSEX window_param;//这个是RegisterClass()函数的参数，依靠这个来进行一系列构建
	window_param.cbSize = sizeof WNDCLASSEX;//WNDCLASSEX 的大小 UINT类型
	window_param.style = CS_HREDRAW | CS_VREDRAW;//从这个窗口类派生的窗口具有的风格。用'|'操作符来把几个风格或到一起。
	//HREDRAW:如果窗口的位置或宽度发生改变，将重绘窗口。
	//VREDRAW:如果窗口的位置或高度改变，将重绘窗口。
	//一般窗口常用的取值为CS_HREDRAW | CS_VREDRAW
	window_param.cbClsExtra = 0;//指定紧跟在窗口类结构后的附加字节数。
	window_param.cbWndExtra = 0;//指定紧跟在窗口实例的附加字节数。
	window_param.hInstance = NULL;
	window_param.hIcon = NULL;
	window_param.hCursor = LoadCursor(NULL, IDC_ARROW);//LoadCursor是一个函数，从应用中载入指定的光标资源
	window_param.hbrBackground = HBRUSH(RGB(0, 0, 0));
	window_param.lpszMenuName = "";//这里也要对项目进行调整：使用多字节字符集，如果使用Unicode会报错
	window_param.lpszClassName = "Wind";//指向类名称的指针。
	window_param.hIconSm = NULL;
	if (!RegisterClassExA(&window_param)) {//如果失败，返回为0，然后取逆  转为正，进入这个if语句
		throw std::exception("窗口注册失败");
	}
	//窗口注册完成，下面开始创建窗口
	//CreateWindowEx该函数用来创建一个具有扩展风格的层叠式窗口、弹出式窗口或子窗口，其他功能与CreateWindow函数相同。
	//函数类型HWND

	//FindWindow()
	//函数功能：该函数获得一个顶层窗口（HWND）的句柄，该窗口的类名和窗口名与给定的字符串相匹配。这个函数不查找子窗口。在查找时不区分大小写。
	target_hwnd = FindWindow(class_name.data(), window_name.data());//首先要找到目标窗口，再返回这个句柄
	
	GetWindowRect(target_hwnd, &target_rect);//GetWindowRect是一个Windows API函数。
	/*该函数返回指定窗口的边框矩形的大小，函数原型为void,
	第二个参数为lpRect：指向一个RECT结构的指针，该结构接收窗口的左上角和右下角的屏幕坐标。*/

	Wind_hwnd = CreateWindowExA(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
		"Wind",
		"",
		WS_VISIBLE | WS_POPUP,
		target_rect.left, target_rect.top, target_rect.width(), target_rect.height(),
		NULL, NULL, NULL, NULL
	);
	/*
		第一个参数：窗口扩展风格
			我们用WS_EX_TOPMOST  该窗口应放置在所有非最顶层窗口上方，并且应保持其上方，即使窗口已停用也是如此。因为绘制方框，是这样的
			WS_EX_LAYERED:创建一个分层窗口
			WS_EX_TRANSPARENT:指定以这个风格创建的窗口在窗口下的同属窗口已重画时，该窗口才可以重画。
		第二个参数：指向注册类名的指针
		第三个参数：指向窗口名称的指针
		第四个参数：窗口风格
			WS_VISIBLE：创建一个初始状态为可见的窗口。
			WS_POPUP：创建一个弹出式窗口。
		*/

	//第三部分：窗口显示
	//以下操作都是在进行一些针对窗口的设置，目的是让其完全透明
	//让窗口用起来的时候透明：DwmExtendFrameIntoClientArea()函数  隶属于头文件dwmapi.h
	//让窗口点的时候透明：SetLayeredWindowAttributes()函数
	MARGINS m = { target_rect.left, target_rect.top, target_rect.width(), target_rect.height() };
	//MARGINS属于头文件dwmapi.h  这里是设置透明的区域
	DwmExtendFrameIntoClientArea(Wind_hwnd, &m);//第一个参数：目标窗口的句柄 第二个参数：透明的区域
	SetLayeredWindowAttributes(Wind_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	/*
	第一个参数：指定分层窗口句柄
	第二个参数：指定需要透明的背景颜色值，可用RGB()宏
	第三个参数：设置透明度，0表示完全透明，255表示不透明
	第四个参数：透明方式 LWA_ALPHA时：crKey参数无效，bAlpha参数有效；也就是只有第三个参数有效
	*/

	//至此，窗口创建已完成，下面开始绘制框
	init_directX9();
}

void WND::Wind::init_directX9()
{
	my_brush = Direct3DCreate9(D3D_SDK_VERSION);//此函数创建一个IDirect3D9 画笔对象,从而支持枚举,允许 IDirect3DDevice9 对象的创建.

	D3DPRESENT_PARAMETERS pp;//这是一个参数对象，下面进行D3DPRESENT_PARAMETERS详细设置

	ZeroMemory(&pp, sizeof pp);//首先用0填充一块区域
	pp.Windowed = true;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	pp.BackBufferWidth = target_rect.width();
	pp.BackBufferHeight = target_rect.height();
	pp.hDeviceWindow = Wind_hwnd;//该装置的窗口确定位置和屏幕上的后台缓冲区大小。这是用Direct3D在后台缓冲区的内容复制到前台缓冲区中
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//刷新模式，取立即刷新

	if ((my_brush->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		Wind_hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&pp,
		&my_device)) != D3D_OK)throw std::exception("Brush初始化失败！");
		//创建一个设备对象代表显示的显卡。
		/*第一个参数表示显示器  默认主显示器
		第二个参数：预设类型的驱动器类型，说白了就是选择硬件加速还是软件加速
		第三个参数：在哪个窗口绘制
			到了这里清晰了：窗口就是窗口  我们之前搞半天就搞了个窗口出来，因为框是要在窗口里面的，所以先搞窗口，再搞框，这里传入窗口句柄
		第四个参数：选择软件顶点处理/硬件顶点处理

		第六个参数：一个DIRECT3DDEVICE9类型的指针用来返回创建的设备 也就是说 创建好了就给到这个指针 就是画笔*/

}

LRESULT WND::Wind::m_win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_DESTROY:
		exit(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}
