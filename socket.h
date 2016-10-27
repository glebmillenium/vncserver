#ifndef SOCKET_H
#define SOCKET_H


/**
 * sys/types.h - различные типы данных
 * sys/socket.h - работа с сокетом
 * sys/time.h - работа со временем (С lang)
 * netinet/in.h - работа с протоколом in
 * stdio - консоль ввода/вывода
 * unistd.h - функции, константы POSIX (Ip, mac ..)
 * fcntl.h - открытие и вывод каталогов
 * algorithm - работа с каталогами
 * set - реализация контейнера
 *
 */
#include <rfb/rfb.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>
#include <cstring>
#include <string>
using namespace std;

/**
 * Class Socket - listener socket on network Ethernet
 * on safely chanel (SOCK_STREAM)
 *
 */
class Socket
{
private:
    /**
     * Field data class
     */
    int listener;
    struct sockaddr_in addr;
    int bytes_read;

public:
    /**
     * Constructor
     */
    Socket();

    Socket(int ip, int port, int sock, int type_sock, int type_protocol = 0);

    int start(int argc, char *argv[]);
};

#endif // SOCKET_H
