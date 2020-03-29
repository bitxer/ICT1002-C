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
	if (chatbot_is_question(intent) == 0) {
		// This should not happen since checking have been done previously
		// Included as safety net
		snprintf(response, n, "I don't understand \"%s\".", intent);
		return KB_INVALID;
	}

	INTENT_PTR current = head;

	while (current != NULL) {
		if (compare_token(current->entity, entity) == 0) {
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
	if (chatbot_is_question(intent) == KB_INVALID) {
		return KB_INVALID;
	}
	int found = 0;
	INTENT_PTR current = head;
	while (current != NULL) {
		if (compare_token(current->entity, entity) == 0) {
			found = 1;
			break;
		}
		current = current->next;
	}

	if (current == NULL) {
		current = (INTENT_PTR) calloc(1, sizeof(INTENT));
		if (current == NULL) {
			return KB_NOMEM;
		}
		current->next = NULL;
	}

	if (!current) {
		return KB_NOMEM;
	}
	if (!found){
		snprintf(current->entity, MAX_ENTITY, "%s", entity);
	}

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
	} else if (!found) {
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

	int keypair = 0;

	int len = MAX_ENTITY + 1 + MAX_RESPONSE + 1;
	int linelen = 0;
	char * buf = calloc(1, len);

	if (buf == NULL) {
		return KB_NOMEM;
	}
	
	char section[6];
	int valid = 0;
	while (!feof(f)) {
		linelen = getline(&buf, (size_t *) &len, f);
		if (*buf == '[' && *(buf + linelen - 3) == ']') {
			buf = strtok(buf, "[");
			snprintf(section, 6, "%s", strtok(buf, "]"));
			valid = chatbot_is_question(section);
			continue;
		} else if (!valid) {
			continue;
		}

		if (!isalpha(buf[0])){
			continue;
		}

		char entity[MAX_ENTITY];
		memset(entity, 0, MAX_ENTITY);
		snprintf(entity, MAX_ENTITY, "%s", strtok(buf, "="));

		char response[MAX_RESPONSE];
		memset(response, 0, MAX_RESPONSE);
		snprintf(response, MAX_RESPONSE, "%s", strtok(NULL, "\r\n"));

		int result = knowledge_put(section, entity, response);
		if (result != KB_OK) {
			return result;
		}
		// reset variables
		memset(buf, 0, len);
		linelen = 0;
		keypair++;
	}

	// INTENT_PTR current = head;
	// while (current != NULL) {
	// 	printf("entity: %s\n", current->entity);
	// 	printf("what: %s\n", current->what);
	// 	printf("who: %s\n", current->who);
	// 	printf("where: %s\n", current->where);
	// 	printf("----------------------\n\n");
	// 	current = current->next;
	// }

	return keypair;
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
	int end_what = 7;
	int end_where = end_what + 1 + 8; 

	// int max_line_length = MAX_ENTITY + 1 + MAX_RESPONSE + 1;
	INTENT_PTR current = head;
	// Save what to file
	fputs("[what]\n", f);
	while (current != NULL) {
		if (current->what[0] != '\0') {
			fprintf(f, "%s=%s\n", current->entity, current->what);
		}
		current = current->next;
	}

	// Save where to file
	fputs("\n[where]\n", f);
	current = head;
	while (current != NULL) {
		if (current->where[0] != '\0') {
			fprintf(f, "%s=%s\n", current->entity, current->where);
		}
		current = current->next;
	}

	// Save who to file
	fputs("\n[who]\n", f);
	current = head;
	while (current != NULL) {
		if (current->who[0] != '\0') {
			fprintf(f, "%s=%s\n", current->entity, current->who);
		}
		current = current->next;
	}
}
