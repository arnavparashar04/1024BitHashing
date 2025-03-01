#include <iostream>
#include <vector>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdint>
using namespace std;
const int HASH_LENGTH = 1024;
const uint64_t PRIME1 = 11400714785074694791ULL;
const uint64_t PRIME2 = 14029467366897019727ULL;
const int MEMORY_HARDNESS = 1024 * 1024 * 5;
uint64_t Salt(const string &password, const string &username)
{
    uint64_t salt = PRIME1;
    for (char c : (password + username))
    {
        salt ^= static_cast<uint64_t>(c) * PRIME2;
        salt = (salt << 7) | (salt >> (64 - 7)); // shifting
        salt *= PRIME1;
        salt ^= (salt >> 17) | (salt << (64 - 17)); // more mixing
    }
    return salt;
}

vector<uint64_t> Lattice(const string &password, uint64_t salt)
{
    vector<uint64_t> poly(HASH_LENGTH / 64);
    for (size_t i = 0; i < password.size(); i++)
    {
        poly[i % poly.size()] ^= (static_cast<uint64_t>(password[i]) + salt) * PRIME1;
    }
    return poly;
}

void memoryHardening(vector<uint64_t> &poly, uint64_t salt)
{
    vector<uint64_t> memory(MEMORY_HARDNESS / 8, 0);
    for (size_t i = 0; i < MEMORY_HARDNESS / 8; i++)
    {
        memory[i] = (poly[i % poly.size()] + salt) ^ PRIME2;
        memory[i] = (memory[i] << 13) | (memory[i] >> (64 - 13));
    }
    for (size_t round = 0; round < 5; round++)
    {
        for (size_t i = 0; i < poly.size(); i++)
        {
            poly[i] ^= memory[i % memory.size()];
            poly[i] *= PRIME1;
            poly[i] ^= (poly[i] << 19) | (poly[i] >> (64 - 19));
        }
    }
}

void bitwiseMixing(vector<uint64_t> &poly)
{
    for (size_t i = 0; i < poly.size(); i++)
    {
        poly[i] ^= (poly[i] << 31) | (poly[i] >> (64 - 31));
        poly[i] *= PRIME1;
        poly[i] ^= (poly[i] << 17) | (poly[i] >> (64 - 17));
    }
}

string generateHash(const vector<uint64_t> &poly)
{
    stringstream hashStream;
    hashStream.fill('0');
    for (size_t i = 0; i < poly.size(); i++)
    {
        hashStream << hex << setw(16) << poly[i]; // setw to ignore after 16, 0s if less
    }
    return hashStream.str();
}

string Hash(const string &password, const string &username)
{
    uint64_t salt = Salt(password, username);
    vector<uint64_t> poly = Lattice(password, salt);
    memoryHardening(poly, salt);
    bitwiseMixing(poly);
    return generateHash(poly);
}

int main()
{
    char buffer = 't';
    while (buffer != 'q')
    {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        string hash = Hash(password, username);
        cout << hash << endl;

        cin >> buffer;
    }
}
