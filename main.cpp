#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;



// функция создания файла
bool create_file(const string& filename)
{
	ofstream file(filename);
	return file.is_open();
}



// функция проверки существования файла
bool file_exist(const string& filename)
{
	ifstream file(filename);
	return file.is_open();
}



// функция создания простого пароля
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



// функция шифрования файла
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



// функция дешифровки файла
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



// функция создания ключей RSA
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
    system("chcp 1251");
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
        "1. Шифрование\n"
        "2. Дешифрование\n"
        "3. Добавить Диму\n"
        "4. Вариант 4\n"
        "5. Вариант 5\n"
        "6. Выход"
        "\n-----------------\n"
        "Выбери вариант: ";

    int choise = 0;
    while (choise != 6) {
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
        case 4:
            
            break;
        case 5:
            
            break;
        case 6:
            break;
        default:
            cout << "\nThere is no such option.";
        }
    }
}