#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/**
 * Asus ROG GL553VW Fan
 * Made By Zlynt (Ivan Teixeira)
 **/

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define ACPI_CALL_PATH "/proc/acpi/call"

#define VERSION "1.0.0" //Driver version

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

bool file_exists(const char *path)
{
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
        return false;

    fclose(fptr);

    return true;
}

void write_to_file(const char *file_path, const char *data)
{
    FILE *fptr = fopen(file_path, "w");
    fputs(data, fptr);
    fclose(fptr);
}

char *read_from_file(const char *filename)
{
    long int size = 0;
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        fputs("File error.\n", stderr);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    char *result = (char *)malloc(size);
    if (!result)
    {
        fputs("Memory error.\n", stderr);
        fclose(file);
        return NULL;
    }

    if (fread(result, 1, size, file) != size)
    {
        fputs("Read error.\n", stderr);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return result;
}

void write_acpi_command(const char *command)
{
    write_to_file(ACPI_CALL_PATH, command);
    char *received_response = read_from_file(ACPI_CALL_PATH);
}

void set_auto_fan()
{
    write_acpi_command("\\_SB.PCI0.LPCB.EC0.SFNV 0 0");
}

void set_fan_speed(int speed)
{
    float finalSpeed = (float)speed;
    if (finalSpeed < 0)
        finalSpeed = 0;
    else if (finalSpeed > 100)
        finalSpeed = 100;
    finalSpeed = finalSpeed * 2.55;

    char finalSpeedString[3];
    sprintf(finalSpeedString, "%0.0f", finalSpeed);

    //Prepare the command
    char *base_command = "\\_SB.PCI0.LPCB.EC0.SFNV 1 ";
    char *final_command = concat(base_command, finalSpeedString);

    //Write Command
    write_acpi_command(final_command);

    //Free allocated memory
    free(final_command);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Send only 1 parameter.\n");
        return -1;
    }

    //Check if it is running has root
    if (geteuid() != 0)
    {
        printf("Run me has root.\n");
        return -1;
    }

    system("modprobe acpi_call");
    //Check if acpi_call is working
    if (!file_exists(ACPI_CALL_PATH))
    {
        printf("Cannot load acpi module into kernel.\n");
        return -1;
    }

    int fan_value = atoi(argv[1]);
    if (fan_value == 0)
    {
        set_auto_fan();
        printf("Fan auto mode enabled.\n");
    }
    else
    {
        printf("Fan speed set to %d%%.\n", fan_value);
        set_fan_speed(fan_value);
    }
    return 0;
}