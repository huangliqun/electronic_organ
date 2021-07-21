
#include "cJSON.h"
#include "cJsonDataDeal.h"

cJSON*  Api_reset(cJSON * receverJsonData)
{
	cJSON *returnDataJson = cJSON_CreateObject();	 
	cJSON_AddStringToObject(returnDataJson, "api", 	"reset");
	cJSON_AddStringToObject(returnDataJson, "flag", "true");
	return returnDataJson;
}

//心跳包
cJSON*   ApiHeartbeat( void)
{
	cJSON *returnDataJson = cJSON_CreateObject();
	cJSON_AddStringToObject(returnDataJson, "api", "heart");
	cJSON_AddStringToObject(returnDataJson, "id", sysCfg.device_id);
	//cJSON_AddStringToObject(returnDataJson,"Flag","True");
	return returnDataJson;
}
 

//停止
cJSON*   ApiStop( cJSON * receverJsonData)
{
		u8 group=cJSON_GetObjectItem(receverJsonData, "group")->valueint;
		cJSON *returnDataJson = cJSON_CreateObject();	 
	 
	 
	cJSON_AddStringToObject(returnDataJson, "api", "stop");
	cJSON_AddStringToObject(returnDataJson, "id", sysCfg.device_id);
	cJSON_AddNumberToObject(returnDataJson, "code", 1);
	cJSON_AddNumberToObject(returnDataJson, "group", group);
	return returnDataJson;
}

////游戏包
//cJSON * ApiPacket(cJSON * receverJsonData)
//{
//	cJSON *returnDataJson = cJSON_CreateObject();
//	cJSON_AddStringToObject(returnDataJson, "api", "packet");
//	cJSON_AddStringToObject(returnDataJson, "id", sysCfg.device_id);
//	cJSON_AddNumberToObject(returnDataJson, "code", 1);	 
//	return returnDataJson;
//}

// 开始
cJSON*   ApiStart( cJSON * receverJsonData)
{
  
	cJSON *returnDataJson = cJSON_CreateObject();
	cJSON_AddStringToObject(returnDataJson, "api", "start");
	cJSON_AddStringToObject(returnDataJson, "id", sysCfg.device_id);
 	cJSON_AddNumberToObject(returnDataJson, "code", 1);
 
	 
	return returnDataJson;
}
 
//获取信息
cJSON*    Api_info(cJSON * receverJsonData)
{
	cJSON *returnDataJson = cJSON_CreateObject();
	cJSON_AddStringToObject(returnDataJson, "api", 	"info");
	cJSON_AddStringToObject(returnDataJson, "id", sysCfg.device_id);
	cJSON_AddNumberToObject(returnDataJson, "code", 1);
	cJSON_AddStringToObject(returnDataJson, "flag", "true");
	cJSON_AddNumberToObject(returnDataJson, "bright", sysCfg.bright);
	cJSON_AddNumberToObject(returnDataJson, "bat", sysCfg.bat);
	return returnDataJson;
}
 

//设置查询进度
cJSON*    ApiInquire(cJSON * receverJsonData)
{
	cJSON *returnDataJson = cJSON_CreateObject();
  
 		
	
	cJSON_AddStringToObject(returnDataJson, "api", 	"inquire");
  
	return returnDataJson;
}

 
//获取设备列表
cJSON*    ApiGetDriverList(cJSON * receverJsonData)
{
	cJSON *returnDataJson = cJSON_CreateObject();
	
	cJSON *DataArr = cJSON_CreateArray();
	
	cJSON *DataArrOne ; 
	
	
	cJSON_AddStringToObject(returnDataJson, "Api", 	"GetDriverList");
  cJSON_AddStringToObject(returnDataJson, "Flag", 	"true");
	
	DataArrOne = cJSON_CreateObject();
	cJSON_AddStringToObject(DataArrOne, "DriverID", 	sysCfg.device_id);
	cJSON_AddNumberToObject(DataArrOne, "IsUsed", 	gamePack1.IsUsed);
	cJSON_AddItemToArray(DataArr,DataArrOne);
	
 
	
	cJSON_AddItemToObject(returnDataJson,"Data",DataArr);
	
	return returnDataJson;
}

cJSON*    ApiGetSysSta(cJSON * receverJsonData)
{
	cJSON *returnDataJson = cJSON_CreateObject();
	cJSON_AddStringToObject(returnDataJson, "Api", 	"GetSysSta");
  cJSON_AddStringToObject(returnDataJson, "Flag", 	"true");
	cJSON_AddNumberToObject(returnDataJson, "Bat", 	100);
	return returnDataJson;
}

