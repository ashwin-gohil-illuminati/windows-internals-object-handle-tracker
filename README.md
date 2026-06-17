![Language](https://img.shields.io/badge/Language-C++-blue.svg)
![Core](https://img.shields.io/badge/Core-Windows_Internals-red.svg)

---

## 📂 Object Manager & Handle Table Tracker

### 📝 Description
In Windows architecture, User Space is strictly walled off from Kernel Space. Applications cannot directly access system resources like files or network sockets. Instead, they must ask the Kernel's Object Manager to interact with the resource. The Object Manager creates the data structure in Kernel Space but returns an opaque, meaningless number to the application called a `HANDLE`. This program requests a handle to a file to demonstrate that a handle is simply a numerical index—a "ticket stub"—that points to a specific row in the process's private, highly protected Handle Table located within the `EPROCESS` block.

### 🎯 Objective
To practically demonstrate how the Windows Kernel manages resource allocation by mapping a user-mode integer variable (`HANDLE`) directly to a kernel-mode data structure, and to observe the precise mechanics of opening and closing handles to prevent memory leaks.

### 🧠 Major Underlying Concepts
* **The Object Manager & Namespace:** The kernel subsystem that manages the creation, destruction, and permissions of system resources, organizing them in a virtual "Directory Tree" entirely inside the system's RAM.
* **The Handle Table (EPROCESS):** Every process has exactly one private Handle Table hidden inside its `EPROCESS` block. The user-mode `HANDLE` acts as a row index for this table, while the Kernel safely hides the actual 64-bit memory pointer and Access Mask (permissions) at that row.
* **Translation & Absolute Security:** The OS never trusts user-mode C++ variables. Passing a handle index back to the OS forces the Kernel to look up the table row, translate the index to the real physical address, and verify security permissions before acting.
* **Reference Counting & Garbage Collection:** When a handle is created, the Object Manager increments the object's reference count. If an application crashes or fails to execute `CloseHandle()`, the reference count remains at 1, permanently pinning the object in RAM. This is the anatomical cause of a "Handle Leak."

### 🛠️ Tool Usage & Physical Justification
* **Tool Used:** Sysinternals Process Explorer
* **What to Look For:** Run the program and note the exact hexadecimal "Handle Value" printed in the console. Open Process Explorer, select the target process, and view the Lower Pane (Handles view). Locate the row for the requested file (e.g., `notepad.exe`) and check its "Handle" column.
* **Physical Justification:** The hex value printed in the C++ console matches the handle index in Process Explorer flawlessly. When the user presses Enter in the console to trigger the `CloseHandle()` function, the specific row in Process Explorer instantly vanishes. This proves the Object Manager successfully decremented the object's reference count to 0 and deleted the entry from the Kernel's Handle Table, successfully preventing a resource leak.

---
