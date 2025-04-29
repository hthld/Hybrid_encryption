#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;



// File creation function
bool create_file(const string& filename)
{
	ofstream file(filename);
	return file.is_open();
}



// File existence check function
bool file_exist(const string& filename)
{
	ifstream file(filename);
	return file.is_open();
}



// Simple password function
char* make_pass() {
    srand(time(nullptr));
    char* pass = new char[65];
    for (int i = 0; i < 64; ++i) {
        switch (rand() % 3) {
        case 0: pass[i] = rand() % 10 + '0'; break;
        case 1: pass[i] = rand() % 26 + 'A'; break;
        case 2: pass[i] = rand() % 26 + 'a'; break;
        }
    }
    pass[64] = '\0';
    return pass;
}



// File encryption function
void  encrypt(const string& filename, const string& openSSL_path)
{
    const char* password = make_pass();
    string command = openSSL_path + "\\openssl enc -aes-256-cbc -e -in " + filename + " -out " + filename + ".enc -iter 200000 -k " + password;
    system(command.c_str());

    if (remove(filename.c_str()) != 0)
    {
        cout << "[ERROR] - deleting " + filename << endl;
    }

    ofstream file;
    string key = "key_" + filename;
    file.open(key, ios::binary);
    file.write(password, 64);
    file.close();

    command = openSSL_path + "\\openssl.exe pkeyutl -encrypt -inkey public.key -pubin -in " + key + " -out " + key + ".enc";
    system(command.c_str());

    if (remove(key.c_str()) != 0)
    {
        cout << "[ERROR] - deleting key.txt" << endl;
    }

}



// File decryption function
void decrypt(const string& filename, const string& openSSL_path)
{
    string command = openSSL_path + "\\openssl.exe pkeyutl -decrypt -inkey private.key -in key_" + filename + ".enc -out key_" + filename;
    system(command.c_str());

    string key = "key_" + filename + ".enc";
    if (remove(key.c_str()) != 0)
    {
        cout << "[ERROR] - deleting file" << endl;
    }

    char* password = new char[65];
    ifstream file;
    key = "key_" + filename;
    file.open(key, ios::binary);
    file.read(password, 64);
    file.close();
    password[64] = '\0';

    if (remove(key.c_str()) != 0)
    {
        cout << "[ERROR] - deleting file" << endl;
    }

    command = openSSL_path + "\\openssl enc -aes-256-cbc -d -in " + filename + ".enc -out " + filename + " -iter 200000 -k " + password;
    system(command.c_str());

    string filenameENC = filename + ".enc";
    if (remove(filenameENC.c_str()) != 0)
    {
        cout << "[ERROR] - deleting file" << endl;
    }
}



// RSA key creation function
void create_rsa_keys()
{
    string privateKey = "private.key";
    string publicKey = "public.key";
    if (file_exist(privateKey)) {
        cout << "Private key already exists." << endl;
    }
    else { system("openssl genpkey -algorithm RSA -out private.key -pkeyopt rsa_keygen_bits:2048"); }
    if (file_exist(publicKey)) {
        cout << "Public key already exists" << endl;
    }
    else { system("openssl rsa -pubout -in private.key -out public.key"); }
}




int main()
{
    string openSSL_path = "C:\\Libs\\openssl-3.2.1\\builds\\v120-x64-release-shared\\bin";

    create_rsa_keys();

    string filename;
    cout << "Enter filename: ";
    getline(cin, filename);

    if (!file_exist(filename))
    {
        create_file(filename);
    }
 
    string contextMenu = 
        "\n-----------------\n"
        "1. Encryption\n"
        "2. Decryption\n"
        "3. End\n"
        "\n-----------------\n"
        "Choose option: ";

    int choise = 0;
    while (choise != 3) {
        cout << contextMenu;
        cin >> choise;
        switch (choise) {
        case 1:
            encrypt(filename, openSSL_path);
            cout << "\nEncrypted!" << endl;
            break;
        case 2:
            decrypt(filename, openSSL_path);
            cout << "\nDecrypted!" << endl;
            break;
        case 3:
            break;
        default:
            cout << "\nThere is no such option.";
        }
    }
}
