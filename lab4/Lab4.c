#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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

void cShow(char *file)
{
	FILE *f = fopen(file, "rb");
	char temp[10];
	char *bites = (char *)malloc(sizeof(char) * sizeOfFile(file));

	fread(temp, 1, 10, f);
	int meta_size = temp[6]*128*128*128+temp[7]*128*128+temp[8]*128+temp[9];
	fread(bites, 1, sizeOfFile(file) - 10, f);
	int i = 0;
	while (i<meta_size)
	{
		if(bites[i]==0){
			++i;
		}else{
			char name[5] = {bites[i], bites[i + 1], bites[i + 2], bites[i + 3], '\0'};
			printf("%s: ", name);

			i += 4;
			int size = bites[i] * 256 * 256 * 256 + bites[i + 1] * 256 * 256 + bites[i + 2] * 256 + bites[i + 3];

			i += 6;
			for (int j = 0; j < size; ++i, ++j)
				printf("%c", bites[i]);
			printf("\n");
		}
	}
	fclose(f);
	free(bites);
}

void cGet(char *file, char frame[5])
{
	FILE *f = fopen(file, "rb");
	char *bites = (char *)malloc(sizeof(char) * (sizeOfFile(file) + 1));
	fread(bites, 1, sizeOfFile(file), f);
	int meta_size = bites[6]*128*128*128+bites[7]*128*128+bites[8]*128+bites[9];

	int i = 10;
	while (i<meta_size)
	{
		int size = 0;
		if (bites[i] == frame[0] && bites[i + 1] == frame[1] && bites[i + 2] == frame[2] && bites[i + 3] == frame[3])
		{
			printf("%c%c%c%c: ", bites[i], bites[i + 1], bites[i + 2], bites[i + 3]);

			i += 4;
			size = bites[i] * 256 * 256 * 256 + bites[i + 1] * 256 * 256 + bites[i + 2] * 256 + bites[i + 3];

			i += 6;
			for (int j = 0; j < size; ++i, ++j)
				printf("%c", bites[i]);
			printf("\n");
			break;
		}
		else
		{
			i += 4;
			int size = bites[i] * 256 * 256 * 256 + bites[i + 1] * 256 * 256 + bites[i + 2] * 256 + bites[i + 3];
			i += size + 6;
		}
	}
	fclose(f);
	free(bites);
}

void cSet(char *file, char frame[5], char *value, int len)
{
	FILE *f = fopen(file, "rb");
	char *bites = (char *)malloc(sizeof(char) * sizeOfFile(file));
	fread(bites, 1, sizeOfFile(file), f);
	int meta_size = bites[6]*128*128*128+bites[7]*128*128+bites[8]*128+bites[9];
	int kl = 0;
	for (int i = 10; i < sizeOfFile(file) - 4 && i<1000; ++i)
	{
		if (bites[i] == frame[0] && bites[i + 1] == frame[1] && bites[i + 2] == frame[2] && bites[i + 3] == frame[3])
		{
			FILE *tmp = fopen("temp.mp3", "ab");

			int j = 6;
			meta_size += len - bites[i+7];
			bites[j] = meta_size / (128 * 128 * 128);
			bites[j + 1] = (meta_size / (128 * 128)) % 128;
			bites[j + 2] = (meta_size / 128) % 128;
			bites[j + 3] = meta_size % 128;
			fwrite(bites, 1, i + 10, tmp);

			i += 7;
			int size = bites[i];

			fwrite(value, 1, size, tmp);
			fseek(f, i + size + 3, SEEK_SET);

			char temp[1000] = {0};
			int cnt;

			while (!feof(f))
			{
				cnt = fread(&temp, 1, 1000 * sizeof(char), f);
				fwrite(&temp, 1, cnt * sizeof(char), tmp);
			}

			fclose(f);
			fclose(tmp);

			tmp = fopen("temp.mp3", "rb");
			f = fopen(file, "wb");
			fseek(tmp, 0, SEEK_SET);

			while (!feof(tmp))
			{
				cnt = fread(&temp, 1, 1000 * sizeof(char), tmp);
				fwrite(&temp, 1, cnt * sizeof(char), f);
			}

			fclose(tmp);
			remove("temp.mp3");
			kl = 1;
			break;
		}
	}
	if (!kl)
	{	
		int i = 6;
		meta_size+= len + 10;
		bites[i] = meta_size / (128 * 128 * 128);
		bites[i + 1] = (meta_size / (128 * 128)) % 128;
		bites[i + 2] = (meta_size / 128) % 128;
		bites[i + 3] = meta_size % 128;
		i = 10;
		while (bites[i] != 0)
		{
			i += 4;
			int size = bites[i] * 256 * 256 * 256 + bites[i + 1] * 256 * 256 + bites[i + 2] * 256 + bites[i + 3];
			i += size + 6;
		}
		FILE *tmp = fopen("temp.mp3", "ab");

		bites[i] = frame[0];
		bites[i + 1] = frame[1];
		bites[i + 2] = frame[2];
		bites[i + 3] = frame[3];

		i += 4;
		bites[i] = len / (256 * 256 * 256);
		bites[i + 1] = (len / (256 * 256)) % 256;
		bites[i + 2] = (len / 256) % 256;
		bites[i + 3] = len % 256;
		i += 6;
		fwrite(bites, 1, i, tmp);
		int size = len;

		fwrite(value, 1, size, tmp);
		fseek(f, i + size + 3, SEEK_SET);

		char temp[1000] = {0};
		int cnt;

		while (!feof(f))
		{
			cnt = fread(&temp, 1, 1000 * sizeof(char), f);
			fwrite(&temp, 1, cnt * sizeof(char), tmp);
		}

		fclose(f);
		fclose(tmp);

		tmp = fopen("temp.mp3", "rb");
		f = fopen(file, "wb");
		fseek(tmp, 0, SEEK_SET);

		while (!feof(tmp))
		{
			cnt = fread(&temp, 1, 1000 * sizeof(char), tmp);
			fwrite(&temp, 1, cnt * sizeof(char), f);
		}

		fclose(tmp);
		remove("temp.mp3");
	}
	free(bites);
}

