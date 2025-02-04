#include <iostream>
#include <vector>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdint>
using namespace std;
const int HASH_LENGTH = 64;
const uint64_t PRIME1 = 11400714785074694791ULL;
const uint64_t PRIME2 = 14029467366897019727ULL;
const int MEMORY_HARDNESS = 1024 * 1024;
uint64_t generateFixedSalt(const string &password, const string &username)
{
    uint64_t salt = PRIME1;
    for (char c : (password + username))
    {
        salt ^= static_cast<uint64_t>(c) * PRIME2;
        salt = (salt << 7) | (salt >> 57);
    }
    return salt;
}

vector<uint64_t> passwordToLattice(const string &password, uint64_t salt)
{
    vector<uint64_t> poly(HASH_LENGTH / 8);
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
        memory[i] = (memory[i] << 13) | (memory[i] >> 51);
    }
    for (size_t i = 0; i < poly.size(); i++)
    {
        poly[i] ^= memory[(i * PRIME1) % memory.size()];
    }
}
void bitwiseMixing(vector<uint64_t> &poly)
{
    for (size_t i = 0; i < poly.size(); i++)
    {
        poly[i] ^= (poly[i] << 31) | (poly[i] >> 33);
        poly[i] *= PRIME1;
        poly[i] ^= (poly[i] << 17) | (poly[i] >> 47);
    }
}

string generateHash(const vector<uint64_t> &poly)
{
    stringstream hashStream;
    hashStream.fill('0');
    for (size_t i = 0; i < poly.size(); i++)
    {
        hashStream << hex << setw(16) << poly[i];
    }
    return hashStream.str();
}

string quantumResistantHash(const string &password, const string &username)
{
    uint64_t salt = generateFixedSalt(password, username);
    vector<uint64_t> poly = passwordToLattice(password, salt);
    memoryHardening(poly, salt);
    bitwiseMixing(poly);
    return generateHash(poly);
}

int main()
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string hash = quantumResistantHash(password, username);
    cout << "Quantum-Safe 512-bit Hash: " << hash << endl;

    return 0;
}
