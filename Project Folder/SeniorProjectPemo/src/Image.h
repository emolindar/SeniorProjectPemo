#include <vector>
using namespace std;


class Pixel{
public:
	Pixel(int red, int green, int blue){
		rgb_[0]=red;
		rgb_[1]=green;
		rgb_[2]=blue;
	}

	int red(){return rgb_[0];}
	int green(){return rgb_[1];}
	int blue(){return rgb_[2];}
	
	void red(int r){rgb_[0]= r;}
	void blue(int b){rgb_[2]=b;}
	void green(int g){rgb_[1]=g;}
	void rgb(vector<int> rgb){rgb_ = rgb;}

private: 
	vector<int> rgb_ = {0,0,0}; 
};



class Image{
public:
	Image(int width,int height) : width_(width), height_(height) {}
	const int width(){return width_;}
	const int height(){return height_;}
	vector<Pixel> image_data(){return image_data_;}

private: 
	int width_ = 0;
	int height_ =0;
	vector<Pixel> image_data_;
};


