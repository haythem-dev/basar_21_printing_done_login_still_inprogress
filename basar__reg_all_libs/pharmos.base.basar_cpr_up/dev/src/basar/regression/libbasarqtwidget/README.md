# libbasarqtwidget

## Role in Basar Package

**libbasarqtwidget** is the Qt widget library for Basar. It provides enhanced Qt widgets with Basar integration:

- Enhanced combo box with data binding
- Date editor with locale support
- Table widget with property binding
- Time editor with validation
- Custom widget type definitions

This library extends standard Qt widgets to work seamlessly with Basar's property system and data binding framework.

## Dependencies

- Qt5 libraries
- `libbasarguitie` - GUI data binding
- `libbasarguiqt` - Qt GUI framework
- `libbasarproperty` - Property management

## Regression Tests

| Test | Description |
|------|-------------|
| `combo_box_test` | Tests enhanced combo box functionality |
| `date_edit_test` | Validates date editor with locale handling |
| `table_widget_test` | Tests table widget with property binding |
| `time_edit_test` | Validates time editor functionality |
| `widget_types_test` | Tests custom widget type definitions |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.

## Notes

Widget tests may require Qt platform plugins. Non-visual tests can run without a display using `QT_QPA_PLATFORM=offscreen`.
