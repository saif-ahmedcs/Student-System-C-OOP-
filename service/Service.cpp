#include "Service.h"
#include "../SchoolConstants.h"
using namespace std;

// ─────────────────────────────────────────────
//  TeacherServiceImpl
// ─────────────────────────────────────────────
TeacherServiceImpl::TeacherServiceImpl(TeacherRepository& teacherRepo, CourseRepository& courseRepo, TeacherValidator&  validator)
    : teacherRepository(teacherRepo), courseRepository(courseRepo), teacherValidator(validator) {}

int TeacherServiceImpl::getMinAvailableSeatsForStage(Stage stage) const {
    switch (stage) {
        case Stage::Primary:   return SchoolConstants::MIN_AVAILABLE_SEATS_PRIMARY;
        case Stage::Middle:    return SchoolConstants::MIN_AVAILABLE_SEATS_MIDDLE;
        case Stage::Secondary: return SchoolConstants::MIN_AVAILABLE_SEATS_SECONDARY;
    }
    return 0;
}

int TeacherServiceImpl::getMaxStudentsForStage(Stage stage) const {
    switch (stage) {
        case Stage::Primary:   return SchoolConstants::MAX_STUDENTS_IN_PRIMARY;
        case Stage::Middle:    return SchoolConstants::MAX_STUDENTS_IN_MIDDLE;
        case Stage::Secondary: return SchoolConstants::MAX_STUDENTS_IN_SECONDARY;
    }
    return 0;
}

Teacher* TeacherServiceImpl::findTeacherByNationalNumber(const string& nationalNumber) {
    return teacherRepository.findTeacherByNationalNumber(nationalNumber);
}

Teacher* TeacherServiceImpl::findTeacherById(const string& id) {
    return teacherRepository.findTeacherById(id);
}

int TeacherServiceImpl::getMaxTeachersForGrade(int grade) const {
    return teacherRepository.getMaxTeachersForGrade(grade);
}

string TeacherServiceImpl::addTeacher(int grade, Teacher& teacher) {
    // ── duplicate check (business rule) ──────────────────────────────────
    if (teacherRepository.findTeacherByNationalNumber(teacher.getNationalNumber()))
        return "Teacher already exists.";

    // ── input validation ──────────────────────────────────────────────────
    string errors;
    if (!teacherValidator.validateName(teacher.getName()))
        errors += "- Teacher name cannot be empty.\n";
    if (!teacherValidator.validateAge(teacher.getAge()))
        errors += "- Teacher age does not comply with school policy.\n";
    if (!teacherValidator.validateExperienceYears(teacher.getExperienceYears()))
        errors += "- Years of experience do not meet school requirements.\n";
    if (!teacherValidator.validateGrade(grade))
        errors += "- Grade must be between 1 and 12.\n";
    if (!errors.empty())
        return "Teacher cannot be added:\n" + errors;

    // ── capacity check (business rule) ───────────────────────────────────
    if (teacherRepository.getTeachersInGrade(grade) >= teacherRepository.getMaxTeachersForGrade(grade))
        return "Maximum number of teachers reached for this grade.";

    return teacherRepository.addTeacher(grade, teacher);
}

string TeacherServiceImpl::editTeacher(const string& id, const Teacher& newData) {
    if (!teacherRepository.findTeacherById(id))
        return "Teacher not found.";

    string errors;
    if (!teacherValidator.validateName(newData.getName()))
        errors += "- Teacher name cannot be empty.\n";
    if (!teacherValidator.validateAge(newData.getAge()))
        errors += "- Teacher age does not comply with school policy.\n";
    if (!teacherValidator.validateExperienceYears(newData.getExperienceYears()))
        errors += "- Years of experience do not meet school requirements.\n";
    if (!teacherValidator.validateGrade(newData.getGrade()))
        errors += "- Grade must be between 1 and 12.\n";
    if (!errors.empty())
        return "Teacher cannot be updated:\n" + errors;

    return teacherRepository.editTeacher(id, newData);
}

