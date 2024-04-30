#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <chrono>
#include <algorithm>
#include <thread>
#include <chrono>

std::string id_input;

bool is_valid_id_number(const std::string &id_number)
{
    if (!std::all_of(id_number.begin(), id_number.end(), ::isdigit) || id_number.length() != 18)
        return false;

    int factors[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    char id_remainders[] = "10x98765432";
    int total_sum = 0;
    for (int i = 0; i < 17; ++i)
    {
        total_sum += (id_number[i] - '0') * factors[i];
    }
    int remainder = total_sum % 11;
    return id_remainders[remainder] == id_number[17];
}

void get_memory_usage()
{
    while (true)
    {
        std::ifstream proc_file("/proc/self/status");
        std::string line;
        while (std::getline(proc_file, line))
        {
            if (line.find("VmRSS") != std::string::npos)
            {
                std::istringstream iss(line);
                std::string key, value, unit;
                iss >> key >> value >> unit;
                std::cout << ">>> 正在将数据集加载到内存中，已使用 (MB)：" << value << std::endl;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main()
{
    std::cout << "请输入身份证号码，用星号 (*) 替代未知数字：";
    std::cin >> id_input;

    if (id_input.length() != 18 || id_input.find_first_not_of("0123456789*") != std::string::npos)
    {
        std::cout << "请输入18位身份证号码，并用星号 (*) 替代缺失的数字。" << std::endl;
        return 1;
    }

    int count_asterisks = std::count(id_input.begin(), id_input.end(), '*');
    if (count_asterisks > 18)
    {
        std::cout << ">>> 星号的数量不能超过18个。" << std::endl;
        return 1;
    }

    if (count_asterisks == 0)
    {
        std::cout << ">>> 没有缺失的数字。" << std::endl;
        return 1;
    }

    std::string city_code = id_input.substr(0, 6);
    std::string birth_year = id_input.substr(6, 4);
    std::string birth_month = id_input.substr(10, 2);
    std::string birth_day = id_input.substr(12, 2);
    std::string tail_digits = id_input.substr(14, 4);

    std::cout << ">>> 城市代码:" << city_code << std::endl;
    std::cout << ">>> 出生年份:" << birth_year << std::endl;
    std::cout << ">>> 出生月份:" << birth_month << std::endl;
    std::cout << ">>> 出生日期:" << birth_day << std::endl;
    std::cout << ">>> 顺序号:" << tail_digits << std::endl;

    std::ifstream city_file("citycodes.txt");
    std::vector<std::string> cities;
    std::string city;
    while (std::getline(city_file, city))
    {
        cities.push_back(city);
    }

    std::vector<std::string> valid_cities;
    std::string city_pattern = city_code;
    std::replace(city_pattern.begin(), city_pattern.end(), '*', '.');
    for (const auto &city : cities)
    {
        if (std::regex_search(city, std::regex(city_pattern)))
        {
            valid_cities.push_back(city);
        }
    }

    if (valid_cities.empty())
    {
        std::cout << ">>> 错误：无效的城市代码" << std::endl;
        return 1;
    }

    std::cout << ">>> 有效的城市代码:";
    for (const auto &valid_city : valid_cities)
    {
        std::cout << " " << valid_city;
    }
    std::cout << std::endl;

    std::vector<int> birth_year_options;
    if (birth_year == "****")
    {
        int current_year = std::stoi(birth_year);
        for (int year = 1949; year <= current_year; ++year)
        {
            birth_year_options.push_back(year);
        }
    }
    else
    {
        std::string birth_year_pattern = birth_year;
        std::replace(birth_year_pattern.begin(), birth_year_pattern.end(), '*', '.');
        for (int year = 1949; year <= std::stoi(birth_year); ++year)
        {
            if (std::regex_match(std::to_string(year), std::regex(birth_year_pattern)))
            {
                birth_year_options.push_back(year);
            }
        }
    }

    std::cout << ">>> 有效的出生年份:";
    for (const auto &year : birth_year_options)
    {
        std::cout << " " << year;
    }
    std::cout << std::endl;

    // Similarly handle birth_month, birth_day, and tail_digits options...
    std::vector<std::string> birth_month_options;
    if (birth_month == "**")
    {
        for (int month = 1; month <= 12; ++month)
        {
            birth_month_options.push_back(std::to_string(month));
        }
    }
    else
    {
        std::string birth_month_pattern = birth_month;
        std::replace(birth_month_pattern.begin(), birth_month_pattern.end(), '*', '.');
        for (int month = 1; month <= 12; ++month)
        {
            if (std::regex_match(std::to_string(month), std::regex(birth_month_pattern)))
            {
                birth_month_options.push_back(std::to_string(month));
            }
        }
    }

    std::cout << ">>> 有效的出生月份:";
    for (const auto &month : birth_month_options)
    {
        std::cout << " " << month;
    }
    std::cout << std::endl;

    std::map<std::string, std::vector<std::string>> year_days = {
        {"01", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"}},
        {"02", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28"}},
        {"03", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"}},
        {"04", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"}},
        {"05", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"}},
        {"06", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"}},
        {"07", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"}},
        {"08", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"}},
        {"09", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"}},
        {"10", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"}},
        {"11", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"}},
        {"12", {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"}}};

    std::map<std::string, std::vector<std::string>> birth_day_options;
    for (const auto &month_option : birth_month_options)
    {
        if (birth_day == "**")
        {
            birth_day_options[month_option] = year_days[month_option];
        }
        else
        {
            std::string birth_day_pattern = birth_day;
            std::replace(birth_day_pattern.begin(), birth_day_pattern.end(), '*', '.');
            for (const auto &day : year_days[month_option])
            {
                if (std::regex_match(day, std::regex(birth_day_pattern)))
                {
                    birth_day_options[month_option].push_back(day);
                }
            }
        }
    }

    std::cout << ">>> 有效的出生日期:";
    for (const auto &entry : birth_day_options)
    {
        std::cout << " 月份 " << entry.first << ":";
        for (const auto &day : entry.second)
        {
            std::cout << " " << day;
        }
    }
    std::cout << std::endl;

    std::vector<std::string> tail_options;
    if (tail_digits == "****")
    {
        for (int tail = 0; tail < 10000; ++tail)
        {
            tail_options.push_back(std::to_string(tail));
        }
    }
    else
    {
        // Handle tail_digits options...
        std::vector<std::string> tail_options;
        if (tail_digits == "****")
        {
            for (int tail = 0; tail < 10000; ++tail)
            {
                tail_options.push_back(std::to_string(tail));
            }
        }
        else
        {
            int count_asterisks = std::count(tail_digits.begin(), tail_digits.end(), '*');
            if (count_asterisks == 0)
            {
                tail_options.push_back(tail_digits);
            }
            else
            {
                std::string tail_pattern = tail_digits;
                std::replace(tail_pattern.begin(), tail_pattern.end(), '*', '.');
                for (int tail = 0; tail < 10000; ++tail)
                {
                    if (std::regex_match(std::to_string(tail), std::regex(tail_pattern)))
                    {
                        tail_options.push_back(std::to_string(tail));
                    }
                }
            }
        }

        std::cout << ">>> 有效的后四位数字:";
        for (const auto &tail : tail_options)
        {
            std::cout << " " << tail;
        }
        std::cout << std::endl;
    }

    return 0;
}
