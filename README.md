# RSA_encryption-porject

## Overview :

This project implements an RSA encryption tool in C++ that supports 128-bit encryption and 64-bit encryption for securing text and image files. It uses:

* Fast Modular Exponentiation for efficient calculations

* Miller-Rabin Primality Test for prime number generation

* Extended Euclidean Algorithm for computing modular inverses

## Features :

* RSA Key Generation: Generates public and private keys.

* Text Encryption & Decryption: Encrypts and decrypts text messages.

* Image Encryption & Decryption: Encrypts and decrypts image files.

* 128-bit & 64-bit Security: Supports high-security key sizes.

## Installation :

### Prerequisites :

  * C++ Compiler (G++ or Clang)

  * C++17 or higher

### Compilation

   ```  g++ -o rsa_tool rsa_tool.cpp -std=c++17 ```
  
### Running the Program

   ```  ./rsa_tool ```

## How It Works :

### Key Generation

  1. Two large prime numbers (p, q) are generated using the Miller-Rabin test.

  2. Compute n = p * q and φ(n) = (p-1) * (q-1).

  3. Select e = 65537 as the public exponent.

  4. Compute the private key d as the modular inverse of e modulo φ(n).

### Encryption 

  * Each character m in the message is encrypted using:

  ``` c = (m^e) mod n```

### Decryption

* Each encrypted value c is decrypted using:

``` m = (c^d) mod n ```

## Image Encryption & Decryption :

  * Each byte of the image file is encrypted and decrypted similarly to text.

## Example Usage :

``` you want security higer = 1 or default = 0 : 1 ```

``` Enter a message to encrypt: hello world ```

``` Original Message: hello world ```

``` Encrypted Message: [234923849238, 923842984928, ...] ```

``` Decrypted Message: hello world  ```

## Potential Issues & Fixes :

### Incorrect Decryption (Random Symbols)

  * Ensure mod_exp() is correctly handling large numbers.

  * Validate n > 256 for proper ASCII character encryption.

### Image Corruption After Decryption :

  * Check file read/write operations.

  * Ensure proper handling of binary data.

## Future Improvements :

  * GUI Integration using Qt

  * Support for Larger Key Sizes (e.g., 256-bit, 512-bit)

  * Hybrid Encryption (RSA + AES for performance)

# License :

This project is released under the MIT License.

# Author :

Developed by [Navneet Bhatt(B1aZ3e)]

