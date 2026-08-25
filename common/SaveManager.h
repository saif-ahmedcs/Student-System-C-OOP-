#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include "../repositories/student/StudentRepository.h"
#include "../repositories/course/CourseRepository.h"
#include "../repositories/teacher/TeacherRepository.h"
#include "../common/SchoolConstants.h"
#include <iostream>

inline void saveAll(StudentRepository& studentRepo, CourseRepository& courseRepo, TeacherRepository& teacherRepo) {
    bool ok = true;
    if (!studentRepo.saveToFile(SchoolConstants::FILE_STUDENTS))
    {
        std::cout << "[ERROR] Failed to save student data.\n";
        ok = false;
    }
    if (!courseRepo.saveToFile(SchoolConstants::FILE_COURSES))
    {
        std::cout << "[ERROR] Failed to save course data.\n";
        ok = false;
    }
    if (!teacherRepo.saveToFile(SchoolConstants::FILE_TEACHERS))
    {
        std::cout << "[ERROR] Failed to save teacher data.\n";
        ok = false;
    }
    if (ok)
    {
        std::cout << "[Saved]\n";
    }
}

#endif
