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


INTENT_PTR head = NULL;
INTENT_PTR end = NULL;
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
	if (is_valid_intent(intent) == KB_INVALID) {
		// This should not happen since checking have been done previously
		// Included as safety net
		snprintf(response, n, "I don't understand \"%s\".", intent);
		return KB_INVALID;
	}

	INTENT_PTR current = head;

	while (current != NULL) {
		if (strcmp(current->entity, entity) == 0) {
			char * buf = NULL;;
			if (compare_token(intent, WHAT) == 0) {
				buf = current->what;
			} else if (compare_token(intent, WHERE) == 0) {
				buf = current->where;
			} else if (compare_token(intent, WHO) == 0) {
				buf = current->who;
			}
			if (buf[0] == '\0') {
				return KB_NOTFOUND;
			}
			snprintf(response, n, "%s", buf);
			return KB_OK;
		}
		current = current->next;
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
 *   KB_OK, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
	if (is_valid_intent(intent) == KB_INVALID) {
		return KB_INVALID;
	}

	INTENT_PTR current = head;

	while (current != NULL) {
		if (strcmp(current->entity, entity) == 0) {
			break;
		}
		current = current->next;
	}
	if (current == NULL) {
		current = (INTENT_PTR) malloc(sizeof(INTENT));
		current->entity = (char *) calloc(1, MAX_ENTITY * sizeof(char));
		current->who = (char *) calloc(1, MAX_RESPONSE * sizeof(char));
		current->what = (char *) calloc(1, MAX_RESPONSE * sizeof(char));
		current->where = (char *) calloc(1, MAX_RESPONSE * sizeof(char));
	}

	if (!current) {
		return KB_NOMEM;
	}

	snprintf(current->entity, MAX_ENTITY, "%s", entity);
	if (compare_token(intent, WHAT) == 0) {
		snprintf(current->what, MAX_ENTITY, "%s", response);
	} else if (compare_token(intent, WHERE) == 0) {
		snprintf(current->where, MAX_ENTITY, "%s", response);
	} else if (compare_token(intent, WHO) == 0) {
		snprintf(current->who, MAX_ENTITY, "%s", response);
	}

	if (head == NULL) {
		head = current;
		end = head;
	} else {
		end->next = current;
		end = current;
	}
	return KB_OK;
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
