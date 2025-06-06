#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

using namespace std;
using namespace mongocxx;
using namespace bsoncxx::builder::stream;

// Validation functions
bool validate_phone(const string& phone) {
    return regex_match(phone, regex("^\\d{10}$"));
}

bool validate_email(const string& email) {
    return regex_match(email, regex("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$"));
}

bool validate_date(const string& date) {
    return regex_match(date, regex("^\\d{4}-\\d{2}-\\d{2}$"));
}

bool validate_gender(const string& gender) {
    return (gender == "Male" || gender == "Female" || gender == "Other");
}

bool validate_age(int age) {
    return (age >= 0 && age <= 150);
}

// Main program
int main() {
    // Initialize MongoDB driver
    try {
        mongocxx::instance inst{};
        mongocxx::client conn{mongocxx::uri{"mongodb://localhost:27017"}};
        auto db = conn["healthcare_db"];
        auto patients = db["patients"];

        // Create indexes
        patients.create_index(document{} << "patient_id" << 1 << finalize, {{"unique", true}});
        patients.create_index(document{} << "name" << "text" << "condition" << "text" << finalize);

        string command;
        while (true) {
            cout << "\nHealthcare Patient Management System\n";
            cout << "Commands: add, search, update, delete, stats, exit\n";
            cout << "Enter command: ";
            cin >> command;

            if (command == "exit") break;

            if (command == "add") {
                string patient_id, name, gender, phone, email, allergy, history, condition, visit_date;
                int age;
                cout << "Enter patient_id: "; cin >> patient_id;
                cin.ignore();
                cout << "Enter name: "; getline(cin, name);
                cout << "Enter gender (Male/Female/Other): "; cin >> gender;
                cout << "Enter age: "; cin >> age;
                cout << "Enter phone (10 digits): "; cin >> phone;
                cout << "Enter email: "; cin >> email;
                cin.ignore();
                cout << "Enter allergy: "; getline(cin, allergy);
                cout << "Enter medical history: "; getline(cin, history);
                cout << "Enter condition: "; getline(cin, condition);
                cout << "Enter visit date (YYYY-MM-DD): "; cin >> visit_date;

                // Validate inputs
                if (!validate_phone(phone)) {
                    cout << "Error: Invalid phone number (must be 10 digits)\n";
                    continue;
                }
                if (!validate_email(email)) {
                    cout << "Error: Invalid email format\n";
                    continue;
                }
                if (!validate_date(visit_date)) {
                    cout << "Error: Invalid date format (use YYYY-MM-DD)\n";
                    continue;
                }
                if (!validate_gender(gender)) {
                    cout << "Error: Invalid gender (use Male/Female/Other)\n";
                    continue;
                }
                if (!validate_age(age)) {
                    cout << "Error: Invalid age (0-150)\n";
                    continue;
                }

                try {
                    auto doc = document{}
                        << "patient_id" << patient_id
                        << "name" << name
                        << "gender" << gender
                        << "age" << age
                        << "phone" << phone
                        << "email" << email
                        << "allergies" << open_array << allergy << close_array
                        << "medical_history" << open_array << history << close_array
                        << "condition" << condition
                        << "visit_date" << visit_date
                        << finalize;
                    patients.insert_one(doc.view());
                    cout << "Patient added successfully\n";
                } catch (const mongocxx::exception& e) {
                    cout << "Error adding patient: " << e.what() << endl;
                }
            }
            else if (command == "search") {
                string field, value;
                cout << "Search by (patient_id/name/condition/visit_date): "; cin >> field;
                cin.ignore();
                cout << "Enter value: "; getline(cin, value);

                try {
                    bsoncxx::builder::stream::document filter{};
                    if (field == "patient_id") {
                        filter << "patient_id" << value;
                    } else if (field == "name" || field == "condition") {
                        filter << "$text" << document{} << "$search" << value << finalize;
                    } else if (field == "visit_date") {
                        if (!validate_date(value)) {
                            cout << "Error: Invalid date format\n";
                            continue;
                        }
                        filter << "visit_date" << document{} << "$gte" << value << finalize;
                    } else {
                        cout << "Error: Invalid search field\n";
                        continue;
                    }

                    auto cursor = patients.find(filter.view());
                    for (auto&& doc : cursor) {
                        cout << bsoncxx::to_json(doc) << endl;
                    }
                } catch (const mongocxx::exception& e) {
                    cout << "Error searching patients: " << e.what() << endl;
                }
            }
            else if (command == "update") {
                string patient_id, field, value;
                cout << "Enter patient_id: "; cin >> patient_id;
                cout << "Field to update (name/gender/age/phone/email/allergies/history/condition/visit_date): "; cin >> field;
                cin.ignore();
                cout << "Enter new value: "; getline(cin, value);

                // Validate inputs
                if (field == "phone" && !validate_phone(value)) {
                    cout << "Error: Invalid phone number\n";
                    continue;
                }
                if (field == "email" && !validate_email(value)) {
                    cout << "Error: Invalid email format\n";
                    continue;
                }
                if (field == "visit_date" && !validate_date(value)) {
                    cout << "Error: Invalid date format\n";
                    continue;
                }
                if (field == "gender" && !validate_gender(value)) {
                    cout << "Error: Invalid gender\n";
                    continue;
                }
                if (field == "age") {
                    try {
                        int age = stoi(value);
                        if (!validate_age(age)) {
                            cout << "Error: Invalid age\n";
                            continue;
                        }
                        value = to_string(age);
                    } catch (...) {
                        cout << "Error: Invalid age format\n";
                        continue;
                    }
                }

                try {
                    auto filter = document{} << "patient_id" << patient_id << finalize;
                    auto update = document{} << "$set" << open_document;
                    if (field == "allergies" || field == "history") {
                        update << field << open_array << value << close_array;
                    } else {
                        update << field << value;
                    }
                    update << close_document << finalize;

                    auto result = patients.update_one(filter.view(), update.view());
                    if (result && result->matched_count() > 0) {
                        cout << "Patient updated successfully\n";
                    } else {
                        cout << "Patient not found\n";
                    }
                } catch (const mongocxx::exception& e) {
                    cout << "Error updating patient: " << e.what() << endl;
                }
            }
            else if (command == "delete") {
                string patient_id;
                cout << "Enter patient_id: "; cin >> patient_id;

                try {
                    auto result = patients.delete_one(document{} << "patient_id" << patient_id << finalize);
                    if (result && result->deleted_count() > 0) {
                        cout << "Patient deleted successfully\n";
                    } else {
                        cout << "Patient not found\n";
                    }
                } catch (const mongocxx::exception& e) {
                    cout << "Error deleting patient: " << e.what() << endl;
                }
            }
            else if (command == "stats") {
                try {
                    auto pipeline = pipeline{}
                        << stage{} << "$group" << open_document
                        << "_id" << "$condition"
                        << "count" << open_document << "$sum" << 1 << close_document
                        << close_document
                        << stage{} << "$sort" << open_document << "count" << -1 << close_document
                        << finalize;
                    auto cursor = patients.aggregate(pipeline);
                    for (auto&& doc : cursor) {
                        cout << bsoncxx::to_json(doc) << endl;
                    }
                } catch (const mongocxx::exception& e) {
                    cout << "Error generating stats: " << e.what() << endl;
                }
            }
            else {
                cout << "Error: Invalid command\n";
            }
        }
    } catch (const mongocxx::exception& e) {
        cerr << "MongoDB connection error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
