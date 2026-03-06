#include "Repo.h"
#include "../SchoolConstants.h"
#include <string>
using namespace std;

string generateStudentID(int grade){

static int id[13]={0};

string idStart;

 if (grade == 1) idStart = "2019";
    else if (grade == 2) idStart = "2018";
    else if (grade == 3) idStart = "2017";
    else if (grade == 4) idStart = "2016";
    else if (grade == 5) idStart = "2015";
    else if (grade == 6) idStart = "2014";
    else if (grade == 7) idStart = "2013";
    else if (grade == 8) idStart = "2012";
    else if (grade == 9) idStart = "2011";
    else if (grade == 10) idStart = "2010";
    else if (grade == 11) idStart = "2009";
    else if (grade == 12) idStart = "2008";
    else return "Invalid grade";

    if (id[grade] >= 0 && id[grade] <= 9){
        id[grade]++;
        return idStart + "00" + to_string(id[grade]);
    }
    else if (id[grade] >= 10 && id[grade] <= 99){
        id[grade]++;
        return idStart + "0" + to_string(id[grade]);
    }
    else {
        id[grade]++;
        return idStart + to_string(id[grade]);
    }

}
// Generates a unique course ID based on the course name and grade and specialization.
string generateCourseID(const string& courseName, int grade, const string& specialization) {

    static int id[13] = {0};
    string idStart = "";


    for (int i = 0; i < (int)courseName.length(); i++) {
        if (courseName[i] != ' ')
            idStart += tolower(courseName[i]);
    }
    idStart +='-';
    idStart += to_string(grade);
    idStart +='-';


    for (int i = 0; i < (int)specialization.length(); i++) {
        if (specialization[i] != ' ')
            idStart += tolower(specialization[i]);
    }


    id[grade]++;
    int count = id[grade];

    if (count <= 9)
        return idStart + "00" + to_string(count);
    else if (count <= 99)
        return idStart + "0" + to_string(count);
    else
        return idStart + to_string(count);
}


// Generates a unique teacher ID based on the teacher's grade.

string generateTeacherID(int grade) {

    static int id[13] = {0};
    string idStart;

    if (grade >= 1 && grade <= 12)
        idStart = to_string(grade);
    else
        return "Invalid grade";

    id[grade]++;

    if (id[grade] <= 9)
        return idStart + "00" + to_string(id[grade]);
    else if (id[grade] <= 99)
        return idStart + "0" + to_string(id[grade]);
    else
        return idStart + to_string(id[grade]);
}


// ─────────────────────────────────────────────
//  TeacherRepositoryImpl
// ─────────────────────────────────────────────
int TeacherRepositoryImpl::getTeachersInGrade(int grade) const {
    std::map<int, std::vector<int>>::const_iterator it = gradeIndex.find(grade);
    if (it != gradeIndex.end()) {
        return (int)it->second.size();
    }
    return 0;
}

int TeacherRepositoryImpl::getMaxTeachersForGrade(int grade) const {
    switch (getStageFromGrade(grade)) {
        case Stage::Primary:
            return SchoolConstants::MAX_TEACHERS_IN_PRIMARY;
        case Stage::Middle:
            return SchoolConstants::MAX_TEACHERS_IN_MIDDLE;
        case Stage::Secondary:
            return SchoolConstants::MAX_TEACHERS_IN_SECONDARY;
    }
    return 0;
}

Teacher* TeacherRepositoryImpl::findTeacherByNationalNumber(const std::string& nationalNumber) {
    for (int i = 0; i < (int)allTeachers.size(); i++) {
        Teacher& teacher = allTeachers[i];
        if (teacher.getNationalNumber() == nationalNumber)
            return &teacher;
    }
    return nullptr;
}

Teacher* TeacherRepositoryImpl::findTeacherById(const std::string& id) {
    for (int i = 0; i < (int)allTeachers.size(); i++) {
        Teacher& teacher = allTeachers[i];
        if (teacher.getId() == id)
            return &teacher;
    }
    return nullptr;
}

