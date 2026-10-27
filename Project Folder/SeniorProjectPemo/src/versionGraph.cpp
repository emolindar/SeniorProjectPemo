#include "versionGraph.h"


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
    //add the logic for separating branches
    Node* curr = root_;

    //going to the current working node
    for(int i=0; i<index_of_working_image.size(); i++){
        curr = curr -> next(index_of_working_image[i]);
    }


    //have a few options about how to go about this
    //can just push_back and let the vector that has the indexes ( index_of_working_image) decide where we branch
    //or can decide where to branch here and then add to index of working image

    //leaving like this for now, but this will always end up at 0 for now;
    curr -> next_.push_back(new Node(changeFunct, value));
    index_of_working_image.push_back(curr->next_.size() - 1);

    //finally, now that i have added the change to the graph, let me actually apply the change
    changeFunct(working_image, value);
}

Image constructCurrImage(){
    //logic for adding all changes made to the "edited" version of the picture

}


vector<std::function<Image&>> vectorOfChanges(){
    //go through all the nodes (at least one path of nodes) and collect the changes made
}