#include "StudentRepository.h"
#include "../../common/SchoolConstants.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

static string generateStudentID(int grade) {
    static int id[13] = {0};
    string idStart;
    if (grade == 1) { idStart = "2019"; }
    else if (grade == 2) { idStart = "2018"; }
    else if (grade == 3) { idStart = "2017"; }
    else if (grade == 4) { idStart = "2016"; }
    else if (grade == 5) { idStart = "2015"; }
    else if (grade == 6) { idStart = "2014"; }
    else if (grade == 7) { idStart = "2013"; }
    else if (grade == 8) { idStart = "2012"; }
    else if (grade == 9) { idStart = "2011"; }
    else if (grade == 10) { idStart = "2010"; }
    else if (grade == 11) { idStart = "2009"; }
    else if (grade == 12) { idStart = "2008"; }
    else { return "Invalid grade"; }

    if (id[grade] >= 0 && id[grade] <= 9) {
        id[grade]++;
        return idStart + "00" + to_string(id[grade]);
    } else if (id[grade] >= 10 && id[grade] <= 99) {
        id[grade]++;
        return idStart + "0" + to_string(id[grade]);
    } else {
        id[grade]++;
        return idStart + to_string(id[grade]);
    }
}

static void syncStudentIDCounter(int grade, int maxSuffix) {
    string prefix;
    if (grade == 1) { prefix = "2019"; }
    else if (grade == 2) { prefix = "2018"; }
    else if (grade == 3) { prefix = "2017"; }
    else if (grade == 4) { prefix = "2016"; }
    else if (grade == 5) { prefix = "2015"; }
    else if (grade == 6) { prefix = "2014"; }
    else if (grade == 7) { prefix = "2013"; }
    else if (grade == 8) { prefix = "2012"; }
    else if (grade == 9) { prefix = "2011"; }
    else if (grade == 10) { prefix = "2010"; }
    else if (grade == 11) { prefix = "2009"; }
    else if (grade == 12) { prefix = "2008"; }
    else { return; }

    string first = generateStudentID(grade);
    int current = stoi(first.substr(prefix.length()));
    for (int i = current; i < maxSuffix; i++) {
        generateStudentID(grade);
    }
}

Student* StudentRepositoryImpl::findStudentByNationalNumber(const string& nn) {
    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i].getNationalNumber() == nn) {
            return &allStudents[i];
        }
    }
    return nullptr;
}

Student* StudentRepositoryImpl::findStudentById(const string& id) {
    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i].getId() == id) {
            return &allStudents[i];
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
        result.push_back(&allStudents[it->second[i]]);
    }
    return result;
}

int StudentRepositoryImpl::getMaxStudentsForGrade(int grade) const {

    switch (getStageFromGrade(grade)) {
        case Stage::Primary: {
           return SchoolConstants::MAX_STUDENTS_IN_PRIMARY;
        }
        case Stage::Middle: {
           return SchoolConstants::MAX_STUDENTS_IN_MIDDLE;
        }
        case Stage::Secondary: {
           return SchoolConstants::MAX_STUDENTS_IN_SECONDARY;
        }
    }

    return 0;
}

string StudentRepositoryImpl::addStudent(int grade, Student& student) {
    string finalId = generateStudentID(grade);
    student.setId(finalId);

    int studentsInGrade = (int)gradeIndex[grade].size();
    int classNum = (studentsInGrade % SchoolConstants::CLASSES_PER_GRADE) + 1;
    student.setClassNumber(classNum);
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
                idx = i; break;
        }
    }

    if (idx == -1) {
            return "Student not found.";
    }

    int oldGrade = allStudents[idx].getGrade();
    int newGrade = newData.getGrade();

    allStudents[idx].setName(newData.getName());
    allStudents[idx].setPhoneNumber(newData.getPhoneNumber());
    allStudents[idx].setGpa(newData.getGpa());
    allStudents[idx].setAge(newData.getAge());
    allStudents[idx].setGrade(newGrade);

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
            if (indices.empty()) {
                gradeIndex.erase(git);
            }
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
            if (indices.empty()) {
                stageIndex.erase(sit);
            }
        }

        string newId = generateStudentID(newGrade);
        allStudents[idx].setId(newId);

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
    for (int i = 0; i < (int)allStudents.size(); i++) {
        if (allStudents[i].getId() == id) {
            removeIndex = i; break;
        }
    }
    if (removeIndex == -1) {
        return "Student not found.";
    }

    int grade = allStudents[removeIndex].getGrade();
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

    allStudents.erase(allStudents.begin() + removeIndex);
    return "Student removed successfully.";
}

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
