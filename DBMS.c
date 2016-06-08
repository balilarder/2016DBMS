/*input: read book.txt, sellRecord.txt, total 8 attributes, hash to 8 hash tables, each has 10 buckets.
output: 8 files of hash table.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
typedef struct 
{
	Slot_Book *buckets[10];
}HashTable_author;
typedef struct 
{
	Slot_Book *buckets[10];
}HashTable_title;
typedef struct 
{
	Slot_Book *buckets[10];
}HashTable_price;
typedef struct 
{
	Slot_Book *buckets[10];
}HashTable_subject;
typedef struct 
{
	Slot_sellRecord *buckets[10];
}HashTable_uid;
typedef struct 
{
	Slot_sellRecord *buckets[10];
}HashTable_no;
typedef struct 
{
	Slot_sellRecord *buckets[10];
}HashTable_isbn_no;

int countlines(FILE *fp);			//know how many entries
char *str_duplicate(const char *s);	//write data to struct of entries

//link list
Slot_Book *create_book_node();
Slot_sellRecord *create_sell_node();
//do hashing
int hash_function(char *key);
void hashing_isbn(HashTable_isbn *h, EntryBook *entries, int num);
void hashing_author(HashTable_author *h, EntryBook *entries, int num);
void hashing_title(HashTable_title *h, EntryBook *entries, int num);
void hashing_price(HashTable_price *h, EntryBook *entries, int num);
void hashing_subject(HashTable_subject *h, EntryBook *entries, int num);
void hashing_uid(HashTable_uid *h, EntrySellRecord *entries, int num);
void hashing_no(HashTable_no *h, EntrySellRecord *entries, int num);
void hashing_isbn_no(HashTable_isbn_no *h, EntrySellRecord *entries, int num);
//parse SQL
void fill_sql(char *target, char *start, char *end);
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
    	
    	
    	//process file 2: sellRecord.txt
    	i = 0;	//use to remove first line, i control the lines

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
	        	while(line[t] != 13)
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


    	fclose(fp1);
    	fclose(fp2);


    	//build 8 hash table and write them to files
    	//1.
    	HashTable_isbn allIsbn;
    	for(i = 0; i < 10; i++)
    		allIsbn.buckets[i] = NULL;
    	hashing_isbn(&allIsbn, allEntryBook, fp1lines - 1);
    	//write to file:
    	FILE *write_isbn;
    	write_isbn = fopen("HashTable_isbn.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_isbn, "Bucket %d   ",i );
    		Slot_Book *ptr = allIsbn.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_isbn, "%s(%d), ", ptr -> data, ptr -> EntryBook_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_isbn, "\n");
    	}
    	fclose(write_isbn);
    	//2.
    	HashTable_author allAuthor;
    	for(i = 0; i < 10; i++)
    		allAuthor.buckets[i] = NULL;
    	hashing_author(&allAuthor, allEntryBook, fp1lines - 1);
    	//write to file:
    	FILE *write_author;
    	write_author = fopen("HashTable_author.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_author, "Bucket %d   ",i );
    		Slot_Book *ptr = allAuthor.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_author, "%s(%d), ", ptr -> data, ptr -> EntryBook_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_author, "\n");
    	}
    	fclose(write_author);
    	//3.
    	HashTable_title allTitle;
    	for(i = 0; i < 10; i++)
    		allTitle.buckets[i] = NULL;
    	hashing_title(&allTitle, allEntryBook, fp1lines - 1);
    	//write to file:
    	FILE *write_title;
    	write_title = fopen("HashTable_title.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_title, "Bucket %d   ",i );
    		Slot_Book *ptr = allTitle.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_title, "%s(%d), ", ptr -> data, ptr -> EntryBook_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_title, "\n");
    	}
    	fclose(write_title);
    	//4.
    	HashTable_price allPrice;
    	for(i = 0; i < 10; i++)
    		allPrice.buckets[i] = NULL;
    	hashing_price(&allPrice, allEntryBook, fp1lines - 1);
    	//write to file:
    	FILE *write_price;
    	write_price = fopen("HashTable_price.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_price, "Bucket %d   ",i );
    		Slot_Book *ptr = allPrice.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_price, "%s(%d), ", ptr -> data, ptr -> EntryBook_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_price, "\n");
    	}
    	fclose(write_price);
    	//5.
    	HashTable_subject allSubject;
    	for(i = 0; i < 10; i++)
    		allSubject.buckets[i] = NULL;
    	hashing_subject(&allSubject, allEntryBook, fp1lines - 1);
    	//write to file:
    	FILE *write_subject;
    	write_subject = fopen("HashTable_subject.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_subject, "Bucket %d   ",i );
    		Slot_Book *ptr = allSubject.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_subject, "%s(%d), ", ptr -> data, ptr -> EntryBook_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_subject, "\n");
    	}
    	fclose(write_subject);
    	//6.
    	HashTable_uid allUid;
    	for(i = 0; i < 10; i++)
    		allUid.buckets[i] = NULL;
    	hashing_uid(&allUid, allEntrySellRecord, fp2lines - 1);
    	//write to file:
    	FILE *write_uid;
    	write_uid = fopen("HashTable_uid.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_uid, "Bucket %d   ",i );
    		Slot_sellRecord *ptr = allUid.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_uid, "%s(%d), ", ptr -> data, ptr -> EntrySellRecord_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_uid, "\n");
    	}
    	fclose(write_uid);
    	//7.
    	HashTable_no allNo;
    	for(i = 0; i < 10; i++)
    		allNo.buckets[i] = NULL;
    	hashing_no(&allNo, allEntrySellRecord, fp2lines - 1);
    	//write to file:
    	FILE *write_no;
    	write_no = fopen("HashTable_no.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_no, "Bucket %d   ",i );
    		Slot_sellRecord *ptr = allNo.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_no, "%s(%d), ", ptr -> data, ptr -> EntrySellRecord_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_no, "\n");
    	}
    	fclose(write_no);
    	//8.
    	HashTable_isbn_no allIsbn_No;
    	for(i = 0; i < 10; i++)
    		allIsbn_No.buckets[i] = NULL;
    	hashing_isbn_no(&allIsbn_No, allEntrySellRecord, fp2lines - 1);
    	//write to file:
    	FILE *write_isbn_no;
    	write_isbn_no = fopen("HashTable_isbn_no.txt", "w");
    	for(i = 0;i < 10; i++)
    	{
    		fprintf(write_isbn_no, "Bucket %d   ",i );
    		Slot_sellRecord *ptr = allIsbn_No.buckets[i];
    		while(ptr != NULL)
    		{
    			fprintf(write_isbn_no, "%s(%d), ", ptr -> data, ptr -> EntrySellRecord_id);
    			ptr = ptr -> next;
    		}
    		fprintf(write_isbn_no, "\n");
    	}
    	fclose(write_isbn_no);
	


	//start to read query:
    	char *query;
	char select[100], from[100], where[100];
	char *start, *end;
	bool select_exist = 0, from_exist = 0, where_exist = 0, ending_exist = 0;
	query = "SELECT title FROM books,sellRecord WHERE isbn = isbn_no AND author =‘J. K. Rowling’; ";
	//fgets(query, 1000, stdin);
	printf("%s\n",query );

	if(strstr(query, "SELECT") != NULL)
		select_exist = 1;
	if(strstr(query, "FROM") != NULL)
		from_exist = 1;
	if(strstr(query, "WHERE") != NULL)
		where_exist = 1;
	if(strstr(query, ";") != NULL)
		ending_exist = 1;

	printf("%d %d %d\n",select_exist,from_exist,where_exist );
	if(select_exist == 0 || from_exist == 0 || ending_exist == 0)
	{
		printf("SQL syntax error\n");
		return 0;
	}
	
	if(where_exist == 0)
	{
		//a query don't have condition
		printf("a query don't have condition\n");
		start = strstr(query, "SELECT") + 7;
		end = strstr(query, "FROM") - 2;
		if(!(start <= end))
		{
			printf("order wrong\n");
			return 0;
		}
		fill_sql(select, start,end);
		printf("%s\n",select );

		start = strstr(query, "FROM") + 5;
		end = strstr(query, ";") - 1;
		if(!(start <= end))
		{	
			printf("order wrong\n");
			return 0;
		}
		fill_sql(from, start,end);
		printf("%s\n",from);

		

		printf("let's analyze(1,1,0)\n");
		//three types: 
		/*
		(1)select = '*' (contain other than * is error)
		(2)select = some attribute
		(3)select contain DISTINCT
		*/
		if(strstr(select, "*") != NULL)
		{
			if(strcmp(select, "*") == 0)
			{
				printf("select == *, accept\n");
			}
			else
			{
				printf("syntax error\n");
				return 0;
			}
		}
		else if(strstr(select, "DISTINCT") != NULL)
		{
			if(strstr(select, " DISTINCT ") == NULL)
			{
				printf("DISTINCT syntax error\n");
				return 0;
			}
			else
			{
				printf("DISTINCT correct\n");
			}

		}
		else
		{
			printf("some attribute\n");
		}
	}
	else
	{
		//a query has all select, from, where
		printf("a query has all select, from, where\n");
		start = strstr(query, "SELECT") + 7;
		end = strstr(query, "FROM") - 2;
		if(!(start <= end))
		{
			printf("order wrong\n");
			return 0;
		}
		fill_sql(select, start,end);
		printf("%s\n",select );

		start = strstr(query, "FROM") + 5;
		end = strstr(query, "WHERE") - 2;
		if(!(start <= end))
		{	
			printf("order wrong\n");
			return 0;
		}
		fill_sql(from, start,end);
		printf("%s\n",from);

		start = strstr(query, "WHERE") + 6;
		end = strstr(query, ";") - 1;
		if(!(start <= end))
		{	
			printf("order wrong\n");
			return 0;
		}
		fill_sql(where, start,end);
		printf("%s\n",where);


		printf("let's analyze(1,1,1)\n");
		/*
		two types:
		(1) in a table(no join)
		(2) in two table(join)
		*/
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
Slot_Book *create_book_node()
{
	Slot_Book *ptr = (Slot_Book *)malloc(sizeof(Slot_Book));
	if(ptr ==NULL)
		exit(1);

	return ptr;
}
Slot_sellRecord *create_sell_node()
{
	Slot_sellRecord *ptr = (Slot_sellRecord *)malloc(sizeof(Slot_sellRecord));
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
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].isbn );
		//do hash
		int hash_result = hash_function(entries[i].isbn);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_book_node();
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
			now -> next = create_book_node();
			now -> next -> data = str_duplicate(entries[i].isbn);
			now -> next -> EntryBook_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}
