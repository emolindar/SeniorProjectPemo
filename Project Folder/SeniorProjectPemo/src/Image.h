#pragma once


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

    void setPixel(float r, float g, float b);
private:
    //starting out with a black image ( and initial values, so that those can be changed later on)
	vector<float> rgb_ = {0,0,0}; 
};



class Image{
public:
    Image(int width,int height);

    //deep copy used to create different versions of the image
    Image(const Image& img) : width_(img.width_), height_(img.height_), image_data_(img.image_data_) {}

	int width() const {return width_;}
	int height() const {return height_;}
	vector<vector<Pixel>> image_data(){return image_data_;}

	Pixel getPixel(int r, int c) const {
		return image_data_[r][c];
	}
	void setPixel(int r, int c, Pixel p){
		image_data_[r][c] = p;
	}
private: 
	int width_ = 0;
	int height_ =0;
	vector<vector<Pixel>> image_data_;
};


