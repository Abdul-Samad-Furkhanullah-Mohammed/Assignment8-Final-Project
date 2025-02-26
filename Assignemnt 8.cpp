#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdint>

using namespace std;

// the constant vales
const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967
};

// performing the rotation to the right
uint32_t rotate_right(uint32_t value, unsigned int bits) {
    return (value >> bits) | (value << (32 - bits));
}

// Computation of hash
string sha256_hash(const string &input) {

    uint32_t H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // Conversion of the input into byte array form
    vector<uint8_t> message(input.begin(), input.end());
    uint64_t original_size_bits = message.size() * 8;


    message.push_back(0x80);


    while ((message.size() * 8) % 512 != 448) {
        message.push_back(0x00);
    }


    for (int i_10 = 7; i_10 >= 0; i_10--) {
        message.push_back((original_size_bits >> (i_10 * 8)) & 0xFF);
    }

    // Process in 512-bit (64-byte) chunks
    for (size_t chunk_start = 0; chunk_start < message.size(); chunk_start += 64) {
        uint32_t W[64] = {0};


        for (int i = 0; i < 16; i++) {
            W[i] = (message[chunk_start + i * 4] << 24) |
                   (message[chunk_start + i * 4 + 1] << 16) |
                   (message[chunk_start + i * 4 + 2] << 8) |
                   (message[chunk_start + i * 4 + 3]);
        }

        for (int i_01 = 16; i_01 < 64; i_01++) {
            uint32_t s0 = rotate_right(W[i_01 - 15], 7) ^ rotate_right(W[i_01 - 15], 18) ^ (W[i_01 - 15] >> 3);
            uint32_t s1 = rotate_right(W[i_01 - 2], 17) ^ rotate_right(W[i_01 - 2], 19) ^ (W[i_01 - 2] >> 10);
            W[i_01] = W[i_01 - 16] + s0 + W[i_01 - 7] + s1;
        }

        // Initialize working variables
        uint32_t a = H[0], b = H[1], c = H[2], d = H[3];
        uint32_t e = H[4], f = H[5], g = H[6], h = H[7];

        // SHA-256 compression function
        for (int i = 0; i < 64; i++) {
            uint32_t S1 = rotate_right(e, 6) ^ rotate_right(e, 11) ^ rotate_right(e, 25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp1 = h + S1 + ch + K[i] + W[i];

            uint32_t S0 = rotate_right(a, 2) ^ rotate_right(a, 13) ^ rotate_right(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // updating the values of the hash
        H[0] += a;
        H[1] += b;
        H[2] += c;
        H[3] += d;
        H[4] += e;
        H[5] += f;
        H[6] += g;
        H[7] += h;
    }

    // Convert hash values to hex string
    stringstream hash_output;
    for (int i = 0; i < 8; i++) {
        hash_output << hex << setw(8) << setfill('0') << H[i];
    }
    return hash_output.str();
}

// Function to read a file and return its content
string read_file(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file '" << filename << "'!" << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Main function
int main() {
    string filename = "Book_of_Mark.txt";

    // Read file content
    string text = read_file(filename);
    if (text.empty()) {
        return 1;
    }

    // Compute SHA-256 hash
    string hash_result = sha256_hash(text);

    // Display result
    cout << "SHA-256 Hash of '" << filename << "': " << hash_result << endl;

    return 0;
}
