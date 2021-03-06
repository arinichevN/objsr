

#include "../dstructure_auto.h"
#include "../app.h"
#include "../timef.h"

#include "../util.h"

#include "ACP.h"

int acp_goodCmdChar (int v) {
  if ((v >= 65 && v <= 90) || (v >= 97 && v <= 122)) {
    return 1;
  }
  return 0;
}

void acp_dumpBuf ( const char *buf, size_t buf_size ) {
    for ( int i = 0; i < buf_size; i++ ) {
        printf ( "%hhu.", buf[i] );
        if ( buf[i] == '\0' ) {
            putchar ( '\n' );
            return;
        }
    }
    putchar ( '\n' );
}

static size_t acp_gotoCell(const char *pack_str, int cell_ind){
	int c = 0;
	size_t l = strlen(pack_str);
	size_t i;
	for(i = 1; i < l; i++){
		if(c == cell_ind){
			break;
		}
		if(pack_str[i] == ACP_DELIMITER_COLUMN || pack_str[i] == ACP_DELIMITER_END){
			c++;
		}
	}
	return i;
}

int acp_packGetCellS (const char *pack_str, int cell_ind, char *out, size_t out_len){
	size_t i1 = acp_gotoCell(pack_str, cell_ind);
	size_t l = strlen(pack_str);
	size_t j = 0;
	for(size_t i = i1; i < l; i++){
		char c = pack_str[i];
		if( c == ACP_DELIMITER_COLUMN || pack_str[i] == ACP_DELIMITER_END){
			if(i > 1){return 1;} else {return 0;}
		}
		if(!acp_goodCmdChar((int) c)){
			return 0;
		}
		if(j < out_len){
			out[j] = c;j++;
		}else{
			return 0;
		}
	}
	if(j>0) return 1;
	return 0;
}

int acp_packGetCellI (const char *pack_str, int cell_ind, int *out){
	size_t i1 = acp_gotoCell(pack_str, cell_ind);
	char buf[ACP_INT_STR_MAX_LEN];
	memset(buf, 0, sizeof buf);
	size_t l = strlen(pack_str);
	size_t j = 0;
	for(size_t i = i1; i < l; i++){
		char c = pack_str[i];
		if( c == ACP_DELIMITER_COLUMN || pack_str[i] == ACP_DELIMITER_END){
			if(i > 1){
				//printdo("int buffer: %s\n", buf);
				int n = sscanf(buf, "%d", out);
				if (n == 1){
					return 1;
				}
			} else {
				return 0;
			}
		}
		if(j < (ACP_INT_STR_MAX_LEN - 1)){
			buf[j] = c;j++;
		}else{
			return 0;
		}
	}
	
	return 0;
}

int acp_packGetCellUl(const char *pack_str, int cell_ind, unsigned long *out){
	size_t i1 = acp_gotoCell(pack_str, cell_ind);
	char buf[ACP_INT_STR_MAX_LEN];
	memset(buf, 0, sizeof buf);
	size_t l = strlen(pack_str);
	size_t j = 0;
	for(size_t i = i1; i < l; i++){
		char c = pack_str[i];
		if( c == ACP_DELIMITER_COLUMN || pack_str[i] == ACP_DELIMITER_END){
			if(i > 1){
				int n = sscanf(buf, "%lu", out);
				if (n == 1){
					return 1;
				}
			} else {return 0;}
		}
		if(j < (ACP_INT_STR_MAX_LEN - 1)){
			buf[j] = c;j++;
		}else{
			return 0;
		}
	}
	
	return 0;
}

int acp_packGetCellF (const char *pack_str, int cell_ind, double *out){
	size_t i1 = acp_gotoCell(pack_str, cell_ind);
	char buf[ACP_INT_STR_MAX_LEN];
	memset(buf, 0, sizeof buf);
	size_t l = strlen(pack_str);
	size_t j = 0;
	for(size_t i = i1; i < l; i++){
		char c = pack_str[i];
		if( c == ACP_DELIMITER_COLUMN || pack_str[i] == ACP_DELIMITER_END){
			if(i > 1){
				//printdo("float buffer: %s\n", buf);
				int n = sscanf(buf, "%lf", out);
				if (n == 1){
					return 1;
				}
			} else {
				return 0;
			}
		}
		if(j < (ACP_FLOAT_STR_MAX_LEN - 1)){
			buf[j] = c;j++;
		}else{
			return 0;
		}
	}
	
	return 0;
}

int acp_checkStrColumnChar(char v){
	if(v >= '0' && v <= '9') return 1;
	if(v >= 'a' && v <= 'z') return 1;
	if(v >= 'A' && v <= 'Z') return 1;
	return 0;
}

const char *acp_getResultStr(int v){
	switch(v){
		case ACP_BUSY:								return "BUSY";
		case ACP_SUCCESS:							return "SUCCESS";
		case ACP_DONE:								return "DONE";
		case ACP_NOT_FOUND:							return "NOTFOUND";
		case ACP_SENDING_REQUIRED:					return "SENDING?";
		case ACP_ERROR:								return "ERROR";
		case ACP_ERROR_BAD_CHANNEL_ID:				return "EBADID";
		case ACP_ERROR_WAIT_RESPONSE_TIMEOUT:		return "ERESPTOUT";
		case ACP_ERROR_PACK_TIMEOUT:				return "EPACKTOUT";
		case ACP_ERROR_BUFFER_OVERFLOW:				return "EBUFFOVER";
		case ACP_ERROR_NO_START_DELIMITER:			return "ENOSTARTDELIM";
		case ACP_ERROR_SHORT_PACKAGE:				return "ESHORTPACK";
		case ACP_ERROR_FORMAT:						return "EFORMAT";
		case ACP_ERROR_WRITE:						return "EWRITE";
		case ACP_ERROR_READ:						return "EREAD";
		case ACP_ERROR_CRC:							return "ECRC";
		case ACP_ERROR_CONNECTION:					return "ECONN";
		case ACP_ERROR_NO_RESPONSE:					return "ENORESP";
		case ACP_ERROR_SHORT_RESPONSE:				return "ESHORTRESP";
		case ACP_ERROR_DEVICE_DISCONNECTED:			return "EDEVDISCON";
		case ACP_NOISE:								return "NOISE";
		case ACP_NO_DATA:							return "NODATA";
	}
	return "?";
}
#include "data.c"







