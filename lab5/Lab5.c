#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//"--input Test2.bmp", "--output C:\\Users\\1\\Desktop\\Algo\\GoF\\Frame", "--max_iter 101", "--dump_freq 1"
size_t sizeOfFile(const char *file_name)
{
    size_t fileSize = 0;
    FILE *f = fopen(file_name, "rb");
    if (f == NULL)
        fileSize = -1;
    else
    {
        fseek(f, 0, SEEK_END);
        fileSize = ftell(f);
        fclose(f);
    }
    return fileSize;
}

void pushOldGame(char *bites, unsigned int **a, int start, int x, int y)
{
    int id = 128;
    int x_oct = x / 8;
    if (x % 8 > 0)
        x_oct++;
    for (int i = y - 1; i >= 0; --i)
    {
        for (int j = 0; j < x_oct + (4 - x_oct % 4) % 4; ++j)
        {
            for (int p = 0; p < 8; ++p)
            {
                if (j * 8 + p < x)
                {
                    a[i][j * 8 + p] = ((unsigned int)bites[start] / id) % 2;
                }
                id /= 2;
            }
            start++;
            id = 128;
        }
    }
}

void newStep(unsigned int **a, unsigned int **b, int start, int x, int y)
{
    int count;
    for (int i = 1; i < y - 1; ++i)
    {
        for (int j = 1; j < x - 1; ++j)
        {
            count = a[i - 1][j - 1] + a[i - 1][j] + a[i][j - 1] + a[i + 1][j] + a[i + 1][j + 1] + a[i][j + 1] + a[i - 1][j + 1] + a[i + 1][j - 1];
            if (a[i][j] == 0 && (count == 5 || count == 6))
                b[i][j] = 0;
            else if (a[i][j] == 1 && count == 5)
                b[i][j] = 0;
            else
                b[i][j] = 1;
        }
    }

    count = a[0][1] + a[1][0] + a[1][1];
    if (a[0][0] == 1 && count == 0)
        b[0][0] = 0;
    else if (a[0][0] == 0 && count < 2)
        b[0][0] = 0;
    else
        b[0][0] = 1;

    count = a[y - 2][0] + a[y - 1][1] + a[y - 2][1];
    if (a[y - 1][0] == 1 && count == 0)
        b[y - 1][0] = 0;
    else if (a[0][0] == 0 && count < 2)
        b[y - 1][0] = 0;
    else
        b[y - 1][0] = 1;

    count = a[0][x - 2] + a[1][x - 1] + a[1][x - 2];
    if (a[0][x - 1] == 1 && count == 0)
        b[0][x - 1] = 0;
    else if (a[0][x - 1] == 0 && count < 2)
        b[0][x - 1] = 0;
    else
        b[0][x - 1] = 1;

    count = a[y - 2][x - 1] + a[y - 2][x - 2] + a[y - 1][x - 2];
    if (a[y - 1][x - 1] == 1 && count == 0)
        b[y - 1][x - 1] = 0;
    else if (a[y - 1][x - 1] == 0 && count < 2)
        b[y - 1][x - 1] = 0;
    else
        b[y - 1][x - 1] = 1;

    for (int i = 1; i < y - 1; ++i)
    {
        count = a[i - 1][1] + a[i][1] + a[i + 1][1] + a[i - 1][0] + a[i + 1][0];
        if (a[i][0] == 0 && (count == 2 || count == 3))
            b[i][0] = 0;
        else if (a[i][0] == 1 && count == 2)
            b[i][0] = 0;
        else
            b[i][0] = 1;

        count = a[i - 1][x - 2] + a[i][x - 2] + a[i + 1][x - 2] + a[i - 1][x - 1] + a[i + 1][x - 1];
        if (a[i][x - 1] == 0 && (count == 2 || count == 3))
            b[i][x - 1] = 0;
        else if (a[i][x - 1] == 1 && count == 2)
            b[i][x - 1] = 0;
        else
            b[i][x - 1] = 1;
    }
    for (int i = 0; i < x - 1; ++i)
    {
        count = a[1][i - 1] + a[1][i] + a[1][i + 1] + a[0][i - 1] + a[0][i + 1];
        if (a[0][i] == 0 && (count == 2 || count == 3))
            b[0][i] = 0;
        else if (a[0][i] == 1 && count == 2)
            b[0][i] = 0;
        else
            b[0][i] = 1;

        count = a[y - 2][i - 1] + a[y - 2][i] + a[y - 2][i + 1] + a[y - 1][i - 1] + a[y - 1][i + 1];
        if (a[y - 1][i] == 0 && (count == 2 || count == 3))
            b[y - 1][i] = 0;
        else if (a[y - 1][i] == 1 && count == 2)
            b[y - 1][i] = 0;
        else
            b[y - 1][i] = 1;
    }
}

