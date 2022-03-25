#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int sum;
int chk26( char s[]);
int chkalpha( char s[]);
int chkrepeat(char s[]);
void encipher(char c[]);
int main(int argc, string argv[])
{
    char copied[26];
    strcpy(copied, argv[1]);
    if(argc == 2)
    {
        sum = chk26(copied);
        if(sum!=0)
        {
            printf("Key must contain 26 characters.");
            return 1;
        }
        else
        {
            sum = chkalpha(copied);
            if(sum!=0)
            {
                printf("Key must only contain alphabetic characters.");
                return 1;
            }
            else
            {
               sum = chkrepeat(copied);
               if(sum!=0)
               {
                   printf("Key must not contain repeated characters.");
                   return 1;
               }
               else
               {
                   encipher(copied);
               }
            }
        }
    }
    else
    return 1;
}

int chk26( char s[])
{
    if(strlen(s)==26)
    sum=0;
    else
    sum=1;
    
    return sum;
}

int chkalpha( char s[])
{
    for(int i=0;i<strlen(s); i++)
    {
        if( (s[i]>='a' && s[i]<='z')||(s[i]>='A' && s[i]<='Z') )
        sum+=0;
        
        else
        sum+=1;
    }
    return sum;
}

int chkrepeat(char s[])
{
    for(int c=0;c<26;c++)
    {
        for(int d=(c+1);d<26;d++)
        {
            if(s[c]==s[d])
            sum+=1;
            else
            sum+=0;
        }
    }
    return sum;
}

void encipher(char c[])
{
    string plntxt= get_string("plaintext: ");
    int l= strlen(plntxt), b;
    char text[l], final[l], a;
    strcpy(text , plntxt);
    for(int i=0;i< l; i++)
    {
        a = text[i];

        if(text[i]==' ')
        {
            final[i]=' ';
            //printf("%c ",final[i]);
        }
        else if(islower(text[i]))
        {
            b=a-97;
            final[i]=(tolower(c[b]));
            //printf("%C ", final[i]);
        }
        else if(isupper(text[i]))
        {
            b=a-65;
            final[i]=(toupper(c[b]));
            //printf("%c ",final[i]);
        }
        else if(text[i])
        {
            final[i]=text[i];
            //printf("%c", final[i]);
        }
        final[l] = '\0';
    }
    printf("ciphertext: %s\n", final);
}