void hashing_author(HashTable_author *h, EntryBook *entries, int entry_number)
{
	//init hash table
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].author );
		//do hash
		int hash_result = hash_function(entries[i].author);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_book_node();
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].author);
			h -> buckets[hash_result] -> EntryBook_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_Book *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_book_node();
			now -> next -> data = str_duplicate(entries[i].author);
			now -> next -> EntryBook_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}
void hashing_title(HashTable_title *h, EntryBook *entries, int entry_number)
{
	//init hash table
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].title);
		//do hash
		int hash_result = hash_function(entries[i].title);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_book_node();
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].title);
			h -> buckets[hash_result] -> EntryBook_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_Book *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_book_node();
			now -> next -> data = str_duplicate(entries[i].title);
			now -> next -> EntryBook_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}
void hashing_price(HashTable_price *h, EntryBook *entries, int entry_number)
{
	//init hash table
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].price);
		//do hash
		int hash_result = hash_function(entries[i].price);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_book_node();
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].price);
			h -> buckets[hash_result] -> EntryBook_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_Book *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_book_node();
			now -> next -> data = str_duplicate(entries[i].price);
			now -> next -> EntryBook_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}
void hashing_subject(HashTable_subject *h, EntryBook *entries, int entry_number)
{
	//init hash table
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].subject);
		//do hash
		int hash_result = hash_function(entries[i].subject);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_book_node();
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].subject);
			h -> buckets[hash_result] -> EntryBook_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_Book *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_book_node();
			now -> next -> data = str_duplicate(entries[i].subject);
			now -> next -> EntryBook_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}
