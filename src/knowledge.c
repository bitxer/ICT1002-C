/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"


Intent * start_where = NULL;
Intent * end_where = NULL;

Intent * start_what = NULL;
Intent * end_what = NULL;

Intent * start_who = NULL;
Intent * end_who = NULL;

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {
	Intent * current = NULL;

	if (compare_token(intent, WHAT) == 0) {
		current = start_what;

	} else if (compare_token(intent, WHERE) == 0) {
		current = start_where;

	} else if (compare_token(intent, WHO) == 0) {
		current = start_who;
	} else {
		// This should not happen since checking have been done previously
		// Included as safety net
		snprintf(response, n, "I don't understand \"%s\".", intent);
		return KB_INVALID;
	}
	
	while (current != NULL) {
		if (strcmp(current->entity, current->entity) == 0) {
			snprintf(response, n, "%s", current->response);
			return KB_OK;
		}
	}
	return KB_NOTFOUND;

}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {

	/* TODO: implement */

	// current = malloc(INTENTSIZE())
	// printf("inv[%d]=%s, %ld, ", 1, inv[1], sizeof(&inv)/sizeof(inv[0]));
	// for (int i = 1; i < inc; i++) {
	// 	if (i == 1 && (!compare_token(inv[1], "is") || !compare_token(inv[1], "are"))){
	// 		continue;
	// 	}
	// 	if (strlen(current->entity) + strlen(inv[i]) + 1 > MAX_ENTITY) {
	// 		printf("Entity is too large");
	// 		break;
	// 	}
	// 	printf("inv[%d]=%s, ", i, inv[i]);
	// 	strcat(current->entity, inv[i]);
	// 	current->entity[strlen(current->entity) + 1] = '\0';
	// 	current->entity[strlen(current->entity)] = ' ';
	// }

	// printf("entity=%s\n", current->entity);
	return KB_INVALID;

}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {

	/* TODO: implement */

	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

	/* TODO: implement */

}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {

	/* TODO: implement */

}
