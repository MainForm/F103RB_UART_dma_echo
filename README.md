# STM32 C++ Development Environment Template

STM32CubeMX가 생성하는 코드와 사용자가 작성하는 Application 코드를 분리하여,
C++로 STM32 애플리케이션을 개발하기 위한 템플릿입니다.

예제는 **NUCLEO-F103RB** 보드(STM32F103RBT6)를 기준으로 작성되었습니다.

## 핵심 구조

이 프로젝트에서는 STM32CubeMX가 `main()` 함수를 생성하지 않도록 설정하고,
Application 영역인 `app/Src/main.cpp`에서 `main()`을 직접 정의합니다.

```text
F103RB_CPP_dev/
|-- app/
|   `-- Src/
|       `-- main.cpp          # C++ Application entry point
|-- Core/                     # STM32CubeMX generated code
|-- Drivers/                  # STM32CubeMX generated HAL/CMSIS drivers
|-- cmake/stm32cubemx/        # STM32CubeMX generated CMake configuration
|-- CMakeLists.txt            # Project-level CMake configuration
`-- F103RB_CPP_dev.ioc        # STM32CubeMX project file
```

`F103RB_CPP_dev.ioc`에는 다음 설정이 적용되어 있습니다.

```ini
ProjectManager.NoMain=true
```

따라서 `Core/Src/main.c`에는 `SystemClock_Config()`와 같은 CubeMX 생성 함수가
포함되지만, 프로그램의 진입점인 `main()`은 생성되지 않습니다.

## C++ Application 추가

최상위 `CMakeLists.txt`에서 C++ 언어를 활성화하고 `app/Src/main.cpp`를 빌드
대상에 추가합니다.

```cmake
enable_language(C CXX ASM)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    app/Src/main.cpp
)
```

이를 통해 CubeMX가 생성한 C/HAL 코드는 그대로 사용하면서 Application 코드는
C++로 작성할 수 있습니다.

CubeMX가 C로 생성한 함수를 C++ 코드에서 호출할 때는 C++ name mangling을
비활성화하기 위해 `extern "C"`를 사용합니다.

```cpp
// Defined in Core/Src/main.c.
// extern "C" disables C++ name mangling so the C++ linker can find the C function.
extern "C"
void SystemClock_Config(void);
```

## 장점

- CubeMX 생성 코드와 Application 코드의 역할 및 위치가 명확하게 분리됩니다.
- Application 코드에서 클래스, 네임스페이스 등 C++ 기능을 사용할 수 있습니다.
- CubeMX에서 코드를 다시 생성해도 `app/` 아래의 Application 코드는 덮어쓰지
  않으므로 독립적으로 유지할 수 있습니다.
- 주변장치 설정은 CubeMX로 관리하면서 애플리케이션 로직은 별도로 확장할 수
  있습니다.

> CubeMX에서 코드를 재생성할 때는 **Do not generate the `main()`** 설정을
> 유지해야 합니다. 또한 Application 소스 등록은 최상위 `CMakeLists.txt`에서
> 관리합니다.

## Build

GNU Arm Embedded Toolchain과 Ninja가 준비된 환경에서 다음 CMake preset으로
빌드할 수 있습니다.

```sh
cmake --preset Debug
cmake --build --preset Debug
```
