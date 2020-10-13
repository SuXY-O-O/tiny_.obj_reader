#include "Reader.h"
#include "Render.h"
#include "BMPSaver.h"

int main()
{
	char input[100] = "\0";
	double rate;
	int width;
	int height;
	string output_file = "out.bmp";
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
	Render* render = new Render(reader);
	render->compute_render();
	render->depth_to_color();
	string data = render->color_to_string();
	//printf("%s\n", data.c_str());
	unsigned char* color_data = (unsigned char*)data.c_str();
	BMPSaver::MySaveBmp(output_file.c_str(), color_data, width, height);
	return 0;
}