#include "CourseRepository.h"
#include "../../common/SchoolUtils.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

CourseRepositoryImpl::CourseRepositoryImpl() : BaseRepository<Course>() {
}

string CourseRepositoryImpl::generateCourseID(const string& courseName, int grade, const string& specialization) {
    string idStart = "";
    for (int i = 0; i < (int)courseName.length(); i++) {
        if (courseName[i] != ' ') {
            idStart += tolower(courseName[i]);
        }
    }
    idStart += '-';
    idStart += to_string(grade);
    idStart += '-';
    for (int i = 0; i < (int)specialization.length(); i++) {
        if (specialization[i] != ' ') {
            idStart += tolower(specialization[i]);
        }
    }

    idCounters[grade]++;
    return idStart + formatIdCounter(idCounters[grade]);
}

void CourseRepositoryImpl::syncCourseIDCounter(int grade, int maxSuffix) {
    while (idCounters[grade] < maxSuffix) {
        idCounters[grade]++;
    }
}

int CourseRepositoryImpl::getNumberOfCoursesInGrade(int grade) const {
    for (map<int, vector<int>>::const_iterator it = gradeIndex.begin(); it != gradeIndex.end(); ++it) {
        if (it->first == grade) {
            return (int)it->second.size();
        }
    }
    return 0;
}

vector<Course> CourseRepositoryImpl::getCoursesInSchoolVector() {
    return allEntities;
}

int CourseRepositoryImpl::getMaxCoursesForGrade(int grade) const {
    return getMaxCoursesForStage(getStageFromGrade(grade));
}

Course* CourseRepositoryImpl::findCourseById(const string& id) {
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            return &allEntities[i];
        }
    }
    return nullptr;
}

string CourseRepositoryImpl::addCourse(int grade, Course& course) {
    string finalId = generateCourseID(course.getName(), grade, course.getSpecialization());
    course.setId(finalId);
    allEntities.reserve(allEntities.size() + 1);
    allEntities.push_back(course);
    int idx = (int)allEntities.size() - 1;
    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);
    return "Course added successfully to grade " + to_string(grade) + ". ID: " + finalId;
}

string CourseRepositoryImpl::editCourse(const string& id, const Course& newData) {
    int removeIndex = -1;
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            removeIndex = i;
            break;
        }
    }
    if (removeIndex == -1) {
        return "Course not found.";
    }

    Course* c = &allEntities[removeIndex];
    int oldGrade = c->getGrade();
    int newGrade = newData.getGrade();

    if (oldGrade != newGrade) {
        Stage oldStage = getStageFromGrade(oldGrade);
        Stage newStage = getStageFromGrade(newGrade);

        removeFromGradeIndex(oldGrade, removeIndex);

        if (oldStage != newStage) {
            removeFromStageIndex(oldStage, removeIndex);
            stageIndex[newStage].push_back(removeIndex);
        }

        gradeIndex[newGrade].push_back(removeIndex);

        string newId = generateCourseID(newData.getName(), newGrade, newData.getSpecialization());
        c->setId(newId);
    }

    c->setName(newData.getName());
    c->setGrade(newGrade);
    c->setSubjectHours(newData.getSubjectHours());
    c->setSpecialization(newData.getSpecialization());
    return "Course data updated successfully. New ID: " + c->getId();
}

static string doAssignTeacherToCourse(Course* course, const string& courseId, const string& teacherId, const string& teacherName) {
    if (!course->assignTeacher(teacherId, teacherName)) {
        return "Error: Teacher already assigned to course " + courseId + ".";
    }
    return "Teacher assigned to course successfully.";
}

string CourseRepositoryImpl::assignTeacherToCourse(const string& courseId, const string& teacherId, const string& teacherName) {
    Course* course = findCourseById(courseId);
    if (!course) {
        return "Error: Course " + courseId + " not found.";
    }
    return doAssignTeacherToCourse(course, courseId, teacherId, teacherName);
}

string CourseRepositoryImpl::assignTeacherToCourseForReplace(const string& courseId, const string& teacherId, const string& teacherName) {
    Course* course = findCourseById(courseId);
    if (!course) {
        return "Error: Course " + courseId + " not found.";
    }
    return doAssignTeacherToCourse(course, courseId, teacherId, teacherName);
}

string CourseRepositoryImpl::assignTeacherToClassInCourse(const string& courseId, int classNum, const string& teacherId) {
    Course* course = findCourseById(courseId);
    if (!course) {
        return "Error: Course " + courseId + " not found.";
    }
    if (!course->assignTeacherToClass(classNum, teacherId)) {
        return "Error: Class " + to_string(classNum) + " already has a teacher in course " + courseId + ".";
    }
    return "Teacher assigned to class successfully.";
}

