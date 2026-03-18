#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

struct StudentCourse {
    std::string courseId;
    std::string teacherName;
};

class ShareData {
private:
    std::string name;
    std::string id;
public:
    virtual ~ShareData() = default;
    void setName(const std::string& name);
    void setId(const std::string& id);
    std::string getName() const;
    std::string getId() const;
};

class BaseEntity : public ShareData {
private:
    int age;
    std::string phoneNumber;
    std::string nationalNumber;
public:
    void setAge(int age);
    void setPhoneNumber(const std::string& phone);
    void setNationalNumber(const std::string& number);
    int getAge() const;
    std::string getPhoneNumber() const;
    std::string getNationalNumber() const;
};

class Student : public BaseEntity {
private:
    int grade;
    double gpa = 0.00;
    std::vector<StudentCourse> assignedCourses;
public:
    void setGrade(int grade);
    void setGpa(double gpa);
    bool assignCourse(const std::string& courseId, const std::string& teacherName);
    bool isCourseAssigned(const std::string& courseId) const;
    bool updateTeacherForCourse(const std::string& courseId, const std::string& newTeacherName);
    int getGrade() const;
    double getGpa() const;
    const std::vector<StudentCourse>& getAssignedCourses() const;
    int getNumberOfAssignedCourses() const;
};

#endif
