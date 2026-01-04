import Domain;
import DataLayer;
import LogicLayer;
import Presentation;
import std;


int main() {
    std::shared_ptr<IDataAccess> db = std::make_shared<MockDB>();


    auto controller = std::make_unique<CourseController>(db);


    ConsoleView view(std::move(controller));


    view.showMainMenu();

    return 0;
}
