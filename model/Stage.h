#ifndef STAGE_H
#define STAGE_H

#include <stdexcept>
#include <string>

// ─────────────────────────────────────────────────────────────────────────
//  Stage
//
//  Domain concept that classifies grades into school stages.
//  Separated into its own header so it can be included independently
//  without pulling in the full model hierarchy.
// ─────────────────────────────────────────────────────────────────────────
enum class Stage { Primary, Middle, Secondary };

inline Stage getStageFromGrade(int grade) {
    if (grade >= 1  && grade <= 6)
        return Stage::Primary;
    if (grade >= 7  && grade <= 9)
        return Stage::Middle;
    if (grade >= 10 && grade <= 12)
       return Stage::Secondary;
    throw std::invalid_argument(
        "getStageFromGrade: grade must be 1-12, got " + std::to_string(grade));
}

#endif
