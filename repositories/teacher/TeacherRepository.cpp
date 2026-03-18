#include "TeacherRepository.h"
#include "../../common/SchoolConstants.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

static string generateTeacherID(int grade) {
    static int id[13] = {0};
    string idStart;
    if (grade >= 1 && grade <= 12) { idStart = to_string(grade); }
    else {
        return "Invalid grade";
    }

    id[grade]++;
    if (id[grade] <= 9) {
        return idStart + "00" + to_string(id[grade]);
    }
    else if (id[grade] <= 99) {
        return idStart + "0" + to_string(id[grade]);
    }
    else {
        return idStart + to_string(id[grade]);
    }
}

static void syncTeacherIDCounter(int grade, int maxSuffix) {
    string first = generateTeacherID(grade);
    int prefixLen = (int)to_string(grade).length();
    int current = stoi(first.substr(prefixLen));

    for (int i = current; i < maxSuffix; i++) {
        generateTeacherID(grade);
    }
}

int TeacherRepositoryImpl::getTeachersInGrade(int grade) const {

    map<int, vector<int>>::const_iterator it = gradeIndex.find(grade);

    if (it != gradeIndex.end()) {
        return (int)it->second.size();
    }
    return 0;
}

int TeacherRepositoryImpl::getMaxTeachersForGrade(int grade) const {

    switch (getStageFromGrade(grade)) {
        case Stage::Primary: {
            return SchoolConstants::MAX_TEACHERS_IN_PRIMARY;
         }
        case Stage::Middle: {
            return SchoolConstants::MAX_TEACHERS_IN_MIDDLE;
         }
        case Stage::Secondary: {
            return SchoolConstants::MAX_TEACHERS_IN_SECONDARY;
         }
    }
    return 0;
}

Teacher* TeacherRepositoryImpl::findTeacherByNationalNumber(const string& nationalNumber) {
    for (int i = 0; i < (int)allTeachers.size(); i++) {
        if (allTeachers[i].getNationalNumber() == nationalNumber) {
                return &allTeachers[i];
        }
    }
    return nullptr;
}

Teacher* TeacherRepositoryImpl::findTeacherById(const string& id) {
    for (int i = 0; i < (int)allTeachers.size(); i++) {
        if (allTeachers[i].getId() == id) {
            return &allTeachers[i];
        }
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
    if (idx == -1) {
        return "Teacher not found.";
    }

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
        map<int, vector<int>>::iterator git = gradeIndex.find(oldGrade);
        if (git != gradeIndex.end()) {
            vector<int>& indices = git->second;
            for (int i = 0; i < (int)indices.size(); i++) {
                if (indices[i] == idx) {
                    indices.erase(indices.begin() + i);
                    break;
                }
            }
            if (indices.empty()) { gradeIndex.erase(git); }
        }
        Stage oldStage = getStageFromGrade(oldGrade);
        map<Stage, vector<int>>::iterator sit = stageIndex.find(oldStage);
        if (sit != stageIndex.end()) {
            vector<int>& indices = sit->second;
            for (int i = 0; i < (int)indices.size(); i++) {
                if (indices[i] == idx) {
                    indices.erase(indices.begin() + i);
                    break;
                }
            }

            if (indices.empty()) { stageIndex.erase(sit); }
        }
        gradeIndex[newGrade].push_back(idx);
        stageIndex[getStageFromGrade(newGrade)].push_back(idx);
    }
    return "Teacher data updated successfully.";
}

string TeacherRepositoryImpl::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds) {
    Teacher* teacher = findTeacherById(teacherId);
    if (!teacher) {
        return "Teacher not found.";
    }
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

    map<int, vector<int>>::iterator git = gradeIndex.find(grade);
    if (git != gradeIndex.end()) {
        vector<int>& indices = git->second;
        for (int i = 0; i < (int)indices.size(); i++) {
            if (indices[i] == removeIndex) {
                indices.erase(indices.begin() + i); i--;
            }
            else if (indices[i] > removeIndex) {
                indices[i] = indices[i] - 1;
            }
        }
        if (indices.empty()) { gradeIndex.erase(git); }
    }

    map<Stage, vector<int>>::iterator sit = stageIndex.find(stage);
    if (sit != stageIndex.end()) {
        vector<int>& indices = sit->second;
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

bool TeacherRepositoryImpl::saveToFile(const string& filename) {
    string tmp = filename + ".tmp";
    ofstream f(tmp.c_str());
    if (!f) {
      return false;
    }

    f << allTeachers.size() << "\n";
    for (int i = 0; i < (int)allTeachers.size(); i++) {
        Teacher& t = allTeachers[i];
        f << t.getId() << "\n";
        f << t.getName() << "\n";
        f << t.getNationalNumber() << "\n";
        f << t.getAge() << "\n";
        f << t.getGrade() << "\n";
        f << t.getExperienceYears() << "\n";
        f << t.getMonthlySalary() << "\n";
        f << t.getSubject() << "\n";
        f << t.getSpecialization() << "\n";

        const vector<string>& courses = t.getAssignedCourses();
        f << courses.size() << "\n";

        for (int j = 0; j < (int)courses.size(); j++) {
            f << courses[j] << "\n";
        }
    }
    f.flush();
    if (!f.good()) {
        f.close(); remove(tmp.c_str());
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
            allTeachers.clear(); gradeIndex.clear(); stageIndex.clear();
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
            string cid; getline(f, cid); t.assignCourse(cid);
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
                if (suffix > maxSuffixPerGrade[grade]) { maxSuffixPerGrade[grade] = suffix; }
            }
        }
    }

    for (int g = 1; g <= 12; g++) {
        if (maxSuffixPerGrade[g] > 0) { syncTeacherIDCounter(g, maxSuffixPerGrade[g]); }
    }
}
