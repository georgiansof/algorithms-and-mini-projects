#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::ifstream;
using std::ostringstream;

enum exception_t { INSUFFICIENT_ARGS = 1, INCORRECT_SYNTAX, MINMAX_RANGE, INCORRECT_ESCAPE, INCORRECT_RANGE };
char exception_msgs[][100] = { "Unknown exception", "Insufficient arguments", "Incorrect syntax", "Times of appearance range min is greater than max", "Escape character wrong command", "Incorrect character range (first > last ascii-wise)" };


string read_file(char* file_name);
string parse_regex(int &start, int end, int &fstart);
int parse_number(int& start);
void AddWhiteSpace(bool* vec);
void AddAlphaNum(bool* vec);
void AddNonAlphaNum(bool* vec);
void AddNonWhiteSpace(bool* vec);
void SimplifyExpression(string& regex);
void case_insensitive();

int debug = 0;
string regex;
string file_contents;
char escapable_chars[] = ".*+?[]()";
bool fallback;

int main(int argc, char** argv) {
    try {
        if (argc < 3) /// executable_name regex file
            throw INSUFFICIENT_ARGS;

        regex = read_file(argv[1]);
        file_contents = read_file(argv[2]);

        if (argc == 4)
            if (strcmp(argv[3], "insensitive") == 0)
                case_insensitive();

        cout << "RegEx: " << regex << "\nOccurencies:\n";

        int start = 0, end = regex.size();
        for (int i = 0; i < file_contents.size(); ++i) {
            fallback = false;
            int i_cpy = i;
            string matched = parse_regex(start, end, i_cpy);
            if (!fallback)
                cout << matched << " @ char no: " << i << '\n';
            start = 0;
        }
    }
    catch (exception_t ex) {
        cout << exception_msgs[ex] << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

string read_file(char* file_name) {
    ifstream input(file_name);
    ostringstream input_stream;
    input_stream << input.rdbuf();
    return input_stream.str();
}

void case_insensitive() {
    for (int i = 0; i < regex.size(); ++i) {
        if (isalpha(regex[i]) && (i - 1 < 0 || regex[i - 1] != '\\'))
            regex[i] = tolower(regex[i]);
    }
    for (int i = 0; i < file_contents.size(); ++i)
        if (isalpha(file_contents[i]))
            file_contents[i] = tolower(file_contents[i]);
}

string parse_regex(int &i, int end, int &k) {
    vector<string> groups;
    groups.push_back(string("dummy")); /// indexing from 1
    string return_string = "";
    string last_matchable_string = "";
    int start = i;
    if (regex[start] == '+' || regex[start] == ')' || regex[start] == ']')
        throw INCORRECT_SYNTAX;
    if (regex[start] == '?' || regex[start] == '*')
        ++i;
    for (; i < end && k < file_contents.size() && !fallback; ++i) {
        if (regex[i] == '(' && (i - 1 < start || regex[i - 1] != '\\')) { /// find subgroup boundaries and find other groups recursively
            int j = i + 1;
            int depth = 0;
            while (j < end) {
                if (regex[j] == '(' && regex[j - 1] != '\\') {
                    ++depth;
                }

                if (regex[j] == ')' && regex[j - 1] != '\\' && depth == 0)
                    break;
                else
                    if (depth != 0 && regex[j] == ')' && regex[j-1] != '\\')
                        --depth;
                ++j;
            }
            if (j == end)
                throw INCORRECT_SYNTAX;
            ++i; /// exclude '(' operator from subgroup string
            string matched_string = parse_regex(i, j, k);
            if (fallback && (j + 1 >= end || (regex[j + 1] != '*' && regex[j + 1] != '?'))) {
                return string("");
            }
            else {
                if(fallback && (j+1 < end && (regex[j+1] == '*' || regex[j+1] == '?')))
                    ++j; /// -> i = pozitia parantezei + 1 (pozitia *?)
                fallback = false;
            }

            i = start = j;
            groups.push_back(matched_string);
            last_matchable_string = matched_string;
            return_string += matched_string;
            continue;
        }

        if (regex[i] == ')' && regex[i - 1] != '\\') /// closed paranthesis already considered, extra means syntax error
            throw INCORRECT_SYNTAX;

        if (file_contents[k] == '\r' && regex[i] != 'r' && regex[i - 1] != '\\') {
            ++k;
            continue; /// Windows CRLF & legacy MacOS CR compatibility measure.
        }
        if (regex[i] == '.' && file_contents[k] != '\n') {// any character matches except \n
            last_matchable_string.clear();
            last_matchable_string += file_contents[k];
            return_string += file_contents[k];
            ++k;
            continue;
        }
        else
            if (regex[i] == '.' && regex[i] == '\n')
            {
                if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                    fallback = true;
                    return string("");
                }
                else
                    if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                        /// return_string += nothing 
                        last_matchable_string.clear();
                        ++i;
                        continue;
                    }
            }

        if (regex[i] == '\\') {
            if (i + 1 >= end)
                throw INCORRECT_SYNTAX; /// escape marker followed by nothing
            ++i;

            if (regex[i] >= '0' && regex[i] <= '9') {
                int capt_number = parse_number(i);
                --i;

                string to_match = groups[capt_number];

                if (to_match == string(""))
                    continue;

                if (k + to_match.size() >= file_contents.size()) {
                    if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                        last_matchable_string.clear();
                        ++i;
                        continue;
                    }
                    else {
                        fallback = true;
                        return string("");
                    }
                }
                    
                if (std::mismatch(to_match.begin(), to_match.end(), file_contents.begin() + k).first == to_match.end()) {
                    return_string += to_match;
                    k += to_match.size();
                    last_matchable_string.clear();
                    last_matchable_string = to_match;
                }
                else {
                    if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                        last_matchable_string.clear();
                        ++i;
                        continue;
                    }
                    else {
                        fallback = true;
                        return string("");
                    }
                }
                continue;
            }

            if (regex[i] == '\\') {
                if (file_contents[k] == '\\') {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }
            }

            if (regex[i] == 't')  /// check for tab
                if (file_contents[k] == '\t') {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }

            if (regex[i] == 'n')  /// check LF
                if (file_contents[k] == '\n') {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }

            if (regex[i] == 'r')  /// check for CR
                if (file_contents[k] == '\r') {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }

            if (regex[i] == 'w') /// is word char
                if (isalnum(file_contents[k]) || file_contents[k] == '_') {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }
            if (regex[i] == 'W') /// is NOT word char
                if (!isalnum(file_contents[k]) && file_contents[k] != '_') {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }
            if (regex[i] == 's') /// is whitespace
                if (isspace(file_contents[k])) {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }
            if (regex[i] == 'S') /// is NOT whitespace
                if (!isspace(file_contents[k])) {
                    return_string += file_contents[k];
                    last_matchable_string.clear();
                    last_matchable_string += file_contents[k];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }

            /// escaping . * + ? [ ] ( ) 
            if (strchr(escapable_chars, regex[i]) != 0)
                if (regex[i] == file_contents[k])
                {
                    return_string += regex[i];
                    last_matchable_string.clear();
                    last_matchable_string += regex[i];
                    ++k;
                    continue;
                }
                else {
                    if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                        fallback = true;
                        return string("");
                    }
                    else
                        if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                            /// return_string += nothing 
                            last_matchable_string.clear();
                            ++i;
                            continue;
                        }
                }

            /// invalid escaped character
            throw INCORRECT_ESCAPE;
        }

        if (regex[i] == '{') {
            int last_chr_pos = i;
            int apmin = 0, apmax = INT_MAX;
            if (i + 1 >= end)
                throw INCORRECT_SYNTAX;
            ++i;
            while (regex[i] == ' ')
                ++i;

            if (isdigit(regex[i]))
                apmin = parse_number(i);
            else
                if (regex[i] != ',')
                    throw INCORRECT_SYNTAX;

            while (regex[i] == ' ')
                ++i;
            if (regex[i] == ',')
                ++i;
            while (regex[i] == ' ')
                ++i;

            if (isdigit(regex[i]))
                apmax = parse_number(i);
            else
                if (regex[i] != '}')
                    throw INCORRECT_SYNTAX;

            while (regex[i] == ' ')
                ++i;

            if (regex[i] != '}')
                throw INCORRECT_SYNTAX;


            if (apmin > apmax)
                throw MINMAX_RANGE;
            string to_match = last_matchable_string; /// character or group to be matched x times
            if (last_matchable_string == string("")) {
                continue;
            }
            int tms;
            for (tms = 1; tms < apmax; ++tms) { /// TODO multiple matches w/ vector<vector<string>>
                if (k + to_match.size() >= file_contents.size())
                    break;
                if (std::mismatch(to_match.begin(), to_match.end(), file_contents.begin() + k).first == to_match.end()) {
                    return_string += to_match;
                    k += to_match.size();
                }
                else
                    break;
            }

            if (tms < apmin) {
                fallback = true;
                return string("");
            }

            for (; tms > apmin; --tms) /// append x-1 times
                last_matchable_string += to_match;
            continue;
        }

        if (regex[i] == '*' || regex[i] == '+' || regex[i] == '?') {
            if (strchr("*+?", regex[i - 1]) != 0)
                throw INCORRECT_SYNTAX; /// two operators cannot be placed near each other without grouping

            int tms;
            if (last_matchable_string.size() > 0)
                tms = 1;
            else
                tms = 0;
            string to_match = last_matchable_string;
            for (int pp = 1; pp <= to_match.size(); ++pp);
            return_string.pop_back();
            int k_bk = k;
            while (true) {
                if (k + to_match.size() >= file_contents.size())
                    break;
                if (std::mismatch(to_match.begin(), to_match.end(), file_contents.begin() + k).first == to_match.end()) {
                    k += to_match.size();
                    ++tms;
                }
                else
                    break;
            }

            if (tms > 1 && regex[i] == '?') {
                k = k_bk + to_match.size();
                last_matchable_string = to_match;
                return_string += to_match;
                continue;
            }

            if (tms == 0 && regex[i] == '+') {
                fallback = true;
                return string("");
            }
            else
                if (tms == 0)
                    last_matchable_string.clear();

            for (int L = tms; L > 1; --L) { /// tms - 1 times
                return_string += to_match;
                last_matchable_string += to_match; /// already has 1 from before
            }

            return_string += to_match;
            continue;
        }

        if (regex[i] == '[') { /// inside [] everything is literal except escaping character (\)
            if (i + 1 >= end || regex[i + 1] == '-')
                throw INCORRECT_SYNTAX;
            bool chr_range[128] = {};
            bool inverse = false;
            /// implement for special characters inside range
            if (regex[i] == '^') {
                inverse = true;
                ++i;
            }


            while (i < end && (regex[i] != ']' && regex[i - 1] != '\\')) {
                if (regex[i] == '-' && regex[i-1] != '\\') {
                    if (i - 1 < start)
                        throw INCORRECT_SYNTAX;
                    char start_chr = regex[i - 1];
                    if (i + 1 >= end)
                        throw INCORRECT_SYNTAX;
                    char end_chr = regex[i + 1];
                    i += 2;
                    if (start_chr > end_chr)
                        throw INCORRECT_RANGE;
                    for (char c = start_chr; c <= end_chr; ++c)
                        chr_range[c] = true;
                }

                if (i < end && regex[i] == '\\') {
                    ++i;
                    if (i >= end)
                        throw INCORRECT_SYNTAX;
                    switch (regex[i]) {
                    case '\\':
                        chr_range[regex[i]] = true;
                        break;
                    case 's':
                        AddWhiteSpace(chr_range);
                        break;
                    case 'w':
                        AddAlphaNum(chr_range);
                        break;
                    case 'W':
                        AddNonAlphaNum(chr_range);
                        break;
                    case 'S':
                        AddNonWhiteSpace(chr_range);
                        break;
                    case 't':
                        chr_range['\t'] = true;
                        break;
                    case '-':
                        chr_range['-'] = true;
                        break;
                    default:
                        throw INCORRECT_ESCAPE;
                    }
                    ++i;
                }

                if (i < end && regex[i] != '\\') {
                    chr_range[regex[i]] = true;
                    ++i;
                }
            }

            if (inverse)
                for (int q = 0; q < 128; ++q)
                    chr_range[q] = !chr_range[q];

            /// verify range in string
            if (chr_range[file_contents[k]]) {
                return_string += file_contents[k];
                last_matchable_string.clear();
                last_matchable_string += file_contents[k];
                ++k;
            }
            else {
                if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                    fallback = true;
                    return string("");
                }
                else
                    if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                        /// return_string += nothing 
                        last_matchable_string.clear();
                        ++i;
                        continue;
                    }
            }
            continue;
        }

        if (file_contents[k] == regex[i]) {
            return_string += file_contents[k];
            last_matchable_string.clear();
            last_matchable_string += file_contents[k];
            ++k;
        }
        else {
            if (i + 1 >= end || (regex[i + 1] != '*' && regex[i + 1] != '?')) {
                fallback = true;
                return string("");
            }
            else
                if (regex[i + 1] == '*' || regex[i + 1] == '?') {
                    /// return_string += nothing 
                    last_matchable_string.clear();
                    ++i;
                    continue;
                }
        }
    }

    if (fallback)
        return string("");

    return return_string;
}

