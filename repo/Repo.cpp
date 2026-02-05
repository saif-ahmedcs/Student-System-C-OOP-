#include "Repo.h"

const int maxStudentsPerGrade = 2;

////////////////// Global functions \\\\\\\\\\\\\\\


// Determines the educational stage (Primary, Middle, Secondary)
// based on the given grade number (1–12). Throws an exception for invalid grades.
Stage getStageFromGrade(int grade) {
    if (grade >= 1 && grade <= 6) return Stage::Primary;
    if (grade >= 7 && grade <= 9) return Stage::Middle;
    if (grade >= 10 && grade <= 12) return Stage::Secondary;
}

// Generates a unique student ID based on the student's grade.
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
// Generates a unique course ID based on the course name and grade.
string generateCourseID(const string& courseName, int grade) {

    static int id[13] = {0};
    string idStart = "";


    for (int i = 0; i < courseName.length(); i++) {
        if (courseName[i] != ' ')
            idStart += tolower(courseName[i]);
    }

    idStart += to_string(grade);
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


////////////////// TeacherRepositoryImpl \\\\\\\\\\\\\\\

void TeacherRepositoryImpl::addTeacherInGrade(int grade, Teacher &teacher) {
    teachersInGrade[grade].push_back(teacher);
}

void TeacherRepositoryImpl::addTeacherInStage(int grade, Teacher &teacher) {
    Stage stage = getStageFromGrade(grade);
    teachersInStage[stage].push_back(teacher);
}

void TeacherRepositoryImpl::addTeacherInSchool(Teacher &teacher) {
    teachersInSchool.push_back(teacher);
}

const vector<Teacher>& TeacherRepositoryImpl::getTeachersInGrade(int grade) const {
    static vector<Teacher> empty;
    auto it = teachersInGrade.find(grade);
    if (it != teachersInGrade.end()) {
      return it->second;
    } else {
      return empty;
    }

}

int TeacherRepositoryImpl::getMaxTeachersForStage(Stage stage) const {
    auto it = maxTeachersPerGradeInStage.find(stage);

    if (it == maxTeachersPerGradeInStage.end())
        return 0;

    return it->second;
}

Teacher* TeacherRepositoryImpl::findTeacherById(const string& id) {
    for (auto& [grade, teachers] : teachersInGrade) {
        for (auto& t : teachers) {
            if (t.getId() == id)
                return &t;
        }
    }
    return nullptr;
}


string TeacherRepositoryImpl::addTeacher(int grade, Teacher &teacher) {

    Stage stage = getStageFromGrade(grade);
    string finalId = generateTeacherID(grade);
    teacher.setId(finalId);
    addTeacherInGrade(grade, teacher);
    addTeacherInStage(grade, teacher);
    addTeacherInSchool(teacher);
    return "Teacher added successfully. Assigned ID: " + finalId;
}

string TeacherRepositoryImpl::editTeacher(const string& id, const Teacher& newData){

    Teacher* t = findTeacherById(id);
       if (!t){
        return "Teacher not found.";
        }

        t->setName(newData.getName());
        t->setAge(newData.getAge());
        t->setTeacherGrade(newData.getTeacherGrade());
        t->setTeacherSubject(newData.getTeacherSubject());
        t->setExperienceYears(newData.getTeacherExperienceYears());
        t->setMonthlySalary(newData.getMonthlySalary());

    return "Teacher data updated successfully.";
}


////////////////// CourseRepositoryImpl \\\\\\\\\\\\\\\

void CourseRepositoryImpl::addCourseInGrade(int grade, Course &course) {
    coursesInGrade[grade].push_back(course);
}

void CourseRepositoryImpl::addCourseInStage(int grade, Course &course) {
    Stage stage = getStageFromGrade(grade);
    coursesInStage[stage].push_back(course);
}

void CourseRepositoryImpl::addCourseInSchool(Course &course) {
    coursesInSchool.push_back(course);
}

const vector<Course>& CourseRepositoryImpl::getCoursesInGrade(int grade) const {
    static vector<Course> empty;
    auto it = coursesInGrade.find(grade);
    if (it != coursesInGrade.end())
        return it->second;
    return empty;
}

int CourseRepositoryImpl::getMaxCoursesForStage(Stage stage) const {
    auto it = maxCoursesPerGradeInStage.find(stage);

    if (it == maxCoursesPerGradeInStage.end())
        return 0;

    return it->second;
}

Course* CourseRepositoryImpl::findCourseById(const string& id) {
    for (auto& [grade, courses] : coursesInGrade) {
        for (auto& c : courses) {
            if (c.getId() == id)
                return &c;
        }
    }
    return nullptr;
}



string CourseRepositoryImpl::addCourse(int grade, Course &course) {

    Stage stage = getStageFromGrade(grade);
    string finalId = generateCourseID(course.getName(),grade);
    course.setId(finalId);
    addCourseInGrade(grade, course);
    addCourseInStage(grade, course);
    addCourseInSchool(course);

return string("Course added successfully to grade ") + to_string(grade) + " System." + "ID: " + finalId;
}

string CourseRepositoryImpl::editCourse(const string& id, const Course& newData) {
  Course* c = findCourseById(id);
    if (!c){
      return "Course not found.";
      }

    c->setName(newData.getName());
    c->setCourseTeacherName(newData.getCourseTeacherName());
    c->setGrade(newData.getGrade());
    c->setSubjectHours(newData.getSubjectHours());

      return "Course data updated successfully.";

}

////////////////// StudentRepositoryImpl \\\\\\\\\\\\\\\

void StudentRepositoryImpl::addStudentInGrade(int grade, Student &student) {
    studentsInGrade[grade].push_back(student);
}

void StudentRepositoryImpl::addStudentInStage(int grade, Student &student) {
    Stage stage = getStageFromGrade(grade);
    studentsInStage[stage].push_back(student);
}

void StudentRepositoryImpl::addStudentInSchool(Student &student) {
    studentsInSchool.push_back(student);
}

Student* StudentRepositoryImpl::findStudentById(const string& id) {
    for (auto& [grade, students] : studentsInGrade) {
        for (auto& s : students) {
            if (s.getId() == id) {
                return &s;
            }
        }
    }
    return nullptr;
}


string StudentRepositoryImpl::addStudent(int grade, Student &student) {

    string finalId = generateStudentID(grade);
    student.setId(finalId);
    addStudentInGrade(grade, student);
    addStudentInStage(grade, student);
    addStudentInSchool(student);

    return "Student added successfully. Assigned ID: " + finalId;
}


string StudentRepositoryImpl::editStudent(const string& id, const Student& newData) {
    Student* s = findStudentById(id);
    if (!s) return "Student not found.";

    s->setName(newData.getName());
    s->setPhoneNumber(newData.getPhoneNumber());
    s->setGpa(newData.getGpa());
    s->setAge(newData.getAge());
    s->setSchoolYear(newData.getSchoolYear());

    return "Student data updated successfully.";
}
