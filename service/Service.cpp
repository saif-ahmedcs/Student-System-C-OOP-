#include "Service.h"
#include "../repo/Repo.h"

////////////////// TeacherServiceImpl \\\\\\\\\\\\\\\

TeacherServiceImpl::TeacherServiceImpl(TeacherRepositoryImpl& repo,CourseService& courseSrv) : teacherRepository(repo),courseService(courseSrv)
{}

bool TeacherServiceImpl::validateTeacherName(const string &name) {
    return !name.empty();
}

bool TeacherServiceImpl::validateTeacherAge(int age){
  int minAge = 23;
  int maxAge = 60;
    return age >= minAge && age <= maxAge;
}

bool TeacherServiceImpl::validateTeacherExperience(int ey){
  int requiredExperienceYears = 2;
    return ey >= requiredExperienceYears;
}

bool TeacherServiceImpl::validateTeacherGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

bool TeacherServiceImpl::validateTeachersLimit(int grade) {
    return teacherRepository.getTeachersInGrade(grade) < teacherRepository.getMaxTeachersForGrade(grade);
}

Teacher* TeacherServiceImpl::findTeacherByNationalNumber(const string& nationalNumber){
  return teacherRepository.findTeacherByNationalNumber(nationalNumber);
}

Teacher* TeacherServiceImpl::findTeacherById(const string& id){
  return teacherRepository.findTeacherById(id);
}

int TeacherServiceImpl::getMaxTeachersForGrade(int grade) const {
    return teacherRepository.getMaxTeachersForGrade(grade);
}


bool TeacherServiceImpl::isTeacherAlreadyExists(const string& nationalNumber) {
    return teacherRepository.findTeacherByNationalNumber(nationalNumber) != nullptr;
}


string TeacherServiceImpl::addTeacher(int grade, Teacher &teacher) {

    string errors = "";

if (isTeacherAlreadyExists(teacher.getTeacherNationalNum())) {
        return "Teacher already exists.";
}

if (!validateTeacherName(teacher.getName()))
    errors += "- Teacher name cannot be empty.\n";

if (!validateTeacherAge(teacher.getAge()))
    errors += "- The entered age for the teacher does not comply with the school policy.\n";

if (!validateTeacherExperience(teacher.getTeacherExperienceYears()))
    errors += "- The entered years of experience do not meet the school's requirements.\n";

if (!validateTeacherGrade(grade))
    errors += "- Grade must be between 1 and 12.\n";

if (!errors.empty())
    return "Teacher cannot be added due to invalid input:\n" + errors;

if (!validateTeachersLimit(grade))
    return "- Maximum number of teachers reached for this grade.\n";


    return teacherRepository.addTeacher(grade, teacher);
}

string TeacherServiceImpl::editTeacher(const string& id, const Teacher& newData){


    string errors = "";

if (!validateTeacherName(newData.getName()))
    errors += "- Teacher name cannot be empty.\n";

if (!validateTeacherAge(newData.getAge()))
    errors += "- The entered age for the teacher does not comply with the school policy.\n";

if (!validateTeacherExperience(newData.getTeacherExperienceYears()))
    errors += "- The entered years of experience do not meet the school's requirements.\n";

if (!validateTeacherGrade(newData.getTeacherGrade()))
    errors += "- Grade must be between 1 and 12.\n";

if (!errors.empty())
    return "Teacher cannot be added due to invalid input:\n" + errors;

if (!validateTeachersLimit(newData.getTeacherGrade()))
    return "- Maximum number of teachers reached for this grade.\n";

    // Everything valid, add to repository
     return teacherRepository.editTeacher(id, newData);

}

