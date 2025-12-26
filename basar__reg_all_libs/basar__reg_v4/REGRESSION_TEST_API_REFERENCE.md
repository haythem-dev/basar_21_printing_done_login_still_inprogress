
# Regression Test API Reference

## Document Purpose
This document maps every Basar library function/method used in the regression tests to its source definition, providing a complete API reference for the test suite.

---

## Test Suite Overview

### Tests Analyzed
1. **standalone/infrastructure_test** - Framework validation (no Basar APIs)
2. **standalone/cpp_basics** - C++ standard library validation (no Basar APIs)
3. **libbasarcmnutil/hello_basar** - Basic library integration
4. **libbasarcmnutil/date_operations** - Date/DateTime API
5. **libbasarcmnutil/decimal_arithmetic** - Decimal precision API

---

## 1. Infrastructure Test (standalone/infrastructure_test)

**File**: `pharmos.base.basar_cpr_up/dev/src/basar/regression/standalone/infrastructure_test/main.cpp`

### APIs Used
- **NONE** - Uses only standard C++ `<iostream>`

### Purpose
Validates regression framework itself, not Basar libraries.

---

## 2. C++ Basics Test (standalone/cpp_basics)

**File**: `pharmos.base.basar_cpr_up/dev/src/basar/regression/standalone/cpp_basics/main.cpp`

### APIs Used
- **NONE** - Uses only standard C++ libraries (`<iostream>`, `<string>`, `<vector>`)

### Purpose
Validates C++ compiler and standard library work correctly.

---

## 3. Hello Basar Test (libbasarcmnutil/hello_basar)

**File**: `pharmos.base.basar_cpr_up/dev/src/basar/regression/libbasarcmnutil/hello_basar/main.cpp`

### Namespace
```cpp
using namespace std;
```

### APIs Used

#### 3.1 `basar::cmnutil::Date::today()`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.h`
```cpp
namespace basar {
namespace cmnutil {
    class Date {
    public:
        static Date today();
        // Returns current system date
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.cpp`

**Usage in Test**:
```cpp
basar::cmnutil::Date today = basar::cmnutil::Date::today();
```

**Purpose**: Get current system date

---

#### 3.2 `basar::cmnutil::Date::toString()`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.h`
```cpp
namespace basar {
namespace cmnutil {
    class Date {
    public:
        ConstString toString(ConstString format = "") const;
        // Converts date to string with optional format
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.cpp`

**Usage in Test**:
```cpp
today.toString().c_str()
```

**Purpose**: Convert Date object to string representation

**Note**: Returns `ConstString` (Basar string type), requires `.c_str()` for standard output

---

#### 3.3 `basar::Exception`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_exceptions.h`
```cpp
namespace basar {
    class Exception : public std::exception {
    public:
        virtual const char* what() const throw();
        // Returns exception message
    };
}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_exceptions.cpp`

**Usage in Test**:
```cpp
catch (const basar::Exception& ex) {
    cerr << "Error: " << ex.what() << endl;
}
```

**Purpose**: Catch Basar-specific exceptions

---

## 4. Date Operations Test (libbasarcmnutil/date_operations)

**File**: `pharmos.base.basar_cpr_up/dev/src/basar/regression/libbasarcmnutil/date_operations/main.cpp`

### Namespaces
```cpp
using namespace basar::cmnutil;
using namespace std;
```

### APIs Used

