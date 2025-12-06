# SRMS-Project--Role-Based-Student-Management-System-Simulation
Student Management System (C Project)

Student Management System (C Language)
A console-based Student Management System built in C, featuring user authentication, role-based access control (Admin, Staff, Guest), and persistent data storage using text files.

Features

Role-Based Access Control: Users can log in as ADMIN, STAFF, or GUEST, each with specific permissions.

Persistent Storage: Student records and user credentials are saved in dedicated text files (students.txt and credentials.txt).

Student Operations: Add, Display, Search, Update, and Delete student records.

Data Analysis: Sort students, find Top N performers, and filter by marks range or name substring.

Security: Password masking during login using getch().

Account Management: Users can change their password.

Utility: Counts total students and tracks last login time.

🛠️ Prerequisites
To compile and run this program, you will need a C compiler (like GCC) and the ability to use the conio.h library for the password masking feature (getch()).

Compiler: GCC or equivalent.

Operating System: Typically requires a Windows environment or a Linux/macOS environment with a compatibility layer (like ncurses or a custom getch implementation) to support conio.h for the password masking feature.

 File Structure
 
 students.txt- Stores all student records (Roll, Name, Marks).
 
 credentials.txt- Stores user login details (Username, Password, Role, Last Login).
 
 temp.txt- Temporary file used during Update, Delete, and Password Change operations.

System Functions

loginSystem(): Handles user authentication against credentials.txt with up to 3 tries and password masking.

changePassword(): Allows the currently logged-in user to change their password after verifying the old one.

saveLastLogin(): Updates the last login timestamp in the credentials.txt file for the current user.

Code Structure Notes

struct Student: Used to hold a single student's data (roll, name, marks).

struct User: Used to hold a user's credentials (username, password, role, lastLogin).

File I/O Technique: Most record manipulation functions (updateStudent, deleteStudent, saveLastLogin, changePassword) employ the standard C practice of reading from the main file, writing to a temporary file (temp.txt), and then using remove() and rename() to replace the main file with the updated temporary file.

Sorting: The sortStudents() and topNStudents() functions read all records into an array of structs (students[MAX_STUDENTS]), perform an in-memory Bubble Sort, and then display the results.
