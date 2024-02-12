#ifndef ADASH_UTILS_H
#define ADASH_UTILS_H

#include <time.h>
#include <fstream>
#include <iostream>
#include <algorithm> 
//#include <cctype>
#include "utils.hpp"

#define LOGFILE_PATH    "logs/log.txt"

struct v2d {
    int x{0};
    int y{0};
};

namespace Utils {

    // == DATE and TIME =======================================================
    inline std::string getCurrentDateTime(std::string s){
        time_t now = time(0);
        struct tm  timeinfo;
        char  buf[80];
        timeinfo = *localtime(&now);
        if(s=="now")
            strftime(buf, sizeof(buf), "%Y-%m-%d %X", &timeinfo);
        else if(s=="date")
            strftime(buf, sizeof(buf), "%Y-%m-%d", &timeinfo);
        return std::string(buf);
    };

    // == LOG =======================================================
    inline void Log(std::string logMsg){
        std::string now = getCurrentDateTime("now");
        std::ofstream ofs(LOGFILE_PATH, std::ios_base::out | std::ios_base::app );
        ofs << now << '\t' << logMsg << std::endl;
        ofs.close();
    };

    // == TRIM =======================================================
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }
    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }
    inline void trim(std::string &s) {
        rtrim(s);
        ltrim(s);
    }
    inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }
    inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }
    inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }
}

#endif