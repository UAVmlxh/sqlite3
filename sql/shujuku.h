#ifndef __shujuku_h
#define __shujuku_h
#include <stdio.h>

#define selsql "select * from rfid;"
#define get_count "select count(*) from rfid;"


void chage(const unsigned char *card_num, unsigned char *card_string,int len);
int input_sql(unsigned char *num);
int getcount_sql(char *max_count);
int findsql(unsigned char *num);
int wf_callback(void *arg, int nr, char **values, char **names);
int select_sql();


int test(unsigned char *num,unsigned int count_data);


#endif
