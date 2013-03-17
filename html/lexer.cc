#include <iostream>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

using namespace std;

class lexer {
public:
    enum Lex {Int,Real,String,Char,Chars,Err,Eol,Eof};
    int zint;
    double zreal;
    lexer(const char *);
    ~lexer();
    Lex get();
private:
    int zp1;
    char *zbuf;
    int zbufl;
};

lexer::~lexer() {
    free(zbuf);
}

lexer::lexer(const char * ibuf) {
    zp1=0;
    zbufl = strlen(ibuf)+1;
    zbuf=(char *)malloc(zbufl);
    zbufl--;
}

lexer::Lex lexer::get() {
    int i,c,state=0;
    for (i=zp1; i<=zbufl; i++) {
	c=zbuf[i];
	if (state==0) {
	    if (c=='\n') {
		zp1=i+1;
		return Eol;
	    }
	    else if (c==0) return Eof;
	    else if (isdigit(c)) {
		zint=c-'0';
		state = 2;
	    }
	}
	else if (state==2) {
	    if (isdigit(c)) zint=zint*10+c-'0';
	    else {
		zp1=i+1;
		return Int;
	    }
	}
    }
    return Eol;
}

int main() {
    lexer lex(" 12 123");
    cout << "hey " << lex.get() << endl ;
    return 0;
}