string TeacherServiceImpl::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds) {
    if (courseIds.empty() || courseIds.size() > 3)
        return "Teacher must be assigned between 1 and 3 courses.";

    Teacher* teacher = teacherRepository.findTeacherById(teacherId);
    if (!teacher)
        return "Teacher not found.";

    if ((teacher->getAssignedCourses().size() + courseIds.size()) > 3)
        return "Teacher cannot be assigned more than 3 courses.";

    string errors = "";

    for (int i = 0; i < courseIds.size(); i++) {
        string cid = courseIds[i];
        Course* c = courseService.findCourseById(cid);

        if (!c) {
            errors += "- Course " + cid + " not found.\n";
        }
        else if (c->getTeacherNames().size() >= 3) {
            errors += "- Course " + cid + " already has 3 teachers.\n";
        }
        else if (!courseService.validateCourseTeacherStage(teacherId, c->getGrade())) {
            errors += "- Course " + cid + " stage mismatch.\n";
        }
        else if (!courseService.validateCourseTeacherSpecialization(teacherId, c->getCourseSpecialization())) {
            errors += "- Course " + cid + " specialization mismatch.\n";
        }
        else if (teacher->isCourseAssigned(cid)) {
            errors += "- Course " + cid + " already assigned.\n";
        }
        else {
            c->addTeacher(teacher);
        }
    }

    if (!errors.empty())
        return "Assignment failed:\n" + errors;

    return teacherRepository.assignCoursesToTeacher(teacherId, courseIds);
}


void TeacherServiceImpl::showTeacher(const string& id){

    Teacher* teacher = teacherRepository.findTeacherById(id);
      if (!teacher){
      cout << "-----------------------\n";
      cout << "Teacher not found.\n";
      cout << "-----------------------\n";
      return;
      }

        cout << "--------------------------\n";
        cout << "Teacher Name: " << teacher->getName() << endl;
        cout << "Age: " << teacher->getAge() << endl;
        cout << "Experience Years: " << teacher->getTeacherExperienceYears() << endl;
        cout << "Subject: " << teacher->getTeacherSubject() << endl;
        cout << "Grade: " << teacher->getTeacherGrade() << endl;
        cout << "Teacher Assigned Courses IDs: ";

        const vector<string>& courses = teacher->getAssignedCourses();
        if (courses.empty()){
            cout <<"NONE";
        }
        else {
        for (int i = 0; i < courses.size(); i++) {
            cout << courses[i];
        if (i != courses.size() - 1){
            cout << ", ";
        }
        }
        }
        cout << endl;
        cout << "Monthly Salary: $" << teacher->getMonthlySalary() << endl;
        cout << "--------------------------\n";
        }


////////////////// CourseServiceImpl \\\\\\\\\\\\\\\

CourseServiceImpl::CourseServiceImpl(
    CourseRepositoryImpl& courseRepo,
    TeacherRepositoryImpl& teacherRepo,
    StudentRepositoryImpl& studentRepo
)
: courseRepository(courseRepo),
  teacherRepository(teacherRepo),
  studentRepository(studentRepo)
{}

// Validation functions

bool CourseServiceImpl::validateCourseName(const string &name) {
    return !name.empty();
}

bool CourseServiceImpl::validateSubjectHours(int hours) {
    return hours >= 2 && hours <= 6;
}

bool CourseServiceImpl::validateGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

bool CourseServiceImpl::validateCourseTeacherStage(const string& teacherId, int courseGrade) {
    Teacher* teacher = teacherRepository.findTeacherById(teacherId);
    if (!teacher){
        return false;
        }

    Stage teacherStage = getStageFromGrade(teacher->getTeacherGrade());
    Stage courseStage = getStageFromGrade(courseGrade);

    return teacherStage == courseStage;
}

bool CourseServiceImpl::validateCourseTeacherSpecialization(const string& teacherId, const string& courseSpecialization) {
    Teacher* teacher = teacherRepository.findTeacherById(teacherId);

    if (!teacher){
        return false;
    }

    return teacher->getTeacherSpecialization() == courseSpecialization;
}

bool CourseServiceImpl::validateCoursesLimit(int grade) {
    int currentCount = courseRepository.getNumberOfCoursesInGrade(grade);
    int maxCount = courseRepository.getMaxCoursesForGrade(grade);
    return currentCount < maxCount;
}

bool CourseServiceImpl::validateCourseStudentsLimit(int grade, int currentStudents) {

    int maxStudentsForGrade = studentRepository.getMaxStudentsForGrade(grade);

    if (currentStudents >= maxStudentsForGrade) {
        return false;
    }

    return true;
}


Course* CourseServiceImpl::findCourseById(const string& id){
  return courseRepository.findCourseById(id);
}

