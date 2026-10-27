#ifndef VERSIONGRAPH_H
#define VERSIONGRAPH_H

#include <vector>
#include <functional>
#include "image.h"

class Node{
// each node contains only one change and it is relative to the previous node.

private:

    //the idea is that each node will simply hold the changes made to the image, not a whole image

    //plan for the functions is that I don't specify the exact amount of parameters but I rather add them in as lambda functions
    std::function<void(Image&)> changeMade;

    Node *prev_;
    vector<Node*> next_;
    float changeValue;

public:

    Node(std::function<void(Image&)> changeFunct, float value);
    Node(std::function<void(Image&)> changeFunct, float value, Node *prev_);
    void addChange(std::function<void(Image&)> changeFunct, float value);


};


class VersionGraph{

private:
    Node* start_;
    Node* curr_;

    Image root_image;
    Image working_image;
public:

    VersionGraph(Image& img) : root_image(img), working_image(img) {};
    void addNode(std::function<void(Image&)> changeFunct);

    Image constructCurrImage();
    vector<std::function<Image&>> vectorOfChanges();
};


#endif // VERSIONGRAPH_H
