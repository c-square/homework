#include "stdafx.h"
#include <wininet.h>
using namespace std;

struct FTP
{
	char* ftp;
	char* local_file;
	char* user;
	char* pass;
} f;
wchar_t* c_to_w(char* var)
{
	size_t size;
	size = strlen(var);
	wchar_t* w_var = new wchar_t[size + 1];
	for (size_t i = 0; i < size; i++)
		w_var[i] = var[i];
	w_var[size] = '\0';
	return w_var;
}
int _tmain(int argc, _TCHAR* argv[])
{
	HINTERNET hi;
	HINTERNET hc;
	HINTERNET ho;
	HINTERNET hftp;

	char buff[1024];
	char* tk = NULL;
	char* context = NULL;
	DWORD read;

	// HTTP REQUEST
	hi = InternetOpen("HTTPGET", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (hi == NULL)
	{
		printf("InternetOpen fail %d\n", GetLastError());
		return -1;
	}
	hc = InternetConnect(hi, "localhost", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hc == NULL)
	{
		printf("InternetConnect fail %d\n", GetLastError());
		return -1;
	}
	ho = HttpOpenRequest(hc, "GET", "/testfile.txt", NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
	if (ho == NULL)
	{
		printf("HttpOpenRequest fail %d\n", GetLastError());
		return -1;
	}
	if (!HttpSendRequest(ho, NULL, 0, NULL, 0))
	{
		printf("HttpSendRequest fail %d\n", GetLastError());
		return -1;
	}

	// READ
	memset(buff, 0, 1024);
	if (!InternetReadFile(ho, (BYTE*)buff, 1024, &read))
	{
		printf("InternetReadFile fail %d\n", GetLastError());
		return -1;
	}
	tk = strtok(buff, "\r\n");
	int line = 0;
	while (tk != NULL)
	{
		*((&f.ftp) + line) = (char*)malloc(256);
		strcpy((char*)*((&f.ftp) + line), tk);
		line++;
		tk = strtok(NULL, "\r\n");
	}
	InternetCloseHandle(ho);
	InternetCloseHandle(hc);
	InternetCloseHandle(hi);

	printf("\"%s\"\n", f.ftp);
	printf("'%s'\n", f.local_file);
	printf("'%s'\n", f.user);
	printf("'%s'\n", f.pass);
	char* wftp = f.ftp;
	char* wfile = f.local_file;
	char* wuser = f.user;
	char* wpass = f.pass;


	// ftp
	hi = InternetOpen("FTP_Client_Sample", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (hi == NULL)
	{
		printf("InternetOpen fail %d\n", GetLastError());
		//return -1;
	}
	hc = InternetConnect(hi, wftp, INTERNET_DEFAULT_FTP_PORT, wuser, wpass, INTERNET_SERVICE_FTP, NULL, 0);
	if (hc == NULL)
	{
		printf("InternetConnect fail %d\n", GetLastError());
		//return -1;
	}
	if (!FtpPutFile(hc, "blah.txt", "blah.txt", FTP_TRANSFER_TYPE_BINARY, NULL))
	{
		printf("FtpPutFile fail %d\n", GetLastError());
		DWORD err;
		char ee[1024];
		DWORD a = 1024;
		InternetGetLastResponseInfo(&err, ee, &a);
		printf("%s\n", ee);
		printf("%d\n", err);
		//return -1;
	}
	InternetCloseHandle(hc);
	InternetCloseHandle(hi);
	//InternetCloseHandle(hftp);
	printf("DONE\n");
	//scanf_s("%d", read);
	return 0;
}

