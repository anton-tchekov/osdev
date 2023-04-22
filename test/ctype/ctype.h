#ifndef __CTYPE_H__
#define __CTYPE_H__

/**
 * @brief This function checks whether the passed character is alphanumeric.
 */
int isalnum(int c);

/**
 * @brief This function checks whether the passed character is alphabetic.
 */
int isalpha(int c);

/**
 * @brief This function checks whether the passed character is
 *        a control character.
 */
int iscntrl(int c);

/**
 * @brief This function checks whether the passed character is decimal digit.
 */
int isdigit(int c);

/**
 * @brief This function checks whether the passed character has
 *        a graphical representation.
 */
int isgraph(int c);

/**
 * @brief This function checks whether the passed character is lowercase letter.
 */
int islower(int c);

/**
 * @brief This function checks whether the passed character is printable.
 */
int isprint(int c);

/**
 * @brief This function checks whether the passed character is
 *        a punctuation character.
 */
int ispunct(int c);

/**
 * @brief This function checks whether the passed character is white-space.
 */
int isspace(int c);

/**
 * @brief This function checks whether the passed character is
 *        an uppercase letter.
 */
int isupper(int c);

/**
 * @brief This function checks whether the passed character is
 *        a hexadecimal digit.
 */
int isxdigit(int c);

/**
 * @brief This function converts uppercase letters to lowercase.
 */
int tolower(int c);

/**
 * @brief This function converts lowercase letters to uppercase.
 */
int toupper(int c);

#endif /* __CTYPE_H__ */
