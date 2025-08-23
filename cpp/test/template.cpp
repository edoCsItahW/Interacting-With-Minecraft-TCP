// Copyright (c) 2024. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file template.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2024/10/25 上午8:54
 * @brief 优质模板代码
 * @copyright CC BY-NC-SA 2024. All rights reserved.
 * */

#include <functional>
#include <iostream>
#include <type_traits>  // 类型特征库
#include <utility>      // 通用工具库

// --------------------------------- 编译器求成员个数 --------------------------------

/**
 * @details @c AnyType 是一个空的结构体，里面定义了一个模板成员函数。
 * 这个成员函数使用了类型转换操作符 operator T()，允许 AnyType 的实例转换为任意类型 T。
 * 这种设计通常用于实现类型安全的类型擦除，或是提供灵活的类型转换。
 * */
struct AnyType {
    template<typename T>
    operator T();
};

/**
 * @details @c CountMember 是一个模板结构体，用于计算某个类型 T 可以接受的构造函数参数的数量。
 * @tparam Args 是可变模板参数，sizeof...(Args) 计算此参数包的大小。这里，value 被定义为参数数量减一。
 * @remark 第二个模板参数 typename = void 是为了使得特化时可以忽略参数。
 * */
template<typename T, typename = void, typename... Args>
struct CountMember {
    constexpr static std::size_t value = sizeof...(Args) - 1;
};

/**
 * @details @c CountMember 的特化版本，只有在 T 的构造函数可以接受 Args... 参数时才有效。
 * 如果可以调用，则递归调用 CountMember，此时省略一个参数，从而实现对可以接受的参数数量的递归计数。
 * @note 使用 std::void_t 和 decltype 检查构造函数是否能被调用。
 * ## 示例
 * @code
 * struct A { int x; int y; int z; };
 * CountMember<A>::value // 3
 * @endcode
 * @see 编译器正则表达式 https://github.com/hanickadot/compile-time-regular-expressions
 * @see 元编程库 https://github.com/ericniebler/meta
 * */
template<typename T, typename... Args>
struct CountMember<T, std::void_t<decltype(T{std::declval<Args>()...})>, Args...> {
    constexpr static std::size_t value = CountMember<T, void, Args...>::value;
};

/**
 * @brief C++20实现
 * @code
 * struct AnyType {
 *     template <typename T>
 *     operator T();
 * };
 *
 * template <typename T, typename... Args>
 * constexpr auto count_member() {
 *     if constexpr (requires { T{std::declval<Args>()...}; }) return count_member<T, Args..., AnyType>();
 *     return sizeof...(Args) - 1;
 * }
 *
 * struct A{int x, y, z;};
 * constexpr auto x = count_member<A>(); // x = 3
 * @endcode
 * */

// --------------------------------- 简洁地遍历tuple -------------------------------

/**
 * @details 通用的 for_each 模板函数，该函数可以接受一个元组和一个函数，通过展开索引序列遍历元组内的每个元素，并将给定的函数应用于每个元素。
 * @tparam T 元组类型
 * @tparam F 函数类型
 * @param tuple 通用引用（可以是左值或右值）的元组 tuple
 * @param f 通用引用的函数 f
 * @return 函数
 * @remark std::apply实现
 * @code
 * std::apply([&f](auto&&... elems) { (f(std::forward<decltype(elems)>(elems)), ...); }, std::forward<T>(tuple));
 * @endcode
 * */
template<class T, class F>
constexpr decltype(auto) for_each(T&& tuple, F&& f) {
    // 定义了一个带有模板参数包的 lambda 表达式。std::size_t... I 用于接受一系列索引，用于访问元组中的元素。
    return []<std::size_t... I>(T&& tuple, F&& f, std::index_sequence<I...>) {  // 元组 tuple、函数 f 和一个 std::index_sequence，后者用于生成一组索引。
        (f(std::get<I>(tuple)), ...);  // 这是一个折叠表达式，使用逗号运算符。std::get<I>(tuple) 用于获取元组中索引为 I 的元素，然后对其应用函数 f，完成对每个元素的处理。
        return f;  // 最后，lambda 返回函数 f，这实际上是为了满足返回类型的需求。
    }
    // 使用 std::forward 转发元组和函数，保持它们的值类别。
    (std::forward<T>(tuple), std::forward<F>(f),
     // std::make_index_sequence<std::tuple_size<std::remove_reference_t<tuple_type>>::value> 用于生成从 0 到元组大小的索引序列，从而使 lambda 能够遍历元组。
     std::make_index_sequence<std::tuple_size<std::remove_reference_t<T>>::value>{});
}

