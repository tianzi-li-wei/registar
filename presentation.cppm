export module Presentation;

import LogicLayer;
import std;

export class ConsoleView {
private:
    std::unique_ptr<CourseController> controller;
    std::string currentStudentId;

    //学生操作循环
    void studentLoop();

    // 显示列表
    void showCourseList();

    // 处理输入
    void handleEnrollment();

public:

    ConsoleView(std::unique_ptr<CourseController> ctrl);

    // 显示主菜单
    void showMainMenu();
};
