#include <stdlib.h>
#include <string.h>
/*
 * Concatenate array safely into a string.
 * 
 * Input:
 *  dest		- Memory location to store string to
 * 	src			- Array containing words to be contained in strings
 * 	src_size	- Number of elements in array
 * 	dest_size	- Size of destination buffer
 * 	offset		- Offset in src where first word is contained
 */
void safe_concat(char *dest, char *src[], const size_t src_size, const size_t dest_size, int offset){
	for (int i = offset; i < src_size; i++){
		// Check length
		if (strlen(dest) < dest_size + 2) {
			strcat(dest, src[i]);
			// Check if last element
			if (i != src_size - 1){
				strcat(dest, " ");
			}
		} else {
			strncat(dest, src[i], dest_size - strlen(dest) - 1);
		}
	}
}
