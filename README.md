📚 Course Registration & Grade Prediction System (C++)

This project is a **Course Registration and Grade Prediction System** built in **C++**, demonstrating advanced usage of **data structures, arrays, file handling, text parsing, similarity calculation, and algorithmic logic**.  

It reads student academic data from files, compares grades using a distance-based algorithm, and predicts the student's performance in **Data Structures** based on similar students.

🚀 Features

✔ Reads student's own academic history (My-data.txt)  
✔ Reads multiple students’ records from CSV (Student-data.csv)  
✔ Uses similarity algorithm to find closest matching students  
✔ Predicts expected grade in **Data Structures**  
✔ Processes large datasets efficiently  
✔ Loads, parses, and compares course titles & grades  
✔ Uses weighted distance formula  
✔ Demonstrates data structure concepts through arrays & string handling  

🧠 Concepts Used

- 2D & 3D character arrays  
- File handling (`fstream`, `getline`, `strtok`)  
- String tokenization  
- Similarity algorithm (distance calculation)  
- Grade-to-score mapping  
- Searching & matching titles and course codes  
- Data Structures knowledge  
- Modular programming

  Course-Registration-System
│── CourseRegistration.cpp # Main C++ source code
│── My-data.txt # Student’s personal academic record
│── Student-data.csv # Dataset of other students
│── README.md

🔍 How the Prediction Works

1️⃣ Reads your completed courses (semester, code, title, grade)  
2️⃣ Reads CSV data of many students  
3️⃣ Converts grades into numeric scores  
4️⃣ Calculates difference between your grades and each student  
5️⃣ Applies extra weight for CS courses  
6️⃣ Selects **k closest students**  
7️⃣ Finds the most common grade among them  
8️⃣ Predicts your expected **Data Structures** grade  

👩‍💻 Author

Manahil Rizvi 
BS Computer Science (UCP)  
5th Semester  
