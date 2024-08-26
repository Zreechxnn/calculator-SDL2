# calculator-SDL2
# SDL2 Calculator



A simple calculator application developed using SDL2 and SDL_ttf. This calculator includes various mathematical functions and operations, such as square root, pi, exponentiation, trigonometric functions, and basic arithmetic operations.
![image](https://e.top4top.io/p_31596566j1.png)

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
   ```bash
   git clone https://github.com/Zreechxnn/calculator-SDL2.git
   cd calculator-SDL2
   ```

2. **Install dependencies**:

   - On Ubuntu/Debian-based systems:
     ```bash
     sudo apt-get install libsdl2-dev libsdl2-ttf-dev
     ```
   - On Fedora-based systems:
     ```bash
     sudo dnf install SDL2-devel SDL2_ttf-devel
     ```

3. **Build the project**:
   ```bash
   gcc -o calculator main.cpp -lSDL2 -lSDL2_ttf -lm
   ```

4. **Run the application**:
   ```bash
   ./calculator
   ```

## Usage

- **Basic Operations**: Click the buttons to input numbers and operators. Click the `=` button to calculate the result.
- **Advanced Functions**: Use buttons like `sqrt`, `pi`, `^`, `!`, `deg`, `sin`, `cos`, `tan`, and `AD` for advanced calculations.
- **Clear and Delete**: Click `D` to delete the last character and `0` to reset the input.

## Troubleshooting

- **Font Issues**: Ensure `arial.ttf` is present in the same directory as the executable or adjust the path in the source code.
- **Library Errors**: Verify that SDL2 and SDL2_ttf libraries are installed and properly linked during compilation.

## Contributing

Feel free to open issues or submit pull requests if you find bugs or have suggestions for improvements.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [SDL2](https://www.libsdl.org/) - Simple DirectMedia Layer
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) - SDL2 font rendering library


