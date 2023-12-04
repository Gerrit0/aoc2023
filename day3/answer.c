#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_LINE = 255;

int is_symbol(char c)
{
    return !isdigit(c) && c != '.';
}

void part1(const char *file)
{

    char buf1[MAX_LINE];
    char buf2[MAX_LINE];

    memset(buf1, '.', sizeof(buf1));
    memset(buf2, '.', sizeof(buf2));
    buf1[MAX_LINE] = '\0';
    buf2[MAX_LINE] = '\0';

    char *previous_line = buf1;
    char *line = buf2;

    FILE *fp = fopen(file, "r");
    if (!fp)
    {
        puts("Failed to open input file.");
        exit(1);
    }

    int all_part_nums = 0;

    int line_count = 0;
    while (fgets(line, MAX_LINE, fp))
    {
        // Doctor the line to remove trailing newline as it'll be detected as a symbol
        size_t line_len = strlen(line);
        line[line_len - 1] = '.';
        printf("%s\n", line);

        for (int i = 0; i < MAX_LINE; i++)
        {
            if (!line[i])
            {
                break;
            }

            if (isdigit(line[i]))
            {
                // This is a number, maybe a part number.
                int num = atoi(line + i);
                int is_part =
                    is_symbol(line[i > 0 ? i - 1 : i]) ||        // left of part num
                    is_symbol(previous_line[i > 0 ? i - 1 : i]); // left above of part num

                while (isdigit(line[i]))
                {
                    is_part |= is_symbol(previous_line[i]);
                    i++;
                }
                is_part |= is_symbol(previous_line[i]);
                is_part |= is_symbol(line[i]);

                if (is_part)
                {
                    printf("part from current line: %d\n", num);
                    all_part_nums += num;
                }
            }

            if (is_symbol(line[i]))
            {
                // Check previous line for part part numbers adjacent to us
                int part_start = -1;
                if (i > 0 && isdigit(previous_line[i - 1]))
                {
                    part_start = i - 1;
                    while (part_start > 0 && isdigit(previous_line[part_start - 1]))
                    {
                        part_start--;
                    }
                }
                else if (isdigit(previous_line[i]))
                {
                    part_start = i;
                }
                else if (isdigit(previous_line[i + 1]))
                {
                    part_start = i + 1;
                }

                if (part_start != -1)
                {
                    int num = atoi(previous_line + part_start);
                    printf("part from previous line: %d\n", num);
                    all_part_nums += num;

                    // Edge case:
                    // 714.840
                    // ...*...
                    while (isdigit(previous_line[part_start]))
                    {
                        part_start++;
                    }
                    part_start++;

                    if (part_start == i + 1 && isdigit(previous_line[part_start]))
                    {
                        int num = atoi(previous_line + part_start);
                        printf("part from previous line: %d\n", num);
                        all_part_nums += num;
                    }
                }
            }
        }

        char *swap = previous_line;
        previous_line = line;
        line = swap;
        line_count++;
    }

    fclose(fp);

    printf("Sum of all part numbers: %d\n", all_part_nums);
    printf("Input was %d lines\n", line_count);
}

// Very low memory method of iterating through the file used in part 1
// is biting me for part 2. I can't easily check each symbol for adjacent
// parts without reading the next line... so I guess we'll do that, then process
// the last line by filling it with dots.

