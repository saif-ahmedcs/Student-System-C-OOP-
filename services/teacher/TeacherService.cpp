#include "TeacherService.h"
#include "../../common/SchoolConstants.h"
using namespace std;

TeacherServiceImpl::TeacherServiceImpl(TeacherRepository& teacherRepo, CourseRepository& courseRepo, StudentRepository& studentRepo, TeacherValidator& validator)
    : teacherRepository(teacherRepo), courseRepository(courseRepo), studentRepository(studentRepo), teacherValidator(validator) {}

int TeacherServiceImpl::getMinAvailableSeatsForStage(Stage stage) const {
    switch (stage) {
        case Stage::Primary: return SchoolConstants::MIN_AVAILABLE_SEATS_PRIMARY;
        case Stage::Middle: return SchoolConstants::MIN_AVAILABLE_SEATS_MIDDLE;
        case Stage::Secondary: return SchoolConstants::MIN_AVAILABLE_SEATS_SECONDARY;
    }
    return 0;
}

int TeacherServiceImpl::getMaxStudentsForStage(Stage stage) const {
    switch (stage) {
        case Stage::Primary: return SchoolConstants::MAX_STUDENTS_IN_PRIMARY;
        case Stage::Middle: return SchoolConstants::MAX_STUDENTS_IN_MIDDLE;
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

string TeacherServiceImpl::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds, const vector<vector<int>>& courseClasses) {
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
            errors += "- Course " + cid + " already has the maximum number of teachers.\n";
            continue;
        }
        if (getStageFromGrade(teacher->getGrade()) != getStageFromGrade(c->getGrade())) {
            errors += "- Course " + cid + " is in a different school stage than the teacher.\n";
            continue;
        }
        if (teacher->getSpecialization() != c->getSpecialization()) {
            errors += "- Course " + cid + " specialization does not match the teacher's.\n";
            continue;
        }
        if (teacher->isCourseAssigned(cid)) {
            errors += "- Course " + cid + " is already assigned to this teacher.\n";
            continue;
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
        const vector<int>& classes = courseClasses[i];
        for (int j = 0; j < (int)classes.size(); j++) {
            if (c->isClassTaken(classes[j])) {
                errors += "- Course " + cid + " class " + to_string(classes[j]) + " already has a teacher assigned.\n";
            }
        }
    }
    if (!errors.empty()){
        return "Assignment failed:\n" + errors;
    }

    for (int i = 0; i < (int)courseIds.size(); i++){
        courseRepository.assignTeacherToCourse(courseIds[i], teacher->getId(), teacher->getName());
        const vector<int>& classes = courseClasses[i];
        for (int j = 0; j < (int)classes.size(); j++) {
            courseRepository.assignTeacherToClassInCourse(courseIds[i], classes[j], teacher->getId());
        }
    }

    return teacherRepository.assignCoursesToTeacher(teacherId, courseIds);
}

string TeacherServiceImpl::replaceTeacherInCourse(const string& courseId, const string& oldTeacherId, const string& newTeacherId) {
    if (oldTeacherId == newTeacherId)
        return "Old and new teacher IDs must be different.";

    Course* course = courseRepository.findCourseById(courseId);
    if (!course)
        return "Course not found.";

    Teacher* oldTeacher = teacherRepository.findTeacherById(oldTeacherId);
    if (!oldTeacher)
        return "Old teacher not found.";

    Teacher* newTeacher = teacherRepository.findTeacherById(newTeacherId);
    if (!newTeacher)
        return "New teacher not found.";

    if (!oldTeacher->isCourseAssigned(courseId))
        return "Old teacher is not assigned to this course.";

    const vector<string>& courseTeacherIds = course->getTeacherIds();
    bool oldInCourse = false;
    for (int i = 0; i < (int)courseTeacherIds.size(); i++) {
        if (courseTeacherIds[i] == oldTeacherId) { oldInCourse = true; break; }
    }
    if (!oldInCourse) return "Old teacher is not registered for this course.";

    if (newTeacher->isCourseAssigned(courseId))
        return "New teacher is already assigned to this course.";

    if (getStageFromGrade(newTeacher->getGrade()) != getStageFromGrade(course->getGrade()))
        return "New teacher is in a different school stage than the course.";

    if (newTeacher->getSpecialization() != course->getSpecialization())
        return "Course specialization does not match the new teacher's specialization.";

    int currentNewCourses = (int)newTeacher->getAssignedCourses().size();
    if (currentNewCourses + 1 > SchoolConstants::MAX_COURSES_PER_TEACHER)
        return "New teacher cannot be assigned more than " + to_string(SchoolConstants::MAX_COURSES_PER_TEACHER) + " courses.";

    Stage courseStage = getStageFromGrade(course->getGrade());
    int maxSeats = getMaxStudentsForStage(courseStage);
    int enrolled = course->getNumberOfAssignedStudents();
    int available = maxSeats - enrolled;
    int requiredSeats = getMinAvailableSeatsForStage(courseStage);
    if (available < requiredSeats)
        return "Course does not have enough available seats (" + to_string(available) + " available, " + to_string(requiredSeats) + " required).";

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
        if (student) { student->updateTeacherForCourse(courseId, newTeacher->getName()); }
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
    if (!teacher)
        return "Teacher not found.";

    const vector<string>& assignedCourses = teacher->getAssignedCourses();
    string blockingCourses;
    for (int i = 0; i < (int)assignedCourses.size(); i++) {
        Course* course = courseRepository.findCourseById(assignedCourses[i]);
        if (!course)
            continue;
        if (course->getNumberOfAssignedStudents() > 0)
            blockingCourses += "- " + course->getName() + " (ID: " + course->getId() + ")\n";
    }

    if (!blockingCourses.empty())
        return "Cannot remove teacher. The teacher still has students registered in the following courses:\n" + blockingCourses + "Please reassign or remove those students/courses first.";

    for (int i = 0; i < (int)assignedCourses.size(); i++) {
        Course* course = courseRepository.findCourseById(assignedCourses[i]);
        if (!course)
            continue;
        course->removeTeacherById(teacher->getId());
    }

    teacher->removeAllCourses();
    return teacherRepository.removeTeacher(id);
}
