#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

class Student {
public:
    int rollNo;
    char name[50];
    int age;
    char course[50];

    void input() {
        cout << "Enter Roll Number: ";
        cin >> rollNo;
        cin.ignore(); // clear newline
        cout << "Enter Name: ";
        cin.getline(name, 50);
        cout << "Enter Age: ";
        cin >> age;
        cin.ignore();
        cout << "Enter Course: ";
        cin.getline(course, 50);
    }

    void display() {
        cout << setw(5) << rollNo 
             << setw(20) << name 
             << setw(5) << age 
             << setw(15) << course << endl;
    }
};

// Function Prototypes
void addStudent();
void viewStudents();
void updateStudent();
void deleteStudent();

int main() {
    int choice;
    do {
        cout << "\n===== Student Record Manager =====\n";
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Update Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 5);

    return 0;
}

void addStudent() {
    Student s;
    s.input();
    ofstream outFile("students.dat", ios::binary | ios::app);
    outFile.write((char*)&s, sizeof(Student));
    outFile.close();
    cout << "Student added successfully!\n";
}

void viewStudents() {
    Student s;
    ifstream inFile("students.dat", ios::binary);
    if (!inFile) {
        cout << "No records found.\n";
        return;
    }
    cout << setw(5) << "Roll" << setw(20) << "Name" 
         << setw(5) << "Age" << setw(15) << "Course" << endl;
    cout << "----------------------------------------------------\n";
    while(inFile.read((char*)&s, sizeof(Student))) {
        s.display();
    }
    inFile.close();
}

void updateStudent() {
    int roll;
    cout << "Enter Roll Number to update: ";
    cin >> roll;

    fstream file("students.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File not found!\n";
        return;
    }

    Student s;
    bool found = false;
    while(file.read((char*)&s, sizeof(Student))) {
        if(s.rollNo == roll) {
            cout << "Enter new details:\n";
            s.input();
            file.seekp(-sizeof(Student), ios::cur);
            file.write((char*)&s, sizeof(Student));
            cout << "Record updated successfully!\n";
            found = true;
            break;
        }
    }

    if(!found) cout << "Student not found!\n";
    file.close();
}

void deleteStudent() {
    int roll;
    cout << "Enter Roll Number to delete: ";
    cin >> roll;

    ifstream inFile("students.dat", ios::binary);
    if (!inFile) {
        cout << "No records to delete.\n";
        return;
    }
    ofstream outFile("temp.dat", ios::binary);

    Student s;
    bool found = false;
    while(inFile.read((char*)&s, sizeof(Student))) {
        if(s.rollNo != roll) {
            outFile.write((char*)&s, sizeof(Student));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if(found) cout << "Record deleted successfully!\n";
    else cout << "Student not found!\n";
}
