# libbasarguitie

## Role in Basar Package

**libbasarguitie** is the GUI data binding library for Basar. It provides the "tie" between data models and GUI widgets:

- Event policy management for widget events
- Exception handling for GUI operations
- Matcher policies for data-to-widget mapping
- Data transformation for display
- Widget type abstractions

This library implements the Model-View pattern, connecting `libbasarproperty` data models to Qt widgets.

## Dependencies

- Qt5 libraries
- `libbasarguiqt` - Qt GUI framework
- `libbasarproperty` - Property management
- `libbasarcmnutil` - Common utilities

## Regression Tests

| Test | Description |
|------|-------------|
| `event_policy_test` | Tests widget event policy handling |
| `exception_handler_test` | Validates GUI exception handling |
| `matcher_policy_test` | Tests data-to-widget matching policies |
| `transformer_test` | Validates data transformation for display |
| `widget_types_test` | Tests widget type abstractions |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.
