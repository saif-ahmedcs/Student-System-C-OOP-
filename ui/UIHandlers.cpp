#include "UIHandlers.h"
#include "../common/SchoolConstants.h"
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace std;


int readInt(const string& prompt) {
    if (!prompt.empty()) {
        cout << prompt;
    }

    string line;
    getline(cin, line);
    try {
        if (line.empty()) {
            return -1;
        }

        return stoi(line);
    }
    catch (const exception&) {
        return -1;
    }
}


double readDouble(const string& prompt) {
    if (!prompt.empty()) {
        cout << prompt;
    }

    string line;
    getline(cin, line);
    try {
        if (line.empty()) {
            return -1.0;
        }
        return stod(line);
    } catch (const exception&) {
        return -1.0;
    }
}


bool confirmAction(const string& message) {
    cout << message << " (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return confirm == 'y' || confirm == 'Y';
}


void displayMainMenu() {
    cout << "\033[41m\033[37m\t\t********************* Welcome To The School System *********************\n\n\033[0m";
    cout << "The Process you need is:\n";
    cout << "1- About Student\t2- About Course\n";
    cout << "3- About Teacher\t4- Exit\n";
}

void showProcesses(const string& s) {
    cout << "Please enter the process you need!\n\n";
    if (s == "Teacher") {
        cout << "1- Add " << s << "\t\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t\t4- Assign Courses to " << s << "\n";
        cout << "5- Show " << s << " Info\t\t6- Unassign Course from " << s << "\n";
        cout << "7- List All Teachers by Grade\n";
    } else if (s == "Student") {
        cout << "1- Add " << s << "\t\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t\t4- Assign Courses to " << s << "\n";
        cout << "5- Show " << s << " Info\t\t6- List All Students by Grade\n";
        cout << "7- List All Students by GPA\n";
    } else if (s == "Course") {
        cout << "1- Add " << s << "\t\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t\t4- Show ALL Assigned Students\n";
        cout << "5- Show " << s << " Info\t\t6- Show Course students (By Teacher)\n";
        cout << "7- Replace Teacher in Course\t8- List All Courses by Grade\n";
    }
    cout << "\n";
}


void handleAddStudent(StudentController& studentCtrl) {
    Student student;

    cout << "- Please enter the required student information below:\n";

    cout << "Student Full Name: ";
    string name;
    getline(cin, name);
    student.setName(name);

    cout << "Student National Number: ";
    string nn;
    getline(cin, nn);
    student.setNationalNumber(nn);

    int grade = readInt("Student School Grade (1-12): ");
    student.setGrade(grade);

    int age = readInt("Student Age: ");
    student.setAge(age);

    cout << "Student Phone Number (10-12 digits): ";
    string phone;
    getline(cin, phone);
    student.setPhoneNumber(phone);

    cout << "\n";

    string result = studentCtrl.addStudent(grade, student);
    cout << result << "\n";

    if (result.find("failed") == string::npos &&
        result.find("exists") == string::npos &&
        result.find("Invalid") == string::npos &&
        result.find("Maximum") == string::npos &&
        result.find("maximum") == string::npos) {
        studentCtrl.save();
     }
}


void handleRemoveStudent(StudentController& studentCtrl) {
    cout << "\nEnter Student ID to remove: ";
    string id;
    getline(cin, id);

    Student* s = studentCtrl.findStudentById(id);
    if (!s) {
        cout << "Student not found.\n";
        return;
    }

    cout << "\nYou are about to remove student: " << s->getName() << " (ID: " << s->getId() << ")\n";

    if (!confirmAction("Are you sure?")) {
        cout << "Operation cancelled.\n";
        return;
    }

    string result = studentCtrl.removeStudent(id);
    cout << result << "\n";

    if (result.find("not found") == string::npos) {
        studentCtrl.save();
    }
}


