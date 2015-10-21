#include <iostream>
#include <fstream>
#include <filesystem>
#include <locale>
#include <codecvt>

// Created by AV-IO
// Program replaces all semi-colons in c/c++/c# filetypes with greek question marks.
// These two characters look identical, but a compiler will not accept a greek question mark.
// Use responsibly (or not, it's up to you)
// It's probably not wise to run this on C:/ or root

bool is_c_extension(std::string s) {
	for (unsigned long i = s.length() - 2; i > s.length() - 4; --i)
		if (s[i] == '.' && (s[i + 1] == 'c' || s[i + 1] == 'h')) {
			if (i == s.length() - 2) { return true; } // .c || .h
			if (i == s.length() - 3 && ((s[i + 1] == 'c' && s[i + 2] == 'c') || (s[i + 1] == 'c' && s[i + 2] == 's'))) { return true; } // .cc || .cs
			if (i == s.length() - 4 && ((s[i + 2] == 'p' && s[i + 3] == 'p') || (s[i + 2] == 'x' && s[i + 3] == 'x') || (s[i + 1] == 'c' && s[i + 2] == 'b' && s[i + 3] == 'p'))) { return true; } // .(c/h)pp || .(c/h)xx || .cbp
		}
	return false;
}

int main(int argc, char *argv[]) {
	if(argc > 1 && std::string(argv[1]) != "-r") {
		std::cout << "\n Run without any arguments to convert from normal to unicode\n Run with \"-r\" to reverse the process.\n\n Use responsibly (or not, it's up to you)\n";
		return 0;
	}
	std::experimental::filesystem::file_status fStatus;
	std::ifstream i_file; //Original file
	std::wofstream o_file; //Created file
	std::cout << "\n Working...\n (This may take some time depending on the size of the directory you started in)\n\n ";
	for (auto& rdi : std::experimental::filesystem::recursive_directory_iterator(std::experimental::filesystem::current_path())) {
		std::experimental::filesystem::path pth = rdi; //Creating path object from iterator
		fStatus = std::experimental::filesystem::status(pth); //Getting status (for directory/file determination) of current item in directory
		if (std::experimental::filesystem::is_regular_file(fStatus) && is_c_extension(pth.string())) { //If item in directory is a file, and has an extension that coincides with common c/c++/c# file types
			std::experimental::filesystem::rename(pth.string(), pth.string() + "1");
			i_file.open(pth.string() + "1", std::ios::in);
			o_file.open(pth.string(), std::ios::out);
			o_file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>())); //UTF-8 compatible output
			std::string current_line;
			while (!i_file.eof()) {
				std::getline(i_file, current_line);
				for (unsigned long i = 0; i < current_line.length(); ++i)
					if(argc <= 1) //If process is not being reversed:
						current_line[i] != ';' ? o_file << current_line[i] : o_file << L'\uCDBE';
					else //If process is being reversed:
						current_line[i] != L'\uCDBE' ? o_file << current_line[i] : o_file << ';';
			}
			i_file.close();
			o_file.close();
			std::experimental::filesystem::remove(pth.string() + "1"); // deleting original file
		}
	}
	std::cout << "Finished!\n";
	return 0;
}