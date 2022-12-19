#include "Request.hpp"

Request::Request()
        : _buffer(""), _method(""), _dir(""), _protocol(""), _body(""), _requestHeader(true), _requestBody(false) {}

Request::Request( const Request& rhs ) {
    *this = rhs;
}

Request::~Request() {}

Request&	Request::operator=( const Request& rhs )
{
	this->_buffer = rhs._buffer;
    this->_dir = rhs._dir;
    this->_method = rhs._method;
    this->_protocol = rhs._protocol;
    this->_input = rhs._input;
	this->_header = rhs._header;
	this->_body = rhs._body;
	this->_requestHeader = rhs._requestHeader;
	this->_requestBody = rhs._requestBody;

    return *this;
}

bool	Request::appendBuffer(string recvBuffer) {
	_buffer.append(recvBuffer);

	if (this->getRequestHeader() == true) {
		this->parseBufferHeader();
		if (this->getRequestHeader() == false && this->getRequestBody() == false)
			return false;
		return true;
	}
	if (this->getRequestHeader() == false && this->getRequestBody() == true) {
		this->parseBufferBody();
		if (this->getRequestBody() == false)
			return false;
		return true;
	}
	return false;
}

void	Request::parseBufferHeader() {
	stringstream	ss(_buffer);
	string			tmp;
//	cout << "test buffer:" << _buffer << endl;
	while (getline(ss, tmp)) {
		if (tmp.empty())
			continue ;
		else if (tmp.compare("\r") == 0) {
//			cout << "this happens now" << endl;
			this->setupHeader();// buffer moet iets van tmp zijn
			return ;
		}
		else if (tmp.find("\r") == string::npos) {
//			cout << "this is tmp:" << tmp ;
//			cerr << ":inside\n";
			_buffer = tmp;
			return ;
		}
		_input.push_back(tmp);
	}
	_buffer.clear();
}

void	Request::setupHeader() {
	this->setAttributes();
	this->setHeaderContent();
	if (_header.find("Content-Length") != _header.end() && !getMethod().compare("POST"))
		this->setRequestBody(true);
	this->setRequestHeader(false);
	this->output();
//	_input.clear();
	_buffer.clear();
}

void	Request::parseBufferBody() {
	stringstream	ss(_buffer);
	string			tmp;

	while (getline(ss, tmp)) {
		if (tmp.find("\r") == string::npos) {
			_buffer = tmp;
			return ;
		}
		else if (tmp.compare("\r") == 0) {
			this->setRequestBody(false);
			return ;
		}
		_body.append(tmp);
		_body.append("\n");
	}
}
//
//Request::Request(string input) {
//    setInput(input);
//    setAttributes();
//    setContent();
//}
//
//// Setters
//void Request::setInput(string input) {
//    string vecPush;
//    stringstream ss(input);
//
//    while (getline(ss, vecPush, '\n')) {
//        _input.push_back(vecPush);
//    }
//}

void	Request::setAttributes() {
	stringstream 	ss(_input[0]);
	string 			tmp;

	getline(ss, tmp, ' ');
	if (tmp.empty() || (tmp.compare("GET") && tmp.compare("POST") && \
	tmp.compare("DELETE"))) {
		throw WSException::MethodNotAllowed();
	}
	setMethod(tmp);
	getline(ss, tmp, ' ');
	if (tmp.empty())
		throw WSException::BadRequest();
	setDir(tmp);
	getline(ss, tmp);
	if (tmp.empty())
		throw WSException::BadRequest();
	if (tmp.compare("HTTP/1.1\r")) {
		cerr << " http version "  << tmp << endl;
		throw WSException::HTTPVersionNotAvailable();
	}
	setProtocol(tmp);
}

void	Request::setHeaderContent() {
    string	key;
    string	value;
    size_t	idx;
    size_t	i = 1;
    size_t	size = _input.size();

    while (i < size) { // && _input[i].compare(0, 1,"\n")) {
        cout << "setHeaderContent:" << _input[i] << endl;
		idx = _input[i].find(':', 0);
        if (idx == string::npos)
            break ;
        key = _input[i].substr(0, idx);
        value = _input[i].substr(idx + 2, _input[i].size() - 1);
        setHeaderValue(key, value);
        i++;
    }
}

// Output
void Request::output() {
    std::cout << "_method : " << _method << std::endl;
    std::cout << "_dir : " << _dir << std::endl;
    std::cout << "_protocol : " << _protocol << std::endl;
	cout << "_requestHeader : " << _requestHeader << endl;
	cout << "_requestBody : " << _requestBody << endl;
}
