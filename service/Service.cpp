#include "Service.h"
#include "../SchoolConstants.h"
using namespace std;

// ─────────────────────────────────────────────
//  TeacherServiceImpl
// ─────────────────────────────────────────────
TeacherServiceImpl::TeacherServiceImpl(TeacherRepository& teacherRepo, CourseRepository& courseRepo, StudentRepository& studentRepo, TeacherValidator&  validator)
    : teacherRepository(teacherRepo), courseRepository(courseRepo), studentRepository(studentRepo), teacherValidator(validator) {}

int TeacherServiceImpl::getMinAvailableSeatsForStage(Stage stage) const {
    switch (stage) {
        case Stage::Primary:
            return SchoolConstants::MIN_AVAILABLE_SEATS_PRIMARY;
        case Stage::Middle:
            return SchoolConstants::MIN_AVAILABLE_SEATS_MIDDLE;
        case Stage::Secondary:
            return SchoolConstants::MIN_AVAILABLE_SEATS_SECONDARY;
    }
    return 0;
}

int TeacherServiceImpl::getMaxStudentsForStage(Stage stage) const {
    switch (stage) {
        case Stage::Primary:
            return SchoolConstants::MAX_STUDENTS_IN_PRIMARY;
        case Stage::Middle:
            return SchoolConstants::MAX_STUDENTS_IN_MIDDLE;
        case Stage::Secondary:
            return SchoolConstants::MAX_STUDENTS_IN_SECONDARY;
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

    if (teacherRepository.findTeacherByNationalNumber(teacher.getNationalNumber()))
        return "Teacher already exists.";

    string errors;
    if (!teacherValidator.validateName(teacher.getName()))
        errors += "- Teacher name cannot be empty.\n";
    if (!teacherValidator.validateNationalNumber(teacher.getNationalNumber()))
        errors += "- National number must be 14 characters (make sure your input is DIGITS ONLY).\n";
    if (!teacherValidator.validateAge(teacher.getAge()))
        errors += "- Teacher age does not comply with school policy (make sure your input is DIGITS ONLY)\n";
    if (!teacherValidator.validateExperienceYears(teacher.getExperienceYears()))
        errors += "- Years of experience do not meet school requirements (make sure your input is DIGITS ONLY)\n";
    if (!teacherValidator.validateSpecialization(teacher.getSpecialization()))
        errors += "- Specialization cannot be empty.\n";
    if (!teacherValidator.validateMonthlySalary(teacher.getMonthlySalary()))
        errors += "- Monthly salary must be at least 7000 (make sure your input is DIGITS ONLY).\n";
    if (!teacherValidator.validateGrade(grade))
        errors += "- Grade must be between 1 and 12 (make sure your input is DIGITS ONLY)\n";
    if (!errors.empty())
        return "Teacher cannot be added:\n" + errors;

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
        errors += "- Teacher age does not comply with school policy (make sure your input is DIGITS ONLY).\n";
    if (!teacherValidator.validateExperienceYears(newData.getExperienceYears()))
        errors += "- Years of experience do not meet school requirements (make sure your input is DIGITS ONLY).\n";
    if (!teacherValidator.validateSpecialization(newData.getSpecialization()))
        errors += "- Specialization cannot be empty.\n";
    if (!teacherValidator.validateMonthlySalary(newData.getMonthlySalary()))
        errors += "- Monthly salary must be at least 7000 (make sure your input is DIGITS ONLY).\n";
    if (!teacherValidator.validateGrade(newData.getGrade()))
        errors += "- Grade must be between 1 and 12 (make sure your input is DIGITS ONLY).\n";
    if (!errors.empty())
        return "Teacher cannot be updated:\n" + errors;

    return teacherRepository.editTeacher(id, newData);
}

string TeacherServiceImpl::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds) {
    if (courseIds.empty() || (int)courseIds.size() > SchoolConstants::MAX_COURSES_PER_TEACHER)
        return "Teacher must be assigned between 1 and " + to_string(SchoolConstants::MAX_COURSES_PER_TEACHER) + " courses.";

    Teacher* teacher = teacherRepository.findTeacherById(teacherId);
    if (!teacher)
        return "Teacher not found.";

    if ((int)(teacher->getAssignedCourses().size() + courseIds.size()) > SchoolConstants::MAX_COURSES_PER_TEACHER)
        return "Teacher cannot be assigned more than " + to_string(SchoolConstants::MAX_COURSES_PER_TEACHER) + " courses.";

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
    if (!errors.empty())
        return "Assignment failed:\n" + errors;

    for (int i = 0; i < (int)courseIds.size(); i++) {
        courseRepository.assignTeacherToCourse(courseIds[i], teacher->getId(), teacher->getName());
    }

    return teacherRepository.assignCoursesToTeacher(teacherId, courseIds);
}

