#pragma once
#include <string>
#include <Windows.h>
//���Ҫ���Ȱ�װDirectX SDK  Ȼ������Ŀ��������  ��VC++Ŀ¼��->������Ŀ¼��->include Ȼ�󡰿�Ŀ¼��->"DirectX Lib x86"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")  //��ס��d3dx9 ��Ҫ�����


namespace render {
	typedef class DirectX_Render {
	public:
		DirectX_Render(IDirect3DDevice9 *brush_finished_In);
		~DirectX_Render();

		void begin_render();
		void end_render();
		void draw_rect(int x0, int y0, int width, int height, unsigned long color);
		void draw_line(int x0, int y0, int x1, int y1, unsigned long color);//��һ��������Ҫ�����ߣ�������дһ������
		void draw_text(std::string_view text, int x0, int y0, unsigned long color);

	private:
		IDirect3DDevice9 *brush_using;//��������൥��������һ��IDirect3DDevice*���͵Ļ��ʣ��ڹ��캯���л���и�ֵ
		//�����ߵĶ��󡢻������ֵĶ���
		ID3DXFont *m_font = nullptr;
		ID3DXLine *m_line = nullptr;
	}dx_render;
}