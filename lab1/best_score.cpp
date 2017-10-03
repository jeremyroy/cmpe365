#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm> // random_shuffle

std::vector<double> cvs_to_vector(std::string filename)
{
    std::string line;
    std::ifstream in_file(filename);
    std::vector<double> entries;

    if (in_file.is_open())
    {        
        // loop through all lines in file
        while (getline (in_file, line))
        {
            // seperate commas, push csv entries to vector
            std::stringstream lineStream(line);
            std::string cell;
            while (std::getline(lineStream,cell, ','))
            {
                entries.push_back(std::stod(cell));
            }
        }
    }
    in_file.close();

    return entries;
}

// Prints best score and index of best score from input vector
double print_best_score(std::vector<double> scores)
{
    // Find best score
    double best_score = 0;
    int best_score_index = 0;
    for (int i = 0; i < scores.size(); ++i)
    {
        if (scores.at(i) > best_score)
        {
            best_score = scores.at(i);
            best_score_index = i;
        }
    }
    
    std::cout << "Best score: " << best_score << std::endl;
    std::cout << "Best score index: " << best_score_index << std::endl;
    return best_score;
}

// Runs best employee algorithm, prints results to std::out
double print_best_employee(std::vector<double> scores)
{
    double best_score = 0;
    int best_employee_index = 0;

    // interview first n/e applicants, find best applicant
    for (int i = 0; i < (scores.size() / M_E); ++i)
    {
        if (scores.at(i) > best_score)
        {
            best_score = scores.at(i);
            best_employee_index = i;
        }
    }

    // Find first applicant who has better score than n/e first applicants.
    // If no applicants are better, take last applicant
    for (int i = (scores.size() / M_E); i < scores.size(); ++i)
    {
        if (scores.at(i) > best_score)
        {
            best_score = scores.at(i);
            best_employee_index = i;
            break;
        }
    }

    // Possible improvement: if no applicants are better, use best applicant from n/e first applicants 
    
    // std::cout << "Best score: " << best_score << std::endl;
    // std::cout << "Best employee index: " << best_employee_index << std::endl;
    return best_score;
}

int main(int argc, char** argv)
{
    // Read file
    std::string filename = argv[1];
    std::vector<double> scores = cvs_to_vector(filename);

    // Get best score
    double best_score = print_best_score(scores);

    // shuffle the input dataset and find the best employee 100 times
    // Check if employee is overall "Best employee"
    int num_success = 0;
    int num_itterations = std::stoi(argv[2]);
    for (int i = 0; i < num_itterations; ++i)
    {
        std::random_shuffle(scores.begin(), scores.end());
        double best_employee = print_best_employee(scores);

        if (best_employee == best_score)
        {
            ++num_success;
        }
    }

    std::cout << "Accuracy: " << ( (1.0 * num_success) / num_itterations ) * 100 << std::endl;


    return 0;
}