#ifndef COURSE_SERVICE_H
#define COURSE_SERVICE_H

#include "../../repositories/course/CourseRepository.h"
#include "../../repositories/teacher/TeacherRepository.h"
#include "../../validators/course/CourseValidator.h"
#include <string>
#include <vector>

class CourseService {
public:
    virtual Course* findCourseById(const std::string& id) = 0;
    virtual int getMaxCoursesForGrade(int grade) const = 0;
    virtual std::string addCourse(int grade, Course& course) = 0;
    virtual std::string editCourse(const std::string& id, const Course& newData) = 0;
    virtual std::string removeCourse(const std::string& id) = 0;
    virtual ~CourseService() = default;
};

class CourseServiceImpl : public CourseService {
private:
    CourseRepository& courseRepository;
    TeacherRepository& teacherRepository;
    CourseValidator& courseValidator;

public:
    CourseServiceImpl(CourseRepository& courseRepo, TeacherRepository& teacherRepo, CourseValidator& validator);

    Course* findCourseById(const std::string& id) override;
    int getMaxCoursesForGrade(int grade) const override;
    std::string addCourse(int grade, Course& course) override;
    std::string editCourse(const std::string& id, const Course& newData) override;
    std::string removeCourse(const std::string& id) override;
};

#endif
