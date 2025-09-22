#include <sstream>
#include "Utility.h"

#ifdef WIN32
#include <bitset>
#include <Windows.h>
#include <WinInet.h>
#include <Shlwapi.h>

#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Shlwapi.lib")
#endif

std::ostringstream Utility::noNewline;

void Utility::convert_to_hex_string(std::ostringstream& op, const unsigned char* data, int size)
{
	using namespace std;

	// Format flags
	ostream::fmtflags old_flags = op.flags();
	// Fill characters
	char old_fill = op.fill();
	op << hex << setfill('0');
	int charCounter = 0;
	for (int i = 0; i < size; i++)
	{
		charCounter++;
		// Give space between two hex values
		if (i > 0)
			op << ' ';
		// force output to use hex version of ascii code
		op << "0x" << setw(2) << static_cast<int>(data[i]) << ",";
		noNewline << "0x" << setw(2) << static_cast<int>(data[i]) << ",";
		if (charCounter >= 16)
		{
			op << "\n";
			charCounter = 0;
		}
	}
	op.flags(old_flags);
	op.fill(old_fill);
	noNewline.flags(old_flags);
	noNewline.fill(old_fill);
}

void Utility::convert_to_hex_slConfig(std::ostringstream& op, const unsigned char* data, int size)
{
	using namespace std;

	// Format flags
	ostream::fmtflags old_flags = op.flags();
	// Fill characters
	char old_fill = op.fill();
	op << hex << setfill('0');
	int charCounter = 0;
	for (int i = 0; i < size; i++)
	{
		charCounter++;
		op << setw(2) << static_cast<int>(data[i]);
	}

	op.flags(old_flags);
	op.fill(old_fill);
}

void Utility::convert_to_struct(std::istream& ip, unsigned char* data, int size)
{
	using namespace std;

	// Get the line we want to process
	string line;
	std::getline(ip, line);

	istringstream ip_convert(line);
	ip_convert >> hex;

	// Read in unsigned ints, as wrote out hex version
	// of ascii code
	unsigned int u;
	int i = 0;

	while ((ip_convert >> u) && (i < size))
		if ((0x00 <= u) && (0xff >= u))
			data[i++] = static_cast<unsigned char>(u);
}

pugi::xml_node Utility::parameterSet(pugi::xml_node docw, const char* jebo, const char* jebo2)
{
	using namespace std;

	pugi::xml_node ret;

	ret = docw.append_child("parameter");
	ret.append_attribute("name") =  jebo;
	ret.append_attribute("value") = jebo2;

	return ret;
}

pugi::xml_node Utility::parameterSetComm(pugi::xml_node docw, const char* jebo)
{
	using namespace std;

	pugi::xml_node ret;
	ret = docw.append_child();
	ret.set_value(jebo);

	return ret;
}

void Utility::convert_to_hex_stringTest(std::ostringstream& op, const unsigned char* data, int size)
{
	using namespace std;

	// Format flags
	ostream::fmtflags old_flags = op.flags();

	// Fill characters
	char old_fill = op.fill();
	op << hex << setfill('0');

	for (int i = 0; i < size; i++)
	{
		// Give space between two hex values
		if (i > 0)
			op << ' ';

		// force output to use hex version of ascii code
		op << "0x" << setw(2) << static_cast<int>(data[i]);
	}

	op.flags(old_flags);
	op.fill(old_fill);
}

void Utility::convert_to_structTest(std::istream& ip, unsigned char* data, int size)
{
	using namespace std;

	// Read in hex values separated by commas
	for (int i = 0; i < size; i++) {
		int value;
		if (ip >> hex >> value) {
			if (value >= 0x00 && value <= 0xff) {
				data[i] = static_cast<unsigned char>(value);
			}
			// Check for a comma separator
			if (ip.peek() == ',') {
				ip.ignore();
			}
		}
		else {
			// Stop reading if we reach the end of the input stream or encounter an error
			break;
		}
	}
}

bool Utility::UploadFile(const std::string& filePath, const std::string& ftpServer, const std::string& userName, const std::string& password)
{
#ifdef WIN32
	HINTERNET hInternet = InternetOpenA("FTP Upload", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		return false;
	}

	HINTERNET hFtpSession = InternetConnectA(hInternet, ftpServer.c_str(), INTERNET_DEFAULT_FTP_PORT, userName.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hFtpSession == NULL)
	{
		InternetCloseHandle(hInternet);
		return false;
	}

	std::string str1 = "USER\\LOAD\\";
	LPCSTR str2 = PathFindFileNameA(filePath.c_str());
	std::string str3 = str1 + str2;

	LPCSTR lpcstr = str3.c_str();

	if (FtpPutFileA(hFtpSession, filePath.c_str(), lpcstr, FTP_TRANSFER_TYPE_BINARY, 0) == FALSE)
	{
		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
		return false;
	}
	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);

#endif
	return true;
}

