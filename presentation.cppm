export module Presentation;
import Domain;
import LogicLayer;
import std;
using std::size_t;

export class ConsoleView {
private:
    std::unique_ptr<CourseController> controller;
    std::string currentStudentId;
    std::string currentTeacherId;
    int choice;

    void showCourseList();
    void handleEnrollment();
    void handleDropCourse();
    void teacherLoop(); // 添加教师循环
    void showCourseStudents(); // 查看课程选课学生
    void handleGradeInput(); // 录入/修改成绩

public:
    using ControllerPtr = std::unique_ptr<CourseController>;

    ConsoleView(ControllerPtr ctrl);

    void showMainMenu();
    void studentLoop();
    void teacherLoopImpl(); // 教师主循环实现
    auto run()->int;
};

// 构造函数实现
ConsoleView::ConsoleView(ControllerPtr ctrl) : controller(std::move(ctrl)) {}

void ConsoleView::showMainMenu() {
    std::cout << "=== 欢迎使用选课系统 ===\n";
    std::cout << "请选择您的身份登录：\n";
    std::cout << "1. 学生\n";
    std::cout << "2. 教师\n";
    std::cout << "3. 教学秘书\n";
    std::cout << "0. 退出系统\n";
    std::cout << "请输入选项（0-3）：";
}

void ConsoleView::studentLoop() {
    int choice;
    while (true) {
        auto studentOpt = controller->getStudentInfo(currentStudentId);
        if (!studentOpt) {
            std::cout << "学生信息不存在！\n";
            return;
        }

        Student& student = studentOpt.value();
        std::cout << "[学生] " << student.name << "（学号：" << student.id << "）您好！\n";
        std::cout << "请选择操作：\n";
        std::cout << "1. 查看可选课程列表\n";
        std::cout << "2. 选课\n";
        std::cout << "3. 退课\n";
        std::cout << "4. 查看已选课程及成绩\n";
        std::cout << "5. 返回上级菜单\n";
        std::cout << "请输入选项（1-5）：";

        std::cin >> choice;

        switch (choice) {
            case 1:
                showCourseList();
                break;
            case 2:
                handleEnrollment();
                break;
            case 3:
                handleDropCourse();
                break;
            case 4:
                std::cout << "\n已选课程及成绩：\n";
                for (const auto& courseId : student.enrolledCourseIds) {
                    double grade = student.getGrade(courseId);
                    std::cout << "课程ID: " << courseId << " | 成绩: " << grade;
                    if (!student.hasGrade(courseId)) {
                        std::cout << " (未录入)";
                    }
                    std::cout << std::endl;
                }
                break;
            case 5:
                return;
            default:
                std::cout << "无效选项，请重新输入！\n";
        }
    }
}

void ConsoleView::teacherLoopImpl() {
    int choice;
    while (true) {
        auto teacherOpt = controller->getTeacherInfo(currentTeacherId);
        if (!teacherOpt) {
            std::cout << "教师信息不存在！\n";
            return;
        }

        Teacher& teacher = teacherOpt.value();
        std::cout << "[教师] " << teacher.name << "（工号：" << teacher.id << "）您好！\n";
        std::cout << "请选择操作：\n";
        std::cout << "1. 查看我的授课列表\n";
        std::cout << "2. 查看课程选课学生\n";
        std::cout << "3. 录入/修改课程成绩\n";
        std::cout << "4. 返回上级菜单\n";
        std::cout << "请输入选项（1-4）：";

        std::cin >> choice;

        switch (choice) {
            case 1: {
                auto courses = controller->getTeachingCourses(currentTeacherId);
                std::cout << "\n授课列表：\n";
                for (const auto& course : courses) {
                    std::cout << course.toString() << std::endl;
                }
                break;
            }
            case 2:
                showCourseStudents();
                break;
            case 3:
                handleGradeInput();
                break;
            case 4:
                return;
            default:
                std::cout << "无效选项，请重新输入！\n";
        }
    }
}

