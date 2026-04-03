#include "CourseService.h"
using namespace std;

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
        {
            return "Course already exists.";
        }
    }

    string errors;

    if (!courseValidator.validateCourseName(course.getName())) {
        errors += "- Invalid course name.\n";
    }

    if (!courseValidator.validateGrade(course.getGrade())) {
        errors += "- Invalid grade. Must be between 1 and 12 (make sure your input is DIGITS ONLY).\n";
    }

    if (!courseValidator.validateSubjectHours(course.getSubjectHours())) {
        errors += "- Subject hours must be between 2 and 6 (make sure your input is DIGITS ONLY).\n";
    }

    if (!courseValidator.validateSpecialization(course.getSpecialization())) {
        errors += "- Specialization cannot be empty.\n";
    }

    if (!errors.empty()) {
        return "Course cannot be added:\n" + errors;
    }

    if (courseRepository.getNumberOfCoursesInGrade(grade) >= courseRepository.getMaxCoursesForGrade(grade)) {
        return "Maximum number of courses reached for this grade.";
    }

    return courseRepository.addCourse(grade, course);
}

string CourseServiceImpl::editCourse(const string& id, const Course& newData) {
    Course* existing = courseRepository.findCourseById(id);
    if (!existing) {
        return "Course not found.";
    }

    if (existing->getNumberOfAssignedStudents() > 0) {
        return "cannot modify course: there are students registered in this course.";
    }

    bool gradeChanged = (existing->getGrade() != newData.getGrade());
    bool specChanged = (existing->getSpecialization() != newData.getSpecialization());

    string errors;

    if (!courseValidator.validateCourseName(newData.getName())) {
        errors += "- Invalid course name.\n";
    }

    if (!courseValidator.validateGrade(newData.getGrade())) {
        errors += "- Invalid grade. Must be between 1 and 12 (make sure your input is DIGITS ONLY).\n";
    }

    if (!courseValidator.validateSubjectHours(newData.getSubjectHours())) {
        errors += "- Subject hours must be between 2 and 6 (make sure your input is DIGITS ONLY).\n";
    }

    if (!courseValidator.validateSpecialization(newData.getSpecialization())) {
        errors += "- Specialization cannot be empty.\n";
    }

    if (!errors.empty()) {
        return "Course cannot be updated:\n" + errors;
    }

    if (gradeChanged) {
        if (getStageFromGrade(existing->getGrade()) != getStageFromGrade(newData.getGrade())) {
            return "cannot change grade to a different stage.";
        }

        if (courseRepository.getNumberOfCoursesInGrade(newData.getGrade()) >= courseRepository.getMaxCoursesForGrade(newData.getGrade())) {
            return "cannot update: maximum number of courses reached for the target grade.";
        }

        vector<Course> all = courseRepository.getCoursesInSchoolVector();
        for (int i = 0; i < (int)all.size(); i++) {
            if (all[i].getId() == id) {
                continue;
            }
            if (all[i].getName() == newData.getName() && all[i].getGrade() == newData.getGrade() && all[i].getSpecialization() == newData.getSpecialization()) {
                return "cannot update: a course with the same name, grade, and specialization already exists.";
            }
        }
    }

    if (specChanged) {
        vector<string> teacherIdsCopy = existing->getTeacherIds();
        for (int i = 0; i < (int)teacherIdsCopy.size(); i++) {
            Teacher* t = teacherRepository.findTeacherById(teacherIdsCopy[i]);
            if (t && t->getSpecialization() != newData.getSpecialization()) {
                t->removeCourse(id);
                existing->removeTeacherById(teacherIdsCopy[i]);
            }
        }
    }

    vector<string> remainingTeacherIds = existing->getTeacherIds();

    string result = courseRepository.editCourse(id, newData);

    if (gradeChanged) {
        string newCourseId = existing->getId();
        if (newCourseId != id) {
            for (int i = 0; i < (int)remainingTeacherIds.size(); i++) {
                Teacher* t = teacherRepository.findTeacherById(remainingTeacherIds[i]);
                if (t) {
                    t->removeCourse(id);
                    t->assignCourse(newCourseId);
                }
            }
        }
    }

    return result;
}

string CourseServiceImpl::removeCourse(const string& id) {
    Course* course = courseRepository.findCourseById(id);

    if (!course) {
        return "Course not found.";
    }

    if (course->getNumberOfAssignedStudents() > 0) {
        return "Cannot remove course. There are students enrolled in it.";
    }

    const vector<string>& teacherIds = course->getTeacherIds();

    for (int i = 0; i < (int)teacherIds.size(); i++) {
        Teacher* teacher = teacherRepository.findTeacherById(teacherIds[i]);
        if (teacher) {
            teacher->removeCourse(id);
        }
    }

    return courseRepository.removeCourse(id);
}
