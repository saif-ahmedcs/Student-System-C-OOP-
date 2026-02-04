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



////////////////// CourseServiceImpl \\\\\\\\\\\\\\\

CourseServiceImpl::CourseServiceImpl(CourseRepositoryImpl &repo) : courseRepository(repo) {}

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


bool CourseServiceImpl::validateCoursesLimit(int grade) {
    Stage stage = getStageFromGrade(grade);
    return courseRepository.getCoursesInGrade(grade).size() < courseRepository.getMaxCoursesForStage(stage);
}


// Add course with validation
string CourseServiceImpl::addCourse(int grade, Course &course) {
    string errorMessages = "";

    if (!validateCourseName(course.getName()))
        errorMessages += "- Invalid course name.\n";

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
     return courseRepository.addCourse(grade, course);
}


string CourseServiceImpl::editCourse(const string& id, const Course& newData){


    string errorMessages = "";

    if (!validateCourseName(newData.getName()))
        errorMessages += "- Invalid course name.\n";

    if (!validateGrade(newData.getGrade()))
        errorMessages += "- Invalid academic year.\n";

    if (!validateSubjectHours(newData.getSubjectHours()))
        errorMessages += "- Subject hours must be between 2 and 6.\n";

    if (!validateGrade(newData.getGrade()))
        errorMessages += "- Invalid grade. Must be between 1 and 12.\n";

    if (!validateCoursesLimit(newData.getGrade()))
        errorMessages += "- Maximum number of courses reached for this grade.\n";

    if (!errorMessages.empty()) {
        return "Course cannot be added due to the following issues:\n" + errorMessages;
    }

    // Everything valid, add to repository
     return courseRepository.editCourse(id, newData);

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
    if (!student) {
        cout << "Student not found.\n";
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
