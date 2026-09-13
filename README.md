# 🌐 Panduan Hosting KeyAuth ke GitHub (100% Gratis & Publik)

Dengan edisi ini, Anda **tidak perlu menyewa VPS/server**. Cukup upload ke GitHub dan aktifkan **GitHub Pages**.

---

## 📁 Struktur Folder GitHub Edition

```text
github_edition/
├── index.html              # Dashboard Web (Upload ini ke repo GitHub untuk GitHub Pages)
├── keys.json               # File database lisensi publik di GitHub
└── cpp_client/
    ├── KeyAuthGitHub.hpp   # Header C++ Client (WinINet HTTPS)
    ├── main.cpp            # Contoh Program C++
    └── build.bat           # Script compile otomatis
```

---

## 🚀 Langkah 1: Buat Repositori di GitHub

1. Buka [https://github.com/new](https://github.com/new).
2. Beri nama repositori (contoh: `my-keyauth`).
3. Pilih **Public** (agar bisa diakses oleh program C++ pengguna).
4. Centang **Add a README file** $\rightarrow$ klik **Create repository**.

---

## 📤 Langkah 2: Upload File ke GitHub

Upload 2 file utama ke repositori tersebut:
1. `index.html`
2. `keys.json`

*(Anda bisa langsung klik tombol **Add file** $\rightarrow$ **Upload files** di halaman GitHub Anda, lalu drag `index.html` dan `keys.json`).*

---

## 🌐 Langkah 3: Aktifkan GitHub Pages (Untuk Dashboard Web Publik)

1. Di repositori GitHub Anda, buka menu **Settings** (di tab atas).
2. Di menu samping kiri, klik **Pages**.
3. Di bagian **Build and deployment** $\rightarrow$ **Branch**:
   - Pilih `main` dan folder `/(root)`.
   - Klik tombol **Save**.
4. Tunggu 1 menit, Anda akan mendapatkan URL web publik:
   👉 `https://USERNAME.github.io/my-keyauth/`

---

## 💻 Langkah 4: Hubungkan ke Program C++

1. URL Raw file lisensi Anda adalah:
   `https://raw.githubusercontent.com/USERNAME/my-keyauth/main/keys.json`
2. Buka file `cpp_client/main.cpp`, ganti baris:
   ```cpp
   std::string GITHUB_RAW_URL = "https://raw.githubusercontent.com/USERNAME/my-keyauth/main/keys.json";
   ```
   *(Ganti `USERNAME` dan `my-keyauth` sesuai akun GitHub Anda).*
3. Compile dan jalankan `main.cpp` menggunakan `build.bat` atau:
   ```bash
   g++ main.cpp -o main.exe -lwininet
   ./main.exe
   ```

---

## ⚡ Fitur Auto-Sync Langsung dari Web Dashboard

Di web dashboard (`index.html`), klik tombol **Pengaturan GitHub**:
- Masukkan **Username**, **Repo**, dan **GitHub Token (PAT)**.
- Setiap kali Anda klik **Tambah Key** atau **Hapus Key**, dashboard akan **otomatis update ke GitHub** tanpa Anda perlu buka git terminal!
