#include <vector>

class Image{
public:
	Image(width, height) : width_(width), height_(height) {}
	int width(){return width_;}
	int height(){return height_;}
	vector<vector<float>> image_data(){return image_data_;}	

}
