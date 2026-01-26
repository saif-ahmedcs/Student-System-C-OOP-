#include <iostream>
#include "controller/Controller.h"
using namespace std;


void displaySystem() {
    cout << "\t********************* Welcome To Student System *********************" << endl << endl;
    cout << "Please enter the process you need !" << endl;
    cout << "1- About Student\t2- About Course" << endl;
    cout << "3- About Teacher\t4- Exit" << endl;
}

void showProcesses(string s) {
    cout << "1- Add " << s << "\t\t2- Remove " << s << endl;
    cout << "3- Edit " << s << "\t\t4- Show " << s << " Info" << endl;
}

int main() {
    StudentRepositoryImpl studentRepo;
    StudentServiceImpl studentService(studentRepo);
    StudentController studentController(studentService);

    CourseRepositoryImpl courseRepo;
    CourseServiceImpl courseService(courseRepo);
    CourseController courseController(courseService);

    TeacherRepositoryImpl teacherRepo;
    TeacherServiceImpl teacherService(teacherRepo);
    TeacherController teacherController(teacherService);

    displaySystem();

    int process;
    bool flag = true;

    while (flag) {
        cout << "\nEnter process number: ";
        cin >> process;

        switch (process) {

            case 1: // Student
            {
                showProcesses("Student");

                int studentProcess;
                cin >> studentProcess;

                if (studentProcess == 1) {
                    cout << "Please Enter Student Data :" << endl;
                    Student student;

                    cin.ignore();
                    cout << "Enter Student Name:" << endl;
                    string name;
                    getline(cin, name);
                    student.setName(name);

                    cout << "Enter Student GPA:" << endl;
                    double gpa;
                    cin >> gpa;
                    student.setGpa(gpa);

                    cout << "Enter Student school grade:" << endl;
                    int grade;
                    cin >> grade;

                    cout << "Enter Student phone number:" << endl;
                    string phoneNumber;
                    cin >> phoneNumber;
                    student.setPhoneNumber(phoneNumber);

                    cout << studentController.addStudent(grade, student) << endl;
                }
            }
            break;

            case 2: // Course
            {
                showProcesses("Course");

                cout << "Please Enter Course Data :" << endl;
                Course course;

                cin.ignore();
                cout << "Enter Course Name:" << endl;
                string courseName;
                getline(cin, courseName);
                course.setName(courseName);

                cout << "Enter Academic Year:" << endl;
                string year;
                getline(cin, year);
                course.setAcademicYear(year);

                cout << "Enter Subject Hours:" << endl;
                int hours;
                cin >> hours;
                course.setSubjectHours(hours);

                cout << "Enter Grade (1-12):" << endl;
                int grade;
                cin >> grade;

                if (courseController.addCourse(grade, course))
                    cout << "Course added successfully!" << endl;
                else
                    cout << "Cannot add course. Limit reached or invalid grade." << endl;
            }
            break;

            case 3: // Teacher
            {
                showProcesses("Teacher");

                cout << "Please Enter Teacher Data :" << endl;
                Teacher teacher;

                cin.ignore();
                cout << "Enter Teacher Name:" << endl;
                string name;
                getline(cin, name);
                teacher.setName(name);

                cout << "Enter Teacher Subject:" << endl;
                string subject;
                getline(cin, subject);
                teacher.setTeacherSubject(subject);

                cout << "Enter Teacher Monthly Salary:" << endl;
                double salary;
                cin >> salary;
                teacher.setMonthlySalary(salary);

                cout << "Enter Grade (1-12):" << endl;
                int grade;
                cin >> grade;

                if (teacherController.addTeacher(grade, teacher))
                    cout << "Teacher added successfully!" << endl;
                else
                    cout << "Cannot add teacher. Limit reached or invalid grade." << endl;
            }
            break;

            case 4: // Exit
                cout << "Exiting program..." << endl;
                flag = false;
                break;

            default:
                cout << "Invalid Choice. Please enter 1-4." << endl;
        }
    }

    return 0;
}
