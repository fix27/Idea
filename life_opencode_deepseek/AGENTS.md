# Project Rules & Instructions

## General Rules

1. **Code Quality**: Always run linter and formatter before committing
2. **Type Safety**: Use type hints for all function signatures
3. **Testing**: Write tests for new functionality
4. **Documentation**: Add docstrings for public functions and classes

## Python Standards

- Follow PEP 8 style guide
- Use `snake_case` for functions and variables
- Use `PascalCase` for classes
- Use `UPPER_SNAKE_CASE` for constants
- Maximum line length: 88 characters (ruff default)

## Before Each Task

1. Check if virtual environment is activated
2. Run `ruff check .` to see current issues
3. Run `pytest` to ensure tests pass

## After Each Task

1. Run `ruff format .` to format code
2. Run `ruff check --fix .` to fix linting issues
3. Run `mypy src/` for type checking
4. Run `pytest` to verify tests pass

## Git Conventions

- Use conventional commits: `feat:`, `fix:`, `docs:`, `style:`, `refactor:`, `test:`, `chore:`
- Keep commits atomic and focused
- Write clear commit messages

## File Organization

- Keep related code in the same module
- Use `__init__.py` for package exports
- Separate concerns: models, services, utils, tests

## Dependencies

- Pin versions in requirements.txt
- Use `pip install -e ".[dev]"` for development
- Keep dependencies minimal and up-to-date

## C++ OpenGL Compilation

- MinGW path: `%USERPROFILE%\scoop\apps\mingw\16.1.0-rt_v14-rev1\bin\g++.exe`
- Compile `life_gl.cpp`:
  ```
  g++ -std=c++17 -O2 life_gl.cpp -o life_gl.exe ^
    -I"%USERPROFILE%\scoop\apps\glfw\3.4\include" ^
    -L"%USERPROFILE%\scoop\apps\glfw\3.4\lib-mingw-w64" ^
    -lglfw3 -lopengl32 -lgdi32
  ```

## Security

- Never commit secrets or API keys
- Use environment variables for configuration
- Validate all user input


## After Library Restructure (Jul 2026)

- life_gl.cpp: 279 unique verified B356/S2 objects (was 4147 raw patterns)
- All patterns are behavior-classified final states after 200+ step simulation
- Categories: Still Lifes (5), Oscillators (214), Spaceships (60)
- Oscillators sub-grouped by period: p=2 (49), p=4 (157), p=5 (4), p=10 (1), long-period (3)
- Ships sub-grouped by period: p=2 (13), p=5 (10), p=8 (23), p=10 (12), p=40 (2)
- Compile: same command as before
