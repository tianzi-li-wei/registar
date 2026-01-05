export module Presentation;
import Domain;
import LogicLayer;
import std;

export class ConsoleView {
private:
    std::unique_ptr<CourseController> controller;
    std::string currentStudentId;
    int choice;  // 添加choice成员变量

    void showCourseList();
    void handleEnrollment();
    void handleDropCourse();

public:
    using ControllerPtr = std::unique_ptr<CourseController>;

    ConsoleView(ControllerPtr ctrl);

    void showMainMenu();
    void studentLoop();
    auto run()->int;
};

// 修正构造函数实现
ConsoleView::ConsoleView(ControllerPtr ctrl) : controller(std::move(ctrl)) {}

void ConsoleView::showMainMenu() {
    std::cout << "=== 欢迎使用选课系统 ===\n";
    std::cout << "请选择您的身份登录：\n";
    std::cout << "1. 学生\n";
    std::cout << "2. 教师\n";  // 修正引号
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
                // 显示已选课程
                std::cout << "\n已选课程：\n";
                for (const auto& courseId : student.enrolledCourseIds) {
                    // 这里可以显示课程详情
                    std::cout << courseId << std::endl;
                }
                break;
            case 5:
                return;
            default:
                std::cout << "无效选项，请重新输入！\n";
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
                std::cout << "教师功能正在开发中...\n";
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