void hashing_uid(HashTable_uid *h, EntrySellRecord *entries, int entry_number)
{
	//init hash table
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].uid);
		//do hash
		int hash_result = hash_function(entries[i].uid);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_sell_node();
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].uid);
			h -> buckets[hash_result] -> EntrySellRecord_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_sellRecord *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_sell_node();
			now -> next -> data = str_duplicate(entries[i].uid);
			now -> next -> EntrySellRecord_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}
void hashing_no(HashTable_no *h, EntrySellRecord *entries, int entry_number)
{
	//init hash table
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].no);
		//do hash
		int hash_result = hash_function(entries[i].no);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_sell_node();
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].no);
			h -> buckets[hash_result] -> EntrySellRecord_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_sellRecord *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_sell_node();
			now -> next -> data = str_duplicate(entries[i].no);
			now -> next -> EntrySellRecord_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}
void hashing_isbn_no(HashTable_isbn_no *h, EntrySellRecord *entries, int entry_number)
{
	//init hash table
	int i;
	for(i = 0; i < entry_number; i++)
	{	
		//printf("%s-----",entries[i].isbn_no);
		//do hash
		int hash_result = hash_function(entries[i].isbn_no);
		//printf("hash result is %d\n",hash_result );
		if(h -> buckets[hash_result] == NULL)
		{	
			
			h -> buckets[hash_result] = create_sell_node();
			h -> buckets[hash_result] -> data = str_duplicate(entries[i].isbn_no);
			h -> buckets[hash_result] -> EntrySellRecord_id = i;
			h -> buckets[hash_result] -> next = NULL;
		}
		else
		{
			//go to the final element, and insert new one in behind
			Slot_sellRecord *now = h ->buckets[hash_result];

			while(now -> next != NULL)	
				now = now -> next;
			now -> next = create_sell_node();
			now -> next -> data = str_duplicate(entries[i].isbn_no);
			now -> next -> EntrySellRecord_id = i;
			now -> next -> next = NULL;
		} 
	}
	return;
}

//parse SQL
void fill_sql(char *target, char *start, char *end)
{
	int i = 0;
	while(start <= end)
	{
		
		target[i] = *start;
		start++;
		i++;
	}
	target[i] = '\0';
}