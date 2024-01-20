#include"mbytesreader.h"
#include"compare.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string>

#define TS(x) std::string(x)

inline int prepare(int argc, char** argv);
inline void extract_docx(const std::string& file1);
inline void cleanup(const std::string& file1, const std::string& file2);
int main(int argc, char** argv)
{
  //1) check input data correctness
	if (prepare(argc, argv) == -1) return -1;
  //2) extract each docx to its own specific dir
	extract_docx(TS(argv[1]));
	extract_docx(TS(argv[2]));
  //3) compute comparision
	Comparerer comp{TS(argv[1]),TS(argv[2])};

	system("clear");
        comp.print();
  //4) clean up all dirs
	cleanup(TS(argv[1]),TS(argv[2]));
	return 0;
}
int prepare(int argc, char** argv)
{
  //check arguments
  
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
  //extract content of .docx files. Since it's just zipped directory full of media and .xml files
  auto command  = "mkdir "+file+".ext && ";
  command      += "cd "+file+".ext && ";
  command      += "unzip ../"+file;
  system(command.c_str());
  system("clear");
}
void cleanup(const std::string& file1, const std::string& file2)
{
  //remove everything that was extracted
  auto command = "rm "+file1+" && rm "+file2;
  system(command.c_str());
  
  command = "rm -rf "+file1+".ext"+ "&& rm -rf "+file2+".ext";
  system(command.c_str());
}
