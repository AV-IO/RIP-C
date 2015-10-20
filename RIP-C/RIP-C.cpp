#include <iostream>
#include <fstream>
#include <filesystem>
#include <locale>
#include <codecvt>

bool is_c_extension(std::string s) {
	for (unsigned long i = s.length() - 2; i > s.length() - 4; --i) {
		if (s[i] == '.' && (s[i + 1] == 'c' || s[i + 1] == 'h')) {
			if (i == s.length() - 2) { return true; } // .c || .h
			if (i == s.length() - 3 && ((s[i + 1] == 'c' && s[i + 2] == 'c') || (s[i + 1] == 'c' && s[i + 2] == 's'))) { return true; } // .cc || .cs
			if (i == s.length() - 4 && ((s[i + 2] == 'p' && s[i + 3] == 'p') || (s[i + 2] == 'x' && s[i + 3] == 'x') || (s[i + 1] == 'c' && s[i + 2] == 'b' && s[i + 3] == 'p'))) { return true; } // .(c/h)pp || .(c/h)xx || .cbp
		}
	}
	return false;
}

int main() {
	std::experimental::filesystem::file_status fStatus;
	std::ifstream i_file;
	std::wofstream o_file;
	for (auto& rdi : std::experimental::filesystem::recursive_directory_iterator(std::experimental::filesystem::current_path())) {
		std::experimental::filesystem::path pth = rdi;
		fStatus = std::experimental::filesystem::status(pth);
		if (std::experimental::filesystem::is_regular_file(fStatus) && is_c_extension(pth.string())) {
			std::experimental::filesystem::rename(pth.string(), pth.string() + "1");
			i_file.open(pth.string() + "1", std::ios::in);
			o_file.open(pth.string(), std::ios::out);
			o_file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
			std::string current_line;
			while (!i_file.eof()) {
				std::getline(i_file, current_line);
				for (unsigned long i = 0; i < current_line.length(); ++i) {
					if (current_line[i] != ';') {
						o_file << current_line[i];
					}
					else {
						o_file << L"\uCDBE";
					}
				}
			}
			i_file.close();
			o_file.close();
		}
	}
	return 0;
}