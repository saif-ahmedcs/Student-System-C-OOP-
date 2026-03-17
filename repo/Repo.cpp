#include "Repo.h"
#include <iostream>
#include "../SchoolConstants.h"
#include <string>
#include <fstream>
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


void syncStudentIDCounter(int grade, int maxSuffix) {
    string prefix;
    if (grade == 1) {
        prefix = "2019";
    }
    else if (grade == 2) {
        prefix = "2018";
    }
    else if (grade == 3) {
        prefix = "2017";
    }
    else if (grade == 4) {
        prefix = "2016";
    }
    else if (grade == 5) {
        prefix = "2015";
    }
    else if (grade == 6) {
        prefix = "2014";
    }
    else if (grade == 7) {
        prefix = "2013";
    }
    else if (grade == 8) {
        prefix = "2012";
    }
    else if (grade == 9) {
        prefix = "2011";
    }
    else if (grade == 10) {
        prefix = "2010";
    }
    else if (grade == 11) {
        prefix = "2009";
    }
    else if (grade == 12) {
        prefix = "2008";
    }
    else {
        return;
    }

    string first = generateStudentID(grade);
    int current = stoi(first.substr(prefix.length()));

    for (int i = current; i < maxSuffix; i++) {
        generateStudentID(grade);
    }
}

void syncTeacherIDCounter(int grade, int maxSuffix) {
    string first = generateTeacherID(grade);
    int prefixLen = (int)to_string(grade).length();
    int current = stoi(first.substr(prefixLen));

    for (int i = current; i < maxSuffix; i++) {
        generateTeacherID(grade);
    }
}

void syncCourseIDCounter(int grade, int maxSuffix) {
    string first = generateCourseID("x", grade, "x");

    int start = (int)first.length() - 1;
    while (start > 0 && first[start - 1] >= '0' && first[start - 1] <= '9') {
        start--;
    }
    int current = stoi(first.substr(start));

    for (int i = current; i < maxSuffix; i++) {
        generateCourseID("x", grade, "x");
    }
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

    allTeachers.reserve(allTeachers.size() + 1);
    allTeachers.push_back(teacher);
    int idx = (int)allTeachers.size() - 1;

    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);

    return "Teacher added successfully. Assigned ID: " + finalId;
}

string TeacherRepositoryImpl::editTeacher(const string& id, const Teacher& newData) {
    int idx = -1;
    for (int i = 0; i < (int)allTeachers.size(); i++) {
        if (allTeachers[i].getId() == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1)
        return "Teacher not found.";

    int oldGrade = allTeachers[idx].getGrade();
    int newGrade = newData.getGrade();

    allTeachers[idx].setName(newData.getName());
    allTeachers[idx].setAge(newData.getAge());
    allTeachers[idx].setGrade(newGrade);
    allTeachers[idx].setSubject(newData.getSubject());
    allTeachers[idx].setExperienceYears(newData.getExperienceYears());
    allTeachers[idx].setMonthlySalary(newData.getMonthlySalary());
    allTeachers[idx].setSpecialization(newData.getSpecialization());

    if (oldGrade != newGrade) {
        std::map<int, std::vector<int>>::iterator git = gradeIndex.find(oldGrade);
        if (git != gradeIndex.end()) {
            std::vector<int>& indices = git->second;
            for (int i = 0; i < (int)indices.size(); i++) {
                if (indices[i] == idx) {
                    indices.erase(indices.begin() + i);
                    break;
                }
            }
            if (indices.empty())
                gradeIndex.erase(git);
        }

        Stage oldStage = getStageFromGrade(oldGrade);

        std::map<Stage, std::vector<int>>::iterator sit = stageIndex.find(oldStage);
        if (sit != stageIndex.end()) {
            std::vector<int>& indices = sit->second;
            for (int i = 0; i < (int)indices.size(); i++) {
                if (indices[i] == idx) {
                    indices.erase(indices.begin() + i);
                    break;
                }
            }
            if (indices.empty())
                stageIndex.erase(sit);
        }

        gradeIndex[newGrade].push_back(idx);
        stageIndex[getStageFromGrade(newGrade)].push_back(idx);
    }

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

    allCourses.reserve(allCourses.size() + 1);
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
    c->setSpecialization(newData.getSpecialization());

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

string CourseRepositoryImpl::removeStudentFromCourse(const string& studentId, const string& courseId) {
    Course* course = findCourseById(courseId);
    if (!course)
        return "Error: Course " + courseId + " not found.";

    course->removeStudentById(studentId);

    return "Student removed from course successfully.";
}

string CourseRepositoryImpl::removeCourse(const string& id) {
    int removeIndex = -1;
    for (int i = 0; i < (int)allCourses.size(); i++) {
        if (allCourses[i].getId() == id) {
            removeIndex = i;
            break;
        }
    }
    if (removeIndex == -1)
        return "Course not found.";

    int grade = allCourses[removeIndex].getGrade();
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
        if (indices.empty())
            gradeIndex.erase(git);
    }

    std::map<Stage, std::vector<int>>::iterator sit = stageIndex.find(stage);
    if (sit != stageIndex.end()) {
        std::vector<int>& indices = sit->second;
        for (int i = 0; i < (int)indices.size(); i++) {
            if (indices[i] == removeIndex) {
                indices.erase(indices.begin() + i);
                i--;
            } else if (indices[i] > removeIndex) {
                indices[i] = indices[i] - 1;
            }
        }
        if (indices.empty())
            stageIndex.erase(sit);
    }

    allCourses.erase(allCourses.begin() + removeIndex);
    return "Course removed successfully.";
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

    allStudents.reserve(allStudents.size() + 1);
    allStudents.push_back(student);
    int idx = (int)allStudents.size() - 1;

    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);

    return "Student added successfully. Assigned ID: " + finalId;
}

