#ifndef AUTHOLAS_H
#define AUTHOLAS_H

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <fstream>
#endif

using json = nlohmann::json;

// Configuration
extern const std::string API_KEY;
extern const std::string API_URL;

// Callback structure for HTTP response
struct WriteCallback {
    std::string data;
};

// Type definitions for SHA-256
typedef unsigned int uint32;
typedef unsigned char uint8;

// Function declarations
static size_t WriteCallbackFunc(void* contents, size_t size, size_t nmemb, WriteCallback* userp);

// System info functions
std::string getSystemUsername();
std::string getSystemHostname();
std::string getSystemArchitecture();

// SHA-256 helper functions
uint32 rotr(uint32 x, uint32 n);
uint32 ch(uint32 x, uint32 y, uint32 z);
uint32 maj(uint32 x, uint32 y, uint32 z);
uint32 e0(uint32 x);
uint32 e1(uint32 x);
uint32 s0(uint32 x);
uint32 s1(uint32 x);

// Main functions
std::string sha256(const std::string& input);
std::string getHardwareID();
void handleAuthError(const std::string& errorCode, const std::string& errorMessage);
bool authenticateUser(const std::string& username, const std::string& password, const std::string& hwid);

#endif // AUTHOLAS_H