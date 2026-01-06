export module DataLayer;
import Domain;
import std;

export class IDataAccess {
public:
    virtual ~IDataAccess() = default;

    virtual std::vector<Course> getAllCourses() = 0;


    virtual std::vector<Teacher> getAllTeachers() = 0;

    virtual std::optional<Student> getStudentById(const std::string& id) = 0;
    virtual std::optional<Teacher> getTeacherById(const std::string& id) = 0;

    virtual std::vector<Course> getTeachingCourses(const std::string& teacherId) = 0;
    virtual std::vector<Student> getCourseStudents(const std::string& courseId) = 0;

    virtual bool updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade) = 0;
    virtual bool enrollStudent(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool dropCourse(const std::string& studentId, const std::string& courseId) = 0;


    virtual bool assignTeacher(const std::string& courseId, const std::string& teacherId) = 0;
};

export class MockDB : public IDataAccess {
private:
    std::vector<Course> courses;
    std::vector<Student> students;
    std::vector<Teacher> teachers;

public:
    MockDB();

    std::vector<Course> getAllCourses() override;
    std::vector<Teacher> getAllTeachers() override; // 实现新增接口
    std::optional<Student> getStudentById(const std::string& id) override;
    std::optional<Teacher> getTeacherById(const std::string& id) override;
    std::vector<Course> getTeachingCourses(const std::string& teacherId) override;
    std::vector<Student> getCourseStudents(const std::string& courseId) override;
    bool updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade) override;
    bool enrollStudent(const std::string& studentId, const std::string& courseId) override;
    bool dropCourse(const std::string& studentId, const std::string& courseId) override;
    bool assignTeacher(const std::string& courseId, const std::string& teacherId) override; // 实现新增接口
};

MockDB::MockDB() {
    teachers = { Teacher("T001", "李教授"), Teacher("T002", "王教授") };
    // 初始状态把 MATH101 的老师置空，方便测试排课
    courses = {
        Course("CS101", "程序设计基础", 3, 30, "T001"),
        Course("CS102", "数据结构", 4, 25, "T001"),
        Course("MATH101", "高等数学", 4, 40, "") // 待安排
    };
    students = { Student("S001", "张三"), Student("S002", "李四") };

    // 初始化关系
    teachers[0].teachingCourseIds.push_back("CS101");
    teachers[0].teachingCourseIds.push_back("CS102");

    // 初始化选课
    enrollStudent("S001", "CS101");
}

std::vector<Course> MockDB::getAllCourses() { return courses; }

// [新增]
std::vector<Teacher> MockDB::getAllTeachers() { return teachers; }

std::optional<Student> MockDB::getStudentById(const std::string& id) {
    for (auto& s : students) if (s.id == id) return s;
    return std::nullopt;
}

std::optional<Teacher> MockDB::getTeacherById(const std::string& id) {
    for (auto& t : teachers) if (t.id == id) return t;
    return std::nullopt;
}

std::vector<Course> MockDB::getTeachingCourses(const std::string& teacherId) {
    std::vector<Course> result;
    // 直接访问 teacherId，不使用 getter
    for (const auto& c : courses) if (c.teacherId == teacherId) result.push_back(c);
    return result;
}

std::vector<Student> MockDB::getCourseStudents(const std::string& courseId) {
    std::vector<Student> result;
    for (const auto& s : students) if (s.isEnrolled(courseId)) result.push_back(s);
    return result;
}

bool MockDB::updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade) {
    for (auto& s : students) {
        if (s.id == studentId && s.isEnrolled(courseId)) {
            // 直接访问 public 成员，不使用 setter
            s.courseGrades[courseId] = grade;
            return true;
        }
    }
    return false;
}

bool MockDB::enrollStudent(const std::string& studentId, const std::string& courseId) {
    auto sOpt = getStudentById(studentId);
    if (!sOpt) return false;

    // 使用指针查找，以便修改
    Course* targetCourse = nullptr;
    for (auto& c : courses) {
        if (c.id == courseId) { targetCourse = &c; break; }
    }

    if (!targetCourse || targetCourse->isFull() || sOpt->isEnrolled(courseId)) return false;

    // 执行修改
    targetCourse->enroll();

    for (auto& s : students) {
        if (s.id == studentId) {
            s.enrolledCourseIds.push_back(courseId);
            s.courseGrades[courseId] = 0.0;
            return true;
        }
    }
    return false;
}

bool MockDB::dropCourse(const std::string& studentId, const std::string& courseId) {
    auto sOpt = getStudentById(studentId);
    if (!sOpt || !sOpt->isEnrolled(courseId)) return false;

    Course* targetCourse = nullptr;
    for (auto& c : courses) {
        if (c.id == courseId) { targetCourse = &c; break; }
    }
    if (targetCourse) targetCourse->drop();

    for (auto& s : students) {
        if (s.id == studentId) {
            std::erase(s.enrolledCourseIds, courseId);
            s.courseGrades.erase(courseId);
            return true;
        }
    }
    return false;
}


bool MockDB::assignTeacher(const std::string& courseId, const std::string& teacherId) {

    Course* targetCourse = nullptr;
    for (auto& c : courses) {
        if (c.id == courseId) { targetCourse = &c; break; }
    }
    if (!targetCourse) return false;


    Teacher* targetTeacher = nullptr;
    for (auto& t : teachers) {
        if (t.id == teacherId) { targetTeacher = &t; break; }
    }
    if (!targetTeacher) return false;


    targetCourse->teacherId = teacherId;


    if (!targetTeacher->isTeaching(courseId)) {
        targetTeacher->teachingCourseIds.push_back(courseId);
    }
    return true;
}