int parse_number(int& i) {
    int number = 0, p = 1;
    for (; isdigit(regex[i]); ++i) {
        number = p * number + regex[i] - '0';
        p *= 10;
    }
    return number;
}

void AddWhiteSpace(bool* vec) {
    for (int c = 0; c <= 127; ++c) {
        if (isspace(c))
            vec[c] = true;
    }
}

void AddAlphaNum(bool* vec) {
    for (int c = 0; c < 128; ++c)
        if (isalnum(c))
            vec[c] = true;
}

void AddNonAlphaNum(bool* vec) {
    for (int c = 0; c < 128; ++c)
        if (!isalnum(c))
            vec[c] = true;
}

void AddNonWhiteSpace(bool* vec) {
    for (int c = 0; c < 128; ++c)
        if (!isspace(c))
            vec[c] = true;
}

/*
void SimplifyExpression(string& regex) { /// transform (EXP){0,x} in ((EXP)?){1,x}
    string new_regex = "";
    for (int i = 0; i < regex.size(); ++i) {
        if (regex[i] == '{') {
            if (i - 1 < 0)
                throw INCORRECT_SYNTAX;

            int j = i + 1;
            while (j == ' ' && j < regex.size()) ++j;

            if (j == regex.size() || regex[j] != '0')
                continue;

            /// modificare
            if (regex[i - 1] != ')') { /// nu e grup
                /*new_regex.pop_back();
                new_regex += "(";
                new_regex += regex[i - 1];
                new_regex += "?";
                new_regex += ")";
                new_regex += '{';
                regex.insert(regex.begin() + (i - 1), '(');
                ++i;
                regex.insert(regex.begin() + i + 1, '?');
                regex.insert(regex.begin() + i + 2, ')');
                continue;
            }
            else { /// e grup
                int depth = 0;
                j = i - 2;
                while (j >= 0) {
                    if (regex[j] == ')' && (j < 1 || regex[j - 1] != '\\')) {
                        ++depth;
                    }

                    if (regex[j] == '(' && (j < 1 || regex[j - 1] != '\\')) {
                        --depth;
                        if (depth == -1)
                            break;
                    }
                    --j;
                }
                if (regex[j] != '(')
                    throw INCORRECT_SYNTAX;

                regex.insert(regex.begin() + j, '(');
                ++i, ++j;
                regex.insert(regex.begin() + i, '?');
                ++i;
                regex.insert(regex.begin() + i, ')');
                ++i;

                while (regex[i] != '0') 
                    ++i;
                regex[i] = '1';
                ++i;
                
                while (regex[i] == ' ')
                    ++i;
                ++i; /// ,
                while (regex[i] == ' ')
                    ++i;

                int icpy = i;
                int old_nr = parse_number(icpy);
                int new_nr = old_nr + 1;
                int old_nr_cnt = 0;
                while (old_nr)
                    old_nr /= 10, ++old_nr_cnt;
                int new_nr_vec[20] = {};
                while (new_nr)
                    new_nr_vec[++new_nr_vec[0]] = new_nr % 10, new_nr /= 10;

                if (new_nr_vec[0] != old_nr_cnt)
                    regex.insert(regex.begin() + i, '0');

                for (int loop = new_nr_vec[0]; loop >= 1; --loop)
                    regex[i++] = new_nr_vec[loop] + '0';
            }
        }
    }
}*/