int CourseServiceImpl::getMaxCoursesForGrade(int grade) const {
    return courseRepository.getMaxCoursesForGrade(grade);
}



bool CourseServiceImpl::isCourseAlreadyExists(const string& name, int grade, const string& specialization) {

    vector<Course> courses = courseRepository.getCoursesInSchoolVector();

    for (int i = 0; i < courses.size(); i++) {
        if (courses[i].getName() == name && courses[i].getGrade() == grade && courses[i].getCourseSpecialization() == specialization) {
            return true;
        }
    }
    return false;
}



// Add course with validation
string CourseServiceImpl::addCourse(int grade, Course &course) {
    string errorMessages = "";

    if (isCourseAlreadyExists(course.getName(), course.getGrade(), course.getCourseSpecialization())) {
            return "Course already exists.";
    }

    if (!validateCourseName(course.getName()))
        errorMessages += "- Invalid course name.\n";

    if (!validateGrade(course.getGrade()))
        errorMessages += "- Invalid grade. Must be between 1 and 12.\n";

    if (!validateSubjectHours(course.getSubjectHours()))
        errorMessages += "- Subject hours must be between 2 and 6.\n";

    if (!errorMessages.empty()) {
        return "Course cannot be added due to invalid input:\n" + errorMessages;
    }

    if (!validateCoursesLimit(grade)) {
        return "- Maximum number of courses reached for this grade.\n";
    }

    // Everything valid, add to repository

    return courseRepository.addCourse(grade, course);
}



string CourseServiceImpl::editCourse(const string& id, const Course& newData){


    string errorMessages = "";

     if (!validateCourseName(newData.getName()))
       errorMessages += "- Invalid course name.\n";

     if (!validateGrade(newData.getGrade()))
       errorMessages += "- Invalid grade. Must be between 1 and 12.\n";

     if (!validateSubjectHours(newData.getSubjectHours()))
       errorMessages += "- Subject hours must be between 2 and 6.\n";

     if (!errorMessages.empty())
       return "Course cannot be added due to invalid input:\n" + errorMessages;

     else if (!validateCourseTeacherStage(newData.getCourseTeacherId(), newData.getGrade()))
    errorMessages += "- Teacher does not teach the selected stage.\n";

     if (!errorMessages.empty())
       return "Course cannot be added due to invalid relations:\n" + errorMessages;

     if (!validateCoursesLimit(newData.getGrade()))
       return "- Maximum number of courses reached for this grade.\n";

    // Everything valid, add to repository
     return courseRepository.editCourse(id, newData);
}

void CourseServiceImpl::showCourse(const string& id){

    Course* course = courseRepository.findCourseById(id);
      if (!course){
      cout << "-----------------------\n";
      cout << "Course not found.\n";
      cout << "-----------------------\n";
      return;
      }

    cout << "-----------------------------------\n";
    cout << "Course Name: " << course->getName() << endl;
    cout << "Course ID: " << course->getId() << endl;
    cout << "Course Grade: " << course->getGrade() << endl;
    cout << "Subject Hours: " << course->getSubjectHours() << endl;

    const vector<string>& teachers = course->getTeacherNames();
    if (teachers.empty()) cout << "Teachers: NONE\n";
    else {
        cout << "Teachers: ";
        for (int i = 0; i < teachers.size(); i++) {
            cout << teachers[i];
            if (i != teachers.size() - 1)
            cout << ", ";
        }
        cout << endl;
    }
    cout << "-----------------------------------\n";

}

void CourseServiceImpl::showCourseStudents(const string& courseId) {
    Course* course = courseRepository.findCourseById(courseId);

    if (!course) {
        cout << "-----------------------\n";
        cout << "Course not found.\n";
        cout << "-----------------------\n";
        return;
    }

    const vector<string>& assignedStudents = course->getAssignedStudents();

    cout << "\n======================================\n";
    cout << "Course: " << course->getName() << " (ID: " << course->getId() << ")\n";
    cout << "Grade: " << course->getGrade() << endl;
    cout << "======================================\n";
    cout << "Total Assigned Students: " << assignedStudents.size() << endl;
    cout << "--------------------------------------\n";

    if (assignedStudents.empty()) {
        cout << "No students assigned in this course yet.\n";
    } else {
        cout << "Assigned Students:\n\n";
        for (int i = 0; i < assignedStudents.size(); i++) {
            Student* student = studentRepository.findStudentById(assignedStudents[i]);
            if (student) {
                cout << (i + 1) << ". " << student->getName()
                     << " (ID: " << student->getId() << ")"
                     << " - Grade: " << student->getSchoolYear() << endl;
            }
        }
    }
    cout << "======================================\n\n";
}