string TeacherServiceImpl::replaceTeacherInCourse(const string& courseId, const string& oldTeacherId, const string& newTeacherId) {
    if (oldTeacherId == newTeacherId) {
        return "Old and new teacher IDs must be different.";
    }

    Course* course = courseRepository.findCourseById(courseId);
    if (!course) {
        return "Course not found.";
    }

    Teacher* oldTeacher = teacherRepository.findTeacherById(oldTeacherId);
    if (!oldTeacher) {
        return "Old teacher not found.";
    }

    Teacher* newTeacher = teacherRepository.findTeacherById(newTeacherId);
    if (!newTeacher) {
        return "New teacher not found.";
    }

    if (!oldTeacher->isCourseAssigned(courseId)) {
        return "Old teacher is not assigned to this course.";
    }

    const vector<string>& courseTeacherIds = course->getTeacherIds();
    bool oldInCourse = false;
    for (int i = 0; i < (int)courseTeacherIds.size(); i++) {
        if (courseTeacherIds[i] == oldTeacherId) {
            oldInCourse = true;
            break;
        }
    }
    if (!oldInCourse) {
        return "Old teacher is not registered for this course.";
    }

    if (newTeacher->isCourseAssigned(courseId)) {
        return "New teacher is already assigned to this course.";
    }

    if (getStageFromGrade(newTeacher->getGrade()) != getStageFromGrade(course->getGrade())) {
        return "New teacher is in a different school stage than the course.";
    }

    if (newTeacher->getSpecialization() != course->getSpecialization()) {
        return "Course specialization does not match the new teacher's specialization.";
    }

    int currentNewCourses = (int)newTeacher->getAssignedCourses().size();

    if (currentNewCourses + 1 > SchoolConstants::MAX_COURSES_PER_TEACHER) {
        return "New teacher cannot be assigned more than " + to_string(SchoolConstants::MAX_COURSES_PER_TEACHER) + " courses.";
    }

    Stage courseStage = getStageFromGrade(course->getGrade());
    int maxSeats = getMaxStudentsForStage(courseStage);
    int enrolled = course->getNumberOfAssignedStudents();
    int available = maxSeats - enrolled;
    int requiredSeats = getMinAvailableSeatsForStage(courseStage);
    if (available < requiredSeats) {
        return "Course does not have enough available seats (" + to_string(available) + " available, " + to_string(requiredSeats) + " required).";
    }

    oldTeacher->removeCourse(courseId);
    course->removeTeacherById(oldTeacherId);

    string assignResult = courseRepository.assignTeacherToCourseForReplace(courseId, newTeacherId, newTeacher->getName());
    if (assignResult.find("Error:") != string::npos)
        return assignResult;

    vector<string> oneCourse;
    oneCourse.push_back(courseId);

    teacherRepository.assignCoursesToTeacher(newTeacherId, oneCourse);

    const vector<string>& enrolledStudentIds = course->getAssignedStudents();

    for (int i = 0; i < (int)enrolledStudentIds.size(); i++) {

        Student* student = studentRepository.findStudentById(enrolledStudentIds[i]);
        if (student) {
            student->updateTeacherForCourse(courseId, newTeacher->getName());
        }
    }

    return "Teacher successfully replaced in course.";
}