string CourseRepositoryImpl::assignStudentToCourse(const string& studentId, const string& courseId) {
    Course* course = findCourseById(courseId);
    if (!course) {
        return "Error: Course " + courseId + " not found.";
    }
    if (!course->assignStudent(studentId)) {
        return "Error: Student already assigned to course " + courseId + ".";
    }
    return "Student assigned to course successfully.";
}

string CourseRepositoryImpl::removeStudentFromCourse(const string& studentId, const string& courseId) {
    Course* course = findCourseById(courseId);
    if (!course) {
        return "Error: Course " + courseId + " not found.";
    }
    course->removeStudentById(studentId);
    return "Student removed from course successfully.";
}

string CourseRepositoryImpl::removeCourse(const string& id) {
    int removeIndex = -1;
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            removeIndex = i;
            break;
        }
    }
    if (removeIndex == -1) {
        return "Course not found.";
    }

    int grade = allEntities[removeIndex].getGrade();
    Stage stage = getStageFromGrade(grade);

    map<int, vector<int>>::iterator git = gradeIndex.find(grade);
    if (git != gradeIndex.end()) {
        vector<int>& indices = git->second;
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

    map<Stage, vector<int>>::iterator sit = stageIndex.find(stage);
    if (sit != stageIndex.end()) {
        vector<int>& indices = sit->second;
        for (int i = 0; i < (int)indices.size(); i++) {
            if (indices[i] == removeIndex) {
                indices.erase(indices.begin() + i);
                i--;
            } else if (indices[i] > removeIndex) {
                indices[i] = indices[i] - 1;
            }
        }
        if (indices.empty()) {
            stageIndex.erase(sit);
        }
    }

    allEntities.erase(allEntities.begin() + removeIndex);
    return "Course removed successfully.";
}

bool CourseRepositoryImpl::saveToFile(const string& filename) {
    string tmp;
    ofstream f;
    if (!atomicSaveOpen(filename, f, tmp)) {
        return false;
    }

    f << allEntities.size() << "\n";
    for (int i = 0; i < (int)allEntities.size(); i++) {
        Course& c = allEntities[i];
        f << c.getId() << "\n";
        f << c.getName() << "\n";
        f << c.getGrade() << "\n";
        f << c.getSubjectHours() << "\n";
        f << c.getSpecialization() << "\n";
        const std::vector<std::string> tids = c.getTeacherIds();
        const std::vector<std::string> tnames = c.getTeacherNames();
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
        const map<int, string>& ctm = c.getClassTeacherMap();
        f << ctm.size() << "\n";
        for (map<int, string>::const_iterator it = ctm.begin(); it != ctm.end(); ++it) {
            f << it->first << "\n";
            f << it->second << "\n";
        }
    }
    return atomicSaveFinish(f, tmp, filename);
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
        cout << "[WARNING] \"" << filename << "\" is corrupt or empty \xe2\x80\x94 starting fresh.\n";
        return;
    }

    int maxSuffixPerGrade[13] = {0};

    for (int i = 0; i < count; i++) {
        Course c;
        string id, name, spec;
        int grade, hours;

        getline(f, id);
        getline(f, name);
        f >> grade; f.ignore();
        f >> hours; f.ignore();
        getline(f, spec);

        if (f.fail()) {
            discardLoadedData(filename, i + 1);
            return;
        }

        c.setId(id);
        c.setName(name);
        c.setGrade(grade);
        c.setSubjectHours(hours);
        c.setSpecialization(spec);

        int numTeachers;
        f >> numTeachers; f.ignore();
        for (int j = 0; j < numTeachers; j++) {
            string tid, tname;
            getline(f, tid);
            getline(f, tname);
            c.assignTeacher(tid, tname);
        }

        int numStudents;
        f >> numStudents; f.ignore();
        for (int j = 0; j < numStudents; j++) {
            string sid;
            getline(f, sid);
            c.assignStudent(sid);
        }

        int numClassEntries;
        f >> numClassEntries; f.ignore();
        for (int j = 0; j < numClassEntries; j++) {
            int classNum;
            string tid;
            f >> classNum; f.ignore();
            getline(f, tid);
            c.assignTeacherToClass(classNum, tid);
        }

        if (f.fail() && i < count - 1) {
            discardLoadedData(filename, i + 1);
            return;
        }

        allEntities.push_back(c);
        int idx = (int)allEntities.size() - 1;
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
