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
}EntrySellRecord;

typedef struct node_book
{
	char* data;
	int EntryBook_id;
	struct node_book *next;	
}Slot_Book;

typedef struct node_sell
{
	char* data;
	int EntrySellRecord_id;
	struct node_sell *next;
}Slot_sellRecord;

typedef struct 
{
	Slot_Book *buckets[10];
}HashTable_isbn;

int countlines(FILE *fp);			//know how many entries
char *str_duplicate(const char *s);	//write data to struct of entries

//link list
Slot_Book *create_node(char *data, int  EntryBook_id);

//do hashing
int hash_function(char *key);
void hashing_isbn(HashTable_isbn *h, EntryBook *entries, int num);

int  main(int argc, char const *argv[])
{
	FILE *fp1;
	FILE *fp2;
	fp1 = fopen("books.txt", "r");
	fp2 = fopen("sellRecord.txt", "r");
	
	//count how many lines and know how many entrys
	int fp1lines = countlines(fp1);
	//printf("file1 has %d lines\n",fp1lines );
	int fp2lines = countlines(fp2);
	//printf("file2 has %d lines\n",fp2lines );
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
		/*turn '\n' to null char*/
	        	int t = 0;
	        	while(line[t] != '\n')
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
    	
    	
    	//process file 2: sellRecord.txt
    	i = 0;	//use to remove first line, i control the lines
	printf("use allSellRecord\n");
	fill = 0;	//fill control pos to fill for allSellRecord
	while (fgets(line, sizeof(line), fp2)) 
	{
	       	/* note that fgets don't strip the terminating \n, checking its
	           	presence would allow to handle lines longer that sizeof(line) */
		int j = 0;		//j controls strings to [0]~[2]
		if (i == 0)
		{
			i++;
			continue;
		}
		/*turn '\n' to null char*/
	        	int t = 0;
	        	while(line[t] != '\n')
	        		t++;
	        	line[t] = '\0';
	        	/*split string by '|' and has 3 attributes*/
	        	char *split;
	        	split = strtok(line, "|");
  		while (split != NULL)
  		{
    			strings[j] = split;
    			j++;
    			split = strtok (NULL, "|");

  		} 
  		allEntrySellRecord[fill].uid = str_duplicate(strings[0]);
  		allEntrySellRecord[fill].no = str_duplicate(strings[1]);
  		allEntrySellRecord[fill].isbn_no = str_duplicate(strings[2]);
  		fill++;
    	}

    	/*for(i = 0; i < fp1lines - 1; i++)
    		printf("%s\n",allEntryBook[i].title );
    	for(i = 0; i < fp2lines - 1; i++)
    		printf("%s\n",allEntrySellRecord[i].isbn_no );*/

    	fclose(fp1);
    	fclose(fp2);


    	//build 8 hash table and write them to files
    	//1.
    	HashTable_isbn allIsbn;
    	for(i = 0; i < 10; i++)
    		allIsbn.buckets[i] = NULL;
    	hashing_isbn(&allIsbn, allEntryBook, fp1lines - 1);
    	//check link list:
    	printf("check link list:\n");
    	for(i = 0;i < 10; i++)
    	{
    		printf("%d: ",i);
    		Slot_Book *ptr = allIsbn.buckets[i];
    		while(ptr != NULL)
    		{
    			printf("%s -> ", ptr -> data);
    			ptr = ptr -> next;
    		}
    		printf("\n");
    	}

    	
    	
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
Slot_Book *create_node(char *data, int EntryBook_id)
{
	Slot_Book *ptr = (Slot_Book *)malloc(sizeof(Slot_Book));
	if(ptr ==NULL)
		exit(1);

	return ptr;
}
int hash_function(char *key)	//return a integer to decide which bucket 
{
	int i, k;
	unsigned int hv = 0;
	k = strlen(key);
	for(i = 0; i<k; i++)
	{
		hv = (hv << 5) + hv + key[i];
	}
	hv = hv % 10;
	return hv;
}

void hashing_isbn(HashTable_isbn *h, EntryBook *entries, int entry_number)
{
	//init hash table
	printf("print all of isbn:\n");
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		printf("%s-----",entries[i].isbn );
		//do hash
		int hash_result = hash_function(entries[i].isbn);
		printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_node(entries[i].isbn, hash_result);
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].isbn);
			h -> buckets[hash_result] -> EntryBook_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_Book *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_node(entries[i].isbn, hash_result);
			now -> next -> data = str_duplicate(entries[i].isbn);
			now -> next -> EntryBook_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}

