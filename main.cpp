#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

std::vector<std::vector<float>> InpData(const int numInstances, const int numFeats){
    std::vector<std::vector<float>> retListOfLists;
    std::ifstream myfile("C:\\Users\\alexr\\CLionProjects\\FeatureSelection_NearestNeighbor\\CS170_small_special_testdata__95.txt");
    std::vector<float> numList;
    float num;

    for(int i= 0; i< numInstances; i++){
        numList.clear();
        myfile>> num;
        numList.push_back(num);             //first element of each vector indicates its class
        for(int j= 0; j< numFeats; j++){
            myfile>> num;
            numList.push_back(num);         //rest of vector represents values for each feature
        }
        retListOfLists.push_back(numList);      //store all the vectors inside a single vector (2-D vector)
    }
    return retListOfLists;
}

bool intIsInFeats(std::vector<int> feats_set, int feature){
    for(int i= 0; i< feats_set.size(); i++){
        if(feats_set.at(i)== feature){
            return true;
        }
    }
    return false;
}

float calc_dist(std::vector<std::vector<float>> data, int compare1, int compare2, std::vector<int> features_to_check){
    float total= 0.0;
    float curr= 0.0;
    for(int i= 0; i< features_to_check.size(); i++){
        curr= data.at(compare1).at(features_to_check.at(i)) - data.at(compare2).at(features_to_check.at(i));
        curr*= curr;
        total+= curr;
    }
    return sqrt(total);
}

float calc_acc(std::vector<std::vector<float>> data, std::vector<int> features_to_check){
    float curr_distance;
    float curr_neighbor_distance= 9999.0;
    int nearest= -1;
    float nearest_label= -1.0;
    int temp1, temp2;
    float num_correct= 0;
    float num_total= 0;
    for(int i= 0; i< data.size(); i++){
        temp1= i+ 1;
        for(int j= 0; j< data.size(); j++){
            temp2= j+ 1;
            if(temp2!= temp1) {
                curr_distance= calc_dist(data, i, j, features_to_check);
                if(curr_distance< curr_neighbor_distance){
                    curr_neighbor_distance= curr_distance;
                    nearest= temp2;
                    nearest_label= data.at(j).at(0);
                }
            }
        }
        curr_neighbor_distance= 9999.0;
        if(data.at(i).at(0)== nearest_label){
            num_correct++;
        }
        num_total++;
    }
    num_correct*= 100000.0;
    num_total*= 1000.0;
    return num_correct/ num_total;
}

int main() {
    int numInstances;    //usually 300
    int numFeats;        //usually 10
    std::cout<< "How many instances are there?";
    std::cin>> numInstances;
    std::cout<< "How many features are there?";
    std::cin>> numFeats;
    std::cout<< std::endl;
    std::vector<std::vector<float>> ListOfLists= InpData(numInstances, numFeats);
    float accuracy;


    int level;
    float best_acc_yet= -1.0;
    float curr_acc;
    int feat_to_add;
    std::vector<int> curr_feats;
    std::vector<int> test_feats;
    int feat_to_check;

    for(int i= 0; i< numFeats; i++){
        level= i+ 1;
        std::cout<< "On level "<< level<< " of the search tree\n";
        for(int j= 0; j< numFeats; j++){
            feat_to_check= j+ 1;
            if(!intIsInFeats(curr_feats, feat_to_check)) {
//                std::cout << "--Considering adding feature " << feat_to_check;
                std::cout << "--Considering adding feature " << feat_to_check;
                test_feats = curr_feats;
                test_feats.push_back(feat_to_check);
                curr_acc = calc_acc(ListOfLists, test_feats);
                std::cout<< " with accuracy of "<< curr_acc<< "%\n";

                if (curr_acc > best_acc_yet) {
                    best_acc_yet = curr_acc;
                    feat_to_add = feat_to_check;
                }
            }
        }
        curr_feats.push_back(feat_to_add);
        std::cout<< "For level "<< level<< ", feature "<< feat_to_add<< " was added to current set\n";
        best_acc_yet= -1.0;
    }
    return 0;
}
