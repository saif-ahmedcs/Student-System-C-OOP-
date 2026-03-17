#ifndef SCHOOL_CONSTANTS_H
#define SCHOOL_CONSTANTS_H

// ─────────────────────────────────────────────────────────────────────────
//  SchoolConstants
//
//  Single source of truth for all school policy numbers.
//  Any business rule change is made here only.
// ─────────────────────────────────────────────────────────────────────────

namespace SchoolConstants {

    // ── Grade range ───────────────────────────────────────────────────────
    constexpr int MIN_GRADE = 1;
    constexpr int MAX_GRADE = 12;

    // ── Assignment limits ─────────────────────────────────────────────────
    constexpr int MAX_COURSES_PER_TEACHER = 3;
    constexpr int MAX_TEACHERS_PER_COURSE = 3;

    // ── Teacher hiring policy ─────────────────────────────────────────────
    constexpr int MIN_TEACHER_AGE = 23;
    constexpr int MAX_TEACHER_AGE = 60;
    constexpr int MIN_EXPERIENCE_YEARS = 2;

    // ── Student age policy (expected age = grade + BASE ± TOLERANCE) ──────
    constexpr int STUDENT_AGE_BASE = 5;
    constexpr int STUDENT_AGE_TOLERANCE = 1;

    // ── Course hours ──────────────────────────────────────────────────────
    constexpr int MIN_SUBJECT_HOURS = 2;
    constexpr int MAX_SUBJECT_HOURS = 6;

    // ── GPA scale ─────────────────────────────────────────────────────────
    constexpr float MIN_GPA = 0.0f;
    constexpr float MAX_GPA = 4.0f;

    // ── Phone number length ───────────────────────────────────────────────
    constexpr int MIN_PHONE_LENGTH = 10;
    constexpr int MAX_PHONE_LENGTH = 12;

    // ── National number length ────────────────────────────────────────────
    constexpr int NATIONAL_NUMBER_LENGTH = 14;

    // ── Teacher salary ────────────────────────────────────────────────────
    constexpr double MIN_MONTHLY_SALARY = 7000.0;

    // ── Minimum available student seats required before teacher assignment ─
    constexpr int MIN_AVAILABLE_SEATS_PRIMARY = 35;
    constexpr int MIN_AVAILABLE_SEATS_MIDDLE = 32;
    constexpr int MIN_AVAILABLE_SEATS_SECONDARY = 30;

    // ── Grade capacity limits (max entities per grade) ────────────────────
    constexpr int MAX_TEACHERS_IN_PRIMARY =  7;
    constexpr int MAX_TEACHERS_IN_MIDDLE =  9;
    constexpr int MAX_TEACHERS_IN_SECONDARY = 12;

    constexpr int MAX_COURSES_IN_PRIMARY =  8;
    constexpr int MAX_COURSES_IN_MIDDLE = 11;
    constexpr int MAX_COURSES_IN_SECONDARY = 13;

    constexpr int MAX_STUDENTS_IN_PRIMARY = 110;
    constexpr int MAX_STUDENTS_IN_MIDDLE = 100;
    constexpr int MAX_STUDENTS_IN_SECONDARY = 90;


    // ── Data file paths ───────────────────────────────────────────────────
    constexpr const char* FILE_STUDENTS = "students.txt";
    constexpr const char* FILE_COURSES  = "courses.txt";
    constexpr const char* FILE_TEACHERS = "teachers.txt";

} // namespace SchoolConstants

#endif