void handleEditStudent(StudentController& studentCtrl) {
    cout << "\nEnter Student ID to edit: ";
    string id;
    getline(cin, id);

    if (!studentCtrl.findStudentById(id)) {
        cout << "Student not found.\n";
        return;
    }

    Student newData;

    cout << "------- Enter NEW DATA : -------\n";

    cout << "Full Name: ";
    string name;
    getline(cin, name);
    newData.setName(name);

    int grade = readInt("Grade (1-12): ");
    newData.setGrade(grade);

    int age = readInt("Age: ");
    newData.setAge(age);

    cout << "Phone Number: ";
    string phone;
    getline(cin, phone);
    newData.setPhoneNumber(phone);

    double gpa = readDouble("GPA: ");
    newData.setGpa(gpa);

    cout << "\n";

    string result = studentCtrl.editStudent(id, newData);
    cout << result << "\n";

    if (result.find("not found") == string::npos && result.find("cannot") == string::npos) {
        studentCtrl.save();
    }
}


void handleAssignCoursesToStudent(StudentController& studentCtrl, CourseController& courseCtrl) {
    cout << "\nEnter Student ID: ";
    string studentId;
    getline(cin, studentId);

    Student* stu = studentCtrl.findStudentById(studentId);
    if (!stu) {
        cout << "Student not found.\n";
        return;
    }

    int maxCourses = courseCtrl.getMaxCoursesForGrade(stu->getGrade());

    int numCourses = readInt("How many courses to assign (1-" + to_string(maxCourses) + ")? ");
    if (numCourses < 1 || numCourses > maxCourses) {
        cout << "Invalid number. Must be between 1 and " << maxCourses << ".\n";
        return;
    }

    vector<string> courseIds;
    vector<string> teacherNames;

    for (int i = 0; i < numCourses; i++) {
        cout << "\n--- Course #" << (i + 1) << " ---\n";

        cout << "Enter Course ID: ";
        string cid;
        getline(cin, cid);

        vector<string> av = courseCtrl.getCourseTeacherNames(cid);
        string sel;

        if (!av.empty()) {
            cout << "Available teachers:\n";
            for (int j = 0; j < (int)av.size(); j++) {
                cout << (j + 1) << ". " << av[j] << "\n";
            }

            if (av.size() == 1) {
                sel = av[0];
                cout << "Auto-selected: " << sel << "\n";
            } else {
                int choice = readInt("Select teacher number: ");
                if (choice >= 1 && choice <= (int)av.size()) {
                    sel = av[choice - 1];
                }
            }
        }

        courseIds.push_back(cid);
        teacherNames.push_back(sel);
    }

    string result = studentCtrl.assignCoursesToStudent(studentId, courseIds, teacherNames);
    cout << "\n" << result << "\n";

    if (result.find("not found") == string::npos &&
        result.find("failed") == string::npos &&
        result.find("No courses") == string::npos)
    {
        studentCtrl.save();
    }
}


void handleShowStudent(StudentController& studentCtrl) {
    cout << "\nEnter Student ID: ";
    string id;
    getline(cin, id);

    if (!studentCtrl.findStudentById(id)) {
        cout << "Student not found.\n";
        return;
    }

    studentCtrl.showStudent(id);
}


void handleListStudentsByGrade(StudentController& studentCtrl) {
    int grade = readInt("\nEnter Grade (1-12): ");
    studentCtrl.listStudentsByGrade(grade);
}


void handleListStudentsByGpa(StudentController& studentCtrl) {
    int grade = readInt("\nEnter Grade (1-12): ");
    studentCtrl.listStudentsByGpa(grade);
}


void handleStudentOperations(StudentController& studentCtrl, CourseController& courseCtrl) {
    cout << "\n\t************ STUDENT BRANCH ************\n\n";

    showProcesses("Student");

    int op = readInt("");
    cout << "\n";

    if (op < 1 || op > 7) {
        cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN\n";
        return;
    }

    if (op == 1) {
        handleAddStudent(studentCtrl);
    }
    else if (op == 2) {
        handleRemoveStudent(studentCtrl);
    }
    else if (op == 3) {
        handleEditStudent(studentCtrl);
    }
    else if (op == 4) {
        handleAssignCoursesToStudent(studentCtrl, courseCtrl);
    }
    else if (op == 5) {
        handleShowStudent(studentCtrl);
    }
    else if (op == 6) {
        handleListStudentsByGrade(studentCtrl);
    }
    else if (op == 7) {
        handleListStudentsByGpa(studentCtrl);
    }
}


