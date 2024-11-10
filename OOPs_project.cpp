#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;
class College
{
protected:
    string collegeName;

public:
    College() : collegeName("SVNIT") {}

    string getCollegeName() const { return collegeName; }
};

class Department
{
private:
    string departmentName;

public:
    vector<string> subjects;
    Department() {}
    Department(string name, const vector<string> &subs)
        : departmentName(name), subjects(subs) {}
        
    string getName() const { return departmentName; }

    void printSubjects() const
    {
        cout << "Subjects in " << departmentName << ": ";
        for (const auto &subject : subjects)
        {
            cout << subject << " ";
        }
        cout << endl;
    }
};

class Student
{
protected:
    string DOB;
    vector<int> marks;

public:
    string name;
    int studentID;
    int year;
    int regYear;
    string deptName;

    Student(string n, int id, string dob, int Year, int regYear, string department) : name(n), studentID(id), DOB(dob), year(Year), regYear(regYear), deptName(department) {}

    string getKey() const { return to_string(studentID) + to_string(regYear); }

    void setMarks(const vector<int> &mks) { marks = mks; }

    const vector<int> &getMarks() const { return marks; }
    string printdob() const
    {
        std::ostringstream oss;
        oss << DOB;
        return oss.str();
    }
    void printMarks() const
    {
        cout << "Marks for " << name << ": ";
        for (const auto &mark : marks)
        {
            cout << mark << " ";
        }
        cout << endl;
    }
};

class Result
{
public:
    static int calculateTotalMarks(const Student &student)
    {
        int total = 0;
        for (const auto &mark : student.getMarks())
        {
            total += mark;
        }
        return total;
    }
};

class StudentManagementSystem : public College, public Result
{
    friend class attendance;
private:
    vector<Student> students;
    vector<Department> departments;

    bool studentExists(const string &key)
    {
        for (const auto &student : students)
        {
            if (student.getKey() == key)
                return true;
        }
        return false;
    }
    bool departmentExists(const string &deptName) const
    {
        for (const auto &dept : departments)
        {
            if (dept.getName() == deptName)
            {
                return true;
            }
        }
        return false;
    }

public:
    void addDepartment(const Department &dept)
    {
        departments.push_back(dept);
    }

    void getMarks(const string &key, const string &deptName)
    {
        for (auto &student : students)
        {
            if (student.getKey() == key)
            {
                const Department* dept = getDepartmentByName(deptName);
                if (dept)
                {
                    vector<int> marks;
                    cout << "Enter marks for each subject in " << dept->getName() << ":\n";
                    for (const auto &subject : dept->subjects)
                    {
                        int mark;
                        cout << subject << ": ";
                        cin >> mark;
                        marks.push_back(mark);
                    }
                    student.setMarks(marks);
                }
                else
                {
                    cout << "Department '" << deptName << "' does not exist.\n";
                }
                return;
            }
        }
        cout << "Student with key '" << key << "' not found.\n";
    }

    void addStudent(Student newStudent, const Department &dept)
    {
        string key = newStudent.getKey();
        newStudent.deptName = dept.getName();
        if (!studentExists(key))
        {
            students.push_back(newStudent);
            sort(students.begin(), students.end(), [](const Student &a, const Student &b)
                 { return a.getKey() < b.getKey(); });
            cout << "Student added successfully.\n";
        }
        else
        {
            cout << "Student already exists. Updating record.\n";
            for (auto &student : students)
            {
                if (student.getKey() == key)
                {
                    student = newStudent;
                    break;
                }
            }
        }
    }

    void printStudentsInDepartment(const string &deptName) const
    {
        const Department *dept = getDepartmentByName(deptName);

        if (!dept)
        {
            cout << "Department '" << deptName << "' does not exist.\n";
            return;
        }

        cout << "Students in " << deptName << " Department:\n";
        bool found = false;

        for (const auto &student : students)
        {
            if (deptName == "CSE" || deptName == "Electrical" ||
                deptName == "Electronics" || deptName == "Civil" || deptName == "Mechanical" || deptName == "Chemical")
            {
                cout << student.name << "\n";
                found = true;
            }
        }

        if (!found)
        {
            cout << "No students found in " << deptName << " department.\n";
        }
    }
    void printStudentResult(const string &key) const
    {
        for (const auto &student : students)
        {
            if (student.getKey() == key)
            {

                cout << "Total Marks: " << calculateTotalMarks(student) << endl;
                return;
            }
        }
        cout << "Student with key '" << key << "' not found.\n";
    }