int process_line(const char *previous_line, const char *line, const char *next_line)
{
    int result = 0;

    for (int i = 0; i < MAX_LINE; i++)
    {
        if (line[i] == '\0')
        {
            break;
        }

        if (line[i] != '*')
        {
            continue;
        }

        // Gear if we have two adjacent part numbers
        int part_num1 = -1;
        int part_num2 = -1;

        // Check previous line for part part numbers adjacent to us
        int part_start = -1;
        if (i > 0 && isdigit(previous_line[i - 1]))
        {
            part_start = i - 1;
            while (part_start > 0 && isdigit(previous_line[part_start - 1]))
            {
                part_start--;
            }
        }
        else if (isdigit(previous_line[i]))
        {
            part_start = i;
        }
        else if (isdigit(previous_line[i + 1]))
        {
            part_start = i + 1;
        }

        if (part_start != -1)
        {
            part_num1 = atoi(previous_line + part_start);

            // Edge case:
            // 714.840
            // ...*...
            while (isdigit(previous_line[part_start]))
            {
                part_start++;
            }
            part_start++;

            if (part_start == i + 1 && isdigit(previous_line[part_start]))
            {
                part_num2 = atoi(previous_line + part_start);
            }
        }

        // Check current line for part numbers
        if (i > 0 && isdigit(line[i - 1]))
        {
            part_start = i - 1;
            while (part_start > 0 && isdigit(line[part_start - 1]))
            {
                part_start--;
            }

            if (part_num1 == -1)
            {
                part_num1 = atoi(line + part_start);
            }
            else if (part_num2 == -1)
            {
                part_num2 = atoi(line + part_start);
            }
            else
            {
                continue; // Not a gear, adjacent to 3 parts.
            }
        }
        if (isdigit(line[i + 1]))
        {
            part_start = i + 1;
            if (part_num1 == -1)
            {
                part_num1 = atoi(line + part_start);
            }
            else if (part_num2 == -1)
            {
                part_num2 = atoi(line + part_start);
            }
            else
            {
                continue; // Not a gear, adjacent to 3 parts.
            }
        }

        // Check next line for part numbers
        part_start = -1;
        if (i > 0 && isdigit(next_line[i - 1]))
        {
            part_start = i - 1;
            while (part_start > 0 && isdigit(next_line[part_start - 1]))
            {
                part_start--;
            }
        }
        else if (isdigit(next_line[i]))
        {
            part_start = i;
        }
        else if (isdigit(next_line[i + 1]))
        {
            part_start = i + 1;
        }

        if (part_start != -1)
        {
            if (part_num1 == -1)
            {
                part_num1 = atoi(next_line + part_start);
            }
            else if (part_num2 == -1)
            {
                part_num2 = atoi(next_line + part_start);
            }
            else
            {
                continue; // Adjacent to 3 parts
            }

            // Edge case:
            // ...*...
            // 714.840
            while (isdigit(next_line[part_start]))
            {
                part_start++;
            }
            part_start++;

            if (part_start == i + 1 && isdigit(next_line[part_start]))
            {
                if (part_num2 == -1)
                {
                    part_num2 = atoi(next_line + part_start);
                }
                else
                {
                    continue; // Adjacent to 3 parts
                }
            }
        }

        if (part_num1 != -1 && part_num2 != -1)
        {
            printf("Found gear, %d / %d\n", part_num1, part_num2);
            result += part_num1 * part_num2;
        }
    }

    return result;
}

void part2(const char *file)
{
    char buf1[MAX_LINE];
    char buf2[MAX_LINE];
    char buf3[MAX_LINE];

    memset(buf1, '.', sizeof(buf1));
    memset(buf2, '.', sizeof(buf2));
    memset(buf3, '.', sizeof(buf3));
    buf1[MAX_LINE] = '\0';
    buf2[MAX_LINE] = '\0';
    buf3[MAX_LINE] = '\0';

    char *previous_line = buf1;
    char *line = buf2;
    char *next_line = buf3;

    FILE *fp = fopen(file, "r");
    if (!fp)
    {
        puts("Failed to open input file.");
        exit(1);
    }

    int total_gear_ratios = 0;
    while (fgets(next_line, MAX_LINE, fp))
    {
        // Doctor the line to remove trailing newline as it'll be detected as a symbol
        size_t line_len = strlen(next_line);
        next_line[line_len - 1] = '.';

        int line_result = process_line(previous_line, line, next_line);
        printf("%s    %d\n", line, line_result);
        total_gear_ratios += line_result;

        char *temp = previous_line;
        previous_line = line;
        line = next_line;
        next_line = temp;
    }

    memset(next_line, '.', MAX_LINE);
    next_line[MAX_LINE] = '\0';
    total_gear_ratios += process_line(previous_line, line, next_line);

    fclose(fp);

    // 62,048,346 is too low
    printf("Total gear ratios: %d\n", total_gear_ratios);
}

int main(int argc, const char **argv)
{
    if (argc != 2)
    {
        puts("Usage: ./answer <input>");
        return 1;
    }

    // part1(argv[1]);
    part2(argv[1]);

    return 0;
}