string TeacherServiceImpl::unassignCourseFromTeacher(const string& teacherId, const string& courseId) {
    Teacher* teacher = teacherRepository.findTeacherById(teacherId);
    if (!teacher)
        return "Teacher not found.";

    Course* course = courseRepository.findCourseById(courseId);
    if (!course)
        return "Course not found.";

    if (!teacher->isCourseAssigned(courseId))
        return "This course is not assigned to the teacher.";

    if (course->getNumberOfAssignedStudents() > 0)
        return "Cannot reassign course with enrolled students. Use replace option instead.";

    teacher->removeCourse(courseId);
    course->removeTeacherById(teacherId);

    return "Course unassigned from teacher successfully.";
}

string TeacherServiceImpl::removeTeacher(const string& id) {
    Teacher* teacher = teacherRepository.findTeacherById(id);
    if (!teacher) {
        return "Teacher not found.";
    }

    const vector<string>& assignedCourses = teacher->getAssignedCourses();
    string blockingCourses;

    for (int i = 0; i < (int)assignedCourses.size(); i++) {
        const string& courseId = assignedCourses[i];
        Course* course = courseRepository.findCourseById(courseId);
        if (!course) {
            continue;
        }
        if (course->getNumberOfAssignedStudents() > 0) {
            blockingCourses += "- " + course->getName() + " (ID: " + course->getId() + ")\n";
        }
    }

    if (!blockingCourses.empty()) {
        return "Cannot remove teacher. The teacher still has students registered in the following courses:\n" + blockingCourses + "Please reassign or remove those students/courses first.";
    }

    for (int i = 0; i < (int)assignedCourses.size(); i++) {
        const string& courseId = assignedCourses[i];
        Course* course = courseRepository.findCourseById(courseId);
        if (!course) {
            continue;
        }
        course->removeTeacherById(teacher->getId());
    }

    teacher->removeAllCourses();

    return teacherRepository.removeTeacher(id);
}


// ─────────────────────────────────────────────
//  CourseServiceImpl
// ─────────────────────────────────────────────
CourseServiceImpl::CourseServiceImpl(CourseRepository& courseRepo, TeacherRepository& teacherRepo, CourseValidator& validator)
    : courseRepository(courseRepo), teacherRepository(teacherRepo), courseValidator(validator) {}

Course* CourseServiceImpl::findCourseById(const string& id) {
    return courseRepository.findCourseById(id);
}

int CourseServiceImpl::getMaxCoursesForGrade(int grade) const {
    return courseRepository.getMaxCoursesForGrade(grade);
}

string CourseServiceImpl::addCourse(int grade, Course& course) {

    vector<Course> existing = courseRepository.getCoursesInSchoolVector();
    for (int i = 0; i < (int)existing.size(); i++) {
        if (existing[i].getName() == course.getName() &&
            existing[i].getGrade() == course.getGrade() &&
            existing[i].getSpecialization() == course.getSpecialization())
            return "Course already exists.";
    }

    string errors;
    if (!courseValidator.validateCourseName(course.getName()))
        errors += "- Invalid course name.\n";
    if (!courseValidator.validateGrade(course.getGrade()))
        errors += "- Invalid grade. Must be between 1 and 12 (make sure your input is DIGITS ONLY).\n";
    if (!courseValidator.validateSubjectHours(course.getSubjectHours()))
        errors += "- Subject hours must be between 2 and 6 (make sure your input is DIGITS ONLY).\n";
    if (!courseValidator.validateSpecialization(course.getSpecialization()))
        errors += "- Specialization cannot be empty.\n";
    if (!errors.empty())
        return "Course cannot be added:\n" + errors;

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
        errors += "- Invalid grade. Must be between 1 and 12 (make sure your input is DIGITS ONLY).\n";
    if (!courseValidator.validateSubjectHours(newData.getSubjectHours()))
        errors += "- Subject hours must be between 2 and 6 (make sure your input is DIGITS ONLY).\n";
    if (!courseValidator.validateSpecialization(newData.getSpecialization()))
        errors += "- Specialization cannot be empty.\n";
    if (!errors.empty())
        return "Course cannot be updated:\n" + errors;

    return courseRepository.editCourse(id, newData);
}

