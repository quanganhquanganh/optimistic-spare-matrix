#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ShiftBitAPI.c"

int BUFFER_SIZE = 5;

void testUpdateEmptyEXB();
void testUpdateFullEXB();
void testUpdateWithEmptySQ();

int main(int argc, char** argv) 
{
    //------- EXB of Host -----------//
    testUpdateEmptyEXB();

    testUpdateFullEXB();

    testUpdateWithEmptySQ();
}

void testUpdateEmptyEXB()
{
    //PacketInEXBHost[i][0] = -1;
    //PacketInEXBHost[i][2] = -1;
    int result = checkUpdateEXBHost(-1, -1, BUFFER_SIZE);
    int isFullEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 0 || indexOfUpdate > BUFFER_SIZE)
    {
        printf("Wrong index of Update, should be 0 instead of %d\n", indexOfUpdate);
    }
    if(isFullEXB == 1)
    {
        printf("It should be an empty EXB\n");
    }
}

void testUpdateFullEXB()
{
    //PacketInEXBHost[i][0] = 7;
    //PacketInSQ[i][1] = 7 + BUFFER_SIZE;
    int result = checkUpdateEXBHost(7, 7 + BUFFER_SIZE, BUFFER_SIZE);
    int isFullEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 0 || indexOfUpdate > BUFFER_SIZE)
    {
        printf("At testUpdateFullEXB, wrong index of Update, should be 0 instead of %d\n", indexOfUpdate);
    }
    if(isFullEXB == 0)
    {
        printf("At testUpdateFullEXB, it should be a full EXB\n");
    }
}


void testUpdateWithEmptySQ()
{
    //The case SQ is empty;
    int result = checkUpdateEXBHost(7, -1, BUFFER_SIZE);
    int isFullEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 0 || indexOfUpdate > BUFFER_SIZE)
    {
        printf("At testUpdateWithEmptySQ, wrong index of update, should be 0 instead of %d\n", indexOfUpdate);
    }

    if(isFullEXB == 0)
    {
        printf("At testUpdateWithEmptySQ, it should be a non empty EXB\n");
    }

    return;
}

