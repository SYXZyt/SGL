#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include <type_traits>

namespace sgl
{
    template <typename F>
    concept FunctionPointer = std::is_pointer_v<F> &&
        std::is_function_v<std::remove_pointer_t<F>>;

    template <typename... Args>
    class Event final
    {
    public:
        using Handler = std::function<void(Args...)>;
        using Handle = size_t;

    private:
        struct Entry final
        {
            size_t id;
            Handler handler;
            void* function; // or method
            void* instance;
        };

        std::vector<Entry> mHandlers;
        Handle mNextId = 0ull;

        template <typename Pred>
        void RemoveIf(Pred pred)
        {
            auto it = std::remove_if(mHandlers.begin(), mHandlers.end(), pred);
            mHandlers.erase(it, mHandlers.end());
        }

    public:
        Handle AddHandler(const Handler& handler)
        {
            Handle id = mNextId++;
            mHandlers.push_back({ id, handler, nullptr, nullptr });
            return id;
        }

        template <FunctionPointer F>
        void AddHandler(F func)
        {
            Handle id = mNextId++;
            mHandlers.push_back({ id, func, (void*)func, nullptr });
        }

        template <typename T>
        void AddHandler(T* instance, void(T::* method)(Args...))
        {
            Handle id = mNextId++;
            mHandlers.push_back({
                id,
                [instance, method](Args... args)
                {
                    (instance->*method)(args...);
                },
                *(void**)(&method),
                (void*) instance
                });
        }

        void RemoveHandler(Handle handle)
        {
            RemoveIf([&](const Entry& e)
                {
                    return e.id == handle;
                });
        }

        template <FunctionPointer F>
        void RemoveHandler(F func)
        {
            RemoveIf([&](const Entry& e)
                {
                    return e.function == (void*)func;
                });
        }

        template <typename T>
        void RemoveHandler(T* instance, void(T::* method)(Args...))
        {
            void* methodId = *(void**)&method;

            RemoveIf([&](const Entry& e)
                {
                    return e.instance == (void*)instance &&
                        e.function == methodId;
                });
        }

        Handle operator+=(const Handler& handler) {
            return AddHandler(handler);
        }

        template <FunctionPointer F>
            requires std::is_pointer_v<F>&& std::is_function_v<std::remove_pointer_t<F>>
        void operator+=(F func) {
            AddHandler(func);
        }

        template <typename T>
        void operator+=(const std::pair<T*, void(T::*)(Args...)>& args) {
            AddHandler(args.first, args.second);
        }

        void operator-=(Handle handle) {
            RemoveHandler(handle);
        }

        template <FunctionPointer F>
        void operator-=(F func) {
            RemoveHandler(func);
        }

        template <typename T>
        void operator-=(const std::pair<T*, void(T::*)(Args...)>& args) {
            RemoveHandler(args.first, args.second);
        }

        void Invoke(Args... args) const
        {
            for (const auto& e : mHandlers)
                e.handler(args...);
        }
    };
}