string CourseServiceImpl::removeCourse(const string& id) {
    Course* course = courseRepository.findCourseById(id);
    if (!course)
        return "Course not found.";

    if (course->getNumberOfAssignedStudents() > 0){
        return "Cannot remove course. There are students enrolled in it.";
    }

    const vector<string>& teacherIds = course->getTeacherIds();

    for (int i = 0; i < (int)teacherIds.size(); i++) {
        Teacher* teacher = teacherRepository.findTeacherById(teacherIds[i]);
        if (teacher)
            teacher->removeCourse(id);
    }

    return courseRepository.removeCourse(id);
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

    if (studentRepository.findStudentByNationalNumber(student.getNationalNumber()))
        return "Student already exists.";


    string errors;
    if (!studentValidator.validateName(student.getName()))
        errors += "- Name cannot be empty.\n";
    if (!studentValidator.validateNationalNumber(student.getNationalNumber()))
        errors += "- National number must be 14 characters (make sure your input is DIGITS ONLY).\n";
    if (!studentValidator.validateGrade(grade))
        errors += "- Grade must be between 1 and 12 (make sure your input is DIGITS ONLY).\n";
    if (!studentValidator.validateAge(student.getAge(), grade))
        errors += "- Student age does not match the expected range for this grade (make sure your input is DIGITS ONLY).\n";
    if (!studentValidator.validatePhoneNumber(student.getPhoneNumber()))
        errors += "- Phone number must be 10-12 characters (make sure your input is DIGITS ONLY)\n";
    if (!errors.empty())
        return "Student registration failed:\n" + errors;

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
        errors += "- Age does not match the expected range for this grade (make sure your input is DIGITS ONLY).\n";
    if (!studentValidator.validatePhoneNumber(newData.getPhoneNumber()))
        errors += "- Phone number must be 10-12 characters (make sure your input is DIGITS ONLY)\n";
    if (!studentValidator.validateGpa((float)newData.getGpa()))
        errors += "- GPA must be between 0.0 and 4.0 (make sure your input is DIGITS ONLY)\n";
    if (!studentValidator.validateGrade(newData.getGrade()))
        errors += "- Grade must be between 1 and 12 characters (make sure your input is DIGITS ONLY)\n";
    if (!errors.empty())
        return "Student cannot be updated:\n" + errors;

    return studentRepository.editStudent(id, newData);
}

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

    int maxStudentsPerCourse = studentRepository.getMaxStudentsForGrade(studentGrade);
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
            if (teachers[j] == teacherName) {
                teacherBelongsToCourse = true;
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

    for (int i = 0; i < (int)validatedIds.size(); i++)
        courseRepository.assignStudentToCourse(studentId, validatedIds[i]);

    string result = studentRepository.assignCoursesToStudent(studentId, validatedIds, validatedTeachers);

    if (!errors.empty())
        return "Partially assigned. Failed courses:\n" + errors;

    return result;
}

string StudentServiceImpl::removeStudent(const string& id) {
    Student* student = studentRepository.findStudentById(id);
    if (!student)
        return "Student not found.";

    const vector<StudentCourse>& courses = student->getAssignedCourses();

    for (int i = 0; i < (int)courses.size(); i++) {
        courseRepository.removeStudentFromCourse(id, courses[i].courseId);
    }

    return studentRepository.removeStudent(id);
}
