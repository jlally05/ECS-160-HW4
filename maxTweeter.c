#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct User {
	char* name;
	int numTweets;
	struct User* next;
};

struct User* head;


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
	while(temp != NULL) {
		if (strcmp(temp->name,tweeter) == 0) {
			temp->numTweets++;
			return;
		}
		prev = temp;
		temp = temp->next;
	}

	struct User* newNode = malloc(sizeof(struct User));
	newNode->name = malloc(strlen(tweeter) *sizeof(char));
	strcpy(newNode->name,tweeter);
	newNode->numTweets = 1; 
	newNode->next = NULL;
	prev->next = newNode;
}


//global array of structs maybe?
//or a global linked list
//and we track the top ten dynamically?

// 0,1,2,3,4,5,6,7,8 (name)


int parseHeader (char* line) {
	// TODO: ADD checking 
	int index = 0;
	//printf("%s\n", line);
	char* tok = strtok(line, ",");
	
	
	while(tok != NULL) {
		if (strcmp(tok, "name") == 0) { // need to null terminate?
			return index;
		}
		tok = strtok(NULL, ",");

		index++;
	}

	return -1;
}


int parseLine(char* line, int targetCol) {
	int numCommaSeen = 0;
	int charIndex = 0;

	while(numCommaSeen < targetCol) {
		if (line[charIndex] == ',') {
			numCommaSeen++;
		}
		charIndex++;
	}

	char* tok = strtok(line + charIndex, ",");
	addTweeter(tok);

	//printf("%s\n",tok);
	//return 0;
	/*
	char *copyStr = malloc(sizeof(targetCol));
	strncpy(copyStr,line,targetCol+1);
	char *copyTok = strtok(copyStr, ",");
	if(copyTok == NULL) {
		return -1;
	}
	int indexCount = 0;
	int offset = 0;
	printf("%d\n", targetCol);
	while(indexCount != targetCol) {
		printf("%d", indexCount);
		printf("%s\n", copyTok);
		if(strcmp(copyTok,"\n")) {
			offset++;
		}
		copyTok = strtok(NULL, ",");
		indexCount++;
	}
	free(copyStr);
	
	char *tok = strtok(line, ",");
	
	int numItem = 1;
	while(tok != NULL) {
		if(numItem == (targetCol - offset)) {
			printf("Name:\t%s\n", tok);
			return 0;
		}
		//printf("%s\n", tok);
		tok = strtok(NULL, ",");
		//tok = strsep(&line,",");
		numItem++;
	}
	*/	
	return 0;
	
}

void printTopTen() {
	struct User topTweeters[10];
	for (int i = 0; i < 10; i++) {
		topTweeters[i].numTweets = 0;
	}
	struct User* temp = head;
	while(temp != NULL) {
		//printf("%s:%d\n", temp->name, temp->numTweets);
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
	//int argc, char** argv
	
	if (argc != 2) {

		printf("Invalid Input Format1.\n"); // not sure what to actually print
	}


	FILE *filepointer = fopen(argv[1], "r");
	
	if (filepointer == NULL) {
		printf("Invalid Input Format2");
		return 0;
	}

	char buffer[1024];
	int numItem = 0;
	int nameCol = 0;
	if (fgets(buffer, 1024, filepointer) == NULL) {
		printf("Invalid Input Format3");
		return 0;
	}

	nameCol = parseHeader(buffer);
	//printf("%d\n",nameCol);

	if (nameCol == -1) {
		printf("Invalid Input Format4");
		return 0;
	}

	while(fgets(buffer, 1024, filepointer)) {
		if (parseLine(buffer, nameCol) == -1) {
			printf("Invalid Input Format5");
			return 0;
		}
		//return 0; //FIXME

	}
	//printf("%s:%d\n", head->name, head->numTweets);
	printTopTen();


	fclose(filepointer);
	return 0;
}