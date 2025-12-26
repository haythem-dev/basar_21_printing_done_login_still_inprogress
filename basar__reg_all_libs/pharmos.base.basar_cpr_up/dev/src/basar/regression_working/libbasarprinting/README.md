# libbasarprinting

## Role in Basar Package

**libbasarprinting** is the printing and document generation library for Basar. It provides output generation capabilities:

- PDF document conversion
- Print job management
- PostScript/Ghostscript interpretation
- Raw printer output handling
- File extension detection for document types

This library handles all document output operations, from screen display to physical printing.

## Dependencies

- Ghostscript libraries
- `libbasarcmnutil` - Common utilities

## Regression Tests

| Test | Description |
|------|-------------|
| `file_extension_test` | Tests document type detection by extension |
| `interpreter_test` | Validates PostScript/Ghostscript interpretation |
| `pdf_conversion_test` | Tests PDF generation and conversion |
| `print_utils_test` | Validates print utility functions |
| `raw_print_test` | Tests raw printer output handling |

## Running Tests

```bash
cd <test_directory>
./go.sh
```

Output is logged to `go.out` in each test directory.