cJSON*    ApiSetTime(cJSON * receverJsonData)
{
	cJSON *returnDataJson = cJSON_CreateObject();
	cJSON_AddStringToObject(returnDataJson, "Api", 	"SetTime");
  cJSON_AddStringToObject(returnDataJson, "Flag", 	"true");
	return returnDataJson;
}

 
cJSON*   ApiSendMusic(cJSON * receverJsonData)
{
	u8  firstStep=1;
	u16 i,strLen;
	char *Data;
	cJSON *returnDataJson = cJSON_CreateObject();

	gamePackData_TypeDef *gamePack;
  gamePack=&gamePack1;
	gamePack_Clear(gamePack);
	
	gamePack->Group=cJSON_GetObjectItem(receverJsonData, "Count")->valueint;
	gamePack->Mode=	cJSON_GetObjectItem(receverJsonData, "Mode")->valueint;
	gamePack->Beat=	cJSON_GetObjectItem(receverJsonData, "Beat")->valueint;
	
	Data=cJSON_GetObjectItem(receverJsonData, "Data")->valuestring;
	
  strLen=strlen(Data);
	
  
	for(i=0;i<strLen;i++)
	{
		  if( Data[i] >='0' && Data[i] <='9'  )
			{
					gamePackAction_TypeDef *p;
					p=gamePackAction_Creatlink();
					p->floorID= Data[i]-'0';	
					p->type=1;	
					p->color=1;		
					p->time=4;  //3*0.5 =1.5s
					p->next=NULL;
					 
					if( firstStep ) 
					{
						gamePack->head=p;	
						firstStep=0;						
					}
					else
					{
						gamePackAction_InsertNode(gamePack->head,p);			 		
					}			
			}
	}
	
	gamePack->IsUsed=1;
	GameRunFlag1=0;  
  cJSON_AddStringToObject(returnDataJson, "Api", 	"SendMusic");
  cJSON_AddStringToObject(returnDataJson, "Flag", 	"true");
	cJSON_AddStringToObject(returnDataJson, "DriverID", 	sysCfg.device_id);
	return returnDataJson;
}


cJSON*   ApiGetSchedule(cJSON * receverJsonData)
{
  cJSON *returnDataJson = cJSON_CreateObject();

	gamePackData_TypeDef *gamePack;
  gamePack=&gamePack1;
 
  cJSON_AddStringToObject(returnDataJson, "Api", 	"GetSchedule");
  cJSON_AddStringToObject(returnDataJson, "Flag", 	"true");
	cJSON_AddStringToObject(returnDataJson, "DriverID", 	sysCfg.device_id);
	
	cJSON_AddNumberToObject(returnDataJson, "RightCount", 	gamePack->RightCount);
	cJSON_AddNumberToObject(returnDataJson, "ErrCount", 	gamePack->ErrCount);
	cJSON_AddNumberToObject(returnDataJson, "FinGroup", 	gamePack->FinGroup);
	cJSON_AddNumberToObject(returnDataJson, "IsUsed", 	gamePack->IsUsed);
	
	return returnDataJson;
}

cJSON*   ApiStopMusic(cJSON * receverJsonData)
{
  cJSON *returnDataJson = cJSON_CreateObject();

	gamePackData_TypeDef *gamePack;
  gamePack=&gamePack1;
	gamePack->IsUsed=0;
	
  cJSON_AddStringToObject(returnDataJson, "Api", 	"StopMusic");
  cJSON_AddStringToObject(returnDataJson, "Flag", 	"true");
	cJSON_AddStringToObject(returnDataJson, "DriverID", 	sysCfg.device_id);
	
	cJSON_AddNumberToObject(returnDataJson, "RightCount", 	gamePack->RightCount);
	cJSON_AddNumberToObject(returnDataJson, "ErrCount", 	gamePack->ErrCount);
	cJSON_AddNumberToObject(returnDataJson, "FinGroup", 	gamePack->FinGroup);
	cJSON_AddNumberToObject(returnDataJson, "IsUsed", 	gamePack->IsUsed);
	
	return returnDataJson;
}

char *   main_json_data_deal(const char *data)//char* returnData,
{
	cJSON * receverJsonData = cJSON_Parse(data);
	cJSON *returnJsonData=NULL;//?????json??
	char *returnData=NULL;
	char * Api;


	if (NULL != receverJsonData)
	{
		if(cJSON_GetObjectItem(receverJsonData, "Api") == NULL)
		{
			goto exit;
		}
		Api = cJSON_GetObjectItem(receverJsonData, "Api")->valuestring;
		if(Api == NULL)
		{
			goto exit;
		}

 
	  if (!strcmp(Api, "GetDriverList"))
		{
			returnJsonData=ApiGetDriverList(receverJsonData);
 		}
		else		if (!strcmp(Api, "GetSysSta"))
		{
			returnJsonData=ApiGetSysSta(receverJsonData);
 		}
		else if (!strcmp(Api, "SetTime")) {
			returnJsonData=ApiSetTime(receverJsonData);
 		}

		else if (!strcmp(Api, "SendMusic")) {
						returnJsonData=ApiSendMusic(receverJsonData);
			
 		}
		else if(!strcmp(Api, "GetSchedule"))
		{
			returnJsonData=ApiGetSchedule(receverJsonData);
 		}
		else if(!strcmp(Api, "StopMusic"))
		{
			returnJsonData=ApiStopMusic(receverJsonData);
 		} 
		else
		{
			goto exit;
		}
	}
	 

	exit:	
	
	if(receverJsonData!=NULL)
	cJSON_Delete(receverJsonData);

	if(returnJsonData)
	{
		returnData=cJSON_PrintUnformatted(returnJsonData);
		cJSON_Delete(returnJsonData);
	}


	

	return returnData;
}