    void deleteStudent(const string &key, const string &filename,string nameToDelete)
    {
        ifstream file(filename);
        vector<string> rows;
        string line;

        if (!file.is_open())
        {
            cerr << "Error opening file!" << endl;
            return;
        }

        // Read header and add it back to rows
        getline(file, line);
        rows.push_back(line);

        // Read each line and filter out the row matching the name
        while (getline(file, line))
        {
            stringstream ss(line);
            string name;
            getline(ss, name, ','); // Assuming name is in the first column

            if (name != nameToDelete)
            {
                rows.push_back(line); // Only keep rows that don't match
            }
        }
        file.close();

        // Rewrite the file with filtered data
        ofstream outfile(filename, std::ios::out | std::ios::trunc);
        for (const auto &row : rows)
        {
            outfile << row << "\n";
        }
        outfile.close();

        cout << "Student with name '" << nameToDelete << "' deleted successfully (if found)." << endl;
    
    auto it = remove_if(students.begin(), students.end(), [&](const Student &student)
                        { return student.getKey() == key; });

    if (it != students.end())
    {
        students.erase(it, students.end());
        cout << "Student deleted successfully.\n";
    }
    else
    {
        cout << "Student not found.\n";
    }
    }

const Department *getDepartmentByName(const string &deptName) const
{
    for (const auto &dept : departments)
    {
        if (dept.getName() == deptName)
            return &dept;
    }
    return nullptr;
}
void printCommonNameStudents() const
{
    unordered_map<string, unordered_set<string>> nameToDepartments;

    for (const auto &student : students)
    {
        nameToDepartments[student.name].insert(student.deptName);
    }
    bool found = false;
    for (const auto &entry : nameToDepartments)
    {
        const string &name = entry.first;
        const unordered_set<string> &departments = entry.second;

        if (departments.size() > 1)
        {
            cout << name << " is present in the following departments: ";
            for (const auto &dept : departments)
            {
                cout << dept << " ";
            }
            cout << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No common names found across different departments." << endl;
    }
}



    void checkForDuplicateEntries() const
{
    unordered_set<string> keys;
    bool duplicateFound = false;

    for (const auto &student : students)
    {
        if (keys.count(student.getKey()) > 0)
        {
            cout << "Duplicate entry found for student: " << student.name << " with key: " << student.getKey() << endl;
            duplicateFound = true;
        }
        else
        {
            keys.insert(student.getKey());
        }
    }

    if (!duplicateFound)
    {
        cout << "No duplicate entries found.\n";
    }
}

void printAllStudents() const
{
    if (students.empty())
    {
        cout << "No students in the system.\n";
        return;
    }

    for (const auto &student : students)
    {
        cout << "Name: " << student.name << ", ID: " << student.studentID
             << ", Year: " << student.year << ", Registration Date: " << student.regYear << ", Unique Key: " << student.getKey() << endl;
    }
}
};



class attendance : public Department
{
private:
    vector<float> attendanceData;

public:
    void setattend(const vector<float> &at) { attendanceData = at; }
    // Add attendance for a student for a specific subject
    void addattendance(StudentManagementSystem &sms, const string &key, const string &deptName)
    {

        for (auto &student : sms.students)
        {
            if (student.getKey() == key)
            {
                const Department *dept = sms.getDepartmentByName(deptName);
                if (dept)
                {

                    cout << "Enter attendance for each subject in " << dept->getName() << ":\n";
                    for (const auto &subject : dept->subjects)
                    {
                        float attendance;
                        cout << subject << ": ";
                        cin >> attendance;
                        attendanceData.push_back(attendance);
                    }
                    setattend(attendanceData);
                }
            }
            else
            {
                cout << "Department '" << deptName << "' does not exist.\n";
            }
            return;
        }

        cout << "Student with key '" << key << "' not found.\n";
    }
    void displayattendance(StudentManagementSystem &sms, const string &key, const string &deptName) const
    {
        for (auto &student : sms.students)
        {
            if (student.getKey() == key)
            {                                                               // Check if the student key matches
                const Department *dept = sms.getDepartmentByName(deptName); // Get the department by name
                if (dept)
                {
                    cout << "The attendance for each subject in " << dept->getName() << " for student " << endl;

                    if (attendanceData.size() != dept->subjects.size())
                    {
                        cout << "Attendance data does not match the number of subjects in the department.\n";
                        return;
                    }

                    // Display the attendance for each subject
                    for (size_t i = 0; i < dept->subjects.size(); ++i)
                    {
                        cout << dept->subjects[i] << ": " << attendanceData[i] << "\n";
                    }
                }
                else
                {
                    cout << "Department '" << deptName << "' does not exist.\n";
                    return;
                }
            }
        }
        cout << "Student with key '" << key << "' not found.\n";
    }
};
void login();
void registr();
void forgot();
void mainMenu();

bool isLoggedIn = false;
void login()
{
    int count = 0;
    string user, pass, u, p;
    int n, r, y, rey, s;
    string name, de, d, key, dept;
    Department CSE("CSE", {"OOPs", "DBMS", "DAA", "DM", "CO"});
    Department Maths("Mathematics", {"Calculus", "Linear Algebra", "Statistics"});
    Department Mechanical("Mechanical", {"Hydraulics", "Material", "Thermodynamics"});
    Department Civil("Civil", {"Hydraulics", "Material", "Thermodynamics"});
    Department Chemical("Chemical", {"MO", "FFO", "HTO", "MTO", "IES"});
    Department Electrical("Electrical", {"EM", "SS", "DC", "ET", "EEE"});
    Department Electronics("Electronics", {"AC", "SSs", "EPBM", "PCS", "M&M"});
    attendance a;
    StudentManagementSystem sms;
    sms.addDepartment(CSE);
    sms.addDepartment(Electrical);
    sms.addDepartment(Electronics);
    sms.addDepartment(Chemical);
    sms.addDepartment(Maths);
    sms.addDepartment(Civil);
    sms.addDepartment(Mechanical);

    cout << "Please enter the following details\n";
    cout << "USERNAME: ";
    cin >> user;
    cout << "PASSWORD: ";
    cin >> pass;

    ifstream input("database.txt");
    while (input >> u >> p)
    {
        if (u == user && p == pass)
        {
            count = 1;
            isLoggedIn = true; // Set login status to true
            break;
        }
    }
    input.close();

    if (count == 1)
    {
        cout << "\nHello " << user << "\n<LOGIN SUCCESSFUL>\n";
        cout << "*************************** Welcome to Student Database System ***********************" << endl;
        cout << "+++++++++++++++++++++++++ Contains information of all the students +++++++++++++++++" << endl;
    uu:
        cout << "Enter the index of action you want to perform: \n 1. Enter student's data \n 2. Delete a student's data \n 3. Display student's data branch wise \n 4. Enter student's marks for all subjects \n 5. Get result of a student \n 6. Check same first name students in different branches \n 7. Get all the students data in a csv file \n 8.To add atendance \n 9. To display attendance \n 10.exit" << endl;
        cin >> s;
        switch (s)
        {
        case 1:
        {
            cout << "Enter the number of students you want to enter: ";
            cin >> n;
            for (int i = 0; i < n; i++)
            {
                cout << "Enter the name: ";
                cin >> name;
                cout << "Enter the registration id: ";
                cin >> r;
                cout << "Enter the DOB: ";
                cin >> d;
                cout << "Enter the year: ";
                cin >> y;
                cout << "Enter the registration year: ";
                cin >> rey;
                cout << "Enter the department: ";
                cin >> de;

                const Department *dept = sms.getDepartmentByName(de);
                if (!dept)
                {
                    cout << "Department '" << de << "' does not exist.\n";
                    continue;
                }

                Student newStudent(name, r, d, y, rey, de);
                sms.addStudent(newStudent, *dept);
                // cout << "Unique key for this student is " << newStudent.getKey();
            }
            sms.printAllStudents();
            goto uu;
        }
        case 2:
            cout << "Enter key of student to delete: ";
            cin >> key;
            sms.deleteStudent(key,"trial.csv","we");
            goto uu;
        case 3:
            cout << "Enter the name of department to display student details: ";
            cin >> dept;
            sms.printStudentsInDepartment(dept);
            goto uu;
        case 4:
            cout << "Enter the student's key and department to enter marks: ";
            cin >> key >> dept;
            sms.getMarks(key, dept);
            goto uu;
        case 5:
            cout << "Enter the student's key to get result: ";
            cin >> key;
            sms.printStudentResult(key);
            goto uu;
        case 6:
            sms.printCommonNameStudents();
            goto uu;
        case 7:
            sms.deleteStudent(key,"trial.csv","we");
            goto uu;
        case 8:
            cout << "Enter the student's key and department to enter attendance: ";
            cin >> key >> dept;
            a.addattendance(sms, key, dept);
            goto uu;
        case 9:
            cout << "Enter the student's key and department to get attendance : ";
            cin >> key >> dept;
            a.displayattendance(sms, key, dept);
            goto uu;
        case 10:
            exit(0);
        default:
            cout << "Invalid selection!" << endl;
            goto uu;
        }
    }
    else
    {
        cout << "\nLOGIN ERROR\nPlease check your username and password\n";
    }
}

void registr()
{
    string reguser, regpass;
    cout << "Enter the username: ";
    cin >> reguser;
    cout << "Enter the password: ";
    cin >> regpass;

    ofstream reg("database.txt", ios::app);
    reg << reguser << ' ' << regpass << endl;
    isLoggedIn = true; // Set login status to true upon successful registration
    cout << "\nRegistration Successful\n";
    int n, r, y, rey, s;
    string name, de, d, key, dept;
    Department CSE("CSE", {"OOPs", "DBMS", "DAA", "DM", "CO"});
    Department Maths("Mathematics", {"Calculus", "Linear Algebra", "Statistics"});
    Department Mechanical("Mechanical", {"Hydraulics", "Material", "Thermodynamics"});
    Department Civil("Civil", {"Hydraulics", "Material", "Thermodynamics"});
    Department Chemical("Chemical", {"MO", "FFO", "HTO", "MTO", "IES"});
    Department Electrical("Electrical", {"EM", "SS", "DC", "ET", "EEE"});
    Department Electronics("Electronics", {"AC", "SSs", "EPBM", "PCS", "M&M"});
    attendance a;
    StudentManagementSystem sms;
    sms.addDepartment(CSE);
    sms.addDepartment(Electrical);
    sms.addDepartment(Electronics);
    sms.addDepartment(Chemical);
    sms.addDepartment(Maths);
    sms.addDepartment(Civil);
    sms.addDepartment(Mechanical);
    cout << "*************************** Welcome to Student Database System ***********************" << endl;
    cout << "+++++++++++++++++++++++++ Contains information of all the students +++++++++++++++++" << endl;
uu:
    cout << "Enter the index of action you want to perform: \n 1. Enter student's data \n 2. Delete a student's data \n 3. Display student's data branch wise \n 4. Enter student's marks for all subjects \n 5. Get result of a student \n 6. Check same first name students in different branches \n 7. Get all the students data in a csv file \n 8.To add atendance \n 9. To display attendance \n 10.exit" << endl;
    cin >> s;
    switch (s)
    {
    case 1:
    {
        cout << "Enter the number of students you want to enter: ";
        cin >> n;
        for (int i = 0; i < n; i++)
        {
            cout << "Enter the name: ";
            cin >> name;
            cout << "Enter the registration id: ";
            cin >> r;
            cout << "Enter the DOB: ";
            cin >> d;
            cout << "Enter the year: ";
            cin >> y;
            cout << "Enter the registration year: ";
            cin >> rey;
            cout << "Enter the department: ";
            cin >> de;

            const Department *dept = sms.getDepartmentByName(de);
            if (!dept)
            {
                cout << "Department '" << de << "' does not exist.\n";
                continue;
            }

            Student newStudent(name, r, d, y, rey, de);
            sms.addStudent(newStudent, *dept);
            cout << "Unique key for thsi student is " << newStudent.getKey();
        }
        sms.printAllStudents();
        goto uu;
    }
    case 2:
        cout << "Enter key of student to delete: ";
        cin >> key;
        sms.deleteStudent(key,"trial.csv","we");
        goto uu;
    case 3:
        cout << "Enter the name of department to display student details: ";
        cin >> dept;
        sms.printStudentsInDepartment(dept);
        goto uu;
    case 4:
        cout << "Enter the student's key and department to enter marks: ";
        cin >> key >> dept;
        sms.getMarks(key, dept);
        goto uu;
    case 5:
        cout << "Enter the student's key to get result: ";
        cin >> key;
        sms.printStudentResult(key);
        goto uu;
    case 6:
        sms.printCommonNameStudents();
        goto uu;
    case 7:
        sms.deleteStudent(key,"trial.csv","we");
        goto uu;
    case 8:
        cout << "Enter the student's key and department to enter attendance: ";
        cin >> key >> dept;
        a.addattendance(sms, key, dept);
        goto uu;
    case 9:
        cout << "Enter the student's key and department to get attendance : ";
        cin >> key >> dept;
        a.displayattendance(sms, key, dept);
        goto uu;
    case 10:
        exit(0);
    default:
        cout << "Invalid selection!" << endl;
        goto uu;
    }
}

void mainMenu()
{
    int choice;
    cout << "\n*******************        MAIN MENU        *******************************\n\n";
    cout << "1. FORGOT PASSWORD (or) USERNAME\n";
    cout << "2. LOGOUT\n";
    cout << "3. EXIT\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        forgot();
        break;
    case 2:
        isLoggedIn = false; // Set login status to false to logout
        cout << "\nYou have logged out successfully.\n";
        break;
    case 3:
        cout << "Thanks for using this program.\n";
        exit(0); // Exit the program
    default:
        cout << "Invalid choice. Please try again.\n";
    }
}

