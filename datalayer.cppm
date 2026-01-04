export module DataLayer;

import Domain;
import std;

export class IDataAccess {
public:
    virtual ~IDataAccess() = default;

    // 获取所有课程
    virtual std::vector<Course> getAllCourses() = 0;

    // 根据ID查学生
    virtual std::optional<Student> getStudentById(const std::string& id) = 0;

    // 选课操作
    virtual bool enrollStudent(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool dropCourse(const std::string& studentId, const std::string& courseId) = 0;
};


export class MockDB : public IDataAccess {
private:
    std::vector<Course> courses;
    std::vector<Student> students;

public:
    MockDB();

    std::vector<Course> getAllCourses() override;
    std::optional<Student> getStudentById(const std::string& id) override;
    bool enrollStudent(const std::string& studentId, const std::string& courseId) override;
    bool dropCourse(const std::string& studentId, const std::string& courseId) override;
};

//实现
MockDB::MockDB() {
    // 初始化测试数据
    courses = {
        Course("CS101", "程序设计基础", 3, 30),
        Course("CS102", "数据结构", 4, 25),
        Course("MATH101", "高等数学", 4, 40)
    };

    students = {
        Student("S001", "张三"),
        Student("S002", "李四")
    };
}

std::vector<Course> MockDB::getAllCourses() override {
    return courses;
}

std::optional<Student> MockDB::getStudentById(const std::string& id) override {
    for (auto& student : students) {
        if (student.id == id) {
            return student;
        }
    }
    return std::nullopt;
}

bool MockDB::enrollStudent(const std::string& studentId, const std::string& courseId) override {
    // 查找学生
    auto studentOpt = getStudentById(studentId);
    if (!studentOpt) return false;

    // 查找课程
    Course* targetCourse = nullptr;
    for (auto& course : courses) {
        if (course.id == courseId) {
            targetCourse = &course;
            break;
        }
    }
    if (!targetCourse || targetCourse->isFull()) return false;

    // 检查是否已选
    if (studentOpt->isEnrolled(courseId)) return false;

    // 执行选课
    targetCourse->enrollStudent();

    // 更新学生选课记录
    for (auto& student : students) {
        if (student.id == studentId) {
            student.addCourse(courseId);
            return true;
        }
    }
    return false;
}

bool MockDB::dropCourse(const std::string& studentId, const std::string& courseId) override {
    // 查找学生
    auto studentOpt = getStudentById(studentId);
    if (!studentOpt) return false;

    // 查找课程
    Course* targetCourse = nullptr;
    for (auto& course : courses) {
        if (course.id == courseId) {
            targetCourse = &course;
            break;
        }
    }
    if (!targetCourse) return false;

    // 检查是否已选
    if (!studentOpt->isEnrolled(courseId)) return false;

    // 执行退课
    targetCourse->dropStudent();

    // 更新学生选课记录
    for (auto& student : students) {
        if (student.id == studentId) {
            student.removeCourse(courseId);
            return true;
        }
    }
    return false;
}
