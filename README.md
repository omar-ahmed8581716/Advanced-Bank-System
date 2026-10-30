# 🏦 Advanced Bank & Client Management System (C++)

A robust, console-based Banking Management System built with modern C++. This project is an advanced extension featuring dual-role authentication (Staff/Admin & Clients), dynamic role-based access control (RBAC), self-service client onboarding workflows, and secure file persistence.

---

## 🌟 Key Features

### 👤 1. Client Portal (Self-Service ATM/Banking)
- **Account Dashboard:** View personal details, account number, linked phone, and current balance.
- **Transactions:** Real-time deposits and balance-guarded withdrawals.
- **Security:** Change 6-digit PIN code with validation checks.
- **Session Control:** Secure logout and session termination.

### 🛡️ 2. Administrative & Staff Portal
- **Client Management:** Add, update, search, and delete client profiles.
- **User & Permissions Management:** Fine-grained bitwise permission system granting access per operational module (Clients, Transactions, Approvals, User Management).
- **Transaction Operations:** Execute deposits and withdrawals on behalf of clients, view aggregated system balances.
- **System Safeguards:** Protection against accidental deletion of the root `admin` or active sessions.

### 📝 3. Client Onboarding & Registration Workflow
- **Client Self-Registration:** Prospective clients can submit account requests with custom account numbers and contact info.
- **Request Verification:** Prevents account duplication against active accounts and pending requests.
- **Status Inquiry:** Allows applicants to check whether their application is pending, approved, or rejected.
- **Admin Review Queue:** Staff can review, accept, or reject registration requests in real time.

---

## 🛠️ Built With

- **Language:** C++ (C++11 or higher)
- **Concepts Applied:**
  - Dynamic Memory & Standard Template Library (`std::vector`, `std::string`, `std::streamsize`)
  - Bitwise Flag Permissions (`enum enPermissions`)
  - File Streams & Custom Record Delimiters (`#//#`)
  - Input Validation & Stream Cleansing (`cin.fail()`, buffer flush)
  - Modular Procedural Architecture

---

## 📁 File Structure

```text
├── main.cpp                  # Main application source code
├── DataInfo.txt              # Client records database
├── Users.txt                 # Admin & staff credentials and permissions
├── RegistrationRequests.txt  # Pending client registrations
└── README.md                 # Project documentation
```

---

## 🚀 Getting Started

### Prerequisites
Make sure you have a C++ compiler installed (GCC/Clang or MSVC via Visual Studio).

### Compilation & Execution

#### Using GCC / MinGW:
```bash
# 1. Compile the code
g++ -std=c++11 -O2 main.cpp -o AdvancedBankSystem

# 2. Run the application
./AdvancedBankSystem
```

#### Using Visual Studio:
1. Open the project folder or add `main.cpp` into a new C++ Console Application project.
2. Ensure `DataInfo.txt`, `Users.txt`, and `RegistrationRequests.txt` reside in the working directory (beside the executable).
3. Build and run (`Ctrl + F5`).

---

## 🔐 Default Access (Testing)

| Role | Username / Account | Password / PIN | Permissions |
| :--- | :--- | :--- | :--- |
| **Admin** | `admin` | *(Defined in Users.txt)* | Full Access (`-1`) |

---

## 📄 License
This project is open-source and available under the [MIT License](LICENSE).
