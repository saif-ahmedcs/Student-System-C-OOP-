#include <iostream>
#include "controller/Controller.h"
using namespace std;

void displaySystem() {
    cout << "\033[41m\033[37m\t\t\********************* Welcome To The School System *********************\n\n\033[0m";
    cout << "The Process you need is:\n";
    cout << "1- About Student\t2- About Course\n";
    cout << "3- About Teacher\t4- Exit\n";
}

void showProcesses(const string& s) {
    cout << "Please enter the process you need!\n\n";

    if (s == "Teacher") {
        cout << "1- Add " << s << "\t\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t\t4- Assign Courses to " << s << "\n";
        cout << "5- Show " << s << " Info\n";
    }
    else if (s == "Student") {
        cout << "1- Add " << s << "\t\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t\t4- Assign Courses to " << s << "\n";
        cout << "5- Show " << s << " Info\n";
    }
    else if (s == "Course") {
        cout << "1- Add " << s << "\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t4- Show Assigned Students\n";
        cout << "5- Show " << s << " Info\n";
    }
    else {
        cout << "1- Add " << s << "\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t4- Show " << s << " Info\n";
    }

    cout << endl;
}

int main() {

    StudentRepositoryImpl studentRepo;
    CourseRepositoryImpl courseRepo;
    TeacherRepositoryImpl teacherRepo;

    CourseServiceImpl courseService(courseRepo, teacherRepo, studentRepo);
    StudentServiceImpl studentService(studentRepo, courseRepo);
    TeacherServiceImpl teacherService(teacherRepo, courseService);

    StudentController studentController(studentService);
    CourseController courseController(courseService);
    TeacherController teacherController(teacherService);

    displaySystem();

    int process;
    bool flag = true;

    while (flag) {
        cout << "\nEnter your choice (1-4): ";
        cin >> process;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (process) {

            case 1: {
                cout << "\n\t************ STUDENT BRANCH ************\n\n";
                showProcesses("Student");

                int studentProcess;
                cin >> studentProcess;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl;
                if (studentProcess < 1 || studentProcess > 5){
                    cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN" << endl;
                }

                if (studentProcess == 1) { // Add Student
                    Student student;
                    cout << "- Please enter the required student information below:\n";

                    cout << "Student Full Name: ";
                    string name;
                    getline(cin, name);
                    student.setName(name);

                    cout << "Student National Number: ";
                    string nn;
                    getline(cin, nn);
                    student.setStudentNationalNum(nn);

                    cout << "Student School Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    student.setSchoolYear(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Student Age: ";
                    int age;
                    cin >> age;
                    student.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Student Phone Number (10-12 digits): ";
                    string phoneNumber;
                    getline(cin, phoneNumber);
                    student.setPhoneNumber(phoneNumber);

                    cout << endl;
                    cout << studentController.addStudent(grade, student) << endl;
                }

                else if (studentProcess == 3) { // Edit Student
                    cout << "\nEnter Student ID to edit: ";
                    string id;
                    getline(cin, id);

                    Student* existingStudent = studentController.findStudentById(id);
                    if (!existingStudent) {
                        cout << "Student with ID " << id << " not found. Cannot edit.\n";
                        break;
                    }

                    Student newData;
                    cout << "------- Enter NEW DATA : -------\n";

                    cout << "Full Name: ";
                    string name;
                    getline(cin, name);
                    newData.setName(name);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    newData.setSchoolYear(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Age: ";
                    int age;
                    cin >> age;
                    newData.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Phone Number: ";
                    string phone;
                    getline(cin, phone);
                    newData.setPhoneNumber(phone);

                    cout << "GPA: ";
                    double gpa;
                    cin >> gpa;
                    newData.setGpa(gpa);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << endl;
                    cout << studentController.editStudent(id, newData) << endl;
                }

                else if (studentProcess == 4) { // Assign Courses to Student
                    cout << "\nEnter Student ID: ";
                    string studentId;
                    getline(cin, studentId);


                    cout << "How many courses to assign? ";
                    int numCourses;
                    cin >> numCourses;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (numCourses < 1) {
                        cout << "Invalid number.\n";
                        break;
                    }

                    vector<string> courseIds;
                    for (int i = 0; i < numCourses; i++) {
                        cout << "Enter Course ID #" << (i + 1) << ": ";
                        string cid;
                        getline(cin, cid);
                        courseIds.push_back(cid);
                    }

                    cout << endl;
                    cout << studentController.assignCoursesToStudent(studentId, courseIds) << endl;
                }

                else if (studentProcess == 5) { // Show Student Info
                    cout << "\nEnter Student ID: ";
                    string id;
                    getline(cin, id);
                    studentController.showStudent(id);
                }

                break;
            }

            case 2: {
                cout << "\n\t************ COURSE BRANCH ************\n";
                showProcesses("Course");

                int courseProcess;
                cin >> courseProcess;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl;

                    if (courseProcess < 1 || courseProcess > 5){
                    cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN" << endl;
                }


                if (courseProcess == 1) { // Add Course
                    Course course;
                    cout << "- Please Enter Course Data:\n";

                    cout << "Course Name: ";
                    string courseName;
                    getline(cin, courseName);
                    course.setName(courseName);

                    cout << "Course Specialization: ";
                    string specialization;
                    getline(cin, specialization);
                    course.setCourseSpecialization(specialization);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    course.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Subject Hours: ";
                    int hours;
                    cin >> hours;
                    course.setSubjectHours(hours);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << endl;
                    cout << courseController.addCourse(grade, course) << endl;
                }

                else if (courseProcess == 3) { // Edit Course
                    cout << "\nEnter Course Id to edit: ";
                    string id;
                    getline(cin, id);

                    Course* existingCourse = courseController.findCourseById(id);
                    if (!existingCourse) {
                        cout << "Course with ID " << id << " not found. Cannot edit.\n";
                        break;
                    }

                    Course newData;
                    cout << "------- Enter NEW DATA : -------\n";

                    cout << "Course Name: ";
                    string name;
                    getline(cin, name);
                    newData.setName(name);

                    cout << "Course Specialization: ";
                    string specialization;
                    getline(cin, specialization);
                    newData.setCourseSpecialization(specialization);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    newData.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Subject Hours: ";
                    int hours;
                    cin >> hours;
                    newData.setSubjectHours(hours);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << endl;
                    cout << courseController.editCourse(id, newData) << endl;
                }
                else if (courseProcess == 4) { // Show Assigned Students
                    cout << "\nEnter Course ID: ";
                    string id;
                    getline(cin, id);

                    Course* existingCourse = courseController.findCourseById(id);
                    if (!existingCourse) {
                        cout << "Course with ID " << id << " not found.\n";
                        break;
                    }

                    courseController.showCourseStudents(id);
                }
                else if (courseProcess == 5) { // Show Course Info
                    cout << "\nEnter Course Id: ";
                    string id;
                    getline(cin, id);

                    Course* existingCourse = courseController.findCourseById(id);
                    if (!existingCourse) {
                        cout << "Course with ID " << id << " not found. Cannot edit.\n";
                        break;
                    }

                    courseController.showCourse(id);
                }
                break;
            }

            case 3: {
                cout << "\n\t************ TEACHER BRANCH ************\n";
                showProcesses("Teacher");

                int teacherProcess;
                cin >> teacherProcess;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl;

                if (teacherProcess < 1 || teacherProcess > 5){
                    cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN" << endl;
                    }

                if (teacherProcess == 1) { // Add Teacher
                    Teacher teacher;
                    cout << "- Please Enter Teacher Data:\n";

                    cout << "Teacher Name: ";
                    string name;
                    getline(cin, name);
                    teacher.setName(name);

                    cout << "Teacher National Number: ";
                    string nn;
                    getline(cin, nn);
                    teacher.setTeacherNationalNum(nn);

                    cout << "Teacher Age: ";
                    int age;
                    cin >> age;
                    teacher.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher experience years: ";
                    int experienceYears;
                    cin >> experienceYears;
                    teacher.setExperienceYears(experienceYears);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher Specialization: ";
                    string specialization;
                    getline(cin, specialization);
                    teacher.setTeacherSpecialization(specialization);

                    cout << "Teacher Subject: ";
                    string subject;
                    getline(cin, subject);
                    teacher.setTeacherSubject(subject);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    teacher.setTeacherGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Monthly Salary: ";
                    double salary;
                    cin >> salary;
                    teacher.setMonthlySalary(salary);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << endl;
                    cout << teacherController.addTeacher(grade, teacher) << endl;
                }

                else if (teacherProcess == 3) { // Edit Teacher
                    cout << "\nEnter Teacher Id to edit: ";
                    string id;
                    getline(cin, id);

                    Teacher* existingTeacher = teacherController.findTeacherById(id);
                    if (!existingTeacher) {
                        cout << "Teacher with ID " << id << " not found. Cannot edit.\n";
                        break;
                    }

                    Teacher newData;
                    cout << "------- Enter NEW DATA : -------\n";

                    cout << "Teacher Name: ";
                    string name;
                    getline(cin, name);
                    newData.setName(name);

                    cout << "Teacher Age: ";
                    int age;
                    cin >> age;
                    newData.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher experience years: ";
                    int experienceYears;
                    cin >> experienceYears;
                    newData.setExperienceYears(experienceYears);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    newData.setTeacherGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher monthly salary : ";
                    double MonthlySalary;
                    cin >> MonthlySalary;
                    newData.setMonthlySalary(MonthlySalary);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher Specialization: ";
                    string specialization;
                    getline(cin, specialization);
                    newData.setTeacherSpecialization(specialization);

                    cout << "Teacher subject : ";
                    string subject;
                    getline(cin, subject);
                    newData.setTeacherSubject(subject);

                    cout << endl;
                    cout << teacherController.editTeacher(id, newData) << endl;
                }

                else if (teacherProcess == 4) { // Assign Courses To teacher
                    cout << "\nEnter Teacher ID to assign courses: ";
                    string teacherId;
                    getline(cin, teacherId);

                    Teacher* teacher = teacherController.findTeacherById(teacherId);
                    if (!teacher) {
                        cout << "Teacher with ID " << teacherId << " not found.\n";
                        break;
                    }

                    int numCourses;
                    cout << "How many courses to assign (1-3)? ";
                    cin >> numCourses;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (numCourses < 1 || numCourses > 3) {
                        cout << "Invalid number of courses. Must be between 1 and 3.\n";
                        break;
                    }

                    int totalCourses = teacher->getAssignedCourses().size() + numCourses;
                    if (totalCourses > 3) {
                        cout << "Cannot assign " << numCourses << " courses. Teacher already has "
                             << teacher->getAssignedCourses().size() << " assigned. Maximum allowed is 3.\n";
                        break;
                    }

                    vector<string> courseIds;
                    for (int i = 0; i < numCourses; i++) {
                        cout << "Enter Course ID #" << (i + 1) << ": ";
                        string cid;
                        getline(cin, cid);

                        bool duplicate = false;
                        for (int j = 0; j < courseIds.size(); j++) {
                            if (courseIds[j] == cid) {
                                duplicate = true;
                                break;
                            }
                        }

                        if (duplicate || teacher->isCourseAssigned(cid)) {
                            continue;
                        }

                        courseIds.push_back(cid);
                    }

                    if (courseIds.empty()) {
                        cout << "No new courses to assign. All entered courses are either duplicates or already assigned.\n";
                    } else {
                        cout << teacherController.assignCoursesToTeacher(teacherId, courseIds) << endl;
                    }
                    break;
                }

                else if (teacherProcess == 5) { // Show Teacher Info
                    cout << "\nEnter Teacher ID: ";
                    string id;
                    getline(cin, id);
                    teacherController.showTeacher(id);
                }

                break;
            }

            case 4:
                cout << "\nExiting program...\n";
                flag = false;
                break;

            default:
                cout << "\nInvalid Choice. Please enter a number between 1-4.\n";
        }
    }

    return 0;
}
