simple_blockchain
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

#define DIFFICULTY 2

typedef struct Block {
    int index;
    time_t timestamp;
    char data[256];
    int nonce;
    unsigned char previous_hash[SHA256_DIGEST_LENGTH];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    struct Block *next;
} Block;

typedef struct Blockchain {
    Block *head;
} Blockchain;

// Function to calculate SHA-256 hash of a block
void calculate_hash(Block *block, unsigned char *hash) {
    char input[1024];
    snprintf(input, sizeof(input), "%d%ld%s%d%s",
             block->index, block->timestamp, block->data,
             block->nonce, block->previous_hash);
    
    SHA256((unsigned char*)input, strlen(input), hash);
}

// Function to create a new block
Block* create_block(int index, const char *data, unsigned char *previous_hash) {
    Block *block = (Block*)malloc(sizeof(Block));
    block->index = index;
    block->timestamp = time(NULL);
    strncpy(block->data, data, sizeof(block->data) - 1);
    block->nonce = 0;
    memcpy(block->previous_hash, previous_hash, SHA256_DIGEST_LENGTH);
    calculate_hash(block, block->hash);
    block->next = NULL;
    return block;
}

// Proof of work algorithm
void proof_of_work(Block *block) {
    char hash_string[SHA256_DIGEST_LENGTH*2+1];
    do {
        block->nonce++;
        calculate_hash(block, block->hash);
        
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(&hash_string[i * 2], "%02x", block->hash[i]);
        }
    } while (strncmp(hash_string, "00", DIFFICULTY) != 0);
}

// Function to initialize the blockchain with a genesis block
Blockchain* create_blockchain() {
    Blockchain *blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    blockchain->head = create_block(0, "Genesis Block", (unsigned char*) "");
    proof_of_work(blockchain->head);
    return blockchain;
}

// Function to add a block to the blockchain
void add_block(Blockchain *blockchain, const char *data) {
    Block *last_block = blockchain->head;
    while (last_block->next) {
        last_block = last_block->next;
    }
    
    Block *new_block = create_block(last_block->index + 1, data, last_block->hash);
    proof_of_work(new_block);
    last_block->next = new_block;
}

// Function to verify the blockchain
int is_chain_valid(Blockchain *blockchain) {
    Block *current = blockchain->head;
    
    while (current->next) {
        Block *next = current->next;
        
        unsigned char hash[SHA256_DIGEST_LENGTH];
        calculate_hash(current, hash);
        
        if (memcmp(current->hash, hash, SHA256_DIGEST_LENGTH) != 0) {
            return 0;
        }
        
        if (memcmp(current->hash, next->previous_hash, SHA256_DIGEST_LENGTH) != 0) {
            return 0;
        }
        
        current = next;
    }
    return 1;
}

// Function to print a block
void print_block(Block *block) {
    printf("Block %d\n", block->index);
    printf("Timestamp: %ld\n", block->timestamp);
    printf("Data: %s\n", block->data);
    printf("Nonce: %d\n", block->nonce);
    
    printf("Hash: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", block->hash[i]);
    }
    printf("\n");
    
    printf("Previous Hash: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", block->previous_hash[i]);
    }
    printf("\n\n");
}

// Function to print the blockchain
void print_blockchain(Blockchain *blockchain) {
    Block *current = blockchain->head;
    while (current) {
        print_block(current);
        current = current->next;
    }
}

int main() {
    Blockchain *blockchain = create_blockchain();

    // Adding blocks to the blockchain
    add_block(blockchain, "First block data");
    add_block(blockchain, "Second block data");
    add_block(blockchain, "Third block data");

    // Printing the blockchain
    print_blockchain(blockchain);

    // Validating the blockchain
    printf("Blockchain valid: %s\n", is_chain_valid(blockchain) ? "Yes" : "No");

    // Freeing memory (not fully implemented in this example)
    // TODO: Implement full cleanup of allocated memory

    return 0;
}