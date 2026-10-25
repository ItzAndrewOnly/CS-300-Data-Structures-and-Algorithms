// ProjectTwo.cpp
// ABCU course planner
// Stores course objects in a hash table and sorts course numbers for display.
// Uses only standard C++ headers. No separate project headers are needed.

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

using CourseTable = unordered_map<string, Course>;

// Remove extra spaces, tabs, and Windows line endings.
string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }
    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

string toUpper(string text) {
    for (char& letter : text) {
        letter = static_cast<char>(toupper(static_cast<unsigned char>(letter)));
    }
    return text;
}

// The supplied file uses simple comma-separated fields without quoted commas.
vector<string> splitLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream lineStream(line);
    while (getline(lineStream, field, ',')) {
        fields.push_back(trim(field));
    }
    return fields;
}

bool loadCourses(const string& fileName, CourseTable& courses) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cout << "Error: Could not open " << fileName << ".\n";
        return false;
    }

    // Build a temporary table so a bad file cannot replace valid loaded data.
    CourseTable newCourses;
    string line;
    int lineNumber = 0;
    while (getline(inputFile, line)) {
        ++lineNumber;
        // Some text editors add a UTF-8 byte order mark to the first line.
        if (lineNumber == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
            line.erase(0, 3);
        }
        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitLine(line);
        if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
            cout << "Error on line " << lineNumber
                 << ": A course number and title are required.\n";
            return false;
        }

        Course course;
        course.courseNumber = toUpper(fields[0]);
        course.title = fields[1];
        if (newCourses.find(course.courseNumber) != newCourses.end()) {
            cout << "Error on line " << lineNumber << ": Duplicate course "
                 << course.courseNumber << ".\n";
            return false;
        }

        for (size_t i = 2; i < fields.size(); ++i) {
            // Empty prerequisite columns are normal in the supplied CSV.
            if (!fields[i].empty()) {
                string prerequisite = toUpper(fields[i]);
                if (prerequisite == course.courseNumber) {
                    cout << "Error: " << course.courseNumber
                         << " cannot be its own prerequisite.\n";
                    return false;
                }
                if (find(course.prerequisites.begin(), course.prerequisites.end(),
                         prerequisite) == course.prerequisites.end()) {
                    course.prerequisites.push_back(prerequisite);
                }
            }
        }
        newCourses.emplace(course.courseNumber, course);
    }

    if (inputFile.bad()) {
        cout << "Error: The file could not be read completely.\n";
        return false;
    }
    if (newCourses.empty()) {
        cout << "Error: The file does not contain any courses.\n";
        return false;
    }

    // Check after reading all lines because prerequisites may appear later.
    for (const auto& entry : newCourses) {
        for (const string& prerequisite : entry.second.prerequisites) {
            if (newCourses.find(prerequisite) == newCourses.end()) {
                cout << "Error: " << entry.first << " lists missing prerequisite "
                     << prerequisite << ".\n";
                return false;
            }
        }
    }

    courses.swap(newCourses);
    cout << "Loaded " << courses.size() << " courses successfully.\n";
    return true;
}

void printCourseList(const CourseTable& courses) {
    vector<string> courseNumbers;
    for (const auto& entry : courses) {
        courseNumbers.push_back(entry.first);
    }

    // Hash tables are not ordered, so sort the keys before printing.
    sort(courseNumbers.begin(), courseNumbers.end());
    cout << "Here is a sample schedule:\n\n";
    for (const string& number : courseNumbers) {
        const Course& course = courses.at(number);
        cout << course.courseNumber << ", " << course.title << '\n';
    }
}

void printCourse(const CourseTable& courses, const string& courseNumber) {
    auto found = courses.find(courseNumber);
    if (found == courses.end()) {
        cout << "Course not found: " << courseNumber << ".\n";
        return;
    }

    const Course& course = found->second;
    cout << course.courseNumber << ", " << course.title << '\n';
    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None\n";
        return;
    }

    // Include titles as well as numbers, as required by the directions.
    cout << "Prerequisites:\n";
    for (const string& number : course.prerequisites) {
        cout << "  " << number << ", " << courses.at(number).title << '\n';
    }
}

void printMenu() {
    cout << "\n  1. Load Data Structure.\n"
         << "  2. Print Course List.\n"
         << "  3. Print Course.\n"
         << "  9. Exit\n\n"
         << "What would you like to do? ";
}

int main() {
    CourseTable courses;
    string choice;
    cout << "Welcome to the course planner.\n";

    while (true) {
        printMenu();
        // Read whole lines so letters and mixed input do not break the menu.
        if (!getline(cin, choice)) {
            break;
        }
        choice = trim(choice);

        if (choice == "1") {
            cout << "Enter the course data file name: ";
            string fileName;
            if (!getline(cin, fileName)) {
                break;
            }
            loadCourses(trim(fileName), courses);
        } else if (choice == "2" || choice == "3") {
            if (courses.empty()) {
                cout << "Please load the course data using option 1 first.\n";
                continue;
            }
            if (choice == "2") {
                printCourseList(courses);
            } else {
                cout << "What course do you want to know about? ";
                string courseNumber;
                if (!getline(cin, courseNumber)) {
                    break;
                }
                courseNumber = toUpper(trim(courseNumber));
                if (courseNumber.empty()) {
                    cout << "Error: Please enter a course number.\n";
                } else {
                    printCourse(courses, courseNumber);
                }
            }
        } else if (choice == "9") {
            break;
        } else {
            cout << (choice.empty() ? "Blank input" : choice)
                 << " is not a valid option.\n";
        }
    }

    cout << "Thank you for using the course planner!\n";
    return 0;
}