////////////////// StudentServiceImpl \\\\\\\\\\\\\\\

StudentServiceImpl::StudentServiceImpl(StudentRepositoryImpl &repo, CourseRepositoryImpl &courseRepo)
    : studentRepository(repo), courseRepository(courseRepo) {}

bool StudentServiceImpl::validateName(const string &name) {
    return !name.empty();
}

bool StudentServiceImpl::validateAge(int age, int grade) {
    int expectedAge = grade + 5;
    int minAge = expectedAge - 1;
    int maxAge = expectedAge + 1;

    return age >= minAge && age <= maxAge;
}


bool StudentServiceImpl::validatePhoneNumber(const string &phone) {
    if (phone.length() < 10 || phone.length() > 12)
        return false;

    for (char c : phone) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool StudentServiceImpl::validateGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

bool StudentServiceImpl::validateNewGpa(float gpa){
  return gpa>=0.0 && gpa<=4.0;
}

Student* StudentServiceImpl::findStudentByNationalNumber(const string& nationalNumber){
  return studentRepository.findStudentByNationalNumber(nationalNumber);
}

Student* StudentServiceImpl::findStudentById(const string& id){
  return studentRepository.findStudentById(id);
}

int StudentServiceImpl::getMaxStudentsForGrade(int grade) const {
    return studentRepository.getMaxStudentsForGrade(grade);
}

bool StudentServiceImpl::isStudentAlreadyExists(const string& nationalNumber) {
    return studentRepository.findStudentByNationalNumber(nationalNumber) != nullptr;
}

bool StudentServiceImpl::validateStudentsLimit(int grade) {
    int currentCount = studentRepository.getStudentsInGrade(grade);
    int maxCount = studentRepository.getMaxStudentsForGrade(grade);
    return currentCount < maxCount;
}


string StudentServiceImpl::addStudent(int grade, Student& student) {

    string errors;

    if (isStudentAlreadyExists(student.getStudentNationalNum())) {
        return "student already exists.";
    }

    if (!validateName(student.getName()))
        errors += "- Name cannot be empty.\n";

    if (!validateGrade(grade))
        errors += "- Grade must be between 1 and 12.\n";

    if (!validateAge(student.getAge(), grade))
        errors += "- Student age does not match the expected range for the selected grade.\n";

    if (!validatePhoneNumber(student.getPhoneNumber()))
        errors += "- Phone number must contain only digits and be 10 to 12 characters long.\n";

    if (validateGrade(grade) && !validateStudentsLimit(grade))
        errors += "- Cannot add student: Grade " + to_string(grade) + " has reached its maximum capacity.\n";

    if (!errors.empty()) {
        return "Student registration failed due to the following issues:\n" + errors;
    }

    return studentRepository.addStudent(grade, student);
}

string StudentServiceImpl::editStudent(const string& id, const Student& newData) {
    string errors = "";

    if (!validateName(newData.getName()))
        errors += "- Name cannot be empty.\n";

    if (!validateAge(newData.getAge(),newData.getSchoolYear()))
        errors += "- The age entered does not match the expected range for this grade.\n";

    if (!validatePhoneNumber(newData.getPhoneNumber()))
        errors += "- Phone number must be 10-12 digits.\n";

    if (!validateNewGpa(newData.getGpa()))
        errors += "- Grade must be between 0.0 and 4.0.\n";

    if (!validateGrade(newData.getSchoolYear()))
        errors += "- Grade must be between 1 and 12.\n";

    if (!errors.empty())
        return "Invalid data. Please fix the following errors:\n" + errors;

    return studentRepository.editStudent(id, newData);
}

string StudentServiceImpl::assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) {
    Student* student = studentRepository.findStudentById(studentId);

    if (!student)
        return "Student not found.";

    if (courseIds.empty())
        return "No courses provided.";

    if (courseIds.size() != teacherNames.size())
        return "Number of courses and teachers must match.";

    int studentGrade = student->getSchoolYear();
    int currentCourses = student->getNumberOfAssignedCourses();
    int requiredCourses = courseRepository.getMaxCoursesForGrade(studentGrade);

    if (requiredCourses == 0)
        return "Invalid grade.";

    if (currentCourses >= requiredCourses)
        return "Student already has all required courses (" + to_string(requiredCourses) + ").";

    if (currentCourses + courseIds.size() > requiredCourses) {
        int remaining = requiredCourses - currentCourses;
        return "Cannot assign " + to_string(courseIds.size()) +
               " courses. Student has " + to_string(currentCourses) + "/" + to_string(requiredCourses) + " courses. Only " +
               to_string(remaining) + " more needed.";
    }

    string errors = "";

    for (int i = 0; i < courseIds.size(); i++) {

        string cid = courseIds[i];
        string teacherName = teacherNames[i];

        Course* c = courseRepository.findCourseById(cid);

        if (!c) {
            errors += "- Course " + cid + " not found.\n";
            continue;
        }

        if (c->getGrade() != studentGrade) {
            errors += "- Course " + cid + " grade mismatch.\n";
            continue;
        }

        if (student->isCourseAssigned(cid)) {
            errors += "- Course " + cid + " already assigned.\n";
            continue;
        }

        vector<string> courseTeachers = c->getTeacherNames();

        if (courseTeachers.empty()) {
            errors += "- Course " + cid + " has no teachers assigned.\n";
            continue;
        }

        if (teacherName.empty()) {
            errors += "- No teacher selected for course " + cid + ".\n";
            continue;
        }

        bool teacherFound = false;


        for (int j = 0; j < courseTeachers.size(); j++) {
            if (courseTeachers[j] == teacherName) {
                teacherFound = true;
                break;
            }
        }

        if (!teacherFound) {
            errors += "- Teacher " + teacherName + " not assigned to course " + cid + ".\n";
            continue;
        }

        int currentStudents = c->getNumberOfAssignedStudents();
        int maxStudentsForGrade = studentRepository.getMaxStudentsForGrade(studentGrade);

        if (currentStudents >= maxStudentsForGrade) {
            errors += "- Course " + cid + " reached maximum students limit.\n";
            continue;
        }
    }




    if (!errors.empty())
        return "Assignment failed:\n" + errors;


    return studentRepository.assignCoursesToStudent(studentId, courseIds, teacherNames);
}



