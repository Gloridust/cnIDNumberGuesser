import time
import re
import psutil
import os
import threading

thread_count = 8

id_input = input("请输入身份证号码，用星号 (*) 替代未知数字：")

def is_valid_id_number(id_number):
    if not id_number.isdigit() or len(id_number) != 18:
        return False
    factors = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2]
    id_remainders = ['1', '0', 'x', '9', '8', '7', '6', '5', '4', '3', '2']
    total_sum = sum(int(id_number[i]) * factors[i] for i in range(17))
    remainder = total_sum % 11
    return id_remainders[remainder] == str(id_number[17])

disable_memory = False

def get_memory_usage():
    while True:
        if disable_memory:
            break
        memory = psutil.Process(os.getpid()).memory_info().rss / 1024 / 1024
        print('>>> 正在将数据集加载到内存中，已使用 (MB)：', memory, end='\r')

year_days = {
    '01': [str(x).zfill(2) for x in range(1, 32)],
    '02': [str(x).zfill(2) for x in range(1, 29)],
    '03': [str(x).zfill(2) for x in range(1, 32)],
    '04': [str(x).zfill(2) for x in range(1, 31)],
    '05': [str(x).zfill(2) for x in range(1, 32)],
    '06': [str(x).zfill(2) for x in range(1, 31)],
    '07': [str(x).zfill(2) for x in range(1, 32)],
    '08': [str(x).zfill(2) for x in range(1, 32)],
    '09': [str(x).zfill(2) for x in range(1, 31)],
    '10': [str(x).zfill(2) for x in range(1, 32)],
    '11': [str(x).zfill(2) for x in range(1, 31)],
    '12': [str(x).zfill(2) for x in range(1, 32)]
}

if len(id_input) != 18:
    print(len(id_input))
    print('>>> 请输入18位身份证号码，并用星号 (*) 替代缺失的数字。')
    exit()

count_asterisks = id_input.count('*')
if count_asterisks > 18:
    print('>>> 星号的数量不能超过18个。')
    exit()

if count_asterisks == 0:
    print('>>> 没有缺失的数字。')
    exit()

city_code = id_input[0:6]
birth_year = id_input[6:10]
birth_month = id_input[10:12]
birth_day = id_input[12:14]
tail_digits = id_input[14:18]

print('>>> 城市代码:', city_code)
print('>>> 出生年份:', birth_year)
print('>>> 出生月份:', birth_month)
print('>>> 出生日期:', birth_day)
print('>>> 顺序号:', tail_digits)

cities = None
with open('citycodes.txt', 'r', encoding='utf-8') as f:
    cities = f.read().split('\n')

valid_cities = []
city_pattern = city_code.replace('*', r'(\d)')
for city in cities:
    if re.search(city_pattern, city):
        valid_cities.append(city)

if len(valid_cities) == 0:
    print('>>> 错误：无效的城市代码')
    exit()

print('>>> 有效的城市代码:', valid_cities)

birth_year_options = []
if birth_year == '****':
    birth_year_options = [x for x in range(1949, time.localtime().tm_year + 1)]
else:
    birth_year_pattern = birth_year.replace('*', r'(\d)')
    birth_year_options = [i for i in range(1949, time.localtime().tm_year + 1) if re.match(birth_year_pattern, str(i))]

print('>>> 有效的出生年份:', birth_year_options)

if birth_month == '00':
    print('>>> 错误：无效的月份')
    exit()

birth_month_options = []
if birth_month == '**':
    birth_month_options = [str(x).zfill(2) for x in range(1, 13)]
else:
    if birth_month[0] == '*' and birth_month[1] != '*':
        birth_month_options = [str(x).zfill(2) for x in range(1, 13) if int(birth_month[1]) == x // 10]
    elif birth_month[0] != '*' and birth_month[1] == '*':
        birth_month_options = [str(x).zfill(2) for x in range(10, 13) if int(birth_month[0]) == x // 10]
    else:
        birth_month_options = [birth_month]

print('>>> 有效的出生月份:', birth_month_options)

birth_day_options = {}

for month_option in birth_month_options:
    if birth_day == '**':
        birth_day_options[month_option] = year_days[month_option]
    else:
        if birth_day[0] == '*' and birth_day[1] != '*':
            birth_day_options[month_option] = [str(x).zfill(2) for x in range(1, 32) if int(birth_day[1]) == x // 10]
        elif birth_day[0] != '*' and birth_day[1] == '*':
            if int(birth_day[0]) == 1:
                birth_day_options[month_option] = [str(x).zfill(2) for x in range(10, 20)]
            elif int(birth_day[0]) == 0:
                birth_day_options[month_option] = [str(x).zfill(2) for x in range(1, 10)]
            elif int(birth_day[0]) == 2:
                birth_day_options[month_option] = [str(x).zfill(2) for x in range(20, 32)]
            else:
                print('>>> 错误：无效的日期')
                exit()
        else:
            birth_day_options[month_option] = [birth_day]

print('>>> 有效的出生日期:', birth_day_options)

tail_options = []
if tail_digits == '****':
    tail_options = [str(x).zfill(4) for x in range(10000)]
else:
    if tail_digits.count('*') == 0:
        tail_options = [tail_digits]
    else:
        if tail_digits.count('*') == 1:
            tail_options = [tail_digits.replace('*', str(x)) for x in range(10)]
        elif tail_digits.count('*') == 2:
            tail_options = [tail_digits.replace('*', str(x), 1).replace('*', str(y)) for x in range(10) for y in range(10)]
        elif tail_digits.count('*') == 3:
            tail_options = [tail_digits.replace('*', str(x), 1).replace('*', str(y), 1).replace('*', str(z))
                            for x in range(10) for y in range(10) for z in range(10)]
        else:
            print('>>> 错误：无效的后四位数字')
            exit()

threading.Thread(target=get_memory_usage).start()

all_options = []
for city_option in valid_cities:
    for year_option in birth_year_options:
        for month_option in birth_month_options:
            for day_option in birth_day_options[month_option]:
                for tail_option in tail_options:
                    all_options.append(city_option + str(year_option) + month_option + day_option + tail_option)

disable_memory = True
time.sleep(1)
os.system('cls')
disable_memory = False

all_count = len(all_options)
print('>>> 数据集大小:', all_count)
lock = threading.Lock()
cache = []

def split_and_verify_range(option_range):
    result = []
    for option in option_range:
        real_id = str(option)
        if is_valid_id_number(real_id):
            result.append(real_id)
    return result

range_size = all_count // thread_count
option_ranges = [all_options[i:i+range_size] for i in range(0, len(all_options), range_size)]

start_time = time.time()
threads = []
for option_range in option_ranges:
    t = threading.Thread(target=lambda: cache.extend(split_and_verify_range(option_range)))
    t.start()
    threads.append(t)

for t in threads:
    t.join()

disable_memory = True
time.sleep(1)
os.system('cls')

print('>>> 所有线程已完成')
print('>>> 耗时:', time.time() - start_time)
result = "\n".join(cache)
with open('resultID.txt', 'a') as f:
    f.write(result)
print(result)