#include "versionGraph.h"
#include <string>
#include <set>
#include "algorithms/exposure.h"
#include "algorithms/contrast.h"
#include "algorithms/highlights.h"
#include "algorithms/shadows.h"

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

//Version graph functions -----------------------------------------------------------------------

VersionGraph::VersionGraph(Image &img) :
    full_scale_image(img),

    aspect_ratio((float)img.width()/(float)img.height()),
    root_image(downscale_image_lanczos(img, 800, 800/aspect_ratio)),
    working_image(root_image),
    preview(downscale_image_lanczos(root_image, 800, 800/aspect_ratio)),
    preview_before_slider(preview){

    root_ = new Node();
    curr_ = root_;
    index_of_working_image.clear();
    image_version = 1;
    downscaling_function_ = downscale_image_lanczos;


    //add functionality to change these

    preview_width = 800;
    preview_height = preview_width/aspect_ratio;
    interpolation_radius = 16;

    //setting up default branch
    branches["master"] = curr_;
    currBranchName = "master";

};

Image& VersionGraph::getWorkingImage(){
    //returns the working image(i.e. the latest edited image)
    return working_image;
}


void VersionGraph::set_downscaling_function(std::function<Image(Image&, int,int)> func){
    //changing the downscaling function ( when the radio button is selected)
    //need to 'refresh' the downscaling_function_ variable and then also
    //need to refersh the preview image as well


    downscaling_function_ = func;
    Image temp = getWorkingImage();
    preview = downscaling_function_(temp, preview_width, preview_height);
    preview_before_slider = preview;
}

std::function<void(Image&,float)> VersionGraph::getAlgorithm(const std::string func){
    if(func == "exposure"){
        return exposure;
    }else if(func == "contrast"){
        return contrast;
    }else if(func == "highlights"){
        return highlights;
    }else if(func == "shadows"){
        return shadows;
    }
    return nullptr;
}


void VersionGraph::updatePreview(std::string adjustment_func, float value){
    //this is like a setter for preview
    //idea is that i will use this to perform the live updates
    //need to remember to make a refresh function which sets it to a downscaled version of the workign image
    preview=preview_before_slider;
    std::function<void(Image&, float)> func = getAlgorithm(adjustment_func);
    if(func){
        func(preview, value);
    }
}

void VersionGraph::addNode(std::string name, std::string changeFunct, float value){

    std::function<void(Image&, float)> func = getAlgorithm(changeFunct);
    Node* newNode = new Node(name, func, value, curr_);
    //have a few options about how to go about this
    //can just push_back and let the vector that has the indexes ( index_of_working_image) decide where we branch
    //or can decide where to branch here and then add to index of working image

    curr_ -> next_.push_back(newNode);
    index_of_working_image.push_back(curr_->next_.size() - 1);

    //set the curr_
    curr_ = newNode;
    branches[currBranchName] = curr_;
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

    preview = downscaling_function_(result, preview_width, preview_height);
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


//creating and managing branches
void VersionGraph::createBranch(std::string branchName){
    //creates a branch from the current node
    if(branches.find(branchName) == branches.end()){
        branches[branchName] = curr_;
        currBranchName = branchName;
    }
}


void VersionGraph::checkout(std::string branchName){
    //goes to the branch with the name provided
    if(branches.find(branchName) != branches.end()){
        curr_ = branches[branchName];
        currBranchName = branchName;
        working_image = constructCurrImage();
    }

}

std::string VersionGraph::vizualize(){
    //visualizing the branches

    //inverting the branches map so that i can check which branch a node belongs to
    std::map<Node*, std::vector<std::string>> labels;
    for(auto &[name, node] : branches){
        labels[node].push_back(name);
    }

    std::string result = "\n";
    std::string indentation = "";
    visualizeNode(root_, indentation, labels, result);
    return result;
}


void VersionGraph::visualizeNode(Node* node, std::string& indentation, std::map<Node*, std::vector<std::string>>& labels, std::string& result){
    //recursive function that generates the string for the vizualization
    result += indentation;
    //edge/end case
    if(node -> prev_ == nullptr || node ->prev_ ->next_.size() > 1){
        result += "+++";
    }
    else if(node -> next_.size() == 0){
        result += "---";
    }else{
        result += "|--";
    }


    if(node == root_){
        result += "(root)";
    }else{
        result += node -> name + " " + to_string(node -> changeValue);
    }

    std::map<Node*, std::vector<std::string>>::iterator it = labels.find(node);
    //printing the branche name
    if(it != labels.end()){
        result+= " [";
        for(int i=0; i< it->second.size();i++){
            if(i>0){
                result += ", ";
            }
            //marking the current branch and node
            if(it ->second[i] == currBranchName){
                result+="*";
            }
            result += it->second[i];
        }
        result+= " ]";
    }
    result +="\n";

    std::string currIndent = indentation;
    if(node -> next_.size() >1){
        currIndent += "|      ";
    }else{
        currIndent += "     ";
    }

    for(int i=0; i<node -> next_.size(); i++){
        visualizeNode(node->next_[i], currIndent, labels, result);
    }
}

std::vector<std::string> VersionGraph::getBranchNames(){
    //returns a vector of all branch names
    std::vector<std::string> result;

    for(auto [name,node]: branches){
        result.push_back(name);
    }

    return result;
}

float VersionGraph::getLatestValueFor(const std::string& adjustmentName){
    //this function returns the value of the latest time the adjustment was applied
    //takes a string name for said adjustment
    Node* temp = curr_;
    while(temp != nullptr){
        if(temp->name == adjustmentName){
            return temp->changeValue;
        }
        temp = temp->prev_;
    }
    return 0;
}
//destructors

Node::~Node(){
    for(int i=0; i<next_.size(); i++){
        delete next_[i];
    }
}


VersionGraph::~VersionGraph(){
    delete root_;
}