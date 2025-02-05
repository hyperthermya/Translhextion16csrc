#ifndef _HEX_H
#define _HEX_H

//takes hex string
//returns decimal value of hex string
inline int hex_value(string x)
{
	if(x.length()<= 0)
		return -1;
	x = uppercase(x);
	switch(x[0])
	{
		case '0':return 0;
		case '1':return 1;
		case '2':return 2;
		case '3':return 3;
		case '4':return 4;
		case '5':return 5;
		case '6':return 6;
		case '7':return 7;
		case '8':return 8;
		case '9':return 9;
		case 'A':return 10;
		case 'B':return 11;
		case 'C':return 12;
		case 'D':return 13;
		case 'E':return 14;
		case 'F':return 15;
		default:return -1;
	}
}

inline bool is_hex(string x)
{
	int i = 0;
	for(i = 0;i<x.length();i++)
	{
		if(hex_value(x[i]) == -1)
			return false;
	}
	return true;
}

//takes int or unsigned character
//returns hex value of character
inline string hex_char(int c)
{
	string a;
	char cBuf[6];
	sprintf(cBuf, "%02X ", (unsigned char) c); 
	//a = (string) cBuf;
	a += cBuf[0];
	a += cBuf[1];
	return a.substr(0,2);
}

//takes hex string
//returns character value of hex string
inline unsigned char char_hex(string hex)
{
	string x = hex.substr(0,1);
	string y = hex.substr(1,1);
	return (unsigned char) (( 16 * hex_value(x) ) + hex_value(y));
}

//takes start, end, buffer, filestream
//returns dumps hex from start to end into buffer
void hex_dump(fstream& fs, vector<string>& data, long start, long end)
{
	unsigned char c;
	int i = start;
	fs.seekg(start,ios::beg);
	while (!fs.eof() && i != end)
	{
		c = fs.get();
		data.push_back(hex_char(c));
		i++;
	}
}

//takes start, end, buffer, filestream
//returns replaces hex from start to end into file from buffer
void hex_overwrite(fstream& fs, vector<string>& data, long start, long end)
{
	int i = start;
	int j = 0;
	fs.seekg(start,ios::beg);
	while (!fs.eof() && i != end && j < data.size())
	{
		fs.put((unsigned char) (char_hex(data[j])));
		i++;
		j++;
	}
}

#endif