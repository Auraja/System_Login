#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cctype>

using namespace std;

const int MAX_SIZE = 100;
char nimLogin[11] = ""; 

struct User {
    char nim[11];
    char nama[21];
    char kodeAkses[13];
};

bool cekDuplikasiData(const char* nim, const char* kodeAkses) {
    ifstream file;
    file.open("data.txt");

    char line[MAX_SIZE];

    while (file.getline(line, sizeof(line))) {
        char savedNim[11];
        char savedKodeAkses[13];

        sscanf(line, "#%[^;];%*[^;];%[^;];", savedNim, savedKodeAkses);

        if (strcmp(nim, savedNim) == 0 || strcmp(kodeAkses, savedKodeAkses) == 0) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool cekKataSandi(const char* nim, const char* kodeAkses) {
    ifstream file;
    file.open("data.txt");

    char line[MAX_SIZE];

    while (file.getline(line, sizeof(line))) {
        char savedNim[11];
        char savedNama[21];
        char savedKodeAkses[13];

        sscanf(line, "#%[^;];%[^;];%[^;];", savedNim, savedNama, savedKodeAkses);

        if (strcmp(savedNim, nim) == 0 && strcmp(savedKodeAkses, kodeAkses) == 0) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool validasiNIM(const char* nim) {
    int len = strlen(nim);
    if (len != 10) {
        cout << "NIM harus memiliki 10 digit!" << endl;
        return false;
    }

    for (int i = 0; i < len; i++) {
        if (!isdigit(nim[i])) {
            cout << "NIM harus berupa angka!" << endl;
            return false;
        }
    }

    return true;
}

bool validasiNama(const char* nama) {
    int len = strlen(nama);
    if (len > 20) {
        cout << "Nama tidak boleh lebih dari 20 karakter!" << endl;
        return false;
    }

    return true;
}

bool validasiKodeAkses(const char* kodeAkses, const char* nim) {
    int len = strlen(kodeAkses);
    if (len != 12) {
        cout << "Kode Akses harus memiliki 12 karakter!" << endl;
        return false;
    }

    for (int i = 0; i < 2; i++) {
        if (!isupper(kodeAkses[i])) {
            cout << "Dua karakter pertama pada Kode Akses harus huruf kapital!" << endl;
            return false;
        }
    }

    for (int i = 2; i < len; i++) {
        if (!isdigit(kodeAkses[i])) {
            cout << "Sepuluh karakter terakhir pada Kode Akses harus berupa angka NIM!" << endl;
            return false;
        }
    }

    char nimFromKodeAkses[11];
    strncpy(nimFromKodeAkses, kodeAkses + 2, 10);
    nimFromKodeAkses[10] = '\0';

    if (strcmp(nim, nimFromKodeAkses) != 0) {
        cout << "Kode Akses tidak sesuai dengan NIM yang dimasukkan!" << endl;
        return false;
    }

    return true;
}

void rekamData() {
    User user;
    cout << "=== Pendaftaran Pengguna ===" << endl;

    do {
        cout << "Masukkan NIM (10 digit): ";
        cin >> user.nim;
        cin.ignore();
    } while (!validasiNIM(user.nim));

    if (cekDuplikasiData(user.nim, "")) {
        cout << "Data dengan NIM tersebut sudah terdaftar!" << endl;
        return;
    }

    do {
        cout << "Masukkan Nama (max 20 karakter): ";
        cin >> user.nama;
        cin.ignore();
    } while (!validasiNama(user.nama));

    char kodeAkses[13];

    do {
        cout << "Masukkan Kode Akses (2 huruf kapital dan 10 angka NIM): ";
        cin >> kodeAkses;
        cin.ignore();
    } while (!validasiKodeAkses(kodeAkses, user.nim));

    if (cekDuplikasiData("", kodeAkses)) {
        cout << "Data dengan Kode Akses tersebut sudah terdaftar!" << endl;
        return;
    }

    ofstream file;
    file.open("data.txt", ios::app);
    file << "#" << user.nim << ";" << user.nama << ";" << kodeAkses << ";" << endl;
    file.close();

    cout << "Pendaftaran pengguna berhasil!" << endl;
}

void tampilkanData() {
    cout << "=== Data Pengguna ===" << endl;

    ifstream file;
    file.open("data.txt");

    char line[MAX_SIZE];

    while (file.getline(line, sizeof(line))) {
        char nim[11];
        char nama[21];
        char kodeAkses[13];

        sscanf(line, "#%[^;];%[^;];%[^;];", nim, nama, kodeAkses);

        cout << "NIM: " << nim << endl;
        cout << "Nama: " << nama << endl;
        cout << "Kode Akses: " << kodeAkses << endl;
        cout << "--------------------------" << endl;
    }

    file.close();
}

void buatLog(const char* nim, const char* tipeLog) {
    ofstream file;
    file.open("userlog.txt", ios::app);

    time_t now = time(0);
    char* dt = ctime(&now);

    dt[strlen(dt) - 1] = '\0';

    file << "NIM " << nim << ", Waktu " << tipeLog << ": " << dt << endl;
    file << "----------------------------" << endl;

    file.close();
}

void login() {
    char nim[11];
    char kodeAkses[13];

    cout << "=== Login ===" << endl;
    cout << "Masukkan NIM: ";
    cin >> nim;
    cin.ignore();

    cout << "Masukkan Kode Akses: ";
    cin >> kodeAkses;
    cin.ignore();

    if (cekKataSandi(nim, kodeAkses)) {
        strncpy(nimLogin, nim, sizeof(nimLogin) - 1);
        nimLogin[sizeof(nimLogin) - 1] = '\0';

        char namaPengguna[21];
        ifstream file;
        file.open("data.txt");
        char line[MAX_SIZE];
        while (file.getline(line, sizeof(line))) {
            char savedNim[11];
            char savedNama[21];
            char savedKodeAkses[13];

            sscanf(line, "#%[^;];%[^;];%[^;];", savedNim, savedNama, savedKodeAkses);

            if (strcmp(savedNim, nim) == 0) {
                strncpy(namaPengguna, savedNama, sizeof(namaPengguna) - 1);
                namaPengguna[sizeof(namaPengguna) - 1] = '\0';
                break;
            }
        }
        file.close();

        cout << "Login berhasil! \nSelamat datang, " << namaPengguna << "!" << endl;
        buatLog(nimLogin, "Login");
    } else {
        cout << "NIM atau Kode Akses yang Anda masukkan salah!" << endl;
    }
}

void logout() {
    cout << "=== Logout ===" << endl;
    if (strlen(nimLogin) > 0) {  
        buatLog(nimLogin, "Logout");
        cout << "Logout berhasil!" << endl;
        nimLogin[0] = '\0';  
    } else {
        cout << "Tidak ada pengguna yang login saat ini!" << endl;
    }
}

int main() {
    int pilihan;

    do {
        cout << "=== Menu ===" << endl;
        cout << "1. Registrasi Pengguna Baru" << endl;
        cout << "2. Login" << endl;
        cout << "3. Logout" << endl;
        cout << "4. Tampilkan Data Pengguna" << endl;
        cout << "0. Exit Program" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                rekamData();
                break;
            case 2:
                login();
                break;
            case 3:
                logout();
                break;
            case 4:
                tampilkanData();
                break;
            case 0:
                cout << "Terima kasih! Program selesai." << endl;
                if (strlen(nimLogin) > 0) {  
                    buatLog(nimLogin, "Logout");
                    nimLogin[0] = '\0'; 
                }
                break;
            default:
                cout << "Pilihan menu tidak valid!" << endl;
                break;
        }

        cout << endl;

    } while (pilihan != 0);

    return 0;
}
