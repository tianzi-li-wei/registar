export module Domain;
import std;

export class Course {
public:
    std::string id;
    std::string name;
    int credit;
    int maxCapacity;
    int currentEnrollment;

    // 构造函数声明
    Course(std::string _id, std::string _name, int _credit, int _max);


    std::string toString() const;
    bool isFull() const;
        bool enrollStudent();
        void dropStudent();
};
 //实现
 Course::Course(std::string _id, std::string _name, int _credit, int _max)
     : id(_id), name(_name), credit(_credit), maxCapacity(_max), currentEnrollment(0) {}

 std::string Course::toString() const {
     return "课程ID: " + id + " | 名称: " + name +
            " | 学分: " + std::to_string(credit) +
            " | 容量: " + std::to_string(currentEnrollment) + "/" +
         std::to_string(maxCapacity);
 }

 bool Course::isFull() const {
     return currentEnrollment >= maxCapacity;
 }

 bool Course::enrollStudent() {
     if (isFull()) return false;
     currentEnrollment++;
     return true;
 }

 void Course::dropStudent() {
     if (currentEnrollment > 0) currentEnrollment--;
 }

export class Student {
public:
    std::string id;
    std::string name;
    std::vector<std::string> enrolledCourseIds;

    // 构造函数声明
    Student(std::string _id, std::string _name);
    bool isEnrolled(const std::string& courseId) const;
        void addCourse(const std::string& courseId);
        void removeCourse(const std::string& courseId);
};

// 实现
Student::Student(std::string _id, std::string _name)
    : id(_id), name(_name) {}

bool Student::isEnrolled(const std::string& courseId) const {
    return std::find(enrolledCourseIds.begin(), enrolledCourseIds.end(), courseId)
           != enrolledCourseIds.end();
}

void Student::addCourse(const std::string& courseId) {
    if (!isEnrolled(courseId)) {
        enrolledCourseIds.push_back(courseId);
    }
}

void Student::removeCourse(const std::string& courseId) {
    auto it = std::find(enrolledCourseIds.begin(), enrolledCourseIds.end(), courseId);
    if (it != enrolledCourseIds.end()) {
        enrolledCourseIds.erase(it);
    }
}