string TeacherRepositoryImpl::addTeacher(int grade, Teacher& teacher) {
    string finalId = generateTeacherID(grade);
    teacher.setId(finalId);

    allTeachers.push_back(teacher);
    int idx = (int)allTeachers.size() - 1;

    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);

    return "Teacher added successfully. Assigned ID: " + finalId;
}

string TeacherRepositoryImpl::editTeacher(const string& id, const Teacher& newData) {
    Teacher* t = findTeacherById(id);
    if (!t) return "Teacher not found.";

    t->setName(newData.getName());
    t->setAge(newData.getAge());
    t->setGrade(newData.getGrade());
    t->setSubject(newData.getSubject());
    t->setExperienceYears(newData.getExperienceYears());
    t->setMonthlySalary(newData.getMonthlySalary());
    t->setSpecialization(newData.getSpecialization());

    return "Teacher data updated successfully.";
}

string TeacherRepositoryImpl::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds) {
    Teacher* teacher = findTeacherById(teacherId);
    if (!teacher)
        return "Teacher not found.";

    for (int i = 0; i < (int)courseIds.size(); i++) {
        teacher->assignCourse(courseIds[i]);
    }

    return "Courses assigned to teacher successfully.";
}

string TeacherRepositoryImpl::removeTeacher(const string& id) {
    int removeIndex = -1;
    for (int i = 0; i < (int)allTeachers.size(); i++) {
        if (allTeachers[i].getId() == id) {
            removeIndex = i;
            break;
        }
    }
    if (removeIndex == -1) {
        return "Teacher not found.";
    }

    int grade = allTeachers[removeIndex].getGrade();
    Stage stage = getStageFromGrade(grade);

    std::map<int, std::vector<int>>::iterator git = gradeIndex.find(grade);

    if (git != gradeIndex.end()) {
        std::vector<int>& indices = git->second;
        for (int i = 0; i < (int)indices.size(); i++) {
            if (indices[i] == removeIndex) {
                indices.erase(indices.begin() + i);
                i--;
            } else if (indices[i] > removeIndex) {
                indices[i] = indices[i] - 1;
            }
        }
        if (indices.empty()) {
            gradeIndex.erase(git);
        }
    }

    std::map<Stage, std::vector<int>>::iterator sit = stageIndex.find(stage);
    if (sit != stageIndex.end()) {
        std::vector<int>& indices = sit->second;
        for (int i = 0; i < (int)indices.size(); i++) {
            if (indices[i] == removeIndex) {
                indices.erase(indices.begin() + i);
                i--;
            }
            else if (indices[i] > removeIndex) {
                indices[i] = indices[i] - 1;
            }
        }


        if (indices.empty()) {
            stageIndex.erase(sit);
        }
    }

    allTeachers.erase(allTeachers.begin() + removeIndex);
    return "Teacher removed successfully.";
}

// ─────────────────────────────────────────────
//  CourseRepositoryImpl
// ─────────────────────────────────────────────
int CourseRepositoryImpl::getNumberOfCoursesInGrade(int grade) const {
    for (std::map<int, std::vector<int>>::const_iterator it = gradeIndex.begin();
         it != gradeIndex.end(); ++it) {
        if (it->first == grade) {
            return (int)it->second.size();
        }
    }
    return 0;
}

vector<Course> CourseRepositoryImpl::getCoursesInSchoolVector() {
    return allCourses;
}

int CourseRepositoryImpl::getMaxCoursesForGrade(int grade) const {
    switch (getStageFromGrade(grade)) {
        case Stage::Primary:
            return SchoolConstants::MAX_COURSES_IN_PRIMARY;
        case Stage::Middle:
            return SchoolConstants::MAX_COURSES_IN_MIDDLE;
        case Stage::Secondary:
            return SchoolConstants::MAX_COURSES_IN_SECONDARY;
    }
    return 0;
}

Course* CourseRepositoryImpl::findCourseById(const string& id) {
    for (int i = 0; i < (int)allCourses.size(); i++) {
        if (allCourses[i].getId() == id)
            return &allCourses[i];
    }
    return nullptr;
}

