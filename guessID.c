#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <regex.h>

#define MAX_ID_LENGTH 19
#define MAX_CITIES 4000
#define MAX_OPTIONS 1000000

// 全局变量
char id_input[MAX_ID_LENGTH];
char cities[MAX_CITIES][7];
int city_count = 0;
char valid_cities[MAX_CITIES][7];
int valid_city_count = 0;
int birth_year_options[3000];
int birth_year_count = 0;
char birth_month_options[13][3];
int birth_month_count = 0;
char birth_day_options[13][32][3];
int birth_day_count[13];
char tail_options[10000][5];
int tail_count = 0;
char all_options[MAX_OPTIONS][19];
int all_count = 0;
char cache[MAX_OPTIONS][19];
int cache_count = 0;

// 函数声明
int is_valid_id_number(const char* id_number);
void load_cities();
void generate_valid_cities();
void generate_birth_year_options();
void generate_birth_month_options();
void generate_birth_day_options();
void generate_tail_options();
void generate_all_options();
void verify_all_options();

int main() {
    printf("请输入身份证号码，用星号 (*) 替代未知数字：");
    scanf("%18s", id_input);

    if (strlen(id_input) != 18) {
        printf(">>> 请输入18位身份证号码，并用星号 (*) 替代缺失的数字。\n");
        return 1;
    }

    int count_asterisks = 0;
    for (int i = 0; i < 18; i++) {
        if (id_input[i] == '*') count_asterisks++;
    }

    if (count_asterisks > 18) {
        printf(">>> 星号的数量不能超过18个。\n");
        return 1;
    }

    if (count_asterisks == 0) {
        printf(">>> 没有缺失的数字。\n");
        return 1;
    }

    printf(">>> 城市代码: %.6s\n", id_input);
    printf(">>> 出生年份: %.4s\n", id_input + 6);
    printf(">>> 出生月份: %.2s\n", id_input + 10);
    printf(">>> 出生日期: %.2s\n", id_input + 12);
    printf(">>> 顺序号: %.4s\n", id_input + 14);

    load_cities();
    generate_valid_cities();
    generate_birth_year_options();
    generate_birth_month_options();
    generate_birth_day_options();
    generate_tail_options();
    generate_all_options();

    clock_t start_time = clock();

    verify_all_options();

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf(">>> 验证完成\n");
    printf(">>> 耗时: %f 秒\n", time_spent);

    FILE* f = fopen("resultID.txt", "a");
    for (int i = 0; i < cache_count; i++) {
        fprintf(f, "%s\n", cache[i]);
        printf("%s\n", cache[i]);
    }
    fclose(f);

    return 0;
}