// --------------------------------- 对参数包依次执行任何操作 ---------------------

template<typename... Args>
void print1(const Args&... args) {
    (... && [](std::ostream& os, const auto& val) -> bool {
        os << val << " ";
        return true;
    }(std::cout, args));
    std::cout << std::endl;
}

template<typename... Args>
void print2(const Args&... args) {
    (..., [](std::ostream& os, const auto& val) -> void { os << val << " "; }(std::cout, args));
    std::cout << std::endl;
}

/**
 * @details 对于&&，还可以通过让lambda返回false让其断在某个地方，比如输出遇到的第一个整数就停止
 * @see 折叠表达式技巧 https://www.foonathan.net/2020/05/fold-tricks/#content
 * @remark 其它折叠表达式技巧
 * @code
 * // 总和
 * auto sum(auto&& head, auto&&... tail) { return head + ... + tail; }
 *
 * // 应用函数于每一个元素
 * void applyFor(auto&& f, auto&&... args) { (..., f(args)); }
 *
 * // 反向应用函数于每一个元素
 * void applyForReverse(auto&& f, auto&&... args) {
 *     int dummy;
 *     (dummy = ... = (f(args), 0));  // UB?
 * }
 *
 * // 应用函数于每一个元素直到某个条件满足
 * void applyForUntil(auto&& f, auto&& pred, auto&&... args) { (... && (pred(args) ? false : (f(args), true))); }
 *
 * // 检查是否有任何元素与谓词匹配
 * bool anyOf(auto&& pred, auto&&... args) { return ... || pred(args); }
 *
 * // 检查所有元素是否与谓词匹配
 * bool allOf(auto&& pred, auto&&... args) { return ... && pred(args); }
 *
 * // 检查是否没有元素与谓词匹配
 * bool noneOf(auto&& pred, auto&&... args) { return !(... || pred(args)); }
 *
 * // 计算有多少元素与谓词匹配
 * std::size_t countIf(auto&& pred, auto&&... args) { return std::size_t(0) + ... + (pred(args) ? 1 : 0); }
 *
 * // 找到与元素匹配的第一个元素
 * auto findFirst(auto&& pred, auto&&... args) {
 *     std::common_type_t<std::decay_t<decltype(args)>...> result{};
 *     (... || (pred(args) ? (result = args, true) : false));
 *     return result;
 * }
 *
 * // 获取第n个元素
 * auto nthElement(std::size_t n, auto&&... args) {
 *     std::common_type_t<std::decay_t<decltype(args)>...> result{};
 *     std::size_t i = 0;
 *     (... || (i++ == n ? (result = args, true) : false));
 *     return result;
 * }
 *
 * // 获取参数包的第一个元素
 * auto firstElement(auto&&... args) {
 *     std::common_type_t<std::decay_t<decltype(args)>...> result{};
 *     (... || (result = args, true));
 *     return result;
 * }
 *
 * // 获取参数包的最后一个元素
 * auto lastElement(auto&&... args) { return ..., args; }
 *
 * // 获取最小元素
 * auto minElement(auto&&... args) {
 *     auto min = (args, ...);  // last element
 *     return ..., (args < min ? (min = args, min) : min);
 * }
 * @endcode
 * */
template<typename... Args>
void print4(const Args&... args) {
    (... && [](std::ostream& os, const auto& val) -> bool {
        os << val << " ";
        return !std::is_same_v<std::decay_t<decltype(val)>, int>;
    }(std::cout, args));
    std::cout << std::endl;
}

// --------------------------------- 编译器排序 ---------------------------------

/**
 * @details @c List 是一个模板结构体，用于存储一组整数。它采用可变参数模板int... xs，允许存储任意数量的整数。
 * */
template<int... xs>
struct List {};

/**
 * @details @c _Head 是一个部分特化结构，用于提取列表的第一个元素（头元素）。Head是一个常量表达式，提供了获取头元素的方式。
 * */