void ConsoleView::showCourseStudents() {
    auto courses = controller->getTeachingCourses(currentTeacherId);
    if (courses.empty()) {
        std::cout << "您当前没有授课课程！\n";
        return;
    }

    std::cout << "\n请选择要查看的课程：\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        std::cout << i + 1 << ". " << courses[i].toString() << std::endl;
    }
    std::cout << "请输入课程编号：";

    int courseChoice;
    std::cin >> courseChoice;

    if (courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        std::cout << "无效的课程编号！\n";
        return;
    }

    const std::string& courseId = courses[courseChoice - 1].id;
    auto students = controller->getCourseStudents(courseId);

    std::cout << "\n课程 " << courseId << " 的选课学生列表：\n";
    if (students.empty()) {
        std::cout << "该课程暂无学生选课。\n";
    } else {
        for (const auto& student : students) {
            double grade = student.getGrade(courseId);
            std::cout << "学号: " << student.id << " | 姓名: " << student.name << " | 成绩: " << grade;
            if (!student.hasGrade(courseId)) {
                std::cout << " (未录入)";
            }
            std::cout << std::endl;
        }
    }
}

void ConsoleView::handleGradeInput() {
    auto courses = controller->getTeachingCourses(currentTeacherId);
    if (courses.empty()) {
        std::cout << "您当前没有授课课程！\n";
        return;
    }

    std::cout << "\n请选择要录入成绩的课程：\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        std::cout << i + 1 << ". " << courses[i].toString() << std::endl;
    }
    std::cout << "请输入课程编号：";

    int courseChoice;
    std::cin >> courseChoice;

    if (courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        std::cout << "无效的课程编号！\n";
        return;
    }

    const std::string& courseId = courses[courseChoice - 1].id;
    auto students = controller->getCourseStudents(courseId);

    if (students.empty()) {
        std::cout << "该课程暂无学生选课，无法录入成绩。\n";
        return;
    }

    std::cout << "\n为课程 " << courseId << " 录入成绩：\n";
    for (const auto& student : students) {
        double currentGrade = student.getGrade(courseId);
        std::cout << "学生: " << student.name << "（学号: " << student.id << "）";
        if (student.hasGrade(courseId)) {
            std::cout << " [当前成绩: " << currentGrade << "]";
        }
        std::cout << " 请输入新成绩：";

        double newGrade;
        std::cin >> newGrade;

        if (controller->updateStudentGrade(student.id, courseId, newGrade)) {
            std::cout << "成绩录入成功！\n";
        } else {
            std::cout << "成绩录入失败！\n";
        }
    }
}

void ConsoleView::showCourseList() {
    const auto& courses = controller->getAvailableCourses();
    std::cout << "\n可选课程列表：\n";
    for (const auto& course : courses) {
        std::cout << course.toString() << std::endl;
    }
}

void ConsoleView::handleEnrollment() {
    std::string courseId;
    std::cout << "请输入要选择的课程ID：";
    std::cin >> courseId;

    if (controller->enrollCourse(currentStudentId, courseId)) {
        std::cout << "选课成功！\n";
    } else {
        std::cout << "选课失败，请检查课程ID或是否已选该课程！\n";
    }
}

void ConsoleView::handleDropCourse() {
    std::string courseId;
    std::cout << "请输入要退选的课程ID：";
    std::cin >> courseId;

    if (controller->dropCourse(currentStudentId, courseId)) {
        std::cout << "退课成功！\n";
    } else {
        std::cout << "退课失败，请检查课程ID或是否已选该课程！\n";
    }
}

auto ConsoleView::run() -> int {
    int choice;
    while (true) {
        showMainMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: // 学生登录
                currentStudentId = "S001"; // 默认使用张三的学号
                studentLoop();
                break;
            case 2: // 教师
                currentTeacherId = "T001"; // 默认使用李教授的工号
                teacherLoopImpl();
                break;
            case 3: // 教学秘书
                std::cout << "教学秘书功能正在开发中...\n";
                break;
            case 0:
                std::cout << "感谢使用选课系统，再见！\n";
                return 0;
            default:
                std::cout << "无效选项，请重新输入！\n";
        }
    }
    return 0;
}
