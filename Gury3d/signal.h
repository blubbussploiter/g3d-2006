
#ifndef SIGNAL_H
#define SIGNAL_H

#include <vector>
#include <functional>

namespace Reflection
{
    template <typename... T>
    struct signal {
        typedef std::function<T...> function_type;
        typedef std::vector<function_type> container_type;

        container_type _slots;

        template <typename... Args>
        void operator()(Args... args) {
            for (function_type f : _slots) {
                f(args...);
            }
        }
        void connect(function_type slot) {
            _slots.push_back(slot);
        }
    };
}

#endif