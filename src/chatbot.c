/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include "chat1002.h"


/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {
	return "Chatbot";
}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {
	return "User";
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	int *indx;
	*indx = -1;
	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {
	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {
	snprintf(response, n, "Goodbye!");
	return 1;
}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {
	return compare_token(intent, "load") == 0;
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {
	char * invalidfs = "Please enter a filename";
	if (inc < 2) {
		snprintf(response, n, "%s", invalidfs);
		return 0;
	}

	int indx = 1;
	if (compare_token(inv[1], "from") == 0){
		indx = 2;
	}

	if (indx == 2 && inc < 3) {
		snprintf(response, n, "%s", invalidfs);
		return 0;
	}
	
	FILE * fp;
	fp = fopen(inv[indx], "r");
	if (fp != NULL) {
		int result = knowledge_read(fp);
		fclose(fp);
		if (result == KB_NOMEM){
			snprintf(response, n, "Sorry, I ran out of memory");
		} else {
			snprintf(response, n, "Read %d responses from %s", result, inv[indx]);
		}
	} else {
		snprintf(response, n, "File %s not found", inv[indx]);
	}
	return 0;

}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {
	char reg_intent[3][6] = {WHAT, WHERE, WHO};
	for (int i = 0; i < 3; i++) {
		if (compare_token(reg_intent[i], intent) == 0) {
			return 1;
		}
	}
	return 0;
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {
	int result = 100;
	int indx = 0;
	if (inc > 1) {
		if (!compare_token(inv[1], "is") || !compare_token(inv[1], "are")){
			indx = 2;
		} else {
			indx = 1;
		}
		result = knowledge_get(inv[0], inv[indx], response, n);
	} else {
		snprintf(response, n, "Please ask a question with an entity.");
	}
	
	if (result == KB_NOTFOUND) {
		// Rebuild question to be displayed
		char question[MAX_INPUT] = "";
		int len = 0;
		for (int i = 0; i < inc; i++) {
			len += strlen(inv[i]) + 1;
			strcat(question, inv[i]);
			question[len] = '\0';
			question[len - 1] = ' ';
		}
		// Remove space
		question[len - 1] = '\0';

		// Capitalise first word in question
		question[0] = toupper(question[0]);
		char ans[MAX_RESPONSE];
		prompt_user(ans, MAX_RESPONSE, "I don't know. %s?", question);
		result = knowledge_put(inv[0], inv[indx] , ans);
		if (result == KB_OK){
			snprintf(response, MAX_RESPONSE, "Thank you.");
		} else if (result == KB_NOMEM) {
			snprintf(response, MAX_RESPONSE, "Insfficient memory space");
		} else if (result == KB_INVALID) {
			snprintf(response, MAX_RESPONSE, "Invalid intent specified");
		}
	}
	return 0;

}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {
	if (!compare_token("RESET", intent)) {
		return 1;
	}
	return 0;

}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {

	/* TODO: implement */

	return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {
	return compare_token(intent, "save") == 0;
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {
	char * invalidfs = "Please enter a filename";
	if (inc < 2) {
		snprintf(response, n, "%s", invalidfs);
		return 0;
	}

	int indx = 1;
	if (compare_token(inv[1], "as") == 0 || compare_token(inv[1], "to") == 0){
		indx = 2;
	}

	if (indx == 2 && inc < 3) {
		snprintf(response, n, "%s", invalidfs);
		return 0;
	}
	
	char * filename = inv[indx];
	if (filename[0] == 0) {
		snprintf(response, n, "%s", invalidfs);
		return 0;
	}
	FILE * file;
	file = fopen(filename, "r");
	if (file != NULL) {
		char ans[3];
		prompt_user(ans, 3, "%s is present. Do you want to overwrite it? [Y/n]", filename);
		fclose(file);

		switch (ans[0]) {
			case 'n':
				snprintf(response, n, "My knowledge is not saved as the file provided exists");
				return 0;
			case 'Y':
				break;
			default:
				snprintf(response, n, "My knowledge is not saved as the file exists and I do not understand the response.");
				return 0;

		}
	}
	file = fopen(filename, "w");
	knowledge_write(file);
	fclose(file);
	
	snprintf(response, n, "My knowledge has been saved to %s", filename);

	return 0;

}


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {
	return compare_token("Hello", intent) == 0 ||
			compare_token("It's", intent) == 0 || 
			compare_token("Good", intent) == 0 ||
			compare_token("Goodbye", intent) == 0;
}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
		if (compare_token("Hello", inv[0]) == 0){
			snprintf(response, n, "Hello");
		} else if (compare_token("It's", inv[0]) == 0) {
			snprintf(response, n, "Indeed it is");
		} else if (compare_token("Good", inv[0]) == 0) {
			snprintf(response, n, "Good %s", inv[1]);
		} else if (compare_token("Goodbye", inv[0]) == 0) {
			snprintf(response, n, "Goodbye");
			return 1;
		}
	return 0;
}
