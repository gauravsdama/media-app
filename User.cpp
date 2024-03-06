#include <string>
#include <stdexcept>
#include "User.h"

using std::string;
using std::vector;

User::User(string userName) : userName(userName), userPosts() {
    // Constructor: Check if the provided userName is valid

    // Check for empty username
    if (userName.empty()) {
        throw std::invalid_argument("Empty username");
    }

    // Check if the first character is not lowercase
    if (!islower(userName[0])) {
        throw std::invalid_argument("Username must start with a lowercase letter");
    }

    // Check for uppercase letters in the rest of the username
    for (size_t i = 1; i < userName.size(); i++) {
        if (isupper(userName[i])) {
            throw std::invalid_argument("Uppercase letters are not allowed in the username");
        }
    }

    // Initialize the userName after passing all checks
    this->userName = userName;
}

string User::getUserName() {
    // Getter method for userName
    return userName;
}

vector<Post*>& User::getUserPosts() {
    // Getter method for userPosts
    return userPosts;
}

void User::addUserPost(Post* post) {
    // Method to add a post to userPosts

    if (post == nullptr) {
       throw std::invalid_argument("Cannot add a null post to userPosts");
    }

    // Add the post to userPosts
    userPosts.push_back(post);
}