// ─────────────────────────────────────────────────────────────────────────
//  assignCoursesToTeacher
//
//  Business rules enforced here:
//    - Teacher exists
//    - Total courses after assignment does not exceed MAX_COURSES_PER_TEACHER
//    - Each course exists
//    - Each course does not already have MAX_TEACHERS_PER_COURSE
//    - Teacher and course are in the same school stage
//    - Teacher and course specializations match
//    - Course is not already assigned to this teacher
// ─────────────────────────────────────────────────────────────────────────
string TeacherServiceImpl::assignCoursesToTeacher(const string& teacherId,
                                                   const vector<string>& courseIds) {
    if (courseIds.empty() || (int)courseIds.size() > SchoolConstants::MAX_COURSES_PER_TEACHER)
        return "Teacher must be assigned between 1 and " +
               to_string(SchoolConstants::MAX_COURSES_PER_TEACHER) + " courses.";

    Teacher* teacher = teacherRepository.findTeacherById(teacherId);
    if (!teacher) return "Teacher not found.";

    if ((int)(teacher->getAssignedCourses().size() + courseIds.size()) >
              SchoolConstants::MAX_COURSES_PER_TEACHER)
        return "Teacher cannot be assigned more than " +
               to_string(SchoolConstants::MAX_COURSES_PER_TEACHER) + " courses.";

    string errors;
    for (int i = 0; i < (int)courseIds.size(); i++) {
        const string& cid = courseIds[i];
        Course* c = courseRepository.findCourseById(cid);

        if (!c) {
            errors += "- Course " + cid + " not found.\n"; continue;
        }
        if (c->getNumberOfTeachers() >= SchoolConstants::MAX_TEACHERS_PER_COURSE) {
            errors += "- Course " + cid + " already has the maximum number of teachers.\n"; continue;
        }
        if (getStageFromGrade(teacher->getGrade()) != getStageFromGrade(c->getGrade())) {
            errors += "- Course " + cid + " is in a different school stage than the teacher.\n"; continue;
        }
        if (teacher->getSpecialization() != c->getSpecialization()) {
            errors += "- Course " + cid + " specialization does not match the teacher's.\n"; continue;
        }
        if (teacher->isCourseAssigned(cid)) {
            errors += "- Course " + cid + " is already assigned to this teacher.\n"; continue;
        }

        // ── Business rule: sufficient unregistered seats must remain ──────
        {
            Stage courseStage = getStageFromGrade(c->getGrade());
            int maxSeats = getMaxStudentsForStage(courseStage);
            int enrolled = c->getNumberOfAssignedStudents();
            int available = maxSeats - enrolled;
            int required = getMinAvailableSeatsForStage(courseStage);
            if (available < required) {
                errors += "- Course " + cid + " does not have enough available seats (" + to_string(available) + " available, " + to_string(required) + " required).\n";
                continue;
            }
        }
    }
    if (!errors.empty())
        return "Assignment failed:\n" + errors;


    for (int i = 0; i < (int)courseIds.size(); i++){
        courseRepository.assignTeacherToCourse(courseIds[i], teacher->getId(), teacher->getName());
    }

    return teacherRepository.assignCoursesToTeacher(teacherId, courseIds);
}

// ─────────────────────────────────────────────
//  CourseServiceImpl
// ─────────────────────────────────────────────
CourseServiceImpl::CourseServiceImpl(CourseRepository& courseRepo, CourseValidator& validator)
: courseRepository(courseRepo), courseValidator(validator) {}

Course* CourseServiceImpl::findCourseById(const string& id) {
    return courseRepository.findCourseById(id);
}

int CourseServiceImpl::getMaxCoursesForGrade(int grade) const {
    return courseRepository.getMaxCoursesForGrade(grade);
}

