#ifndef LIBPAS_ACP_MAIN_H
#define LIBPAS_ACP_MAIN_H

#include <netinet/in.h>

#include "../dstructure_auto.h"

//#define ACP_CMD_MAX_LENGTH 16
#define ACP_BUF_MAX_LENGTH 256
#define ACP_DATA_MAX_LENGTH (ACP_BUF_MAX_LENGTH - 5)
#define ACP_PACK_MIN_LENGTH 6
#define ACP_TCP_PACK_MIN_LENGTH ACP_PACK_MIN_LENGTH - 1
#define ACP_WBUF_MAX_LENGTH 128
#define ACP_CRC_BUF_MAX_LENGTH 2
#define ACP_CHANNEL_ID_STRLEN 12
#define ACP_INT_STR_MAX_LEN 12
#define ACP_FLOAT_STR_MAX_LEN 16
#define ACP_CRC_INI 33
#define ACP_CRC_EXTRA 11

#define ACP_CHECK_CRC 1

#define ACP_PACK_TIMEOUT_MS 300UL
#define ACP_BUSY_TIMEOUT_MS 5UL

#define ACP_DELIMITER_START						'<'
#define ACP_DELIMITER_COLUMN					';'
#define ACP_DELIMITER_END						'>'

#define ACP_DELIMITER_START_STR					"<"
#define ACP_DELIMITER_COLUMN_STR				";"
#define ACP_DELIMITER_END_STR					">"

#define ACP_SIGN_REQUEST_GET					'?' //response required
#define ACP_SIGN_REQUEST_SET					'&' //no response required 
#define ACP_SIGN_REQUEST_GET_BROADCAST			'*' //for everyone in the net, response required
#define ACP_SIGN_REQUEST_SET_BROADCAST			'#' //for everyone in the net, no response required
#define ACP_SIGN_RESPONSE						'!' //response to request

#define ACP_SIGN_REQUEST_GET_STR				"?"
#define ACP_SIGN_REQUEST_SET_STR				"&"
#define ACP_SIGN_REQUEST_GET_BROADCAST_STR		"*"
#define ACP_SIGN_REQUEST_SET_BROADCAST_STR		"#"
#define ACP_SIGN_RESPONSE_STR					"!"

#define ACP_EMPTY_PACK_STR			ACP_DELIMITER_START_STR ACP_DELIMITER_END_STR

#define ADSS ACP_DELIMITER_START_STR
#define ADCS ACP_DELIMITER_COLUMN_STR 
#define ADES ACP_DELIMITER_END_STR
#define ASREQ ACP_SIGN_REQUEST_STR
#define ASRES ACP_SIGN_RESPONSE_STR

#define ACP_IND_SIGN				1

#define ACP_REQUEST_IND_CMD			1
#define ACP_REQUEST_IND_ID			2
#define ACP_REQUEST_IND_PARAM1		3
#define ACP_REQUEST_IND_PARAM2		4
#define ACP_REQUEST_IND_PARAM3		5
#define ACP_REQUEST_IND_PARAM4		6

#define ACP_RESPONSE_IND_ID			1
#define ACP_RESPONSE_IND_PARAM1		2
#define ACP_RESPONSE_IND_PARAM2		3
#define ACP_RESPONSE_IND_PARAM3		4
#define ACP_RESPONSE_IND_PARAM4		5

enum ACPResultE {
	ACP_BUSY,
	ACP_SUCCESS,
	ACP_DONE,
	ACP_NOT_FOUND,
	ACP_SENDING_REQUIRED,
	ACP_ERROR,
	ACP_ERROR_BAD_CHANNEL_ID,
	ACP_ERROR_WAIT_RESPONSE_TIMEOUT,
	ACP_ERROR_PACK_TIMEOUT,
	ACP_ERROR_BUFFER_OVERFLOW,
	ACP_ERROR_NO_START_DELIMITER,
	ACP_ERROR_SHORT_PACKAGE,
	ACP_ERROR_FORMAT,
	ACP_ERROR_WRITE,
	ACP_ERROR_READ,
	ACP_ERROR_CRC,
	ACP_ERROR_CONNECTION,
	ACP_ERROR_NO_RESPONSE,
	ACP_ERROR_SHORT_RESPONSE,
	ACP_ERROR_DEVICE_DISCONNECTED,
	ACP_NOISE,
	ACP_NO_DATA
};


#define ACP_PEER_ID_LENGTH  16
#define ACP_PEER_ADDR_STR_LENGTH  48


#define ACP_CHANNEL_EXISTS 1

typedef struct {
    char id[ACP_PEER_ID_LENGTH];
    char addr_str[ACP_PEER_ADDR_STR_LENGTH];
    int iid;
    int port;
    int fd;
    int fd_conn;
    struct sockaddr_in addr;
    socklen_t addr_size;
} Peer;
DEC_LIST(Peer)


extern void acp_dumpBuf ( const char *buf, size_t buf_size );

extern int acp_packGetCellS (const char *pack_str, int cell_ind, char *out, size_t out_len);

extern int acp_packGetCellI (const char *pack_str, int cell_ind, int *out);

extern int acp_packGetCellUl(const char *pack_str, int cell_ind, unsigned long *out);

extern int acp_packGetCellF (const char *pack_str, int cell_ind, double *out);

extern int acp_checkStrColumnChar(char v);

#endif 

