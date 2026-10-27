#ifndef VERSIONGRAPH_H
#define VERSIONGRAPH_H

#include <vector>
#include <functional>
#include "image.h"


/*TODO:
 *
 *  1. Think about adding a caching -- every however many nodes, have the version of the image saved
 *  2. Complete the constructeImage function and make it so that every time a change is applied,
 *   that change is added to the wokring_image through that, not by abusing that image.
 */

class Node{
// each node contains only one change and it is relative to the previous node.

private:

    //the idea is that each node will simply hold the changes made to the image, not a whole image

    //plan for the functions is that I don't specify the exact amount of parameters but I rather add them in as lambda functions
    std::function<void(Image&, float)> changeMade;


    float changeValue;

public:

    Node *prev_;
    vector<Node*> next_;

    Node();
    Node(std::function<void(Image&, float)> changeFunct, float value);
    Node(std::function<void(Image&, float)> changeFunct, float value, Node *prev_);
    void addChange(std::function<void(Image&, float)> changeFunct, float value);
    Node* next(int index_of_next);


};


class VersionGraph{

private:
    Node* start_;
    Node* curr_;
    Node* root_;

    Image root_image;
    Image working_image;
    vector<int> index_of_working_image;
    int image_version;
public:

    VersionGraph(Image& img) : root_image(img), working_image(img) {
        root_ = new Node();
        curr_ = root_;
        index_of_working_image.clear();
        image_version = 1;
    };

    Image& getWorkingImage() {return working_image;}
    Image& getRootImage() {return root_image;}
    void addNode(std::function<void(Image&, float)> changeFunct, float value);

    Image constructCurrImage();
    vector<std::function<void(Image&, float)>> vectorOfChanges();
    void next_working_image();
};


#endif // VERSIONGRAPH_H
