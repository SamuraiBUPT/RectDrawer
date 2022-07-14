#include "window_Create.h"
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")//����һ�����ļ�����ʹ������Ե�����Ӧ�Ķ�̬���ӿ⡣

static HWND target_hwnd;//ע�����static����cpp�ļ����������ģ�������.h,��Ϊ����Ҳ��һ�����ڣ����Ե��ҵ�������ڵĴ��ھ��
static HWND Wind_hwnd;//���ǻ��� �� �� ���� �Ĵ���ָ��
WND::Wind::Wind(std::string_view window_name, std::string_view class_name)
{
	if (window_name.empty() || class_name.empty())throw std::exception("�Ҳ����ý���");//����һ�����У�ȷ�����ҵ��������

	/*���̣�����ע��RegisterClass()
	->���ڴ���CreateWindow()��createWindowEx()
	->������ʾ
	*/
	WNDCLASSEX window_param;//�����RegisterClass()�����Ĳ������������������һϵ�й���
	window_param.cbSize = sizeof WNDCLASSEX;//WNDCLASSEX �Ĵ�С UINT����
	window_param.style = CS_HREDRAW | CS_VREDRAW;//����������������Ĵ��ھ��еķ����'|'���������Ѽ�������һ��
	//HREDRAW:������ڵ�λ�û��ȷ����ı䣬���ػ洰�ڡ�
	//VREDRAW:������ڵ�λ�û�߶ȸı䣬���ػ洰�ڡ�
	//һ�㴰�ڳ��õ�ȡֵΪCS_HREDRAW | CS_VREDRAW
	window_param.cbClsExtra = 0;//ָ�������ڴ�����ṹ��ĸ����ֽ�����
	window_param.cbWndExtra = 0;//ָ�������ڴ���ʵ���ĸ����ֽ�����
	window_param.hInstance = NULL;
	window_param.hIcon = NULL;
	window_param.hCursor = LoadCursor(NULL, IDC_ARROW);//LoadCursor��һ����������Ӧ��������ָ���Ĺ����Դ
	window_param.hbrBackground = HBRUSH(RGB(0, 0, 0));
	window_param.lpszMenuName = "";//����ҲҪ����Ŀ���е�����ʹ�ö��ֽ��ַ��������ʹ��Unicode�ᱨ��
	window_param.lpszClassName = "Wind";//ָ�������Ƶ�ָ�롣
	window_param.hIconSm = NULL;
	if (!RegisterClassExA(&window_param)) {//���ʧ�ܣ�����Ϊ0��Ȼ��ȡ��  תΪ�����������if���
		throw std::exception("����ע��ʧ��");
	}
	//����ע����ɣ����濪ʼ��������
	//CreateWindowEx�ú�����������һ��������չ���Ĳ��ʽ���ڡ�����ʽ���ڻ��Ӵ��ڣ�����������CreateWindow������ͬ��
	//��������HWND

	//FindWindow()
	//�������ܣ��ú������һ�����㴰�ڣ�HWND���ľ�����ô��ڵ������ʹ�������������ַ�����ƥ�䡣��������������Ӵ��ڡ��ڲ���ʱ�����ִ�Сд��
	target_hwnd = FindWindow(class_name.data(), window_name.data());//����Ҫ�ҵ�Ŀ�괰�ڣ��ٷ���������
	
	GetWindowRect(target_hwnd, &target_rect);//GetWindowRect��һ��Windows API������
	/*�ú�������ָ�����ڵı߿���εĴ�С������ԭ��Ϊvoid,
	�ڶ�������ΪlpRect��ָ��һ��RECT�ṹ��ָ�룬�ýṹ���մ��ڵ����ϽǺ����½ǵ���Ļ���ꡣ*/

	Wind_hwnd = CreateWindowExA(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
		"Wind",
		"",
		WS_VISIBLE | WS_POPUP,
		target_rect.left, target_rect.top, target_rect.width(), target_rect.height(),
		NULL, NULL, NULL, NULL
	);
	/*
		��һ��������������չ���
			������WS_EX_TOPMOST  �ô���Ӧ���������з���㴰���Ϸ�������Ӧ�������Ϸ�����ʹ������ͣ��Ҳ����ˡ���Ϊ���Ʒ�����������
			WS_EX_LAYERED:����һ���ֲ㴰��
			WS_EX_TRANSPARENT:ָ���������񴴽��Ĵ����ڴ����µ�ͬ���������ػ�ʱ���ô��ڲſ����ػ���
		�ڶ���������ָ��ע��������ָ��
		������������ָ�򴰿����Ƶ�ָ��
		���ĸ����������ڷ��
			WS_VISIBLE������һ����ʼ״̬Ϊ�ɼ��Ĵ��ڡ�
			WS_POPUP������һ������ʽ���ڡ�
		*/

	//�������֣�������ʾ
	//���²��������ڽ���һЩ��Դ��ڵ����ã�Ŀ����������ȫ͸��
	//�ô�����������ʱ��͸����DwmExtendFrameIntoClientArea()����  ������ͷ�ļ�dwmapi.h
	//�ô��ڵ��ʱ��͸����SetLayeredWindowAttributes()����
	MARGINS m = { target_rect.left, target_rect.top, target_rect.width(), target_rect.height() };
	//MARGINS����ͷ�ļ�dwmapi.h  ����������͸��������
	DwmExtendFrameIntoClientArea(Wind_hwnd, &m);//��һ��������Ŀ�괰�ڵľ�� �ڶ���������͸��������
	SetLayeredWindowAttributes(Wind_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	/*
	��һ��������ָ���ֲ㴰�ھ��
	�ڶ���������ָ����Ҫ͸���ı�����ɫֵ������RGB()��
	����������������͸���ȣ�0��ʾ��ȫ͸����255��ʾ��͸��
	���ĸ�������͸����ʽ LWA_ALPHAʱ��crKey������Ч��bAlpha������Ч��Ҳ����ֻ�е�����������Ч
	*/

	//���ˣ����ڴ�������ɣ����濪ʼ���ƿ�
	init_directX9();
}

void WND::Wind::init_directX9()
{
	my_brush = Direct3DCreate9(D3D_SDK_VERSION);//�˺�������һ��IDirect3D9 ���ʶ���,�Ӷ�֧��ö��,���� IDirect3DDevice9 ����Ĵ���.

	D3DPRESENT_PARAMETERS pp;//����һ�����������������D3DPRESENT_PARAMETERS��ϸ����

	ZeroMemory(&pp, sizeof pp);//������0���һ������
	pp.Windowed = true;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	pp.BackBufferWidth = target_rect.width();
	pp.BackBufferHeight = target_rect.height();
	pp.hDeviceWindow = Wind_hwnd;//��װ�õĴ���ȷ��λ�ú���Ļ�ϵĺ�̨��������С��������Direct3D�ں�̨�����������ݸ��Ƶ�ǰ̨��������
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//ˢ��ģʽ��ȡ����ˢ��

	if ((my_brush->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		Wind_hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&pp,
		&my_device)) != D3D_OK)throw std::exception("Brush��ʼ��ʧ�ܣ�");
		//����һ���豸���������ʾ���Կ���
		/*��һ��������ʾ��ʾ��  Ĭ������ʾ��
		�ڶ���������Ԥ�����͵����������ͣ�˵���˾���ѡ��Ӳ�����ٻ����������
		���������������ĸ����ڻ���
			�������������ˣ����ھ��Ǵ���  ����֮ǰ�����͸��˸����ڳ�������Ϊ����Ҫ�ڴ�������ģ������ȸ㴰�ڣ��ٸ�����ﴫ�봰�ھ��
		���ĸ�������ѡ��������㴦��/Ӳ�����㴦��

		������������һ��DIRECT3DDEVICE9���͵�ָ���������ش������豸 Ҳ����˵ �������˾͸������ָ�� ���ǻ���*/

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
