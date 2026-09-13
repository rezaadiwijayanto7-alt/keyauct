#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <wininet.h>
#include <sstream>

#pragma comment(lib, "wininet.lib")

namespace KeyAuthLib {

    struct AuthResponse {
        bool success = false;
        std::string status = "";
        std::string message = "";
        std::string licenseKey = "";
        std::string duration = "";
        std::string remaining = "";
        std::string expiresAt = "";
    };

    class KeyAuth {
    private:
        std::string serverHost;
        int serverPort;
        bool isHttps;

        std::string ExtractJsonValue(const std::string& json, const std::string& key) {
            std::string searchKey = "\"" + key + "\":\"";
            size_t start = json.find(searchKey);
            if (start != std::string::npos) {
                start += searchKey.length();
                size_t end = json.find("\"", start);
                if (end != std::string::npos) {
                    return json.substr(start, end - start);
                }
            }
            return "";
        }

        std::string GetHWID() {
            DWORD serialNumber = 0;
            GetVolumeInformationA("C:\\", NULL, 0, &serialNumber, NULL, NULL, NULL, 0);

            char compName[MAX_COMPUTERNAME_LENGTH + 1];
            DWORD size = sizeof(compName);
            GetComputerNameA(compName, &size);

            std::stringstream ss;
            ss << compName << "-" << std::hex << serialNumber;
            return ss.str();
        }

    public:
        KeyAuth(const std::string& host = "localhost", int port = 3000, bool https = false)
            : serverHost(host), serverPort(port), isHttps(https) {}

        // Verifikasi License Key Saja
        AuthResponse AuthenticateKey(const std::string& licenseKey) {
            AuthResponse resp;
            
            HINTERNET hInternet = InternetOpenA("KeyAuthClient/1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (!hInternet) {
                resp.message = "Gagal menginisialisasi koneksi internet (WinINet).";
                return resp;
            }

            HINTERNET hConnect = InternetConnectA(
                hInternet, 
                serverHost.c_str(), 
                (INTERNET_PORT)serverPort, 
                NULL, 
                NULL, 
                INTERNET_SERVICE_HTTP, 
                0, 
                0
            );

            if (!hConnect) {
                InternetCloseHandle(hInternet);
                resp.message = "Tidak dapat terhubung ke server " + serverHost + ":" + std::to_string(serverPort);
                return resp;
            }

            std::string hwid = GetHWID();
            std::string urlPath = "/api/verify?key=" + licenseKey + "&hwid=" + hwid;

            DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;
            if (isHttps) flags |= INTERNET_FLAG_SECURE;

            HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", urlPath.c_str(), NULL, NULL, NULL, flags, 0);
            if (!hRequest) {
                InternetCloseHandle(hConnect);
                InternetCloseHandle(hInternet);
                resp.message = "Gagal membuat request HTTP ke server.";
                return resp;
            }

            BOOL bSend = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
            if (!bSend) {
                InternetCloseHandle(hRequest);
                InternetCloseHandle(hConnect);
                InternetCloseHandle(hInternet);
                resp.message = "Gagal mengirim data ke server. Pastikan server aktif!";
                return resp;
            }

            char buffer[4096];
            DWORD bytesRead = 0;
            std::string responseBody = "";

            while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                responseBody += buffer;
            }

            InternetCloseHandle(hRequest);
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);

            if (responseBody.find("\"authenticated\":true") != std::string::npos) {
                resp.success = true;
                resp.status = "success";
                resp.message = ExtractJsonValue(responseBody, "message");
                resp.licenseKey = ExtractJsonValue(responseBody, "licenseKey");
                resp.duration = ExtractJsonValue(responseBody, "duration");
                resp.remaining = ExtractJsonValue(responseBody, "remaining");
                resp.expiresAt = ExtractJsonValue(responseBody, "expiresAt");
            } else {
                resp.success = false;
                resp.status = ExtractJsonValue(responseBody, "status");
                resp.message = ExtractJsonValue(responseBody, "message");
                if (resp.message.empty()) {
                    resp.message = "Lisensi tidak ditemukan atau telah dihapus!";
                }
            }

            return resp;
        }
    };
}
