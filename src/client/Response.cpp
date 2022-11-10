#include "Response.hpp"
// Constructor initializes attributes to 0 by default
Response::Response(const char *errorMessage, int newSockFD)
	: _sockFD(newSockFD), _filePath("../../www/error/"), _head("HTTP/1.1 ") {
	stringstream	ss(errorMessage);
	string			tmp;

	getline(ss, tmp, ' ');
	_filePath.append(tmp);
	_filePath.append(".html");

	ifstream		file(_filePath, ios::binary);
	file.seekg(0, ios::end);
	setFileSize((size_t)file.tellg());
	string size = to_string(getFileSize());
	appendToHead(errorMessage);
	appendToHead("\n");
	appendToHead("Content-Type: ");
	appendToHead(gContentType["html"]);
	appendToHead("\n");
	appendToHead("Content-Length: ");
	appendToHead(size);
	appendToHead("\n");
	appendToHead("\r\n\r");
}

Response::Response(string filePath, string message, string contentType, int newSockFD)
	: _sockFD(newSockFD), _filePath(filePath) {
	(void)file, (void)messsage, (void)contentType;
}

Response&	Response::operator=( const Response& rhs )
{
	this->_sockFD = rhs._sockFD;
	this->_file = rhs._file;
	this->_fileSize = rhs._fileSize;
	this->_head = rhs._head;
	return *this;
}
 
// Output
void Response::output() {
  std::cout << "sockFD : " << _sockFD << std::endl; 
  std::cout << "head : " << _head << std::endl; 
  std::cout << "file : " << _file << std::endl; 
  std::cout << "fileSize : " << _fileSize << std::endl; 
}

// functionality
void Response::sendResponse() {
	int fd = open(getFilePath().c_str, O_RDONLY);
	send(_sockFD, fd, _header.size(), 0);
	sendfile(_sockFD, _file, NULL, getFileSize());
	close(fd);
}


