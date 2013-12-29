#include <stdio.h>

//void

int main() {
    printf("Content-type: text/html; charset=utf-8\n\n");
    printf(
	"<!DOCTYPE HTML>\n"
	"<html>\n"
	"<head>\n"
	"<meta http_equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n"
	"<title>\n"
	"Test Page\n"
	"</title>\n"
    );
    printf(
	"</head>\n"
	"<body>\n"
	"<h2>Hey Now!</h2>\n"
	"</body>\n"
	"</html>\n"
    );
    return 0;
}
