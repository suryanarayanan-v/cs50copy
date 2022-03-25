#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int countLetters(string text);
int countWords(string text);
int countSentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    int letters = countLetters(text);
    int words = countWords(text);
    int sentences = countSentences(text);

    float L = ((float)letters / words) * 100;
    float S = ((float)sentences / words) * 100;
    
    float index = (0.0588 * L) - (0.296 * S) - 15.8;

    //printf("Letter (s): %i\n", letters);
    //printf("Word (s): %i\n", words);
    //printf("Sentences (s): %i\n", sentences);
    int intindex = round(index);
    
    if (intindex < 1)
    {
        printf("Before Grade 1\n");
    } 
    else if (intindex > 16) 
    {
        printf("Grade 16+\n");
    } 
    else 
    {
        printf("Grade %i\n", intindex);
    }
}

int countLetters(string text) 
{
    int letters = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i])) 
        {
            letters++;
        }
    }
    
    return letters;
}

int countWords(string text) 
{
    int words = 1;
    for (int i = 0; text[i] != '\0'; i++) 
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }
    
    return words;
    
}

int countSentences(string text)
{
    int sentences = 0;
    for (int i = 0; text[i] != '\0'; i++) 
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') 
        {
        
            sentences++;
        }
    }
    return sentences;
}


