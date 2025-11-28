class Teacher {
  private :
      string name;
      int age;
      string id;
      string phone_number;
      double Monthly_salary;

  public :
    // Setters
    void setName(string name){
     this->name = name;
    }

    void setAge(int age){
     this->age = age;
    }

    void setId(string id){
     this->id = id;
    }

    void setPhoneNumber(string phone_number){
     this->phone_number = phone_number;
    }

    void setMonthlySalary(double Monthly_salary){
     this->Monthly_salary = Monthly_salary;
    }


    // Getters
    string GetName(){
     return name;
    }

    int GetAge(){
     return age;
    }

    string GetId(){
     return id;
    }

    string GetPhoneNumber(){
     return phone_number;
    }


    double GetMonthlySalary(){
     return Monthly_salary;
    }




};

