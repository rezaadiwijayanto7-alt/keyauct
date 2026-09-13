#include <iostream>
#include <string>
#include <windows.h>
#include "KeyAuthGitHub.hpp"

// Utility untuk warna console Windows
void SetConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    SetConsoleTitleA("C++ KeyAuth - GitHub Online Edition");

    SetConsoleColor(11); // Cyan
    std::cout << "========================================================\n";
    std::cout << "     SISTEM AUTENTIKASI LISENSI C++ (GITHUB PUBLIC)     \n";
    std::cout << "========================================================\n\n";
    SetConsoleColor(7);

    // =========================================================================
    // GANTI URL DI BAWAH INI DENGAN URL RAW GITHUB ANDA:
    // Format: https://raw.githubusercontent.com/<USERNAME>/<REPO>/main/keys.json
    // =========================================================================
    std::string GITHUB_RAW_URL = "https://raw.githubusercontent.com/rezaadiwijayanto7-alt/keyauct/main/keys.json";

    KeyAuthGitHub::Client auth(GITHUB_RAW_URL);

    std::string username, key;
    std::cout << "[?] Masukkan Username   : ";
    std::getline(std::cin, username);

    std::cout << "[?] Masukkan License Key: ";
    std::getline(std::cin, key);

    std::cout << "\n[*] Menghubungi GitHub untuk memverifikasi lisensi...\n";

    KeyAuthGitHub::LicenseInfo res = auth.Verify(username, key);

    if (res.isValid) {
        SetConsoleColor(10); // Hijau
        std::cout << "\n[+] ====================================================\n";
        std::cout << "[+] STATUS: AUTENTIKASI SUKSES (ONLINE GITHUB)!\n";
        std::cout << "[+] ====================================================\n";
        SetConsoleColor(15);
        std::cout << "    Pengguna    : " << res.username << "\n";
        std::cout << "    License Key : " << res.licenseKey << "\n";
        std::cout << "    Status      : " << res.status << "\n";
        std::cout << "    Catatan     : " << res.notes << "\n\n";

        SetConsoleColor(14); // Kuning
        std::cout << "[*] Membuka akses penuh ke program C++...\n\n";

        // ==========================================================
        //  TEMPATKAN KODE / FITUR UTAMA PROYEK C++ ANDA DI BAWAH INI
        // ==========================================================
        SetConsoleColor(11);
        std::cout << "[INFO] Program C++ berhasil dijalankan!\n";

    } else {
        SetConsoleColor(12); // Merah
        std::cout << "\n[-] ====================================================\n";
        std::cout << "[-] STATUS: AKSES DITOLAK!\n";
        std::cout << "[-] ====================================================\n";
        std::cout << "    Pesan: " << res.message << "\n\n";

        SetConsoleColor(7);
        std::cout << "[-] Pastikan Username & Key terdaftar di file keys.json di GitHub.\n";
    }

    SetConsoleColor(7);
    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.get();
    return 0;
}
