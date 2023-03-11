// #pragma once
// #include <concepts>

// template <typename T>
// concept vector_type = requires
// {
//     typename T::value_type;
//     typename T::size_type;
//     typename T::allocator_type;
//     {
//         std::declval<T>().size()
//     }
//     ->std::convertible_to<typename T::size_type>;
//     {
//         std::declval<T>().begin()
//     }
//     ->std::same_as<typename T::iterator>;
//     {
//         std::declval<T>().end()
//     }
//     ->std::same_as<typename T::iterator>;
// };

// template <typename T>
// concept isVector = requires(T t)
// {
//     []<typename Tv>(std::vector<Tv>) {}(t);
// };

// template <typename T>
// void printz(const T &t)
// {
//     std::cout << t << std::endl;
// }
// template <typename T>
// void printz(const T &t) requires isVector<T>
// {
//     for (const auto &v : t)
//     {
//         std::cout << v << std::endl;
//     }
//     // std::cout << std::endl;
// }
