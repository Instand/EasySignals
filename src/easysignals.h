#ifndef SIGNALS_H
#define SIGNALS_H

#include <functional>
#include <vector>

namespace Room427
{
    //base signal
    template <typename T>
    class Signal
    {
    public:
        Signal(){}

        template<class... Args>
        void operator() (Args... args)
        {
            for (auto& elem : _signals)
                elem(args...);
        }

        Signal<T>& operator = (T&& s)
        {
            _signals.push_back(s);
            return *this;
        }

        Signal<T>& operator = (void* ptr)
        {
            if (ptr == nullptr)
                _signals.clear();

            return *this;
        }

    private:
        std::vector<T> _signals;

    };

    //helper namespace
    namespace Args
    {
        template <typename T>
        struct GetArguments : GetArguments<decltype(&T::operator())> {};

        template <typename T, typename... Args>
        struct GetArguments<T(*)(Args...)> : std::integral_constant<unsigned, sizeof...(Args)> {};

        template <typename T, typename C, typename... Args>
        struct GetArguments<T(C::*)(Args...)> : std::integral_constant<unsigned, sizeof...(Args)> {};

        template <typename T, typename C, typename... Args>
        struct GetArguments<T(C::*)(Args...) const> : std::integral_constant<unsigned, sizeof...(Args)> {};

        //binds
        template<int>
        class CheckArgs {};

        template<>
        class CheckArgs<0>
        {
        public:

            template<typename T, typename Slot>
            auto connect(T& slotObj, Slot&& slot) -> decltype(std::bind(std::forward<Slot>(slot), slotObj))
            {
                return std::bind(std::forward<Slot>(slot), slotObj);
            }
        };

        template<>
        class CheckArgs<1>
        {
        public:

            template<typename T, typename Slot>
            auto connect(T& slotObj, Slot&& slot) -> decltype(std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1))
            {
                return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1);
            }
        };

        template<>
        class CheckArgs<2>
        {
        public:

            template<typename T, typename Slot>
            auto connect(T& slotObj, Slot&& slot) -> decltype(std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2))
            {
                return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2);
            }
        };

        template<>
        class CheckArgs<3>
        {
        public:

            template<typename T, typename Slot>
            auto connect(T& slotObj, Slot&& slot) -> decltype(std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
            {
                return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            }
        };

        template<>
        class CheckArgs<4>
        {
        public:

            template<typename T, typename Slot>
            auto connect(T& slotObj, Slot&& slot) -> decltype(std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                                                        std::placeholders::_4))
            {
                return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                 std::placeholders::_4);
            }
        };
    }

    //base connector
    class Connector
    {
    public:

        template<typename Signal, typename T, typename Slot>
        static void connect(Signal& signal, T& slotObj, Slot&& slot)
        {
            constexpr int _size = Args::GetArguments<Slot>();
            signal = Args::CheckArgs<_size>().connect(slotObj, std::forward<Slot>(slot));
        }

        template<typename Signal>
        static void disconnect(Signal& signal)
        {
            signal = nullptr;
        }

    private:
        Connector() = delete;
        Connector(const Connector&) = delete;
    };

}

#endif // SIGNALS_H