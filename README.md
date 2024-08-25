# calculator-SDL2
# SDL2 Calculator

A simple calculator application developed using SDL2 and SDL_ttf. This calculator includes various mathematical functions and operations, such as square root, pi, exponentiation, trigonometric functions, and basic arithmetic operations.

## Features

- **Basic Arithmetic Operations**: Addition, subtraction, multiplication, and division.
- **Mathematical Functions**: Square root, exponentiation, factorial, and more.
- **Trigonometric Functions**: Sine, cosine, and tangent.
- **Advanced Features**: Decimal points, parentheses, percentage calculations, and more.

## Getting Started

### Prerequisites

To build and run this project, you need to have the following installed:

- SDL2
- SDL2_ttf

You can install these libraries using your package manager or from the official SDL website.

### Installation

1. **Clone the repository**:

   ```sh
   git clone https://github.com/Zreechxnn/calculator-SDL2.git
   cd calculator-SDL2
Install dependencies:

On Ubuntu/Debian-based systems, you can install SDL2 and SDL2_ttf using:

sh
Copy code
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
On Fedora-based systems:

sh
Copy code
sudo dnf install SDL2-devel SDL2_ttf-devel
Build the project:

You can use gcc to compile the project:

sh
Copy code
gcc -o calculator main.cpp -lSDL2 -lSDL2_ttf -lm
Run the application:

sh
Copy code
./calculator
Usage
Basic Operations: Click the buttons to input numbers and operators. Click the = button to calculate the result.
Advanced Functions: Use buttons like sqrt, pi, ^, !, deg, sin, cos, tan, and AD for advanced calculations.
Clear and Delete: Click D to delete the last character and 0 to reset the input.
Troubleshooting
Font Issues: Ensure arial.ttf is present in the same directory as the executable or adjust the path in the source code.
Library Errors: Verify that SDL2 and SDL2_ttf libraries are installed and properly linked during compilation.
Contributing
Feel free to open issues or submit pull requests if you find bugs or have suggestions for improvements.

License
This project is licensed under the MIT License. See the LICENSE file for details.

Acknowledgments
SDL2 - Simple DirectMedia Layer
SDL2_ttf - SDL2 font rendering library
