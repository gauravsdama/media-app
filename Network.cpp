// Include necessary libraries and header files
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Network.h"
#include "User.h"
#include <cctype>

// Using specific elements from std namespace
using std::string;
using std::vector;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::runtime_error;

// Network class implementation
Network::Network() {
  // Constructor: Empty containers of vectors are already created
}

void Network::loadFromFile(string fileName) {
  // Method to load user and post information from a file
// Open the file
    ifstream inputFileStream;
    inputFileStream.open(fileName);
    
    // Check if the file is successfully opened
    if (!inputFileStream.is_open()) {
        throw std::invalid_argument("Failed to open file: " + fileName);
    }

    string line;
    while (getline(inputFileStream, line)) {
        // Separate the line into an identifier and the rest of the line
        string recordType;
        stringstream lineStream(line);

        // Read the identifier from the line
        if (!(lineStream >> recordType)) {
            throw std::runtime_error("Error reading identifier from file");
        }

        // Process based on the identifier
        if (recordType == "User") {
            // For User records
            string username;
            if (!(lineStream >> username) || username.empty()) {
                throw std::runtime_error("Error reading user information from file");
            }

            try {
                // Attempt to add the user to the network
                addUser(username);
            } catch (const std::invalid_argument& e) {
                // Catch any invalid argument exceptions during user addition
                throw std::runtime_error("Error adding user to the network: ");
            }
        } else if (recordType == "Post") {
            // For Post records
            int postId = 0;
            string username;
            string postText;

            // Read post ID, username, and post text from the line
            if (!(lineStream >> postId)) {
                throw std::runtime_error("Error reading post ID from file");
            }

            if (!(lineStream >> username) || username.empty() ||
                username.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos) {
                throw std::runtime_error("Error reading post author from file");
            }

            lineStream >> std::ws;
            if (!(getline(lineStream, postText))) {
                throw std::runtime_error("Error reading post text from file");
            }

            if (postText.empty()) {
                throw std::runtime_error("Error reading empty post text from file");
            }

            try {
                // Attempt to add the post to the network
                addPost(postId, username, postText);
            } catch (const std::invalid_argument& e) {
                // Catch any invalid argument exceptions during post addition
                throw std::runtime_error("Error adding post to the network: ");
            }
        } else {
            // Unrecognized identifier in the file
            throw std::runtime_error("Error reading unrecognized identifier from file");
        }
    }

    // Close the file after processing
    inputFileStream.close();
}

void Network::addUser(string userName) {
    // Create a new user and add it to the network

    // Convert the username to lowercase without modifying the original input
    string lowercaseUserName;
    for (char character : userName) {
        lowercaseUserName += tolower(character);
    }

    // Check if the username already exists
    for (const User* existingUser : users) {
        if (const_cast<User*>(existingUser)->getUserName() == lowercaseUserName) {
            throw std::invalid_argument("User with the same name already exists");
        }
    }

    // Create a new user with the lowercase username
    User* newUser = new User(lowercaseUserName);
    users.push_back(newUser);

    // Print a message indicating the user has been added
    std::cout << "Added User " << lowercaseUserName << std::endl;

}

void Network::addPost(unsigned int postId, string userName, string postText) {
  // Create a post and add it to the network

    // Check if the user exists
    bool userExists = false;
    User* authorUser = nullptr;
    for (User* user : users) {
        if (user->getUserName() == userName) {
            authorUser = user;
            userExists = true;
            break;
        }
    }

    if (!userExists) {
        throw std::invalid_argument("User not found in the network");
    }

    // Check if a post with the same ID already exists
    for (Post* existingPost : posts) {
        if (existingPost->getPostId() == postId) {
            throw std::invalid_argument("Post with the same ID already exists");
        }
    }

    // Create a new post and add it to the posts vector
    Post* newPost = new Post(postId, userName, postText);
    posts.push_back(newPost);

    // Add the post to the user's posts
    authorUser->addUserPost(newPost);

    // Find and add tags associated with the new post
    vector<string> postTags = newPost->findTags();
    vector<Tag*> tagPointers;

    for (string tag : postTags) {
        if (tag.empty()) {
            continue;
        }

        // Check if the tag already exists
        bool tagExists = false;
        for (Tag* existingTag : tags) {
            if (existingTag->getTagName() == tag) {
                tagExists = true;
                tagPointers.push_back(existingTag);
                break;
            }
        }

        // If the tag does not exist, create a new tag and add it to the tags vector
        if (!tagExists) {
            try {
                Tag* newTag = new Tag(tag);
                tagPointers.push_back(newTag);
                tags.push_back(newTag);
            } catch (const std::invalid_argument&) {
                continue;
            }
        }
    }

    // Add the post to the associated tags
    for (Tag* tagPointer : tagPointers) {
        tagPointer->addTagPost(newPost);
    }

    std::cout << "Added Post " << postId << " by " << userName << std::endl;
}

