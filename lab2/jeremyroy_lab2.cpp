// Jeremy Roy
// Student number: 10092487

#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <stdint.h>
#include <chrono>

void collatz_program_set_memory(int number)
{
    static std::set<int> overall_numbers; // Stores all numbers that lead to completion
    std::set<int> numbers_this_round; // Stores all new numbers that are not yet known to lead to completion
    while (number != 1)
    {
        // if the previous term is even, the next term is one half the previous term
        if ( (number % 2) == 0)
        {
            number /= 2;
        }
        else // Otherwise, the next term is 3 times the previous term plus 1
        {
            number = number * 3 + 1;
        }

        // Check if this number has already been discovered as having a solution.
        if ( overall_numbers.find(number) != overall_numbers.end())
        {
            break;
        }

        // Mark number as not-yet discovered
        numbers_this_round.insert(number);
    }

    overall_numbers.insert(numbers_this_round.begin(), numbers_this_round.end());
    // std::cout << *overall_numbers.rbegin() << std::endl;
}

void collatz_program_array_memory(int number)
{
    bool *memory = (bool*) calloc(number * 26, sizeof(bool));

    while (number != 1)
    {
        // if the previous term is even, the next term is one half the previous term
        if ( (number % 2) == 0)
        {
            number /= 2;
        }
        else // Otherwise, the next term is 3 times the previous term plus 1
        {
            number = number * 3 + 1;
        }

        // Check if this number has already been discovered as having a solution.
        if ( memory[number] == 1)
        {
            break;
        }

        // Mark number as discovered
        memory[number] = 1;
        // std::cout << number << std::endl;
    }
}

void collatz_program_no_memory(int number)
{
    while (number != 1)
    {
        // if the previous term is even, the next term is one half the previous term
        if ( (number % 2) == 0)
        {
            number /= 2;
        }
        else // Otherwise, the next term is 3 times the previous term plus 1
        {
            number = number * 3 + 1;
        }
        std::cout << number << std::endl;
    }
}

int main(int argc, char* argv[])
{    
    // Loop until user types "exit" to std::in
    while (true)
    {
        std::string user_input;

        std::cout << "Enter the number you would like to check: ";
        std::cin >> user_input;
        
        // Check if user wants to exit program
        if (user_input == "exit" || user_input == "quit")
        {
            break;
        }
        
        // Verify if user input is a number
        std::istringstream user_istream(user_input);
        int64_t numeral_input;

        if ((user_istream >> numeral_input).fail() || numeral_input < 0) 
        {
            std::cerr << "Invalid input!.  Please enter a positive integer number, \'exit\' or \'quit\'" << std::endl;
            continue;
        }

        std::cout << "The number you entered: " << numeral_input << std::endl;

        // The user input a valid number. Run the algorithm on the number.
        auto t_start = std::chrono::high_resolution_clock::now();
        for (int i = numeral_input; i > 0; --i)
        {
            collatz_program_array_memory(i);
        }
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << "Time elapsed: "
                  << std::chrono::duration<double, std::milli>(t_end-t_start).count()
                  << " ms"
                  << std::endl;
    }
    
    return 0;
}