#include "StudentService.h"
#include "../../common/ServiceUtils.h"
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

bool StudentServiceImpl::isGradeAtCapacity(int grade) const {
    return ::isGradeAtCapacity(studentRepository.getStudentsInGrade(grade), studentRepository.getMaxStudentsForGrade(grade));
}

string StudentServiceImpl::validateFields(const Student& student, int grade) {
    string errors;
    errors += buildValidationError(studentValidator.validateName(student.getName()), "Name cannot be empty.");
    errors += buildValidationError(studentValidator.validateAge(student.getAge(), grade), "Age does not match the expected range for this grade (make sure your input is DIGITS ONLY).");
    errors += buildValidationError(studentValidator.validatePhoneNumber(student.getPhoneNumber()), "Phone number must be 10-12 characters (make sure your input is DIGITS ONLY)");
    errors += buildValidationError(studentValidator.validateGpa((float)student.getGpa()), "GPA must be between 0.0 and 4.0 (make sure your input is DIGITS ONLY)");
    errors += buildValidationError(studentValidator.validateGrade(grade), "Grade must be between 1 and 12 characters (make sure your input is DIGITS ONLY)");
    return errors;
}

string StudentServiceImpl::validateAddFields(const Student& student, int grade) {
    string errors;
    errors += buildValidationError(studentValidator.validateNationalNumber(student.getNationalNumber()), "National number must be 14 characters (make sure your input is DIGITS ONLY).");
    errors += validateFields(student, grade);
    return errors;
}

string StudentServiceImpl::addStudent(int grade, Student& student) {
    if (studentRepository.findStudentByNationalNumber(student.getNationalNumber())) {
        return "Student already exists.";
    }

    string errors = validateAddFields(student, grade);
    if (!errors.empty()) {
        return "Student registration failed:\n" + errors;
    }

    if (isGradeAtCapacity(grade)) {
        return "Grade " + to_string(grade) + " has reached its maximum capacity.";
    }

    return studentRepository.addStudent(grade, student);
}

string StudentServiceImpl::editStudent(const string& id, const Student& newData) {
    Student* student = studentRepository.findStudentById(id);
    if (!student) {
        return "Student not found.";
    }

    string errors = validateFields(newData, newData.getGrade());
    if (!errors.empty()) {
        return wrapUpdateErrors("Student", errors);
    }

    if (student->getGrade() != newData.getGrade()) {
        if (isGradeAtCapacity(newData.getGrade())) {
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
