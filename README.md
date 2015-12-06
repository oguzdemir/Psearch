# Psearch
The program reads a bunch of files for given keyword in a multi processed manner and reports the matching lines in a report file.

For each file, a child process is created. Each child process saves its report on temporary files. Main process delete them after read operation.
The report file is sorted by filename and the first character of the lines. For storing the matching
lines, linked list is used and the list is sorted with merge sort at the end.

To run the program, file names and keyword should be given via command line. 
For example : /psearch hello 2 in1 in2 out 
-hello is the keyword
-2 is the number of input files (it can be up to 20)
-in1 and in2 are the names of input files
-out is the name of output file

