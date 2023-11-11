

#include <ta_common.h>

typedef struct 
{
   TA_RetCode retCode;
   const char * const enumStr;
   const char * const infoStr;
} TA_InternalRetCodeInfo;

static TA_InternalRetCodeInfo retCodeInfoTable[] = {
         {(TA_RetCode)0,"TA_SUCCESS","No error"},
         {(TA_RetCode)1,"TA_LIB_NOT_INITIALIZE","TA_Initialize was not sucessfully called"},
         {(TA_RetCode)2,"TA_BAD_PARAM","A parameter is out of range"},
         {(TA_RetCode)3,"TA_ALLOC_ERR","Possibly out-of-memory"},
         {(TA_RetCode)4,"TA_GROUP_NOT_FOUND","No Info"},
         {(TA_RetCode)5,"TA_FUNC_NOT_FOUND","No Info"},
         {(TA_RetCode)6,"TA_INVALID_HANDLE","No Info"},
         {(TA_RetCode)7,"TA_INVALID_PARAM_HOLDER","No Info"},
         {(TA_RetCode)8,"TA_INVALID_PARAM_HOLDER_TYPE","No Info"},
         {(TA_RetCode)9,"TA_INVALID_PARAM_FUNCTION","No Info"},
         {(TA_RetCode)10,"TA_INPUT_NOT_ALL_INITIALIZE","No Info"},
         {(TA_RetCode)11,"TA_OUTPUT_NOT_ALL_INITIALIZE","No Info"},
         {(TA_RetCode)12,"TA_OUT_OF_RANGE_START_INDEX","No Info"},
         {(TA_RetCode)13,"TA_OUT_OF_RANGE_END_INDEX","No Info"},
         {(TA_RetCode)14,"TA_INVALID_LIST_TYPE","No Info"},
         {(TA_RetCode)15,"TA_BAD_OBJECT","No Info"},
         {(TA_RetCode)16,"TA_NOT_SUPPORTED","No Info"},
         {(TA_RetCode)5000,"TA_INTERNAL_ERROR","No Info"},
         {(TA_RetCode)0xFFFF,"TA_UNKNOWN_ERR","Unknown Error"}
};

#define NB_RET_CODE_INFO (sizeof(retCodeInfoTable)/sizeof(TA_InternalRetCodeInfo))

void TA_SetRetCodeInfo( TA_RetCode theRetCode, TA_RetCodeInfo *retCodeInfo )
{
   unsigned int i;
   
   /* Trap internal error code */
   if( (theRetCode >= 5000) && (theRetCode <= 5999) )
   {
      retCodeInfo->enumStr = "TA_INTERNAL_ERROR";
      retCodeInfo->infoStr = "Unexpected Internal Error";
      return;
   }
   
   /* Check among all the error code defined in ta_common.h */
   for( i=0; i < (NB_RET_CODE_INFO-1); i++ )
   {
      if( theRetCode == retCodeInfoTable[i].retCode )
      {
         /* Error code found. */
         retCodeInfo->enumStr = retCodeInfoTable[i].enumStr;
         retCodeInfo->infoStr = retCodeInfoTable[i].infoStr;
         return;
      }
   }

   /* Error code not found. */

   /* "TA_UNKNOWN_ERR" is ALWAYS the last entry in the table. */
   retCodeInfo->enumStr = retCodeInfoTable[i].enumStr;
   retCodeInfo->infoStr = retCodeInfoTable[i].infoStr;
}

/***************/
/* End of File */
/***************/

