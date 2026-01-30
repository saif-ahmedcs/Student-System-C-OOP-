#include "Service.h"
#include "../repo/Repo.h"

////////////////// TeacherServiceImpl \\\\\\\\\\\\\\\

TeacherServiceImpl::TeacherServiceImpl(TeacherRepositoryImpl &repo) : teacherRepository(repo) {}

bool TeacherServiceImpl::validateTeacherName(const string &name) {
    return !name.empty();
}

bool TeacherServiceImpl::validateTeacherGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

bool TeacherServiceImpl::validateTeachersLimit(int grade) {
    Stage stage = getStageFromGrade(grade);
    return teacherRepository.getTeachersInGrade(grade).size() < teacherRepository.getMaxTeachersForStage(stage);
}


string TeacherServiceImpl::addTeacher(int grade, Teacher &teacher) {

    string errors;

    if (!validateTeacherName(teacher.getName()))
        errors += "- Teacher name cannot be empty.\n";

    if (!validateTeacherGrade(grade))
        errors += "- Grade must be between 1 and 12.\n";

    if (!validateTeachersLimit(grade))
        errors += "- Maximum number of teachers reached for this grade.\n";

    if (!errors.empty())
        return "Teacher cannot be added due to the following issues:\n" + errors;

    return teacherRepository.addTeacher(grade, teacher);
}

////////////////// CourseServiceImpl \\\\\\\\\\\\\\\

CourseServiceImpl::CourseServiceImpl(CourseRepositoryImpl &repo) : courseRepository(repo) {}

// Validation functions
bool CourseServiceImpl::validateCourseName(const string &name) {
    return !name.empty();
}

bool CourseServiceImpl::validateAcademicYear(const string &year) {
    return !year.empty();
}

bool CourseServiceImpl::validateSubjectHours(int hours) {
    return hours >= 2 && hours <= 6;
}

bool CourseServiceImpl::validateGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

// Check if course already exists in the grade
bool CourseServiceImpl::isCourseAlreadyRegistered(int grade, const Course &course) {
    const auto &coursesInGrade = courseRepository.getCoursesInGrade(grade);
    for (const auto &c : coursesInGrade) {
        if (c.getName() == course.getName()) {
            return true;
        }
    }
    return false;
}

bool CourseServiceImpl::validateCoursesLimit(int grade) {
    Stage stage = getStageFromGrade(grade);
    return courseRepository.getCoursesInGrade(grade).size() < courseRepository.getMaxCoursesForStage(stage);
}

// Add course with validation
string CourseServiceImpl::addCourse(int grade, Course &course) {
    string errorMessages;

    if (!validateCourseName(course.getName()))
        errorMessages += "- Invalid course name.\n";

    if (!validateAcademicYear(course.getAcademicYear()))
        errorMessages += "- Invalid academic year.\n";

    if (!validateSubjectHours(course.getSubjectHours()))
        errorMessages += "- Subject hours must be between 2 and 6.\n";

    if (!validateGrade(grade))
        errorMessages += "- Invalid grade. Must be between 1 and 12.\n";

    if (isCourseAlreadyRegistered(grade, course))
        errorMessages += "- Course already exists in this grade.\n";

    if (!validateCoursesLimit(grade))
        errorMessages += "- Maximum number of courses reached for this grade.\n";

    if (!errorMessages.empty()) {
        return "Course cannot be added due to the following issues:\n" + errorMessages;
    }

    // Everything valid, add to repository
     return courseRepository.addCourse(grade, course);
}




////////////////// StudentServiceImpl \\\\\\\\\\\\\\\

StudentServiceImpl::StudentServiceImpl(StudentRepositoryImpl &repo) : studentRepository(repo) {}

bool StudentServiceImpl::validateName(const string &name) {
    return !name.empty();
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

