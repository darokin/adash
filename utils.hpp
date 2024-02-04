#ifndef ADASH_UTILS_H
#define ADASH_UTILS_H

#include <time.h>
#include <fstream>
#include <iostream>
#include "utils.hpp"

#define LOGFILE_PATH    "logs/log.txt"

struct v2d {
    int x{0};
    int y{0};
};

namespace Utils {
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

    inline void Log(std::string logMsg){
        std::string now = getCurrentDateTime("now");
        std::ofstream ofs(LOGFILE_PATH, std::ios_base::out | std::ios_base::app );
        ofs << now << '\t' << logMsg << std::endl;
        ofs.close();
    };
}

#endif