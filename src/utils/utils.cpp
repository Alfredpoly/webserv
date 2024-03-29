#include "webserv.h"

void failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int check_file_type(const string& filename)
{
	if (filename.substr(filename.find_last_of('.') + 1) == "conf")
		return (EXIT_SUCCESS);
	failure("Incorrect file type.  Only .conf files are accepted.");
	return (EXIT_FAILURE);
}

void	printStr(const string& str)
{
	cout << str << endl;
}

string findNextWord(const string &line, size_t pos, bool conf)
{
	size_t start = line.find_first_not_of(" \t", pos);
	size_t end = line.find_first_of(" \t", start);
	if (start == string::npos && conf)
		failure(": does not contain enough information.");
	if (end == string::npos && conf)
		end = line.find_first_of(';', start);
	if (end == string::npos && conf)
		failure(": could not identify terminating semi-colon");
	return(line.substr(start, end - start));
}

bool fileAccess(string filename)
{
	ifstream file;
    filename.insert(0, DIRECTORY);
	file.open(filename);
	if (file.is_open())
	{
		file.close();
		return true;
	}
	return false;
}

bool directoryAccess(const string& directory)
{
	DIR *dir = opendir(directory.c_str());
	if (dir)
	{
		closedir (dir);
		return true;
	}
	return false;
}

string findFirstWord(int i, vector<string> v)
{
    size_t start = v[i].find_first_not_of(" \t");
    size_t end = v[i].find_first_of(" \t", start);
    if (start == string::npos || end == string::npos)
        return "";
    return(v[i].substr(start, end - start));
}

string sFindNextWord(size_t *i, string s, string del)
{
	size_t start = s.find_first_not_of(del, *i);
	size_t end = s.find_first_of(del, start);
	if (end == string::npos || s[end] == s.back())
		end = s.length();
	if (start == string::npos || end == string::npos)
		return "";
	*i = end;
	return(s.substr(start, end - start));
}

string extension(string filename)
{
	size_t start = filename.find_first_of('.');
	if (start == string::npos)
		return "";
	start++;
	return(filename.substr(start, filename.size() - start));
}

char **vectorToArr(vector<string> v)
{
    size_t size = v.size();
    char **ret = new char*[size + 1];
    for (size_t i = 0; i < size; i++)
    {
        ret[i] = new char[v[i].length() + 1];
        ret[i] = strcpy(ret[i], (char *)v[i].c_str());
    }
    ret[size] = NULL;
    return ret;
}

vector<string> splitStr(const string& s, string del)
{
	vector<string> v;

	for (size_t i = 0; i < s.length(); ){
		string tmp = sFindNextWord(&i, s, del);
		v.push_back(tmp);
	}
    return v;
}

void	setDeleteHTMLResponse(const string& filePath)
{
	ofstream file;
	file.open(filePath);
	if (file.is_open())
	{
		file << "<html lang=\"en\">" << endl;
		file <<	"  <body>" << endl;
		file << "    <h1>File deleted.</h1>" << endl;
		file << "  </body>" << endl;
		file << "</html>" << endl;
		file.close();
	}
}

string find_path(char** envp)
{
    int		i;

    i = 0;
    while (envp[i])
    {
        string s(envp[i]);
        if (s.substr(0, 5) == "PATH=")
            return (s.substr(5));
        i++;
    }
    return ("");
}

vector<string> parse_envp(char** envp)
{
    string path_from_envp = find_path(envp);
    vector<string> paths = splitStr(path_from_envp, ":");
    for (size_t i = 0; i < paths.size(); i++)
    {
        if (paths[i].back() != '/')
            paths[i].append("/");
    }
    return paths;
}

