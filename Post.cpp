// Include necessary libraries and header files
#include <iostream>
#include <string>
#include <stdexcept>
#include "Post.h"
#include <cctype>
#include <sstream>

// Using specific elements from std namespace
using std::string;
using std::vector;
using std::stringstream;

Post::Post(unsigned int postId, string userName, string postText) : postId(postId), userName(userName), postText(postText) {
    // Validate input parameters
    if (postId == 0 || userName.empty() || postText.empty()) {
        throw std::invalid_argument("Post constructor: Invalid parameter values");
    }
}

unsigned int Post::getPostId() {
    // Getter method for postId
    return postId;
}

string Post::getPostUser() {
    // Getter method for userName
    return userName;
}

string Post::getPostText() {
    // Getter method for postText
    return postText;
}

vector<string> Post::findTags() {
    // Method to find tags in the post text

    // Vector to store the found tags
    vector<string> tags;

    // Use istringstream to tokenize the postText
    std::istringstream iss(postText);
    string word;

    // Iterate through each word in the postText
    while (iss >> word) {
        // Check if the word starts with '#'
        if (!word.empty() && word[0] == '#') {
            // Remove trailing punctuation
            size_t lastCharIndex = word.size() - 1;
            while (lastCharIndex > 0 && ispunct(word[lastCharIndex])) {
                --lastCharIndex;
            }

            // Resize the word to remove trailing punctuation
            word.resize(lastCharIndex + 1);

            // Convert to lowercase
            for (char& c : word) {
                c = std::tolower(c);
            }

            // Add the tag to the vector
            tags.push_back(word);
        }
    }

    // Return the vector of found tags
    return tags;
}