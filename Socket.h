﻿#pragma once
#include<WinSock2.h>
#include<iostream>
#include<Windows.h>
#include"StringMatcher.h"
using namespace std;
SOCKET sock;
string resultPage;
string request;
char buffer[1024];
//universal getpage function
string *getPage(string &host, string &directory) {
retry:;
	resultPage = ""; request = "";

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		closesocket(sock);
		goto retry;
	}

	sockaddr_in address = { AF_INET };

	if (bind(sock, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}
	hostent *HostInfo = gethostbyname(host.c_str());
	if (HostInfo == 0) {
		closesocket(sock);
		goto retry;
	}

	address.sin_port = htons(80);
	memcpy(&address.sin_addr, HostInfo->h_addr, 4);

	if (connect(sock, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}

	request = "GET " + directory + " HTTP/1.1\r\nHost: " + host + "\r\nConnection:Close\r\n\r\n";

	if (send(sock, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}

	Sleep(1000);
	int recive;
	while (recive = recv(sock, buffer, sizeof(buffer) - 1, 0))
		resultPage += buffer;

	closesocket(sock);
	return &resultPage;
}
string *getPageWithJSESSIONID(string &JSESSIONID, string &host, string &directory) {
retry:;
	resultPage = ""; request = "";

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		closesocket(sock);
		goto retry;
	}

	sockaddr_in address = { AF_INET };

	if (bind(sock, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}
	hostent *HostInfo = gethostbyname(host.c_str());
	if (HostInfo == 0) {
		closesocket(sock);
		goto retry;
	}

	address.sin_port = htons(80);
	memcpy(&address.sin_addr, HostInfo->h_addr, 4);

	if (connect(sock, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}

	request = "GET " + directory + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection:Close\r\n" +
		"Cookie:JSESSIONID=" + JSESSIONID + "\r\n" +
		"\r\n";

	if (send(sock, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}

	Sleep(1000);
	int recive;
	while (recive = recv(sock, buffer, sizeof(buffer) - 1, 0))
		resultPage += buffer;

	closesocket(sock);
	return &resultPage;
}
extern string toString(int);
//login to poj.org
void PostDataWithJSESSIONID(string &JsessionID, string &data, string &dir, string &host) {
	request = (string)"POST http://poj.org/login HTTP/1.1\r\n" +
		"Content-Type: application/x-www-form-urlencoded\r\n" +
		"Content-Length:" + toString(data.length()) + "\r\n" +
		"Host:" + host + "\r\n" +
		"Connection: Keep-Alive\r\n" +
		"Pragma: no-cache\r\n" +
		"Cookie:JSESSIONID=" + JsessionID + "\r\n" +
		"\r\n" + data;
retry:;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		closesocket(sock);
		goto retry;
	}

	sockaddr_in address = { AF_INET };

	if (bind(sock, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}
	hostent *HostInfo = gethostbyname(host.c_str());
	if (HostInfo == 0) {
		closesocket(sock);
		goto retry;
	}

	address.sin_port = htons(80);
	memcpy(&address.sin_addr, HostInfo->h_addr, 4);

	if (connect(sock, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}

	if (send(sock, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
		closesocket(sock);
		goto retry;
	}

	closesocket(sock);
	return;
}

string *getJSESSIONID(string &host) {
	getPage(host, (string)"/");
	int cookiePos = match(resultPage, (string)"JSESSIONID=") + 11;
	int scout = cookiePos;
	while (resultPage[scout] != ';')scout++;
	resultPage = resultPage.substr(cookiePos, scout-cookiePos);

	return &resultPage;
}












//send final code 
//UNAVAILIBLE
const string ContentType_HEADER = "\r\nContent-Type: application/x-www-form-urlencoded";
//表单默认content type 为变量所示
//multipart/form-data 常用
const string ContentLeng_HEADER = "\r\nContent-Length: ";

void sendForm(string &CODE, string &pID) {

	{
		request = "\r\nCache-Control: max-age=0\r\n";
		request += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8O\r\n";
		request += "Origin: http://poj.org\r\n";
		request += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36\r\n";
		request += ("Referer: http://poj.org/submit?problem_id=" + pID + "\r\n");
		request += "Accept-Encoding: gzip, deflate\r\nAccept-Language: zh-CN,zh;q=0.8";
	}


}
//function encodeSource() {
//	document.getElementById("source").value = Base64.encode(document.getElementById("source").value);
//	return true;
//}
//<form method = POST action = submit onsubmit = "return encodeSource()">