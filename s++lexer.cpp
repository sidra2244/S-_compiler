// S++ Lexer
// Complete rewrite with standard C++ headers

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
using namespace std;

// Tokens of the S++ language
enum Tokens
{
    // program starting token
    smainTok,
    
    // i/o statements
    sinTok, soutTok,
    
    // relational operators
    sltTok, sgtTok, sseeTok, sleTok, sgeTok, sneTok,
    
    // arithmetic operators
    splusTok, sminTok, smulTok, sdivTok, smodTok, spowTok,
    
    // assignment operator
    seqTok,
    
    // data types
    scharTok, sintTok, sfloatTok, sdoubleTok, sstringTok,
    
    // punctuations
    commaTok, seosTok, startParenTok, endParenTok,
    startBraceTok, endBraceTok, startSquareTok, endSquareTok,
    
    // logical operators
    sandTok, sorTok, snotTok,
    
    // bitwise operators
    sbaTok, sboTok, sbnTok, sbrsTok, sblsTok,
    
    // Identifier (starts with @)
    idenTok,
    
    // constant declaration
    sconstTok,
    
    // conditional statements
    sifTok, selifTok, selseTok, sswitchTok, scaseTok, sdefaultTok,
    
    // loops
    sforTok, swhileTok, sdoTok,
    
    // jump statements
    sjumTok,
    
    // constants
    charConstTok, intConstTok, realConstTok, stringConstTok,
    
    // end of file token
    eofTok,
    
    // illegal character
    illegalTok
};

// Structure for reserved words
struct resWords
{
    char name[12];
    Tokens tok;
};

// Table for reserved words in S++ (all start with 's')
resWords RWtable[] =
{
    {"sbreak", sjumTok},
    {"scase", scaseTok},
    {"schar", scharTok},
    {"sconst", sconstTok},
    {"scontinue", sjumTok},
    {"sdefault", sdefaultTok},
    {"sdo", sdoTok},
    {"sdouble", sdoubleTok},
    {"selif", selifTok},
    {"selse", selseTok},
    {"sfloat", sfloatTok},
    {"sfor", sforTok},
    {"sif", sifTok},
    {"sint", sintTok},
    {"smain", smainTok},
    {"sreturn", sjumTok},
    {"sin", sinTok},
    {"sout", soutTok},
    {"sstring", sstringTok},
    {"sswitch", sswitchTok},
    {"swhile", swhileTok}
};

// Global variables
ifstream infile;
char ch = ' ';
Tokens token;
int lineNo = 1, tempLineNo;
const int ID_LENGTH = 16;
char idenValue[ID_LENGTH + 1];
const int NSRW = 21;  // number of reserved words
const int LSRW = 10;
const int STR_LENGTH = 200;
char stringValue[STR_LENGTH + 1];
char chValue;
int intValue;
float floatValue;
int stringLen = 0;
char illegalLexeme;

// Global functions
void nextCh();
void nextToken();
void evaluateNumber();
void evaluateChar();
void evaluateString();
void isRSW(char[]);
void printTokens(Tokens);

// Main program
int main()
{
    char sourceFile[100];
    cout << "Enter path of the source text program: ";
    cin >> sourceFile;
    
    infile.open(sourceFile);
    if (!infile)
    {
        while (!infile)
        {
            cout << "Entered path does not exist, Please re-enter: ";
            cin >> sourceFile;
            infile.open(sourceFile);
        }
    }
    
    cout << "\n\n  List of Tokens for S++ Language" << endl << endl;
    cout << "Lexemes                    Tokens" << endl << endl;
    
    while (ch != '\0')
    {
        nextToken();
        printTokens(token);
    }
    
    infile.close();
    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();
    
    return 0;
}

void nextCh()
{
    infile.get(ch);
    if (infile.eof())
        ch = '\0';
    else if (ch == '\n')
        lineNo++;
}

