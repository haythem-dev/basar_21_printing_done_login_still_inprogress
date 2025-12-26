# libbasarguiqt

## Role in Basar Package

**libbasarguiqt** is the Qt GUI framework library for Basar. It provides the foundation for Qt-based user interfaces:

- GUI definition structures for declarative UI
- DLL/shared library management for plugins
- GUI initialization and lifecycle
- Manager initialization for GUI components
- Version information handling

This library provides the core Qt integration layer that other GUI libraries (`libbasarguitie`, `libbasarqtwidget`) build upon.

## Dependencies

- Qt5 libraries
- `libbasarcmnutil` - Common utilities
- `libbasarappl` - Application framework

## Regression Tests

| Test | Description |
|------|-------------|
| `definition_struct_test` | Tests GUI definition structure handling |
| `dll_struct_test` | Validates DLL/plugin structure management |
| `init_struct_test` | Tests GUI initialization structures |
| `manager_init_test` | Validates GUI manager initialization |
| `version_info_test` | Tests version information handling |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.

## Notes

These tests focus on non-visual aspects of Qt integration. Visual testing requires a display environment.
