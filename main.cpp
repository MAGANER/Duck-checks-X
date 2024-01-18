#include"strcomp.h"
#include"mbytesreader.h"
#include<stdio.h>
#include<unistd.h>

#define TS(x) std::string(x)

inline int prepare(int argc, char** argv);
int main(int argc, char** argv)
{
  //1) check input data correctness
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