template<typename T>
struct _Head;

template<int x, int... xs>
struct _Head<List<x, xs...>> {
    static const int value = x;
};

template<typename T>
constexpr int Head = _Head<T>::value;

/**
 * @brief 计算长度Length
 * @details @c _Length 是一个结构，计算List的长度。长度的递归计算用于对List中元素进行计数，基于递归和特殊化实现。
 * */
template<typename T>
struct _Length;
template<typename T>
constexpr int Length = _Length<T>::value;

template<>
struct _Length<List<>> {
    static const int value = 0;
};

template<int x, int... xs>
struct _Length<List<x, xs...>> {
    static const int value = 1 + Length<List<xs...>>;
};

/**
 * @brief 推送元素Push
 * @details @c _Push 用于将新元素x推入List中。通过特化实现将新元素放在列表的最前面。
 * */
template<int x, typename T>
struct _Push;

template<int x, int y, int... xs>
struct _Push<x, List<y, xs...>> {
    using result = List<x, y, xs...>;
};

template<int x>
struct _Push<x, List<>> {
    using result = List<x>;
};

template<int x, typename T>
using Push = typename _Push<x, T>::result;

/**
 * @brief 冒泡排序实现Bubble
 * @details @c _Bubble 是一个结构，它实现了冒泡排序的逻辑。通过比较相邻元素，将较小的元素推到前面。std::conditional_t用于根据条件选择结果。
 * */
template<typename T>
struct _Bubble;
template<typename T>
using Bubble = typename _Bubble<T>::result;

template<int x>
struct _Bubble<List<x>> {
    using result = List<x>;
};

template<int x, int y, int... xs>
struct _Bubble<List<x, y, xs...>> {
    using result = std::conditional_t<(x > y), Push<y, Bubble<Push<x, List<xs...>>>>, Push<x, Bubble<Push<y, List<xs...>>>>>;
};

/**
 * @brief 多次执行冒泡排序RepeatBubble
 * @details @c RepeatBubble 用于多次执行Bubble排序操作，精确实现冒泡排序所需的迭代次数。
 * */
template<size_t n, typename T>
struct _RepeatBubble;
template<size_t n, typename T>
using RepeatBubble = typename _RepeatBubble<n, T>::result;

template<typename T>
struct _RepeatBubble<0, T> {
    using result = T;
};

template<size_t n, typename T>
struct _RepeatBubble {
    using result = Bubble<RepeatBubble<n - 1, T>>;
};

/**
 * @brief 排序入口BubbleSort
 * @details @c BubbleSort 是用户可以调用的接口，它计算List的长度并触发冒泡排序。
 * ## 示例
 * @code
 * int main() {
 *     static_assert(std::is_same_v<BubbleSort<List<23, 4, 56, 7, 88, 45, 9, 36, 83, 10>>, List<4, 7, 9, 10, 23, 36, 45, 56, 83, 88>>);
 *     getchar();
 *     return 0;
 * }
 * @endcode
 * */
template<typename T>
using BubbleSort = RepeatBubble<Length<T> - 1, T>;

// --------------------------------- 元函数 -------------------------------

/**
 * @brief 求绝对值
 * ## 示例
 * @code
 * int main() {
 *     int const n = -10;
 *     cout << ABS<n>::value << "\n";  // output: 10 return 0;
 * }
 * @endcode
 * */
template<int N>
struct ABS {
    static_assert(N != INT_MIN);
    static constexpr auto value = (N > 0) ? N : -N;
};

/**
 * @brief 求数组维度
 * ## 示例
 * @code
 * int main() {
 *     using array_t = int[10][20][30];
 *     cout << Rank<array_t>::value << "\n"; // output: 3 return 0;
 * }
 * @endcode
 * */
template<class T>
struct Rank {
    static constexpr size_t value = 0u;
};

template<class U, size_t N>
struct Rank<U[N]> {
    static constexpr size_t value = 1u + Rank<U>::value;
};

/**
 * @brief 判断某一类型序列中所有元素是否满足某种条件
 * ## 示例
 * @code
 * int main() {
 *     std::cout << All(std::vector<int>{1, 2, 3, 4}, is_even) << "\n";  // output: 0
 *     return 0;
 * }
 * @endcode
 * */
