#ifndef VERSIONGRAPH_H
#define VERSIONGRAPH_H

#include "Image.h"
#include "algorithms/dowscale.h"
#include <functional>
#include <vector>

/*TODO:
 *
 *  1. Think about adding a caching -- every however many nodes, have the
 * version of the image saved
 */

class Node {
    // each node contains only one change and it is relative to the previous node.

    friend class VersionGraph;
private:
    // the idea is that each node will simply hold the changes made to the image,
    // not a whole image

    //store changes as function pointers
    std::function<void(Image &, float)> changeMade;
    float changeValue;
    std::string name;

    Node *prev_ = nullptr;
    vector<Node *> next_;
public:


    Node();
    Node(std::string name, std::function<void(Image &, float)> changeFunct, float value);
    Node(std::string name, std::function<void(Image &, float)> changeFunct, float value,
         Node *prev_);
    void addChange(std::function<void(Image &, float)> changeFunct, float value);
    Node *next(int index_of_next);
};

class VersionGraph {

private:
    Node *start_;
    Node *curr_;
    Node *root_;

    Image root_image;
    Image working_image;

    Image preview, preview_before_slider;
    int preview_width, preview_height, interpolation_radius;

    std::function<Image( Image&, int,int,int)> downscaling_function_; // default downscaling function is going to be the Lanczos interpolation
    vector<int> index_of_working_image;
    int image_version;

public:
    VersionGraph(Image &img) : root_image(img), working_image(img), preview(downscale_image_bicubic(img, 150,200, 16)), preview_before_slider(downscale_image_bicubic(img, 150,200, 16)){
        root_ = new Node();
        curr_ = root_;
        index_of_working_image.clear();
        image_version = 1;
        downscaling_function_ = downscale_image_bicubic;


        //add functionality to change these
        preview_width = 150;
        preview_height = 150;
        interpolation_radius = 16;
    };

    std::function<Image(Image&, int,int, int)> downscaling_function() { return downscaling_function_;}
    void set_downscaling_function(std::function<Image(Image&, int,int, int)> func);

    Image &getWorkingImage();
    Image &getRootImage() { return root_image; }
    void addNode(std::string name, std::function<void(Image &, float)> changeFunct, float value);

    //functions for preview
    Image& getPreview(){ return preview; }
    void updatePreview( std::function<void(Image&, float)> adjustment_func, float value);
    void refreshPreview() { preview = preview_before_slider;}


    Image constructCurrImage();
    vector<std::function<void(Image &, float)>> vectorOfChanges();
    void next_working_image();
};

#endif // VERSIONGRAPH_H