void handleAddCourse(CourseController& courseCtrl) {
    Course course;

    cout << "- Please Enter Course Data:\n";

    cout << "Course Name: ";
    string name;
    getline(cin, name);
    course.setName(name);

    int grade = readInt("Grade (1-12): ");
    course.setGrade(grade);

    int hours = readInt("Subject Hours (2-6): ");
    course.setSubjectHours(hours);

    cout << "Specialization: ";
    string spec;
    getline(cin, spec);
    course.setSpecialization(spec);

    cout << "\n";

    string result = courseCtrl.addCourse(grade, course);
    cout << result << "\n";

    if (result.find("cannot") == string::npos &&
        result.find("exists") == string::npos &&
        result.find("Invalid") == string::npos &&
        result.find("Maximum") == string::npos)
    {
        courseCtrl.save();
    }
}


void handleRemoveCourse(CourseController& courseCtrl) {
    cout << "\nEnter Course ID to remove: ";
    string id;
    getline(cin, id);

    Course* c = courseCtrl.findCourseById(id);
    if (!c) {
        cout << "Course not found.\n";
        return;
    }

    cout << "\nYou are about to remove course: " << c->getName() << " (ID: " << c->getId() << ")\n";

    if (!confirmAction("Are you sure?")) {
        cout << "Operation cancelled.\n";
        return;
    }

    string result = courseCtrl.removeCourse(id);
    cout << result << "\n";

    if (result.find("not found") == string::npos && result.find("Cannot") == string::npos) {
        courseCtrl.save();
    }
}


void handleEditCourse(CourseController& courseCtrl) {
    cout << "\nEnter Course ID to edit: ";
    string id;
    getline(cin, id);

    if (!courseCtrl.findCourseById(id)) {
        cout << "Course not found.\n";
        return;
    }

    Course newData;

    cout << "------- Enter NEW DATA : -------\n";

    cout << "Course Name: ";
    string name;
    getline(cin, name);
    newData.setName(name);

    int grade = readInt("Grade (1-12): ");
    newData.setGrade(grade);

    int hours = readInt("Subject Hours (2-6): ");
    newData.setSubjectHours(hours);

    cout << "Specialization: ";
    string spec;
    getline(cin, spec);
    newData.setSpecialization(spec);

    cout << "\n";

    string result = courseCtrl.editCourse(id, newData);
    cout << result << "\n";

    if (result.find("not found") == string::npos && result.find("cannot") == string::npos) {
        courseCtrl.save();
    }
}


void handleReplaceTeacherInCourse(TeacherController& teacherCtrl, CourseController& courseCtrl) {
    cout << "\nEnter Course ID: ";
    string courseId;
    getline(cin, courseId);

    if (!courseCtrl.findCourseById(courseId)) {
        cout << "Course not found.\n";
        return;
    }

    cout << "Enter OLD Teacher ID: ";
    string oldId;
    getline(cin, oldId);

    cout << "Enter NEW Teacher ID: ";
    string newId;
    getline(cin, newId);

    string result = teacherCtrl.replaceTeacherInCourse(courseId, oldId, newId);
    cout << "\n" << result << "\n";

    if (result.find("successfully") != string::npos) {
        teacherCtrl.save();
    }
}


void handleShowCourseStudents(CourseController& courseCtrl) {
    cout << "\nEnter Course ID: ";
    string id;
    getline(cin, id);

    if (!courseCtrl.findCourseById(id)) {
        cout << "Course not found.\n";
        return;
    }

    courseCtrl.showCourseStudents(id);
}


void handleShowCourse(CourseController& courseCtrl) {
    cout << "\nEnter Course ID: ";
    string id;
    getline(cin, id);

    if (!courseCtrl.findCourseById(id)) {
        cout << "Course not found.\n";
        return;
    }

    courseCtrl.showCourse(id);
}


