#pragma once
#include <string>
#include <Windows.h>
//这个要首先安装DirectX SDK  然后在项目设置里面  “VC++目录”->“包含目录”->include 然后“库目录”->"DirectX Lib x86"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")  //记住是d3dx9 不要打错了


namespace render {
	typedef class DirectX_Render {
	public:
		DirectX_Render(IDirect3DDevice9 *brush_finished_In);
		~DirectX_Render();

		void begin_render();
		void end_render();
		void draw_rect(int x0, int y0, int width, int height, unsigned long color);
		void draw_line(int x0, int y0, int x1, int y1, unsigned long color);//画一个矩阵需要四条线，这里再写一个方法
		void draw_text(std::string_view text, int x0, int y0, unsigned long color);

	private:
		IDirect3DDevice9 *brush_using;//这是这个类单独建立的一个IDirect3DDevice*类型的画笔，在构造函数中会进行赋值
		//绘制线的对象、绘制文字的对象
		ID3DXFont *m_font = nullptr;
		ID3DXLine *m_line = nullptr;
	}dx_render;
}