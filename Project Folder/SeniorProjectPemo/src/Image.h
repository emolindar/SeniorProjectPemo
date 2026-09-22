#include <vector>
using namespace std;
class Image{
public:
	Image(int width,int height) : width_(width), height_(height) {}
	int width(){return width_;}
	int height(){return height_;}
	vector<Pixel> image_data(){return image_data_;}

private: 
	int width_ = 0;
	int height_ =0;
	vector<Pixel> image_data_;
};



class Pixel{
public:
	Pixel(float red, float green, float blue, float opacity){}

private: 
	vector<float> rgb_; 
	float opacity_; 
};
