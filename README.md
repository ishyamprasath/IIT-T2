# 🏥 Healthcare Patient Management System (MongoDB)

A robust command-line C++ application designed for managing patient records within a hospital system, leveraging MongoDB for efficient data storage, retrieval, and analysis. This system supports comprehensive CRUD operations, stringent data validation, and advanced MongoDB features like indexing and aggregation.

---

## ✨ Features

This system offers a range of functionalities to streamline patient data management:

### CRUD Operations
Full control over patient records:
*   **Add:** Register new patients with detailed information.
*   **Search:** Find patients using various criteria (ID, name, condition, visit date).
*   **Update:** Modify existing patient details by their unique ID.
*   **Delete:** Remove patient records securely.

**Patient Data Fields:**
*   `patient_id` (String, Unique)
*   `name` (String)
*   `gender` (String: "Male", "Female", "Other")
*   `age` (Integer)
*   `phone` (String, 10 digits)
*   `email` (String, valid email format)
*   `allergies` (String, comma-separated if multiple)
*   `medical_history` (String)
*   `condition` (String)
*   `visit_date` (String, YYYY-MM-DD)

### 🔒 Data Validation
Ensures data integrity with built-in checks:
*   **Phone:** Must be exactly 10 digits (e.g., `1234567890`).
*   **Email:** Must follow a standard email format (e.g., `user@example.com`).
*   **Date:** Must be in `YYYY-MM-DD` format (e.g., `2025-06-01`).
*   **Gender:** Must be one of `Male`, `Female`, or `Other`.
*   **Age:** Must be an integer between `0` and `150`.

### 🚀 MongoDB Features
Leverages MongoDB's power for performance and insights:
*   **Unique Index:** `patient_id` is uniquely indexed for rapid lookups and to prevent duplicate entries.
*   **Text Index:** `name` and `condition` fields are text-indexed for efficient and flexible text-based searches.
*   **Aggregation Pipeline:** An aggregation pipeline is used to count patients grouped by their `condition`, sorted by the count in descending order, providing valuable statistical insights.

### 💻 Command-Line Interface (CLI)
An interactive and user-friendly menu for easy navigation and operation.

---

## 📋 Prerequisites

Before you begin, ensure you have the following installed:

*   **MongoDB:** Server running locally, typically on `mongodb://localhost:27017`.
*   **MongoDB C++ Driver:** `mongocxx` and `bsoncxx` libraries.
    *   Follow the official [MongoDB C++ Driver Installation Guide](http://mongocxx.org/mongocxx-v3/installation/).
*   **CMake:** Version `3.10` or higher.
*   **C++ Compiler:** A C++17 compatible compiler (e.g., GCC, Clang, MSVC).
*   **Operating System:** Linux, macOS, or Windows (with a compatible development environment like MinGW or WSL for Windows).

---

## 🔧 Installation & Setup

Follow these steps to get the project up and running:

1.  **Clone the Repository:**
    ```bash
    git clone https://github.com/<your-username>/healthcare-patient-management-cpp.git
    cd healthcare-patient-management-cpp
    ```

2.  **Install MongoDB:**
    *   Follow the official [MongoDB Installation Guide](https://www.mongodb.com/try/download/community) for your operating system.
    *   Create a data directory (e.g., in your project root):
        ```bash
        mkdir data
        ```
    *   Start the MongoDB server (ensure `mongod` is in your PATH, or provide the full path):
        ```bash
        mongod --dbpath ./data
        ```
        *(This command should be run from the `healthcare-patient-management-cpp` directory if using `./data`)*

3.  **Install MongoDB C++ Driver:**
    *   This is the most platform-dependent step. Refer to the [MongoDB C++ Driver Installation Guide](http://mongocxx.org/mongocxx-v3/installation/).
    *   Ensure that `libmongocxx.so` (or `.dylib`/`.dll`) and `libbsoncxx.so` (or `.dylib`/`.dll`) are installed and discoverable by your linker. You might need to set `CMAKE_PREFIX_PATH` or other environment variables.

4.  **Build the Project:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
    *Note: If CMake cannot find the MongoDB C++ driver, you might need to specify its installation path, e.g.:*
    ```bash
    cmake -DCMAKE_PREFIX_PATH=/path/to/mongo-cxx-driver/installation ..
    ```

5.  **Run the Application:**
    Navigate back to the project's `build` directory if you aren't already there:
    ```bash
    ./healthcare
    ```

---

## 🧪 Running Tests (Optional)

If your project includes CTest configurations:
```bash
cd build  # Ensure you are in the build directory
ctest

🚀 Usage

Once the application is running, you'll be greeted by an interactive menu. Use the following commands:

add: Prompts for patient details to add a new patient record.

search: Allows searching for patients by patient_id, name, condition, or visit_date.

update: Prompts for a patient_id and the field to update, then the new value.

delete: Prompts for a patient_id to delete the corresponding patient record.

stats: Displays a report of patient counts, grouped by condition and sorted.

exit: Quits the application.

Example Interaction:
Healthcare Patient Management System
-------------------------------------
Commands: add, search, update, delete, stats, exit
Enter command: add
Enter patient_id: P001
Enter name: John Doe
Enter gender (Male/Female/Other): Male
Enter age: 30
Enter phone (10 digits): 1234567890
Enter email: john.doe@example.com
Enter allergies (comma-separated, or none): Peanuts
Enter medical history: Asthma
Enter condition: Flu
Enter visit date (YYYY-MM-DD): 2025-06-01
Patient P001 added successfully.

Enter command: search
Search by (id, name, condition, date): name
Enter name to search: John Doe
Found 1 patient(s):
ID: P001, Name: John Doe, Age: 30, Gender: Male, Phone: 1234567890, Email: john.doe@example.com, Allergies: Peanuts, History: Asthma, Condition: Flu, Visit: 2025-06-01

Enter command: stats
Patient Count by Condition:
- Flu: 1
- ... (other conditions and counts)

Enter command: exit
Exiting application.

📁 Project Structure

A typical layout for the project:

healthcare-patient-management-cpp/
├── CMakeLists.txt          # Main CMake build script
├── README.md               # This file
├── include/                # Project-specific header files (.h, .hpp)
│   └── patient_manager.hpp
│   └── validation.hpp
│   └── ...
├── src/                    # Source files (.cpp)
│   └── main.cpp
│   └── patient_manager.cpp
│   └── validation.cpp
│   └── ...
├── build/                  # Build output (created by CMake)
├── data/                   # MongoDB data directory (created by you)
└── tests/                  # Unit tests (optional)
    └── test_validation.cpp
    └── ...
