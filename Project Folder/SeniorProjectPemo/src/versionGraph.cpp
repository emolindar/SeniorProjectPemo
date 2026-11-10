#include "versionGraph.h"
#include <string>
#include <set>

Node::Node(){
    changeMade = nullptr;
    changeValue = 0;
    prev_ = nullptr;
}


Node::Node(std::string name, std::function<void(Image&, float)> changeFunct, float value){
    changeMade = changeFunct;
    changeValue = value;
    this -> name = name;
}

Node::Node(std::string name, std::function<void(Image&, float)> changeFunct, float value, Node *prev){
    changeMade = changeFunct;
    changeValue = value;
    prev_ = prev;
    this -> name = name;
}

Node* Node::next(int index_of_next){
    return next_[index_of_next];
}



void Node::addChange(std::function<void(Image&, float)> changeFunct, float value){
    changeMade = changeFunct;
    changeValue = value;

}

//Version graph functions

Image& VersionGraph::getWorkingImage(){
    //returns the working image(i.e. the latest edited image)
    return working_image;
}


void VersionGraph::set_downscaling_function(std::function<Image(Image&, int,int, int)> func){
    //changing the downscaling function ( when the radio button is selected)
    //need to 'refresh' the downscaling_function_ variable and then also
    //need to refersh the preview image as well


    downscaling_function_ = func;
    Image temp = getWorkingImage();
    preview = downscaling_function_(temp, preview_width, preview_height, 16);
    preview_before_slider = preview;
}

void VersionGraph::updatePreview(std::function<void(Image&, float)> adjustment_func, float value){
    //this is like a setter for preview
    //idea is that i will use this to perform the live updates
    //need to remember to make a refresh function which sets it to a downscaled version of the workign image
    preview=preview_before_slider;
    adjustment_func(preview, value);
}

void VersionGraph::addNode(std::string name, std::function<void(Image&, float)> changeFunct, float value){

    Node* newNode = new Node(name, changeFunct, value, curr_);
    //have a few options about how to go about this
    //can just push_back and let the vector that has the indexes ( index_of_working_image) decide where we branch
    //or can decide where to branch here and then add to index of working image

    //leaving like this for now, but this will always end up at 0 for now;
    curr_ -> next_.push_back(newNode);
    index_of_working_image.push_back(curr_->next_.size() - 1);

    //set the curr_
    curr_ = newNode;

    //finally, now that i have added the change to the graph, let me actually apply the change
    working_image = constructCurrImage();

}

Image VersionGraph::constructCurrImage(){
    //constructing the edited image
    //idea is that it goes from (either the root or the latest cached image) to the current node
    // it collects the functions and applies it fresh, not on top of each other, which
    //would be the case if they were applied in any other way

    std::vector<Node*> path;

    Node* n= curr_;
    while(n != nullptr && n!= root_){
        path.push_back(n);
        n = n->prev_;
    }

    //adding mechanism to make sure that the same slider doesn't get stacked on top of each other
    std::set<std::string> slider_applied; // collection of sliders that have been changed
    std::vector<std::function<void(Image&, float)>> unique_functs; // collection of functions to apply
    std::vector<float> unique_functs_values;
    for(int i=0; i< path.size(); i++){
        if(slider_applied.count(path[i]->name) == 0){
            slider_applied.insert(path[i] -> name);
            unique_functs.push_back(path[i] -> changeMade);
            unique_functs_values.push_back(path[i] -> changeValue);
        }
    }

    Image result(root_image);
    for(int i = unique_functs.size() -1; i >= 0 ; i--){
        unique_functs[i](result, unique_functs_values[i]);
    }

    preview = downscaling_function_(result, preview_width, preview_height, interpolation_radius);
    preview_before_slider = preview;
    return result;
}

std::string VersionGraph::history(){
    /*returns a string that represent the
     * history of edits for the current working image
    */


    std::string result = "\nEdit History\n\n";
    std::vector<Node*> result_path;

    Node* n = curr_;
    int counter = 1;
    while(n!= nullptr && n!= root_){
        result_path.push_back(n);
        result= result + to_string(counter) + " " +  n->name + " " + to_string(n->changeValue) + "\n";
        n = n-> prev_;
        counter++;
    }

    return result;
}

Image VersionGraph::constructExportImage(){
    //returns the full scale resolution image with the final adjustments applied
    //function follows all the same logic as constructCurrImage();
    std::vector<Node*> path;

    Node* n= curr_;
    while(n != nullptr && n!= root_){
        path.push_back(n);
        n = n->prev_;
    }

    //adding mechanism to make sure that the same slider doesn't get stacked on top of each other
    std::set<std::string> slider_applied; // collection of sliders that have been changed
    std::vector<std::function<void(Image&, float)>> unique_functs; // collection of functions to apply
    std::vector<float> unique_functs_values;
    for(int i=0; i< path.size(); i++){
        if(slider_applied.count(path[i]->name) == 0){
            slider_applied.insert(path[i] -> name);
            unique_functs.push_back(path[i] -> changeMade);
            unique_functs_values.push_back(path[i] -> changeValue);
        }
    }

    Image result(full_scale_image);


    for(int i=unique_functs.size() - 1; i>=0; i--){
        unique_functs[i](result, unique_functs_values[i]);
    }


    return result;
}

void VersionGraph::setPreviewDimensions(int w){
    preview_width = w;
    preview_height = w/aspect_ratio;
}