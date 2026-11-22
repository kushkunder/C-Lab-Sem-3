#include <iostream>
#include <string>
using namespace std;

class Student {
private:                       // Encapsulation: data is private
    int rollNo;
    string name;
    float marks;
    static int count;          // Static member: counts number of students

public:
    // Parameterized constructor
    Student(int r, string n, float m) {
        rollNo = r;
        name = n;
        marks = m;
        count++;
    }

    // Destructor (decrements count when a student object is destroyed)
    ~Student() {
        count--;
    }

    // Getters and setters (controlled access to private data)
    int getRollNo() const { return rollNo; }
    string getName() const { return name; }
    float getMarks() const { return marks; }

    void setMarks(float m) {
        if (m >= 0 && m <= 100)
            marks = m;
        else
            cout << "Invalid marks! Must be between 0 and 100." << endl;
    }

    // Overloaded functions (same name, different parameters)
    void display() const {
        cout << "Roll No: " << rollNo << ", Name: " << name
             << ", Marks: " << marks << endl;
    }

    void display(bool showGrade) const {
        display();
        if (showGrade) {
            cout << "Grade: " << calculateGrade() << endl;
        }
    }

    void display(const char* title) const {
        cout << "--- " << title << " ---" << endl;
        display();
    }

    char calculateGrade() const {
        if (marks >= 90) return 'A';
        else if (marks >= 75) return 'B';
        else if (marks >= 60) return 'C';
        else if (marks >= 40) return 'D';
        else return 'F';
    }

    // Static member function to access the static count
    static int getCount() {
        return count;
    }

    // Overloaded > operator: compares students based on marks
    bool operator>(const Student& other) const {
        return marks > other.marks;
    }

    // Friend function: can access private members directly
    friend void showDetails(const Student& s);
};

// Definition of static member
int Student::count = 0;

// Friend function definition
void showDetails(const Student& s) {
    cout << "[Friend Function] Accessing private data directly:" << endl;
    cout << "Roll No: " << s.rollNo << ", Name: " << s.name
         << ", Marks: " << s.marks << endl;
}

int main() {
    Student s1(101, "Amit", 88.5);
    Student s2(102, "Rahul", 92.0);
    Student s3(103, "Anita", 76.5);

    cout << "Total students: " << Student::getCount() << endl << endl;

    // Overloaded display functions
    s1.display();
    cout << endl;
    s2.display(true);
    cout << endl;
    s3.display("Student Record");
    cout << endl;

    // Friend function
    showDetails(s1);
    cout << endl;

    // Overloaded > operator
    if (s1 > s2)
        cout << s1.getName() << " has higher marks than " << s2.getName() << endl;
    else
        cout << s2.getName() << " has higher marks than " << s1.getName() << endl;

    if (s2 > s3)
        cout << s2.getName() << " has higher marks than " << s3.getName() << endl;
    else
        cout << s3.getName() << " has higher marks than " << s2.getName() << endl;

    cout << endl << "Total students: " << Student::getCount() << endl;

    return 0;
}