void saveBMP(unsigned int **a, char *bites, int start, int number, char *argv, int x, int y)
{
    int count = 1;
    int tmp = number;

    while (tmp > 9)
    {
        count++;
        tmp /= 10;
    }
    int siz = strlen(argv);
    char *fileNew = (char *)malloc(sizeof(char) * (count + siz - 5));

    for (int j = 0; j < siz - 9; ++j)
    {
        fileNew[j] = argv[j + 9];
    }
    for (int j = count + siz - 10; j >= siz - 9; --j)
    {
        fileNew[j] = (char)(number % 10) + '0';
        number /= 10;
    }
    fileNew[count + siz - 9] = '.';
    fileNew[count + siz - 8] = 'b';
    fileNew[count + siz - 7] = 'm';
    fileNew[count + siz - 6] = 'p';
    fileNew[count + siz - 5] = '\0';

    // printf("%d %s \n", count, fileNew);

    FILE *f = fopen(fileNew, "wb");
    for (int i = 0; i < start; ++i)
        fputc(bites[i], f);
    // fwrite(bites, 1, start, f);

    tmp = 0;
    char c;
    int x_oct = x / 8;
    if (x % 8 > 0)
        x_oct++;
    for (int i = y - 1; i >= 0; --i)
    {
        for (int j = 0; j < x_oct + (4 - x_oct % 4) % 4; ++j)
        {
            tmp = 0;
            for (int p = 0; p < 8; ++p)
            {
                if (j * 8 + p < x)
                {
                    tmp *= 2;
                    tmp += a[i][j * 8 + p];
                }
                else
                {
                    tmp *= 2;
                }
            }
            c = (char)tmp;
            fputc(c, f);
        }
    }
    fclose(f);
}

int main(int argc, char *argv[])
{
    int iter = 100000000;
    int freq = 1;
    if (argc > 3)
    {
        iter = 0;
        for (int i = 11; i < strlen(argv[3]); ++i)
        {
            iter *= 10;
            iter += (int)argv[3][i] - (int)'0';
        }
    }
    if (argc > 4)
    {
        freq = 0;
        for (int i = 12; i < strlen(argv[4]); ++i)
        {
            freq *= 10;
            freq += (int)argv[4][i] - (int)'0';
        }
    }

    FILE *fout = fopen("text.txt", "w");

    char *file = (char *)malloc(sizeof(char) * (strlen(argv[1]) - 8));

    for (int j = 0; j < strlen(argv[1]) - 9; ++j)
    {
        file[j] = argv[1][j + 8];
    }
    file[strlen(argv[1]) - 8] = '\0';

    FILE *f = fopen(file, "rb");

    char *bites = (char *)malloc(sizeof(char) * sizeOfFile(file));
    fread(bites, 1, sizeOfFile(file), f);

    int start = (int)bites[13] * 256 * 256 * 256 + (int)bites[12] * 256 * 256 + (int)bites[11] * 256 + (int)bites[10];
    int xsize = (int)bites[21] * 256 * 256 * 256 + (int)bites[20] * 256 * 256 + (int)bites[19] * 256 + (int)bites[18];
    int ysize = (int)bites[25] * 256 * 256 * 256 + (int)bites[24] * 256 * 256 + (int)bites[23] * 256 + (int)bites[22];

    unsigned int **gamePrev = (unsigned int **)malloc(sizeof(unsigned int *) * ysize);
    for (int i = 0; i < ysize; ++i)
    {
        gamePrev[i] = (unsigned int *)malloc(sizeof(unsigned int) * xsize);
    }

    unsigned int **gameNext = (unsigned int **)malloc(sizeof(unsigned int *) * ysize);
    for (int i = 0; i < ysize; ++i)
    {
        gameNext[i] = (unsigned int *)malloc(sizeof(unsigned int) * xsize);
    }

    pushOldGame(bites, gamePrev, start, xsize, ysize);

    newStep(gamePrev, gameNext, start, xsize, ysize);

    if (freq == 1)
        saveBMP(gameNext, bites, start, 0, argv[2], xsize, ysize);

    for (int i = 1; i <= iter; ++i)
    {
        for (int i = 0; i < ysize; ++i)
            for (int j = 0; j < xsize; ++j)
                gamePrev[i][j] = gameNext[i][j];

        newStep(gamePrev, gameNext, start, xsize, ysize);

        if (i % freq == 0)
            saveBMP(gameNext, bites, start, i / freq, argv[2], xsize, ysize);
    }
}