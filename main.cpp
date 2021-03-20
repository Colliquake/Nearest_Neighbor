#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <iomanip>

std::string get_features(std::vector<int> feats){           //returns a list of features (in vector form) as a string
    std::string ret;
    ret.append("{");
    for(int i= 0; i< feats.size()- 1; i++){
        ret.append(std::to_string(feats.at(i)));
        ret.append(" ");
    }
    ret.append(std::to_string(feats.at(feats.size()- 1)));
    ret.append("}");
    return ret;
}

void print_features(std::vector<int> feats){                //prints the features in 'feats'
    std::cout<< "{";
    for(int i= 0; i< feats.size()- 1; i++){
        std::cout<< feats.at(i);
        if(i!= feats.size()- 1){
            std::cout<< ", ";
        }
    }
    std::cout<< feats.at(feats.size()- 1)<< "}";
}

float calc_default(std::vector<std::vector<float>> data){       //calculates the default rate (no features)
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

std::vector<int> remove_feature(std::vector<int> feats, int remove_feat){           //removes the 'remove_feat' feature from 'feats'
    std::vector<int> ret_vec;
    for(int i= 0; i< feats.size(); i++){
        if(feats.at(i)!= remove_feat){
            ret_vec.push_back(feats.at(i));
        }
    }
    return ret_vec;
}

void print_features(std::vector<int> feats, int feature){               //prints the list of features and an additional feature (for the "--Considering..." part)
    std::cout<< "{";
    for(int i= 0; i< feats.size(); i++){
        std::cout<< feats.at(i);
        if(i!= feats.size()){
            std::cout<< ", ";
        }
    }
    std::cout<< feature<< "}";
}

std::vector<std::vector<float>> InpData(const int numInstances, const int numFeats){            //represents the dataset as a 2D vector
    std::vector<std::vector<float>> retListOfLists;
    std::ifstream myfile("C:\\Users\\alexr\\CLionProjects\\FeatureSelection_NearestNeighbor\\CS170_largetestdata__35.txt");         //MUST change manually to file location!!!
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
        retListOfLists.push_back(numList);      //store all the vectors inside a single vector (2D vector)
    }
    return retListOfLists;
}

bool intIsInFeats(std::vector<int> feats_set, int feature){             //checks if 'feature' is already in the set 'feats_set'
    for(int i= 0; i< feats_set.size(); i++){            //iterates through set, stops as soon as a match is found
        if(feats_set.at(i)== feature){
            return true;
        }
    }
    return false;           //'feature' is not in 'feats_set'
}

float calc_dist(std::vector<std::vector<float>> data, int compare1, int compare2, std::vector<int> features_to_check){          //calculates euclidean for instances 'compare1' and 'compare2'
    float total= 0.0;
    float curr= 0.0;
    for(int i= 0; i< features_to_check.size(); i++){
        curr= data.at(compare1).at(features_to_check.at(i)) - data.at(compare2).at(features_to_check.at(i));
        curr*= curr;
        total+= curr;
    }
    return sqrt(total);
}

