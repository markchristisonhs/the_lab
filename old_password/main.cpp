#include <QCoreApplication>
#include <iostream>

using namespace std;

void  CheckBackDoorPassword(__uint8_t input);
unsigned char getCRC(unsigned char message[], unsigned char length);

int main(int argc, char *argv[])
{
    for(__uint8_t ui=1; ui<=31; ui++)
    {
        CheckBackDoorPassword(ui);
    }
}

void CheckBackDoorPassword(__uint8_t input){
    char password[10];
    char packet[2];

    __uint8_t dayOfMonth ;
    __uint8_t pwU8 = 0;
    __uint8_t pwL8 = 0;


    dayOfMonth = input;
    packet[0]= dayOfMonth/10;
    packet[1] = dayOfMonth - (packet[0] * 10);
    memset(password,0x00,10);
    pwU8 = getCRC((unsigned char *)packet,2);

    packet[0]= (dayOfMonth + 15)/20;
    packet[1] = dayOfMonth*2  - (packet[0] * 11);
    pwL8 = getCRC((unsigned char *)packet,2);


    sprintf(password,"%d%d",pwU8,pwL8);

    cout << password << endl;
}

unsigned char getCRC(unsigned char message[], unsigned char length)
{
    __uint8_t CRC7_POLY = 0x91;
    __uint8_t i, j, crc = 0;

    for (i = 0; i < length; i++)
    {
        crc ^= message[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc ^= CRC7_POLY;
            crc >>= 1;
        }
    }
    return crc;
}
