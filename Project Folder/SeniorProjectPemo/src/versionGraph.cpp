#include "versionGraph.h"


Node::Node(std::function<void(Image&)> changeFunct, float value){
    changeMade = changeFunct;
    changeValue = value;
}

Node::Node(std::function<void(Image&)> changeFunct, float value, Node *prev){
    changeMade = changeFunct;
    changeValue = value;
    prev_ = prev;
}



void Node::addChange(std::function<void(Image&)> changeFunct, float value){
    changeMade = changeFunct;
    changeValue = value;

}

void VersionGraph::addNode(std::function<void(Image&)> changeFunct){
    //add the logic for separating branches
}

Image constructCurrImage(){
    //logic for adding all changes made to the "edited" version of the picture

}


vector<std::function<Image&>> vectorOfChanges(){
    //go through all the nodes (at least one path of nodes) and collect the changes made
}