#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *archive;
    int fixArgc = argc - 4;
    for (int i = 0; i < argc; ++i) {
        if (!strcmp("--file", argv[i]))
            archive = argv[i + 1];
        else if (!strcmp("--create", argv[i])) {
            FILE *arc = fopen(archive, "wb");
            FILE *tmp;
            unsigned long long length[128];

            for (int j = 4; j < argc; ++j) {
                tmp = fopen(argv[j], "rb");
                if (tmp == NULL)
                    continue;
                fseek(tmp, 0, SEEK_END);
                length[j - 4] = ftell(tmp);
                fseek(tmp, 0, SEEK_SET);
                fclose(tmp);
            }
            for (int j = 4; j < argc; ++j) {
                fprintf(arc, "^ %s %llu ^", argv[j], length[j - 4]);
            }
            fprintf(arc, "\n");
            for (int j = 4; j < argc; ++j) {
                tmp = fopen(argv[j], "rb");
                if (tmp == NULL)
                    continue;
                int c = fgetc(tmp);
                while (!feof(tmp))
                {
                    fputc(c, arc);
                    c = fgetc(tmp);
                }
                fclose(tmp);
            }
            
        }
        else if (!strcmp("--extract", argv[i])){    
            FILE *arc = fopen(archive, "rb+");
            unsigned long long now = 0;
            unsigned long long start = 0;
            int c;

            while((c=getc(arc))!='\n')
                start++;
            start++;
            fseek(arc, 0, SEEK_SET);

            char tmp[128];
            unsigned long long length;

            FILE *out;
            while(fscanf(arc, "^ %s %llu ^", tmp, &length)!=0){
                out = fopen(tmp, "wb");
                if (out != NULL){
                    now = ftell(arc);
                    fseek(arc, start, SEEK_SET);
                    start += length;
                    while (length-- > 0)
                        putc((c = getc(arc)), out);
                    fseek(arc, now, SEEK_SET);
                    fclose(out);
                }
            }
        }
        else if (!strcmp("--list", argv[i])){
            FILE *arc = fopen(archive, "rb+");
            unsigned long long length;
            char tmp[128];
            while (fscanf(arc, "^ %s %llu ^", tmp, &length) != 0)
                printf("%s\n", tmp);
            fclose(arc);
        }
    }
}
