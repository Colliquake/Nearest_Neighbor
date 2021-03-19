#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <iomanip>

void print_features(std::vector<int> feats){
    std::cout<< "{";
    for(int i= 0; i< feats.size()- 1; i++){
        std::cout<< feats.at(i);
        if(i!= feats.size()- 1){
            std::cout<< ", ";
        }
    }
    std::cout<< feats.at(feats.size()- 1)<< "}";
}

float calc_default(std::vector<std::vector<float>> data){
    float class1= 0;
    float class2= 0;
    for(int i= 0; i< data.size(); i++){
        if(data.at(i).at(0)== 1.00){
            class1+= 1.00;
        }
        else{
            class2+= 1.00;
        }
    }
    if(class1> class2){
        return 100* class1/ (class1+ class2);
    }
    return 100* class2/ (class1+ class2);
}

std::vector<int> remove_feature(std::vector<int> feats, int remove_feat){
    std::vector<int> ret_vec;
    for(int i= 0; i< feats.size(); i++){
        if(feats.at(i)!= remove_feat){
            ret_vec.push_back(feats.at(i));
        }
    }
    return ret_vec;
}

void print_features(std::vector<int> feats, int feature){
    std::cout<< "{";
    for(int i= 0; i< feats.size(); i++){
        std::cout<< feats.at(i);
        if(i!= feats.size()){
            std::cout<< ", ";
        }
    }
    std::cout<< feature<< "}";
}

std::vector<std::vector<float>> InpData(const int numInstances, const int numFeats){
    std::vector<std::vector<float>> retListOfLists;
    std::ifstream myfile("C:\\Users\\alexr\\CLionProjects\\FeatureSelection_NearestNeighbor\\CS170_SMALLtestdata__10.txt");
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
    std::cout << "Note: Please confirm that you have included the correct file path (in the code)\n";
    int numInstances;    //usually 300
    int numFeats;        //usually 10
    std::cout << "How many instances are there?";
    std::cin >> numInstances;
    std::cout << "How many features are there?";
    std::cin >> numFeats;
    std::cout << std::endl;
    std::vector<std::vector<float>> ListOfLists = InpData(numInstances, numFeats);
    float accuracy;


    int level;
    float best_acc_yet = -1.0;
    float curr_acc;
    int feat_to_add;
    std::vector<int> curr_feats;
    std::vector<int> test_feats;
    std::vector<int> best_feats;
    int feat_to_check;
    std::vector<std::vector<int>> best_of_best;
    std::vector<float> total_accs;
    int bestAccLoc;
    float tempAcc;

    int which_algorithm;
    std::cout << "Select algorithm to run:\n";
    std::cout << "\t 1. Forward Selection\n";
    std::cout << "\t 2. Backward Elimination\n";
    std::cin >> which_algorithm;

    if (which_algorithm == 1) {           //runs forward selection
        clock_t start, end;
        start = clock();
        std::cout << "Default rate: " << calc_default(ListOfLists) << "%\n";
        for (int i = 0; i < numFeats; i++) {
            level = i + 1;
            std::cout << "On level " << level << " of the search tree\n";
            for (int j = 0; j < numFeats; j++) {
                feat_to_check = j + 1;
                if (!intIsInFeats(curr_feats, feat_to_check)) {
//                std::cout << "--Considering adding feature " << feat_to_check;
                    std::cout << "--Considering using feature(s) ";
                    print_features(curr_feats, feat_to_check);
                    test_feats = curr_feats;
                    test_feats.push_back(feat_to_check);
                    curr_acc = calc_acc(ListOfLists, test_feats);
                    std::cout << " with an accuracy of " << curr_acc << "%\n";

                    if (curr_acc > best_acc_yet) {
                        best_acc_yet = curr_acc;
                        feat_to_add = feat_to_check;
                        best_feats = test_feats;
                    }
                }
            }
            tempAcc = calc_acc(ListOfLists, best_feats);
            std::cout << "For level " << level << ", using feature(s) ";
            print_features(curr_feats, feat_to_add);
            std::cout << " was best with an accuracy of " << tempAcc << "%\n\n";
            curr_feats.push_back(feat_to_add);
            best_of_best.push_back(curr_feats);
            total_accs.push_back(tempAcc);
            best_acc_yet = -1.0;
        }
        bestAccLoc = 0;
        tempAcc = total_accs.at(0);
        for (int i = 0; i < total_accs.size(); i++) {
            if (total_accs.at(i) > tempAcc) {
                bestAccLoc = i;
                tempAcc = total_accs.at(i);
            }
        }
        end = clock();
        double tot_time = double(end - start) / double(CLOCKS_PER_SEC);
        tot_time = tot_time / 60.0;       //convert to minutes

        std::cout << "\nDone! The best feature subset is ";
        print_features(best_of_best.at(bestAccLoc));
        std::cout << " with an accuracy of " << total_accs.at(bestAccLoc) << "%\n";
        std::cout << "Total execution time is " << std::fixed << std::setprecision(6) << tot_time << " minutes."
                  << std::endl;
    }
    else {           //runs backward elimination
        clock_t start, end;
        start = clock();
        for(int i= 0; i< numFeats; i++){
            curr_feats.push_back(i+ 1);
        }
        float def= calc_default(ListOfLists);
        std::cout << "Default rate: " << def << "%\n";
        for (int i = numFeats- 1; i>= 1; i--) {
            level = i;
            std::cout << "On level " << level << " of the search tree\n";
            for (int j= numFeats- 1; j>= 0; j--) {
                feat_to_check = j + 1;
                if (intIsInFeats(curr_feats, feat_to_check)) {
                    std::cout << "--Considering using feature(s) ";
                    test_feats= remove_feature(curr_feats, feat_to_check);
                    print_features(test_feats);
                    curr_acc = calc_acc(ListOfLists, test_feats);
                    std::cout << " with an accuracy of " << curr_acc << "%\n";

                    if (curr_acc > best_acc_yet) {
                        best_acc_yet = curr_acc;
                        feat_to_add = feat_to_check;
                        best_feats = test_feats;
                    }
                }
            }
            tempAcc = calc_acc(ListOfLists, best_feats);
            std::cout << "For level " << level << ", using feature(s) ";
            curr_feats= remove_feature(curr_feats, feat_to_add);
            print_features(curr_feats);
            std::cout << " was best with an accuracy of " << tempAcc << "%\n\n";
            best_of_best.push_back(curr_feats);
            total_accs.push_back(tempAcc);
            best_acc_yet = -1.0;
        }
        std::cout<< "On level 0 of the search tree\n";
        std::cout<< "For level 0, using feature(s) {} was best with an accuracy of "<< def<< "%\n\n";

        bestAccLoc = 0;
        tempAcc = total_accs.at(0);
        for (int i = 0; i < total_accs.size(); i++) {
            if (total_accs.at(i) >= tempAcc) {
                bestAccLoc = i;
                tempAcc = total_accs.at(i);
            }
        }
        end = clock();
        double tot_time = double(end - start) / double(CLOCKS_PER_SEC);
        tot_time = tot_time / 60.0;       //convert to minutes

        std::cout << "\nDone! The best feature subset is ";
        print_features(best_of_best.at(bestAccLoc));
        std::cout << " with an accuracy of " << total_accs.at(bestAccLoc) << "%\n";
        std::cout << "Total execution time is " << std::fixed << std::setprecision(6) << tot_time << " minutes."
                  << std::endl;
    }
    return 0;
}