// Definition of nextToken() for S++
void nextToken()
{
    while (isspace(ch))    // skip spaces, tabs, newlines
        nextCh();

    // Single line comment $$
    if (ch == '$')
    {
        nextCh();
        if (ch == '$')
        {
            // Single line comment - skip until end of line
            while (ch != '\n' && ch != '\0')
                nextCh();
            nextToken();
            return;
        }
        else if (ch == '*')
        {
            // Multi-line comment $* ... *$
            int commentStartLine = lineNo;
            nextCh();
            char prevCh = ' ';
            while (ch != '\0')
            {
                if (prevCh == '*' && ch == '$')
                {
                    nextCh();
                    break;
                }
                prevCh = ch;
                nextCh();
            }
            if (ch == '\0')
            {
                cout << "\nUnexpected end of file in comment started on line "
                     << commentStartLine << endl;
            }
            nextToken();
            return;
        }
        else
        {
            // $ alone is illegal
            token = illegalTok;
            illegalLexeme = '$';
            return;
        }
    }
    
    // Identifier: must start with @
    else if (ch == '@')
    {
        nextCh();
        if (isalpha(ch) || isdigit(ch))
        {
            int i = 0;
            idenValue[i] = '@';
            i++;
            idenValue[i] = ch;
            idenValue[i + 1] = '\0';
            nextCh();
            
            while (isalpha(ch) || isdigit(ch))
            {
                i++;
                idenValue[i] = ch;
                idenValue[i + 1] = '\0';
                nextCh();
            }
            token = idenTok;
        }
        else
        {
            // @ followed by invalid character
            token = illegalTok;
            illegalLexeme = '@';
        }
    }
    
    // Check for keywords (start with s)
    else if (ch == 's')
    {
        char tempStr[ID_LENGTH + 1];
        int i = 0;
        tempStr[i] = ch;
        i++;
        nextCh();
        
        while (isalpha(ch))
        {
            tempStr[i] = ch;
            i++;
            nextCh();
        }
        tempStr[i] = '\0';
        
        // Check if it's a reserved word
        int found = 0;
        for (int j = 0; j < NSRW; j++)
        {
            if (strcmp(tempStr, RWtable[j].name) == 0)
            {
                token = RWtable[j].tok;
                strcpy(idenValue, tempStr);
                found = 1;
                break;
            }
        }
        if (!found)
        {
            // Not a keyword - but S++ identifiers must start with @
            token = illegalTok;
            illegalLexeme = tempStr[0];
        }
    }
    
    // Numbers
    else if (isdigit(ch))
    {
        token = intConstTok;
        evaluateNumber();
    }
    
    else
        switch (ch)
        {
            // End of statement - full stop (.)
            case '.':
                token = seosTok;
                nextCh();
                break;
            
            case ',':
                token = commaTok;
                nextCh();
                break;
            
            case '\'':
                evaluateChar();
                break;
            
            case '\"':
                token = stringConstTok;
                nextCh();
                evaluateString();
                nextCh();
                break;
            
            case '(':
                token = startParenTok;
                nextCh();
                break;
            
            case ')':
                token = endParenTok;
                nextCh();
                break;
            
            case '{':
                token = startBraceTok;
                nextCh();
                break;
            
            case '}':
                token = endBraceTok;
                nextCh();
                break;
            
            case '[':
                token = startSquareTok;
                nextCh();
                break;
            
            case ']':
                token = endSquareTok;
                nextCh();
                break;
            
            // Logical AND &&
            case '&':
                nextCh();
                if (ch == '&')
                {
                    token = sandTok;
                    nextCh();
                }
                else
                {
                    token = sbaTok;
                }
                break;
            
            // Logical OR ||
            case '|':
                nextCh();
                if (ch == '|')
                {
                    token = sorTok;
                    nextCh();
                }
                else
                {
                    token = sboTok;
                }
                break;
            
            case '!':
                token = snotTok;
                nextCh();
                break;
            
            case '~':
                token = sbnTok;
                nextCh();
                break;
            
            // Assignment and equality
            case '=':
                token = seqTok;
                nextCh();
                if (ch == '=')
                {
                    token = sseeTok;
                    nextCh();
                }
                break;
            
            // Relational operators
            case '<':
                token = sltTok;
                nextCh();
                if (ch == '=')
                {
                    token = sleTok;
                    nextCh();
                }
                else if (ch == '<')
                {
                    token = sblsTok;
                    nextCh();
                }
                break;
            
            case '>':
                token = sgtTok;
                nextCh();
                if (ch == '=')
                {
                    token = sgeTok;
                    nextCh();
                }
                else if (ch == '>')
                {
                    token = sbrsTok;
                    nextCh();
                }
                break;
            
            case '*':
                token = smulTok;
                nextCh();
                break;
            
            case '/':
                token = sdivTok;
                nextCh();
                break;
            
            case '+':
                token = splusTok;
                nextCh();
                break;
            
            case '-':
                token = sminTok;
                nextCh();
                break;
            
            case '%':
                token = smodTok;
                nextCh();
                break;
            
            case '^':
                token = spowTok;
                nextCh();
                break;
            
            case '\0':
                token = eofTok;
                break;
            
            default:
                illegalLexeme = ch;
                token = illegalTok;
                nextCh();
        }
}

