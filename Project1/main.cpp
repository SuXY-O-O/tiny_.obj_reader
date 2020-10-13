#include "Reader.h"
#include "Render.h"
#include "BMPSaver.h"
#include <Windows.h>

int main()
{
	char input[100] = "\0";
	double rate;
	int width;
	int height;
	string output_file = "output.bmp";
	printf("Please input the input file's name, including \".obj\"\n");
	if (scanf("%s.obj", input) == EOF)
	{
		printf("Wrong input\n");
		exit(1);
	}
	printf("%s\n", input);
	printf("Please input the zoom rate(double), output width and height(int), divide with white space\n");
	if (scanf("%lf %d %d", &rate, &width, &height) == EOF)
	{
		printf("Wrong input\n");
		exit(2);
	}
	string input_file;
	input_file.clear();
	input_file += input;
	//input_file += ".obj";
	Reader *reader = new Reader(input_file, rate, width, height);
	if (!reader->begin_read_file())
	{
		printf("Wrong when read file\n");
		exit(3);
	}
	printf("Computing using z_buffer\n");
	Render* render = new Render(reader);
	render->compute_render();
	printf("Operating color\n");
	render->depth_to_color();
	string data = render->color_to_string();
	printf("Saving to file\n");
	//printf("%s\n", data.c_str());
	unsigned char* color_data = (unsigned char*)data.c_str();
	/*output_file.clear();
	output_file += input_file;
	output_file += "_" + to_string(width) + "*" + to_string(height);
	output_file += ".bmp";*/
	BMPSaver::MySaveBmp(output_file.c_str(), color_data, width, height);
	//
	printf("Printint to screen\n");
	HWND wnd;									//窗口句柄
	HINSTANCE hInstance;						//事件句柄
	HDC dc;										//绘图设备环境句柄
	/*wnd = CreateWindowExA(
		NULL,
		input_file.c_str(),						//更具注册的名称找到注册窗口类的数据
		input_file.c_str(),						//窗口的标题
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
												//窗口的风格
		0,										//位置
		0,
		width,									//宽高
		height,
		0,										//菜单
		0,										//父窗口
		NULL,									//句柄    
		0										//用户自定义的变量
	);		*/									//获取窗口句柄
	wnd = GetForegroundWindow();
	dc = GetDC(wnd);							//获取绘图设备
	int pix;
	int ix, iy;
	int r, g, b;
	vector<vector<tuple<int, int, int> >* >* picture = render->get_color_table();	
	for (iy = 0; iy < height; iy++)
	{
		for (ix = 0; ix < width; ix++)
		{
			tie(r, g, b) = (*(*picture)[iy])[ix];
			pix = RGB(255 - r, 255 - g, 255 - b);
			SetPixel(dc, ix, iy, pix);
		}
	}
	UpdateWindow(wnd);
	ShowWindow(wnd, SW_SHOW);
	return 0;
}