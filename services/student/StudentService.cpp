#include "StudentService.h"
using namespace std;

StudentServiceImpl::StudentServiceImpl(StudentRepository& studentRepo, CourseRepository& courseRepo, StudentValidator& validator)
    : studentRepository(studentRepo), courseRepository(courseRepo), studentValidator(validator) {}

Student* StudentServiceImpl::findStudentByNationalNumber(const string& nationalNumber) {
    return studentRepository.findStudentByNationalNumber(nationalNumber);
}

Student* StudentServiceImpl::findStudentById(const string& id) {
    return studentRepository.findStudentById(id);
}

int StudentServiceImpl::getMaxStudentsForGrade(int grade) const {
    return studentRepository.getMaxStudentsForGrade(grade);
}

string StudentServiceImpl::addStudent(int grade, Student& student) {
    if (studentRepository.findStudentByNationalNumber(student.getNationalNumber())) {
        return "Student already exists.";
    }

    string errors;

    if (!studentValidator.validateName(student.getName())) {
        errors += "- Name cannot be empty.\n";
    }

    if (!studentValidator.validateNationalNumber(student.getNationalNumber())) {
        errors += "- National number must be 14 characters (make sure your input is DIGITS ONLY).\n";
    }

    if (!studentValidator.validateGrade(grade)) {
        errors += "- Grade must be between 1 and 12 (make sure your input is DIGITS ONLY).\n";
    }

    if (!studentValidator.validateAge(student.getAge(), grade)) {
        errors += "- Student age does not match the expected range for this grade (make sure your input is DIGITS ONLY).\n";
    }

    if (!studentValidator.validatePhoneNumber(student.getPhoneNumber())) {
        errors += "- Phone number must be 10-12 characters (make sure your input is DIGITS ONLY)\n";
    }

    if (!errors.empty()) {
        return "Student registration failed:\n" + errors;
    }

    if (studentRepository.getStudentsInGrade(grade) >= studentRepository.getMaxStudentsForGrade(grade)) {
        return "Grade " + to_string(grade) + " has reached its maximum capacity.";
    }

    return studentRepository.addStudent(grade, student);
}

string StudentServiceImpl::editStudent(const string& id, const Student& newData) {
    Student* student = studentRepository.findStudentById(id);
    if (!student) {
        return "Student not found.";
    }

    string errors;

    if (!studentValidator.validateName(newData.getName())) {
        errors += "- Name cannot be empty.\n";
    }

    if (!studentValidator.validateAge(newData.getAge(), newData.getGrade())) {
        errors += "- Age does not match the expected range for this grade (make sure your input is DIGITS ONLY).\n";
    }

    if (!studentValidator.validatePhoneNumber(newData.getPhoneNumber())) {
        errors += "- Phone number must be 10-12 characters (make sure your input is DIGITS ONLY)\n";
    }

    if (!studentValidator.validateGpa((float)newData.getGpa())) {
        errors += "- GPA must be between 0.0 and 4.0 (make sure your input is DIGITS ONLY)\n";
    }

    if (!studentValidator.validateGrade(newData.getGrade())) {
        errors += "- Grade must be between 1 and 12 characters (make sure your input is DIGITS ONLY)\n";
    }

    if (!errors.empty()) {
        return "Student cannot be updated:\n" + errors;
    }

    if (student->getGrade() != newData.getGrade()) {
        if (studentRepository.getStudentsInGrade(newData.getGrade()) >= studentRepository.getMaxStudentsForGrade(newData.getGrade())) {
            return "Grade " + to_string(newData.getGrade()) + " has reached its maximum capacity.";
        }
        const vector<StudentCourse> courses = student->getAssignedCourses();
        for (int i = 0; i < (int)courses.size(); i++) {
            courseRepository.removeStudentFromCourse(id, courses[i].courseId);
        }
        studentRepository.clearStudentCourses(id);
    }

    return studentRepository.editStudent(id, newData);
}

string StudentServiceImpl::assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) {
    if (courseIds.empty()) {
        return "No courses provided.";
    }

    if (courseIds.size() != teacherNames.size()) {
        return "Number of courses and teacher names must match.";
    }

    Student* student = studentRepository.findStudentById(studentId);
    if (!student) {
        return "Student not found.";
    }

    int studentGrade = student->getGrade();
    int currentCourses = student->getNumberOfAssignedCourses();
    int requiredCourses = courseRepository.getMaxCoursesForGrade(studentGrade);

    if (requiredCourses == 0) {
        return "Invalid grade.";
    }

    if (currentCourses >= requiredCourses) {
        return "Student already has all required courses (" + to_string(requiredCourses) + ").";
    }

    if (currentCourses + (int)courseIds.size() > requiredCourses) {
        int remaining = requiredCourses - currentCourses;
        return "Cannot assign " + to_string(courseIds.size()) + " courses. Student has " + to_string(currentCourses) + "/" + to_string(requiredCourses) +
               ". Only " + to_string(remaining) + " more needed.";
    }

    int maxStudentsPerCourse = studentRepository.getMaxStudentsForGrade(studentGrade);
    string errors;
    vector<string> validatedIds;
    vector<string> validatedTeachers;

    for (int i = 0; i < (int)courseIds.size(); i++) {
        const string& cid = courseIds[i];
        const string& teacherName = teacherNames[i];
        Course* c = courseRepository.findCourseById(cid);

        if (!c) {
            errors += "- Course " + cid + " not found.\n";
            continue;
        }

        if (c->getGrade() != studentGrade) {
            errors += "- Course " + cid + " is not in the student's grade.\n";
            continue;
        }

        if (student->isCourseAssigned(cid)) {
            errors += "- Course " + cid + " is already assigned to this student.\n";
            continue;
        }

        if (c->getTeacherNames().empty()) {
            errors += "- Course " + cid + " has no teachers assigned yet.\n";
            continue;
        }

        if (teacherName.empty()) {
            errors += "- No teacher selected for course " + cid + ".\n";
            continue;
        }

        bool teacherBelongsToCourse = false;
        const vector<string>& teachers = c->getTeacherNames();

        for (int j = 0; j < (int)teachers.size(); j++) {
            if (teachers[j] == teacherName) {
                teacherBelongsToCourse = true;
                break;
            }
        }

        if (!teacherBelongsToCourse) {
            errors += "- Teacher '" + teacherName + "' is not assigned to course " + cid + ".\n";
            continue;
        }

        if (c->getNumberOfAssignedStudents() >= maxStudentsPerCourse) {
            errors += "- Course " + cid + " has reached maximum student capacity.\n";
            continue;
        }

        validatedIds.push_back(cid);
        validatedTeachers.push_back(teacherName);
    }

    if (validatedIds.empty()) {
        return "Assignment failed:\n" + errors;
    }

    for (int i = 0; i < (int)validatedIds.size(); i++) {
        courseRepository.assignStudentToCourse(studentId, validatedIds[i]);
    }

    string result = studentRepository.assignCoursesToStudent(studentId, validatedIds, validatedTeachers);

    if (!errors.empty()) {
        return "Partially assigned. Failed courses:\n" + errors;
    }

    return result;
}

string StudentServiceImpl::removeStudent(const string& id) {
    Student* student = studentRepository.findStudentById(id);

    if (!student) {
        return "Student not found.";
    }

    const vector<StudentCourse>& courses = student->getAssignedCourses();

    for (int i = 0; i < (int)courses.size(); i++) {
        courseRepository.removeStudentFromCourse(id, courses[i].courseId);
    }

    return studentRepository.removeStudent(id);
}
