export module LogicLayer;

import Domain;
import DataLayer;
import std;
export class CourseController {
public:

    CourseController(std::shared_ptr<IDataAccess> dataAccess);


    std::vector<Course> getAvailableCourses();


    std::string registerCourse(std::string studentId, std::string courseId);


    bool loginCheck(std::string studentId);

private:

    std::shared_ptr<IDataAccess> db;


};