template<class Seq, class Fn>
bool All(const Seq& seq, Fn fn) {
    // static_assert();
    for (const auto& x : seq)
        if (!fn(x)) return false;
    return true;
}

bool is_even(int num) { return num & 1; }

// --------------------------------- 千级指针 -------------------------------

/**
 * @details 变量模板，输入T，返回T*，相当于给输入类型加了个*。
 * 把它当成一个函数来看，AddPointer<T>用尖括号调用，传入一个类型，而等号后面的 T*就是返回值。
 *
 * ## 示例
 * @code
 * int main() {
 *     SuperPointer<int, 1000>::type p;
 *     return 0;
 * }
 * @endcode
 *
 * @see 模板元俄罗斯方块 https://github.com/mattbierner/Super-Template-Tetris
 * */
template<typename T>
using AddPointer = T*;

/**
 * @details 定义一个struct模板，继承自SuperPointer<AddPointer<T>, Size - 1>。
 * 继承自己？ 这就是模板递归的关键，这一步相当于函数递归中函数调用自身。
 * */
template<typename T, size_t Size>
struct SuperPointer : SuperPointer<AddPointer<T>, Size - 1> {};

/**
 * @details 在类名后面用尖括号指定特殊值，告诉编译器遇到这种值后按以下流程特殊处理，而不是按原来的流程操作，这叫做模板特化。
 * 将所有模板参数一个不剩的定下特殊值，叫做全特化。
 * */
template<typename T>
struct SuperPointer<T, 0> {
    using type = T;
};

// ------------------------------- std::bind技巧 --------------------------

/**
 * @brief 绑定、lambda
 *
 * ## 示例
 * @code
 * int main() {
 *     auto op1 = std::bind(pureFunc, "xiaoming", 18);
 *     op1();
 *
 *     std::string name = "wanger";
 *     int age          = 50;
 *     auto op2         = std::bind([&name, &age]() { std::cout << "name: " << name << ", age: " << age << std::endl; });
 *
 *     op2();
 *
 *     return 0;
 * }
 * @endcode
 */

void pureFunc(std::string name, int age) { std::cout << "name :" << name << ", age:" << age << std::endl; }


/**
 * @brief 绑定成员函数的N种写法
 *
 * ## 示例
 * @code
 * int main() {
 *     TestRun tr;
 *
 *     auto op1 = std::bind(&TestRun::runFunc, &tr, "xiaoming", 18);
 *     op1();
 *
 *     auto op2 = std::bind(&TestRun::runFunc, tr, "xiaoming", 18);  // 会拷贝一份tr
 *     op2();
 *
 *     auto op3 = std::bind(std::ref(tr), "xiaoming", 88.88);
 *     op3();
 *
 *     auto op4 = std::bind(tr, "xiaoming", 88.88);  // 会拷贝一份tr
 *     op4();
 *
 *     return 0;
 * }
 * @endcode
 * */

class TestRun {
public:
    void runFunc(const std::string& name, int age) { std::cout << "ThreadRun::threadFunc. name:" << name << ", age:" << age << std::endl; }

    void operator()(const std::string& name, double score) { std::cout << "ThreadRun::operator(). name:" << name << ", score:" << score << std::endl; }
};



/**
 * @brief 占位符
 *
 * ## 示例
 * @code
 * int main() {
 *     std::string name = "xiaoming";
 *     std::string addr = "guangdong";
 *
 *     auto op1 = std::bind(pureFunc, name, std::placeholders::_1, std::placeholders::_2);
 *
 *     op1(addr, 66.6);
 *
 *     auto op2 = std::bind(pureFunc, std::placeholders::_1, addr, std::placeholders::_2);
 *     op2(name, 67);
 *
 *     return 0;
 * }
 * @endcode
 * */

void pureFunc(std::string name, std::string addr, double score) { std::cout << "name :" << name << ", addr:" << addr << ", score:" << score << std::endl; }


// ------------------------------- 打印变参模板参数包所有参数 -----------------------


template<typename First, typename... Args>
void test(std::ostream& os, const First &firstarg, const Args&... args) {
    // (os << ... << [&]<typename T>(T &args)->T& {os << ','; return args; }(args)) << ")";
    ((os << "(" << firstarg), ..., (os << ", " << args)) << ")";
}
