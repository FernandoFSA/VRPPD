#pragma once

#include <iostream>
#include <utility>
#include <forward_list>
#include <list>
#include <vector>
#include <map>

namespace output_operators
{
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::list<T>& list)
    {
        os << "[";
        for (const auto& element : list)
        {
            os << element << ", ";
        }
        if (!list.empty())
        {
            // Remove the trailing comma
            os << "\b\b";
        }
        os << "]";
        return os;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
    {
        os << "[";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            os << vec[i];
            if (i != vec.size() - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& vec)
    {
        os << "[";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            os << vec[i];
            if (i != vec.size() - 1)
            {
                os << ",\n ";
            }
        }
        os << "]";
        return os;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<std::vector<T>>>& vec)
    {
        os << "[\n";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            os << vec[i];
            if (i != vec.size() - 1)
            {
                os << ",\n\n";
            }
        }
        os << "\n]";
        return os;
    }

    template <typename T, std::size_t N>
    std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr)
    {
        os << "[";
        for (size_t i = 0; i < N; ++i)
        {
            os << arr[i];
            if (i != N - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    template <typename T, std::size_t N, std::size_t M>
    std::ostream& operator<<(std::ostream& os, const std::array<std::array<T, N>, M>& arr)
    {
        os << "[";
        for (size_t i = 0; i < M; ++i)
        {
            os << arr[i];
            if (i != M - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    template <typename T, std::size_t N, std::size_t M, std::size_t L>
    std::ostream& operator<<(std::ostream& os, const std::array<std::array<std::array<T, N>, M>, L>& arr)
    {
        os << "[";
        for (size_t i = 0; i < L; ++i)
        {
            os << arr[i];
            if (i != L - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    template <typename K, typename V>
    std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m)
    {
        os << "{";
        for (auto it = m.cbegin(); it != m.cend(); ++it)
        {
            os << it->first << ": " << it->second;
            if (std::next(it) != m.cend())
            {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

    template <typename K, typename V>
    std::ostream& operator<<(std::ostream& os, const std::multimap<K, V>& mm)
    {
        os << "{";
        for (auto it = mm.cbegin(); it != mm.cend(); ++it)
        {
            os << it->first << ": " << it->second;
            if (std::next(it) != mm.cend())
            {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::forward_list<T>& fl)
    {
        os << "[";
        for (auto it = fl.cbegin(); it != fl.cend(); ++it)
        {
            os << *it;
            if (std::next(it) != fl.cend())
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    template <std::size_t Index = 0, typename... Types>
    typename std::enable_if<Index == sizeof...(Types), void>::type
        print_tuple(std::ostream& os, const std::tuple<Types...>& t)
    {
        // Base case: Do nothing when we've reached the end of the tuple
    }

    template <std::size_t Index = 0, typename... Types>
    typename std::enable_if < Index < sizeof...(Types), void>::type
        print_tuple(std::ostream& os, const std::tuple<Types...>& t)
    {
        // Print the current element
        if (Index != 0) os << ", ";
        os << std::get<Index>(t);
        // Recursively print the rest of the tuple
        print_tuple<Index + 1>(os, t);
    }

    template <typename... Types>
    std::ostream& operator<<(std::ostream& os, const std::tuple<Types...>& t)
    {
        os << "(";
        print_tuple(os, t);
        os << ")";
        return os;
    }

    template <typename T1, typename T2>
    std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p)
    {
        os << "(" << p.first << ", " << p.second << ")";
        return os;
    }

    template<typename T1, typename T2>
    std::ostream& operator<<(std::ostream& os, const std::vector<std::pair<T1, T2>>& vec) {
        os << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            os << vec[i];
            if (i != vec.size() - 1) {
                os << ", ";  // Print a comma between elements
            }
        }
        os << "]";
        return os;
    }

    template <typename T1, typename T2>
    std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<std::pair<T1, T2>>>& vec)
    {
        os << "[";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            os << vec[i];
            if (i != vec.size() - 1)
            {
                os << ",\n ";
            }
        }
        os << "]";
        return os;
    }

    template <typename T1, typename T2>
    std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<std::vector<std::pair<T1, T2>>>>& vec)
    {
        os << "[\n";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            os << vec[i];
            if (i != vec.size() - 1)
            {
                os << ",\n\n";
            }
        }
        os << "\n]";
        return os;
    }
}

namespace io = output_operators;
// add to your code: using io::operator<<;