#include "exceptions.h"

Blaze::BaseException::BaseException(const std::string &filename, const size_t &line, const std::string &class_name,
                                    const std::string &method_name, const std::string &info) {
    _message = "\n Time: " + _time +
               " Error in file " + filename +
               ", line " + std::to_string(line) +
               ", in method " + method_name +
               " of class " + class_name +
               "\n Info: " + info + "\n";

}

const char *Blaze::BaseException::what() const noexcept
{
    return _message.c_str();
}
