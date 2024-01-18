#include"strcomp.h"
#include"mbytesreader.h"
#include<stdio.h>
#include<unistd.h>

#define TS(x) std::string(x)

inline int prepare(int argc, char** argv);
inline void extract_docx(const std::string& file1);
int main(int argc, char** argv)
{
  //1) check input data correctness
	if (prepare(argc, argv) == -1) return -1;
  //2) extract each docx to its own specific dir
	extract_docx(TS(argv[1]));
	extract_docx(TS(argv[2]));
       
	return 0;
}
int prepare(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("DCX error: incorrect number of arguments! \n Usage example: dcx file1.docx file2.docx\n");
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
void extract_docx(const std::string& file)
{
  auto command  = "mkdir "+file+".ext && ";
  command      += "cd "+file+".ext && ";
  command      += "unzip ../"+file;
  system(command.c_str());
}