vector<Post*> Network::getPostsWithTag(string tagName) {
  // TODO(student): return posts containing the given tag
  // Return posts containing the given tag

    // Check if the tag name is empty
    if (tagName.empty()) {
        throw std::invalid_argument("Tag name cannot be empty");
    }

    // Flag to check if the tag is found
    bool found = false;

    // Vector to store posts with the given tag
    vector<Post*> postsWithTag;

    // Iterate through each tag in the network
    for (unsigned int i = 0; i < tags.size(); i++) {
        Tag* currentTag = tags[i];

        // Check if the current tag matches the given tag name
        if (currentTag->getTagName() == tagName) {
            // Retrieve posts associated with the tag
            const vector<Post*>& tagPosts = currentTag->getTagPosts();

            // Use const_cast to remove const qualifier temporarily and assign to the result vector
            postsWithTag.assign(tagPosts.begin(), tagPosts.end());

            // Set the flag to indicate that the tag is found
            found = true;
            break;
        }
    }

    // Check if the tag was not found or has no associated posts
    if (!found || postsWithTag.empty()) {
        throw std::invalid_argument("Tag not found or no posts found with the given tag");
    }

    // Return the vector of posts containing the given tag
    return postsWithTag;
}

vector<Post*> Network::getPostsByUser(string userName) {
  // TODO(student): return posts created by the given user
    // Return posts created by the given user

    // Check if the target user name is empty
    if (userName.empty()) {
        throw std::invalid_argument("User name cannot be empty");
    }

    // Search for the user in the network
    bool userFound = false;
    vector<Post*> userPosts;
    for (unsigned int i = 0; i < users.size(); ++i) {
        User* currentUser = users[i];

        // Check if the current user matches the target user name
        if (currentUser->getUserName() == userName) {
            // Retrieve posts created by the user
            userPosts = currentUser->getUserPosts();
            userFound = true;
            break;
        }
    }

    // Check if the user was not found or has no posts
    if (!userFound || userPosts.empty()) {
        throw std::invalid_argument("User not found or no posts found for the given user");
    }

    // Return the posts created by the user
    return userPosts;
}

vector<string> Network::getMostPopularHashtag() {
  // TODO(student): return the tag occurring in most posts
  // Return the tag occurring in most posts

    // Vector to store the most popular tag(s)
    vector<string> mostPopularTags;

    // Variable to track the maximum count of posts for a tag
    int maxPostCount = 0;

    // Iterate through each tag in the network
    for (const auto& tag : tags) {
        // Get the count of posts associated with the current tag
        int postCount = static_cast<int>(tag->getTagPosts().size());

        // Compare the post count with the current maximum count
        if (postCount > maxPostCount) {
            // If greater, update the most popular tag vector with the current tag
            mostPopularTags = {tag->getTagName()};
            // Update the maximum count
            maxPostCount = postCount;
        } else if (postCount == maxPostCount) {
            // If equal, add the current tag to the most popular tag vector
            mostPopularTags.push_back(tag->getTagName());
        }
    }

    // Return the vector of most popular tag(s)
    return mostPopularTags;

}

Network::~Network() {
  for (unsigned int i = 0; i < users.size(); ++i) {
    delete users.at(i);
  }
  for (unsigned int i = 0; i < tags.size(); ++i) {
    delete tags.at(i);
  }
  for (unsigned int i = 0; i < posts.size(); ++i) {
    delete posts.at(i);
  }
}

