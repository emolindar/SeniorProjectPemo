#include <vector>
using namespace std;


class Pixel{
public:
	Pixel(float red, float green, float blue){
		rgb_[0]=red;
		rgb_[1]=green;
		rgb_[2]=blue;
	}

	float red(){return rgb_[0];}
	float green(){return rgb_[1];}
	float blue(){return rgb_[2];}
	
	void red(float r){rgb_[0]= r;}
	void blue(float b){rgb_[2]=b;}
	void green(float g){rgb_[1]=g;}
	void rgb(vector<float> rgb){rgb_ = rgb;}

	void setPixel(float r, float g, float b){
		rgb_.clear();
		this->rgb_ = {r,g,b};
	}
private: 
	vector<float> rgb_ = {0,0,0}; 
};



class Image{
public:
	Image(int width,int height) : width_(width), height_(height) {
		for(int i=0; i<width; i++){
			image_data_.emplace_back();
			for(int j=0; j<height; j++){
				image_data_.emplace_back();
			}
		}
	}
	const int width(){return width_;}
	const int height(){return height_;}
	vector<vector<Pixel>> image_data(){return image_data_;}
	Pixel getPixel(int r, int c){
		return image_data_[r][c];
	}

private: 
	int width_ = 0;
	int height_ =0;
	vector<vector<Pixel>> image_data_;
};


