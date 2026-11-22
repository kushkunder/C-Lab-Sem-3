#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
using namespace std;

const int SUBJECTS = 3;
const float PASS_MARK = 40.0f;

// Custom exception for invalid marks
class InvalidMarksException : public runtime_error {
public:
    InvalidMarksException(const string& msg) : runtime_error(msg) {}
};

// ---------------- Base class ----------------
class Student {
protected:
    int rollNo;
    string name;

public:
    Student(int r = 0, string n = "") : rollNo(r), name(n) {}
    virtual ~Student() {}                       // virtual destructor

    int getRollNo() const { return rollNo; }
    string getName() const { return name; }

    // Virtual functions
    virtual void displayDetails() const {
        cout << "Roll No: " << rollNo << ", Name: " << name << endl;
    }
    virtual void displayMarks() const {
        cout << "Marks not available for a plain Student." << endl;
    }
    virtual float getTotal() const { return 0; }
    virtual string toRecord() const {
        return to_string(rollNo) + "," + name;
    }
};

// ---------------- Derived class ----------------
class Result : public Student {
private:
    float marks[SUBJECTS];

    static void validate(float m) {
        if (m < 0 || m > 100)
            throw InvalidMarksException("Invalid marks " + to_string((int)m) +
                                        ": must be between 0 and 100.");
    }

public:
    Result(int r, string n, float m1, float m2, float m3) : Student(r, n) {
        validate(m1);
        validate(m2);
        validate(m3);
        marks[0] = m1;
        marks[1] = m2;
        marks[2] = m3;
    }

    float getTotal() const override {
        float t = 0;
        for (int i = 0; i < SUBJECTS; i++) t += marks[i];
        return t;
    }

    float getPercentage() const { return getTotal() / SUBJECTS; }

    string getResult() const {
        for (int i = 0; i < SUBJECTS; i++)
            if (marks[i] < PASS_MARK) return "FAIL";
        return "PASS";
    }

    void displayDetails() const override {
        Student::displayDetails();
    }

    void displayMarks() const override {
        cout << "Marks: ";
        for (int i = 0; i < SUBJECTS; i++) cout << marks[i] << " ";
        cout << "| Total: " << getTotal()
             << " | Percentage: " << getPercentage() << "%"
             << " | Result: " << getResult() << endl;
    }

    string toRecord() const override {
        return Student::toRecord() + "," + to_string(marks[0]) + "," +
               to_string(marks[1]) + "," + to_string(marks[2]);
    }
};

// ---------------- File handling ----------------
void saveToFile(const vector<unique_ptr<Student>>& students, const string& filename) {
    ofstream out(filename);
    if (!out) throw runtime_error("Cannot open file for writing: " + filename);
    for (const auto& s : students) out << s->toRecord() << endl;
}

void loadFromFile(vector<unique_ptr<Student>>& students, const string& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error("Cannot open file for reading: " + filename);

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string roll, name, m1, m2, m3;
        getline(ss, roll, ',');
        getline(ss, name, ',');
        getline(ss, m1, ',');
        getline(ss, m2, ',');
        getline(ss, m3, ',');
        try {
            students.push_back(make_unique<Result>(stoi(roll), name,
                               stof(m1), stof(m2), stof(m3)));
        } catch (const exception& e) {
            cout << "Skipping bad record (" << line << "): " << e.what() << endl;
        }
    }
}

void displayAll(const vector<unique_ptr<Student>>& students) {
    for (const auto& s : students) {
        s->displayDetails();    // virtual call -> Result version at runtime
        s->displayMarks();
        cout << endl;
    }
}

// Helper that adds a student and handles invalid marks
void addStudent(vector<unique_ptr<Student>>& students,
                int roll, string name, float m1, float m2, float m3) {
    try {
        students.push_back(make_unique<Result>(roll, name, m1, m2, m3));
        cout << "Added: " << name << endl;
    } catch (const InvalidMarksException& e) {
        cout << "Could not add " << name << " -> " << e.what() << endl;
    }
}

int main() {
    vector<unique_ptr<Student>> students;
    const string filename = "records.txt";

    cout << "=== Adding students ===" << endl;
    addStudent(students, 1, "Amit", 78, 85, 90);
    addStudent(students, 2, "Rahul", 55, 62, 48);
    addStudent(students, 3, "Anita", 92, 88, 95);
    addStudent(students, 4, "Neha", 35, 60, 70);     // fails one subject
    addStudent(students, 5, "Ravi", 105, 70, 80);    // invalid marks -> exception

    cout << "\n=== Student details, total marks and results ===" << endl;
    displayAll(students);

    // Store records in a file
    saveToFile(students, filename);
    cout << "Records saved to " << filename << endl;

    // Retrieve records from the file
    vector<unique_ptr<Student>> loaded;
    loadFromFile(loaded, filename);
    cout << "\n=== Records retrieved from file (" << loaded.size() << ") ===" << endl;
    displayAll(loaded);

    // STL algorithm + lambda: students scoring above a specified total
    float limit = 240;
    cout << "=== Students scoring above " << limit << " total marks ===" << endl;
    vector<const Student*> toppers;
    for (const auto& s : loaded) toppers.push_back(s.get());

    auto newEnd = remove_if(toppers.begin(), toppers.end(),
                            [limit](const Student* s) { return s->getTotal() <= limit; });
    toppers.erase(newEnd, toppers.end());

    for (const Student* s : toppers) {
        s->displayDetails();
        s->displayMarks();
    }

    int count = count_if(loaded.begin(), loaded.end(),
                         [limit](const unique_ptr<Student>& s) { return s->getTotal() > limit; });
    cout << "\nNumber of students above " << limit << ": " << count << endl;

    return 0;
}
