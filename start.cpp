#include <iostream>
#include "controller/Controller.h"
using namespace std;


void displaySystem() {
    cout << "\t********************* Welcome To Student System *********************" << endl << endl;
    cout << "The Process you need is : " << endl;
    cout << "1- About Student\t2- About Course" << endl;
    cout << "3- About Teacher\t4- Exit" << endl;
}

void showProcesses(string s) {
    cout <<"Please enter the process you need !" << endl;
    cout << "1- Add " << s << "\t\t2- Remove " << s << endl;
    cout << "3- Edit " << s << "\t\t4- Show " << s << " Info" << endl;
}

int main() {
    StudentRepositoryImpl studentRepo;
    CourseRepositoryImpl courseRepo;
    TeacherRepositoryImpl teacherRepo;

    StudentServiceImpl studentService(studentRepo);
    CourseServiceImpl courseService(courseRepo);
    TeacherServiceImpl teacherService(teacherRepo);

    StudentController studentController(studentService);
    CourseController courseController(courseService);
    TeacherController teacherController(teacherService);

    displaySystem();

    int process;
    bool flag = true;

    while (flag) {
        cout << "\nEnter your choice (1-4): ";
        cin >> process;

        switch (process) {

            case 1: // About Student
            {
                cout << "\n\t************ STUDENT BRANCH ************\n";
                showProcesses("Student");

                int studentProcess;
                cin >> studentProcess;

                if (studentProcess == 1) { // Add Student
                    Student student;
                    cout << "- Please enter the required student information below:" << endl;

                    cout << "Student Full Name: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    string name;
                    getline(cin, name);
                    student.setName(name);

                    cout << "Student School Grade (1-12): ";
                    int grade;
                    cin >> grade;

                    cout << "Student Phone Number (10-12 digits): ";
                    string phoneNumber;
                    cin >> phoneNumber;
                    student.setPhoneNumber(phoneNumber);

                    cout << "\n" << studentController.addStudent(grade, student) << endl;
                }
            }
            break;

            case 2: // About Course
            {
                cout << "\n\t************ COURSE BRANCH ************\n";
                showProcesses("Course");

                int courseProcess;
                cin >> courseProcess;

                if (courseProcess == 1) { // Add Course
                    Course course;
                    cin.ignore();

                    cout << "- Please Enter Course Data:" << endl;

                    cout << "Course Name: ";
                    string courseName;
                    getline(cin, courseName);
                    course.setName(courseName);

                    cout << "Academic Year: ";
                    string year;
                    getline(cin, year);
                    course.setAcademicYear(year);

                    cout << "Subject Hours: ";
                    int hours;
                    cin >> hours;
                    course.setSubjectHours(hours);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;

                    cout << courseController.addCourse(grade, course)<<endl;
                }
            }
            break;

            case 3: // About Teacher
            {
                cout << "\n\t************ TEACHER BRANCH ************\n";
                showProcesses("Teacher");

                int courseProcess;
                cin >> courseProcess;

                if (courseProcess == 1) { // Add Teacher

                cout << "- Please Enter Teacher Data:" << endl;
                Teacher teacher;
                cin.ignore();

                cout << "Teacher Name: ";
                string name;
                getline(cin, name);
                teacher.setName(name);

                cout << "Teacher Subject: ";
                string subject;
                getline(cin, subject);
                teacher.setTeacherSubject(subject);

                cout << "Grade (1-12): ";
                int grade;
                cin >> grade;

                cout << "Monthly Salary: ";
                double salary;
                cin >> salary;
                teacher.setMonthlySalary(salary);

               cout << teacherController.addTeacher(grade, teacher) << endl;
            }
            }
            break;




            case 4:
                cout << "\nExiting program..." << endl;
                flag = false;
                break;

            default:
                cout << "\nInvalid Choice. Please enter a number between 1-4." << endl;
        }
    }

    return 0;
}
