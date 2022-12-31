#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "LidarDriver.h"

FILE *lidarFd;

static FILE *lidarInit()
{
    lidarFd = popen("./ultra_simple --channel --serial /dev/ttyUSB0 115200", "r");
    if (lidarFd == NULL)
    {
        fwrite("Could not open lidarFD.\n", strlen("Could not open lidarFD.\n"), 1, stderr);
        exit(1);
    }
    return lidarFd;
}

static void lidarTerminate()
{
    pclose(lidarFd);
}

readLidar()
{
}

// printf("Top of pipe_test\n");
// LIDAR_fd = popen("./LIDAR_drivers/ultra_simple --channel --serial /dev/ttyUSB0 115200", "r");
// line = NULL;
// char *tok;
// size_t len = 0;
// ssize_t nread;
// // float distances[360];
// // float deg;
// // float dist;
// if (LIDAR_fd == NULL)
// {
//     printf("Failed to popen().\n");
//     exit(1);
// }
// else
// {
//     while ((nread = getline(&line, &len, LIDAR_fd)) != -1)
//     {
//         printf("Retrieved line of length %zd: \n", nread);
//         fwrite(line, nread, 1, stdout);
//         // line is about every 0.47 deg
//         if (strstr(line, "theta") == NULL || strstr(line, "Dist") == NULL)
//         {
//             continue;
//         }
//         tok = strtok(line, ": ");
//         while (tok != NULL)
//         {
//             if (strcmp(tok, "theta") == 0)
//             {
//                 tok = strtok(NULL, ": ");
//                 printf("theta: [%f]\n", atof(tok));
//             }
//             else if (strcmp(tok, "Dist") == 0)
//             {
//                 tok = strtok(NULL, ": ");
//                 printf("Dist: [%f]\n", atof(tok));
//             }
//             else
//             {
//                 tok = strtok(NULL, ": ");
//             }
//         }
//     }
//     pclose(LIDAR_fd);
//     free(line);
// }
// printf("Bottom of pipe_test\n");