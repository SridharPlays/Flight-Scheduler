# OS Principles Simulation: Air Traffic Control Scheduler

This project provides a practical simulation of operating system process scheduling principles using the analogy of an Air Traffic Control (ATC) system. It demonstrates how a scheduler manages competing processes (aircraft) for a limited resource (a single runway) based on dynamic priorities.

The project is available in two forms:

1.  **A live, interactive web application** built with HTML and Tailwind CSS.
2.  **A console-based simulation** written in C++.

-----

## ✨ Core Concepts Demonstrated

### **1. Priority-Based Scheduling**

Each aircraft is treated as a process and is assigned a priority. The scheduler always selects the process with the highest current priority to allocate the resource (the runway). The base priority is determined by the flight's type, as shown below:

| Priority Level | Flight Type     | Analogy             |
| :------------- | :-------------- | :------------------ |
| **Highest** | Emergency       | High-Priority Interrupt |
| High           | Low Fuel        | Critical Process    |
| Medium-High    | VIP             | System Process      |
| Medium         | International   | I/O Bound Process   |
| Low            | Domestic        | Normal Process      |
| **Lowest** | Cargo           | Batch Process       |

### **2. Aging**

To prevent **starvation** (where low-priority processes are never served), an aging mechanism is implemented. For every cycle an aircraft waits in the queue, its priority is dynamically increased by a fixed factor. This ensures that even the lowest priority flights will eventually have their priority raised high enough to be scheduled.

-----

## 🚀 Implementations

### **1. Live Web Application (`index.html`)**

An interactive web-based simulation that visualizes the ATC scheduler in real-time.

**Features:**

  - **Real-time View:** See the current runway status and the full waiting queue.
  - **Interactive Controls:** Dynamically add new flights through a user-friendly form.
  - **Live Logging:** A simulation log shows every major event as it happens.
  - **Visual Cues:** Color-coded priority levels make it easy to spot critical flights.

**How to Run:**

  - Simply open the `index.html` file in any modern web browser (like Chrome, Firefox, or Edge). No additional setup is required.

### **2. C++ Console Simulation (`atc_scheduler.cpp`)**

A command-line application that runs the simulation cycle by cycle, printing the status of the runway and waiting queue at each step.

**Features:**

  - **Clear Console Output:** Follow the step-by-step logic of the simulation.
  - **Backend Focused:** Demonstrates the core scheduling algorithm in a clean environment.
  - **Dynamic Simulation:** Includes pre-defined flights and adds new ones during the run.

**How to Compile and Run:**

1.  You need a C++ compiler (like G++).
2.  Open a terminal or command prompt.
3.  Compile the program using the command:
    ```bash
    g++ -o atc_scheduler atc_scheduler.cpp -std=c++11 -pthread
    ```
4.  Run the compiled executable:
    ```bash
    ./atc_scheduler
    ```

-----
This project effectively models key OS concepts in a relatable, real-world scenario, making it an excellent tool for understanding the complexities of process scheduling.
