#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <wininet.h>
#include <sstream>

#pragma comment(lib, "wininet.lib")

namespace KeyAuthGitHub {

    struct LicenseInfo {
        bool isValid = false;
        std::string username = "";
        std::string licenseKey = "";
        std::string status = "";
        std::string notes = "";
        std::string expiresAt = "";
        std::string message = "";
    };

    class Client {
    private:
        std::string rawJsonUrl;

        // Fetch URL content over HTTPS using Windows WinINet
        std::string DownloadString(const std::string& url) {
            HINTERNET hInternet = InternetOpenA("KeyAuthGitHubClient/1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (!hInternet) return "";

            DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE;
            HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, flags, 0);
            if (!hUrl) {
                InternetCloseHandle(hInternet);
                return "";
            }

            char buffer[4096];
            DWORD bytesRead = 0;
            std::string result = "";
            while (InternetReadFile(hUrl, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                result += buffer;
            }

            InternetCloseHandle(hUrl);
            InternetCloseHandle(hInternet);
            return result;
        }

        // Helper to extract JSON string value
        std::string ExtractField(const std::string& block, const std::string& field) {
            std::string key = "\"" + field + "\"";
            size_t pos = block.find(key);
            if (pos == std::string::npos) return "";

            size_t colon = block.find(":", pos);
            if (colon == std::string::npos) return "";

            size_t quoteStart = block.find("\"", colon);
            if (quoteStart == std::string::npos) return "";

            size_t quoteEnd = block.find("\"", quoteStart + 1);
            if (quoteEnd == std::string::npos) return "";

            return block.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        }

        // To uppercase
        std::string ToUpper(std::string str) {
            for (char &c : str) c = toupper(c);
            return str;
        }

    public:
        Client(const std::string& githubRawUrl) : rawJsonUrl(githubRawUrl) {}

        LicenseInfo Verify(const std::string& inputUsername, const std::string& inputKey) {
            LicenseInfo info;

            if (inputUsername.empty() || inputKey.empty()) {
                info.message = "Username dan Key tidak boleh kosong!";
                return info;
            }

            std::string jsonData = DownloadString(rawJsonUrl);
            if (jsonData.empty()) {
                info.message = "Gagal mengunduh database lisensi dari GitHub. Periksa koneksi internet / URL raw!";
                return info;
            }

            // Loop through each key object in the JSON
            size_t pos = 0;
            while ((pos = jsonData.find("{", pos)) != std::string::npos) {
                size_t endPos = jsonData.find("}", pos);
                if (endPos == std::string::npos) break;

                std::string objectBlock = jsonData.substr(pos, endPos - pos + 1);
                std::string uName = ExtractField(objectBlock, "username");
                std::string lKey = ExtractField(objectBlock, "licenseKey");
                std::string status = ExtractField(objectBlock, "status");
                std::string exp = ExtractField(objectBlock, "expiresAt");
                std::string notes = ExtractField(objectBlock, "notes");

                if (ToUpper(uName) == ToUpper(inputUsername) && ToUpper(lKey) == ToUpper(inputKey)) {
                    info.username = uName;
                    info.licenseKey = lKey;
                    info.status = status;
                    info.expiresAt = exp;
                    info.notes = notes;

                    if (status != "active") {
                        info.isValid = false;
                        info.message = "Lisensi telah dinonaktifkan / diblokir (Status: " + status + ")!";
                        return info;
                    }

                    info.isValid = true;
                    info.message = "Autentikasi Berhasil!";
                    return info;
                }

                pos = endPos + 1;
            }

            info.isValid = false;
            info.message = "Username atau License Key tidak ditemukan di GitHub!";
            return info;
        }
    };
}
