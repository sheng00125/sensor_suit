

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <pthread.h>

using namespace std;

using namespace cv;

uint8_t buffer[255] = {};
uint16_t number_cnt;
uint16_t number_pre;
uint8_t buffer2[2400] = {};
uint16_t ok = 1;
unsigned char ar1ray[480][800] = {};

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        cout << "socket 创建失败：" << endl;
        exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7);
    addr.sin_addr.s_addr = inet_addr("192.168.1.10");

    int res = connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1)
    {
        cout << "bind 链接失败：" << endl;
        exit(-1);
    }
    cout << "bind 链接成功：" << endl;
    number_cnt = 0;

    while (1)
    {
        read(socket_fd, buffer, 1);
        if (buffer[0] == (0xaa))
        {
            read(socket_fd, buffer, 1);
            if (buffer[0] == (0x55))
            {
                read(socket_fd, buffer, 2);
                number_pre = number_cnt;
                number_cnt = ((buffer[1] << 8) & 0x0f00) + buffer[0];
                // cout << number_cnt << endl;
                if (number_cnt != (number_pre + 1) & (number_pre != 479))
                {
                    cout << "Lost !" << endl;
                    ok = 0;
                }
                else
                {
                    ok = 1;
                }
                read(socket_fd, buffer2, 800);
                cout << number_cnt << endl;
                if (ok == 1)
                {

                    memcpy((ar1ray + number_cnt), (buffer2), (800));
                    if (number_cnt == 479)
                    {

                        cv::Mat matA(480, 800, CV_8U, (unsigned char *)ar1ray);
                        imshow("this", matA);
                        waitKey(1);
                        cout << "got!" << endl;
                    }
                }

                // memset(ar1ray[number_cnt], (number_cnt), (900));
            }
        }
    }

    close(socket_fd);

    return 0;
}
