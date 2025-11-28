#include "BaseEntity.cpp"

class Teacher : public BaseEntity { // Teacher inherits from BaseEntity to reuse common attributes
  private :
      double Monthly_salary;
      string Teacher_subject;

  public :
    // Setters
    void setMonthlySalary(double Monthly_salary){
     this->Monthly_salary = Monthly_salary;
    }

    void setTeacherSubject(string Teacher_subject){
     this->Teacher_subject = Teacher_subject;
    }

    // Getters
    double GetMonthlySalary(){
     return Monthly_salary;
    }

    string GetTeacherSubject(){
     return Teacher_subject;
    }



};

