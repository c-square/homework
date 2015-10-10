#include <string.h>
#include <string>
#include <fstream>
#include <vector>

#define MaxKeyLen 10

using namespace std;


class ViginereCD
{
public:
	ViginereCD();
	ViginereCD(string buff);
	ViginereCD(const char *filePath);

	bool CryptBuffer();
	bool DecryptBuffer();
	//bool LoadFile(const char *);
	
	bool OpenFile(string);
	bool OpenFile(const char * filename);

	bool LoadFile(ifstream &);

	bool SetBuffer(string buffer);
	bool SetKey(string key);
	bool UnsetKey();
	string GetBuffer();
	string GetKey();

private:
	string buffer;
	string key;
	bool keySet;
	bool bufferSet;

	int detKeyLenght();
	double IndiceCoincidenta(int posKeyLen, int start);

	bool detKey(int keyLen);
	char MIC(vector<double> * frecv);
};
