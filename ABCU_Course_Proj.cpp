//============================================================================
// Name        : ABCU_Course_Proj.cpp
// Author      : J. Tyler O'Connor
// Version     : 1.0
// Copyright   : Copyright � 2026 SNHU COCE
// Description : Project 7-1 Submission
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


//Structure to hold course information
struct Course {
    string courseID; // unique identifier
    string courseName;
    int prereqCount;
    vector<string> prereqList;
    Course() {
        prereqCount = 0;
    }
};

//Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

//Define a class containing data members and methods to implement a binary search tree
class BinarySearchTree {

private:
    Node* root;

    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseID);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseID);
    Course Search(string courseID);
};

//Default Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

//Destructor
BinarySearchTree::~BinarySearchTree() {
    //Delete all nodes from BST
    delete root;
}

//Traverse tree in order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

//Insert a new course to existing tree
void BinarySearchTree::Insert(Course course) {
    Node* curNode = root;
    
    //Check root exists
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        //Loop through tree to insert node
        while (curNode != nullptr) {
            if (course.courseID < curNode->course.courseID) {
                if (curNode->left == nullptr) {
                    curNode->left = new Node(course);
                    curNode = nullptr;
                }
                else {
                    curNode = curNode->left;
                }
            }
            else {
                if (curNode->right == nullptr) {
                    curNode->right = new Node(course);
                    curNode = nullptr;
                }
                else {
                    curNode = curNode->right;
                }
            }
        }
    }
}

//Search and return course with specific courseID
Course BinarySearchTree::Search(string courseID) {
    Node* curNode = root;

    //Loop through BST searching for specific course ID
    while (curNode != nullptr) {
        if (curNode->course.courseID == courseID) {
            return curNode->course;
        }
        else if (courseID < curNode->course.courseID) {
            curNode = curNode->left;
        }
        else {
            curNode = curNode->right;
        }
    }
    Course course;
    return course;
}

//Print all nodes in order
void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) {
        return;
    }
    inOrder(node->left);

    //Output
    cout << "Course ID: " << node->course.courseID << " | Course Name: " << node->course.courseName << endl;
    
    inOrder(node->right);
}

//Display course information
static void displayCourse(Course course) {
        //Print initial course information
        cout << "Course ID: " << course.courseID << " | Course Name: " << course.courseName << " | Number of Prerequisites: " << course.prereqCount
            << " | List of Prerequisites: ";
        //Check for no prerequisites
        if (course.prereqCount == 0) {
            cout << course.courseName << " has no prerequisites." << endl;
        }
        //Print prerequisites
        for (int i = 0; i < course.prereqCount; i++) {
            cout << course.prereqList.at(i); 
            //Formatting based on remaining prerequisite prints
            if (course.prereqCount - 1 != i) {
                cout << ", ";
            }
            else {
                cout << "."; 
            }
        }
        cout << endl;
    return;
}

//Separate data into strings vector 
//delim algorithm found through stack overflow at: 
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
static vector<string> parser(string readLine) {
    //Specify char to separate data
    string split = ",";
    vector<string>parseStrings;
    string temp = "";
    size_t pos = 0;

    while ((pos = readLine.find(split)) != std::string::npos) {
        temp = readLine.substr(0, pos);
        parseStrings.push_back(temp);
        readLine.erase(0, pos + split.length());
    }
    parseStrings.push_back(readLine); 
    return parseStrings;
}

//Load all courses into BST
void loadCourse(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;
    
    ifstream inFS; 
    string line;
    vector<string> stringLine; 
    int lineCount = 0; 

    //open data file
    inFS.open(csvPath);
    //ensure file opens as expected
    if (!inFS.is_open()) {
        cout << "Could not open file." << endl;
    }
    //Add line from file to object
    while(!inFS.eof()){
        //Create new Course from line of file
        Course aCourse;
        lineCount++;
        int prereqCount = 0;

        getline(inFS, line);
        stringLine = parser(line);

        //Catch invalid course data
        if (stringLine.size() < 2) {
            cout << "Invalid course information skipping course " << lineCount << ". Skipping line." << endl;
       
        }
        else {
            //Count prereq and add list to object
            for (int i = 2; i < stringLine.size(); i++) {
                aCourse.prereqList.push_back(stringLine.at(i));
            }
            aCourse.courseID = stringLine[0];
            aCourse.courseName = stringLine[1];
            aCourse.prereqCount = stringLine.size() - 2;
            bst->Insert(aCourse);
        }
    }
    inFS.close();
    //Confirm completion of file reading
    cout << "File read." << endl;
}

//MAIN method
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "ABCU_Advising_Program_Input.csv";
    }
    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Show Prerequisites for specific Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        //Catch invalid choice error
        if (!(choice == 1 || choice == 2 || choice == 3 || choice == 9)) {
            cout << "Input valid choice: " << endl;
        }
        //Proceed if input is valid
        else{
            switch (choice) {

            case 1:
                // Complete the method call to load the courses
                loadCourse(csvPath, bst);

                break;

            case 2:
                //Print all loaded courses in order
                bst->InOrder();
                break;

            case 3:
                //Get course key from user
                cout << "Enter course number (case sensitive): ";
                cin >> courseKey;

                //Search for user entered key
                course = bst->Search(courseKey);

                if (!course.courseID.empty()) {
                    displayCourse(course);
                }
                else {
                    //Error handling for incorrect course ID
                    cout << "Course ID: " << courseKey << " not found." << endl;
                }
                break;
            }
        }
    }

    cout << "Good bye." << endl;

    return 0;
}


