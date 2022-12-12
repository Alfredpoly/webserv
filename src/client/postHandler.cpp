#include "Client.hpp"
#include <iostream>
#include <fstream>

void Client::parsePostPlainRequest(Request clientReq)
{
	string	req;
	int i = 0;
	stringstream ss(clientReq.getFile());

	while (getline(ss, req, '\n')) {
		_postContent.push_back(vector<string>());
		_postContent[i].push_back(req.substr(0, req.find("=")));
		_postContent[i].push_back(req.substr(req.find("=") + 1, req.length()));
		i++;
	}
	while (i-- > 0)
		createFileStorePost(i);
}

void Client::decryptWwwForm(string &data)
{
	size_t pos;

	while ((pos = data.find("+")) != std::string::npos) {
		data.replace(pos, 1, " ");
	}
	while ((pos = data.find("%")) != std::string::npos) {
		if (pos <= data.length() - 3) {
			char replace[2] = {(char)(std::stoi("0x" + data.substr(pos+1,2), NULL, 16)), '\0'};
			data.replace(pos, 3, replace);
		}
	}
	while ((pos = data.find("&")) != std::string::npos) {
		data.replace(pos, 1, "\n");
	}
}

void Client::parsePostWwwRequest(Request clientReq)
{
	string	req;
	int i = 0;
	string data = clientReq.getFile();
	decryptWwwForm(data);
	stringstream ss(data);

	while (getline(ss, req, '\n')) {
		_postContent.push_back(vector<string>());
		_postContent[i].push_back(req.substr(0, req.find("=")));
		_postContent[i].push_back(req.substr(req.find("=") + 1, req.length()));
		i++;
	}
	while (i-- > 0)
		createFileStorePost(i);
}

void Client::makeMapOfMultipartHeader(string tmp, int content_nb)
{
	string s;

	if (tmp.find(": ") < tmp.find("="))
		setHeaderMultipartValue(tmp.substr(0, tmp.find(":")), tmp.substr(tmp.find(":") + 2, tmp.length()),
								content_nb);
	if (tmp.find("=") < tmp.find(": "))
	{
		cerr << tmp << tmp.find("=") << " " << tmp.length() <<endl;
		s = tmp.substr(tmp.find("=") + 1, tmp.length());
		s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
		setHeaderMultipartValue(tmp.substr(0, tmp.find("=")), s, content_nb);
	}
}

void Client::parsePostMultipartRequest(Request clientReq)
{
	string data = clientReq.getFile();
	stringstream ss(data);
	int	content_nb = -1;
	int contentLoop = 0;
	string	req;
	string	tmp;
	string	fileContent;
	size_t pos = 0;

	while (getline(ss, req, '\n') ) {
		if (contentLoop == 0 && req.compare("--" + clientReq.getContentValue("boundary")) == 0) {
			content_nb++;
		}
		else if (contentLoop == 0)
		{
			_headerMultipart.push_back( map<string, string>() );
			if (req.compare("\n") != 0)
			{
				tmp = req;
				while ((pos = req.find(";")) != string::npos) {
					tmp = req.substr(0, pos);
					makeMapOfMultipartHeader(tmp, content_nb);
					req.erase(0, pos + 2);
				}
				cerr << req;
				makeMapOfMultipartHeader(req, content_nb);
			}
		}
		if (req.length() == 1 && contentLoop == 0)
		{
			contentLoop = 1;
			continue;
		}
		if (req.compare("--" + clientReq.getContentValue("boundary") + "--") == 0)
		{
			contentLoop = 0;
			break;
		}
		if (req.compare("--" + clientReq.getContentValue("boundary")) == 0)
		{
			if (fileContent.length() != 0)
			{
				ofstream outfile (_headerMultipart[content_nb]["name"]);
				outfile << fileContent;
				outfile.close();
				fileContent.clear();
			}
			contentLoop = 0;
		}
		if (contentLoop == 1) {
			fileContent = fileContent + req + "\n";
		}
	}
}

void Client::createFileStorePost(int i)
{
	ofstream outfile (_postContent[i][0]);

	outfile << _postContent[i][1];
	outfile.close();
}
