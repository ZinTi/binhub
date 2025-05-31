#include <iostream>
#include <cstring>

#include "HashUtils.h"

#define PROJECT_NAME            "Digest"
#ifdef _WIN32
    #define PROJECT_VER         "0.1-WinNT"
#else
    #define PROJECT_VER         "0.1-GNU/Linux"
#endif
#define PROJECT_YEARS           "2025"
#define AUTHOR                  "Mr. ZENG Lai(Zin)"
#define AUTHOR_CONTACT          "vip201@126.com"

void version();
void help();

int main(int argc, char* argv[]) {
    if (argc == 1) {
        version();
        help();
        std::cerr << "Too few arguments!" << std::endl;
        return -1;
    }

    if (argc == 2) {
        if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "help") || !strcmp(argv[1], "-h") || !strcmp(argv[1], "-?")) {
            help();
        }else if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "version") || !strcmp(argv[1], "-v")) {
            version();
        }else {
            std::cerr << "Invalid argument!" << std::endl;
            return -1;
        }
        return 0;
    }

    if (!strcmp(argv[1], "--md5") || !strcmp(argv[1], "--MD5")) {
        for (int i = 2; i < argc; i++) {
            std::cout << argv[i] << ": " << HashUtils::calculateMD5(argv[i]) << std::endl;
        }
    }
    else if (!strcmp(argv[1], "--sha1") || !strcmp(argv[1], "--SHA1")) {
        for (int i = 2; i < argc; i++) {
            std::cout << argv[i] << ": " << HashUtils::calculateSHA1(argv[i]) << std::endl;
        }
    }
    else if (!strcmp(argv[1], "--sha256") || !strcmp(argv[1], "--SHA256")) {
        for (int i = 2; i < argc; i++) {
            std::cout << argv[i] << ": " << HashUtils::calculateSHA256(argv[i]) << std::endl;
        }
    }
    else if (!strcmp(argv[1], "--sha384") || !strcmp(argv[1], "--SHA384")) {
        for (int i = 2; i < argc; i++) {
            std::cout << argv[i] << ": " << HashUtils::calculateSHA384(argv[i]) << std::endl;
        }
    }
    else if (!strcmp(argv[1], "--sha512") || !strcmp(argv[1], "--SHA512")) {
        for (int i = 2; i < argc; i++) {
            std::cout << argv[i] << ": " << HashUtils::calculateSHA512(argv[i]) << std::endl;
        }
    }else {
        std::cerr << "Invalid option: " << argv[1] << std::endl;
        return -1;
    }

    return 0;
}

void version(){
    std::cout << PROJECT_NAME << " version " << PROJECT_VER << " (compiled " << __TIME__ << ", " << __DATE__ ")" << std::endl;
    std::cout << "Copyleft (c) " << PROJECT_YEARS << " " <<  AUTHOR << ". All rights reserved. " << AUTHOR_CONTACT << std::endl;
}

void help() {
    std::cout << "[USAGE]" << std::endl;
    std::cout << "\tdigest <OPTION> <string> ...\n" << std::endl;

    std::cout << "[OPTION]" << std::endl;
    std::cout << "\t--help, help, -h, -?: show this help" << std::endl;
    std::cout << "\t--version, version, -v: show version" << std::endl;
    std::cout << "\t--md5/sha1/sha256/sha384/sha512: calculate the hash value\n" << std::endl;
}
