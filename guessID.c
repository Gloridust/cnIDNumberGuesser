#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 100    // 假设每行不会超过100个字符

char* id_input;

int is_valid_id_number(const char* id_number) {
    if (strlen(id_number) != 18) {
        return 0;
    }
    const int factors[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    const char* id_remainders = "10x98765432";
    int total_sum = 0;
    for (int i = 0; i < 17; ++i) {
        total_sum += (id_number[i] - '0') * factors[i];
    }
    int remainder = total_sum % 11;
    return id_remainders[remainder] == id_number[17];
}

char** read_city_codes(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("错误：无法打开城市代码文件\n");
        exit(1);
    }

    // Count the number of non-empty lines in the file
    int line_count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            line_count++;
        }
    }
    rewind(file);

    // Allocate memory for city codes
    char** city_codes = malloc(line_count * sizeof(char*));

    // Read non-empty city codes from file
    int index = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            city_codes[index] = malloc(7 * sizeof(char)); // assuming city code length is 6 characters
            sscanf(line, "%s", city_codes[index]); // Read only the first string (city code)
            index++;
        }
    }

    fclose(file);
    return city_codes;
}

void free_city_codes(char** city_codes, int count) {
    for (int i = 0; i < count; ++i) {
        free(city_codes[i]);
    }
    free(city_codes);
}

void process_options(const char* city_code, const char* birth_year, const char* birth_month, const char* birth_day, const char* tail_digits) {
    // Function to process options and verify ID numbers
    // Placeholder for implementation
}

int main() {
    char** city_codes = read_city_codes("citycodes.txt", NULL); // Pass NULL as count pointer
    const int city_count = 0; // Not used in this context, since read_city_codes doesn't return count

    id_input = malloc(19 * sizeof(char)); // Allocate memory for ID input
    printf("请输入身份证号码，用星号 (*) 替代未知数字：");
    fgets(id_input, 19, stdin);
    id_input[strcspn(id_input, "\n")] = 0; // Remove newline character

    // Validate input length
    if (strlen(id_input) != 18) {
        printf("请输入18位身份证号码，并用星号 (*) 替代缺失的数字。\n");
        free(id_input);
        return 1;
    }

    // Check for asterisks
    int count_asterisks = 0;
    for (int i = 0; i < 18; ++i) {
        if (id_input[i] == '*') {
            count_asterisks++;
        }
    }
    if (count_asterisks > 18) {
        printf("星号的数量不能超过18个。\n");
        free(id_input);
        return 1;
    }
    if (count_asterisks == 0) {
        printf("没有缺失的数字。\n");
        free(id_input);
        return 1;
    }

    // Extract parts of ID
    char city_code[7], birth_year[5], birth_month[3], birth_day[3], tail_digits[5];
    strncpy(city_code, id_input, 6); city_code[6] = '\0';
    strncpy(birth_year, id_input + 6, 4); birth_year[4] = '\0';
    strncpy(birth_month, id_input + 10, 2); birth_month[2] = '\0';
    strncpy(birth_day, id_input + 12, 2); birth_day[2] = '\0';
    strncpy(tail_digits, id_input + 14, 4); tail_digits[4] = '\0';

    printf(">>> 城市代码: %s\n", city_code);
    printf(">>> 出生年份: %s\n", birth_year);
    printf(">>> 出生月份: %s\n", birth_month);
    printf(">>> 出生日期: %s\n", birth_day);
    printf(">>> 顺序号: %s\n", tail_digits);

    // Validate city code
    int valid_city = 0;
    for (int i = 0; i < city_count; ++i) {
        if (strstr(city_codes[i], city_code) != NULL) {
            valid_city = 1;
            break;
        }
    }
    if (!valid_city) {
        printf("错误：无效的城市代码\n");
        free(id_input);
        free_city_codes(city_codes, city_count); // Free allocated memory for city codes
        return 1;
    }

    // Process options
    process_options(city_code, birth_year, birth_month, birth_day, tail_digits);

    // Free allocated memory
    free(id_input);
    free_city_codes(city_codes, city_count); // Free allocated memory for city codes

    return 0;
}
