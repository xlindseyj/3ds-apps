#ifndef NETWORK_H
#define NETWORK_H

int initNetworking();
int connectToVPN(const char* api_key);
void cleanupNetworking();

#endif