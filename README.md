# CRAXH ZIP Password Brute-Force Tool

![Build Status](https://img.shields.io/badge/build-experimental-orange)

## Description

**CRAXH** is a command-line utility written in C++ that performs brute-force attacks on password-protected ZIP archives using a user-provided wordlist. It leverages the system’s `unzip` command to test each password candidate and automatically extracts the archive once the correct password is found.

## Features

- Brute-force password testing for `.zip` archives via a wordlist  
- Automatic extraction of ZIP contents upon successful password detection  
- Graceful interrupt handling (CTRL+C)  
- Input validation for ZIP file and wordlist format and existence  
- Configurable output extraction directory (optional)  
- Shell-safe command construction to prevent injection  

## System Requirements

- Linux or Unix-like operating system  
- C++17-compatible compiler (e.g., `g++`, `clang++`)  
- `unzip` utility installed and available in your `PATH`  
- Terminal or shell access  

## Installation

Compile the source code (assuming the file is named `craxh.cpp`):

```bash
apt-get update && apt-get upgrade -y
apt-get install unzip git -y
git clone https://github.com/raxh918/craxh
cd craxh
g++ -std=c++17 -o craxh craxh.cpp
```

## Usage

```bash
./craxh -i <file.zip> -w <wordlist.txt> [-p <output_path>]
```

## **Arguments**

| Flag | Description | Required |
| ----- | ----- | ----- |
| `-i`, `--input` | Path to the password-protected ZIP file | ✓ |
| `-w`, `--wordlist` | Path to the wordlist text file (one password per line) | ✓ |
| `-p`, `--path` | Directory where extracted files will be saved (optional) | ✗ |

**If `-p` is omitted**, CRAXH will create an output folder named after the ZIP file (without the `.zip` extension) in the same directory as the archive.

## **Wordlist Recommendation**

For best results, use comprehensive, real-world password lists. One recommended source:

* [weakpass.com](https://weakpass.com) — extensive archives of leaked password collections

Download a suitable list (e.g., based on target language or complexity) and supply it via the `-w` option.

## **Notes & Limitations**

* Only supports standard `.zip` archives

* Depends on the external `unzip` tool; ensure it is installed and up to date

* Does not support other archive formats (RAR, 7z, etc.) or advanced ZIP encryption modes

* Brute-force speed depends on wordlist size, CPU performance, and disk I/O

* Designed primarily for Linux/Unix; Windows users should run under WSL, Cygwin, or similar

## **License**

This project is licensed under the MIT License. See [LICENSE](https://github.com/raxh918/craxh/LICENSE) for full details.

## **Author & Contact**

**RAXH**  
 Website: [https://raxh.dev](https://raxh.dev)