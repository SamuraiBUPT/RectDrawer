#include "render.h"

render::DirectX_Render::DirectX_Render(IDirect3DDevice9 * brush_finished_In)
{
	if (brush_finished_In == NULL)throw std::exception("����δ�ɹ�����");
	brush_using = brush_finished_In;
	if (FAILED(D3DXCreateLine(brush_using, &m_line)))throw std::exception("������ʼ��ʧ�ܣ�");
	if ((D3DXCreateFont(brush_using, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Tahoma", &m_font)) != S_OK)throw std::exception("���ֳ�ʼ��ʧ�ܣ�");

}

render::DirectX_Render::~DirectX_Render()
{
	if (m_line)m_line->Release();
	if (m_font)m_font->Release();
}

void render::DirectX_Render::begin_render()
{
	//���һ���������棬������ȾĿ�ꡢ�����ȾĿ�ꡢģ�建��������Ȼ�������
	brush_using->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.f, 0);
	//��ʼһ��������
	brush_using->BeginScene();
}

void render::DirectX_Render::end_render()
{
	//����ͨ������ IDirect3DDevice9::BeginScene ��ʼ�ĳ�����
	brush_using->EndScene();
	//�����豸ӵ�еĺ�̨��������������һ�������������ݡ�
	brush_using->Present(NULL, NULL, NULL, NULL);
}

void render::DirectX_Render::draw_rect(int x0, int y0, int width, int height, unsigned long color)
{
	draw_line(x0, y0, x0 + width, y0, color);//��һ������
	draw_line(x0, y0, x0, y0 + height, color);//����
	draw_line(x0, y0 + height, x0 + width, y0 + height, color);
	draw_line(x0 + width, y0, x0 + width, y0 + height, color);
}

void render::DirectX_Render::draw_line(int x0, int y0, int x1, int y1, unsigned long color)
{
	D3DXVECTOR2 line[2] = { D3DXVECTOR2(x0,y0),D3DXVECTOR2(x1,y1) };//����Ļ������ͻ����յ�
	m_line->Begin();
	m_line->Draw(line, 2, color);
	m_line->End();

	//����һ���򵥵Ŀ�ͻ����ˣ�������������������������
}

void render::DirectX_Render::draw_text(std::string_view text, int x0, int y0, unsigned long color)
{
	RECT r{ x0,y0,x0,y0 };
	m_font->DrawTextA(NULL, text.data(), -1, &r, DT_NOCLIP, color);

}