int is_valid_id_number(const char* id_number) {
    if (strlen(id_number) != 18) return 0;
    for (int i = 0; i < 17; i++) {
        if (!isdigit(id_number[i])) return 0;
    }
    
    int factors[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    char id_remainders[] = {'1', '0', 'x', '9', '8', '7', '6', '5', '4', '3', '2'};
    int total_sum = 0;
    
    for (int i = 0; i < 17; i++) {
        total_sum += (id_number[i] - '0') * factors[i];
    }
    
    int remainder = total_sum % 11;
    return id_remainders[remainder] == id_number[17];
}

void load_cities() {
    FILE* f = fopen("citycodes.txt", "r");
    if (f == NULL) {
        printf("无法打开citycodes.txt文件\n");
        exit(1);
    }

    while (fscanf(f, "%6s", cities[city_count]) != EOF && city_count < MAX_CITIES) {
        city_count++;
    }

    fclose(f);
}

void generate_valid_cities() {
    regex_t regex;
    char city_pattern[7];
    strcpy(city_pattern, id_input);
    for (int i = 0; i < 6; i++) {
        if (city_pattern[i] == '*') city_pattern[i] = '.';
    }
    
    if (regcomp(&regex, city_pattern, REG_EXTENDED) != 0) {
        printf("正则表达式编译失败\n");
        exit(1);
    }

    for (int i = 0; i < city_count; i++) {
        if (regexec(&regex, cities[i], 0, NULL, 0) == 0) {
            strcpy(valid_cities[valid_city_count++], cities[i]);
        }
    }

    regfree(&regex);

    if (valid_city_count == 0) {
        printf(">>> 错误：无效的城市代码\n");
        exit(1);
    }

    printf(">>> 有效的城市代码: ");
    for (int i = 0; i < valid_city_count; i++) {
        printf("%s ", valid_cities[i]);
    }
    printf("\n");
}

void generate_birth_year_options() {
    char birth_year[5];
    strncpy(birth_year, id_input + 6, 4);
    birth_year[4] = '\0';

    int current_year = 2024; // 假设当前年份为2024

    if (strcmp(birth_year, "****") == 0) {
        for (int i = 1949; i <= current_year; i++) {
            birth_year_options[birth_year_count++] = i;
        }
    } else {
        regex_t regex;
        char year_pattern[5];
        strcpy(year_pattern, birth_year);
        for (int i = 0; i < 4; i++) {
            if (year_pattern[i] == '*') year_pattern[i] = '.';
        }
        
        if (regcomp(&regex, year_pattern, REG_EXTENDED) != 0) {
            printf("正则表达式编译失败\n");
            exit(1);
        }

        char year_str[5];
        for (int i = 1949; i <= current_year; i++) {
            sprintf(year_str, "%04d", i);
            if (regexec(&regex, year_str, 0, NULL, 0) == 0) {
                birth_year_options[birth_year_count++] = i;
            }
        }

        regfree(&regex);
    }

    printf(">>> 有效的出生年份: ");
    for (int i = 0; i < birth_year_count; i++) {
        printf("%d ", birth_year_options[i]);
    }
    printf("\n");
}

void generate_birth_month_options() {
    char birth_month[3];
    strncpy(birth_month, id_input + 10, 2);
    birth_month[2] = '\0';

    if (strcmp(birth_month, "00") == 0) {
        printf(">>> 错误：无效的月份\n");
        exit(1);
    }

    if (strcmp(birth_month, "**") == 0) {
        for (int i = 1; i <= 12; i++) {
            sprintf(birth_month_options[birth_month_count++], "%02d", i);
        }
    } else if (birth_month[0] == '*' && birth_month[1] != '*') {
        for (int i = 1; i <= 12; i++) {
            if (i % 10 == birth_month[1] - '0') {
                sprintf(birth_month_options[birth_month_count++], "%02d", i);
            }
        }
    } else if (birth_month[0] != '*' && birth_month[1] == '*') {
        for (int i = 10; i <= 12; i++) {
            if (i / 10 == birth_month[0] - '0') {
                sprintf(birth_month_options[birth_month_count++], "%02d", i);
            }
        }
    } else {
        strcpy(birth_month_options[birth_month_count++], birth_month);
    }

    printf(">>> 有效的出生月份: ");
    for (int i = 0; i < birth_month_count; i++) {
        printf("%s ", birth_month_options[i]);
    }
    printf("\n");
}

void generate_birth_day_options() {
    char birth_day[3];
    strncpy(birth_day, id_input + 12, 2);
    birth_day[2] = '\0';

    int year_days[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    for (int m = 0; m < birth_month_count; m++) {
        int month = atoi(birth_month_options[m]);
        
        if (strcmp(birth_day, "**") == 0) {
            for (int d = 1; d <= year_days[month]; d++) {
                sprintf(birth_day_options[month][birth_day_count[month]++], "%02d", d);
            }
        } else if (birth_day[0] == '*' && birth_day[1] != '*') {
            for (int d = 1; d <= year_days[month]; d++) {
                if (d % 10 == birth_day[1] - '0') {
                    sprintf(birth_day_options[month][birth_day_count[month]++], "%02d", d);
                }
            }
        } else if (birth_day[0] != '*' && birth_day[1] == '*') {
            int start = (birth_day[0] - '0') * 10;
            int end = start + 9;
            if (end > year_days[month]) end = year_days[month];
            for (int d = start; d <= end; d++) {
                sprintf(birth_day_options[month][birth_day_count[month]++], "%02d", d);
            }
        } else {
            strcpy(birth_day_options[month][birth_day_count[month]++], birth_day);
        }
    }

    printf(">>> 有效的出生日期: ");
    for (int m = 1; m <= 12; m++) {
        if (birth_day_count[m] > 0) {
            printf("%02d: ", m);
            for (int d = 0; d < birth_day_count[m]; d++) {
                printf("%s ", birth_day_options[m][d]);
            }
            printf(", ");
        }
    }
    printf("\n");
}

void generate_tail_options() {
    char tail_digits[5];
    strncpy(tail_digits, id_input + 14, 4);
    tail_digits[4] = '\0';

    if (strcmp(tail_digits, "****") == 0) {
        for (int i = 0; i < 10000; i++) {
            sprintf(tail_options[tail_count++], "%04d", i);
        }
    } else {
        int asterisk_count = 0;
        for (int i = 0; i < 4; i++) {
            if (tail_digits[i] == '*') asterisk_count++;
        }

        if (asterisk_count == 0) {
            strcpy(tail_options[tail_count++], tail_digits);
        } else if (asterisk_count == 1) {
            for (int i = 0; i < 10; i++) {
                char temp[5];
                strcpy(temp, tail_digits);
                for (int j = 0; j < 4; j++) {
                    if (temp[j] == '*') {
                        temp[j] = '0' + i;
                        break;
                    }
                }
                strcpy(tail_options[tail_count++], temp);
            }
        } else if (asterisk_count == 2) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    char temp[5];
                    strcpy(temp, tail_digits);
                    int replaced = 0;
                    for (int k = 0; k < 4; k++) {
                        if (temp[k] == '*') {
                            temp[k] = replaced == 0 ? '0' + i : '0' + j;
                            replaced++;
                            if (replaced == 2) break;
                        }
                    }
                    strcpy(tail_options[tail_count++], temp);
                }
            }
        } else if (asterisk_count == 3) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    for (int k = 0; k < 10; k++) {
                        char temp[5];
                        strcpy(temp, tail_digits);
                        int replaced = 0;
                        for (int l = 0; l < 4; l++) {
                            if (temp[l] == '*') {
                                temp[l] = replaced == 0 ? '0' + i : (replaced == 1 ? '0' + j : '0' + k);
                                replaced++;
                                if (replaced == 3) break;
                            }
                        }
                        strcpy(tail_options[tail_count++], temp);
                    }
                }
            }
} else {
            printf(">>> 错误：无效的后四位数字\n");
            exit(1);
        }
    }
}

