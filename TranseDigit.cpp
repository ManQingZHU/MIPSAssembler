#include "TranseDigit.h"
#include "Parser.h"


string HexToBin(const string &s)   // don't deal with 0x
{
    string res;
    string::const_iterator iter = s.begin();
    for(; iter != s.end(); ++iter) {
        switch (*iter) {
            case '0':
                res += "0000";
                break;
            case '1':
                res += "0001";
                break;
            case '2':
                res += "0010";
                break;
            case '3':
                res += "0011";
                break;
            case '4':
                res += "0100";
                break;
            case '5':
                res += "0101";
                break;
            case '6':
                res += "0110";
                break;
            case '7':
                res += "0111";
                break;
            case '8':
                res += "1000";
                break;
            case '9':
                res += "1001";
                break;
            case 'a':
            case 'A':
                res += "1010";
                break;
            case 'b':
            case 'B':
                res += "1011";
                break;
            case 'c':
            case 'C':
                res += "1100";
                break;
            case 'd':
            case 'D':
                res += "1101";
                break;
            case 'e':
            case 'E':
                res += "1110";
                break;
            case 'f':
            case 'F':
                res += "1111";
                break;
            default:
                break;
        }
    }
        return res;
}

long int HexToInt(const string& s)  // don't deal with 0x
{
    long int res = 0;
    string::const_iterator iter = s.begin();
    for(; iter != s.end(); ++iter) {
            res *= 16;
            switch (*iter) {
                case '0':
                    res += 0;
                    break;
                case '1':
                    res += 1;
                    break;
                case '2':
                    res += 2;
                    break;
                case '3':
                    res += 3;
                    break;
                case '4':
                    res += 4;
                    break;
                case '5':
                    res += 5;
                    break;
                case '6':
                    res += 6;
                    break;
                case '7':
                    res += 7;
                    break;
                case '8':
                    res += 8;
                    break;
                case '9':
                    res += 9;
                    break;
                case 'a':
                case 'A':
                    res += 10;
                    break;
                case 'b':
                case 'B':
                    res += 11;
                    break;
                case 'c':
                case 'C':
                    res += 12;
                    break;
                case 'd':
                case 'D':
                    res += 13;
                    break;
                case 'e':
                case 'E':
                    res += 14;
                    break;
                case 'f':
                case 'F':
                    res += 15;
                    break;
                default:
                    break;
            }
    }
    return res;
}

string IntToBin(long int i, int n)   // include sign-ext
{
    string res;
    stack<char> s;
    bool Neg = false;
   if(i < 0){
       Neg = true;
       i = -i;
   }
    int cnt = 0;
    char c;
    while(i)
    {
        c = (char)(i%2 + '0');
        s.push(c);
        cnt++;
        i /= 2;
        if(c == '1')
            break;
    }
    if(Neg)
    {
        while(i)
        {
            c = (i%2 == 0) ? '1': '0';
            s.push(c);
            cnt++;
            i /= 2;
        }
        while(cnt != n)
        {
            s.push('1');
            cnt++;
        }
    }
    else{
        while(i)
        {
            c = (char)(i%2 + '0');
            s.push(c);
            cnt++;
            i /= 2;
        }
        while(cnt != n)
        {
            s.push('0');
            cnt++;
        }
    }

    while(!s.empty())
    {
        c = s.top();
        res += c;
        s.pop();
    }

    return res;
}

string UnIntToBin(long int i, int n)
{
    string res;
    stack<char> s;
    int cnt = 0;
    char c;
    while(i)
    {
        c = (char)(i%2 + '0');
        s.push(c);
        cnt++;
        i /= 2;
    }

    while(cnt != n)
    {
        s.push('0');
        cnt++;
    }

    while(!s.empty())
    {
        c = s.top();
        res += c;
        s.pop();
    }

    return res;
}

long int BinToUnInt(string &s)
{
    long int res = 0;
    string::const_iterator iter = s.begin();
    string::const_iterator iter_end = s.end();
    while(iter != iter_end)
    {
        res *= 2;
        res += ((*iter == '1')? 1 : 0);
        iter++;
    }
    return res;
}

int BinToInt(string &s)
{
    int res = 0;
    string::const_iterator iter = s.begin();
    string::const_iterator iter_end = s.end();

    while (iter != iter_end) {
        res *= 2;
        if(s[0] == '1')
            res += ((*iter == '1') ? 0 : 1);
        else res += ((*iter == '1') ? 1 : 0);
        iter++;
    }
    if(s[0] == '1') {
        res = -res;
        res--;
    }
    return res;
}

string IntToStr(int i)
{
    string res;
    if(i == 0)
        return "0";
    stack<char> s;
    bool Neg = false;
    if(i < 0){
        Neg = true;
        i = -i;
    }
    while(i)
    {
        s.push((char)(i%10+'0'));
        i /= 10;
    }
    if(Neg)
        s.push('-');
    while(!s.empty())
    {
        res += s.top();
        s.pop();
    }

    return res;
}

long int GetAddr(string& s)
{
    size_t pos = s.find("x") != string::npos ? s.find("x"): s.find("X");
    s = s.substr(pos+1);
    pos = s.find(";");
    s = s.substr(0, pos);
    return HexToInt(s);
}

