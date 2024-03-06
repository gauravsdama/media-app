// Include necessary libraries and header files
#include <iostream>
#include <string>
#include <stdexcept>
#include "Tag.h"
#include <cctype>

// Using specific elements from std namespace
using std::string;
using std::vector;

// Implementation of the Tag class
Tag::Tag(string tagName) {
  // Constructor: Initializes tag name and performs validity checks
  // Check for minimum tag length
  if(tagName.length() < 2)
    throw std::invalid_argument("Tag length is less than 2.");
  
  // Check if the first character is '#'
  if(tagName.at(0) != '#')
    throw std::invalid_argument("First character of tagName is not '#'.");
  
  // Check each character of the tag for validity
  for(int i = 1; i < static_cast<int>(tagName.size()); i++) {
    // Ensure only letters are allowed
    if(!isalpha(tagName.at(i)))
      throw std::invalid_argument("Only letters allowed.");
    
    // Ensure only lowercase letters are allowed
    if(isupper(tagName.at(i)))
      throw std::invalid_argument("Only lowercase letters allowed.");
  }
  
  // Check for disallowed punctuation at the end of the tag
  if(tagName.at(tagName.length() - 1) == '!' || tagName.at(tagName.length() - 1) == ',' || tagName.at(tagName.length() - 1) == '.' || tagName.at(tagName.length() - 1) == '?')
    throw std::invalid_argument("No punctuation allowed.");

  // Set the tag name after passing all checks
  this->tagName = tagName;
}

string Tag::getTagName() {
  // Getter method to retrieve the tag name
  return tagName;
}

vector<Post*>& Tag::getTagPosts() {
  // Getter method to retrieve the vector of tag posts
  return tagPosts;
}

void Tag::addTagPost(Post* post) {
  // Method to add a post to the vector of tag posts
  // Check if the post is nullptr before adding
  if(post == nullptr)
    throw std::invalid_argument("Nullptr");
  else
    tagPosts.push_back(post);
}
