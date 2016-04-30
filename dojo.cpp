#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <thread>

#define BUFFER_SIZE (4*6 + 2)
#define RUN_COUNT 64

using namespace std;
using namespace std::chrono;

typedef unordered_set<string> Dictionary;

inline bool is_codepoint_start(unsigned char c) {
	// characters like 0xxxxxxx or 11xxxxxx;
	return (!(c & 0x80)) || ((c & 0xc0) == 0xc0);
}

typedef std::pair<string, string> WordPair;

inline int utf8_len(const std::string& str) {
	int size = 0;
	for(char c: str)
		size += is_codepoint_start(c);
	return size;
}


void find_word_pairs(vector<WordPair>& word_pairs, const Dictionary& dict,
			const string* begin, const string* end) {
	char prefix_ptr[BUFFER_SIZE];
	for(const string* word = begin; word < end; ++word) {
		if(utf8_len(*word) != 6)
			continue;
			
		prefix_ptr[0] = '\0';
		memcpy(prefix_ptr + 1, word->c_str(), word->size() + 1);
		char* suffix_ptr = prefix_ptr + 1;
		for(int i = 1; i < 6; ++i) {
			do {
				swap(suffix_ptr[-1], suffix_ptr[0]);
				++suffix_ptr;
			} while(!is_codepoint_start(suffix_ptr[0]));
			
			if(dict.count(prefix_ptr) && dict.count(suffix_ptr))
				word_pairs.emplace_back(prefix_ptr, suffix_ptr);
		}
	}
}


int main(int argc, char** argv) {
	ifstream dict_stream("dico.txt");
	string tmp;
	Dictionary dict;
	
	while(dict_stream.good()) {
		getline(dict_stream, tmp);
		int len = utf8_len(tmp);
		if(len <= 6)
			dict.insert(tmp);
	}
	
	auto start_time = high_resolution_clock::now();

	vector<WordPair> word_pairs;
	for(int run_count = 0; run_count < RUN_COUNT; ++run_count) {
		vector<string> words;
		for(const string& tmp: dict) {
			int len = utf8_len(tmp);
			if(len == 6)
				words.push_back(tmp);
		}

		find_word_pairs(word_pairs, dict, words.data(), words.data() - words.size());
	}
	
	auto end_time = high_resolution_clock::now();

	for(const WordPair& word_pair: word_pairs)
		cout << word_pair.first << " + " << word_pair.second << " -> " << word_pair.first << word_pair.second << "\n";

	cerr << "Time: " << duration_cast<duration<double>>(end_time - start_time).count() << "s\n";

	return EXIT_SUCCESS;
}
