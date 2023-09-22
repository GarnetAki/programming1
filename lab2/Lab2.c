#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define ull unsigned long long
#define uli unsigned int

struct largeInt
{
    uli num[32];
};

struct largeIntDec
{
    ull num[35];
};

typedef struct largeInt uint1024_t;
typedef struct largeIntDec LargeDec;

int CompDec(LargeDec x, LargeDec y){
    int flag = 1;
    for(int i=34; i>=0; --i){
        if(x.num[i]>y.num[i])break;
        if(x.num[i]==y.num[i])continue;
        flag=0;
        break;
    }
    return flag;
}

LargeDec SubDec(LargeDec x, LargeDec y){
    LargeDec p;
    int fl=0;
    for(int i=34; i>=0; --i){
        if(x.num[i]>=y.num[i])p.num[i]=x.num[i]-y.num[i];
        else{
            int fl_ind = i+1;
            while(fl_ind<35 && p.num[fl_ind]==0){
                fl_ind++;
            }
            if(fl_ind==35){
                printf("ERROR");
                exit(0);
            }
            p.num[fl_ind]-=1;
            for(int j=fl_ind-1; j>i; --j)p.num[j]=999999999;
            p.num[i]=((ull)(1000000000)+x.num[i]-y.num[i]);
        }
    }
    return p;
}

LargeDec SumDec(LargeDec x, LargeDec y){
    LargeDec p;
    ull fl = 0;
    for(int i=0; i<35; ++i){
        p.num[i]=(x.num[i]+y.num[i]+fl)%1000000000;
        fl = (x.num[i]+y.num[i]+fl)/1000000000;
    }
    return p;
}

LargeDec MulDec(LargeDec x, LargeDec y){
    LargeDec p;
    for(int i=0;i<35;++i)p.num[i]=0;
    int size_x, size_y;
    for(int i=34; i>=0; --i){
        if(x.num[i]!=0){
            size_x=i+1;
            break;
        }
    }
    for(int i=34; i>=0; --i){
        if(y.num[i]!=0){
            size_y=i+1;
            break;
        }
    }
    for (int ix = 0; ix < size_x; ++ix)
        for (int iy = 0; iy < size_y; ++iy)
            p.num[ix + iy] += x.num[ix] * y.num[iy];
    
    for (int ii = 0; ii < 34; ++ii)
    {
        p.num[ii + 1] +=  p.num[ii] / 1000000000;
        p.num[ii] %= 1000000000;
    }
    return p;
}


LargeDec BinToDec(struct largeInt p){
    LargeDec q;
    LargeDec PowTwo[1024];
    q.num[0]=0;
    PowTwo[0].num[0]=1;
    PowTwo[1].num[0]=2;

    for(int i=1; i<35; ++i){
        PowTwo[0].num[i]=0;
        PowTwo[1].num[i]=0;
        q.num[i]=0;
    }

    for(int i=2; i<1024; ++i){
        PowTwo[i] = MulDec(PowTwo[i-1], PowTwo[1]);
    }
    for(int i=0; i<1024; ++i){
        if((p.num[i/32]&(1<<(i%32)))>>(i%32)==1){
            q=SumDec(q, PowTwo[i]);
        }
    }
    return q;
}

uint1024_t DecToBin(struct largeIntDec p)
{
    uint1024_t q;
    LargeDec PowTwo[1024];

    q.num[0]=0;
    PowTwo[0].num[0]=1;
    PowTwo[1].num[0]=2;
    for(int i=1; i<32; ++i){
        PowTwo[0].num[i]=0;
        PowTwo[1].num[i]=0;
        q.num[i]=0;
    }
    for(int i=32; i<35; ++i){
        PowTwo[0].num[i]=0;
        PowTwo[1].num[i]=0;
    }

    for(int i=2; i<1024; ++i){
        PowTwo[i] = MulDec(PowTwo[i-1], PowTwo[1]);
    }

    for(int i = 1023; i>=0; --i){
        if(CompDec(p, PowTwo[i])){
            q.num[i/32]+=(uli)pow(2, i%32);
            p = SubDec(p, PowTwo[i]);       
        }
    }

    return q;
}

void printf_value(struct largeInt p)
{
    LargeDec q = BinToDec(p);
    int fl = 0;
    for(int i = 34; i>=0; --i){
        if(fl==0 && q.num[i]==0);
        else{
            if(q.num[i]==0){
                printf("000000000");
            }else{
                int temp = 100000000;
                int count = 0;
                while(q.num[i]/temp==0){
                    count++;
                    temp/=10;
                }
                if(fl){
                    for(int j=0; j<count; ++j){
                        printf("0");
                    }
                }
                fl=1;
                printf("%d", q.num[i]);
            }         
        }
    }
    if(fl==0)printf("0");
}

void scanf_value(struct largeInt *p)
{
    LargeDec q;
    char str[311];
    for(int i=0; i<311; ++i)str[i]='a';
    for(int i=0; i<35; ++i)q.num[i]=0;

    scanf("%s", str);
    if(str[310]!='a'){
        printf("ERROR");
        exit(0);
    }

    int count=0;

    while((int)str[count]!=0)count++;
    count--;
    ull ind=0, dec=1;
    while(count>=0){
        uli c = ((uli)str[count])-48;
        q.num[ind]+=dec*c;
        dec*=10;
        if(dec>100000000){
            dec=1;
            ind++;
        }
        count--;
    }
    uint1024_t help = DecToBin(q);
    for(int i=0; i<32; ++i)p->num[i] = help.num[i];
}


uint1024_t from_uint(unsigned int x)
{
    uint1024_t p;
    for(int i=1; i<32; ++i)p.num[i]=0;
    p.num[0]=x;
    return p;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t p;
    int fl=0;
    for(int i=0; i<32; ++i){
        p.num[i]=(uli)((ull)x.num[i]+(ull)y.num[i]+(ull)fl);
        if(((ull)x.num[i]+(ull)y.num[i]+(ull)fl)>p.num[i])fl=1;
        else fl=0;
    }
    if (fl==1){
        printf("ERROR");
        exit(0);
    }
    return p;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t p;
    int fl=0;
    for(int i=31; i>=0; --i){
        if(x.num[i]>=y.num[i])p.num[i]=x.num[i]-y.num[i];
        else{
            int fl_ind = i+1;
            while(fl_ind<32 && p.num[fl_ind]==0){
                fl_ind++;
            }
            if(fl_ind==32){
                printf("ERROR");
                exit(0);
            }
            p.num[fl_ind]--;
            for(int j=fl_ind-1; j>i; --j)p.num[j]=(pow(2,32) - 1);
            p.num[i]=(uli)((ull)(pow(2,32))-(ull)y.num[i]+(ull)x.num[i]);
        }
    }
    return p;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    LargeDec p = BinToDec(x);
    LargeDec q = BinToDec(y);
    LargeDec l = MulDec(p, q);
    uint1024_t t = DecToBin(l);
    return t;
}

int main(int argc, char *argv[])
{
    printf("Input two large int(first>=second) and one small int \n");
    uint1024_t x,y;
    uli z;
    scanf_value(&x);
    scanf_value(&y);
    scanf("%u", &z);
    printf("Sum: ");
    printf_value(add_op(x,y));
    printf("\n");
    printf("Sub: ");
    printf_value(subtr_op(x,y));
    printf("\n");
    printf("Mul: ");
    printf_value(mult_op(x,y));
    printf("\n");
    x = from_uint(z);
    printf("From uint: ");
    printf_value(x);

    return 0;
}