#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <csignal>

namespace fs = std::filesystem;

bool stop = false;

void handle_sigint(int) {
    std::cout << "\n🛑 Stopped by user (CTRL+C)\n";
    stop = true;
}

void printBanner() {
    std::cout << R"(

 ██████╗██████╗  █████╗ ██╗  ██╗██╗  ██╗
██╔════╝██╔══██╗██╔══██╗╚██╗██╔╝██║  ██║
██║     ██████╔╝███████║ ╚███╔╝ ███████║
██║     ██╔══██╗██╔══██║ ██╔██╗ ██╔══██║
╚██████╗██║  ██║██║  ██║██╔╝ ██╗██║  ██║
 ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
                                      
CRAXH Password ZIP Brute-force Tool by RAXH
)" << "\n\n";
}

void printUsage() {
    std::cout << "Usage: ./craxh -i <file.zip> -w <wordlist.txt> [-p <output_path>]\n";
}

bool isZipFile(const std::string& filename) {
    return filename.size() > 4 && filename.substr(filename.size() - 4) == ".zip";
}

bool isTxtFile(const std::string& filename) {
    return filename.size() > 4 && filename.substr(filename.size() - 4) == ".txt";
}

bool fileExistsAndRegular(const std::string& filename) {
    fs::path p(filename);
    return fs::exists(p) && fs::is_regular_file(p) && fs::file_size(p) > 0;
}

std::string escapeShellArg(const std::string& input) {
    std::string escaped = "'";
    for (char c : input) {
        if (c == '\'')
            escaped += "'\\''";
        else
            escaped += c;
    }
    escaped += "'";
    return escaped;
}

bool tryPassword(const std::string& zipFile, const std::string& password) {
    std::string cmd = "unzip -P " + escapeShellArg(password) + " -t " + escapeShellArg(zipFile) + " > /dev/null 2>&1";
    return system(cmd.c_str()) == 0;
}

bool extractZip(const std::string& zipFile, const std::string& password, const std::string& outputDir) {
    std::string cmd = "unzip -P " + escapeShellArg(password) + " " + escapeShellArg(zipFile) + " -d " + escapeShellArg(outputDir) + " > /dev/null 2>&1";
    return system(cmd.c_str()) == 0;
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, handle_sigint);

    printBanner();

    std::string zipFile, wordlist, outputDir;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            zipFile = argv[++i];
        } else if ((arg == "-w" || arg == "--wordlist") && i + 1 < argc) {
            wordlist = argv[++i];
        } else if ((arg == "-p" || arg == "--path") && i + 1 < argc) {
            outputDir = argv[++i];
        }
    }

    if (zipFile.empty() || wordlist.empty()) {
        printUsage();
        return 1;
    }

    if (!isZipFile(zipFile)) {
        std::cerr << "❌ Input file must be a .zip file\n";
        return 1;
    }

    if (!isTxtFile(wordlist)) {
        std::cerr << "❌ Wordlist must be a .txt file\n";
        return 1;
    }

    if (!fileExistsAndRegular(zipFile)) {
        std::cerr << "❌ ZIP file does not exist, is not a regular file, or is empty: " << zipFile << "\n";
        return 1;
    }

    if (!fileExistsAndRegular(wordlist)) {
        std::cerr << "❌ Wordlist does not exist, is not a regular file, or is empty: " << wordlist << "\n";
        return 1;
    }

    if (outputDir.empty()) {
        fs::path zipPath(zipFile);
        outputDir = (zipPath.parent_path() / zipPath.stem()).string();
    }

    std::cout << "🔍 Attempting to crack ZIP password: " << zipFile << "\n";
    std::cout << "📂 Output folder: " << outputDir << "\n";

    std::ifstream infile(wordlist);
    std::string password;
    int attempts = 0;

    while (std::getline(infile, password)) {
        if (stop) break;
        attempts++;
        std::cout << "🔎 Trying: " << password << "\r" << std::flush;
        if (tryPassword(zipFile, password)) {
            std::cout << "\n✅ Password found: " << password << "\n";

            if (!fs::exists(outputDir)) {
                try {
                    fs::create_directories(outputDir);
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "❌ Failed to create output directory: " << e.what() << "\n";
                    return 1;
                }
            }

            if (extractZip(zipFile, password, outputDir)) {
                std::cout << "📁 Files extracted to: " << outputDir << "\n";
            } else {
                std::cerr << "⚠️ Failed to extract files.\n";
            }
            return 0;
        }
    }

    if (!stop)
        std::cout << "\n❌ Password not found in wordlist.\n";

    return 0;
}
