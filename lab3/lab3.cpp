// Author: Jeremy Roy <jeremy.roy@queensu.ca>
// Student number: 10092487

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <utility>   // std::pair

std::vector<std::vector<bool>> cvs_to_2d_vector(std::string filename)
{
    std::string line;
    std::ifstream in_file(filename);
    std::vector<std::vector<bool>> rows;
    std::vector<bool> col;

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
                col.push_back(std::stod(cell));
            }
            rows.push_back(col);
            col.clear();
        }
    }
    in_file.close();

    return rows;
}

bool is_unique(std::map<int,std::pair<int,uint16_t>> image_map, int num_features, uint16_t bitmap)
{
    if (image_map.count(num_features))
    {
        // At least one image in map has same number of features
        auto same_numf = image_map.equal_range(num_features);

        // Iterate through all images with same number of features
        for (auto it = same_numf.first; it != same_numf.second; ++it)
        {
            // Check if this image has identical features.
            auto elem = *it;
            if (elem.second.second == bitmap)
            {
                return false;
            }

            // If last element, image is unique
            if ( it++ == same_numf.second )
            {
                return true;
            }
            --it;          
        }
    }
}

int main(int argc, char** argv)
{
    std::string image_filename = "images.csv";
    std::vector<std::vector<bool>> images;
    int im_keep = std::stoi(argv[1]); // Number of images to keep (k)
    std::map<int,std::pair<int,uint16_t>> best_images; // Order: num_features, index in file, bit-encoded features
    std::map<int,std::pair<int,uint16_t>> other_images;

    // load image parameters
    images = cvs_to_2d_vector(image_filename);

    int num_features = 0;
    uint16_t bitmap = 0;

    // Calculate number of features
    for (int row = 0; row < 1000; ++row)
    {
        num_features = 0;
        bitmap = 0;
        for (int col = 0; col < 10; ++col)
        {
            num_features += images.at(row).at(col);
            bitmap = (bitmap << 1) | images.at(row).at(col);
        }

        // If best images map isn't full, add image if unique
        if (best_images.size() <= im_keep)
        {
            // Best image map full, determine whether or not to keep this image
            // Check if any image with identical feature set exists
            if ( is_unique(best_images, num_features, bitmap) )
            {
                std::pair<int,uint16_t> image_data(row, bitmap);
                best_images.insert(std::pair<int,std::pair<int,uint16_t>>(num_features, image_data));
            }
            else
            {
                // Put image in "not best image" map
                std::pair<int,uint16_t> image_data(row, bitmap);
                other_images.insert(std::pair<int,std::pair<int,uint16_t>>(num_features, image_data));
            }
        }
        else // Map is full.  Check if new image is unique and better than a previous unique image.  If it is, pop worst unique image.
        {
            // If image has less or same number of features as worst element in list, discard image
            auto elem = *best_images.begin();
            if ( num_features > elem.first )
            {
                // Check if image is unique.
                if ( is_unique(best_images, num_features, bitmap) )
                {
                    // Remove worst image from "best image" map.
                    best_images.erase(best_images.begin());

                    // Add image to "best image" map.
                    std::pair<int,uint16_t> image_data(row, bitmap);
                    best_images.insert(std::pair<int,std::pair<int,uint16_t>>(num_features, image_data));
                }
            }
        }
    }

    // If best_images map isn't full, add best elements from other_images map.
    if (best_images.size() < im_keep)
    {
        int missing_images = im_keep - best_images.size();
        for (int i = 0; i < missing_images; ++i)
        {
            best_images.insert(*other_images.rbegin());
        }
    }

    // Print images
    for (auto it = best_images.begin(); it != best_images.end(); ++it)
    {
        auto elem = *it;
        std::cout << elem.second.second << std::endl;
    }

    return 0;
}