# File Management System (FMS)

## Table of Contents

1. [Overview](#overview)
2. [General Information](#general-information)
3. [Tools Used](#tools-used)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Project Structure](#project-structure)
7. [Contributors](#contributors)

---

## Overview
The **File Management System (FMS)** is a project that simulates the core functionality of a file management system, an essential component of operating systems. It models fundamental principles such as organizing and managing files on secondary memory (disk storage).

### Key Features
- **Memory Organization:** Simulates memory using blocks.
- **File Space Allocation:** Handles file storage and management.
- **File Metadata Management:** Tracks information about files.
- **Basic File Operations:** Includes creating, inserting, searching, and deleting files and records.

---

## General Information

### Features
- **Interactive Menu:** Offers options to initialize memory, manage files, view memory status, and perform operations.
- **Memory Visualization:** Displays free and occupied memory blocks graphically.
- **Metadata Viewing:** Allows inspection of file metadata.
- **File Operations:**
  - Add, search, insert, delete, and rename files or records.
  - Logical and physical deletion options.
- **Memory Management:** Includes defragmentation and space management features.

---

## Tools Used
- **C Language:** Implementation of file management logic and memory handling.
- **Raylib:** Provides graphical interface and interactivity.  
  [Raylib Official Site](https://www.raylib.com)

![C Language Logo](/images/Clogo.png)  
![Raylib Logo](/images/raylib_180x180.png)

---

## Installation

### Prerequisites
- **C Compiler:** GCC for Linux, MinGW for Windows.
- **Raylib Installed.**

### Installation Steps
### Linux

#### Linux
1. Clone the repository:
   ```bash
   git clone https://github.com/wassimmho/Project-SFSD
   cd Project-SFSD
    ```
2.  Build and Run the project :

        ./build/build.sh

### Windows

1.  Clone the repository :

    ```bash
        git clone https://github.com/wassimmho/Project-SFSD
        cd Project-SFSD
    ```

2.  Build and Run the project :

        ./build/build.bat

## Usage

This project provides an intuitive interface for managing files and records, offering the following powerful features:

- **Add Files:**  
  Easily add new files or records to the system.

- **Delete Files/Records:**  
  Files and records can be deleted in two ways:  
  - **Logical Deletion:** Marks the file or record as deleted without removing it from the system.  
  - **Physical Deletion:** Completely removes the file or record from the system.

- **Insert Files:**  
  Insert new files at any specified location within the project structure.

- **Rename Files:**  
  Rename files to improve organization and clarity.

- **View Files/Records:**  
  View detailed information or content of selected files or records.

- **Search:**  
  Quickly search for files or records by name or other attributes to find what you're looking for.

- **Free MS (Memory Space):**  
  This section allows users to manage all files and records freely, offering the flexibility to modify, move, or organize them without restrictions.

- **Save Changes:**  
  Ensure all modifications made to the files, records, or structure are saved for future use.


## Project Structure
An explanation of the project files and directories.

```plaintext
SFSD/
├── build/
│   ├── build.bat
│   ├── build.sh
├── debug/
│   └── myprogram.exe
├── Exe file/
│   ├── w64devkit.exe
│   └── w64devkit.ini
├── images/
│   ├── Clogo.png
│   └── raylib_180x180.png
├── include/
│   ├── iconset.rgi.h
│   ├── raygui.h
│   └── raylib.h
├── Project-SFSD/
│   ├── output/
│   ├── FMS_Main.c
│   ├── version1.exe
│   ├── version2.c
├── Wlib/
│   └── libraylib.a
└── README.md
```

## Contributors

**Mouhouche Wassim** - [Mouhouche Wassim](https://github.com/wassimmho)<br>
**Mouzali Hachemi Rayan** - [Mouzali Hachemi Rayan](https://github.com/rayan3230)<br>
**Mehdi Benazza** - [Mehdi Benazza](https://github.com/MehdiBenazza)<br>
**Fahd sifeddine Djedi** - [Fahd sifeddine Djedi](https://github.com/FahdDjedi)<br>
**stambouli eleisse Abdellmouhssine** - [stambouli eleisse Abdellmouhssine](https://github.com/stamboulieleisse)<br>
