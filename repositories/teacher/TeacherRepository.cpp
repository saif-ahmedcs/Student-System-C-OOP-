#include "TeacherRepository.h"
#include "../../common/SchoolUtils.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

TeacherRepositoryImpl::TeacherRepositoryImpl() : BaseRepository<Teacher>() {
}

string TeacherRepositoryImpl::generateTeacherID(int grade) {
    if (grade < 1 || grade > 12) {
        return "Invalid grade";
    }
    idCounters[grade]++;
    string idStart = to_string(grade);
    return idStart + formatIdCounter(idCounters[grade]);
}

void TeacherRepositoryImpl::syncTeacherIDCounter(int grade, int maxSuffix) {
    int prefixLen = (int)to_string(grade).length();
    while (idCounters[grade] < maxSuffix) {
        generateTeacherID(grade);
    }
    (void)prefixLen;
}

int TeacherRepositoryImpl::getTeachersInGrade(int grade) const {

    map<int, vector<int>>::const_iterator it = gradeIndex.find(grade);

    if (it != gradeIndex.end()) {
        return (int)it->second.size();
    }
    return 0;
}

vector<Teacher*> TeacherRepositoryImpl::getTeachersByGrade(int grade) {
    vector<Teacher*> result;
    map<int, vector<int>>::iterator it = gradeIndex.find(grade);
    if (it == gradeIndex.end())
    {
        return result;
    }
    for (int i = 0; i < (int)it->second.size(); i++)
    {
        result.push_back(&allEntities[it->second[i]]);
    }
    return result;
}

int TeacherRepositoryImpl::getMaxTeachersForGrade(int grade) const {
    return getMaxTeachersForStage(getStageFromGrade(grade));
}

Teacher* TeacherRepositoryImpl::findTeacherByNationalNumber(const string& nationalNumber) {
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getNationalNumber() == nationalNumber) {
                return &allEntities[i];
        }
    }
    return nullptr;
}

Teacher* TeacherRepositoryImpl::findTeacherById(const string& id) {
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            return &allEntities[i];
        }
    }
    return nullptr;
}

string TeacherRepositoryImpl::addTeacher(int grade, Teacher& teacher) {
    string finalId = generateTeacherID(grade);
    teacher.setId(finalId);
    allEntities.reserve(allEntities.size() + 1);
    allEntities.push_back(teacher);
    int idx = (int)allEntities.size() - 1;
    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);
    return "Teacher added successfully. Assigned ID: " + finalId;
}

string TeacherRepositoryImpl::editTeacher(const string& id, const Teacher& newData) {
    int idx = -1;
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return "Teacher not found.";
    }

    int oldGrade = allEntities[idx].getGrade();
    int newGrade = newData.getGrade();

    allEntities[idx].setName(newData.getName());
    allEntities[idx].setAge(newData.getAge());
    allEntities[idx].setGrade(newGrade);
    allEntities[idx].setSubject(newData.getSubject());
    allEntities[idx].setExperienceYears(newData.getExperienceYears());
    allEntities[idx].setMonthlySalary(newData.getMonthlySalary());
    allEntities[idx].setSpecialization(newData.getSpecialization());

    if (oldGrade != newGrade) {
        removeFromGradeIndex(oldGrade, idx);
        removeFromStageIndex(getStageFromGrade(oldGrade), idx);

        gradeIndex[newGrade].push_back(idx);
        stageIndex[getStageFromGrade(newGrade)].push_back(idx);

        string newId = generateTeacherID(newGrade);
        allEntities[idx].setId(newId);
        return "Teacher data updated successfully. New ID: " + newId;
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
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            removeIndex = i;
            break;
       }
    }
    if (removeIndex == -1) {
        return "Teacher not found.";
    }

    int grade = allEntities[removeIndex].getGrade();
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

    allEntities.erase(allEntities.begin() + removeIndex);

    return "Teacher removed successfully.";
}

bool TeacherRepositoryImpl::saveToFile(const string& filename) {
    string tmp;
    ofstream f;
    if (!atomicSaveOpen(filename, f, tmp)) {
      return false;
    }

    f << allEntities.size() << "\n";
    for (int i = 0; i < (int)allEntities.size(); i++) {
        Teacher& t = allEntities[i];
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
    return atomicSaveFinish(f, tmp, filename);
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
        cout << "[WARNING] \"" << filename << "\" is corrupt or empty \xe2\x80\x94 starting fresh.\n";
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
            discardLoadedData(filename, i + 1);
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
            discardLoadedData(filename, i + 1);
            return;
        }

        allEntities.push_back(t);
        int idx = (int)allEntities.size() - 1;
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
