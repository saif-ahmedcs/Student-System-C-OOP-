#include "CourseRepository.h"
#include "../../common/SchoolConstants.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

static string generateCourseID(const string& courseName, int grade, const string& specialization) {
    static int id[13] = {0};
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

    id[grade]++;
    int count = id[grade];
    if (count <= 9) {
        return idStart + "00" + to_string(count);
    } else if (count <= 99) {
        return idStart + "0" + to_string(count);
    } else {
        return idStart + to_string(count);
    }
}

static void syncCourseIDCounter(int grade, int maxSuffix) {
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

int CourseRepositoryImpl::getNumberOfCoursesInGrade(int grade) const {
    for (map<int, vector<int>>::const_iterator it = gradeIndex.begin(); it != gradeIndex.end(); ++it) {
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
        case Stage::Primary: {
            return SchoolConstants::MAX_COURSES_IN_PRIMARY;
        }
        case Stage::Middle: {
            return SchoolConstants::MAX_COURSES_IN_MIDDLE;
        }
        case Stage::Secondary: {
            return SchoolConstants::MAX_COURSES_IN_SECONDARY;
        }
    }
    return 0;
}

Course* CourseRepositoryImpl::findCourseById(const string& id) {
    for (int i = 0; i < (int)allCourses.size(); i++) {
        if (allCourses[i].getId() == id) {
            return &allCourses[i];
        }
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
    if (!c) {
        return "Course not found.";
    }
    c->setName(newData.getName());
    c->setGrade(newData.getGrade());
    c->setSubjectHours(newData.getSubjectHours());
    c->setSpecialization(newData.getSpecialization());
    return "Course data updated successfully.";
}

string CourseRepositoryImpl::assignTeacherToCourse(const string& courseId, const string& teacherId, const string& teacherName) {
    Course* course = findCourseById(courseId);
    if (!course) {
        return "Error: Course " + courseId + " not found.";
    }
    if (!course->assignTeacher(teacherId, teacherName)) {
        return "Error: Teacher already assigned to course " + courseId + ".";
    }
    return "Teacher assigned to course successfully.";
}

string CourseRepositoryImpl::assignTeacherToCourseForReplace(const string& courseId, const string& teacherId, const string& teacherName) {
    Course* course = findCourseById(courseId);
    if (!course) {
        return "Error: Course " + courseId + " not found.";
    }
    if (!course->assignTeacher(teacherId, teacherName)) {
        return "Error: Teacher already assigned to course " + courseId + ".";
    }
    return "Teacher assigned to course successfully.";
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
    for (int i = 0; i < (int)allCourses.size(); i++) {
        if (allCourses[i].getId() == id) {
            removeIndex = i;
            break;
        }
    }
    if (removeIndex == -1) {
        return "Course not found.";
    }

    int grade = allCourses[removeIndex].getGrade();
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

    allCourses.erase(allCourses.begin() + removeIndex);
    return "Course removed successfully.";
}

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
        const map<int, string>& ctm = c.getClassTeacherMap();
        f << ctm.size() << "\n";
        for (map<int, string>::const_iterator it = ctm.begin(); it != ctm.end(); ++it) {
            f << it->first << "\n";
            f << it->second << "\n";
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
        f >> grade; f.ignore();
        f >> hours; f.ignore();
        getline(f, spec);

        if (f.fail()) {
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allCourses.clear(); gradeIndex.clear(); stageIndex.clear();
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
            cout << "[WARNING] \"" << filename << "\" is corrupt at record " << (i + 1) << " — discarding loaded data.\n";
            allCourses.clear(); gradeIndex.clear(); stageIndex.clear();
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
