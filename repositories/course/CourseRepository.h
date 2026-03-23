#ifndef COURSE_REPOSITORY_H
#define COURSE_REPOSITORY_H

#include "../../models/Course.h"
#include "../../common/Stage.h"
#include <map>
#include <vector>
#include <string>

class CourseRepository {
public:
    virtual Course* findCourseById(const std::string& id) = 0;
    virtual int getNumberOfCoursesInGrade(int grade) const = 0;
    virtual std::vector<Course> getCoursesInSchoolVector() = 0;
    virtual int getMaxCoursesForGrade(int grade) const = 0;
    virtual std::string addCourse(int grade, Course& course) = 0;
    virtual std::string editCourse(const std::string& id, const Course& newData) = 0;
    virtual std::string assignTeacherToCourse(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) = 0;
    virtual std::string assignTeacherToCourseForReplace(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) = 0;
    virtual std::string assignTeacherToClassInCourse(const std::string& courseId, int classNum, const std::string& teacherId) = 0;
    virtual std::string assignStudentToCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::string removeStudentFromCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::string removeCourse(const std::string& id) = 0;
    virtual bool saveToFile(const std::string& filename) = 0;
    virtual ~CourseRepository() = default;
};

class CourseRepositoryImpl : public CourseRepository {
private:
    std::vector<Course> allCourses;
    std::map<int, std::vector<int>> gradeIndex;
    std::map<Stage, std::vector<int>> stageIndex;
public:
    Course* findCourseById(const std::string& id) override;
    int getNumberOfCoursesInGrade(int grade) const override;
    std::vector<Course> getCoursesInSchoolVector() override;
    int getMaxCoursesForGrade(int grade) const override;
    std::string addCourse(int grade, Course& course) override;
    std::string editCourse(const std::string& id, const Course& newData) override;
    std::string assignTeacherToCourse(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) override;
    std::string assignTeacherToCourseForReplace(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) override;
    std::string assignTeacherToClassInCourse(const std::string& courseId, int classNum, const std::string& teacherId) override;
    std::string assignStudentToCourse(const std::string& studentId, const std::string& courseId) override;
    std::string removeStudentFromCourse(const std::string& studentId, const std::string& courseId) override;
    std::string removeCourse(const std::string& id) override;
    bool saveToFile(const std::string& filename) override;
    void loadFromFile(const std::string& filename);
};

#endif