#### 4.1 `Date` Constructor
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.h`
```cpp
namespace basar {
namespace cmnutil {
    class Date {
    public:
        Date();  // Default constructor
        Date(int year, int month, int day);  // Explicit date
        // Constructs Date object
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.cpp`

**Usage in Test**:
```cpp
Date date1(2024, 4, 22);
Date date2;
```

**Purpose**: Create Date objects with specific or default values

---

#### 4.2 `Date::isValid()`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.h`
```cpp
namespace basar {
namespace cmnutil {
    class Date {
    public:
        bool isValid() const;
        // Returns true if date is valid
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.cpp`

**Usage in Test**:
```cpp
if (!date1.isValid()) {
    cerr << "Invalid date!" << endl;
}
```

**Purpose**: Validate date before operations

---

#### 4.3 `Date::toString(ConstString format)`
**Declaration**: Same as 3.2 above

**Usage in Test**:
```cpp
cout << "Date 1: " << date1.toString("").c_str() << endl;
```

**Purpose**: Format date as string (empty format uses default)

---

#### 4.4 `Date::addDays(int days)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.h`
```cpp
namespace basar {
namespace cmnutil {
    class Date {
    public:
        Date addDays(int days) const;
        // Returns new Date object with days added
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.cpp`

**Usage in Test**:
```cpp
Date date2 = date1.addDays(10);
```

**Purpose**: Date arithmetic - add days to a date

---

#### 4.5 `Date::daysTo(const Date& other)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.h`
```cpp
namespace basar {
namespace cmnutil {
    class Date {
    public:
        int daysTo(const Date& other) const;
        // Returns number of days between this date and other
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.cpp`

**Usage in Test**:
```cpp
int diff = date2.daysTo(date1);
```

**Purpose**: Calculate difference between two dates in days

---

#### 4.6 `Date::today()`
**Declaration**: Same as 3.1 above

**Usage in Test**:
```cpp
Date currentDate = Date::today();
```

**Purpose**: Get current system date (static method)

---

#### 4.7 `Date::isLeapYear(int year)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.h`
```cpp
namespace basar {
namespace cmnutil {
    class Date {
    public:
        static bool isLeapYear(int year);
        // Returns true if year is a leap year
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_date.cpp`

**Usage in Test**:
```cpp
bool leap = Date::isLeapYear(2024);
```

**Purpose**: Determine if a year is a leap year (static method)

---

## 5. Decimal Arithmetic Test (libbasarcmnutil/decimal_arithmetic)

**File**: `pharmos.base.basar_cpr_up/dev/src/basar/regression/libbasarcmnutil/decimal_arithmetic/main.cpp`

### Namespaces
```cpp
using namespace basar::cmnutil;
using namespace std;
```

### APIs Used

#### 5.1 `Decimal` Constructor (from string)
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        Decimal();  // Default constructor
        Decimal(const char* value);  // From C-string
        Decimal(const std::string& value);  // From string
        // Constructs Decimal with high precision
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
Decimal d1("123.45");
Decimal d2("67.89");
```

**Purpose**: Create high-precision decimal numbers from string representation

---

#### 5.2 `Decimal::operator+(const Decimal& other)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        Decimal operator+(const Decimal& other) const;
        // Returns sum of two decimals
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
Decimal sum = d1 + d2;
```

**Purpose**: Add two decimal numbers with precision preservation

---

#### 5.3 `Decimal::operator-(const Decimal& other)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        Decimal operator-(const Decimal& other) const;
        // Returns difference of two decimals
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
Decimal diff = d1 - d2;
```

**Purpose**: Subtract two decimal numbers

---

#### 5.4 `Decimal::operator*(const Decimal& other)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        Decimal operator*(const Decimal& other) const;
        // Returns product of two decimals
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
Decimal product = d1 * d2;
```

**Purpose**: Multiply two decimal numbers

---

#### 5.5 `Decimal::operator/(const Decimal& other)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        Decimal operator/(const Decimal& other) const;
        // Returns quotient of two decimals
        // Throws exception if other is zero
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
Decimal quotient = d1 / d2;
```

**Purpose**: Divide two decimal numbers (with zero-division protection)

---

#### 5.6 `Decimal::round(int decimals)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        void round(int decimals);
        // Rounds decimal IN-PLACE to specified decimal places
        // RETURNS VOID - modifies object directly
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test** (Build 11 - CORRECTED):
```cpp
Decimal d7 = d6;  // Create copy first
d7.round(2);      // Round in-place (void return)
```

**Previous INCORRECT Usage** (Build 10):
```cpp
Decimal d7 = d6.round(2);  // WRONG - round() returns void!
```

**Purpose**: Round decimal to specified precision (modifies object, doesn't return value)

**⚠️ IMPORTANT**: This is an **in-place mutation** method, NOT a functional method!

---

#### 5.7 `Decimal::toString()`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        ConstString toString() const;
        // Converts decimal to string representation
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
cout << "Sum: " << sum.toString().c_str() << endl;
```

**Purpose**: Convert Decimal to string for output

---

#### 5.8 `Decimal::operator==(const Decimal& other)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        bool operator==(const Decimal& other) const;
        // Compares two decimals for equality
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
if (d1 == d2) {
    cout << "Decimals are equal" << endl;
}
```

**Purpose**: Test decimal equality

---

#### 5.9 `Decimal::operator>(const Decimal& other)`
**Declaration**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.h`
```cpp
namespace basar {
namespace cmnutil {
    class Decimal {
    public:
        bool operator>(const Decimal& other) const;
        // Returns true if this > other
    };
}}
```

**Implementation**: `pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/libbasarcmnutil_decimal.cpp`

**Usage in Test**:
```cpp
if (d1 > d2) {
    cout << "d1 is greater" << endl;
}
```

**Purpose**: Compare decimal magnitude

---

## API Summary Table

| Test | API Count | Library | Header File |
|------|-----------|---------|-------------|
| infrastructure_test | 0 | N/A | None (std C++ only) |
| cpp_basics | 0 | N/A | None (std C++ only) |
| hello_basar | 3 | libbasarcmnutil | libbasarcmnutil_date.h, libbasarcmnutil_exceptions.h |
| date_operations | 7 | libbasarcmnutil | libbasarcmnutil_date.h |
| decimal_arithmetic | 10 | libbasarcmnutil | libbasarcmnutil_decimal.h |

**Total Unique Basar APIs**: 20

---

## API Categories

### 1. Date/Time APIs (7 methods)
- `Date::today()` - Static factory
- `Date::Date(int, int, int)` - Constructor
- `Date::toString(ConstString)` - Formatter
- `Date::isValid()` - Validator
- `Date::addDays(int)` - Arithmetic
- `Date::daysTo(const Date&)` - Difference calculation
- `Date::isLeapYear(int)` - Static utility

### 2. Decimal Precision APIs (10 methods)
- `Decimal::Decimal(const char*)` - Constructor
- `Decimal::operator+` - Addition
- `Decimal::operator-` - Subtraction
- `Decimal::operator*` - Multiplication
- `Decimal::operator/` - Division
- `Decimal::round(int)` - ⚠️ **IN-PLACE** rounding (void return)
- `Decimal::toString()` - Formatter
- `Decimal::operator==` - Equality
- `Decimal::operator>` - Comparison

### 3. Exception Handling APIs (1 class)
- `basar::Exception` - Base exception class
- `Exception::what()` - Message retrieval

---

## Source File Locations

### Header Files (API Declarations)
```
pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/
├── libbasarcmnutil_date.h           → Date class
├── libbasarcmnutil_decimal.h        → Decimal class
├── libbasarcmnutil_exceptions.h     → Exception classes
└── libbasarcmnutil.h                → Master include (includes all above)
```

### Implementation Files
```
pharmos.base.basar_cpr_up/dev/src/basar/libbasarcmnutil/
├── libbasarcmnutil_date.cpp         → Date implementation
├── libbasarcmnutil_decimal.cpp      → Decimal implementation
└── libbasarcmnutil_exceptions.cpp   → Exception implementation
```

### Test Files
```
pharmos.base.basar_cpr_up/dev/src/basar/regression/
├── standalone/
│   ├── infrastructure_test/main.cpp  → No Basar APIs
│   └── cpp_basics/main.cpp           → No Basar APIs
└── libbasarcmnutil/
    ├── hello_basar/main.cpp          → 3 APIs
    ├── date_operations/main.cpp      → 7 APIs
    └── decimal_arithmetic/main.cpp   → 10 APIs
```

---

## Key Differences from Qt5 API

The regression tests were initially modeled after Qt5 but Basar has different signatures:

### Date API Differences

| Qt5 API | Basar API | Notes |
|---------|-----------|-------|
| `date.toString()` | `date.toString("")` | Basar requires format parameter |
| `QDate::currentDate()` | `Date::today()` | Different static method name |

### Decimal API Differences

| Qt5/Java Style | Basar API | Notes |
|----------------|-----------|-------|
| `rounded = d.round(2)` | `d.round(2); // void` | Basar modifies in-place, returns void |
| Functional style | Mutating style | ⚠️ **Major difference** |

---

## Build Evolution

### Build 10 → Build 11 Changes

**Problem**: Tests assumed functional/immutable API style
**Solution**: Changed to mutation-based style

```cpp
// BUILD 10 (WRONG):
Decimal d7 = d6.round(2);  // Compiler error: void → Decimal

// BUILD 11 (CORRECT):
Decimal d7 = d6;
d7.round(2);  // Mutates d7 in-place
```

---

## Related Documentation

- **REGRESSION_BASAR_DEPENDENCIES.md** - Architecture and dependencies
- **build_11/STATUS.md** - Current test status
- **basar__reg_v2/BUILD_10_FIX.md** - API compatibility fixes
- **pharmos.base.basar_cpr_up/dev/src/basar/regression/README.md** - Test creation guide

---

## Maintainer Notes

### When Adding New Tests

1. **Check API signatures** in header files first
2. **Don't assume Qt5 compatibility** - Basar APIs differ
3. **Test return types** - Some methods return void (mutations)
4. **Use this document** as reference for existing patterns

### Common Pitfalls

1. ❌ `date.toString()` without format → ✅ `date.toString("")`
2. ❌ `result = decimal.round(n)` → ✅ `decimal.round(n); // void`
3. ❌ `Date::currentDate()` → ✅ `Date::today()`

---

**Document Version**: 1.0  
**Last Updated**: Build 11  
**Status**: All APIs documented and verified