string DefByte(string &s)
{
    unsigned long t = s.size() - 2;

    if(s[0] == '\'')
    {
        s = s.substr(1, t);
        string res;
        string::const_iterator iter = s.begin();
        for(; iter != s.end(); ++iter) {
            res += IntToBin((long int)*iter, 8);
        }
        return res;
    }
    else if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s = s.substr(2, t);
    return HexToBin(s);
}

string DefWord(string &s)
{
    unsigned long t = s.size() - 2;

    if(s[0] == '\'')
    {
        s = s.substr(1, t);
        string res;
        string::const_iterator iter = s.begin();
        for(; iter != s.end(); ++iter) {
            res += IntToBin((long int)*iter, 8);
        }
        if(res.size() % 16)
            res += "00000000";
        return res;
    }
    else if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s = s.substr(2, t);
    return HexToBin(s);
}

string DefD(string &s)
{
    unsigned long t = s.size() - 2;

    if(s[0] == '\'')
    {
        s = s.substr(1, t);
        string res;
        string::const_iterator iter = s.begin();
        for(; iter != s.end(); ++iter) {
            res += IntToBin((long int)*iter, 8);
        }
        while(res.size() % 32)
            res += "00000000";
        return res;
    }
    else if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s = s.substr(2, t);
    return HexToBin(s);
}

string BreakCode(string& s)
{
    long int temp = 0;
    if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        s = s.substr(2);
        temp = HexToInt(s);
    }
    else {
        string::const_iterator iter = s.begin();
        for (; iter != s.end(); ++iter) {
            temp *= 10;
            temp += *iter - '0';
        }
    }
    return IntToBin(temp, 20);
}

string Shamt(string &s)
{
    long int temp = 0;
    if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        s = s.substr(2);
        temp = HexToInt(s);
    }
    else {
        string::const_iterator iter = s.begin();
        for (; iter != s.end(); ++iter) {
            temp *= 10;
            temp += *iter - '0';
        }
    }
    return IntToBin(temp, 5);
}

string Imm(string& s)  // hex decimal
{
    long int temp = 0;
    bool Neg = false;
    if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        temp = HexToInt(s.substr(2));
    }
    else {
        string::const_iterator iter = s.begin();
        if (*iter == '-') {
            Neg = true;
            iter++;
        }

        for (; iter != s.end(); ++iter) {
            temp *= 10;
            temp += *iter - '0';
        }
    }
    if(Neg) temp = -temp;
    return IntToBin(temp, 16);
}

string UnImm(string& s)
{
    long int temp = 0;
    if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        temp = HexToInt(s.substr(2));
    }
    else {
        string::const_iterator iter = s.begin();

        for (; iter != s.end(); ++iter) {
            temp *= 10;
            temp += *iter - '0';
        }
    }

    return UnIntToBin(temp, 16);
}

string Parser::BranchOff(string& s, long int pc)
{
    string res = IntToBin(symbolTable[s] - pc- 4, 18);
    return res.substr(0, 16);
}

string Parser::JumpOff(string& s)
{
    string loc = IntToBin(symbolTable[s], 32);
    return loc.substr(4, 26);
}

string LiImm(string& s, int n)
{
    string res;
    long int temp = 0;
    bool Neg = false;
    if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        temp = HexToInt(s.substr(2));
    }
    else {
        string::const_iterator iter = s.begin();
        if (*iter == '-') {
            Neg = true;
            iter++;
        }

        for (; iter != s.end(); ++iter) {
            temp *= 10;
            temp += *iter - '0';
        }
    }
    if(Neg) temp = -temp;
    res = IntToBin(temp, 32);
    return (n == 1) ? res.substr(0, 16): res.substr(16,16);
}

string Parser::LaImm(string& s, int n)
{
    string res =  IntToBin(symbolTable[s], 32);
    return (n == 1) ? res.substr(0, 16): res.substr(16,16);
}

string BreakInt(string& s)
{
    int code = BinToInt(s);
    return IntToStr(code);
}

string ImmInt(string &s)
{
    int res = BinToInt(s);
    return IntToStr(res);
}

string UnIntToHex(long int i)
{
    stack<char> sta;
    while(i)
    {
        switch(i%16)
        {
            case 0: sta.push('0');
                break;
            case 1: sta.push('1');
                break;
            case 2: sta.push('2');
                break;
            case 3: sta.push('3');
                break;
            case 4: sta.push('4');
                break;
            case 5: sta.push('5');
                break;
            case 6: sta.push('6');
                break;
            case 7: sta.push('7');
                break;
            case 8: sta.push('8');
                break;
            case 9: sta.push('9');
                break;
            case 10: sta.push('A');
                break;
            case 11: sta.push('B');
                break;
            case 12: sta.push('C');
                break;
            case 13: sta.push('D');
                break;
            case 14: sta.push('E');
                break;
            case 15: sta.push('F');
                break;
            default:
                break;
        }
        i /= 16;
    }
    string res;
    while(!sta.empty())
    {
        res += sta.top();
        sta.pop();
    }
    return res;
}

string LuiImm(string &s)
{
    int temp = BinToUnInt(s);
    if(temp == 0)
        return "0x0000";
   string res = "0x";
    res += UnIntToHex(temp);
    return res;
}


string CoeToBin(string& s)
{
    string res= s.substr(0, 8);
    return HexToBin(res);
}