void forgot()
{
    int ch;
    cout << "Forgotten? We're here to help\n";
    cout << "1. Search your ID by username\n";
    cout << "2. Search your ID by password\n";
    cout << "3. Main menu\n";
    cout << "Enter your choice: ";
    cin >> ch;

    switch (ch)
    {
    case 1:
    {
        int count = 0;
        string searchuser, su, sp;
        cout << "\nEnter your remembered username: ";
        cin >> searchuser;

        ifstream searchu("database.txt");
        while (searchu >> su >> sp)
        {
            if (su == searchuser)
            {
                count = 1;
                cout << "\nAccount found\nYour password is: " << sp << endl;
                break;
            }
        }
        searchu.close();

        if (count == 0)
        {
            cout << "\nSorry, your userID is not found in our database\n";
        }
        break;
    }
    case 2:
    {
        int count = 0;
        string searchpass, su2, sp2;
        cout << "\nEnter the remembered password: ";
        cin >> searchpass;

        ifstream searchp("database.txt");
        while (searchp >> su2 >> sp2)
        {
            if (sp2 == searchpass)
            {
                count = 1;
                cout << "\nPassword found in the database\nYour ID is: " << su2 << endl;
                break;
            }
        }
        searchp.close();

        if (count == 0)
        {
            cout << "Sorry, we cannot find your password in our database\n";
        }
        break;
    }
    case 3:
        return;
    default:
        cout << "You've entered the wrong choice. Try again.\n";
    }
}

int main()
{
    int choice;

    // Loop until the user chooses to exit
    while (true)
    {
        if (!isLoggedIn)
        { // Show login/register options if not logged in
            cout << "***********************************************************************\n\n\n";
            cout << "                      Welcome to login page                               \n\n";
            cout << "*******************        MENU        *******************************\n\n";
            cout << "1. LOGIN\n";
            cout << "2. REGISTER\n";
            cout << "3. EXIT\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cout << endl;

            switch (choice)
            {
            case 1:
                login();
                break;
            case 2:
                registr();
                break;
            case 3:
                cout << "Thanks for using this program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
        else
        {
            mainMenu();
        }
    }
}
