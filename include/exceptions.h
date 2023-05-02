#ifndef TASK_5_EXCEPTIONS_H
#define TASK_5_EXCEPTIONS_H

#include <exception>
#include <string>
#include <ctime>

namespace Blaze {

    class BaseException: public std::exception
    {
    public:
        BaseException(const std::string &filename,
                      const size_t &line, const std::string &class_name,
                      const std::string &method_name, const std::string &info);
        ~BaseException() override = default;

        [[nodiscard]] const char *what() const noexcept override;

    protected:
        std::string _message;
        std::string _time;
    };

    class WildPointerException : public BaseException
    {
    public:
        WildPointerException(const std::string &filename,
                             const size_t &line, const std::string &class_name,
                             const std::string &method_name,
                             const std::string &info = "Wild pointer.")
                : BaseException(filename, line, class_name, method_name, info) {};
    };

    class AllocateMemoryException: public BaseException
    {
    public:
        AllocateMemoryException(const std::string &filename,
                                const size_t &line, const std::string &class_name,
                                const std::string &method_name, const std::string &info = "Allocate memory error")
                : BaseException(filename, line, class_name, method_name, info) {};
    };
}

#endif //TASK_5_EXCEPTIONS_H