void generate_all_options() {
    for (int c = 0; c < valid_city_count; c++) {
        for (int y = 0; y < birth_year_count; y++) {
            for (int m = 0; m < birth_month_count; m++) {
                int month = atoi(birth_month_options[m]);
                for (int d = 0; d < birth_day_count[month]; d++) {
                    for (int t = 0; t < tail_count; t++) {
                        sprintf(all_options[all_count], "%s%04d%s%s%s",
                                valid_cities[c],
                                birth_year_options[y],
                                birth_month_options[m],
                                birth_day_options[month][d],
                                tail_options[t]);
                        all_count++;
                        if (all_count >= MAX_OPTIONS) {
                            printf(">>> 警告：超出最大选项数量限制\n");
                            return;
                        }
                    }
                }
            }
        }
    }
    printf(">>> 数据集大小: %d\n", all_count);
}

void verify_all_options() {
    for (int i = 0; i < all_count; i++) {
        if (is_valid_id_number(all_options[i])) {
            strcpy(cache[cache_count++], all_options[i]);
        }
        
        // 每处理1000个选项打印一次进度
        if ((i + 1) % 1000 == 0 || i == all_count - 1) {
            printf("\r>>> 进度: %.2f%%", (float)(i + 1) / all_count * 100);
            fflush(stdout);
        }
    }
    printf("\n");
}