#include "FileUtils.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

#if defined(__GNUC__)
    #include <dirent.h>
    #include <stdlib.h>
    #include <sys/stat.h>
    #include <unistd.h>
#elif defined(_MSC_VER)
    #ifdef _WIN32
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN
        #endif //! WIN32_LEAN_AND_MEAN
        #include <Windows.h>
        #include <Winsock2.h>
        #pragma comment(lib, "Ws2_32.lib")
        #undef WIN32_LEAN_AND_MEAN
    #endif //!_WIN32
    #include <io.h>
#endif //!__GNUC__

const char *const_basename(const char *filepath)
{
    const char *base = strrchr(filepath, '/');
#if defined(_MSC_VER)
    if (!base) {
        base = strrchr(filepath, '\\');
    }
#endif
    return base ? (base + 1) : filepath;
}

bool file_is_exists(const char *filepath)
{
#if defined(__GNUC__)
    return access(filepath, F_OK) == 0;
#elif defined(_MSC_VER)
#endif
}

bool path_is_exists(const char *directory)
{
#if defined(__GNUC__)
    DIR *dir = opendir(directory);
    if (dir != nullptr) {
        closedir(dir);
        return true;
    }
#elif defined(_MSC_VER)
#endif
    return false;
}

void create_directory(const char *directory)
{
#if defined(__GNUC__)
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        mkdir(directory, S_IRWXU);
    } else {
        closedir(dir);
    }
#elif defined(_MSC_VER)
    if (_access(directory, 0) == -1) {
        CreateDirectoryA(directory, NULL);
    }
#endif
}

bool remove_directory(const char *directory)
{
#if defined(__GNUC__)
    if (path_is_exists(directory)) {
        if (rmdir(directory) != 0) {
            return false;
        }
    }
#elif defined(_MSC_VER)
    return false;
#endif
    return true;
}

bool remove_file(const char *filepath)
{
#if defined(__GNUC__)
    if (file_is_exists(filepath)) {
        if (remove(filepath) != 0) {
            return false;
        }
    }
#elif defined(_MSC_VER)
    return false;
#endif
    return true;
}

void create_directory_recurse(const std::string &directory)
{
    unsigned long pos = directory.rfind('/');
#if defined(_MSC_VER)
    if (pos == std::string::npos) {
        pos = directory.rfind('\\');
    }
#endif
    if (pos != std::string::npos && pos != 0) {
        create_directory_recurse(directory.substr(0, pos));
    }
    create_directory(directory.c_str());
}

bool remove_directory_recurse(const char *directory)
{
#if defined(__GNUC__)
    DIR *dir = opendir(directory);
    if (dir == nullptr)
        return false;
    dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        // 跳过当前目录和父目录
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }

        std::string filepath = std::string(directory) + "/" + entry->d_name;
        if (entry->d_type == DT_DIR) {
            if (!remove_directory_recurse(filepath.c_str())) {
                closedir(dir);
                return false;
            }
        } else {
            if (remove(filepath.c_str()) != 0) {
                closedir(dir);
                return false;
            }
        }
    }

    closedir(dir);
    if (rmdir(directory) != 0) {
        return false;
    }
    return true;
#elif defined(_MSC_VER)
    char tmp_filepath[2048] = {0};
    sprintf_s(tmp_filepath, "%s\\*.*", directory);
    WIN32_FIND_DATAA fileData;
    HANDLE           hFile = FindFirstFileA(tmp_filepath, &fileData);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        if (strcmp(".", fileData.cFileName) == 0 || strcmp("..", fileData.cFileName) == 0) {
            continue;
        } else if (fileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
            char filepath[2048] = {0};
            sprintf_s(filepath, "%s\\%s", directory, fileData.cFileName);
            if (!remove_directory_recurse(directory)) {
                return false;
            }
        } else {
            char filepath[2048] = {0};
            sprintf_s(filepath, "%s\\%s", directory, fileData.cFileName);
            if (remove(filepath) != 0) {
                return false;
            }
        }
    } while (FindNextFileA(hFile, &fileData));

    if (!RemoveDirectoryA(directory)) {
        return false;
    }
    return true;
#endif
}