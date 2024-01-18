#ifndef MBYTESREADER_H
#define MBYTESREADER_H
#include<string>
#include<stdio.h>

static int read_file_magic_bytes(const char* f)
{
	FILE* fp = fopen(f, "rb");
	if (fp == NULL)
	{
		printf("DCX error: unable to read %s's magic bytes!\n", f);
		return -1;
	}
	unsigned char magic[4];
	if (fread((void*)magic, 1, 4, fp) != 4)
	{
		printf("DCX error: failed to read %s's magic bytes!\n", f);
		fclose(fp);
		return -1;
	}
	fclose(fp);


	int summ = 0;
	for (size_t i = 0; i < 4; ++i)summ += int(magic[i]);
	return summ;
}

extern bool is_docx_file(const char* f)
{
	return read_file_magic_bytes(f) == 162;
}


#endif