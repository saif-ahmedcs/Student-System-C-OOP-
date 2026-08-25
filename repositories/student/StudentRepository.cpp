#include "StudentRepository.h"
#include "../../common/SchoolUtils.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

StudentRepositoryImpl::StudentRepositoryImpl() : BaseRepository<Student>() {
}

static const std::string gradeYear[13] = {
    "",
    "2019",
    "2018",
    "2017",
    "2016",
    "2015",
    "2014",
    "2013",
    "2012",
    "2011",
    "2010",
    "2009",
    "2008"
};

string StudentRepositoryImpl::generateStudentID(int grade) {
    if (grade < 1 || grade > 12)
        return "Invalid grade";

    string idStart = gradeYear[grade];

    idCounters[grade]++;

    return idStart + formatIdCounter(idCounters[grade]);
}


void StudentRepositoryImpl::syncStudentIDCounter(int grade, int maxSuffix) {
    while (idCounters[grade] < maxSuffix) {
        idCounters[grade]++;
    }
}

Student* StudentRepositoryImpl::findStudentByNationalNumber(const string& nn) {
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getNationalNumber() == nn) {
            return &allEntities[i];
        }
    }
    return nullptr;
}

Student* StudentRepositoryImpl::findStudentById(const string& id) {
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            return &allEntities[i];
        }
    }
    return nullptr;
}

int StudentRepositoryImpl::getStudentsInGrade(int grade) const {
    for (map<int, vector<int>>::const_iterator it = gradeIndex.begin(); it != gradeIndex.end(); ++it) {
        if (it->first == grade) {
            return (int)it->second.size();
        }
    }
    return 0;
}

vector<Student*> StudentRepositoryImpl::getStudentsByGrade(int grade) {
    vector<Student*> result;
    map<int, vector<int>>::iterator it = gradeIndex.find(grade);
    if (it == gradeIndex.end())
    {
        return result;
    }
    for (int i = 0; i < (int)it->second.size(); i++) {
        result.push_back(&allEntities[it->second[i]]);
    }
    return result;
}

int StudentRepositoryImpl::getMaxStudentsForGrade(int grade) const {
    return getMaxStudentsForStage(getStageFromGrade(grade));
}

string StudentRepositoryImpl::addStudent(int grade, Student& student) {
    string finalId = generateStudentID(grade);
    student.setId(finalId);

    int studentsInGrade = (int)gradeIndex[grade].size();
    int classNum = (studentsInGrade % SchoolConstants::CLASSES_PER_GRADE) + 1;
    student.setClassNumber(classNum);
    allEntities.reserve(allEntities.size() + 1);
    allEntities.push_back(student);
    int idx = (int)allEntities.size() - 1;
    gradeIndex[grade].push_back(idx);
    stageIndex[getStageFromGrade(grade)].push_back(idx);

    return "Student added successfully. Assigned ID: " + finalId;
}

string StudentRepositoryImpl::editStudent(const string& id, const Student& newData) {
    int idx = -1;
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
                idx = i; break;
        }
    }

    if (idx == -1) {
            return "Student not found.";
    }

    int oldGrade = allEntities[idx].getGrade();
    int newGrade = newData.getGrade();

    allEntities[idx].setName(newData.getName());
    allEntities[idx].setPhoneNumber(newData.getPhoneNumber());
    allEntities[idx].setGpa(newData.getGpa());
    allEntities[idx].setAge(newData.getAge());
    allEntities[idx].setGrade(newGrade);

    if (oldGrade != newGrade) {
        removeFromGradeIndex(oldGrade, idx);
        removeFromStageIndex(getStageFromGrade(oldGrade), idx);

        string newId = generateStudentID(newGrade);
        allEntities[idx].setId(newId);

        gradeIndex[newGrade].push_back(idx);
        stageIndex[getStageFromGrade(newGrade)].push_back(idx);
    }
    return "Student data updated successfully.";
}

string StudentRepositoryImpl::assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) {
    Student* student = findStudentById(studentId);
    if (!student) {
        return "Student not found.";
    }
    for (int i = 0; i < (int)courseIds.size(); i++) {
        student->assignCourse(courseIds[i], teacherNames[i]);
    }
    return "Courses assigned to student successfully.";
}

void StudentRepositoryImpl::clearStudentCourses(const string& studentId) {
    Student* student = findStudentById(studentId);
    if (student) {
        student->clearAssignedCourses();
    }
}

string StudentRepositoryImpl::removeStudent(const string& id) {
    int removeIndex = -1;
    for (int i = 0; i < (int)allEntities.size(); i++) {
        if (allEntities[i].getId() == id) {
            removeIndex = i; break;
        }
    }
    if (removeIndex == -1) {
        return "Student not found.";
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
            }
            else if (indices[i] > removeIndex) {
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
    return "Student removed successfully.";
}

bool StudentRepositoryImpl::saveToFile(const string& filename) {
    string tmp;
    ofstream f;
    if (!atomicSaveOpen(filename, f, tmp)) {
        return false;
    }

    f << allEntities.size() << "\n";
    for (int i = 0; i < (int)allEntities.size(); i++) {
        Student& s = allEntities[i];
        f << s.getId() << "\n";
        f << s.getName() << "\n";
        f << s.getNationalNumber() << "\n";
        f << s.getAge() << "\n";
        f << s.getGrade() << "\n";
        f << s.getClassNumber() << "\n";
        f << s.getGpa() << "\n";
        f << s.getPhoneNumber() << "\n";

        const vector<StudentCourse>& courses = s.getAssignedCourses();
        f << courses.size() << "\n";

        for (int j = 0; j < (int)courses.size(); j++) {
            f << courses[j].courseId << "\n";
            f << courses[j].teacherName << "\n";
        }
    }
    return atomicSaveFinish(f, tmp, filename);
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
        cout << "[WARNING] \"" << filename << "\" is corrupt or empty \xe2\x80\x94 starting fresh.\n";
        return;
    }

    int maxSuffixPerGrade[13] = {0};

    for (int i = 0; i < count; i++) {
        Student s;
        string id, name, nn, phone;
        int age, grade, classNumber;
        double gpa;

        getline(f, id);
        getline(f, name);
        getline(f, nn);

        f >> age;
        f.ignore();

        f >> grade;
        f.ignore();

        f >> classNumber;
        f.ignore();

        f >> gpa;
        f.ignore();

        getline(f, phone);

        if (f.fail()) {
            discardLoadedData(filename, i + 1);
            return;
        }

        s.setId(id);
        s.setName(name);
        s.setNationalNumber(nn);
        s.setAge(age);
        s.setGrade(grade);
        s.setClassNumber(classNumber);
        s.setGpa(gpa);
        s.setPhoneNumber(phone);

        int numCourses;
        f >> numCourses; f.ignore();
        for (int j = 0; j < numCourses; j++) {
            string cid, tname;
            getline(f, cid);
            getline(f, tname);
            s.assignCourse(cid, tname);
        }

        if (f.fail() && i < count - 1) {
            discardLoadedData(filename, i + 1);
            return;
        }

        allEntities.push_back(s);
        int idx = (int)allEntities.size() - 1;
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
