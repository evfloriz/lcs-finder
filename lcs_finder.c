/*
* Assignment 1
*
* Due 2019-09-29
*
* Cover Sheet Info
* ------------------------------------------------------------------------------
* Submitting student: Evan Florizone
* Collaborating classmates: N/A
* Other collaborators: N/A
* References (excluding textbook and lecture slides):
*   eClass Discussion Forums
*   https://stackoverflow.com/questions/19794268/scanf-reading-enter-key
*   https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
*   https://stackoverflow.com/questions/8611815/determine-if-char-is-a-num-or-letter
*   https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
*   https://stackoverflow.com/questions/5020832/array-size-in-fgets
*   https://eclass.srv.ualberta.ca/pluginfile.php/5339007/mod_resource/content/1/lcs.txt
*   https://www.youtube.com/watch?v=sSno9rV8Rhg
*   https://www.youtube.com/watch?v=NnD96abizww
*   https://ubuntuforums.org/showthread.php?t=1016188
*   https://stackoverflow.com/questions/2524611/how-can-one-print-a-size-t-variable-portably-using-the-printf-family
*   https://linux.die.net/man/3/malloc
*   https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
*   https://www.geeksforgeeks.org/longest-common-subsequence-dp-using-memoization/
*
*
* Specifications:
* 1. Read two input sequences from the terminal
* 2. Check the sequence for non-digit characters
* 3. Print the input sequences
* 4. Compute and print LCS
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_LENGTH 1010

// check sequence to make sure it's all numbers
// return 1 if sequence is valid and 0 if invalid
int sequenceCheck(char *sequence)
{
    int success = 1;
    int i=0;

    // run until \n or \0 is found
    while (1)
    {
        // check for newline char ('\n') and null char ('\0')
        if (sequence[i] == '\n' || sequence[i] == '\0')
        {
            // make sure the final character is a null and break the loop
            sequence[i] = '\0';
            break;
        }

        // if the ascii code is outside of the range of the codes for 0-9
        if (sequence[i] < '0' || sequence[i] > '9')
            success = 0;

        i++;
    }
    return success;
}


// takes the constructed table and deconstructs it into a single LCS string
// going from the bottom up
// LCS will be in reverse order
void deconstructLCS(char *s1, int lenS1, int lenS2, int *lcsTable, int tableSize, char *lcs)
{

    int test1 = *(lcsTable + tableSize*(lenS1-1) + lenS2);
    int test2 = *(lcsTable + tableSize*lenS1 + (lenS2-1));
    int current = *(lcsTable + tableSize*lenS1 + lenS2);
    int lcsLength = strlen(lcs);

    // end current layer of recursion when either horizontal or vertical position
    // in table hits 0
    if (lenS1==0 || lenS2==0)
        return;

    if (current > test1 && current > test2)
    {
        lcs[lcsLength] = s1[lenS1-1];
        lcs[lcsLength+1] = '\0';
        deconstructLCS(s1, lenS1-1, lenS2-1, lcsTable, tableSize, lcs);
    }
    else if (test1 > test2)
    {
        deconstructLCS(s1, lenS1-1, lenS2, lcsTable, tableSize, lcs);
    }
    else // test2>test1
    {
        deconstructLCS(s1, lenS1, lenS2-1, lcsTable, tableSize, lcs);
    }

}


// reverses the lcs so that it is accurate
void reverseLCS(char *lcs)
{
    int len = strlen(lcs);
    int halfLen = len / 2;
    char temp = '\0';

    for (int i=0;i<halfLen;i++)
    {
        temp = lcs[i];
        lcs[i] = lcs[len-1-i];
        lcs[len-1-i] = temp;
    }
}


// contructs a multidimensional array that represents the table
// values filled in recursively
// returns length of LCS
int findLCS(char *s1, char *s2, int lenS1, int lenS2, int *lcsTable, int tableSize)
{
    // printf("lenS1: %d lenS2: %d\n", lenS1, lenS2);

    int test1 = 0;
    int test2 = 0;

    int lastPos1 = lenS1-1;
    int lastPos2 = lenS2-1;

    if (lenS1 <= 0 || lenS2 <= 0)
    {
        return 0;
    }

    // memoization to speed up computuation
    if (*(lcsTable + tableSize*(lastPos1+1) + (lastPos2+1)) != 0)
        return *(lcsTable + tableSize*(lastPos1+1) + (lastPos2+1));

    // if the last char is the same, add one to the diagonal value on the table and make it the current value
    if (s1[lastPos1] == s2[lastPos2])
    {
        *(lcsTable + tableSize*(lastPos1+1) + (lastPos2+1)) = findLCS(s1, s2, lastPos1, lastPos2, lcsTable, tableSize) + 1;
        return *(lcsTable + tableSize*(lastPos1+1) + (lastPos2+1));
    }

    // else test the adjacent values in the table and make the max the current value
    else
    {
        test1 = findLCS(s1, s2, lastPos1, lenS2, lcsTable, tableSize);
        test2 = findLCS(s1, s2, lenS1, lastPos2, lcsTable, tableSize);
        if (test1>test2)
        {
            *(lcsTable + tableSize*(lastPos1+1) + (lastPos2+1)) = test1;
            return test1;
        }
        else
        {
            *(lcsTable + tableSize*(lastPos1+1) + (lastPos2+1)) = test2;
            return test2;
        }
    }
}

int computeLCS(char *s1, char *s2, int lenS1, int lenS2, char *lcs)
{
    // general compute function
    // returns the length of the lcs
    // char *lcs contains the true lcs

    int *lcsTable = (int *)calloc((lenS1+1) * (lenS2+1) + 1, sizeof(int));
    int tableSize = (lenS1 > lenS2) ? lenS1 + 1 : lenS2 + 1;


    // compute LCS length (lcs table is constructed at the same time)
    int lcsLength = findLCS(s1, s2, lenS1, lenS2, lcsTable, tableSize);

    // convert LCS table into a proper sequence
    deconstructLCS(s1, lenS1, lenS2, lcsTable, tableSize, lcs);
    reverseLCS(lcs);

    // free table pointer
    free(lcsTable);

    return lcsLength;

}


int main(int argc, char ** argv)
{
    int lenS1 = 0;
    int lenS2 = 0;

    char *sequence1 = (char *)malloc(MAX_LENGTH * sizeof(char) + 1);
    char *sequence2 = (char *)malloc(MAX_LENGTH * sizeof(char) + 1);

    // read two input sequences from the terminal
    printf("To compute an LCS,\nenter the first sequence: ");
    fgets(sequence1, MAX_LENGTH, stdin);
    printf("enter the second sequence: ");
    fgets(sequence2, MAX_LENGTH, stdin);

    while (!sequenceCheck(sequence1) || !sequenceCheck(sequence2))
    {
        printf("\nError, non-digit character detected!\n\n");

        // read two input sequences from the terminal
        printf("To compute an LCS,\nenter the first sequence: ");
        fgets(sequence1, MAX_LENGTH, stdin);
        printf("enter the second sequence: ");
        fgets(sequence2, MAX_LENGTH, stdin);
    }

    lenS1 = strlen(sequence1);
    lenS2 = strlen(sequence2);

    // realloc sequences based on the real size
    sequence1   = (char *)realloc(sequence1, (lenS1+2) * sizeof(char));         // for \0 at the end and to accomodate chars being shifted up
    sequence2   = (char *)realloc(sequence2, (lenS2+2) * sizeof(char));


    // calculate and print the LCS
    char *lcs = (char *)malloc(((lenS1<lenS2) ? (lenS1+1) : (lenS2+1)) * sizeof(char) + 1);            // wont exceed the length of the smallest
    lcs[0] = '\0';
    int lcsLength = 0;


    lcsLength = computeLCS(sequence1, sequence2, lenS1, lenS2, lcs);
    
    printf("# an LCS (length = %d) for two sequences is:\n%s\n", lcsLength, lcs);

    free(sequence1);
    free(sequence2);
    free(lcs);

}
