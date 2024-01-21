# DuckCheckX
Linux based tool to compare .docx files character by character.

# Usage example
```./dcx  file1.docx file2.docx```

# Dependecies
This tool currently uses ```unzip``` package.
It's popular, so it can be easily obtained with any package manager.


# How it works?
This tool extracts contents of both files and compares their differences.
Then it prints computed percentage. It shows the inner structure of each .docx file.
If they have different files, then they are shown. And tool shows version of .docx file.