string CourseRepositoryImpl::addCourse(int grade, Course& course) {
    string finalId = generateCourseID(course.getName(), grade, course.getSpecialization());
    course.setId(finalId);

    allCourses.push_back(course);
    int idx = (int)allCourses.size() - 1;

    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);

    return "Course added successfully to grade " + to_string(grade) + ". ID: " + finalId;
}

string CourseRepositoryImpl::editCourse(const string& id, const Course& newData) {
    Course* c = findCourseById(id);
    if (!c)
        return "Course not found.";

    c->setName(newData.getName());
    c->setGrade(newData.getGrade());
    c->setSubjectHours(newData.getSubjectHours());

    return "Course data updated successfully.";
}

string CourseRepositoryImpl::assignTeacherToCourse(const string& courseId, const string& teacherId, const string& teacherName) {
    Course* course = findCourseById(courseId);
    if (!course)
        return "Error: Course " + courseId + " not found.";
    if (!course->assignTeacher(teacherId, teacherName))
        return "Error: Teacher already assigned to course " + courseId + ".";
    return "Teacher assigned to course successfully.";
}

string CourseRepositoryImpl::assignTeacherToCourseForReplace(const string& courseId, const string& teacherId, const string& teacherName) {
    Course* course = findCourseById(courseId);
    if (!course)
        return "Error: Course " + courseId + " not found.";
    if (!course->assignTeacher(teacherId, teacherName))
        return "Error: Teacher already assigned to course " + courseId + ".";
    return "Teacher assigned to course successfully.";
}

string CourseRepositoryImpl::assignStudentToCourse(const string& studentId, const string& courseId) {
    Course* course = findCourseById(courseId);
    if (!course)
        return "Error: Course " + courseId + " not found.";
    if (!course->assignStudent(studentId))
        return "Error: Student already assigned to course " + courseId + ".";

    return "Student assigned to course successfully.";
}

// ─────────────────────────────────────────────
//  StudentRepositoryImpl
// ─────────────────────────────────────────────
Student* StudentRepositoryImpl::findStudentByNationalNumber(const string& nn) {
    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i].getNationalNumber() == nn)
            return &allStudents[i];
    }
    return nullptr;
}

Student* StudentRepositoryImpl::findStudentById(const string& id) {
    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i].getId() == id)
            return &allStudents[i];
    }
    return nullptr;
}

int StudentRepositoryImpl::getStudentsInGrade(int grade) const {
    for (std::map<int, std::vector<int>>::const_iterator it = gradeIndex.begin();
         it != gradeIndex.end(); ++it) {
        if (it->first == grade)
            return (int)it->second.size();
    }
    return 0;
}

int StudentRepositoryImpl::getMaxStudentsForGrade(int grade) const {
    switch (getStageFromGrade(grade)) {
        case Stage::Primary:
            return SchoolConstants::MAX_STUDENTS_IN_PRIMARY;
        case Stage::Middle:
            return SchoolConstants::MAX_STUDENTS_IN_MIDDLE;
        case Stage::Secondary:
            return SchoolConstants::MAX_STUDENTS_IN_SECONDARY;
    }
    return 0;
}

string StudentRepositoryImpl::addStudent(int grade, Student& student) {
    string finalId = generateStudentID(grade);
    student.setId(finalId);

    allStudents.push_back(student);
    int idx = (int)allStudents.size() - 1;

    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);

    return "Student added successfully. Assigned ID: " + finalId;
}

string StudentRepositoryImpl::editStudent(const string& id, const Student& newData) {
    Student* s = findStudentById(id);
    if (!s)
        return "Student not found.";

    s->setName(newData.getName());
    s->setPhoneNumber(newData.getPhoneNumber());
    s->setGpa(newData.getGpa());
    s->setAge(newData.getAge());
    s->setGrade(newData.getGrade());

    return "Student data updated successfully.";
}

string StudentRepositoryImpl::assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) {
    Student* student = findStudentById(studentId);
    if (!student)
        return "Student not found.";

    for (int i = 0; i < (int)courseIds.size(); i++)
        student->assignCourse(courseIds[i], teacherNames[i]);

    return "Courses assigned to student successfully.";
}
