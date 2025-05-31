# Base Convert Command

## Overview

The `baseconvert` command is a custom shell script designed for convenient number base conversions. It supports conversions between binary, octal, decimal, and hexadecimal representations.

## Usage

```bash
baseconvert [options] <number>
```

## Options

- `-b` or `--binary`: Convert to binary.
- `-o` or `--octal`: Convert to octal.
- `-d` or `--decimal`: Convert to decimal.
- `-x` or `--hexadecimal`: Convert to hexadecimal.

## Examples

1. Convert binary to decimal:
   ```bash
   baseconvert --binary 101010
   ```

2. Convert decimal to binary:
   ```bash
   baseconvert --decimal 42
   ```

3. Convert hexadecimal to octal:
   ```bash
   baseconvert -x 1A
   ```

## Notes

- Ensure to provide valid input for the specified base.
- The result will be displayed in the requested base.

Feel free to explore different options to perform quick and accurate base conversions!

---