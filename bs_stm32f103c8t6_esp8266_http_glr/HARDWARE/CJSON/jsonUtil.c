#include "jsonUtil.h"
#include "cJSON.h"
#include "stdio.h"
 cJSON		* root;
cJSON		* itemName;
double temperatureLimit = 0;
int		flameLimit = 0;
int		gasLimit = 0;
int		smokeLimit = 0;

void parseJson( char* p )
{
	root		= cJSON_Parse( p );
	//0-���� 1-����

//	printf("%s\n\r",p);
//				printf("\n\r ��ʼ�������¶� \n\r");
	itemName	= cJSON_GetObjectItem( root, "wd" );
	if ( itemName )
	{
			
		temperatureLimit = itemName->valuedouble;
//		printf("\n\r �������¶� \n\r");
	
		
	}
//			printf("\n\r ��ʼ���������� \n\r");
	itemName = cJSON_GetObjectItem( root, "hy" );
	if ( itemName )
	{
//		printf("\n\r ���������� \n\r");
		flameLimit = itemName->valueint;
//		printf("\n\r ���������棺%d \n\r",itemName->valueint);
//			printf("\n\r ���������棺%d \n\r",flameLimit);
	}
	itemName = cJSON_GetObjectItem( root, "trq" );
	if ( itemName )
	{
		gasLimit = itemName->valueint;

	}
//				printf("\n\r ��ʼ���������� \n\r");
	itemName = cJSON_GetObjectItem( root, "yw" );
	if ( itemName )
	{
		
		smokeLimit = itemName->valueint;
//		printf("\n\r ����������%d \n\r",smokeLimit);
	}
	cJSON_Delete( root );
}

int strIntercept( char *pcBuf, char *start, char *end, char *pcRes )
{
	int pcBegin	= NULL;
	int  pcEnd		= NULL;

	pcBegin = strstr( pcBuf, start );
	/* printf( "pcBegin:%s \n", pcBegin ); */
	pcEnd = strstr( pcBuf, end );
	/* printf( "pcEnd:%s \n", pcEnd ); */
	if ( pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd )
	{
		printf( "Mail name not found!\n" );
	}else {
		/*	pcBegin += strlen(":"); */
		memcpy( pcRes, pcBegin + strlen( start ), pcEnd - pcBegin - strlen( start ) );
	}

	return(1);
}
