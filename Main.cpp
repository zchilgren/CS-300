#include <iostream>
#include <fstream>
#include "CSVparser.hpp"

using namespace std;


// Global defenitions

// Courses have an Id, name, and Vector that holds prereqs
struct Course {
    string courseNum;
    string courseTitle;
    vector<string> preReqs;

    Course() {}
};

// Binary Search Tree set up
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

//Class defenition
class CourseBST {

    private:
        Node* root;
        
        void addNode(Node* node, Course course);
        void printSampleSchedule(Node* node);
        void printCourseInfo(Node* node, string courseNum);

    public:
        CourseBST();
        virtual -CourseBST();
        void deleteRecursive(Node* node);
        void Insert(Course course);
        int numPrereqs(Course course);
        void printSampleSchedule();
        void printCourseInfo(string courseNum);
};

//construct Binary Search Tree and initialize root
CourseBST :: CourseBST()
 {
    root = nullptr;
 }
//Deletes tree by recursivly deleting each node
 CourseBST :: -CourseBST() 
 {
    deleteRecursive(root);
 }

 void CourseBST :: deleteRecursive(Node* node) 
 {
    if (node){
        deleteRecursive(node->left);
        deleteRecursive(node->right);
        delete node;
    }
 }
//Insert node into tree
 void CourseBST :: Insert(Course course)
 {
    // if root is not initialized set with current course info
    if (root == nullptr) {
        root = new Node(course);
    }
    // else add node to tree
    else {
        this->addNode(root, course);
    }
 }

// returns number of prereqs for a course
 int CourseBST :: numPrereqs(Course course)
 {
    int count = 0;
    for (unsigned int i = 0; i < course.preReqs.size(); i++) {
        if (course.preReqs.at(i).length() > 0) {
            count++;
        }
    }
    return count;
 }

// displays course info if found
 void CourseBST :: printCourseInfo(string courseNum)
 {
    this->printCourseInfo(root, courseNum);
 }

// uses insert to add nodes to tree
 void CourseBST :: addNode(Node* node, Course course)
 {
    // current courseNum is less than currennt nodes courseNum
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    // current courseNum is equal or greater than nodes courseNum
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
 }

//prints all courses in order
 void CourseBST :: printSampleSchedule(Node* node)
 {
    if (node != nullptr){
        printSampleSchedule(node->left);
        cout << node->course.courseNum << ", " << node->course.courseTitle << endl;
        printSampleSchedule(node->right);
    }
    return;
 }
 
 //displays all information for a selected course
 void CourseBST :: printCourseInfo(Node* curr, string courseNum)
 {
    while (curr != nullptr) {
        if (curr->course.courseNum.compare(courseNum) == 0) {
            cout << endl << curr->course.courseNum << ", " << curr->course.courseTitle << endl;
            unsigned int size = numPrereqs(curr->course);
            cout << "Prerequsits: ";
            unsigned int i = 0;
            for (i=0; i < size; i++) {
                cout << curr->course.preReqs.at(i);
                if (i != size - 1) {
                    cout << ", ";
                }
            }
            if (i == 0) {
                cout << "No prerequisites required" << endl;
                return;
            }
        }
        else if (courseNum.compare(curr->course.courseNum) < 0) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    cout << "Course " << courseNum << " is not found" << endl;
 }

// loads courses from a given file
 bool loadCourses(string csvPath, CourseBST* coursesBST)
 {
    try {
        ifstream courseFile(csvPath);

        if (courseFile.is_open()) {
            while (!courseFile.eof()) {
                vector<string> courseInfo;
                string courseData;
                getline(courseFile, courseData);
                while (courseData.length() > 0) {
                    unsigned int comma = courseData.find(',');
                    if (comma < 100) {
                        courseInfo.push_back(courseData.substr(0, comma));
                        courseData.erase(0, comma + 1);
                    }
                    else {
                        courseInfo.push_back(courseData.substr(0, courseData.length()));
                        courseData = "";
                    }
                }
                Course course;
                course.courseNum = courseInfo[0];
                course.courseTitle = courseInfo[1];

                for (unsigned int i = 2; i < courseInfo.size(); i++) {
                    course.preReqs.push_back(courseInfo[i]);
                }
                coursesBST->Insert(course);
            }
            courseFile.close();
            return true;
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
    return false;
 }

// main method for program
 int main(int argc, char* argv[])
 {
    string csvPath, courseId;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            break;
        case 3:
            csvPath = argv[1];
            courseId = argv[2];
            break;
        default:
            csvPath = "";
            break;
    }

    CourseBST* coursesBST = nullptr;
    cout << "\nWelcome to course planner\n" << endl;

    string choice = "0";
    int userChoice = choice[0] - '0';
// presets selection menu and takes input choice
    while (userChoice != 0) {
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "0. Exit App" << endl;
        cout << "Select Option: " << endl;
        cin >> choice;

        if (choice.length() == 1) {
            userChoice = choice[0] - '0';
        }
        else {
            userChoice = 0;
        }

        bool success = 1;

        switch (userChoice) {
            // loads data for courses
            case 1:
                if (coursesBST == nullptr) {
                    coursesBST = new CourseBST();
                }
                if (csvPath.length() == 0) {
                    cout << "Select file: ";
                    cin >> csvPath;
                }
                success = loadCourses(csvPath, coursesBST);
                if (success) {
                    cout << "Courses loaded" << endl;
                }
                else {
                    cout << "Error: File Not Found" << endl;
                }
                csvPath = "";
                break;
            // prints sample schedule with given courses
            case 2:
                if (coursesBST != nullptr && success) {
                    cout << "Presented Sample Schedule:" << endl;
                    coursesBST->printSampleSchedule();
                    cout << endl;
                }
                else {
                    cout << "Courses need loaded, please load courses" << endl;
                    break;
                }
            // search and display for course info
            case 3:
                if (coursesBST != nullptr && success) {
                    if (courseId.length() == 0) {
                        cout << "Choose a coures";
                        cin >> courseId;
                        for (auto& userChoice : courseId) userChoice = toupper(userChoice);
                    }
                    coursesBST->printCourseInfo(courseId);
                    cout << endl;
                }
                else {
                    cout << "Courses need loaded, please load courses" << endl;
                    courseId = "";
                    break;
                }
            // exits program menu
            default:
                if (userChoice != 0) {
                    cout << choice << "Error: not a valid selection" << endl;
                }
                break;
        } 
    }
    cout << "Thank you for using course planner" << endl;

    return 0;
 }