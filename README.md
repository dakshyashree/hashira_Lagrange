# Hashira Placements – Polynomial Secret Finder (C++)

Recover the constant term `f(0)` (“secret”) of a polynomial from given roots encoded in various bases.  
This program implements exact **Lagrange interpolation at x = 0** using big integers.

---

## ✨ Features
- Parses testcases in **JSON**
- Arbitrary-precision integers via **Boost.Multiprecision (`cpp_int`)**
- JSON parsing with **nlohmann/json** (single-header)
- Exact rational arithmetic (no floating-point errors)
- Prints the secret (`f(0)`) in decimal

---

## 📂 Project Structure
.
├─ CMakeLists.txt
├─ src/
│ └─ main.cpp
├─ include/
│ └─ nlohmann/
│ └─ json.hpp
├─ case1.json
└─ case2.json


> **Boost headers are external**, e.g. `C:/Users/<you>/Downloads/boost_1_89_0/` on Windows.

---

## 🧰 Dependencies
- **C++17** compiler (GCC/Clang/MinGW).  
  (If using MSVC, replace `<bits/stdc++.h>` with standard headers.)
- **Boost.Multiprecision** (header-only)  
  Download from [boost.org](https://www.boost.org/users/download/) and extract.
- **nlohmann/json** (single-header)  
  Download from [GitHub](https://github.com/nlohmann/json) and place at `include/nlohmann/json.hpp`.

---

## ⚙️ Build

### CLion
1. Place `json.hpp` at `include/nlohmann/json.hpp`.
2. Extract Boost headers, e.g. `C:/Users/you/Downloads/boost_1_89_0/`.
3. Update `CMakeLists.txt`:

   ```cmake
   cmake_minimum_required(VERSION 3.16)
   project(secret_finder)

   set(CMAKE_CXX_STANDARD 17)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   add_executable(secret_finder src/main.cpp)
   target_include_directories(secret_finder PRIVATE ${CMAKE_SOURCE_DIR}/include)
   target_include_directories(secret_finder PRIVATE "C:/Users/you/Downloads/boost_1_89_0")
   ```
4. Reload CMake Project in CLion, then Build.
    ```bash
   g++ -std=gnu++17 -O2 src/main.cpp -Iinclude -I"C:/path/to/boost_1_89_0" -o secret
    ```
5. Run
   ```bash
   ./secret case1.json
    ./secret case2.json
   ```
   
