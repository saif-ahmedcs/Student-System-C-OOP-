#include "BaseEntity.cpp"

class Student : public BaseEntity { // Student inherits from BaseEntity to reuse common attributes
  private :
      string school_year;
      double gpa;


  public :
     // Setters
    void setSchoolYear(string school_year){
     this->school_year = school_year;
    }

    void setGpa(double gpa){
     this->gpa = gpa;
    }

    // Getters
    string GetSchoolYear(){
     return school_year;
    }

    double GetGpa(){
     return gpa;
    }




};