void StudentServiceImpl::showStudent(const string& studentId) {

    Student* student = studentRepository.findStudentById(studentId);
      if (!student){
      cout << "-----------------------\n";
      cout << "Student not found.\n";
      cout << "-----------------------\n";
      return;
      }

    cout << "-----------------------------------\n";
    cout << "Student Name: " << student->getName() << endl;
    cout << "Student ID: " << student->getId() << endl;
    cout << "School Grade: " << student->getSchoolYear() << endl;
    cout << "Age: " << student->getAge() << endl;
    cout << "Phone Number: " << student->getPhoneNumber() << endl;
    cout << "GPA: " << student->getGpa() << endl;

    vector<StudentCourse> courses = student->getAssignedCourses();
    int courseCount = courses.size();

    cout << "Registered Courses: " << courseCount << endl;

    if (courseCount == 0) {
        cout << "-----------------------------------\n";
        cout << "No courses registered yet.\n";
        cout << "-----------------------------------\n";

    } else {
        for (int i = 0; i < courseCount; i++) {
            Course* course = courseRepository.findCourseById(courses[i].courseId);
            if (course) {
                cout << "Course #" << (i + 1) << ": " << course->getName() << endl;
                cout << "Teacher Name: " << courses[i].teacherName << endl;
                cout << endl;
            }
        }
    }

    int studentGrade = student->getSchoolYear();
    int requiredCourses = courseRepository.getMaxCoursesForGrade(studentGrade);
    int remaining = requiredCourses - courseCount;

    cout << "-----------------------------------\n";
    cout << "Required courses for this grade: " << requiredCourses << endl;
    cout << "Courses still needed: " << remaining << endl;
    cout << "-----------------------------------\n";
}


