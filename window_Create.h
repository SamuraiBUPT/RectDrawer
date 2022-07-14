#pragma once
#include <Windows.h>
#include <string>
//���Ҫ���Ȱ�װDirectX SDK  Ȼ������Ŀ��������  ��VC++Ŀ¼��->������Ŀ¼��->include Ȼ�󡰿�Ŀ¼��->"DirectX Lib x86"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")  //��ס��d3dx9 ��Ҫ�����
#ifndef my_device brush_init_finished 
#define my_device brush_init_finished 
#endif // !brush_init_finished my_device


/*������һ������һ������(����)���������������ƿ�Ҳ����˵����һ�����ڡ�
	��ǰλ�ã��������ڵĴ��벿��*/

//namespace����ָ��ʶ���ĸ��ֿɼ���Χ��C++��׼������е����б�ʶ������������һ����Ϊstd��namespace�С�

//ע�⣺��Ŀǰ�����ԣ�C++��׼��ΪC++17
namespace WND {
	class Wind {
	public:
		Wind(std::string_view window_name, std::string_view class_name);//���캯����ͬʱҲ������������̣����������̴�������
		~Wind();
		void init_directX9();
		static LRESULT CALLBACK m_win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);//��Ϣ������




		win_rec target_rect;
		IDirect3D9 *my_brush;//IDirect3D9�ӿ��Ǵ���Microsoft Direct3D Ŀ������û�����Ӧ�÷����ࡣ
		IDirect3DDevice9 *brush_init_finished;//Ӧ�ó���ʹ�� IDirect3DDevice9 �ӿڵķ�����ִ�л��� DrawPrimitive ����Ⱦ��������Դ��ʹ��ϵͳ������������ Gamma б�¼���ʹ�õ�ɫ���Լ�������ɫ����


	};


	class win_rec :public RECT {
	public:									//����ǵ�дpublic������֪������Ȩ��
		int width() { return right - left; }//right��left����RECT������
		int height() { return bottom - top; }//ͬ��
	};
}

