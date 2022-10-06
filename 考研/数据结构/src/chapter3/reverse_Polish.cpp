#include <iostream>
#include <stack>
#include <string>
#include <vector>

bool is_operator(const std::string &str)
{
    if (str == "+" || str == "-" || str == "*" || str == "/" || str == "(" || str == ")")
        return true;

    return false;
}

/**
 * @brief 计算一个中缀表达式
 *
 * 栈中保留的是 ”操作数“。
 */
double calculate_expression(const std::vector<std::string> &postfix_expression)
{
    std::stack<double> s;

    for (auto &str : postfix_expression)
    {
        if (is_operator(str))
        {
            double d1 = s.top();
            s.pop();

            double d2 = s.top();
            s.pop();

            if (str == "+")
            {
                s.push(d1 + d2);
            }

            if (str == "-")
            {
                s.push(d1 - d2);
            }

            if (str == "*")
            {
                s.push(d1 * d2);
            }

            if (str == "/")
            {
                s.push(d1 / d2);
            }
        }
        else
        {
            s.push(std::stod(str));
        }
    }

    return s.top();
}

int get_priority(const std::string &oper)
{
    if (oper == "+")
        return 1;
    if (oper == "*")
        return 2;
    if (oper == "(")
        return 3;

    return 0;
}

bool is_lower_priority(const std::string &left, const std::string &right)
{
    return get_priority(left) < get_priority(right);
}

/**
 * @brief 将中缀表达式，转化为后缀表达式。
 * 为简单起见，操作符仅应为："+","*","(",")"
 *
 * 栈中保留的是 “操作符”。
 * 1.当栈顶元素比当前元素的优先级低，则当前元素入栈
 *   例外："("虽然优先级最高，但他只能由于遇到")"而被弹出
 * 2.若相等，则栈顶元素先出栈。
 * 3.若为)，则将栈弹出至一个"("
 */
std::vector<std::string> infix_to_postfix(const std::vector<std::string> &expr)
{
    std::stack<std::string> s;
    std::vector<std::string> v;

    for (auto &str : expr)
    {
        if (is_operator(str))
        {
            if (str == "+" || str == "*" || str == "(")
            {

                while (!s.empty() && s.top() != "(" && !is_lower_priority(s.top(), str))
                {

                    v.push_back(s.top());

                    s.pop();
                }

                s.push(str);
            }

            if (str == ")")
            {
                while (s.top() != "(")
                {
                    v.push_back(s.top());
                    s.pop();
                }

                s.pop();
            }
        }
        else
        {
            v.push_back(str);
        }
    }

    while (!s.empty())
    {
        v.push_back(s.top());
        s.pop();
    }

    return v;
}

int main()
{
    std::vector<std::string> v_post_fix{"6", "5", "2", "3", "+", "8", "*", "+", "3", "+", "*"};
    std::cout << calculate_expression(v_post_fix) << std::endl;
    std::cout << std::endl;

    // a+b*c+(d*e+f)*g
    std::vector<std::string> v_in_fix{"a", "+", "b", "*", "c", "+", "(", "d", "*", "e", "+", "f", ")", "*", "g"};
    std::vector<std::string> translate = infix_to_postfix(v_in_fix);

    for (auto &str : translate)
    {
        std::cout << str << std::endl;
    }

    return 0;
}