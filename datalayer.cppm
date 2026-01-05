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

    // 根据ID查教师
    virtual std::optional<Teacher> getTeacherById(const std::string& id) = 0;

    // 获取教师授课列表
    virtual std::vector<Course> getTeachingCourses(const std::string& teacherId) = 0;

    // 获取课程选课学生
    virtual std::vector<Student> getCourseStudents(const std::string& courseId) = 0;

    // 更新学生成绩
    virtual bool updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade) = 0;

    // 选课操作
    virtual bool enrollStudent(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool dropCourse(const std::string& studentId, const std::string& courseId) = 0;
};

export class MockDB : public IDataAccess {
private:
    std::vector<Course> courses;
    std::vector<Student> students;
    std::vector<Teacher> teachers;

public:
    MockDB();

    std::vector<Course> getAllCourses() override;
    std::optional<Student> getStudentById(const std::string& id) override;
    std::optional<Teacher> getTeacherById(const std::string& id) override;
    std::vector<Course> getTeachingCourses(const std::string& teacherId) override;
    std::vector<Student> getCourseStudents(const std::string& courseId) override;
    bool updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade) override;
    bool enrollStudent(const std::string& studentId, const std::string& courseId) override;
    bool dropCourse(const std::string& studentId, const std::string& courseId) override;
};

MockDB::MockDB() {
    // 初始化教师数据
    teachers = {
        Teacher("T001", "李教授"),
        Teacher("T002", "王教授")
    };

    // 初始化课程数据（关联教师）
    courses = {
        Course("CS101", "程序设计基础", 3, 30, "T001"),
        Course("CS102", "数据结构", 4, 25, "T001"),
        Course("MATH101", "高等数学", 4, 40, "T002")
    };

    students = {
        Student("S001", "张三"),
        Student("S002", "李四")
    };

    // 设置教师授课关系
    teachers[0].addTeachingCourse("CS101");
    teachers[0].addTeachingCourse("CS102");
    teachers[1].addTeachingCourse("MATH101");

    // 初始化一些选课记录
    enrollStudent("S001", "CS101");
    enrollStudent("S001", "CS102");
    enrollStudent("S002", "CS101");
    enrollStudent("S002", "MATH101");
}

std::vector<Course> MockDB::getAllCourses() {
    return courses;
}

std::optional<Student> MockDB::getStudentById(const std::string& id) {
    for (auto& student : students) {
        if (student.id == id) {
            return student;
        }
    }
    return std::nullopt;
}

std::optional<Teacher> MockDB::getTeacherById(const std::string& id) {
    for (auto& teacher : teachers) {
        if (teacher.id == id) {
            return teacher;
        }
    }
    return std::nullopt;
}

std::vector<Course> MockDB::getTeachingCourses(const std::string& teacherId) {
    std::vector<Course> result;
    for (const auto& course : courses) {
        if (course.getTeacherId() == teacherId) {
            result.push_back(course);
        }
    }
    return result;
}

std::vector<Student> MockDB::getCourseStudents(const std::string& courseId) {
    std::vector<Student> result;
    for (const auto& student : students) {
        if (student.isEnrolled(courseId)) {
            result.push_back(student);
        }
    }
    return result;
}

bool MockDB::updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade) {
    for (auto& student : students) {
        if (student.id == studentId && student.isEnrolled(courseId)) {
            student.setGrade(courseId, grade);
            return true;
        }
    }
    return false;
}

bool MockDB::enrollStudent(const std::string& studentId, const std::string& courseId) {
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

bool MockDB::dropCourse(const std::string& studentId, const std::string& courseId) {
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
