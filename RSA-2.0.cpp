#include <iostream>
#include <vector>
#include<tuple>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <random>
#include <bitset>

using namespace std;
using int128 = __int128;

// Function to print a 128-bit integer
void print_int128(int128 num) {
    if (num == 0) {
        cout << "0";
        return;
    }
    string str = "";
    while (num > 0) {
        str = char('0' + (num % 10)) + str;
        num /= 10;
    }
    cout << str;
}

// Fast modular exponentiation
int128 mod_exp(int128 base, int128 exp, int128 mod) {
    int128 result = 1;
    base = base%mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Miller-Rabin primality test
bool is_prime(int128 num, int iterations = 5) {
    if (num < 2 || num % 2 == 0) return false;
    if (num == 2 || num == 3) return true;
    
    int128 d = num - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dist(2, (uint64_t)num-2);
    
    for(int i  = 0; i < iterations ; i++){
        int128 a = 2 + (int128)dist(gen);

        int128 x = mod_exp(a, d, num);
        if (x == 1 || x == num - 1) continue;
        
        bool composite = true;
        for (int r = 0; r < s - 1; r++) {
            x = mod_exp(x, 2, num);
            if (x == num - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

//  128-bit prime number by multiplying two 64-bit primes
int128 generate_large_prime(bool is_128_bit) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dist(1ULL << 30, (1ULL << 31) - 1);
    
    int128 prime;
    do {
        if (is_128_bit) {
            prime = dist(gen) | 1;
        } else {
            prime = dist(gen) | 1;
        }
    } while (!is_prime(prime));
    return prime;
}

// Extended Euclidean Algorithm to find modular inverse
tuple<int128, int128, int128> gcd_extended(int128 a, int128 b) {
    if (a == 0)
        return make_tuple(b, 0, 1);
    int128 gcd, x1, y1;
    tie(gcd, x1, y1) = gcd_extended(b % a, a);
    return make_tuple(gcd, y1 - (b / a) * x1, x1);
}

int128 mod_inverse(int128 e, int128 phi) {
    int128 gcd, x, y;
    tie(gcd, x, y) = gcd_extended(e, phi);
    if (gcd != 1) return -1;
    return (x % phi + phi) % phi;
}

// Generate public-private keys
pair<pair<int128, int128>, pair<int128, int128>> generate_keys(bool is_128_bit) {
    cout << "Generating prime p..." << endl;
    int128 p = generate_large_prime(is_128_bit);
    cout << "p generated." << endl;

    cout << "Generating prime ..." << endl;
    int128 q = generate_large_prime(is_128_bit);
    cout << "q generated." << endl;
    
    int128 n = p * q;
    int128 phi = (p - 1) * (q - 1);
    int128 e = 65537;
    int128 d = mod_inverse(e, phi);
    if (d == -1) {
        cerr << " Could not find modular inverse!" << endl;
        exit(1);
    }
    return {{e, n}, {d, n}};
}

// Encrypt text
vector<int128> encrypt_text(string message, int128 e, int128 n) {
    vector<int128> encrypted;
    for (char ch : message) {
        int128 encrypted_value = mod_exp(static_cast<unsigned char>(ch), e, n);
        encrypted.push_back(encrypted_value);
    }    
    return encrypted;
}

// Decrypt text
string decrypt_text(vector<int128> encrypted, int128 d, int128 n) {
    string decrypted;
    for (int128 num : encrypted){
        int128 decrypted_value = mod_exp(num, d, n);
        decrypted += static_cast<char>(decrypted_value);
    }
    return decrypted;
}

// Encrypt image file
void encrypt_image(const string& filename, const string& out_filename, int128 e, int128 n) {
    ifstream in(filename, ios::binary);
    ofstream out(out_filename, ios::binary);
    char byte;
    while (in.get(byte)) {
        int128 encrypted_byte = mod_exp((unsigned char)byte, e, n);
        out.write(reinterpret_cast<char*>(&encrypted_byte), sizeof(encrypted_byte));
    }
    in.close();
    out.close();
}

// Decrypt image file
void decrypt_image(const string& filename, const string& out_filename, int128 d, int128 n) {
    ifstream in(filename, ios::binary);
    ofstream out(out_filename, ios::binary);
    int128 encrypted_byte;
    while (in.read(reinterpret_cast<char*>(&encrypted_byte), sizeof(encrypted_byte))) {
        char decrypted_byte = mod_exp(encrypted_byte, d, n);
        out.put(decrypted_byte);
    }
    in.close();
    out.close();
}

int main() {
    int x = 0; // default it is for 64 bit
    cout<<"you want security higer = 1 or default = 0 :";
    cin>>x;
    bool use_128_bit;
    if(x == 1) use_128_bit = true;
    else use_128_bit = false;

    pair<pair<int128, int128>, pair<int128, int128>> keys = generate_keys(use_128_bit);
    pair<int128, int128> public_key = keys.first;
    pair<int128, int128> private_key = keys.second;

    int128 e = public_key.first;
    int128 n = public_key.second;
    int128 d = private_key.first;



    // Text Encryption 
    string message;
    cout << "Enter a message to encrypt: ";
    cin.ignore();
    getline(cin, message);

    vector<int128> encrypted = encrypt_text(message, e, n);
    string decrypted = decrypt_text(encrypted, d, n);

    cout << "Original Message: " << message << endl;
    cout << "Encrypted Message: [";
    for (size_t i = 0; i < encrypted.size(); i++) {
        print_int128(encrypted[i]);
        if (i + 1 != encrypted.size()) cout << ", ";
    }
    cout << "]" << endl;    
    cout << "Decrypted Message: " << decrypted << endl;

    encrypt_image("13.jpg", "encrypted.enc", e, n);
    decrypt_image("encrypted.enc", "decrypted.bmp", d, n);

    return 0;
}