string CourseServiceImpl::addCourse(int grade, Course& course) {
    // ── duplicate check (business rule) ──────────────────────────────────
    vector<Course> existing = courseRepository.getCoursesInSchoolVector();
    for (int i = 0; i < (int)existing.size(); i++) {
        if (existing[i].getName() == course.getName() && existing[i].getGrade() == course.getGrade() && existing[i].getSpecialization() == course.getSpecialization())
            return "Course already exists.";
    }

    // ── input validation ──────────────────────────────────────────────────
    string errors;
    if (!courseValidator.validateCourseName(course.getName()))
        errors += "- Invalid course name.\n";
    if (!courseValidator.validateGrade(course.getGrade()))
        errors += "- Invalid grade. Must be between 1 and 12.\n";
    if (!courseValidator.validateSubjectHours(course.getSubjectHours()))
        errors += "- Subject hours must be between 2 and 6.\n";
    if (!errors.empty())
        return "Course cannot be added:\n" + errors;

    // ── capacity check (business rule) ───────────────────────────────────
    if (courseRepository.getNumberOfCoursesInGrade(grade) >= courseRepository.getMaxCoursesForGrade(grade))
        return "Maximum number of courses reached for this grade.";

    return courseRepository.addCourse(grade, course);
}

string CourseServiceImpl::editCourse(const string& id, const Course& newData) {
    if (!courseRepository.findCourseById(id))
        return "Course not found.";

    string errors;
    if (!courseValidator.validateCourseName(newData.getName()))
        errors += "- Invalid course name.\n";
    if (!courseValidator.validateGrade(newData.getGrade()))
        errors += "- Invalid grade. Must be between 1 and 12.\n";
    if (!courseValidator.validateSubjectHours(newData.getSubjectHours()))
        errors += "- Subject hours must be between 2 and 6.\n";
    if (!errors.empty())
        return "Course cannot be updated:\n" + errors;

    return courseRepository.editCourse(id, newData);
}

// ─────────────────────────────────────────────
//  StudentServiceImpl
// ─────────────────────────────────────────────
StudentServiceImpl::StudentServiceImpl(StudentRepository& studentRepo, CourseRepository&  courseRepo, StudentValidator&  validator)
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
    // ── duplicate check (business rule) ──────────────────────────────────
    if (studentRepository.findStudentByNationalNumber(student.getNationalNumber()))
        return "Student already exists.";

    // ── input validation ──────────────────────────────────────────────────
    string errors;
    if (!studentValidator.validateName(student.getName()))
        errors += "- Name cannot be empty.\n";
    if (!studentValidator.validateGrade(grade))
        errors += "- Grade must be between 1 and 12.\n";
    if (!studentValidator.validateAge(student.getAge(), grade))
        errors += "- Student age does not match the expected range for this grade.\n";
    if (!studentValidator.validatePhoneNumber(student.getPhoneNumber()))
        errors += "- Phone number must be 10-12 digits.\n";
    if (!errors.empty())
        return "Student registration failed:\n" + errors;

    // ── capacity check (business rule) ───────────────────────────────────
    if (studentRepository.getStudentsInGrade(grade) >= studentRepository.getMaxStudentsForGrade(grade))
        return "Grade " + to_string(grade) + " has reached its maximum capacity.";

    return studentRepository.addStudent(grade, student);
}

string StudentServiceImpl::editStudent(const string& id, const Student& newData) {
    if (!studentRepository.findStudentById(id))
        return "Student not found.";

    string errors;
    if (!studentValidator.validateName(newData.getName()))
        errors += "- Name cannot be empty.\n";
    if (!studentValidator.validateAge(newData.getAge(), newData.getGrade()))
        errors += "- Age does not match the expected range for this grade.\n";
    if (!studentValidator.validatePhoneNumber(newData.getPhoneNumber()))
        errors += "- Phone number must be 10-12 digits.\n";
    if (!studentValidator.validateGpa((float)newData.getGpa()))
        errors += "- GPA must be between 0.0 and 4.0.\n";
    if (!studentValidator.validateGrade(newData.getGrade()))
        errors += "- Grade must be between 1 and 12.\n";
    if (!errors.empty())
        return "Student cannot be updated:\n" + errors;

    return studentRepository.editStudent(id, newData);
}

