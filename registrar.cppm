export module registrar;

// 导入各层模块
export import Domain;
export import DataLayer;
export import LogicLayer;
export import Presentation;

import std;

export class Registrar {
public:
    auto exec() -> int;
};

// 实现
auto Registrar::exec() -> int {
    // 创建数据访问层实例
    auto dataAccess = std::make_shared<MockDB>();

    // 创建逻辑控制层实例
    auto controller = std::make_unique<CourseController>(dataAccess);

    // 创建表示层实例并运行
    ConsoleView view(std::move(controller));
    return view.run();
}