void handleShowCourseStudentsByTeacher(CourseController& courseCtrl) {
    cout << "\nEnter Course ID: ";
    string id;
    getline(cin, id);

    if (!courseCtrl.findCourseById(id)) {
        cout << "Course not found.\n";
        return;
    }

    courseCtrl.showCourseStudentsByTeacher(id);
}


void handleListCoursesByGrade(CourseController& courseCtrl) {
    int grade = readInt("\nEnter Grade (1-12): ");
    courseCtrl.listCoursesByGrade(grade);
}


void handleCourseOperations(CourseController& courseCtrl, TeacherController& teacherCtrl) {
    cout << "\n\t************ COURSE BRANCH ************\n\n";

    showProcesses("Course");

    int op = readInt("");
    cout << "\n";

    if (op < 1 || op > 8) {
        cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN\n";
        return;
    }

    if (op == 1) {
        handleAddCourse(courseCtrl);
    }
    else if (op == 2) {
        handleRemoveCourse(courseCtrl);
    }
    else if (op == 3) {
        handleEditCourse(courseCtrl);
    }
    else if (op == 4) {
        handleShowCourseStudents(courseCtrl);
    }
    else if (op == 5) {
        handleShowCourse(courseCtrl);
    }
    else if (op == 6) {
        handleShowCourseStudentsByTeacher(courseCtrl);
    }
    else if (op == 7) {
        handleReplaceTeacherInCourse(teacherCtrl, courseCtrl);
    }
    else if (op == 8) {
        handleListCoursesByGrade(courseCtrl);
    }
}

void handleAddTeacher(TeacherController& teacherCtrl) {
    Teacher teacher;

    cout << "- Please Enter Teacher Data:\n";

    cout << "Teacher Name: ";
    string name;
    getline(cin, name);
    teacher.setName(name);

    cout << "Teacher National Number: ";
    string nn;
    getline(cin, nn);
    teacher.setNationalNumber(nn);

    int age = readInt("Teacher Age: ");
    teacher.setAge(age);

    int ey = readInt("Teacher experience years: ");
    teacher.setExperienceYears(ey);

    cout << "Teacher Specialization: ";
    string spec;
    getline(cin, spec);
    teacher.setSpecialization(spec);

    int grade = readInt("Main Grade (1-12): ");
    teacher.setGrade(grade);

    double sal = readDouble("Monthly Salary: ");
    teacher.setMonthlySalary(sal);

    cout << "\n";

    string result = teacherCtrl.addTeacher(grade, teacher);
    cout << result << "\n";

    if (result.find("cannot") == string::npos &&
        result.find("exists") == string::npos &&
        result.find("Invalid") == string::npos &&
        result.find("Maximum") == string::npos)
    {
        teacherCtrl.save();
    }
}


void handleRemoveTeacher(TeacherController& teacherCtrl) {
    cout << "\nEnter Teacher ID to remove: ";
    string id;
    getline(cin, id);

    Teacher* t = teacherCtrl.findTeacherById(id);
    if (!t) {
        cout << "Teacher not found.\n";
        return;
    }

    cout << "\nYou are about to remove teacher: " << t->getName() << " (ID: " << t->getId() << ")\n";
    cout << "This will unassign the teacher from any courses that currently have no students enrolled.\n";

    if (!confirmAction("Are you sure?")) {
        cout << "Operation cancelled.\n";
        return;
    }

    string result = teacherCtrl.removeTeacher(id);
    cout << result << "\n";

    if (result.find("not found") == string::npos && result.find("Cannot") == string::npos) {
        teacherCtrl.save();
    }
}


