#include "Student.h"
#include "Teacher.h"
#include "Course.h"

void ShareData::setName(const std::string& n) {
    name = n;
}

void ShareData::setId(const std::string& i) {
    id = i;
}

std::string ShareData::getName() const {
    return name;
}

std::string ShareData::getId() const {
    return id;
}

void BaseEntity::setAge(int a) {
    age = a;
}

void BaseEntity::setPhoneNumber(const std::string& p) {
    phoneNumber = p;
}

void BaseEntity::setNationalNumber(const std::string& n) {
    nationalNumber = n;
}

int BaseEntity::getAge() const {
    return age;
}

std::string BaseEntity::getPhoneNumber() const {
    return phoneNumber;
}

std::string BaseEntity::getNationalNumber() const {
    return nationalNumber;
}

void Teacher::setGrade(int g) {
    grade = g;
}

void Teacher::setMonthlySalary(double s) {
    monthlySalary = s;
}

void Teacher::setSubject(const std::string& sub) {
    subject = sub;
}

void Teacher::setExperienceYears(int y) {
    experienceYears = y;
}

void Teacher::setSpecialization(const std::string& spec) {
    specialization = spec;
}

bool Teacher::assignCourse(const std::string& courseId) {
    if (isCourseAssigned(courseId)) {
        return false;
    }
    assignedCourseIds.push_back(courseId);
    return true;
}

bool Teacher::isCourseAssigned(const std::string& courseId) const {
    for (int i = 0; i < (int)assignedCourseIds.size(); i++) {
        if (assignedCourseIds[i] == courseId) {
            return true;
        }
    }
    return false;
}

bool Teacher::removeCourse(const std::string& courseId) {
    for (int i = 0; i < (int)assignedCourseIds.size(); i++) {
        if (assignedCourseIds[i] == courseId) {
            assignedCourseIds.erase(assignedCourseIds.begin() + i);
            return true;
        }
    }
    return false;
}

void Teacher::removeAllCourses() {
    assignedCourseIds.clear();
}

int Teacher::getGrade() const {
    return grade;
}

double Teacher::getMonthlySalary() const {
    return monthlySalary;
}

std::string Teacher::getSubject() const {
    return subject;
}

int Teacher::getExperienceYears() const {
    return experienceYears;
}

std::string Teacher::getSpecialization() const {
    return specialization;
}

const std::vector<std::string>& Teacher::getAssignedCourses() const {
    return assignedCourseIds;
}

void Course::setGrade(int g) {
    grade = g;
}

void Course::setSubjectHours(int h) {
    subjectHours = h;
}

void Course::setSpecialization(const std::string& spec) {
    specialization = spec;
}

bool Course::assignTeacher(const std::string& tid, const std::string& tname) {
    for (int i = 0; i < (int)teacherIds.size(); i++) {
        if (teacherIds[i] == tid) {
            return false;
        }
    }
    teacherIds.push_back(tid);
    teacherNames.push_back(tname);
    return true;
}

bool Course::assignStudent(const std::string& studentId) {
    if (isStudentAssigned(studentId)) {
        return false;
    }
    assignedStudentIds.push_back(studentId);
    return true;
}

bool Course::isStudentAssigned(const std::string& studentId) const {
    for (int i = 0; i < (int)assignedStudentIds.size(); i++) {
        if (assignedStudentIds[i] == studentId) {
            return true;
        }
    }
    return false;
}

bool Course::assignTeacherToClass(int classNum, const std::string& teacherId) {
    if (isClassTaken(classNum)) {
        return false;
    }
    classTeacherMap[classNum] = teacherId;
    return true;
}

bool Course::isClassTaken(int classNum) const {
    return classTeacherMap.find(classNum) != classTeacherMap.end();
}

std::string Course::getTeacherForClass(int classNum) const {
    std::map<int, std::string>::const_iterator it = classTeacherMap.find(classNum);
    if (it != classTeacherMap.end()) {
        return it->second;
    }
    return "";
}

const std::map<int, std::string>& Course::getClassTeacherMap() const {
    return classTeacherMap;
}

bool Course::removeTeacherById(const std::string& teacherId) {
    bool removed = false;
    for (int i = 0; i < (int)teacherIds.size(); i++) {
        if (teacherIds[i] == teacherId) {
            teacherIds.erase(teacherIds.begin() + i);
            if (i < (int)teacherNames.size()) {
                teacherNames.erase(teacherNames.begin() + i);
            }
            removed = true;
            i--;
        }
    }
    for (std::map<int, std::string>::iterator it = classTeacherMap.begin(); it != classTeacherMap.end(); ) {
        if (it->second == teacherId) {
            it = classTeacherMap.erase(it);
        } else {
            ++it;
        }
    }
    return removed;
}

bool Course::removeStudentById(const std::string& studentId) {
    for (int i = 0; i < (int)assignedStudentIds.size(); i++) {
        if (assignedStudentIds[i] == studentId) {
            assignedStudentIds.erase(assignedStudentIds.begin() + i);
            return true;
        }
    }
    return false;
}

int Course::getGrade() const {
    return grade;
}

int Course::getSubjectHours() const {
    return subjectHours;
}

std::string Course::getSpecialization() const {
    return specialization;
}

int Course::getNumberOfTeachers() const {
    return (int)teacherIds.size();
}

const std::vector<std::string>& Course::getTeacherIds() const {
    return teacherIds;
}

const std::vector<std::string>& Course::getTeacherNames() const {
    return teacherNames;
}

const std::vector<std::string>& Course::getAssignedStudents() const {
    return assignedStudentIds;
}

int Course::getNumberOfAssignedStudents() const {
    return (int)assignedStudentIds.size();
}

void Student::setGrade(int g) {
    grade = g;
}

void Student::setClassNumber(int c) {
    classNumber = c;
}

void Student::setGpa(double g) {
    gpa = g;
}

bool Student::assignCourse(const std::string& courseId, const std::string& teacherName) {
    if (isCourseAssigned(courseId)) {
        return false;
    }
    StudentCourse sc;
    sc.courseId = courseId;
    sc.teacherName = teacherName;
    assignedCourses.push_back(sc);
    return true;
}

bool Student::isCourseAssigned(const std::string& courseId) const {
    for (int i = 0; i < (int)assignedCourses.size(); i++) {
        if (assignedCourses[i].courseId == courseId) {
            return true;
        }
    }
    return false;
}

bool Student::updateTeacherForCourse(const std::string& courseId, const std::string& newTeacherName) {
    for (int i = 0; i < (int)assignedCourses.size(); i++) {
        if (assignedCourses[i].courseId == courseId) {
            assignedCourses[i].teacherName = newTeacherName;
            return true;
        }
    }
    return false;
}

void Student::clearAssignedCourses() {
    assignedCourses.clear();
}

int Student::getGrade() const {
    return grade;
}

int Student::getClassNumber() const {
    return classNumber;
}

double Student::getGpa() const {
    return gpa;
}

const std::vector<StudentCourse>& Student::getAssignedCourses() const {
    return assignedCourses;
}

int Student::getNumberOfAssignedCourses() const {
    return (int)assignedCourses.size();
}
