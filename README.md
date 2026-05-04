# HealthCare App — CSCE 1102 Capstone Project

A Qt-based healthcare management application featuring medication reminders,
emergency alerts, real-time networking, SQLite persistence, and an AI health
assistant powered by Claude (Anthropic).

---

## Features

- **Login screen** with input validation
- **Medication reminders** — add, view, and manage medications
- **Emergency alerts** — send emergency contact notifications
- **AI Health Assistant** — ask health questions powered by Claude API
- **Async TCP networking** — client/server using Boost.Asio
- **SQLite database** — persistent storage for users and medications
- **43 unit tests** — GoogleTest + GoogleMock

---

## Requirements

- Qt 6.x
- CMake 3.16+
- Boost (Asio + Beast)
- OpenSSL
- SQLite3
- GoogleTest / GoogleMock
- C++17 compiler

---

## Build Instructions

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Run the App

```bash
# Set your Anthropic API key for the AI Assistant feature
export ANTHROPIC_API_KEY=your_key_here

./build/HealthCareApp
```

Default login credentials: **admin / 1234**

## Run Tests

```bash
./build/HealthCareTests

# Or using CTest (as taught in the Unit Testing slides)
ctest --test-dir build
```

## CI/CD

This project uses **GitHub Actions** — every push automatically builds
the project and runs all unit tests. See `.github/workflows/build.yml`.

---

## Project Structure

```
HealthCareApp/
├── .github/workflows/build.yml   # GitHub Actions CI
├── CMakeLists.txt
├── src/
│   ├── gui/          # Qt windows and dialogs
│   ├── core/         # Business logic (no GUI/network code)
│   ├── network/      # Boost.Asio async TCP + JSON
│   ├── database/     # SQLite persistence
│   └── utils/        # Input validation
└── tests/            # GoogleTest + GoogleMock unit tests
```