string StudentRepositoryImpl::editStudent(const string& id, const Student& newData) {
    int idx = -1;
    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i].getId() == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1)
        return "Student not found.";

    int oldGrade = allStudents[idx].getGrade();
    int newGrade = newData.getGrade();

    allStudents[idx].setName(newData.getName());
    allStudents[idx].setPhoneNumber(newData.getPhoneNumber());
    allStudents[idx].setGpa(newData.getGpa());
    allStudents[idx].setAge(newData.getAge());
    allStudents[idx].setGrade(newGrade);

    if (oldGrade != newGrade) {
        std::map<int, std::vector<int>>::iterator git = gradeIndex.find(oldGrade);
        if (git != gradeIndex.end()) {
            std::vector<int>& indices = git->second;
            for (int i = 0; i < (int)indices.size(); i++) {
                if (indices[i] == idx) {
                    indices.erase(indices.begin() + i);
                    break;
                }
            }
            if (indices.empty())
                gradeIndex.erase(git);
        }

        Stage oldStage = getStageFromGrade(oldGrade);
        std::map<Stage, std::vector<int>>::iterator sit = stageIndex.find(oldStage);
        if (sit != stageIndex.end()) {
            std::vector<int>& indices = sit->second;
            for (int i = 0; i < (int)indices.size(); i++) {
                if (indices[i] == idx) {
                    indices.erase(indices.begin() + i);
                    break;
                }
            }
            if (indices.empty())
                stageIndex.erase(sit);
        }

        gradeIndex[newGrade].push_back(idx);
        stageIndex[getStageFromGrade(newGrade)].push_back(idx);
    }

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

string StudentRepositoryImpl::removeStudent(const string& id) {

    int removeIndex = -1;

    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i].getId() == id) {
            removeIndex = i;
            break;
        }
    }
    if (removeIndex == -1)
        return "Student not found.";

    int grade = allStudents[removeIndex].getGrade();
    Stage stage = getStageFromGrade(grade);

    std::map<int, std::vector<int>>::iterator git = gradeIndex.find(grade);

    if (git != gradeIndex.end()) {
        std::vector<int>& indices = git->second;

        for (int i = 0; i < (int)indices.size(); i++) {
            if (indices[i] == removeIndex) {
                indices.erase(indices.begin() + i);
                i--;
            }
            else if (indices[i] > removeIndex) {
                indices[i] = indices[i] - 1;
            }
        }


        if (indices.empty())
            gradeIndex.erase(git);
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

        if (indices.empty())
            stageIndex.erase(sit);
    }

    allStudents.erase(allStudents.begin() + removeIndex);

    return "Student removed successfully.";
}
// ─────────────────────────────────────────────
//  TeacherRepositoryImpl  - file persistence
// ─────────────────────────────────────────────
bool TeacherRepositoryImpl::saveToFile(const string& filename) {
    string tmp = filename + ".tmp";
    ofstream f(tmp.c_str());
    if (!f) {
        return false;
    }

    f << allTeachers.size() << "\n";

    for (int i = 0; i < (int)allTeachers.size(); i++) {
        Teacher& t = allTeachers[i];
        f << t.getId()             << "\n";
        f << t.getName()           << "\n";
        f << t.getNationalNumber() << "\n";
        f << t.getAge()            << "\n";
        f << t.getGrade()          << "\n";
        f << t.getExperienceYears()<< "\n";
        f << t.getMonthlySalary()  << "\n";
        f << t.getSubject()        << "\n";
        f << t.getSpecialization() << "\n";

        const vector<string>& courses = t.getAssignedCourses();
        f << courses.size() << "\n";
        for (int j = 0; j < (int)courses.size(); j++) {
            f << courses[j] << "\n";
        }
    }

    f.flush();
    if (!f.good()) {
        f.close();
        remove(tmp.c_str());
        return false;
    }

    f.close();
    remove(filename.c_str());
    return rename(tmp.c_str(), filename.c_str()) == 0;
}

