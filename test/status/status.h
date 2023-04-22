#ifndef __STATUS_H__
#define __STATUS_H__

typedef enum
{
    STATUS_OK,
    STATUS_FAIL
} Status;

/*
* @brief returns a human-friendly description of an Status
* @param code Statuscode to get explanation from
* @return Human-friendly description of the status code
*/
const char* status_str(Status code);

#endif /* __STATUS_H__ */