// Definition of evaluateNumber()
void evaluateNumber()
{
    intValue = ch - 48;
    nextCh();
    while (isdigit(ch))
    {
        intValue = intValue * 10 + (ch - 48);
        nextCh();
    }
    if (ch == '.')
    {
        token = realConstTok;
        floatValue = (float)intValue;
        nextCh();
        for (int i = 10; isdigit(ch); i *= 10)
        {
            floatValue = floatValue + (float)(ch - 48) / i;
            nextCh();
        }
    }
}

// Definition of evaluateChar()
void evaluateChar()
{
    tempLineNo = lineNo;
    nextCh();
    chValue = ch;
    nextCh();
    if (ch != '\'')
    {
        cout << "\nUnterminated character constant on line " << tempLineNo << "\n\n";
        while (ch != '\n')
            nextCh();
        nextToken();
    }
    else
    {
        token = charConstTok;
        nextCh();
    }
}

// Definition of evaluateString()
void evaluateString()
{
    tempLineNo = lineNo;
    int i = 0;
    for (i = 0; i < STR_LENGTH && ch != '\"' && ch != '\n'; i++)
    {
        stringValue[i] = ch;
        nextCh();
    }
    stringValue[i] = '\0';
    stringLen = i + 1;
    if (ch == '\n')
        cout << "\n    Unterminated string constant starting on line "
             << tempLineNo << "\n\n";
}

// Definition of isRSW(char[])
void isRSW(char item[LSRW + 1])
{
    int found = 0;
    int first = 0;
    int loc;
    int last = NSRW - 1;
    while (first <= last && !found)
    {
        loc = (first + last) / 2;
        if (strcmp(item, RWtable[loc].name) < 0)
            last = loc - 1;
        else if (strcmp(item, RWtable[loc].name) > 0)
            first = loc + 1;
        else
        {
            token = RWtable[loc].tok;
            found = 1;
        }
    }
}

