#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

using namespace mongocxx;
using namespace bsoncxx::builder::stream;

int main() {
    try {
        mongocxx::instance inst{};
        mongocxx::client conn{mongocxx::uri{"mongodb://localhost:27017"}};
        auto db = conn["healthcare_db"];
        auto patients = db["patients"];

        // Test: Add a patient
        auto doc = document{}
            << "patient_id" << "TEST001"
            << "name" << "Test Patient"
            << "gender" << "Male"
            << "age" << 25
            << "phone" << "1234567890"
            << "email" << "test@example.com"
            << "allergies" << open_array << "None" << close_array
            << "medical_history" << open_array << "None" << close_array
            << "condition" << "Test"
            << "visit_date" << "2025-06-01"
            << finalize;
        patients.insert_one(doc.view());

        // Test: Retrieve patient
        auto cursor = patients.find(document{} << "patient_id" << "TEST001" << finalize);
        bool found = false;
        for (auto&& doc : cursor) {
            found = true;
            std::cout << "Test passed: Patient found - " << bsoncxx::to_json(doc) << std::endl;
        }
        if (!found) {
            std::cout << "Test failed: Patient not found\n";
            return 1;
        }

        // Clean up
        patients.delete_one(document{} << "patient_id" << "TEST001" << finalize);
        return 0;
    } catch (const mongocxx::exception& e) {
        std::cerr << "Test failed: MongoDB error - " << e.what() << std::endl;
        return 1;
    }
}
