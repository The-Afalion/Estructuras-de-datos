/**
 * @file types.h
 * @author Profesores EDAT
 * @date February 2026
 * @brief ADT Boolean and Status
 *
 * @details Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by @.
 *
 * @see
 */

#ifndef TYPES_H_
#define TYPES_H_

#define ERROR_I -1
/* START [STR_LENGTH] */
#define STR_LENGTH 64
/* END [STR_LENGTH] */
/**
 * @brief ADT Boolean
 */
typedef enum {
    FALSE=0, /*!< False value */
    TRUE=1  /*!< True value  */
} Bool;

/**
 * @brief ADT Status
 */
typedef enum {
    ERROR=0, /*!< To codify an ERROR output */
    OK=1     /*!< OK output */
} Status;

#endif /* TYPES_H_ */