void TeacherRepositoryImpl::loadFromFile(const string& filename) {
    ifstream f(filename.c_str());
    if (!f) {
        return;
    }

    int count;
    f >> count;
    f.ignore();

    if (f.fail()) {
        cout << "[WARNING] \"" << filename << "\" is corrupt or empty — starting fresh.\n";
        return;
    }

    int maxSuffixPerGrade[13] = {0};

    for (int i = 0; i < count; i++) {
        Teacher t;
        string id, name, nn, subject, spec;
        int age, grade, exp;
        double salary;

        getline(f, id);
        getline(f, name);
        getline(f, nn);

        f >> age;
        f.ignore();

        f >> grade;
        f.ignore();

        f >> exp;
        f.ignore();

        f >> salary;
        f.ignore();

        getline(f, subject);
        getline(f, spec);

        if (f.fail()) {
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allTeachers.clear();
            gradeIndex.clear();
            stageIndex.clear();
            return;
        }

        t.setId(id);
        t.setName(name);
        t.setNationalNumber(nn);
        t.setAge(age);
        t.setGrade(grade);
        t.setExperienceYears(exp);
        t.setMonthlySalary(salary);
        t.setSubject(subject);
        t.setSpecialization(spec);

        int numCourses;
        f >> numCourses;
        f.ignore();

        for (int j = 0; j < numCourses; j++) {
            string cid;
            getline(f, cid);
            t.assignCourse(cid);
        }

        if (f.fail() && i < count - 1) {
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allTeachers.clear();
            gradeIndex.clear();
            stageIndex.clear();
            return;
        }

        allTeachers.push_back(t);
        int idx = (int)allTeachers.size() - 1;
        gradeIndex[grade].push_back(idx);
        stageIndex[getStageFromGrade(grade)].push_back(idx);

        if (grade >= 1 && grade <= 12) {
            int prefixLen = (int)to_string(grade).length();
            if ((int)id.length() > prefixLen) {
                int suffix = stoi(id.substr(prefixLen));
                if (suffix > maxSuffixPerGrade[grade]) {
                    maxSuffixPerGrade[grade] = suffix;
                }
            }
        }
    }

    for (int g = 1; g <= 12; g++) {
        if (maxSuffixPerGrade[g] > 0) {
            syncTeacherIDCounter(g, maxSuffixPerGrade[g]);
        }
    }
}

// ─────────────────────────────────────────────
//  CourseRepositoryImpl  - file persistence
// ─────────────────────────────────────────────
bool CourseRepositoryImpl::saveToFile(const string& filename) {
    string tmp = filename + ".tmp";
    ofstream f(tmp.c_str());
    if (!f) {
        return false;
    }

    f << allCourses.size() << "\n";

    for (int i = 0; i < (int)allCourses.size(); i++) {
        Course& c = allCourses[i];
        f << c.getId() << "\n";
        f << c.getName() << "\n";
        f << c.getGrade() << "\n";
        f << c.getSubjectHours() << "\n";
        f << c.getSpecialization() << "\n";

        const vector<string>& tids = c.getTeacherIds();
        const vector<string>& tnames = c.getTeacherNames();

        f << tids.size() << "\n";
        for (int j = 0; j < (int)tids.size(); j++) {
            f << tids[j] << "\n";
            f << tnames[j] << "\n";
        }

        const vector<string>& students = c.getAssignedStudents();
        f << students.size() << "\n";
        for (int j = 0; j < (int)students.size(); j++) {
            f << students[j] << "\n";
        }
    }

    f.flush();
    if (!f.good()) {
        f.close();
        remove(tmp.c_str());
        return false;
    }

    f.close();
    remove(filename.c_str());
    return rename(tmp.c_str(), filename.c_str()) == 0;
}

