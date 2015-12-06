/*
*	CS342-Project1
*	Oğuz Demir 21201712 Section1
*	
*   This program reads the files according to command line arguments and create and output file and report the
*   the lines which match with given keyword. To sort the linked list, merge sort is used.
*   
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Node
{
	char word[81];
	int number;
	struct Node* next;
};
struct Node* Node_new(char* given,int count)
{
	struct Node* p = malloc(sizeof(struct Node));
	strcpy(p->word,given);
	p->next = NULL;
	p->number = count;
	return p;
}
//Head can change after insertion because it preserves the sorted order while adding
void insert(struct Node** head,char* givenData,int count)
{
	//Creating new node with corresponding data
	struct Node* temp = Node_new(givenData,count);
	struct Node* previousHead = *head;
	*head = temp;	
	temp->next = previousHead;
};


struct Node* merge(struct Node* first, struct Node* second)
{
	if(first == NULL)
		return second;
	if(second == NULL)
		return first;
	struct Node* result;
	//Resulting linked-list head will be the smallest of these two's heads
	int a = strcmp(first->word,second->word);
	if(a < 0)
	{
		result = first;
		first = first->next;
	}
	else if(a == 0)
	{
	    //If names are same
		if(first->number > second->number)
		{	result = second;
			second = second->next;
		}
		else
		{
			result = first;
			first = first->next;
		}
	}
	else
	{
		result = second;
		second = second->next;
	}	
	struct Node* iter = result;

	while(first != NULL && second != NULL)
	{
		int a = strcmp(first->word,second->word);
		if(a < 0)
		{
			iter->next = first;
			first = first->next;
			iter = iter->next;
		}
		else if(a == 0)
		{
			if(first->number > second->number)
			{
				iter->next = second;
				second = second->next;
				iter = iter->next;
			}
			else
			{
				iter->next = first;
				first = first->next;
				iter = iter->next;
			}
		}
		else
		{
			iter->next = second;
			second = second->next;
			iter = iter->next;
		}
	}
	if(first == NULL)
		iter->next = second;
	if(second==NULL)
		iter->next = first;
	return result;
}
void mergeSort(struct Node** given)
{
	struct Node* head = *given;
	if(head == NULL || head->next == NULL)
		return;
	
	//Spliting

	struct Node* firstHalf = head;
	struct Node* backPointer = head;
	struct Node* secondHalf = NULL;
	struct Node* iteration = head->next;
	//To split in to two, middle element should be determined and next pointer of first half's last element should be set to NULL
	//This algorithm splits 2n+1 to first:n and second: n+1 
	while(iteration!=NULL)
	{	
	    //Advance upto next or next next becomes null.
		if(iteration->next == NULL || (iteration->next!=NULL && iteration->next->next == NULL))
		{
		    //Determine the second half's first node and make the first half's last node a ending node
			secondHalf = backPointer->next;
			backPointer->next = NULL;
			break;
		}
		iteration = iteration->next;
		iteration = iteration->next;
		backPointer = backPointer->next;
		//In every iteration, iter should advance 2, backPointer should advance 1 to
	}
	mergeSort(&firstHalf);
	mergeSort(&secondHalf);
	
	*given = merge(firstHalf,secondHalf);
} 

int main(int argc, char *argv[])
{
    //Clock for measuring time
	clock_t begin, end;
	double time_spent;
	begin = clock();
	int n;
	sscanf(argv[2],"%d",&n);
	
	//Input arguments checks
	if(n < 0)
	{
		printf("Number of input files should be positive, program will be terminated.\n");
		return 0;
	}
	if(n > 20)
	{
		printf("Number of input files should be less than 20, program will be terminated.\n");
		return 0;
	}

	if(argc < n + 4)
	{
		printf("There is too few arguments\n");
		return 0;
	}
	if(argc > n + 4)
	{
		printf("There is too much arguments\n");
		return 0;
	}

	int i;	
	
	//A line is 82 character, 80 text except newline, 1 newline and 1 '\0'.
	char line[82];
	FILE* fp;
	

	/* Start children. */
	for(i = 0; i < n ; ++i) {
		FILE* tempfile;
		//Creating files such that out0 to outn-1 
		char temp[10]  = "";
    		sprintf(temp, "out%d", i);
		tempfile = fopen(temp, "w+");
		fclose(tempfile);

		//printf("Tempword: %s\n",tempname);	
		pid_t  a = 0;
		a = fork();
		
		//Child process work
		if (a == 0)
		{
		    //Determine the word for opening input file
			char tempname[127];
			strcpy(tempname,argv[i+3]);	

            //A linked list pointer for storing matching lines
			struct Node *list = NULL;
			fp = fopen(tempname, "r");	
			if(fp== NULL)	//If pointer is null, there is no such file
			{
				printf("File %s is not exist, nothing will be done. \n",tempname );
				exit(0);
			}
			else
			{
				//Count holds the line number
				int count = 1;
				while (fgets(line, sizeof(line),fp) != NULL)	//Loop until end of file
				{					
					int k;	
					for(k= 0; k < strlen(line) ; k++) {
						if(line[k] == '\n')
							line[k] = '\0';
					}
					if( strstr(line, argv[1]))
						insert(&list,line,count);
					count++;
				}
				fclose(fp);
				//Sorting and writing the list
				mergeSort(&list);
				tempfile = fopen(temp, "w+");
				while(list!=NULL)
				{
					fprintf(tempfile,"%s: %s (%d)\n",tempname,list->word,list->number);	
					struct Node* temp = list;
					list = list->next;
					free(temp);
				}	
				fclose(tempfile);		
			}
			exit(0);
		}

	}
	//Waiting for all childs
	int status  = 0;
	for (i = 0; i < n; ++i)
		wait(&status);	

    //Now every result is saved in out file.
    //But out0 corresponds first input file, out1 second and so on.
    //So in a linked list, input file names are stored with their numbers and this list stored according to names
	struct Node* inputNames = NULL;
	for(i = 0; i < n ; i++)
	{
		insert(&inputNames, argv[i+3] , i );		
	}
	mergeSort(&inputNames);

    //Now, in first place of the linked list, there is alphabetically first file with number 5. 
    //So, writing should be started from the out5 file.
    //The while loop takes the number of each node and opens the corresponding number's file.
	FILE *output;
	output = fopen (argv[argc-1],"w+");

	while(inputNames!=NULL){
		i = inputNames->number;
		char temp[10]  = "";
		sprintf(temp, "out%d", i);
		fp = fopen(temp, "r");	

		while (fgets(line, sizeof(line),fp) != NULL)	
		{
			fprintf(output,"%s",line);
		}
		fclose(fp);
		struct Node *tempNode = inputNames;
		inputNames = inputNames->next;
		free(tempNode);
		remove(temp);	
	}
	fclose(output);
	
	//Clock end
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nProgram execution done with %d files in %f \n",n, time_spent);
	return 0;
}
