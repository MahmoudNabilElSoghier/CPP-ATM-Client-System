# 🏧 ATM Client System – C++

![C++](https://img.shields.io/badge/C%2B%2B-11-blue)
![License](https://img.shields.io/badge/License-MIT-green)

A console-based ATM client application written in **C++** using structured programming.

## 🔧 Features

- Secure login with account number & PIN
- Quick withdraw (preset amounts)
- Normal withdraw (multiples of 5)
- Deposit funds
- Check balance
- Data persistence via text file (`clientData.txt`)

## 📁 Project Structure

```text
📦 ATM-Client-CPP
 ┣ 📜 ATMsystem.cpp
 ┣ 📜 clientData.txt
 ┗ 📜 README.md
```
## 🚀 How to Run
```bash
g++ -std=c++11 ATMsystem.cpp -o atm
./atm
```
## 🔗 Integration
This application shares the same `clientData.txt` file with the [Bank Management System](https://github.com/MahmoudNabilElSoghier/Bank-Management-CPP), simulating a real-world ATM connected to a central bank database.

## 🛠️ Technologies & Concepts
- C++17
- File I/O (`fstream`)
- Vectors & Structs
- Input validation
- Procedural programming

## 👤 Author
#### Mahmoud Nabil El-Soghier
[GitHub](https://github.com/MahmoudNabilElSoghier)ئ