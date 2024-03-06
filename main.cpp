# include <iostream>
# include <string>
# include <stdexcept>
# include "Network.h"

/*
Gaurav Dama
Small social media app named CPPeers with posting and viewing capabilities.
*/

using std::cout, std::cin, std::endl, std::string, std::vector;

void printMenu() {
    cout << "Welcome to CPPeers" << endl;
    cout << "The options are: " << endl;
    cout << "1. load data file and add information" << endl;
    cout << "2. show posts by user" << endl;
    cout << "3. show posts with hashtag" << endl;
    cout << "4. show most popular hashtag" << endl;
    cout << "9. quit" << endl;
    cout << "--------> Enter your option: ";
}

void processLoad(Network& cppeers) {
    string fileName = "";
    cout << "Enter filename: ";
    cin >> fileName;
    cppeers.loadFromFile(fileName);
}

void processPostsByUser(Network& cppeers) {
    // TODO: implement
    string name;
    cout << "Enter username: ";
    cin >> name;
    for(unsigned long i = 0; i < cppeers.getPostsByUser(name).size(); i++) {
        cout << cppeers.getPostsByUser(name)[i]->getPostText() << endl;
    }
}

void processPostsWithHashtags(Network& cppeers) {
    // TODO: implement
    string tag;
    cout << "Enter tagname: ";
    cin >> tag;
    for(unsigned long i = 0; i < cppeers.getPostsWithTag(tag).size(); i++) {
        cout << cppeers.getPostsWithTag(tag)[i]->getPostText() << endl;
    }
}

void processMostPopularHashtag(Network& cppeers) {
    // TODO: implement
    for(unsigned long i = 0; i < cppeers.getMostPopularHashtag().size(); i++) {
        cout << cppeers.getMostPopularHashtag()[i] << endl;
    }
}

int main() {
    // TODO: uncomment at the end
    try {
        Network cppeers;

        int choice = 0;

        do {
            printMenu();
            cin >> choice;
            switch(choice) {
                case 1: {
                    processLoad(cppeers);
                    break;
                }
                case 2: {
                    processPostsByUser(cppeers);
                    break;
                }
                case 3: {
                    processPostsWithHashtags(cppeers);
                    break;
                }
                case 4: {
                    processMostPopularHashtag(cppeers);
                    break;
                }
            }
        } while (choice != 9);
    } catch (std::exception& exc) {
        std::cout << exc.what() << endl;
    }


    return 0;
}
