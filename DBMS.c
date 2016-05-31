/*input: read book.txt, sellRecord.txt, total 8 attributes, hash to 8 hash tables, each has 10 buckets.
output: 8 files of hash table.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
	char *isbn;
	char *author;
	char *title;
	char *price;
	char *subject;
}EntryBook;

typedef struct
{
	char *uid;
	char *no;
	char *isbn_no;
} EntrySellRecord;

int countlines(FILE *fp);			//know how many entries
char *str_duplicate(const char *s);	//write data to struct of entries

int  main(int argc, char const *argv[])
{
	FILE *fp1;
	FILE *fp2;
	fp1 = fopen("books.txt", "r");
	fp2 = fopen("sellRecord.txt", "r");
	
	//count how many lines and know how many entrys
	int fp1lines = countlines(fp1);
	printf("file1 has %d lines\n",fp1lines );
	int fp2lines = countlines(fp2);
	printf("file2 has %d lines\n",fp2lines );
	EntryBook allEntryBook[fp1lines - 1];
	EntrySellRecord allEntrySellRecord[fp2lines - 1];

	//read every line and construct struct
	fseek(fp1, 0, SEEK_SET);
	fseek(fp2, 0, SEEK_SET);

	//process file 1: books.txt
	char line[256];
	int i = 0;	//use to remove first line, i control the lines
	char *strings[5];
	printf("use allEntryBook\n");
	int fill = 0;	//fill control pos to fill for allEntryBook
	while (fgets(line, sizeof(line), fp1)) 
	{
	       	/* note that fgets don't strip the terminating \n, checking its
	           	presence would allow to handle lines longer that sizeof(line) */
		int j = 0;		//j controls strings[5]
		if (i == 0)
		{
			i++;
			continue;
		}
		/*turn carriage return to null char*/
	        	int t = 0;
	        	while(line[t] != 13)
	        		t++;
	        	line[t] = '\0';
	        	/*split string by '|' and has 5 attributes*/
	        	char *split;
	        	split = strtok(line, "|");
  		while (split != NULL)
  		{
    			strings[j] = split;
    			j++;
    			split = strtok (NULL, "|");

  		} 
  		allEntryBook[fill].isbn = str_duplicate(strings[0]);
  		allEntryBook[fill].author = str_duplicate(strings[1]);
  		allEntryBook[fill].title = str_duplicate(strings[2]);
  		allEntryBook[fill].price = str_duplicate(strings[3]);
  		allEntryBook[fill].subject = str_duplicate(strings[4]);
  		fill++;
    	}
    	for (int i = 0; i < fp1lines - 1; ++i)
    		printf("%s\n",allEntryBook[i].title);
    	
    	//process file 2: sellRecord.txt
    	fclose(fp1);
    	fclose(fp2);
	return 0;
}

int countlines(FILE *fp)
{
	int countlines = 0;
	char ch;
	while(!feof(fp))
	{
  		ch = fgetc(fp);
  		if(ch == '\n')
    			countlines++;
	}
	return countlines;
}
char *str_duplicate(const char *s)
{
	char *d = malloc(strlen(s) + 1);
	if(d == NULL)	return NULL;
	strcpy(d, s);
	return d ;
}
