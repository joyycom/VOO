#ifndef LuaUtil_H
#define LuaUtil_H

#include <string>


class LuaUtil {
public:
    static bool encrypt(const std::string &src, const std::string &dst);
    static bool decrypt(const std::string &src, const std::string &dst);


};

#endif // LuaUtil_H
