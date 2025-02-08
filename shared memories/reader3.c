#include "D:\network-core\shared memories\header3.h"

#define MAX_TEXT 1024

// Function to count vowels, consonants, spaces, and sentences
void count_characters_and_sentences(char *text, int *vowels, int *consonants, int *spaces, int *sentences)
{
    *vowels = 0;
    *consonants = 0;
    *spaces = 0;
    *sentences = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];

        // Count vowels (both uppercase and lowercase)
        if (ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U' ||
            ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            (*vowels)++;
        }
        // Count consonants (letters other than vowels)
        else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            (*consonants)++;
        }
        // Count spaces
        else if (ch == ' ') {
            (*spaces)++;
        }
        // Count sentences (using '.', '!', or '?')
        else if (ch == '.' || ch == '!' || ch == '?') {
            (*sentences)++;
        }
    }
}

int main()
{
    void *shared_memory;
    char *text;
    int shmid;

    // Retrieve the shared memory segment with key 1122
    shmid = shmget((key_t)1122, MAX_TEXT, 0666);
    if (shmid == -1)
    {
        perror("Error in shmget");
        return 1;
    }

    printf("Key of the shared memory is - %d\n", shmid);

    // Attach to the shared memory segment
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *) -1)
    {
        perror("Error in shmat");
        return 2;
    }

    printf("Process attached at memory address - %p\n", shared_memory);

    // Read the paragraph from shared memory
    text = (char *)shared_memory;

    printf("Data read from shared memory: \n%s\n", text);

    // Variables to store the counts
    int vowels, consonants, spaces, sentences;

    // Call the function to count characters and sentences
    count_characters_and_sentences(text, &vowels, &consonants, &spaces, &sentences);

    // Display the counts
    printf("Total Vowels: %d\n", vowels);
    printf("Total Consonants: %d\n", consonants);
    printf("Total Spaces: %d\n", spaces);
    printf("Total Sentences: %d\n", sentences);

    return 0;
}

