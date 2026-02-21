# AGENTS.md

This file provides essential information for agentic coding agents operating in this ESP32 experiments repository.

## Project Overview

This repository contains multiple ESP-IDF projects for ESP32 microcontroller experiments. Each subdirectory is a separate ESP-IDF project with its own CMakeLists.txt and source files.

## Build System

### ESP-IDF Commands

**Core Build Commands:**
- `idf.py build` - Build the project
- `idf.py clean` - Clean the build directory
- `idf.py fullclean` - Clean everything including downloaded components
- `idf.py menuconfig` - Open project configuration menu
- `idf.py set-target <chip_name>` - Set target chip (esp32, esp32s3, etc.)

**Flash and Monitor:**
- `idf.py -p PORT flash` - Build and flash to device
- `idf.py -p PORT flash monitor` - Build, flash, and start serial monitor
- `idf.py monitor` - Start serial monitor only
- `idf.py -p PORT erase_flash` - Erase entire flash

**Size Analysis:**
- `idf.py size` - Show component sizes
- `idf.py size-components` - Show per-component sizes
- `idf.py size-files` - Show per-file sizes

### Project Structure
Each project follows ESP-IDF structure:
```
project_name/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   └── main.c (or *_main.c)
├── components/ (optional)
│   └── component_name/
│       ├── CMakeLists.txt
│       ├── include/
│       └── *.c
├── pytest_*.py (test files)
└── wokwi.toml (Wokwi simulation config)
```

## Testing

### Python Test Framework
Projects use `pytest` with `pytest-embedded-idf` for testing.

**Test Commands:**
- `pytest` - Run all tests in current directory
- `pytest <test_file>.py` - Run tests in specific file
- `pytest <test_file>.py::<test_function>` - Run single test
- `pytest -v` - Verbose output
- `pytest -k "test_name"` - Run tests matching pattern

**Test Examples from Codebase:**
- `pytest pytest_hello_world.py::test_hello_world`
- `pytest pytest_blink.py::test_blink`

**Test Dependencies:**
- pytest
- pytest-embedded-idf
- pytest-embedded-qemu (for QEMU tests)

## Code Style Guidelines

### C Language Style

**General Formatting:**
- Use 4-space indentation (no tabs)
- Opening braces on same line
- Space after keywords (if, while, for)
- Space around operators
- Maximum line length: 120 characters

**Example from codebase:**
```c
void app_main(void)
{
    printf("Hello world!\n");
    
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s)", 
           CONFIG_IDF_TARGET, chip_info.cores);
}
```

**Naming Conventions:**
- `snake_case` for variables and functions
- `UPPER_SNAKE_CASE` for macros and constants
- `camelCase` for types (esp_chip_info_t)
- Prefix private functions with `static`
- Use descriptive names: `button_isr_handler` not `btn_isr`

**Error Handling:**
- Use `ESP_ERROR_CHECK()` for ESP-IDF API calls
- Check return values from functions
- Log errors with `ESP_LOGE()` or `ESP_LOGW()`
- Include error context in log messages

**Example error handling:**
```c
if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    printf("Get flash size failed");
    return;
}
```

### Header Files
- Use include guards: `#ifndef COMPONENT_NAME_H`
- Include only necessary headers
- Group includes: system, ESP-IDF, local
- Forward declare when possible

**Example header structure:**
```c
#ifndef BUTTON_LED_H
#define BUTTON_LED_H

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

void button_led_init(void);
void led_set_state(bool state);

#endif
```

### Logging
- Use appropriate log levels: `ESP_LOGI`, `ESP_LOGW`, `ESP_LOGE`
- Include module tag: `static const char *TAG = "module_name";`
- Log state changes and errors
- Avoid excessive logging in production code

### Interrupt Service Routines
- Use `IRAM_ATTR` for ISR functions
- Keep ISRs short and fast
- Use FreeRTOS APIs from ISR (suffix `FromISR`)
- Implement debouncing for buttons

**Example ISR:**
```c
static void IRAM_ATTR button_isr_handler(void* arg)
{
    gpio_num_t gpio_num = (gpio_num_t)arg;
    static uint32_t last_press = 0;
    uint32_t now = xTaskGetTickCount();
    
    if ((now - last_press) > pdMS_TO_TICKS(500)) {
        last_press = now;
        // Handle button press
    }
}
```

### Memory Management
- Use stack for small, short-lived variables
- Use heap for large or persistent data
- Check return values from malloc/calloc
- Free allocated memory when done
- Use ESP-IDF memory debugging tools if needed

### Configuration
- Use `sdkconfig.h` for project configuration
- Define constants with `#define` or `enum`
- Use Kconfig for user-configurable options
- Group related configuration options

## Python Test Style

### Test Functions
- Use descriptive test names
- Include type hints
- Use pytest fixtures appropriately
- Follow Arrange-Act-Assert pattern

**Example test:**
```python
@pytest.mark.generic
def test_blink(dut: IdfDut) -> None:
    """Test that LED blinks correctly."""
    dut.expect("Turning the LED OFF!")
    dut.expect("Turning the LED ON!")
```

### Imports
- Group imports: standard library, third-party, local
- Use absolute imports
- Import only what's needed

## Project-Specific Notes

### Common Patterns
1. **Main Function**: `app_main()` is the entry point
2. **Component Structure**: Reusable components in `components/` directory
3. **Configuration**: Use `idf.py menuconfig` for project settings
4. **Logging**: Consistent use of ESP_LOG* macros
5. **Error Checking**: ESP_ERROR_CHECK for critical operations

### Wokwi Simulation
- Projects include `wokwi.toml` for simulation
- Firmware path: `build/flasher_args.json`
- ELF path: `build/project_name.elf`

## Development Workflow

1. **Setup**: `idf.py set-target <chip>`
2. **Configure**: `idf.py menuconfig`
3. **Build**: `idf.py build`
4. **Test**: `pytest` (for Python tests)
5. **Flash**: `idf.py -p PORT flash monitor`
6. **Debug**: Use serial monitor or debugger

## Linting and Formatting

**C/C++:**
- Use `clang-format` with ESP-IDF style
- Enable compiler warnings: `-Wall -Wextra`
- Use `cppcheck` for static analysis

**Python:**
- Use `black` for formatting
- Use `flake8` for linting
- Use `mypy` for type checking

## Common Issues and Solutions

1. **Build fails**: Clean build directory with `idf.py fullclean`
2. **Flash fails**: Check port permissions and connections
3. **Tests fail**: Verify pytest dependencies are installed
4. **Memory issues**: Check heap usage with `esp_get_minimum_free_heap_size()`

## References

- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [ESP-IDF API Reference](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/)
- [FreeRTOS Documentation](https://www.freertos.org/)
- [pytest Documentation](https://docs.pytest.org/)

---

*Last updated: $(date)*  
*For agentic coding assistants operating in this repository.*