void handleEditTeacher(TeacherController& teacherCtrl) {
    cout << "\nEnter Teacher Id to edit: ";
    string id;
    getline(cin, id);

    if (!teacherCtrl.findTeacherById(id)) {
        cout << "Teacher not found.\n";
        return;
    }

    Teacher newData;

    cout << "------- Enter NEW DATA : -------\n";

    cout << "Teacher Name: ";
    string name;
    getline(cin, name);
    newData.setName(name);

    int age = readInt("Teacher Age: ");
    newData.setAge(age);

    int ey = readInt("Teacher experience years: ");
    newData.setExperienceYears(ey);

    int grade = readInt("Teacher Main Grade (1-12): ");
    newData.setGrade(grade);

    double sal = readDouble("Teacher monthly salary: ");
    newData.setMonthlySalary(sal);

    cout << "Teacher Specialization: ";
    string spec;
    getline(cin, spec);
    newData.setSpecialization(spec);

    cout << "\n";

    string result = teacherCtrl.editTeacher(id, newData);
    cout << result << "\n";

    if (result.find("not found") == string::npos && result.find("cannot") == string::npos) {
        teacherCtrl.save();
    }
}


void handleAssignCoursesToTeacher(TeacherController& teacherCtrl) {
    cout << "\nEnter Teacher ID to assign courses: ";
    string teacherId;
    getline(cin, teacherId);

    int numCourses = readInt("How many courses to assign (1-" + to_string(SchoolConstants::MAX_COURSES_PER_TEACHER) + ")? ");
    if (numCourses < 1 || numCourses > SchoolConstants::MAX_COURSES_PER_TEACHER) {
        cout << "Invalid number. Must be between 1 and " << SchoolConstants::MAX_COURSES_PER_TEACHER << ".\n";
        return;
    }

    vector<string> courseIds;

    for (int i = 0; i < numCourses; i++) {
        cout << "Enter Course ID #" << (i + 1) << ": ";
        string cid;
        getline(cin, cid);
        courseIds.push_back(cid);
    }

    string result = teacherCtrl.assignCoursesToTeacher(teacherId, courseIds);
    cout << result << "\n";

    if (result.find("failed") == string::npos &&
        result.find("not found") == string::npos &&
        result.find("cannot") == string::npos)
    {
        teacherCtrl.save();
    }
}


void handleShowTeacher(TeacherController& teacherCtrl) {
    cout << "\nEnter Teacher ID: ";
    string id;
    getline(cin, id);

    if (!teacherCtrl.findTeacherById(id)) {
        cout << "Teacher not found.\n";
        return;
    }

    teacherCtrl.showTeacher(id);
}


void handleUnassignCourseFromTeacher(TeacherController& teacherCtrl) {
    cout << "\nEnter Teacher ID: ";
    string teacherId;
    getline(cin, teacherId);

    cout << "Enter Course ID to unassign: ";
    string courseId;
    getline(cin, courseId);

    string result = teacherCtrl.unassignCourseFromTeacher(teacherId, courseId);
    cout << result << "\n";

    if (result.find("successfully") != string::npos) {
        teacherCtrl.save();
    }
}


void handleListTeachersByGrade(TeacherController& teacherCtrl) {
    int grade = readInt("\nEnter Grade (1-12): ");
    teacherCtrl.listTeachersByGrade(grade);
}


void handleTeacherOperations(TeacherController& teacherCtrl, CourseController& courseCtrl) {
    cout << "\n\t************ TEACHER BRANCH ************\n";

    showProcesses("Teacher");

    int op = readInt("");
    cout << "\n";

    if (op < 1 || op > 7) {
        cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN\n";
        return;
    }

    if (op == 1) {
        handleAddTeacher(teacherCtrl);
    }
    else if (op == 2) {
        handleRemoveTeacher(teacherCtrl);
    }
    else if (op == 3) {
        handleEditTeacher(teacherCtrl);
    }
    else if (op == 4) {
        handleAssignCoursesToTeacher(teacherCtrl);
    }
    else if (op == 5) {
        handleShowTeacher(teacherCtrl);
    }
    else if (op == 6) {
        handleUnassignCourseFromTeacher(teacherCtrl);
    }
    else if (op == 7) {
        handleListTeachersByGrade(teacherCtrl);
    }
}
