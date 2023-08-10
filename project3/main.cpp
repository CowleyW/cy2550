#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <cstring>
#include <cctype>
#include <utility>

using namespace std;

int main(int argc, char **argv) {
	int num_words = 4;
	int num_caps = 0;
	int num_numbers = 0;
	int num_symbols = 0;

	for (int i = 1; i < argc; i += 1) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			cout << "usage: xkcdpwgen [-h] [-w WORDS] [-c CAPS] [-n NUMBERS] [-s SYMBOLS]\n\n"
				<< "Generate a secure, memorable password using the XKCD method\n\n"
				<< "optional arguments:\n\n"
				<< "    -h, --help            show this help message and exit\n"
				<< " 	-w WORDS, --words WORDS\n"
				<< "			      include WORDS words in the password (default=4)\n"
				<< "    -c CAPS, --caps CAPS  capitalize the first letter of CAPS random words\n"
				<< " 			      (default=0)\n"
				<< "    -n NUMBERS, --numbers NUMBERS\n"
				<< "			      insert NUMBERS random numbers in the password\n"
				<< "			      (default=0)\n"
				<< " 	-s SYMBOLS, --symbols SYMBOLS\n"
				<< " 			      insert SYMBOLS random symbols in the password\n"
	                        << "			      (default=0)\n";
			return 0;
		} else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--words") == 0) {
			num_words = stoi(argv[i+1]);
			i += 1;
		} else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--caps") == 0) {
			num_caps = stoi(argv[i+1]);
			i += 1;
		} else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--numbers") == 0) {
			num_numbers = stoi(argv[i+1]);
			i +=1;
		} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--symbols") == 0) {
			num_symbols = stoi(argv[i+1]);
			i +=1;
		} else {
			throw invalid_argument("The given argument was not recognized.");
		}
	}
	
	ifstream file;
	file.open("words.txt");

	string line;
	vector<string> words_list, chosen_words, betweens;

	while (getline(file, line)) {
		words_list.push_back(line);
	}
	
	srand((unsigned)time(NULL));

	// Select the random words
	for (int i = 0; i < num_words; i += 1) {
		int random = rand() % words_list.size();
		chosen_words.push_back(words_list[random]);
		betweens.push_back("");
	}

	// There is one more slot for numbers/symbols to appear than there are words
	betweens.push_back("");

	// Select the words to capitalize
	if (num_caps > num_words) {
		num_caps = num_words;
	}
	
	for (int i = 0; i < num_caps; i += 1) {
		int index = rand() % num_words;
		
		while (true) {
			if (isupper(chosen_words[index][0])) {
				index = rand() % num_words;
			} else {
				chosen_words[index][0] = toupper(chosen_words[index][0]);
				break;
			}
		}
	}

	// Fill the numbers and symbols between
	vector<pair<int, char>> sym_num_weights;
	for (int i = 0; i < num_numbers; i += 1) {
		int num = rand() % 10;
		int weight = rand() % 600;

		bool placed = false;

		for(int k = 0; k < sym_num_weights.size(); k += 1) {
			if (sym_num_weights[k].first < weight) {
				placed = true;
				sym_num_weights.insert(sym_num_weights.begin() + k, make_pair(weight, num + '0'));
				break;
			}
		}

		if (!placed) {
			sym_num_weights.push_back(make_pair(weight, num + '0'));
		}
	}

	// Give Symbols weights
	for (int i = 0; i < num_symbols; i += 1) {
		char symbols[] = "~!@#$%^&*.:;";
		char sym = symbols[rand() % 12];
		int weight = rand() % 600;

		bool placed = false;

		for (int k = 0; k < sym_num_weights.size(); k+= 1) {
			if (sym_num_weights[k].first < weight) {
				placed = true;
				sym_num_weights.insert(sym_num_weights.begin() + k, make_pair(weight, sym));
				break;
			}
		}

		if (!placed) {
			sym_num_weights.push_back(make_pair(weight, sym));
		}
	}

	// Add the symbols and numbers to their respective 'betweens'
	for (pair<int, char> p : sym_num_weights) {
		betweens[p.first / 100].push_back(p.second);
	}

	// Print result
	cout << betweens[0];
	for (int i = 0; i < num_words; i += 1) {
		cout << chosen_words[i] << "" << betweens[i+1];
	}
	cout << std::endl;

	return 0;
}
