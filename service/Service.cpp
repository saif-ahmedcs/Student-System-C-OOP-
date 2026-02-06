#include "Service.h"
#include "../repo/Repo.h"

////////////////// TeacherServiceImpl \\\\\\\\\\\\\\\

TeacherServiceImpl::TeacherServiceImpl(TeacherRepositoryImpl &repo) : teacherRepository(repo) {}

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
    Stage stage = getStageFromGrade(grade);
    return teacherRepository.getTeachersInGrade(grade).size() < teacherRepository.getMaxTeachersForStage(stage);
}

Teacher* TeacherServiceImpl::findTeacherById(const string& id){
  return teacherRepository.findTeacherById(id);
}

string TeacherServiceImpl::addTeacher(int grade, Teacher &teacher) {

    string errors = "";

    if (!validateTeacherName(teacher.getName()))
        errors += "- Teacher name cannot be empty.\n";

    if (!validateTeacherAge(teacher.getAge()))
        errors += "- The entered age for the teacher does not comply with the school policy.\n";

    if (!validateTeacherExperience(teacher.getTeacherExperienceYears()))
        errors += "- The entered years of experience do not meet the school's requirements.\n";

    if (!validateTeacherGrade(grade))
        errors += "- Grade must be between 1 and 12.\n";

    if (!validateTeachersLimit(grade))
        errors += "- Maximum number of teachers reached for this grade.\n";

    if (!errors.empty())
        return "Teacher cannot be added due to the following issues:\n" + errors;

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

    if (!validateTeachersLimit(newData.getTeacherGrade()))
        errors += "- Maximum number of teachers reached for this grade.\n";

    if (!errors.empty())
        return "Teacher cannot be added due to the following issues:\n" + errors;

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
        cout << "Teacher Assigned Course ID: " <<teacher->getAssignedCourse() << endl;
        cout << "Monthly Salary: $" << teacher->getMonthlySalary() << endl;
        cout << "--------------------------\n";
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

bool CourseServiceImpl::validateCourseExisting(const string &courseId){
    Course* existingCourse = courseRepository.findCourseById(courseId);
        if (!existingCourse) {
            return false;
        }
       return true;
}

bool CourseServiceImpl::validateCourseName(const string &name) {
    return !name.empty();
}

bool CourseServiceImpl::validateSubjectHours(int hours) {
    return hours >= 2 && hours <= 6;
}

bool CourseServiceImpl::validateGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

bool CourseServiceImpl::validateCourseTeacherExists(const string& id){
        Teacher* existingTeacher = teacherRepository.findTeacherById(id);
            if (!existingTeacher) {
                return false;
            }
    return true;
}

bool CourseServiceImpl::validateCourseTeacherGrade(const string& teacherId,int courseGrade){
     Teacher* existingTeacher = teacherRepository.findTeacherById(teacherId);
     if (!existingTeacher){
        return false;
       }
   return existingTeacher->getTeacherGrade() == courseGrade;
}


bool CourseServiceImpl::validateCoursesLimit(int grade) {
    Stage stage = getStageFromGrade(grade);
    return courseRepository.getCoursesInGrade(grade).size() < courseRepository.getMaxCoursesForStage(stage);
}

Course* CourseServiceImpl::findCourseById(const string& id){
  return courseRepository.findCourseById(id);
}


// Add course with validation
string CourseServiceImpl::addCourse(int grade, Course &course) {
    string errorMessages = "";

    if (!validateCourseName(course.getName()))
        errorMessages += "- Invalid course name.\n";

    if (!validateCourseTeacherExists(course.getCourseTeacherId())){
        errorMessages += "- Teacher not found!.\n";
        }

    else if (!validateCourseTeacherGrade(course.getCourseTeacherId(),grade)){
        errorMessages += "- Teacher does not teach the selected grade\n";
    }

    if (!validateGrade(course.getGrade()))
        errorMessages += "- Invalid grade. Must be between 1 and 12.\n";

    if (!validateSubjectHours(course.getSubjectHours()))
        errorMessages += "- Subject hours must be between 2 and 6.\n";

    if (!validateCoursesLimit(grade))
        errorMessages += "- Maximum number of courses reached for this grade.\n";

    if (!errorMessages.empty()) {
        return "Course cannot be added due to the following issues:\n" + errorMessages;
    }

    // Everything valid, add to repository

    Teacher* existingTeacher = teacherRepository.findTeacherById(course.getCourseTeacherId());

    if (existingTeacher) {
        course.setCourseTeacherName(existingTeacher->getName());
        }

    string result = courseRepository.addCourse(grade, course);

    existingTeacher->setAssignedCourse(course.getId());

    return result;
}



string CourseServiceImpl::editCourse(const string& id, const Course& newData){


    string errorMessages = "";

    if (!validateCourseName(newData.getName()))
        errorMessages += "- Invalid course name.\n";

    if (!validateCourseTeacherExists(newData.getCourseTeacherId())){
        errorMessages += "- Teacher not found!.\n";
        }

    else if (!validateCourseTeacherGrade(newData.getCourseTeacherId(),newData.getGrade())){
        errorMessages += "- Teacher does not teach the selected grade\n";
        }

    if (!validateGrade(newData.getGrade()))
        errorMessages += "- Invalid grade.\n";

    if (!validateSubjectHours(newData.getSubjectHours()))
        errorMessages += "- Subject hours must be between 2 and 6.\n";

    if (!errorMessages.empty()) {
        return "Course cannot be added due to the following issues:\n" + errorMessages;
    }

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

Student* StudentServiceImpl::findStudentById(const string& id){
  return studentRepository.findStudentById(id);
}

string StudentServiceImpl::addStudent(int grade, Student &student) {

    string errors = "";

    if (!validateName(student.getName()))
        errors += "- Name cannot be empty.\n";

    if (!validateAge(student.getAge(),grade))
        errors += "- The age entered does not match the expected range for this grade.\n";

    if (!validatePhoneNumber(student.getPhoneNumber()))
        errors += "- Phone number must be 10 --> 12 digits and contain digits only.\n";

    if (!validateGrade(grade))
        errors += "- Grade must be between 1 and 12.\n";

    // If any validation errors exist
    if (!errors.empty())
        return "Invalid student data. Please review the following errors:\n" + errors;

    // All validations passed
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
