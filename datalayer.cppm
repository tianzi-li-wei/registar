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
};
