#include <string>

using namespace std;

//defines
#define TYPE_CONTACT "contact"
#define TYPE_ACQUAINTANCE "acquaintance"
#define TYPE_FRIEND "friend"
#define TYPE_COLLEAGUE "colleague"

#define OUTPUT_DELIMITER '|'
#define KEY_DELIMITER ':'
#define KEY_DELIMITER_STR ":"
#define OUTPUT_DELIMITER_STR "|"


struct ExtraInfo
{
	string key;
	string value;
};