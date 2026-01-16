#include "autholas.h"

// API configuration - CHANGE THESE VALUES
const std::string API_KEY = "YOUR_API_KEY_HERE";
const std::string API_URL = "https://autholas.web.id/api/auth";

// Callback function to write response data
static size_t WriteCallbackFunc(void* contents, size_t size, size_t nmemb, WriteCallback* userp) {
    size_t totalSize = size * nmemb;
    userp->data.append((char*)contents, totalSize);
    return totalSize;
}

// ======= SYSTEM INFO FUNCTIONS =======
std::string getSystemUsername() {
#ifdef _WIN32
    char username[256];
    DWORD size = sizeof(username);
    if (GetUserNameA(username, &size)) {
        return std::string(username);
    }
#else
    char* username = getenv("USER");
    if (username) {
        return std::string(username);
    }
#endif
    return "unknown";
}

std::string getSystemHostname() {
#ifdef _WIN32
    char hostname[256];
    DWORD size = sizeof(hostname);
    if (GetComputerNameA(hostname, &size)) {
        return std::string(hostname);
    }
#else
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
#endif
    return "unknown";
}

std::string getSystemArchitecture() {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    switch (sysInfo.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64: return "x64";
    case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
    case PROCESSOR_ARCHITECTURE_ARM64: return "arm64";
    default: return "unknown";
    }
#else
    return "linux";
#endif
}

// ======= SHA-256 IMPLEMENTATION =======
uint32 rotr(uint32 x, uint32 n) {
    return (x >> n) | (x << (32 - n));
}

uint32 ch(uint32 x, uint32 y, uint32 z) {
    return (x & y) ^ (~x & z);
}

uint32 maj(uint32 x, uint32 y, uint32 z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32 e0(uint32 x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

uint32 e1(uint32 x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

uint32 s0(uint32 x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32 s1(uint32 x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

std::string sha256(const std::string& input) {
    const uint32 K[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
        0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
        0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
        0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
        0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
        0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
        0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
        0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
        0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    uint32 h[8] = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };

    std::vector<uint8> data(input.begin(), input.end());
    data.push_back(0x80);
    while ((data.size() + 8) % 64 != 0) data.push_back(0x00);

    uint64_t bit_len = input.size() * 8;
    for (int i = 7; i >= 0; --i) data.push_back((bit_len >> (i * 8)) & 0xFF);

    for (size_t i = 0; i < data.size(); i += 64) {
        uint32 w[64];
        for (int j = 0; j < 16; ++j) {
            w[j] = (data[i + j * 4] << 24) | (data[i + j * 4 + 1] << 16) |
                (data[i + j * 4 + 2] << 8) | data[i + j * 4 + 3];
        }
        for (int j = 16; j < 64; ++j) {
            w[j] = s1(w[j - 2]) + w[j - 7] + s0(w[j - 15]) + w[j - 16];
        }

        uint32 a = h[0], b = h[1], c = h[2], d = h[3];
        uint32 e = h[4], f = h[5], g = h[6], hh = h[7];

        for (int j = 0; j < 64; ++j) {
            uint32 t1 = hh + e1(e) + ch(e, f, g) + K[j] + w[j];
            uint32 t2 = e0(a) + maj(a, b, c);
            hh = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }

    std::ostringstream oss;
    for (int i = 0; i < 8; ++i)
        oss << std::hex << std::setw(8) << std::setfill('0') << h[i];
    return oss.str();
}

// ======= GET HARDWARE ID =======
std::string getHardwareID() {
    std::string hostname = getSystemHostname();
    std::string username = getSystemUsername();
    std::string architecture = getSystemArchitecture();
    std::string combined = hostname + "|" + username + "|" + architecture;
    return sha256(combined);
}

// Error handling function
void handleAuthError(const std::string& errorCode, const std::string& errorMessage) {
    std::map<std::string, std::pair<std::string, std::string>> errorMessages = {
        {"INVALID_CREDENTIALS", {"Login Failed", "Username or password is incorrect.\\nPlease double-check your credentials and try again."}},
        {"USER_BANNED", {"Account Banned", "Your account has been suspended.\\nPlease contact support for assistance."}},
        {"SUBSCRIPTION_EXPIRED", {"Subscription Expired", "Your subscription has ended.\\nPlease renew your subscription to continue."}},
        {"MAX_DEVICES_REACHED", {"Device Limit Reached", "Maximum number of devices exceeded.\\nPlease contact support to reset your devices."}},
        {"HWID_BANNED", {"Device Banned", "This device has been banned.\\nPlease contact support for assistance."}},
        {"INVALID_API_KEY", {"Service Error", "Authentication service unavailable.\\nPlease try again later or contact support."}},
        {"RATE_LIMIT_EXCEEDED", {"Too Many Attempts", "Please wait before trying again."}},
        {"DEVELOPER_SUSPENDED", {"Service Unavailable", "Authentication service is temporarily unavailable.\\nPlease contact support."}},
        {"SERVICE_ERROR", {"Service Error", "Authentication service is temporarily unavailable.\\nPlease try again later."}}
    };

    if (errorMessages.find(errorCode) != errorMessages.end()) {
        std::cout << errorMessages[errorCode].first << std::endl;
        std::cout << errorMessages[errorCode].second << std::endl;
    }
    else {
        std::cout << "Error: " << errorMessage << std::endl;
    }
}

// Function to authenticate user in your application
bool authenticateUser(const std::string& username,
    const std::string& password,
    const std::string& hwid) {

    json payload;
    payload["api_key"] = API_KEY;
    payload["username"] = username;
    payload["password"] = password;
    payload["hwid"] = hwid;
    payload["device_name"] = "User PC";

    std::cout << "Authenticating..." << std::endl;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cout << "Failed to initialize curl" << std::endl;
        return false;
    }

    std::string jsonString = payload.dump();
    WriteCallback response;

    curl_easy_setopt(curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cout << "Network error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    try {
        json responseJson = json::parse(response.data);

        if (responseJson["success"].get<bool>()) {
            std::cout << "Login successful!" << std::endl;
            std::cout << "Welcome, " << username << "!" << std::endl;
            std::cout << "Session token: " << responseJson["session_token"] << std::endl;
            std::cout << "Session expires: " << responseJson["expires_at"] << std::endl;
            std::cout << "Subscription expires: " << responseJson["user"]["expires_at"] << std::endl;
            return true;
        }
        else {
            std::string errorCode = responseJson.value("error_code", "UNKNOWN");
            std::string errorMessage = responseJson.value("error", "Unknown error");
            handleAuthError(errorCode, errorMessage);
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Failed to parse response: " << e.what() << std::endl;
        return false;
    }

}

