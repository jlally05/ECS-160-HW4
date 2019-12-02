#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct User {
	char* name;
	int numTweets;
	struct User* next;
};

struct User* head; // global head to the linked list where we store the tweeters


void addTweeter(char* tweeter) {
	if (head == NULL) {
		head = malloc(sizeof(struct User));
		head->name = malloc(strlen(tweeter) * sizeof(char));
		strcpy(head->name,tweeter);
		head->numTweets = 1; 
		head->next = NULL;
		return;
	}
	struct User* prev = NULL;
	struct User* temp = head;
	// iterate through the linked list to see if it exists already
	while(temp != NULL) {
		if (strcmp(temp->name,tweeter) == 0) {
			temp->numTweets++;
			return;
		}
		prev = temp;
		temp = temp->next;
	}
	// if it doesn't already exist in the linked list
	struct User* newNode = malloc(sizeof(struct User));
	newNode->name = malloc(strlen(tweeter) *sizeof(char));
	strcpy(newNode->name,tweeter);
	newNode->numTweets = 1; 
	newNode->next = NULL;
	prev->next = newNode;
}



int parseHeader (char* line) {
	if (line == NULL || strlen(line) == 0) {
		return -1;
	}
	int length = strlen(line);
	int index = 0;
	int charIndex = 0;
	// checking to see if the first element in the csv header is name
	if (line[0] != ',') {
		char linecopy[length];
		strcpy(linecopy,line);
		char* tok = strtok(linecopy + charIndex, ",");
		if (tok == NULL) {
			return -1;
		}
		if (strcmp(tok, "name") == 0) { 
			return index;
		}
		else if (strcmp(tok, "\"name\"") == 0) {
			return index;
		}
	}

	while(charIndex < length - 1) {
		if (line[charIndex] == ',') {
			index++;
			if (line[charIndex + 1] != ',') {
				char linecopy[length];
				strcpy(linecopy,line);
				char* tok = strtok(linecopy + charIndex, ",");
				if (tok == NULL) {
					return -1;
				}
				if (strcmp(tok, "name") == 0) { 
					return index;
				}
				else if (strcmp(tok, "\"name\"") == 0) {
					return index;
				}
			}

		}
		charIndex++;
	}

	return -1;
}


int parseLine(char* line, int targetCol) {
	if (line == NULL) {
		return -1;
	}
	int numCommaSeen = 0;
	int charIndex = 0;
	int length = strlen(line);
	char* tok;

	if (length == 0) {
		return 0;
	}
	if (length == 1 && line[0] == '\n') {
		return 0;
	}
	// special case where the first entry is the name
	if (targetCol == 0) {
		if (line[0] == ',') {
			tok = "";
		}
		else {
			tok = strtok(line,",");
			if (tok == NULL) {
				return -1;
			}
		}	
		addTweeter(tok);
		return 0;
	}


	while (charIndex < length - 1) {
		if (line[charIndex] == ',') {
				numCommaSeen++;
		}

		if (numCommaSeen == targetCol) {
			tok = strtok(line + charIndex, ",");
			if (line[charIndex + 1] == ',') {
				tok = "";
			}
			if (tok == NULL) {
				return -1;
			}

			addTweeter(tok);
			return 0;
		}
		charIndex++;
	}

	return -1;
	
}

void printTopTen() {
	struct User topTweeters[10];
	for (int i = 0; i < 10; i++) {
		topTweeters[i].numTweets = 0;
	}
	struct User* temp = head;
	while(temp != NULL) {
		for (int i = 0; i < 10; i++) {
			if (temp->numTweets > topTweeters[i].numTweets) {
				// need to shift over all the other elements 
				for (int j = 9; j > i; j--) {
					topTweeters[j] = topTweeters[j-1];
				}
				topTweeters[i].name = temp->name;
				topTweeters[i].numTweets = temp->numTweets;
				break;
			}
		}

		temp = temp->next;
	}

	for (int i = 0; i < 10; i++) {
		if (topTweeters[i].numTweets == 0) {
			// only get here if we have less than 10 tweeters
			return;
		}
		printf("%s: %d\n",topTweeters[i].name, topTweeters[i].numTweets);
	}
	
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Invalid Input Format.\n"); 
		return 0;
	}

	FILE *filepointer = fopen(argv[1], "r");
	int numLines = 0;
	if (filepointer == NULL) {
		printf("Invalid Input Format\n");
		return 0;
	}

	char buffer[1024];
	int numItem = 0;
	int nameCol = 0;
	if (fgets(buffer, 1024, filepointer) == NULL) {
		printf("Invalid Input Format\n");
		return 0;
	}

	nameCol = parseHeader(buffer);
	if (nameCol == -1) {
		printf("Invalid Input Format\n");
		return 0;
	}

	while(fgets(buffer, 1024, filepointer)) {
		numLines++;
		if (numLines > 20000) {
			printf("Invalid Input Format\n");
			fclose(filepointer);
			return 0;
		}
		if (parseLine(buffer, nameCol) == -1) {
			printf("Invalid Input Format\n");
			fclose(filepointer);
			return 0;
		}
		
	}

	printTopTen();
	fclose(filepointer);
	return 0;
}