void CourseRepositoryImpl::loadFromFile(const string& filename) {
    ifstream f(filename.c_str());
    if (!f) {
        return;
    }

    int count;
    f >> count;
    f.ignore();

    if (f.fail()) {
        cout << "[WARNING] \"" << filename << "\" is corrupt or empty — starting fresh.\n";
        return;
    }

    int maxSuffixPerGrade[13] = {0};

    for (int i = 0; i < count; i++) {
        Course c;
        string id, name, spec;
        int grade, hours;

        getline(f, id);
        getline(f, name);

        f >> grade;
        f.ignore();

        f >> hours;
        f.ignore();

        getline(f, spec);

        if (f.fail()) {
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allCourses.clear();
            gradeIndex.clear();
            stageIndex.clear();
            return;
        }

        c.setId(id);
        c.setName(name);
        c.setGrade(grade);
        c.setSubjectHours(hours);
        c.setSpecialization(spec);

        int numTeachers;
        f >> numTeachers;
        f.ignore();

        for (int j = 0; j < numTeachers; j++) {
            string tid, tname;
            getline(f, tid);
            getline(f, tname);
            c.assignTeacher(tid, tname);
        }

        int numStudents;
        f >> numStudents;
        f.ignore();

        for (int j = 0; j < numStudents; j++) {
            string sid;
            getline(f, sid);
            c.assignStudent(sid);
        }

        if (f.fail() && i < count - 1) {
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allCourses.clear();
            gradeIndex.clear();
            stageIndex.clear();
            return;
        }

        allCourses.push_back(c);
        int idx = (int)allCourses.size() - 1;
        gradeIndex[grade].push_back(idx);
        stageIndex[getStageFromGrade(grade)].push_back(idx);

        if (grade >= 1 && grade <= 12 && !id.empty()) {
            int start = (int)id.length() - 1;
            while (start > 0 && id[start - 1] >= '0' && id[start - 1] <= '9') {
                start--;
            }
            int suffix = stoi(id.substr(start));
            if (suffix > maxSuffixPerGrade[grade]) {
                maxSuffixPerGrade[grade] = suffix;
            }
        }
    }

    for (int g = 1; g <= 12; g++) {
        if (maxSuffixPerGrade[g] > 0) {
            syncCourseIDCounter(g, maxSuffixPerGrade[g]);
        }
    }
}

// ─────────────────────────────────────────────
//  StudentRepositoryImpl  - file persistence
// ─────────────────────────────────────────────
bool StudentRepositoryImpl::saveToFile(const string& filename) {
    string tmp = filename + ".tmp";
    ofstream f(tmp.c_str());
    if (!f) {
        return false;
    }


    f << allStudents.size() << "\n";

    for (int i = 0; i < (int)allStudents.size(); i++) {
        Student& s = allStudents[i];
        f << s.getId() << "\n";
        f << s.getName() << "\n";
        f << s.getNationalNumber() << "\n";
        f << s.getAge() << "\n";
        f << s.getGrade() << "\n";
        f << s.getGpa() << "\n";
        f << s.getPhoneNumber() << "\n";

        const vector<StudentCourse>& courses = s.getAssignedCourses();
        f << courses.size() << "\n";
        for (int j = 0; j < (int)courses.size(); j++) {
            f << courses[j].courseId << "\n";
            f << courses[j].teacherName << "\n";
        }
    }

    f.flush();
    if (!f.good()) {
        f.close();
        remove(tmp.c_str());
        return false;
    }

    f.close();
    remove(filename.c_str());
    return rename(tmp.c_str(), filename.c_str()) == 0;
}

void StudentRepositoryImpl::loadFromFile(const string& filename) {
    ifstream f(filename.c_str());
    if (!f) {
        return;
    }

    int count;
    f >> count;
    f.ignore();

    if (f.fail()) {
        cout << "[WARNING] \"" << filename << "\" is corrupt or empty — starting fresh.\n";
        return;
    }

    int maxSuffixPerGrade[13] = {0};

    for (int i = 0; i < count; i++) {
        Student s;
        string id, name, nn, phone;
        int age, grade;
        double gpa;

        getline(f, id);
        getline(f, name);
        getline(f, nn);
        f >> age;
        f.ignore();

        f >> grade;
        f.ignore();

        f >> gpa;
        f.ignore();

        getline(f, phone);

        if (f.fail()) {
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allStudents.clear();
            gradeIndex.clear();
            stageIndex.clear();
            return;
        }

        s.setId(id);
        s.setName(name);
        s.setNationalNumber(nn);
        s.setAge(age);
        s.setGrade(grade);
        s.setGpa(gpa);
        s.setPhoneNumber(phone);

        int numCourses;
        f >> numCourses;
        f.ignore();

        for (int j = 0; j < numCourses; j++) {
            string cid, tname;
            getline(f, cid);
            getline(f, tname);
            s.assignCourse(cid, tname);
        }

        if (f.fail() && i < count - 1) {
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allStudents.clear();
            gradeIndex.clear();
            stageIndex.clear();
            return;
        }

        allStudents.push_back(s);
        int idx = (int)allStudents.size() - 1;
        gradeIndex[grade].push_back(idx);
        stageIndex[getStageFromGrade(grade)].push_back(idx);

        if (grade >= 1 && grade <= 12 && (int)id.length() > 4) {
            int suffix = stoi(id.substr(4));
            if (suffix > maxSuffixPerGrade[grade]) {
                maxSuffixPerGrade[grade] = suffix;
            }
        }
    }

    for (int g = 1; g <= 12; g++) {
        if (maxSuffixPerGrade[g] > 0) {
            syncStudentIDCounter(g, maxSuffixPerGrade[g]);
        }
    }
}
