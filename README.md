## 🎯 Project Overview

This project implements the **lexical analysis phase** of a compiler for the **S++ programming language** - a custom language designed for educational purposes to teach compiler construction concepts.

### What This Compiler Does

| Component | Status | Description |
|-----------|--------|-------------|
| **Lexical Analyzer** | ✅ Complete | Tokenizes source code into meaningful tokens |
| **Pattern Matching** | ✅ Complete | Recognizes identifiers, constants, operators, keywords |
| **Comment Handling** | ✅ Complete | Supports `$$` single-line and `$* *$` multi-line comments |
| **Error Detection** | ✅ Complete | Identifies illegal characters and unterminated strings |
| **Line Tracking** | ✅ Complete | Maintains line numbers for error reporting |

### S++ Language Highlights

- **Unique Identifier Rule:** All identifiers must start with `@` (e.g., `@myVariable`)
- **Keyword Convention:** All keywords start with 's' (e.g., `sint`, `sif`, `sfor`)
- **Statement Terminator:** Uses full stop `.` instead of semicolon `;`
- **Case Sensitive:** Uppercase and lowercase letters are distinct
