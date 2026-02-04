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

                cout << "Student Age: ";
                int age;
                cin>>age;
                student.setAge(age);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Student Phone Number (10-12 digits): ";
                string phoneNumber;
                cin >> phoneNumber;
                student.setPhoneNumber(phoneNumber);

                cout << "\n" << studentController.addStudent(grade, student) << endl;
            }

            else if (studentProcess == 3) { // Edit Student
                cout << "\nEnter Student ID to edit: ";
                string id;
                cin >> id;

                Student newData;

                cout << "New Full Name: ";
                cin.ignore();
                string name;
                getline(cin, name);
                newData.setName(name);

                cout << "New Grade (1-12): ";
                int grade;
                cin >> grade;
                newData.setSchoolYear(grade);

                cout << "New Age: ";
                int age;
                cin>>age;
                newData.setAge(age);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "New Phone Number: ";
                string phone;
                cin >> phone;
                newData.setPhoneNumber(phone);

                cout << "GPA: ";
                double gpa;
                cin >> gpa;
                newData.setGpa(gpa);

                cout << studentController.editStudent(id, newData) << endl;
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

                cout << "Grade (1-12): ";
                int grade;
                cin>>grade;
                course.setGrade(grade);

                cout << "Subject Hours: ";
                int hours;
                cin >> hours;
                course.setSubjectHours(hours);

                cout << "Course Teacher ID: ";
                string id;
                getline(cin, id);
                course.setCourseTeacherId(id);

                cout << courseController.addCourse(grade, course) << endl;
            }

            else if (courseProcess == 3) { // Edit Course
                cout << "\nEnter Course Id to edit: ";
                string id;
                cin >> id;

                Course newData;

                cout << "New Course Name: ";
                cin.ignore();
                string name;
                getline(cin, name);
                newData.setName(name);

                cout << "New Grade (1-12): ";
                int grade;
                cin >> grade;
                newData.setGrade(grade);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "New Subject Hours: ";
                int hours;
                cin >> hours;
                newData.setSubjectHours(hours);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "New Course Teacher ID: ";
                string teacherId;
                getline(cin, teacherId);
                newData.setCourseTeacherId(teacherId);


                cout << courseController.editCourse(id, newData) << endl;
            }

        }
        break;


        case 3: // About Teacher
        {
            cout << "\n\t************ TEACHER BRANCH ************\n";
            showProcesses("Teacher");

            int teacherProcess;
            cin >> teacherProcess;

            if (teacherProcess == 1) { // Add Teacher
                cout << "- Please Enter Teacher Data:" << endl;
                Teacher teacher;
                cin.ignore();

                cout << "Teacher Name: ";
                string name;
                getline(cin, name);
                teacher.setName(name);

                cout << "Teacher Age: ";
                int age;
                cin>>age;
                teacher.setAge(age);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Teacher experience years: ";
                int experienceYears;
                cin >> experienceYears;
                teacher.setExperienceYears(experienceYears);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

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

            else if (teacherProcess == 3) { // Edit Teacher

                cout << "\nEnter Teacher Id to edit: ";
                string id;
                cin >> id;

                Teacher newData;

                cout << "New Teacher Name: ";
                cin.ignore();
                string name;
                getline(cin, name);
                newData.setName(name);

                cout << "New Teacher Age: ";
                int age;
                cin>>age;
                newData.setAge(age);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "New Teacher experience years: ";
                int experienceYears;
                cin >> experienceYears;
                newData.setExperienceYears(experienceYears);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "New Teacher Grade (1-12): ";
                int grade;
                cin >> grade;
                newData.setTeacherGrade(grade);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "New Teacher monthly salary : ";
                double MonthlySalary;
                cin >> MonthlySalary;
                newData.setMonthlySalary(MonthlySalary);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout <<"New Teacher subject : ";
                string subject;
                getline(cin,subject);
                newData.setTeacherSubject(subject);

                cout << teacherController.editTeacher(id, newData) << endl;
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
