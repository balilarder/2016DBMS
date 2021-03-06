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

int countlines(FILE *fp);				//know how many entries
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
	int fp2lines = countlines(fp2);
	EntryBook allEntryBook[fp1lines - 1];
	EntrySellRecord allEntrySellRecord[fp2lines - 1];

	//read every line and construct struct
	fseek(fp1, 0, SEEK_SET);
	fseek(fp2, 0, SEEK_SET);

	//process file 1: books.txt
	char line[256];
	int i = 0;				//use to remove first line, i control the lines
	char *strings[5];
	
	int fill = 0;			//fill control pos to fill for allEntryBook
	while (fgets(line, sizeof(line), fp1)) 
	{

		int j = 0;			//j controls strings[5]
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
    	i = 0;				//use to remove first line, i control the lines
	fill = 0;				//fill control pos to fill for allSellRecord
	while (fgets(line, sizeof(line), fp2)) 
	{

		int j = 0;			//j controls strings to [0]~[2]
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
	char query[1000];
	char select[100], from[100], where[100];		//3 pars of SQL query, split to each of them
	while(1)
	{
		char *start, *end;						//pointer to duplicate a string, given start and end
		bool select_exist = 0, from_exist = 0, where_exist = 0, ending_exist = 0;

		printf("\ninput query:\n");
		fgets(query, 1000, stdin);
		//basic on keywords, have different types(SELECT, FROM, WHERE)
		if(strcmp(query, "exit\n") == 0)			//exit program
			break;
		if(strstr(query, "SELECT ") != NULL)
			select_exist = 1;
		if(strstr(query, " FROM ") != NULL)
			from_exist = 1;
		if(strstr(query, " WHERE ") != NULL)
			where_exist = 1;
		if(strstr(query, ";") != NULL)
		ending_exist = 1;

		
		if(select_exist == 0 || from_exist == 0 || ending_exist == 0)
		{
			printf("SQL syntax error\n");
			continue;
		}
		
		//basic WHERE, define has condition/without condition
		if(where_exist == 0)
		{
			//a query don't have condition
			start = strstr(query, "SELECT") + 7;
			end = strstr(query, "FROM") - 2;
			if(!(start <= end))
			{
				printf("order wrong\n");
				continue;
			}
			fill_sql(select, start,end);				//make a SELECT string, containing attribute
			
			start = strstr(query, "FROM") + 5;
			end = strstr(query, ";") - 1;
			if(!(start <= end))
			{	
				printf("order wrong\n");
				continue;
			}
			fill_sql(from, start,end);				//make a FROM string, contiaining table
			
			printf("\n");
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
					//(1)types, *, for all attribute
					if(strcmp(from, "books") == 0)  
					{
						
						printf("%-15s%-30s%-65s%-8s%-10s\n\n", "isbn", "author", "title", "price", "subject");
						int i;
						for(i = 0; i < fp1lines - 1; i++)
							printf("%-15s%-30s%-65s%-8s%-10s\n", allEntryBook[i].isbn, allEntryBook[i].author, allEntryBook[i].title, allEntryBook[i].price, allEntryBook[i].subject);


					}
					else if (strcmp(from, "sellRecord") == 0)
					{
						
						printf("%-5s%-5s%-15s\n", "uid", "no", "isbn_no");
						int i;
						for(i = 0; i < fp2lines - 1; i++)
							printf("%-5s%-5s%-15s\n", allEntrySellRecord[i].uid, allEntrySellRecord[i].no, allEntrySellRecord[i].isbn_no);
					}
					else
					{
						printf("no such table\n");
						continue;
					}
				}
				else
				{
					printf("syntax error\n");
					continue;
				}
			}
			else if(strstr(select, "DISTINCT") != NULL)
			{
				if(strstr(select, "DISTINCT ") == NULL)
				{
					printf("DISTINCT syntax error\n");
					continue;
				}
				else
				{
					//(2)types, DISTINCT attribute
					if(strcmp(from, "books") == 0)  
					{
						
						char distinct_attri[100];
						start = strstr(query, "DISTINCT") + 9;
						end = strstr(query, "FROM") - 2;
						fill_sql(distinct_attri, start, end);
						printf("%s\n\n",distinct_attri );
						if(strcmp(distinct_attri, "isbn") != 0 && strcmp(distinct_attri, "author") != 0 && strcmp(distinct_attri, "title") != 0 && strcmp(distinct_attri, "price") != 0 && strcmp(distinct_attri, "subject") != 0)
						{
							printf("no such attribute\n");
							continue;
						}
						else
						{
							
							bool find_distinct[fp1lines -1];
							int i;
							for(i = 0; i < fp1lines - 1; i++)
								find_distinct[i] = 1;
							if(strcmp(distinct_attri, "isbn") == 0)
							{

								for(i = 0; i < fp1lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntryBook[i].isbn );
									int j;
									for(j = i + 1; j < fp1lines - 1; j++)
									{
										if(strcmp(allEntryBook[j].isbn, allEntryBook[i].isbn) == 0)
											find_distinct[j] = 0;
									}
								}
							}
							else if(strcmp(distinct_attri, "author") == 0)
							{

								for(i = 0; i < fp1lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntryBook[i].author );
									int j;
									for(j = i + 1; j < fp1lines - 1; j++)
									{
										if(strcmp(allEntryBook[j].author, allEntryBook[i].author) == 0)
											find_distinct[j] = 0;
									}
								}
							}
							else if(strcmp(distinct_attri, "title") == 0)
							{
								
								for(i = 0; i < fp1lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntryBook[i].title );
									int j;
									for(j = i + 1; j < fp1lines - 1; j++)
									{
										if(strcmp(allEntryBook[j].title, allEntryBook[i].title) == 0)
											find_distinct[j] = 0;
									}
								}
							}
							else if(strcmp(distinct_attri, "price") == 0)
							{
								
								for(i = 0; i < fp1lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntryBook[i].price );
									int j;
									for(j = i + 1; j < fp1lines - 1; j++)
									{
										if(strcmp(allEntryBook[j].price, allEntryBook[i].price) == 0)
											find_distinct[j] = 0;
									}
								}
							}
							else
							{
								
								for(i = 0; i < fp1lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntryBook[i].subject );
									int j;
									for(j = i + 1; j < fp1lines - 1; j++)
									{
										if(strcmp(allEntryBook[j].subject, allEntryBook[i].subject) == 0)
											find_distinct[j] = 0;
									}
								}
							}
						}
					}
					else if(strcmp(from, "sellRecord") == 0)  
					{
						char distinct_attri[100];
						start = strstr(query, "DISTINCT") + 9;
						end = strstr(query, "FROM") - 2;
						fill_sql(distinct_attri, start, end);
						printf("%s\n",distinct_attri );
						if(strcmp(distinct_attri, "uid") != 0 && strcmp(distinct_attri, "no") != 0 && strcmp(distinct_attri, "isbn_no") != 0)
						{
							printf("no such attribute\n");
							continue;
						}
						else
						{
							
							bool find_distinct[fp2lines -1];
							int i;
							for(i = 0; i < fp2lines - 1; i++)
								find_distinct[i] = 1;
							if(strcmp(distinct_attri, "uid") == 0)
							{
								
								for(i = 0; i < fp2lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntrySellRecord[i].uid );
									int j;
									for(j = i + 1; j < fp2lines - 1; j++)
									{
										if(strcmp(allEntrySellRecord[j].uid, allEntrySellRecord[i].uid) == 0)
											find_distinct[j] = 0;
									}
								}
							}
							else if(strcmp(distinct_attri, "no") == 0)
							{
								
								for(i = 0; i < fp2lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntrySellRecord[i].no );
									int j;
									for(j = i + 1; j < fp2lines - 1; j++)
									{
										if(strcmp(allEntrySellRecord[j].no, allEntrySellRecord[i].no) == 0)
											find_distinct[j] = 0;
									}
								}
							}
							else
							{
								
								for(i = 0; i < fp2lines - 1; i++)
								{
									if(find_distinct[i])
										printf("%s\n",allEntrySellRecord[i].isbn_no );
									int j;
									for(j = i + 1; j < fp2lines - 1; j++)
									{
										if(strcmp(allEntrySellRecord[j].isbn_no, allEntrySellRecord[i].isbn_no) == 0)
											find_distinct[j] = 0;
									}
								}
							}
							

						}
					}
					else
					{
						printf("no such table\n");
						continue;
					}
				}

			}
			else
			{
				//type(3): a table with some attribute
				if(strcmp(from, "books") == 0)
				{
					bool include[5] = {0, 0, 0, 0, 0};		//include which kinds of attibute in books
					
					char *split;
					bool invalid = 0;
					split = strtok(select, ",");
					while (split != NULL)
					{
						if(strcmp(split, "isbn") == 0)
							include[0] = 1;
						else if(strcmp(split, "author") == 0)
							include[1] = 1;
						else if(strcmp(split, "title") == 0)
							include[2] = 1;
						else if(strcmp(split, "price") == 0)
							include[3] = 1;
						else if(strcmp(split, "subject") == 0)
							include[4] = 1;
						else
						{
							printf("no such attribute\n");
							invalid = 1;
							break;
						}
						split = strtok (NULL, ",");
					} 
					if(invalid)
						continue;
					else
					{
						int i;
						
						for(i = 0; i < 5; i++)				//print header of inclued attribute
						{
							if(include[i])
							{	
								switch(i)
								{
									case 0:
									printf("%-15s","isbn" );
									break;
									case 1:
									printf("%-30s","author" );
									break;
									case 2:
									printf("%-65s","title" );
									break;
									case 3:
									printf("%-8s","price" );
									break;
									case 4:
									printf("%-10s","subject");
									break;

								}

							}
						}
						printf("\n\n");
						for(i = 0 ;i < fp1lines - 1;i++)		//print result of included attribute
						{
							int j;
							for(j = 0; j < 5; j++)
							{
								if(include[j])
								{	
									switch(j)
									{
										case 0:
										printf("%-15s",allEntryBook[i].isbn );
										break;
										case 1:
										printf("%-30s",allEntryBook[i].author );
										break;
										case 2:
										printf("%-65s",allEntryBook[i].title );
										break;
										case 3:
										printf("%-8s",allEntryBook[i].price );
										break;
										case 4:
										printf("%-10s",allEntryBook[i].subject);
										break;
									}
								}
							}
							printf("\n");
						}
						
					}
				}
				else if(strcmp(from, "sellRecord") == 0)
				{
					bool include[3] = {0, 0, 0};			//include which kinds of attibute in sellRecord
					printf("%s\n", select);
					char *split;
					bool invalid = 0;
					split = strtok(select, ",");
					while (split != NULL)
					{
						
						if(strcmp(split, "uid") == 0)
							include[0] = 1;
						else if(strcmp(split, "no") == 0)
							include[1] = 1;
						else if(strcmp(split, "isbn_no") == 0)
							include[2] = 1;
						else
						{
							printf("no such attribute\n");
							invalid = 1;
							break;
						}
						split = strtok (NULL, ",");
					} 
					if(invalid)
						continue;
					else
					{
						int i;
						for(i = 0; i < 3; i++)				//print header of inclued attribute
						{
							if(include[i])
							{	
								switch(i)
								{
									case 0:
									printf("%-5s","uid" );
									break;
									case 1:
									printf("%-5s","no" );
									break;
									case 2:
									printf("%-15s","isbn_no" );
									break;
								}

							}
						}
						printf("\n");
						for(i = 0 ;i < fp2lines - 1;i++)		//print result of included attribute
						{
							int j;
							for(j = 0; j < 3; j++)
							{
								if(include[j])
								{	
									switch(j)
									{
										case 0:
										printf("%-5s",allEntrySellRecord[i].uid );
										break;
										case 1:
										printf("%-5s",allEntrySellRecord[i].no );
										break;
										case 2:
										printf("%-15s",allEntrySellRecord[i].isbn_no );
										break;
										
									}
								}
							}
							printf("\n");
						}
						
					}
				}
				else
				{
					printf("no such table\n");
					continue;
				}
			}
		}
		else
		{
			//a query has all select, from, where
			//the query has condition
			start = strstr(query, "SELECT") + 7;
			end = strstr(query, "FROM") - 2;
			if(!(start <= end))
			{
				printf("order wrong\n");
				continue;
			}
			fill_sql(select, start,end);


			start = strstr(query, "FROM") + 5;
			end = strstr(query, "WHERE") - 2;
			if(!(start <= end))
			{	
				printf("order wrong\n");
				continue;
			}
			fill_sql(from, start,end);
			

			start = strstr(query, "WHERE") + 6;
			end = strstr(query, ";") - 1;
			if(!(start <= end))
			{	
				printf("order wrong\n");
				continue;
			}
			fill_sql(where, start,end);
			
			/*
			two types:
			(1) in a table(no join)
			(2) in two table(join)
			*/
			if(strstr(from, ",") == NULL)
			{
				//type(1) no join
				if(strcmp(from, "books") == 0)
				{
					bool include[5] = {0, 0, 0, 0, 0};			//include which attribute
				
					char *split;
					bool invalid = 0;
					split = strtok(select, ",");
					while (split != NULL)
					{
						
						if(strcmp(split, "isbn") == 0)
							include[0] = 1;
						else if(strcmp(split, "author") == 0)
							include[1] = 1;
						else if(strcmp(split, "title") == 0)
							include[2] = 1;
						else if(strcmp(split, "price") == 0)
							include[3] = 1;
						else if(strcmp(split, "subject") == 0)
							include[4] = 1;
						else
						{
							printf("no such attribute\n");
							invalid = 1;
							break;
						}
						split = strtok (NULL, ",");
					} 
					if(invalid)
						continue;
					else
					{
						int i;
						for(i = 0; i < 5; i++)					//print header of included attribute
						{
							if(include[i])
							{	
								switch(i)
								{
									case 0:
									printf("%-15s","isbn" );
									break;
									case 1:
									printf("%-30s","author" );
									break;
									case 2:
									printf("%-65s","title" );
									break;
									case 3:
									printf("%-8s","price" );
									break;
									case 4:
									printf("%-10s","subject");
									break;

								}

							}
						}
						printf("\n\n");
						//process conditions, split keyword is "AND":
						i = 0;
						char conditions[5][100];
						
						if(strstr(where, "AND") != 0)
						{	
							
							//where contain "AND", need to split
							char *temp = str_duplicate(where);
							while(strstr(temp, "AND") != 0)
							{
								start = temp;
								end = strstr(temp, "AND") - 2;
								fill_sql(conditions[i], start, end);

								temp = end + 6;
								i++;
							}
							//the last substing of remain
							char *x = temp;
							int y = 0;
							while(*x != '\0')
							{
								conditions[i][y] = *x;
								x++;
								y++;
							}
							conditions[i][y] = '\0';
							i++;
						}
						else
						{
							//no "AND", just one condition
							fill_sql(conditions[i], where, where + strlen(where) );
							i++;
						}

						int condition_numbers = i;
						
						if(condition_numbers > 4)
							continue;

						int use_entry_id[fp1lines -1];					//an array to indicate which entry need to be inclued by hash table
						for(i =0 ;i < fp1lines -1 ;i++)
							use_entry_id[i] = 0;

						//process "every" condition
						for(i =0;i < condition_numbers ;i++)
						{	
							
							char where_attribute[100];
							char where_condition[100];
							
							start = conditions[i];
							end = strstr(conditions[i], "=") - 1;
							fill_sql(where_attribute, start, end);

							start = strstr(conditions[i], "=") + 2;
							char *find;
							for(find = conditions[i] + strlen(conditions[i]) ; ;find --)
							{
								if(*find == '\'')
								{	
									end = find - 1;
									break;
								}
							}
							fill_sql(where_condition, start, end);
							//WHERE condition get string between ' '
							
							if(strcmp(where_attribute, "isbn") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_Book *travel = allIsbn.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel -> EntryBook_id] ++;
									travel = travel -> next;
								}
							}
							else if(strcmp(where_attribute, "author") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_Book *travel = allAuthor.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel -> EntryBook_id] ++;
									travel = travel -> next;
								}
							}
							else if(strcmp(where_attribute, "title") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_Book *travel = allTitle.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel -> EntryBook_id] ++;
									travel = travel -> next;
								}
							}
							else if(strcmp(where_attribute, "price") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_Book *travel = allPrice.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel -> EntryBook_id] ++;
									travel = travel -> next;
								}
							}
							else if(strcmp(where_attribute, "subject") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_Book *travel = allSubject.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel -> EntryBook_id] ++;
									travel = travel -> next;
								}
							}
							else
							{
								printf("no this attribute\n");
								invalid = 1;
								break;
							}
						}

						if(invalid)
							continue;

						//output:

						for(i = 0 ;i < fp1lines - 1;i++)
						{
							if(use_entry_id[i] == condition_numbers)		//all condition match, then output 
							{	
								int j;
								for(j = 0; j < 5; j++)
								{
									if(include[j])
									{	
										switch(j)
										{
											case 0:
											printf("%-15s",allEntryBook[i].isbn );
											break;
											case 1:
											printf("%-30s",allEntryBook[i].author );
											break;
											case 2:
											printf("%-65s",allEntryBook[i].title );
											break;
											case 3:
											printf("%-8s",allEntryBook[i].price );
											break;
											case 4:
											printf("%-10s",allEntryBook[i].subject);
											break;
										}
									}
								}
								printf("\n");
							}
						}
					}
				}
				else if(strcmp(from, "sellRecord") == 0)
				{
					bool include[3] = {0, 0, 0};			//include which of attribute
					
					char *split;
					bool invalid = 0;
					split = strtok(select, ",");
					while (split != NULL)
					{
						
						if(strcmp(split, "uid") == 0)
							include[0] = 1;
						else if(strcmp(split, "no") == 0)
							include[1] = 1;
						else if(strcmp(split, "isbn_no") == 0)
							include[2] = 1;
						else
						{
							printf("no such attribute\n");
							invalid = 1;
							break;
						}
						split = strtok (NULL, ",");
					} 
					if(invalid)
						continue;
					else
					{
						int i;
						for(i = 0; i < 3; i++)				//print header of included attribute
						{
							if(include[i])
							{	
								switch(i)
								{
									case 0:
									printf("%-5s","uid" );
									break;
									case 1:
									printf("%-5s","no" );
									break;
									case 2:
									printf("%-15s","isbn_no" );
									break;

								}

							}
						}
						printf("\n\n");

					
						//process conditions, split keyword is "AND":
						i = 0;
						char conditions[5][100];
					
						if(strstr(where, "AND") != 0)		//has AND, do split
						{	
							
							char *temp = str_duplicate(where);
							while(strstr(temp, "AND") != 0)
							{
								start = temp;
								end = strstr(temp, "AND") - 2;
								fill_sql(conditions[i], start, end);

								temp = end + 6;
								i++;
							}
							//this is remainder, doesn't have "AND"
							char *x = temp;
							int y = 0;
							while(*x != '\0')
							{
								conditions[i][y] = *x;
								x++;
								y++;
							}
							conditions[i][y] = '\0';
							i++;
						}
						else			//don't have AND, just a condition
						{
							fill_sql(conditions[i], where, where + strlen(where) );
							i++;
						}		
						int condition_numbers = i;
						
						if(condition_numbers > 4)
							continue;

						int use_entry_id[fp2lines -1];		//an array to indicate which entry need to be inclued by hash table
						for(i =0 ;i < fp2lines -1 ;i++)
							use_entry_id[i] = 0;

						//process "every" condition
						for(i =0;i < condition_numbers ;i++)
						{	
							
							char where_attribute[100];
							char where_condition[100];
							
							start = conditions[i];
							end = strstr(conditions[i], "=") - 1;
							fill_sql(where_attribute, start, end);

							start = strstr(conditions[i], "=") + 2;
							char *find;
							for(find = conditions[i] + strlen(conditions[i]) ; ;find --)
							{
								if(*find == '\'')
								{	
									end = find - 1;
									break;
								}
							}
							fill_sql(where_condition, start, end);
							//WHERE contian string between ' '
							
	
							
							if(strcmp(where_attribute, "uid") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_sellRecord *travel = allUid.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel -> EntrySellRecord_id] ++;
									travel = travel -> next;
								}
							}
							else if(strcmp(where_attribute, "no") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_sellRecord *travel = allNo.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel ->  EntrySellRecord_id] ++;
									travel = travel -> next;
								}
							}
							else if(strcmp(where_attribute, "isbn_no") == 0 )
							{
								
								int lookup = hash_function(where_condition);
								Slot_sellRecord *travel = allIsbn_No.buckets[lookup];
								while(travel != NULL)
								{
									
									if(strcmp(travel -> data, where_condition) == 0)
										use_entry_id[travel -> EntrySellRecord_id] ++;
									travel = travel -> next;
								}
							}
							else
							{
								printf("no this attribute\n");
								invalid = 1;
								break;
							}
						}

						if(invalid)
							continue;

						//output:

						for(i = 0 ;i < fp2lines - 1;i++)
						{
							if(use_entry_id[i] == condition_numbers)			//all condition match, then output 
							{	
								int j;
								for(j = 0; j < 3; j++)
								{
									if(include[j])
									{	
										switch(j)
										{
											case 0:
											printf("%-5s",allEntrySellRecord[i].uid );
											break;
											case 1:
											printf("%-5s",allEntrySellRecord[i].no );
											break;
											case 2:
											printf("%-15s",allEntrySellRecord[i].isbn_no );
											break;
											
										}
									}
								}
								printf("\n");
							}
						}
					}
				}
				else
				{
					printf("no such table\n");
					continue;
				}
			}
			else
			{
				//type(2) need to join
				//join
				char *after_join[100][8];		//string of after join, with 8 attribute
				int join_number = 0;
				if(strcmp(from, "books,sellRecord") == 0 || strcmp(from, "sellRecord,books") == 0)
				{
					bool include[8] = {0, 0, 0, 0, 0, 0, 0, 0};
					
					char *split;
					bool invalid = 0;
					split = strtok(select, ",");
					while (split != NULL)
					{
						
						if(strcmp(split, "isbn") == 0)
							include[0] = 1;
						else if(strcmp(split, "author") == 0)
							include[1] = 1;
						else if(strcmp(split, "title") == 0)
							include[2] = 1;
						else if(strcmp(split, "price") == 0)
							include[3] = 1;
						else if(strcmp(split, "subject") == 0)
							include[4] = 1;
						else if(strcmp(split, "uid") == 0)
							include[5] = 1;
						else if(strcmp(split, "no") == 0)
							include[6] = 1;
						else if(strcmp(split, "isbn_no") == 0)
							include[7] = 1;
						else
						{
							printf("no such attribute\n");
							invalid = 1;
							break;
						}
						split = strtok (NULL, ",");
					} 
					if(invalid)
						continue;
					else
					{
						int i;
						for(i = 0; i < 8; i++)					//print header of include attribute
						{
							if(include[i])
							{	
								switch(i)
								{
									case 0:
									printf("%-15s","isbn" );
									break;
									case 1:
									printf("%-30s","author" );
									break;
									case 2:
									printf("%-65s","title" );
									break;
									case 3:
									printf("%-8s","price" );
									break;
									case 4:
									printf("%-10s","subject");
									break;
									case 5:
									printf("%-5s","uid");
									break;
									case 6:
									printf("%-5s","no");
									break;
									case 7:
									printf("%-15s","isbn_no");
									break;

								}

							}
						}
						printf("\n\n");

						//process conditions:
						i = 0;
						char conditions[5][100];
						
						if(strstr(where, "AND") != 0)			//with AND, has 1+ condition
						{	
							
							char *temp = str_duplicate(where);
							while(strstr(temp, "AND") != 0)
							{
								start = temp;
								end = strstr(temp, "AND") - 2;
								fill_sql(conditions[i], start, end);

								temp = end + 6;
								i++;
							}
							//remainder
							char *x = temp;
							int y = 0;
							while(*x != '\0')
							{
								conditions[i][y] = *x;
								x++;
								y++;
							}
							conditions[i][y] = '\0';
							i++;
						}
						else									//without AND
						{
							fill_sql(conditions[i], where, where + strlen(where) );
							i++;
						}		
						int condition_numbers = i;
						
						if(condition_numbers > 4)
							continue;

						int join_sucess[fp1lines -1];		//an array to indicate which entry make a link by join
						for(i =0 ;i < fp1lines -1 ;i++)
							join_sucess[i] = -1;

						int use_join_entry[(fp1lines -1) * (fp2lines - 1)];		//an array to indicate which entry make a link by join
						for(i =0 ;i < (fp1lines -1) * (fp2lines - 1) ;i++)
							use_join_entry[i] = 0;

						//process "every" condition
						for(i =0;i < condition_numbers ;i++)
						{	
							bool right_is_attribute = 0;
							
							char equal_left[100];		//need to be attribute
							char equal_right[100];		//may be attribute or string
							
							start = conditions[i];
							end = strstr(conditions[i], "=") - 1;
							fill_sql(equal_left, start, end);

							int counting = 0;
							char *cursor = strstr(conditions[i], "=") + 1;
							while(*cursor != '\0')
							{
								if(*cursor == '\'')
									counting++;
								cursor++;
							}
							if(counting == 2)	//right segement is string, not attribute
							{
								
								right_is_attribute = 0;
								start = strstr(conditions[i], "=") + 2;
								char *find;
								for(find = conditions[i] + strlen(conditions[i]) ; ;find --)
								{
									if(*find == '\'')
									{	
										end = find - 1;
										break;
									}
								}
							}
							else			//right segment will seen as attribute
							{
								
								right_is_attribute = 1;
								start = strstr(conditions[i], "=") + 1;
								char *find;
								for(find = conditions[i] + strlen(conditions[i]) ; ;find --)
								{
									if(*find == '\0')
									{	
										end = find - 1;
										break;
									}
								}
							}
							fill_sql(equal_right, start, end);
							

							if(strcmp(equal_left, "isbn") != 0 &&
								strcmp(equal_left, "author") != 0 &&
								strcmp(equal_left, "title") != 0 &&
								strcmp(equal_left, "price") != 0 &&
								strcmp(equal_left, "subject") != 0 &&
								strcmp(equal_left, "uid") != 0 &&
								strcmp(equal_left, "no") != 0 &&
								strcmp(equal_left, "isbn_no") != 0)
							{
								//left is not an attribute
								invalid = 1;
								break;
							}
							if(right_is_attribute)
							{
								if(strcmp(equal_right, "isbn") != 0 && 
									strcmp(equal_right, "author") != 0 && 
									strcmp(equal_right, "title") != 0 && 
									strcmp(equal_right, "price") != 0 &&	
									strcmp(equal_right, "subject") != 0 && 
									strcmp(equal_right, "uid") != 0 && 
									strcmp(equal_right, "no") != 0 &&	
									strcmp(equal_right, "isbn_no") != 0)
								{
									//but right is not an attribute
									invalid = 1;
									break;
								}
								else
								{
									//try to build link between two table
									if( (strcmp(equal_left, "isbn") == 0 && strcmp(equal_right, "isbn_no") == 0) ||	
										(strcmp(equal_left, "isbn_no") == 0 && strcmp(equal_right,"isbn") == 0) )	//has necessary of join
									{
										
										/*then start join, for every entry, get it string,
										compute the hash result to get a bucket id for the other table.*/
										
										int i ;
										for(i = 0; i < fp1lines - 1 ;i++)
										{
											Slot_sellRecord *travel = allIsbn_No.buckets[hash_function(allEntryBook[i].isbn)];
											while(travel != NULL)
											{
												if(strcmp(travel -> data, allEntryBook[i].isbn) == 0)
												{	
													join_sucess[i] = travel -> EntrySellRecord_id;
													
													after_join[join_number][0] = str_duplicate(allEntryBook[i].isbn);
													after_join[join_number][1] = str_duplicate(allEntryBook[i].author);
													after_join[join_number][2] = str_duplicate(allEntryBook[i].title);
													after_join[join_number][3] = str_duplicate(allEntryBook[i].price);
													after_join[join_number][4] = str_duplicate(allEntryBook[i].subject);
													after_join[join_number][5] = str_duplicate(allEntrySellRecord[ travel -> EntrySellRecord_id].uid);
													after_join[join_number][6] = str_duplicate(allEntrySellRecord[ travel -> EntrySellRecord_id].no);
													after_join[join_number][7] = str_duplicate(allEntrySellRecord[ travel -> EntrySellRecord_id].isbn_no);
													join_number++;
												}
												travel = travel -> next;
											}
											
										}
										
										
									}
									else		//without necessary of join
									{
										invalid = 1;
										break;
									}
								}

							}
							else
							{
								//right is string, look up after_join_table
								
								int i;
								if(strcmp(equal_left, "isbn") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_Book *travel = allIsbn.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][0], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;

												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else if(strcmp(equal_left, "author") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_Book *travel = allAuthor.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][1], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;
												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else if(strcmp(equal_left, "title") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_Book *travel = allTitle.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][2], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;

												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else if(strcmp(equal_left, "price") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_Book *travel = allPrice.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][3], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;

												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else if(strcmp(equal_left, "subject") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_Book *travel = allSubject.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][4], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;

												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else if(strcmp(equal_left, "uid") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_sellRecord *travel = allUid.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][5], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;

												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else if(strcmp(equal_left, "no") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_sellRecord *travel = allNo.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][6], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;
												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else if(strcmp(equal_left, "isbn_no") == 0 )
								{
									int i;
									bool can_find_in_join_table[join_number];
									for(i = 0; i < join_number ; i++)
										can_find_in_join_table[i] = 0;
									int lookup = hash_function(equal_right);
									Slot_sellRecord *travel = allIsbn_No.buckets[lookup];
									while(travel != NULL)
									{
										if(strcmp(travel -> data, equal_right) == 0)
										{		
											for(i = 0; i < join_number; i++)
											{
												if(strcmp(after_join[i][7], equal_right) == 0)
												{
													can_find_in_join_table[i] = 1;
												}	
											}
										}
										travel = travel -> next;
									}
									for(i = 0; i < join_number ; i++)
									{	
										if(can_find_in_join_table[i])
											use_join_entry[i] ++;
									}
								}
								else
								{
									printf("no such attribute\n");
									invalid = 1;
									break;
								}


							}

						}

						if(invalid)
							continue;

						//output:
				
						for(i = 0 ;i < join_number;i++)
						{
							if(use_join_entry[i] == condition_numbers - 1)	//because 1 is  join condition
							{	
								int j;
								for(j = 0; j < 8; j++)
								{
									if(include[j])
									{	
										switch(j)
										{
											case 0:
											printf("%-15s",after_join[i][0] );
											break;
											case 1:
											printf("%-30s",after_join[i][1] );
											break;
											case 2:
											printf("%-65s",after_join[i][2] );
											break;
											case 3:
											printf("%-8s",after_join[i][3] );
											break;
											case 4:
											printf("%-10s",after_join[i][4]);
											break;
											case 5:
											printf("%-5s",after_join[i][5]);
											break;
											case 6:
											printf("%-5s",after_join[i][6]);
											break;
											case 7:
											printf("%-15s",after_join[i][7]);
											break;

										}
									}
								}
								printf("\n");
							}
						}
					}
				}
				else
				{
					//join failed
					continue;
				}
			}
		}
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
		
		//do hash
		int hash_result = hash_function(entries[i].isbn);
		
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
		
		//do hash
		int hash_result = hash_function(entries[i].author);
		
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
		
		//do hash
		int hash_result = hash_function(entries[i].title);
		
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
		
		//do hash
		int hash_result = hash_function(entries[i].price);
		
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
		
		//do hash
		int hash_result = hash_function(entries[i].subject);
		
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
		
		//do hash
		int hash_result = hash_function(entries[i].uid);
		
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
		
		//do hash
		int hash_result = hash_function(entries[i].no);
		
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
		
		//do hash
		int hash_result = hash_function(entries[i].isbn_no);
		
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