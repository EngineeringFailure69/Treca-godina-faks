#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
using namespace std;

int rez;

void parse(string str)
{
    int posx = str.find("x=");
    int posy = str.find("y=");
    int posa = str.find("&");
	string vrx = str.substr(posx + 2, posa - posx - 2);
	int ivrx = stoi(vrx);
	string vry = str.substr(posy + 2, str.size() - posy - 2);
	int ivry = stoi(vry);

	if (ivrx > ivry) {
		cout << "Content-Type: text/html;charset=us-ascii \n\n";
		cout << "<html> <head> \n";
		cout << "<title> Aprilski </title> \n";
		cout << "</head> \n";
		cout << "<body> \n";
		cout << "<h1>Aprilski_23 cgi </h1> \n";
		cout << "<p> Kliknuo si iznad dijagonale</p>\n";
		cout << "</body> </html> \n";
	}
	else if (ivrx < ivry) {
		cout << "Content-Type: text/html;charset=us-ascii \n\n";
		cout << "<html> <head> \n";
		cout << "<title> Aprilski </title> \n";
		cout << "</head> \n";
		cout << "<body> \n";
		cout << "<h1>Aprilski_23 cgi </h1> \n";
		cout << "<p> Kliknuo si ispod dijagonale</p>\n";
		cout << "</body> </html> \n";
	}
	else {
		cout << "Content-Type: text/html;charset=us-ascii \n\n";
		cout << "<html> <head> \n";
		cout << "<title> Aprilski </title> \n";
		cout << "</head> \n";
		cout << "<body> \n";
		cout << "<h1>Aprilski_23 cgi </h1> \n";
		cout << "<p> Kliknuo si na dijagonali</p>\n";
		cout << "</body> </html> \n";
	}
}

void main() 
{
	string req_string = getenv("REQUEST_METHOD");
	string query_string = getenv("QUERY_STRING");
    string postInputString;

	if (req_string == "GET") 
	{
		if (!query_string.empty())
			parse(query_string);
	}

    else if (req_string == "POST") 
    {
        if (!query_string.empty())
            parse(query_string);
        int br_byte = atoi(getenv("CONTENT_LENGTH"));
        if (br_byte > 0) 
        {
            cin >> postInputString;
            parse(postInputString);
        }
    }
}