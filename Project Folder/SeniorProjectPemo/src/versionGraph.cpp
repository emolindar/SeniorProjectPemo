#include "versionGraph.h"



Node::Node(){
    changeMade = nullptr;
    changeValue = 0;
    prev_ = nullptr;
}


Node::Node(std::function<void(Image&, float)> changeFunct, float value){
    changeMade = changeFunct;
    changeValue = value;
}

Node::Node(std::function<void(Image&, float)> changeFunct, float value, Node *prev){
    changeMade = changeFunct;
    changeValue = value;
    prev_ = prev;
}

Node* Node::next(int index_of_next){
    return next_[index_of_next];
}



void Node::addChange(std::function<void(Image&, float)> changeFunct, float value){
    changeMade = changeFunct;
    changeValue = value;

}

void VersionGraph::addNode(std::function<void(Image&, float)> changeFunct, float value){

    Node* newNode = new Node(changeFunct, value, curr_);
    //have a few options about how to go about this
    //can just push_back and let the vector that has the indexes ( index_of_working_image) decide where we branch
    //or can decide where to branch here and then add to index of working image

    //leaving like this for now, but this will always end up at 0 for now;
    curr_ -> next_.push_back(newNode);
    index_of_working_image.push_back(curr_->next_.size() - 1);

    //finally, now that i have added the change to the graph, let me actually apply the change
    changeFunct(working_image, value);

    //set the curr_
    curr_ = newNode;
}

Image VersionGraph::constructCurrImage(){
    //logic for adding all changes made to the "edited" version of the picture

}


vector<std::function<void(Image&, float)>> VersionGraph::vectorOfChanges(){
    //go through all the nodes (at least one path of nodes) and collect the changes made
}