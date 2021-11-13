from hashlib import sha256

def SHA256(text):
    return sha256(text.encode("ascii")).hexdigest()

def mine(block_number, transactions, previous_hash, prefix_zeros):
    nonce = 1
    print('block number : ', block_number)
    print('transactions : ', transactions)
    print('previous_hash : ' ,previous_hash)
    print('nonce : ' ,nonce)
    text = str(block_number) + transactions + previous_hash + str(nonce)
    print(text)
    new_hash = SHA256(text)
    return new_hash

if __name__ == '__main__':
    transactions = '''
    Dhaval->Bhavin->20,
    Mando->Cara->45->
    '''
    difficulty = 4
    new_hash = mine(5, transactions, '0000000xa036944e29568d0cff17edbe038f81208fecf9a66be9a2b8321c6ec7', difficulty)

    print(new_hash)