import hashlib
import time

class Block:
    def __init__(self, index, previous_hash, timestamp, data, nonce=0):
        self.index = index
        self.previous_hash = previous_hash
        self.timestamp = timestamp
        self.data = data
        self.nonce = nonce
        self.hash = self.calculate_hash()

    def calculate_hash(self):
        block_string = f"{self.index}{self.previous_hash}{self.timestamp}{self.data}{self.nonce}"
        return hashlib.sha256(block_string.encode()).hexdigest()

    def __repr__(self):
        return f"Block(Index: {self.index}, Hash: {self.hash})"

class Blockchain:
    def __init__(self, difficulty=2):
        self.chain = [self.create_genesis_block()]
        self.difficulty = difficulty

    def create_genesis_block(self):
        return Block(0, "0", time.time(), "Genesis Block")

    def get_latest_block(self):
        return self.chain[-1]

    def add_block(self, new_block):
        new_block.previous_hash = self.get_latest_block().hash
        new_block.hash = self.proof_of_work(new_block)
        self.chain.append(new_block)

    def proof_of_work(self, block):
        block.nonce = 0
        while not block.hash.startswith("0" * self.difficulty):
            block.nonce += 1
            block.hash = block.calculate_hash()
        return block.hash

    def is_chain_valid(self):
        for i in range(1, len(self.chain)):
            current_block = self.chain[i]
            previous_block = self.chain[i - 1]

            if current_block.hash != current_block.calculate_hash():
                print(f"Block {i} has been tampered!")
                return False

            if current_block.previous_hash != previous_block.hash:
                print(f"Block {i} previous hash doesn't match!")
                return False

        return True

# Create a blockchain and add some blocks
my_blockchain = Blockchain()

# Add blocks to the blockchain
my_blockchain.add_block(Block(1, "", time.time(), "First block data"))
my_blockchain.add_block(Block(2, "", time.time(), "Second block data"))
my_blockchain.add_block(Block(3, "", time.time(), "Third block data"))

# Print the blockchain
for block in my_blockchain.chain:
    print(block)

# Validate the blockchain
print("Blockchain valid:", my_blockchain.is_chain_valid())
