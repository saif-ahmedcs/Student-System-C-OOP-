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
    return teacherRepository.getTeachersInGrade(grade) < teacherRepository.getMaxTeachesForGrade(grade);
}

Teacher* TeacherServiceImpl::findTeacherByNationalNumber(const string& nationalNumber){
  return teacherRepository.findTeacherByNationalNumber(nationalNumber);
}

Teacher* TeacherServiceImpl::findTeacherById(const string& id){
  return teacherRepository.findTeacherById(id);
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

        for (int i = 0; i < courses.size(); i++) {
            cout << courses[i];
        if (i != courses.size() - 1){
            cout << ", ";
        }
        }
        cout << endl;
        cout << "Monthly Salary: $" << teacher->getMonthlySalary() << endl;
        cout << "--------------------------\n";
        }


string TeacherServiceImpl::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds) {

    if (courseIds.empty() || courseIds.size() > 3)
        return "Teacher must be assigned between 1 and 3 courses.";

    Teacher* teacher = teacherRepository.findTeacherById(teacherId);
    if (!teacher)
        return "Teacher not found.";

    string errors = "";

    for (const string& cid : courseIds) {

        Course* c = courseService.findCourseById(cid);
        if (!c) {
            errors += "- Course " + cid + ": Course does not exist in the system.\n";
            continue;
        }

        if (!courseService.validateCourseTeacherStage(teacherId, c->getGrade())) {
            errors += "- Course " + cid + ": Teacher does not teach the educational stage of this course.\n";
        }

        if (!courseService.validateCourseTeacherSpecialization(
            teacherId, c->getCourseSpecialization())) {
            errors += "- Course " + cid + ": Teacher specialization does not match the course specialization.\n";
        }
    }

    if (!errors.empty())
        return "Course assignment failed due to the following errors:\n" + errors;

    for (const string& cid : courseIds) {
        teacher->addAssignedCourse(cid);
    }

    return "Courses assigned to teacher successfully.";
}

////////////////// CourseServiceImpl \\\\\\\\\\\\\\\

CourseServiceImpl::CourseServiceImpl(
    CourseRepositoryImpl& courseRepo,
    TeacherRepositoryImpl& teacherRepo
)
: courseRepository(courseRepo),
  teacherRepository(teacherRepo)
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

Course* CourseServiceImpl::findCourseById(const string& id){
  return courseRepository.findCourseById(id);
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
    cout << "Course Teacher: " << course->getCourseTeacherName() << endl;
    cout << "Course Teacher ID: " << course->getCourseTeacherId() <<endl;
    cout << "-----------------------------------\n";

}



////////////////// StudentServiceImpl \\\\\\\\\\\\\\\

StudentServiceImpl::StudentServiceImpl(StudentRepositoryImpl &repo) : studentRepository(repo) {}

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
}
