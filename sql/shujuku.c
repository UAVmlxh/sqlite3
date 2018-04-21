#include <stdio.h>  
#include <stdlib.h>  
#include <sqlite3.h> 
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "shujuku.h"

#define _debug_

unsigned char card_num[4];

void *caozuoshuju(void *arg){
	int *running = arg;
	printf("son xianchen is running !\n");
	while(*running){
	 if(findsql(card_num)==0){
	    printf("no card!\n");
		input_sql(card_num);
		return 0;
		}
	}
	
}


int main( void )  
{  
 int i=0;
 char hanshu[10];
 int flag=0;
 clock_t start,finish;
 double totaltime; 
 unsigned char card_num[4]={0x12,0xdf,0xed,0xab};
 
  test(card_num,5000);

  printf("xixihah\n");
 
 
 
// input_sql(card_num);
//   findsql(card_num);
/*	 
	 while(1)
	 {
		 scanf("%d",&flag);
		 switch(flag){
			 case 1:
			 {     printf("please find card_num:\n");
					for(i=0;i<4;){
						printf("please input card_num[%d]:",i);
						if(scanf("%hhx",&card_num[i]))i++;
					}
					
					if(findsql(card_num)==0)printf("no card!\n");
					flag=0;
			         break;
			 }
			 
			 case 2:
			 {
				  printf("please insert card_num:\n");
					for(i=0;i<4;){
						printf("please input card_num[%d]:",i);
						if(scanf("%hhx",&card_num[i]))i++;
					}
					if(findsql(card_num)==0)printf("can be inserted to sql!\n");
					
					start=clock();	
					input_sql(card_num);
					finish=clock();
					
					totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
					printf("%ld\n",totaltime);
					
					 printf("gg\n");
					 flag=0;
					 break;
			 }
			 default:
					break;
		 }
		 
	 }
*/	 
//   select_sql();
//   getcount_sql(hanshu);
//   printf("%s\n",hanshu);  
 return 0;  
}  

int wf_callback(void *arg, int nr, char **values, char **names)  
{  
    int i;  
    FILE *fd;  
    char str[200];  
    fd = (FILE *)arg; //将void *的参数强制转换为FILE *   
    for (i=0; i<nr; i++) { //将查询结果写入到文件中   
        memset(str, '\0', 200);  
        sprintf(str, "%s", values[i]);  
        fwrite(str, sizeof(char), sizeof(str), fd);  
    }  
    memset(str, '\0', 200);  
    fwrite("\n", sizeof(char), 2, fd); //写完一条记录换行   
    return 0;  //callback函数正常返回0   
}  

//得到行数
int getcount_sql(char *max_count){
	sqlite3 *db=NULL;  
	char *zErrMsg = 0; 
	int rc;
	int jieguo=0;
	char **azResult=NULL; //二维数组存放结果  
	int nrow=0;  
    int ncolumn = 0;  
	int i;
	
	rc = sqlite3_open("/home/pi/sql/test.db", &db);  
	if( rc )  {  
		fprintf(stderr, "Can't open database: %s/n", sqlite3_errmsg(db));  
		sqlite3_close(db);  
			return 1;  
	}  
	//jieguo=sqlite3_exec(db, get_count, NULL, NULL, NULL);  
	sqlite3_get_table( db , get_count , &azResult , &nrow , &ncolumn ,NULL );  
#ifdef _debug_
	printf( "row:%d column=%d\n " , nrow , ncolumn );  
	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )  
    {  
        printf( "azResult[%d] = %s\n", i , azResult[i] );  
    } 