// Definition of printTokens()
void printTokens(Tokens t)
{
    switch (t)
    {
        case smainTok:
            cout << idenValue << "                    smainTok" << endl;
            break;
        case sinTok:
            cout << idenValue << "                    sinTok" << endl;
            break;
        case soutTok:
            cout << idenValue << "                    soutTok" << endl;
            break;
        case sltTok:
            cout << "<" << "                    sltTok" << endl;
            break;
        case sgtTok:
            cout << ">" << "                    sgtTok" << endl;
            break;
        case sseeTok:
            cout << "==" << "                    sseeTok" << endl;
            break;
        case sleTok:
            cout << "<=" << "                    sleTok" << endl;
            break;
        case sgeTok:
            cout << ">=" << "                    sgeTok" << endl;
            break;
        case sneTok:
            cout << "!=" << "                    sneTok" << endl;
            break;
        case splusTok:
            cout << "+" << "                    splusTok" << endl;
            break;
        case sminTok:
            cout << "-" << "                    sminTok" << endl;
            break;
        case smulTok:
            cout << "*" << "                    smulTok" << endl;
            break;
        case sdivTok:
            cout << "/" << "                    sdivTok" << endl;
            break;
        case smodTok:
            cout << "%" << "                    smodTok" << endl;
            break;
        case spowTok:
            cout << "^" << "                    spowTok" << endl;
            break;
        case seqTok:
            cout << "=" << "                    seqTok" << endl;
            break;
        case scharTok:
            cout << idenValue << "                    scharTok" << endl;
            break;
        case sintTok:
            cout << idenValue << "                    sintTok" << endl;
            break;
        case sfloatTok:
            cout << idenValue << "                    sfloatTok" << endl;
            break;
        case sdoubleTok:
            cout << idenValue << "                    sdoubleTok" << endl;
            break;
        case sstringTok:
            cout << idenValue << "                    sstringTok" << endl;
            break;
        case commaTok:
            cout << "," << "                    commaTok" << endl;
            break;
        case seosTok:
            cout << "." << "                    seosTok" << endl;
            break;
        case startParenTok:
            cout << "(" << "                    startParenTok" << endl;
            break;
        case endParenTok:
            cout << ")" << "                    endParenTok" << endl;
            break;
        case startBraceTok:
            cout << "{" << "                    startBraceTok" << endl;
            break;
        case endBraceTok:
            cout << "}" << "                    endBraceTok" << endl;
            break;
        case startSquareTok:
            cout << "[" << "                    startSquareTok" << endl;
            break;
        case endSquareTok:
            cout << "]" << "                    endSquareTok" << endl;
            break;
        case sandTok:
            cout << "&&" << "                    sandTok" << endl;
            break;
        case sorTok:
            cout << "||" << "                    sorTok" << endl;
            break;
        case snotTok:
            cout << "!" << "                    snotTok" << endl;
            break;
        case sbaTok:
            cout << "&" << "                    sbaTok" << endl;
            break;
        case sboTok:
            cout << "|" << "                    sboTok" << endl;
            break;
        case sbnTok:
            cout << "~" << "                    sbnTok" << endl;
            break;
        case sbrsTok:
            cout << ">>" << "                    sbrsTok" << endl;
            break;
        case sblsTok:
            cout << "<<" << "                    sblsTok" << endl;
            break;
        case idenTok:
            cout << idenValue << "                    idenTok" << endl;
            break;
        case sconstTok:
            cout << idenValue << "                    sconstTok" << endl;
            break;
        case sifTok:
            cout << idenValue << "                    sifTok" << endl;
            break;
        case selifTok:
            cout << idenValue << "                    selifTok" << endl;
            break;
        case selseTok:
            cout << idenValue << "                    selseTok" << endl;
            break;
        case sswitchTok:
            cout << idenValue << "                    sswitchTok" << endl;
            break;
        case scaseTok:
            cout << idenValue << "                    scaseTok" << endl;
            break;
        case sforTok:
            cout << idenValue << "                    sforTok" << endl;
            break;
        case swhileTok:
            cout << idenValue << "                    swhileTok" << endl;
            break;
        case sdoTok:
            cout << idenValue << "                    sdoTok" << endl;
            break;
        case sdefaultTok:
            cout << idenValue << "                    sdefaultTok" << endl;
            break;
        case sjumTok:
            cout << idenValue << "                    sjumTok" << endl;
            break;
        case stringConstTok:
            if (stringLen == 1)
                cout << "null";
            else
                for (int i = 0; i < stringLen - 1; i++)
                    cout << stringValue[i];
            cout << "                    stringConstTok" << endl;
            break;
        case charConstTok:
            cout << chValue << "                    charConstTok" << endl;
            break;
        case intConstTok:
            cout << intValue << "                    intConstTok" << endl;
            break;
        case realConstTok:
            cout << floatValue << "                    realConstTok" << endl;
            break;
        case eofTok:
            cout << "\\0" << "                    eofTok" << endl;
            break;
        case illegalTok:
            cout << illegalLexeme << "                    illegalTok" << endl;
            break;
        default:
            break;
    }
}
