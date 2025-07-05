#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]) 
{
    // Open syslog with LOG_USER
    openlog("writer", LOG_PID, LOG_USER);

    // Validate arguments
    if (argc != 3) 
    {
        syslog(LOG_ERR, "Invalid number of arguments", argv[0]);
        fprintf(stderr, "Usage: %s <file> <string>\n", argv[0]);
        closelog();
        return 1;
    }

    const char *filePath = argv[1];
    const char *writeStr = argv[2];

    // Try opening file for writing
    FILE *fp = fopen(filePath, "w");
    if (fp == NULL) 
    {
        syslog(LOG_ERR, "Failed to open file: %s", filePath);
        perror("Error opening file");
        closelog();
        return 1;
    }

    // Write to file
    if (fprintf(fp, "%s", writeStr) < 0)
    {
        syslog(LOG_ERR, "Failed to write to file: %s", filePath);
        perror("Error writing to file");
        fclose(fp);
        closelog();
        return 1;
    }

    // Log successful write
    syslog(LOG_DEBUG, "Writing \"%s\" to %s", writeStr, filePath);

    fclose(fp);
    closelog();
    return 0;
}
