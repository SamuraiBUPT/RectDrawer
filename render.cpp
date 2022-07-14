#include "render.h"

render::DirectX_Render::DirectX_Render(IDirect3DDevice9 * brush_finished_In)
{
	if (brush_finished_In == NULL)throw std::exception("画笔未成功传入");
	brush_using = brush_finished_In;
	if (FAILED(D3DXCreateLine(brush_using, &m_line)))throw std::exception("线条初始化失败！");
	if ((D3DXCreateFont(brush_using, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Tahoma", &m_font)) != S_OK)throw std::exception("文字初始化失败！");

}

render::DirectX_Render::~DirectX_Render()
{
	if (m_line)m_line->Release();
	if (m_font)m_font->Release();
}

void render::DirectX_Render::begin_render()
{
	//清除一个或多个表面，例如渲染目标、多个渲染目标、模板缓冲区和深度缓冲区。
	brush_using->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.f, 0);
	//开始一个场景。
	brush_using->BeginScene();
}

void render::DirectX_Render::end_render()
{
	//结束通过调用 IDirect3DDevice9::BeginScene 开始的场景。
	brush_using->EndScene();
	//呈现设备拥有的后台缓冲区序列中下一个缓冲区的内容。
	brush_using->Present(NULL, NULL, NULL, NULL);
}

void render::DirectX_Render::draw_rect(int x0, int y0, int width, int height, unsigned long color)
{
	draw_line(x0, y0, x0 + width, y0, color);//画一条横线
	draw_line(x0, y0, x0, y0 + height, color);//竖线
	draw_line(x0, y0 + height, x0 + width, y0 + height, color);
	draw_line(x0 + width, y0, x0 + width, y0 + height, color);
}

void render::DirectX_Render::draw_line(int x0, int y0, int x1, int y1, unsigned long color)
{
	D3DXVECTOR2 line[2] = { D3DXVECTOR2(x0,y0),D3DXVECTOR2(x1,y1) };//方框的绘制起点和绘制终点
	m_line->Begin();
	m_line->Draw(line, 2, color);
	m_line->End();

	//至此一个简单的框就画好了，这个函数被上面这个函数调用
}

void render::DirectX_Render::draw_text(std::string_view text, int x0, int y0, unsigned long color)
{
	RECT r{ x0,y0,x0,y0 };
	m_font->DrawTextA(NULL, text.data(), -1, &r, DT_NOCLIP, color);

}

