#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_COUNT 8
#define ID_LENGTH 18
#define MEMORY_POLL_INTERVAL 1

char id_input[ID_LENGTH + 1];
int disable_memory = 0;

void *get_memory_usage(void *arg) {
    while (!disable_memory) {
        FILE *file = fopen("/proc/self/status", "r");
        char line[128];

        while (fgets(line, 128, file) != NULL) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                printf(">>> 正在将数据集加载到内存中，已使用 (MB): %s", line + 7);
                break;
            }
        }

        fclose(file);
        sleep(MEMORY_POLL_INTERVAL);
    }

    pthread_exit(NULL);
}

int is_valid_id_number(const char *id_number) {
    int factors[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    char id_remainders[] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
    int total_sum = 0;

    for (int i = 0; i < ID_LENGTH - 1; i++) {
        if (id_number[i] < '0' || id_number[i] > '9') {
            return 0;
        }
        total_sum += (id_number[i] - '0') * factors[i];
    }

    int remainder = total_sum % 11;
    return id_remainders[remainder] == id_number[ID_LENGTH - 1];
}

void split_and_verify_range(const char *option_range, FILE *output_file) {
    char real_id[ID_LENGTH + 1];

    for (int i = 0; option_range[i]; i += ID_LENGTH) {
        strncpy(real_id, option_range + i, ID_LENGTH);
        real_id[ID_LENGTH] = '\0';

        if (is_valid_id_number(real_id)) {
            fprintf(output_file, "%s\n", real_id);
        }
    }
}

int main() {
    printf("请输入身份证号码，用星号 (*) 替代未知数字：");
    fgets(id_input, sizeof(id_input), stdin);
    id_input[strcspn(id_input, "\n")] = '\0'; // Remove newline character

    // Check if input length is valid
    if (strlen(id_input) != ID_LENGTH) {
        printf(">>> 请输入18位身份证号码，并用星号 (*) 替代缺失的数字。\n");
        return 1;
    }

    int count_asterisks = 0;
    for (int i = 0; i < ID_LENGTH; i++) {
        if (id_input[i] == '*') {
            count_asterisks++;
        }
    }

    if (count_asterisks == 0) {
        printf(">>> 没有缺失的数字。\n");
        return 1;
    }

    if (count_asterisks > ID_LENGTH) {
        printf(">>> 星号的数量不能超过18个。\n");
        return 1;
    }

    char city_code[7];
    strncpy(city_code, id_input, 6);
    city_code[6] = '\0';

    // Open the citycodes.txt file and read valid cities
    FILE *city_file = fopen("citycodes.txt", "r");
    if (city_file == NULL) {
        printf(">>> 错误：无法打开城市代码文件\n");
        return 1;
    }

    char valid_cities[100][64];
    int valid_cities_count = 0;
    char line[64];

    while (fgets(line, sizeof(line), city_file)) {
        if (strstr(line, city_code) != NULL) {
            strncpy(valid_cities[valid_cities_count], line, sizeof(line));
            valid_cities_count++;
        }
    }

    fclose(city_file);

    if (valid_cities_count == 0) {
        printf(">>> 错误：无效的城市代码\n");
        return 1;
    }

    printf(">>> 有效的城市代码:\n");
    for (int i = 0; i < valid_cities_count; i++) {
        printf("%s", valid_cities[i]);
    }

    char birth_year[5];
    strncpy(birth_year, id_input + 6, 4);
    birth_year[4] = '\0';

    int current_year = atoi(birth_year);
    int birth_year_options[100];
    int birth_year_options_count = 0;

    if (strcmp(birth_year, "****") == 0) {
        for (int i = 1949; i <= current_year; i++) {
            birth_year_options[birth_year_options_count++] = i;
        }
    } else {
        int year = atoi(birth_year);
        if (year >= 1949 && year <= current_year) {
            birth_year_options[birth_year_options_count++] = year;
        }
    }

    printf(">>> 有效的出生年份:\n");
    for (int i = 0; i < birth_year_options_count; i++) {
        printf("%d\n", birth_year_options[i]);
    }

    char birth_month[3];
    strncpy(birth_month, id_input + 10, 2);
    birth_month[2] = '\0';

    char *month_options[13] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
    int birth_month_options_count = 0;

    if (strcmp(birth_month, "**") == 0) {
        birth_month_options_count = 12;
    } else {
        int month = atoi(birth_month);
        if (month >= 1 && month <= 12) {
            birth_month_options_count = 1;
            month_options[0] = birth_month;
        } else {
            printf(">>> 错误：无效的月份\n");
            return 1;
        }
    }

    printf(">>> 有效的出生月份:\n");
    for (int i = 0; i < birth_month_options_count; i++) {
        printf("%s\n", month_options[i]);
    }

    char birth_day[3];
    strncpy(birth_day, id_input + 12, 2);
    birth_day[2] = '\0';

    int max_day = 31;
    if (strcmp(birth_month, "02") == 0) {
        max_day = 28; // Assuming non-leap year by default
    } else if (strcmp(birth_month, "04") == 0 || strcmp(birth_month, "06") == 0 ||
               strcmp(birth_month, "09") == 0 || strcmp(birth_month, "11") == 0) {
        max_day = 30;
    }

    char *day_options[32];
    int birth_day_options_count = 0;

    if (strcmp(birth_day, "**") == 0) {
        for (int i = 1; i <= max_day; i++) {
            char *day = (char *)malloc(3 * sizeof(char));
            sprintf(day, "%02d", i);
            day_options[birth_day_options_count++] = day;
        }
    } else {
        int day = atoi(birth_day);
        if (day >= 1 && day <= max_day) {
            char *day_str = (char *)malloc(3 * sizeof(char));
            sprintf(day_str, "%02d", day);
            day_options[birth_day_options_count++] = day_str;
        } else {
            printf(">>> 错误：无效的日期\n");
            return 1;
        }
    }

    printf(">>> 有效的出生日期:\n");
    for (int i = 0; i < birth_day_options_count; i++) {
        printf("%s\n", day_options[i]);
    }

    char tail_digits[5];
    strncpy(tail_digits, id_input + 14, 4);
    tail_digits[4] = '\0';

    printf(">>> 顺序号: %s\n", tail_digits);

    char *tail_options[10000];
    int tail_options_count = 0;

    if (strcmp(tail_digits, "****") == 0) {
        for (int i = 0; i < 10000; i++) {
            char *tail = (char *)malloc(5 * sizeof(char));
            sprintf(tail, "%04d", i);
            tail_options[tail_options_count++] = tail;
        }
    } else {
        int num_asterisks = 0;
        for (int i = 0; i < 4; i++) {
            if (tail_digits[i] == '*') {
                num_asterisks++;
            }
        }

        if (num_asterisks == 0) {
            char *tail = (char *)malloc(5 * sizeof(char));
            strcpy(tail, tail_digits);
            tail_options[tail_options_count++] = tail;
        } else {
            // Implement logic to generate tail options based on asterisks
        }
    }

    printf(">>> 有效的顺序号:\n");
    for (int i = 0; i < tail_options_count; i++) {
        printf("%s\n", tail_options[i]);
    }

    pthread_t memory_thread;
    pthread_create(&memory_thread, NULL, get_memory_usage, NULL);

    int total_count = valid_cities_count * birth_year_options_count * birth_month_options_count * birth_day_options_count * tail_options_count;
    printf(">>> 数据集大小: %d\n", total_count);

    char **all_options = (char **)malloc(total_count * sizeof(char *));
    int all_options_count = 0;

    for (int i = 0; i < valid_cities_count; i++) {
        for (int j = 0; j < birth_year_options_count; j++) {
            for (int k = 0; k < birth_month_options_count; k++) {
                for (int l = 0; l < birth_day_options_count; l++) {
                    for (int m = 0; m < tail_options_count; m++) {
                        int option_length = strlen(valid_cities[i]) + strlen(birth_year_options[j]) + strlen(month_options[k]) + strlen(day_options[l]) + strlen(tail_options[m]) + 1;
                        char *option = (char *)malloc(option_length * sizeof(char));
                        sprintf(option, "%s%s%s%s%s", valid_cities[i], birth_year_options[j], month_options[k], day_options[l], tail_options[m]);
                        all_options[all_options_count++] = option;
                    }
                }
            }
        }
    }

    disable_memory = 1;
    sleep(1);
    system("cls");
    disable_memory = 0;

    printf(">>> 所有线程已完成\n");

    FILE *output_file = fopen("resultID.txt", "a");
    if (output_file == NULL) {
        printf(">>> 错误：无法打开结果文件\n");
        return 1;
    }

    pthread_t threads[THREAD_COUNT];
    int range_size = all_options_count / THREAD_COUNT;
    int remainder = all_options_count % THREAD_COUNT;
    int offset = 0;

    for (int i = 0; i < THREAD_COUNT; i++) {
        int start_index = offset;
        int end_index = offset + range_size + (i < remainder ? 1 : 0);
        offset = end_index;

        pthread_create(&threads[i], NULL, (void *(*)(void *))split_and_verify_range, (void *)start_index);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < all_options_count; i++) {
        free(all_options[i]);
    }
    free(all_options);

    fclose(output_file);

    return 0;
}
