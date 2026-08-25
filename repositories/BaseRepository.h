#ifndef BASE_REPOSITORY_H
#define BASE_REPOSITORY_H

#include "../common/SchoolUtils.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

template<typename T>
class BaseRepository {
protected:
    std::vector<T> allEntities;
    std::map<int, std::vector<int>> gradeIndex;
    std::map<Stage, std::vector<int>> stageIndex;
    int idCounters[13];

    void removeFromGradeIndex(int grade, int idx) {
        typename std::map<int, std::vector<int>>::iterator git = gradeIndex.find(grade);
        if (git != gradeIndex.end()) {
            std::vector<int>& indices = git->second;
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
    }

    void removeFromStageIndex(Stage stage, int idx) {
        typename std::map<Stage, std::vector<int>>::iterator sit = stageIndex.find(stage);
        if (sit != stageIndex.end()) {
            std::vector<int>& indices = sit->second;
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
    }

    void discardLoadedData(const std::string& filename, int recordNum) {
        std::cout << "[WARNING] \"" << filename << "\" is corrupt at record " << recordNum << " \xe2\x80\x94 discarding loaded data.\n";
        allEntities.clear();
        gradeIndex.clear();
        stageIndex.clear();
    }

    bool atomicSaveOpen(const std::string& filename, std::ofstream& f, std::string& tmp) {
        tmp = filename + ".tmp";
        f.open(tmp.c_str());
        if (!f) {
            return false;
        }
        return true;
    }

    bool atomicSaveFinish(std::ofstream& f, const std::string& tmp, const std::string& filename) {
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

public:
    BaseRepository() {
        for (int i = 0; i < 13; i++) {
            idCounters[i] = 0;
        }
    }

    virtual ~BaseRepository() = default;
};

#endif
