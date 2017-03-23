#include "file_helper.h"
/**
 * read file content
 * return buffer length
 */
int readfile(const char *filePath, char buf[], size_t len)
{
	FILE *fp;
	fp = fopen(filePath, "r");
	int n = fread(buf, 1, len, fp);
	fclose(fp);
	return n;
}
