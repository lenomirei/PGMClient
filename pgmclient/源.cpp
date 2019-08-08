#include <iostream>
#include<winsock2.h>
#include<WS2tcpip.h>	//ip_mreqͷ
#include <wsrm.h>
#include <stdio.h>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA WSAData;
    WORD sockVersion = MAKEWORD(2, 2);
    if (WSAStartup(sockVersion, &WSAData) != 0)
        return 0;


    SOCKET        s,
        sclient;
    SOCKADDR_IN   salocal,
        sasession;
    int           sasessionsz, dwSessionPort;

    FILE * fp;
    fopen_s(&fp, "aaatest.webm", "wb+");

    s = socket(AF_INET, SOCK_RDM, IPPROTO_RM);

    //
    // The bind port (dwSessionPort) specified should match that
    // which the sender specified in the connect call
    //
    dwSessionPort = 1234;
    salocal.sin_family = AF_INET;
    salocal.sin_port = htons(dwSessionPort);
    salocal.sin_addr.s_addr = inet_addr("224.4.5.6");
    int receive_buf_size = 65536 * 10;
    if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&receive_buf_size, sizeof(receive_buf_size)) < 0)
    {
        std::cout << "setsockopt():SO_RCVBUF failed with error code" << WSAGetLastError() << std::endl;
    }


    bind(s, (SOCKADDR *)&salocal, sizeof(salocal));

    //
    // Set all relevant receiver socket options here
    //

    listen(s, 10);

    sasessionsz = sizeof(sasession);
    sclient = accept(s, (SOCKADDR *)&sasession, &sasessionsz);

    if (setsockopt(sclient, SOL_SOCKET, SO_RCVBUF, (char*)&receive_buf_size, sizeof(receive_buf_size)) < 0)
    {
        std::cout << "setsockopt():SO_RCVBUF failed with error code" << WSAGetLastError() << std::endl;
    }
    //
    // accept will return the client socket and we are now ready
    // to receive data on the new socket!
    //
    LONG BytesRead;
    char pTestBuffer[1400];

    sockaddr_in clientAddr;
    int iAddrlen = sizeof(clientAddr);

    while (1)
    {
        memset(pTestBuffer, 0, 1400);
        BytesRead = recvfrom(sclient, pTestBuffer, 1400, 0, (sockaddr*)&clientAddr, &iAddrlen);
        if (BytesRead == 0)
        {
            fprintf(stdout, "Session was terminated\n");
        }
        else if (BytesRead == -1)
        {
            std::cout << "no data?!" << std::endl;
        }
        if (BytesRead > 0)
        {
            fwrite(pTestBuffer, 1, BytesRead, fp);
            std::cout << BytesRead << std::endl;
        }
    }
    fclose(fp);
    WSACleanup();
    return 0;
}