export module Domain;
import std;

export class Course {
public:
    std::string id;
    std::string name;
    int credit;
    int maxCapacity;
    int currentEnrollment;
    std::string teacherId; // 添加教师ID字段

    // 构造函数声明
    Course(std::string _id, std::string _name, int _credit, int _max, std::string _teacherId = "");

    std::string toString() const;
    bool isFull() const;
    bool enrollStudent();
    void dropStudent();
    std::string getTeacherId() const { return teacherId; } // 获取教师ID
};

// 实现
Course::Course(std::string _id, std::string _name, int _credit, int _max, std::string _teacherId)
    : id(_id), name(_name), credit(_credit), maxCapacity(_max), currentEnrollment(0), teacherId(_teacherId) {}

std::string Course::toString() const {
    return "课程ID: " + id + " | 名称: " + name +
           " | 学分: " + std::to_string(credit) +
           " | 容量: " + std::to_string(currentEnrollment) + "/" +
           std::to_string(maxCapacity) +
           " | 教师: " + teacherId;
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

export class Teacher {
public:
    std::string id;
    std::string name;
    std::vector<std::string> teachingCourseIds;

    // 构造函数声明
    Teacher(std::string _id, std::string _name);

    // 成员函数
    void addTeachingCourse(const std::string& courseId);
    void removeTeachingCourse(const std::string& courseId);
    bool isTeaching(const std::string& courseId) const;
    std::vector<std::string> getTeachingCourses() const { return teachingCourseIds; }
};

// 实现
Teacher::Teacher(std::string _id, std::string _name)
    : id(_id), name(_name) {}

void Teacher::addTeachingCourse(const std::string& courseId) {
    if (!isTeaching(courseId)) {
        teachingCourseIds.push_back(courseId);
    }
}

void Teacher::removeTeachingCourse(const std::string& courseId) {
    auto it = std::find(teachingCourseIds.begin(), teachingCourseIds.end(), courseId);
    if (it != teachingCourseIds.end()) {
        teachingCourseIds.erase(it);
    }
}

bool Teacher::isTeaching(const std::string& courseId) const {
    return std::find(teachingCourseIds.begin(), teachingCourseIds.end(), courseId)
           != teachingCourseIds.end();
}