float calc_acc(std::vector<std::vector<float>> data, std::vector<int> features_to_check){               //calculates the accuracy of using selected features ('features_to_check')
    float curr_distance;
    float curr_neighbor_distance= 9999.0;           //current neighbor's distance set to inf
    int nearest= -1;
    float nearest_label= -1.0;              //class of nearest neighbor
    int temp1, temp2;
    float num_correct= 0;           //counts how many tests were correct
    float num_total= 0;             //tracks total tests run
    for(int i= 0; i< data.size(); i++){             //loops through all instances
        temp1= i+ 1;
        for(int j= 0; j< data.size(); j++){         //compares each instance to ALL other instances
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
        if(data.at(i).at(0)== nearest_label){           //current instance is same class as its nearest neighbor
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
    int numInstances;
    int numFeats;
    std::cout << "How many instances are there?";
    std::cin >> numInstances;
    std::cout << "How many features are there?";
    std::cin >> numFeats;
    std::cout << std::endl;
    std::vector<std::vector<float>> ListOfLists = InpData(numInstances, numFeats);          //rerpesent dataset with 2D vector
    float accuracy;


    int level;
    float best_acc_yet = -1.0;
    float curr_acc;
    int feat_to_add;
    std::vector<int> curr_feats;
    std::vector<int> test_feats;
    std::vector<int> best_feats;
    int feat_to_check;
    std::vector<std::vector<int>> best_of_best;         //vector to store the best vectors from each level (2D vector)
    std::vector<float> total_accs;                      //vector that stores accuracies for each instance in 'best_of_best'
    int bestAccLoc;                 //points to where highest accuracy is (for 'total_accs')
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
                feat_to_check = j + 1;                  //features start at column 1; j starts at 0; solution: just increase j by 1 and store it
                if (!intIsInFeats(curr_feats, feat_to_check)) {         //make sure the feature we want to check isn't already in our list of best features
                    std::cout << "--Considering using feature(s) ";
                    print_features(curr_feats, feat_to_check);          //print the features we are checking
                    test_feats = curr_feats;
                    test_feats.push_back(feat_to_check);
                    curr_acc = calc_acc(ListOfLists, test_feats);       //calculate the accuracy of the list of features we are checking
                    std::cout << " with an accuracy of " << curr_acc << "%\n";

                    if (curr_acc > best_acc_yet) {              //if we find a list of features that gives us better accuracy
                        best_acc_yet = curr_acc;                //than our current best accuracy features, update those values
                        feat_to_add = feat_to_check;
                        best_feats = test_feats;
                    }
                }
            }
            tempAcc = calc_acc(ListOfLists, best_feats);
            std::cout << "For level " << level << ", using feature(s) ";            //note which features was best for certain levels
            print_features(curr_feats, feat_to_add);
            std::cout << " was best with an accuracy of " << tempAcc << "%\n\n";
            curr_feats.push_back(feat_to_add);                  //add the best features (for current level) into our final list of features
            best_of_best.push_back(curr_feats);
            total_accs.push_back(tempAcc);
            best_acc_yet = -1.0;
        }
        bestAccLoc = 0;
        tempAcc = total_accs.at(0);
        for (int i = 0; i < total_accs.size(); i++) {           //calculate which combination of features gave us highest accuracy
            if (total_accs.at(i) > tempAcc) {
                bestAccLoc = i;
                tempAcc = total_accs.at(i);
            }
        }
        end = clock();
        double tot_time = double(end - start) / double(CLOCKS_PER_SEC);         //measure time it took to run program
        tot_time = tot_time / 60.0;       //convert to minutes

        std::cout << "\nDone! The best feature subset is ";         //print out which was the best set of features
        print_features(best_of_best.at(bestAccLoc));
        std::cout << " with an accuracy of " << total_accs.at(bestAccLoc) << "%\n";
        std::cout << "Total execution time is " << std::fixed << std::setprecision(6) << tot_time << " minutes."
                  << std::endl;



//        std::ofstream stats;                      //output the features (at each level) and their accuracies into a csv file
//        stats.open("C:\\Users\\alexr\\CLionProjects\\FeatureSelection_NearestNeighbor\\stats.csv");
//        for(int f= 0; f< best_of_best.size(); f++){
//            stats<< get_features(best_of_best.at(f))<< ", "<< std::to_string(total_accs.at(f))<< "\n";
//        }
//        stats.close();
    }
    else {           //runs backward elimination
        clock_t start, end;
        start = clock();
        for(int i= 0; i< numFeats; i++){
            curr_feats.push_back(i+ 1);             //start with current features set to all features
        }
        float def= calc_default(ListOfLists);       //default rate is still when we have no features
        std::cout << "Default rate: " << def << "%\n";
        for (int i = numFeats- 1; i>= 1; i--) {     //start at bottom of graph, work towards top
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


//        std::ofstream stats;                      //output the features (at each level) and their accuracies into a csv file
//        stats.open("C:\\Users\\alexr\\CLionProjects\\FeatureSelection_NearestNeighbor\\stats.csv");
//        for(int f= 0; f< best_of_best.size(); f++){
//            stats<< get_features(best_of_best.at(f))<< ", "<< std::to_string(total_accs.at(f))<< "\n";
//        }
//        stats.close();
    }
    return 0;
}
