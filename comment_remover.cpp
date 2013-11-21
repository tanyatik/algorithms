#include <iostream>
#include <vector>

enum AutomataState {
    OutsideComment = 0,
    ReadSlash,
    ReadSlashBackslash,
    ReadStar,
    ReadStarBackslash,
    InsideCComment,
    InsideCCommentBackslash,
    InsideCppComment,
    StringLiteral,
    StringLiteralEscaping,
    CharLiteral,
    CharLiteralEscaping,
};

void removeComments() {
    AutomataState s = OutsideComment;
    char in;
    std::vector<char> slash_skip;

    while (std::cin.get(in)) {
        switch (s) {
            case OutsideComment: // just plain text/code
                if (in == '/') {
                    s = ReadSlash; // may be a comment
                } else if (in == '\"') {
                    std::cout.put(in);
                    s = StringLiteral; // different rules apply for a string literal
                } else if (in == '\'') {
                    std::cout.put(in);
                    s = CharLiteral;   // differs from StringLiteral only in back jumps
                } else {
                    std::cout.put(in);
                }
                break;

            case ReadSlash:
                if (in == '/') {
                    // skip slash_skip (see below)
                    slash_skip.clear(); // we don't need to print them as they are part of a comment
                    s = InsideCComment;
                } else if (in == '*') {
                    // skip slash_skip (see below)
                    slash_skip.clear(); // the same story
                    s = InsideCppComment;
                } else if (in == '\\') {
                    slash_skip.push_back(in); // keep all backslashes
                    s = ReadSlashBackslash;
                }
                else {
                    std::cout.put('/');
                    // print slash_skip (see below)
                    for (std::vector<char>::iterator iter = slash_skip.begin(); 
                         iter != slash_skip.end(); 
                         ++iter) {
                        std::cout.put(*iter); // print all backslashes in plain code
                    }
                    slash_skip.clear();
                    std::cout.put(in);
                    s = OutsideComment;
                }
                break;

            case ReadSlashBackslash: 
                // We need to track what characters were between first slash
                // and a character that makes us able to determine,
                // whenever it is a comment or not
                // Consider example
                // below this is a comment

/\
\
\
**\
\
\
/

                // Consider another example
                /*
                after that, this won't be a comment!
                
                a = a + 
                /\
                \
                \x;

                */
                // In these examples, we need to track backslashes after first slash before star or x
                slash_skip.push_back(in);
                if (in == '\n') {
                    s = ReadSlash;
                }
                break;

            case InsideCComment:
                if (in == '\n') {
                    std::cout.put(in);
                    s = OutsideComment;
                } else if (in == '\\') {
                    s = InsideCCommentBackslash;
                }
                break;

            case InsideCppComment:
                if (in == '*') {
                    s = ReadStar; // probably comment ends
                }
                break;
            case ReadStar:
                if (in == '/') {
                    s = OutsideComment;
                } else if (in == '\\') {
                    s = ReadStarBackslash; // chain of backslashes
                }
                else {
                    s = InsideCppComment;
                }
                break;

            case ReadStarBackslash:
                if (in == '\n') { // backslashes keep their authority till end of line
                    s = ReadStar; // here we are in a comment so we don't need to store backslashes
                }
                break;

            case InsideCCommentBackslash:
                if (in == '\n') {
                    s = InsideCComment;
                };
                break;

            case StringLiteral:
                if (in == '\"') {
                    std::cout.put(in);
                    s = OutsideComment;
                } else if (in == '\\') {
                    std::cout.put(in);
                    // don't mix up escaping characters in the string and backslashes
                    s = StringLiteralEscaping; 
                } else {
                    std::cout.put(in);
                }
                break;

            case StringLiteralEscaping:
                std::cout.put(in);
                s = StringLiteral;
                break;

            case CharLiteral:
                if (in == '\'') {
                    std::cout.put(in);
                    s = OutsideComment;
                } else if (in == '\\') {
                    std::cout.put(in);
                    // again, don't mix up escaping characters in the char literal and backslashes
                    s = CharLiteralEscaping;
                } else {
                    std::cout.put(in);
                }
                break;

            case CharLiteralEscaping:
                std::cout.put(in);
                s = CharLiteral;
                break;

            default:
                break; // never reach here
        }
    }
    if (!slash_skip.empty()) {
        std::cout.put('/');
        // print slash_skip in the rest of file
        // actually I suppose that can't happen in correct C++ code
        for (std::vector<char>::iterator iter = slash_skip.begin(); 
                iter != slash_skip.end(); 
                ++iter) {
            std::cout.put(*iter);
        }
        slash_skip.clear();
    }
}

int main() {
    removeComments();
    return 0;
}

