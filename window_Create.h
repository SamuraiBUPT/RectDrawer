#pragma once
#include <Windows.h>
#include <string>
//这个要首先安装DirectX SDK  然后在项目设置里面  “VC++目录”->“包含目录”->include 然后“库目录”->"DirectX Lib x86"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")  //记住是d3dx9 不要打错了
#ifndef my_device brush_init_finished 
#define my_device brush_init_finished 
#endif // !brush_init_finished my_device


/*首先是一个建立一个程序(进程)，这个程序里面绘制框，也就是说建立一个窗口。
	当前位置：建立窗口的代码部分*/

//namespace，是指标识符的各种可见范围。C++标准程序库中的所有标识符都被定义于一个名为std的namespace中。

//注意：项目前置属性：C++标准改为C++17
namespace WND {
	class Wind {
	public:
		Wind(std::string_view window_name, std::string_view class_name);//构造函数，同时也是锁定这个进程，针对这个进程创建窗口
		~Wind();
		void init_directX9();
		static LRESULT CALLBACK m_win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);//消息处理函数




		win_rec target_rect;
		IDirect3D9 *my_brush;//IDirect3D9接口是创建Microsoft Direct3D 目标和配置环境的应用方法类。
		IDirect3DDevice9 *brush_init_finished;//应用程序使用 IDirect3DDevice9 接口的方法来执行基于 DrawPrimitive 的渲染、创建资源、使用系统级变量、调整 Gamma 斜坡级别、使用调色板以及创建着色器。


	};


	class win_rec :public RECT {
	public:									//这里记得写public，否则不知道访问权限
		int width() { return right - left; }//right和left都是RECT的属性
		int height() { return bottom - top; }//同上
	};
}

