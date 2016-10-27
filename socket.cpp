#include "socket.h"
#include <algorithm>
#include <iostream>
#include <rfb/rfb.h>

Socket::Socket()
{
    this->listener = socket(AF_INET, SOCK_STREAM, 0);
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(3425);
    this->addr.sin_addr.s_addr = INADDR_ANY;
}

Socket::Socket(int ip, int port, int sock, int type_sock, int type_protocol)
{
    this->listener = socket(sock, type_sock, type_protocol);
    this->addr.sin_family = sock;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = ip;
}

int Socket::start(int argc, char *argv[])
{
    /* Простаивание потоков в ожидании коннекта ~15sec*/
    int size = 1024;
    timeval timeout;
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;

    if(this->listener < 0)
    {
        perror("socket");
        exit(1);
    }

    fcntl(this->listener, F_SETFL, O_NONBLOCK);

    if(bind(this->listener, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 10);

    set<int> clients;
    clients.clear();

    while(1)
    {
        // Заполняем множество сокетов
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(listener, &readset);

        for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
            FD_SET(*it, &readset);

        // Задаём таймаут


        // Ждём события в одном из сокетов
        int clts = *clients.begin() > *clients.end() ? *clients.begin() : *clients.end();
        int mx = max(listener, *max_element(clients.begin(), clients.end()));
        if(select(mx+1, &readset, NULL, NULL, &timeout) <= 0)
        {
            continue;
        }

        // Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(listener, &readset))
        {
            // Поступил новый запрос на соединение, используем accept
            int sock = accept(listener, NULL, NULL);
            if(sock < 0)
            {
                perror("accept");
                exit(3);
            }

            fcntl(sock, F_SETFL, O_NONBLOCK);

            clients.insert(sock);
        }

        for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if(FD_ISSET(*it, &readset))
            {
            char *data_client = new char[size];
            // Поступили данные от клиента, читаем их
            bytes_read = recv(*it, data_client, 1024, 0);
            if(bytes_read <= 0)
            {
                    // Соединение разорвано, удаляем сокет из множества
                close(*it);
                clients.erase(*it);
                continue;
            }
            const char *data_answer = new char[1024];
            //data_answer = treatmentData(data_client);
            // Отправляем данные обратно клиенту
            //код
            send(*it, data_answer, 1024, 0);
            //delete data_answer;delete data_client;
            }
        }
    }
    return 1;
}