int main(int argc, char *argv[])
{
	int key = 0;
	char frame[5];
	char *value = (char *)malloc(sizeof(char) * 60);
	for (int i = 0; i < 60; ++i)
		value[i] = 0;

	char *file = (char *)malloc(sizeof(char) * 9);

	for (int j = 0; j < 8; ++j)
		file[j] = argv[1][j + 11];
	file[8] = '\0';

	if (!strcmp("--show", argv[2]))
		key = 1;
	else
	{
		for (int j = 0; j < 4; ++j)
			frame[j] = argv[2][j + 6];
		frame[4] = '\0';
		if (argv[2][2] == 'g')
			key = 2;
		else
		{
			key = 3;
			int c = 8;
			for (int j = 0; j < strlen(argv[3]); ++j, ++c)
			{
				if (argv[3][c] >= 'a' && argv[3][c] <= 'z' || argv[3][c] >= 'A' && argv[3][c] <= 'Z' || argv[3][c] >= '0' && argv[3][c] <= '9' || argv[3][c] == ' ' || argv[3][c] == '.' ||
					argv[3][c] == '-' || argv[3][c] == '_' || argv[3][c] == '/')
					value[j] = argv[3][c];
				else
					break;
			}
			value[c] = '\0';
		}
	}

	/*FILE *f = fopen(file, "rb");
	char *bites = (char *)malloc(sizeof(char) * sizeOfFile(file));
	fread(bites, 1, sizeOfFile(file), f);
	for(int i=205; i<416; ++i)
		bites[i]=0;
	FILE *tmp = fopen("temp.mp3", "ab");
	fwrite(bites, 1, 416, tmp);
	fseek(f, 416, SEEK_SET);
	char temp[1000] = {0};
	int cnt;
	while (!feof(f))
	{
		cnt = fread(&temp, 1, 1000 * sizeof(char), f);
		fwrite(&temp, 1, cnt * sizeof(char), tmp);
	}
	fclose(f);
	fclose(tmp);
	tmp = fopen("temp.mp3", "rb");
	f = fopen(file, "wb");
	fseek(tmp, 0, SEEK_SET);
	while (!feof(tmp))
	{
		cnt = fread(&temp, 1, 1000 * sizeof(char), tmp);
		fwrite(&temp, 1, cnt * sizeof(char), f);
	}
	fclose(tmp);
	remove("temp.mp3");*/

	if (key == 1)
		cShow(file);
	if (key == 2)
		cGet(file, frame);
	if (key == 3)
		cSet(file, frame, value, strlen(argv[3]) - 8);

	return 0;
}