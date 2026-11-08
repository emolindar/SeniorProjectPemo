#pragma once


#include <vector>
using namespace std;


class Pixel{
public:
	Pixel(float red, float green, float blue){
        r_=std::clamp(red, 0.0f, 1.0f);
        g_=std::clamp(green, 0.0f, 1.0f);
        b_=std::clamp(blue, 0.0f, 1.0f);
	}

    float red(){return r_;}
    float green(){return g_;}
    float blue(){return b_;}
	
    void red(float r){r_= r;}
    void blue(float b){b_=b;}
    void green(float g){g_=g;}



    void setPixel(float r, float g, float b);
private:
    //starting out with a black image ( and initial values, so that those can be changed later on)
    float r_ = 0, g_ = 0, b_= 0;
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


