#include"strcomp.h"
#include"mbytesreader.h"
#include<stdio.h>

int prepare(int argc, char** argv);
int main(int argc, char** argv)
{
	if (prepare(argc, argv) == -1) return -1;


	return 0;
}
int prepare(int argc, char** argv)
{
	if (argc != 4)
	{
		printf("DCX error: incorrect number of arguments! \n Usage example: dcx file1.docx file2.docx convertioapi_key\n");
		return -1;
	}

	if (!is_docx_file(argv[1]))
	{
		printf("DCX error: %s must be a docx file!\n", argv[1]);
		return -1;
	}
	if (!is_docx_file(argv[2]))
	{
		printf("DCX error: %s must be a docx file!\n", argv[2]);
		return -1;
	}

	return 0;
}
