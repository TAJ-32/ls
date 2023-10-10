#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

int ls_command(int a, int l, char *files[], int files_length);
int directories_handler(int a, int l, char *file);
int files_handler(int a, int l, char *file);

int main(int argc, char *argv[]) {

        /*
	String to hold name of the current directory, which shouldn't be longer than 256 characters 
	 
	 */

	char current_dir[256];
	
	if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
		perror("getcwd() error");
	}
	else {
		//current_dir = current_dir;
	}


	
	/*
	 * Collect All the non-option arguments (files and directories) and put them in an array
	 * for each of them, check if a is true then check if l is true. Then go to the ls function and pass those booleans to it, among other things
	 * In ls, if have a directory case and a file case
	 * for the directory case, for each element of the directory, add it to a list of dir_element filenames which will be of type char*
	 * 	- and increment a counter
	 * Then for loop through the list of dir_element filenames and do the stat stuff on it or whatever
	 */

	//For loop to get all of the directories listed in the arguments
	//
	char *files[argc];
	int files_length = 0;
	for (int i = 1; i < argc; i++) {

		if (argv[i][0] != '-') {
			files[files_length] = argv[i];
			files_length++;
		}
	}

	int opt;
	int a = 0;
	int l = 0;

	//Condition if myls is ran with no file arguments but has option arugments, puts currentdir inside Directories[]
	if ((files_length == 0)) {
		files[0] = current_dir;
		files_length += 1;
	}

	while ((opt = getopt(argc, argv, "al")) != -1) {

		switch (opt) {
			case 'a':
				a = 1;

			case 'l':
				l = 1;
		}

		ls_command(a, l, files, files_length);
	}

	return 0;
}

int ls_command(int a, int l, char *files[], int files_length) {

	for (int i = 0; i < files_length; i++) {
		printf("FILE BEING LSed: %s", files[i]);
		struct stat checker;
		stat(files[i], &checker);

		if (!S_ISREG(checker.st_mode)) { //directory case
			directories_handler(a, l, files[i]);
		}
		else {
			files_handler(a, l, files[i]);
		}

	}

	return 0;
}

int directories_handler(int a, int l, char *file) {

	printf("Directory being handled: %s\n", file);

	DIR *directory;
	struct dirent *dir_element;
	int num_elements = 0;
	
	struct passwd *username_access;
	struct passwd *group_access;
	struct tm *stattime;
	char timebuf[80];
	struct stat *buffer = malloc(sizeof(struct stat));
	
	if ((directory = opendir(file)) == NULL) {
		perror("Could not open directory");
	}
	else {
		while ((dir_element = readdir(directory)) != NULL) {
			if ((a == 0)) {
				printf("DIR ELEMENT: %s\n", (dir_element -> d_name));
				if ((dir_element -> d_name)[0] != '.') {
					num_elements++;
				}
			}
			else {
				num_elements++;
			}
		}
	}



	printf("GOT ELEMENTS: %d \n", num_elements);

	char *elem_names[num_elements];
	int counter = 0;

	while ((dir_element = readdir(directory)) != NULL) {
		if ((a == 0)) {
			if ((dir_element -> d_name)[0] != '.') {
				printf("DIR ELEMENT: %s\n", (dir_element -> d_name));
				strcpy(elem_names[counter], (dir_element -> d_name));
				counter++;
			}
		}
		else {
			strcpy(elem_names[counter], (dir_element -> d_name));
			counter++;
		}
	}

	for (int j = 0; j < num_elements; j++) {
		printf("ELEMENT: %s\n", elem_names[j]);
	}


	for (int i = 0; i < num_elements; i++) {
		char *filename = elem_names[i];
		stat(filename, buffer);
		
		if ((l == 1)) {
			printf("HELLO\n");
			username_access = getpwuid(buffer->st_uid);
			if (username_access == NULL) {
				perror("getpwuid");
			}
			else {}

			group_access = getpwuid(buffer->st_gid);
			if (group_access == NULL) {
				perror("getpwuid");
			}
			else {}

			stattime = localtime(&buffer->st_ctime);
			//Lists the directory permissions
			strftime(timebuf, 80, "%c", stattime);

			printf((S_ISDIR(buffer->st_mode)) ? "d" : "-");
			printf((S_IRUSR & (buffer->st_mode)) ? "r" : "-");
			printf((S_IWUSR & (buffer->st_mode)) ? "w" : "-");
			printf((S_IXUSR & (buffer->st_mode)) ? "x" : "-");
			printf((S_IRGRP & (buffer->st_mode)) ? "r" : "-");
			printf((S_IWGRP & (buffer->st_mode)) ? "w" : "-");
			printf((S_IXGRP & (buffer->st_mode)) ? "x" : "-");
			printf((S_IROTH & (buffer->st_mode)) ? "r" : "-");
			printf((S_IWOTH & (buffer->st_mode)) ? "w" : "-");
			printf((S_IXOTH & (buffer->st_mode)) ? "x" : "-");


			printf("	%lu	%s	%s	%jd	%s	%s\n", 
			buffer->st_nlink,
			username_access->pw_name, group_access->pw_name, 
			buffer->st_size, timebuf, filename);	
		}
		else {
			printf("%s	", filename);
		}
		printf("\n");

	}

	return 0;
}


int files_handler(int a, int l, char *file) {

	
	struct passwd *username_access;
	struct passwd *group_access;
	struct tm *stattime;
	char timebuf[80];
	struct stat *buffer = malloc(sizeof(struct stat));
	stat(file, buffer);
		
	if ((l == 1)) {
		printf("GOODBYE\n");
		username_access = getpwuid(buffer->st_uid);
		if (username_access == NULL) {
			perror("getpwuid");
		}
		else {}

		group_access = getpwuid(buffer->st_gid);
		if (group_access == NULL) {
			perror("getpwuid");
		}
		else {}

		stattime = localtime(&buffer->st_ctime);
		//Lists the directory permissions
		strftime(timebuf, 80, "%c", stattime);

		printf((S_ISDIR(buffer->st_mode)) ? "d" : "-");
		printf((S_IRUSR & (buffer->st_mode)) ? "r" : "-");
		printf((S_IWUSR & (buffer->st_mode)) ? "w" : "-");
		printf((S_IXUSR & (buffer->st_mode)) ? "x" : "-");
		printf((S_IRGRP & (buffer->st_mode)) ? "r" : "-");
		printf((S_IWGRP & (buffer->st_mode)) ? "w" : "-");
		printf((S_IXGRP & (buffer->st_mode)) ? "x" : "-");
		printf((S_IROTH & (buffer->st_mode)) ? "r" : "-");
		printf((S_IWOTH & (buffer->st_mode)) ? "w" : "-");
		printf((S_IXOTH & (buffer->st_mode)) ? "x" : "-");


		printf("	%lu	%s	%s	%jd	%s	%s\n", 
		buffer->st_nlink,
		username_access->pw_name, group_access->pw_name, 
		buffer->st_size, timebuf, file);	
	}
	else {
		printf("%s	", file);
	}
	printf("\n");

	return 0;
}