// ─────────────────────────────────────────────────────────────────────────
//  assignCoursesToStudent
//
//  Business rules enforced here:
//    - Student exists
//    - Required course count for the grade is not exceeded
//    - Each course exists and matches the student's grade
//    - Course is not already assigned to the student
//    - Selected teacher is actually assigned to the course
//    - Course capacity is not exceeded
// ─────────────────────────────────────────────────────────────────────────
string StudentServiceImpl::assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) {
    if (courseIds.empty())
        return "No courses provided.";

    if (courseIds.size() != teacherNames.size())
        return "Number of courses and teacher names must match.";

    Student* student = studentRepository.findStudentById(studentId);
    if (!student) return "Student not found.";

    int studentGrade = student->getGrade();
    int currentCourses = student->getNumberOfAssignedCourses();
    int requiredCourses = courseRepository.getMaxCoursesForGrade(studentGrade);

    if (requiredCourses == 0)
        return "Invalid grade.";

    if (currentCourses >= requiredCourses)
        return "Student already has all required courses (" + to_string(requiredCourses) + ").";

    if (currentCourses + (int)courseIds.size() > requiredCourses) {
        int remaining = requiredCourses - currentCourses;
        return "Cannot assign " + to_string(courseIds.size()) + " courses. " "Student has " + to_string(currentCourses) + "/" + to_string(requiredCourses) +
               ". Only " + to_string(remaining) + " more needed.";
    }

    int maxStudentsPerCourse = studentRepository.getMaxStudentsForGrade(studentGrade) - 1;
    string errors;
    vector<string> validatedIds;
    vector<string> validatedTeachers;

    for (int i = 0; i < (int)courseIds.size(); i++) {
        const string& cid = courseIds[i];
        const string& teacherName = teacherNames[i];
        Course* c = courseRepository.findCourseById(cid);

        if (!c) {
            errors += "- Course " + cid + " not found.\n"; continue;
        }
        if (c->getGrade() != studentGrade) {
            errors += "- Course " + cid + " is not in the student's grade.\n"; continue;
        }
        if (student->isCourseAssigned(cid)) {
            errors += "- Course " + cid + " is already assigned to this student.\n"; continue;
        }
        if (c->getTeacherNames().empty()) {
            errors += "- Course " + cid + " has no teachers assigned yet.\n"; continue;
        }
        if (teacherName.empty()) {
            errors += "- No teacher selected for course " + cid + ".\n"; continue;
        }

        bool teacherBelongsToCourse = false;
        const vector<string>& teachers = c->getTeacherNames();

        for (int j = 0; j < (int)teachers.size(); j++) {
            if (teachers[j] == teacherName) { teacherBelongsToCourse = true;
             break;
            }
        }

        if (!teacherBelongsToCourse) {
            errors += "- Teacher '" + teacherName + "' is not assigned to course " + cid + ".\n"; continue;
        }
        if (c->getNumberOfAssignedStudents() >= maxStudentsPerCourse) {
            errors += "- Course " + cid + " has reached maximum student capacity.\n"; continue;
        }

        validatedIds.push_back(cid);
        validatedTeachers.push_back(teacherName);
    }

    if (validatedIds.empty())
        return "Assignment failed:\n" + errors;

    // Validation passed — persist both sides of the relationship.
    for (int i = 0; i < (int)validatedIds.size(); i++)
        courseRepository.assignStudentToCourse(studentId, validatedIds[i]);

    string result = studentRepository.assignCoursesToStudent(studentId, validatedIds, validatedTeachers);

    if (!errors.empty())
        return "Partially assigned. Failed courses:\n" + errors;

    return result;
}
