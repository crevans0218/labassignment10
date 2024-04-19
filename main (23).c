#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};

// Trie structure
struct Trie {
    struct TrieNode* root;
};

// Create a new TrieNode
struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Initialize a Trie
struct Trie* createTrie() {
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    if (trie) {
        trie->root = createTrieNode();
    }
    return trie;
}

// Inserts a word into the Trie
void insert(struct Trie* trie, char* word) {
    struct TrieNode* curr = trie->root;
    while (*word != '\0') {
        int index = *word - 'a';
        if (!curr->children[index]) {
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
        word++;
    }
    curr->count++;
}

// Counts the occurrences of a word in the Trie
int numberOfOccurrences(struct Trie* trie, char* word) {
    struct TrieNode* curr = trie->root;
    while (*word != '\0' && curr != NULL) {
        int index = *word - 'a';
        curr = curr->children[index];
        word++;
    }
    if (curr != NULL && curr->count > 0) {
        return curr->count;
    }
    return 0;
}

// Deallocates memory used by the Trie
void deallocateTrie(struct Trie* trie) {
    if (trie) {
        deallocateTrieNode(trie->root);
        free(trie);
    }
}

// Helper function to deallocate TrieNode recursively
void deallocateTrieNode(struct TrieNode* node) {
    if (node) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                deallocateTrieNode(node->children[i]);
            }
        }
        free(node);
    }
}

// Reads dictionary file and stores words in pInWords array
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numWords = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        pInWords[numWords] = strdup(buffer); // Store the word
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    // Create and populate the Trie
    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // Test with predefined words
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the Trie
    deallocateTrie(pTrie);
    
    // Ensure trie is deallocated properly
    if (pTrie != NULL) {
        printf("There is an error in this program\n");
    }

    return 0;
}