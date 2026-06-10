# 2D Graphics Editor in C

A modular, menu-driven terminal 2D Graphics Editor implemented in C. The editor represents graphical objects dynamically via a shape database and renders them onto a character-based 2D grid canvas (initially filled with `_` and drawn with `*`).

This application supports drawing:
- **Lines** (using Bresenham's Line Algorithm)
- **Rectangles** (using outline bounding coordinates)
- **Circles** (using Bresenham's Midpoint Circle Algorithm)
- **Triangles** (using boundary lines between vertices)

Users can dynamically **add**, **modify**, and **delete** objects from the active canvas.

---

## File Structure

- [shapes.h](file:///c:/Users/SEENIVASAN/Desktop/mini_project/shapes.h): Header declaring the geometry structures (Line, Rectangle, Circle, Triangle), shape union type, and drawing function signatures.
- [shapes.c](file:///c:/Users/SEENIVASAN/Desktop/mini_project/shapes.c): Implementation of Bresenham's rasterization algorithms for lines and circles, rectangles, and triangles.
- [canvas.h](file:///c:/Users/SEENIVASAN/Desktop/mini_project/canvas.h): Header specifying the canvas sizing (`CANVAS_WIDTH` = 60, `CANVAS_HEIGHT` = 20) and operations.
- [canvas.c](file:///c:/Users/SEENIVASAN/Desktop/mini_project/canvas.c): Implementation of the canvas array buffer clearing, shape render loop, and the grid axis display (with coordinate headers).
- [editor.c](file:///c:/Users/SEENIVASAN/Desktop/mini_project/editor.c): Main program driver, menu system, safe integer input validators, and shape catalog storage management.
- [build.bat](file:///c:/Users/SEENIVASAN/Desktop/mini_project/build.bat): Automated build batch file for Windows CMD.
- [build.ps1](file:///c:/Users/SEENIVASAN/Desktop/mini_project/build.ps1): Automated build script for Windows PowerShell.

---

## Core Algorithms & Math

1. **Bresenham's Line Algorithm**:
   Plots a line segment on a pixelated grid from $(x_1, y_1)$ to $(x_2, y_2)$ using only integer addition, subtraction, and bit shifting. This avoids expensive floating-point division or multiplication, making it highly efficient.
2. **Bresenham's Midpoint Circle Algorithm**:
   Plots the outline of a circle around a center $(x_c, y_c)$ and radius $r$. It computes points for a single octant ($y \ge x$) and uses 8-way symmetry to plot points across all 8 octants simultaneously.

---

## How to Install a C Compiler on Windows

Since Windows does not ship with a C compiler by default, follow these steps to install one:

### Option 1: MinGW-w64 via MSYS2 (Recommended)
1. Go to [https://www.msys2.org/](https://www.msys2.org/) and download the installer.
2. Run the installer and complete the setup (default path is `C:\msys64`).
3. Once completed, a terminal named **MSYS2 UCRT64** will open.
4. Run the following command in the MSYS2 terminal to install the GCC compiler:
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   ```
5. Confirm the installation (type `Y` and press Enter).
6. Next, add the compiler to your Windows system PATH:
   - Search for **"Edit the system environment variables"** in the Windows Start Menu.
   - Click on **"Environment Variables..."** at the bottom.
   - In the "System variables" section, select the **Path** variable and click **"Edit..."**.
   - Click **"New"** and add: `C:\msys64\mingw64\bin` (or `C:\msys64\ucrt64\bin` depending on MSYS2 default).
   - Click **"OK"** to close all windows.
7. Open a new Command Prompt or PowerShell window and verify by typing:
   ```cmd
   gcc --version
   ```

### Option 2: Visual Studio Community
1. Download Visual Studio Installer from [https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/).
2. Select **"Desktop development with C++"** workload during installation.
3. Once complete, you can compile from the Developer Command Prompt using the `cl` command.

---

## How to Build and Run the Editor

Once a compiler is installed:

### Automatic Build (Windows)
Double-click `build.bat` or run `.\build.ps1` in a PowerShell console:
```powershell
# In PowerShell:
.\build.ps1
```
This automatically locates your compiler (`gcc`, `clang`, or `cl`) and compiles the sources into `graphics_editor.exe`.

### Manual Compilation
Alternatively, compile manually with GCC from your terminal:
```cmd
gcc -o graphics_editor.exe editor.c shapes.c canvas.c -Wall -Wextra -std=c99
```

### Running the Application
Type the following command in your terminal to start the editor:
```cmd
graphics_editor.exe
```

---

## Alternative: Online Compilation
If you prefer not to install any local software, you can run the program in an online compiler:
1. Open [https://www.onlinegdb.com/](https://www.onlinegdb.com/)
2. Set the Language dropdown in the top-right to **C**.
3. Create the five project files (`shapes.h`, `shapes.c`, `canvas.h`, `canvas.c`, `editor.c`) and copy-paste their corresponding contents into the tabs.
4. Click **Run** to launch and interact with the editor.