#endif
	
	memset(max_count,0,10*sizeof(unsigned char));
	sprintf(max_count,"%s\0",azResult[1]);
	
    sqlite3_free_table(azResult); 

	sqlite3_close(db); //关闭数据库  
}
//查找数据
int select_sql(){
	sqlite3 *db=NULL;  
	char *zErrMsg = 0; 
	int rc;
	FILE *fd;  
	
	fd = fopen("/home/pi/sql/shuju.txt", "w");  
	rc = sqlite3_open("/home/pi/sql/test.db", &db);  
	if( rc )  {  
		fprintf(stderr, "Can't open database: %s/n", sqlite3_errmsg(db));  
		sqlite3_close(db);  
			return 1;  
	}  
	sqlite3_exec(db, selsql, wf_callback, fd, NULL);  
	fclose(fd);
	sqlite3_close(db); //关闭数据库  
	
}
//查找卡号
int findsql(unsigned char *num){
	unsigned char card[9] ;
	char sql[300];
	sqlite3 *db=NULL;  
	char *zErrMsg = 0; 
	int rc;
	int i;
	char **azResult=NULL; //二维数组存放结果  
	int nrow=0;  
    int ncolumn = 0;  

	chage(num, card, 4);
	char *str = (char *)card;
	//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件  
	rc = sqlite3_open("/home/pi/sql/test.db", &db);  
	if( rc )  
	{  
		fprintf(stderr, "Can't open database: %s/n", sqlite3_errmsg(db));  
		sqlite3_close(db);  
		return 1;  
	}
	
	sprintf(sql,"%s%s%s","select sid from rfid where card ='",str,"';");
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn ,NULL );
#ifdef _debug_
	printf( "row:%d column=%d\n " , nrow , ncolumn );  
	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )  
    {  
        printf( "azResult[%d] = %s\n", i , azResult[i] );  
    } 
#endif
	if((nrow==0)&&(ncolumn==0)){
		printf("no card!");
		return 0;
	}
	return 1;
}

//插入数据
int  input_sql(unsigned char *num){
	unsigned char card[9] ;
	char sql[300];
	sqlite3 *db=NULL;  
	char *zErrMsg = 0; 
	int rc;
	int i;
	int jieguo=0;
	
	chage(num, card, 4);
	char *str = (char *)card;
	
	//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件  
	rc = sqlite3_open("/home/pi/sql/test.db", &db);  
	if( rc )  
	{  
		fprintf(stderr, "Can't open database: %s/n", sqlite3_errmsg(db));  
		sqlite3_close(db);  
		return 1;  
	}  
	//删除最开始的数据
	sprintf(sql, "%s", "delete from rfid where sid = 1; ");
	sqlite3_exec(db, sql, NULL, NULL, NULL);  
	
	//更新999次数据
	for(i=1;i<=99;i++){
	  memset(sql, '\0', 300);
	  sprintf(sql, "%s%d%s%d%s", "update rfid set sid =", i," where sid = ",i+1,";");
	  sqlite3_exec(db, sql, NULL, NULL, NULL);
	  
	}
	
	//插入最后的数据
	memset(sql, '\0', 300);
	sprintf(sql, "%s%s%s","insert into rfid(sid,card)values(100,'",str,"');");
	sqlite3_exec(db, sql, NULL, NULL, NULL); 

	//关闭数据库 	
	sqlite3_close(db);  
}


int test(unsigned char *num,unsigned int count_data){
	unsigned char card[9] ;
	char sql[300];
	sqlite3 *db=NULL;  
	char *zErrMsg = 0; 
	int rc;
	int i;
	int jieguo=0;
	
	chage(num, card, 4);
	char *str = (char *)card;
	
	//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件  
	rc = sqlite3_open("/home/pi/sql/test.db", &db);  
	if( rc )  
	{  
		fprintf(stderr, "Can't open database: %s/n", sqlite3_errmsg(db));  
		sqlite3_close(db);  
		return 1;  
	}  
	
	//更新999次数据
	for(i=0;i<=count_data;i++){
	  memset(sql, '\0', 300);
	  sprintf(sql, "%s%s%s","insert into rfid(card)values('",str,"');");
	  sqlite3_exec(db, sql, NULL, NULL, NULL);
	  
	}
	//关闭数据库 	
	sqlite3_close(db);  
}


//数组装换成字符串
void chage(const unsigned char *card_num, unsigned char *card_string,int len)
{
	int i;
    char S_num[3];
	for (i = 0; i < len; ++i) {
		sprintf(S_num,"%02x", card_num[i]);
		memcpy(&card_string[i * 2], S_num, 3);
	}
}
