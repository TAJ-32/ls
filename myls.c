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

int ls_command(bool a, bool l, char *files[], int files_length);
int directories_handler(bool a, bool l, char *file);
int files_handler(bool a, bool l, char *file);
int L_Command_Dir(char *file, bool a, DIR *directory);
int L_Command_File(char *file, bool a);

//issue on putting in a file as an argument: IF they put the full path, like HELLO/hi.txt, it'll print HELLO/hi.txt when it should only print hi.txt


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
	bool a = false;
	bool l = false;

	//Condition if myls is ran with no file arguments but has option arugments, puts currentdir inside Directories[]
	if ((files_length == 0)) {
		files[0] = current_dir;
		files_length += 1;
	}

	while ((opt = getopt(argc, argv, "al")) != -1) {

		switch (opt) {
			case 'a':
				a = true;
				break;
			case 'l':
				//printf("L");
				l = true;
				break;			
		}
		//printf("HIIII\n");
		//ls_command(a, l, files, files_length);
	}
	ls_command(a, l, files, files_length);

	return 0;
}

int ls_command(bool a, bool l, char *files[], int files_length) {

	for (int i = 0; i < files_length; i++) {
		//printf("FILE BEING LSed: %s\n", files[i]);
		struct stat checker;
		stat(files[i], &checker);

		if (!S_ISREG(checker.st_mode)) { //directory case
			//printf("DIRECTORIES HANDLER\n");
			directories_handler(a, l, files[i]);
		}
		else {
			files_handler(a, l, files[i]);
		}

	}

	return 0;
}

int directories_handler(bool a, bool l, char *file) {

	//printf("Directory being handled: %s\n", file);

	DIR *directory;
	struct dirent *dir_element;

	if ((directory = opendir(file)) == NULL) {
		perror("Could not open directory");
	}
	else {
		if (l == false) {
			if (a == true) { //-a command
				while ((dir_element = readdir(directory)) != NULL) {
					printf("%s	", (dir_element -> d_name));
				}
			}
			else { //No option arguments
				while ((dir_element = readdir(directory)) != NULL) {
					if ((dir_element -> d_name)[0] != '.') {
						printf("%s	", (dir_element -> d_name));
					}
				}
			}
		}
		else {
			//printf("Gotta blast!\n");
			L_Command_Dir(file, a, directory);
		}
	}

	printf("\n");

	return 0;
}


int files_handler(bool a, bool l, char *file) {

			
	if ((l == true)) {
		L_Command_File(file, a);
	}
	else {
		printf("%s	\n", file);

	}

	printf("\n");

	return 0;
}


int L_Command_Dir(char *file, bool a, DIR *directory) {

	struct dirent *dir_element;
	
	struct passwd *username_access;
	struct passwd *group_access;
	struct tm *stattime;
	char timebuf[80];
	struct stat *buffer = malloc(sizeof(struct stat));

	if (a == true) {
		while ((dir_element = readdir(directory)) != NULL) {
			char *filename = dir_element -> d_name;
			stat(filename, buffer);

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
	}
	else {
		//printf("A\n");
		while ((dir_element = readdir(directory)) != NULL) {
			char *filename = dir_element -> d_name;
			stat(filename, buffer);
			
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
			if (filename[0] != '.') {
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

		}
	
	}

	return 0;

}

int L_Command_File(char *file, bool a) {

	struct passwd *username_access;
	struct passwd *group_access;
	struct tm *stattime;
	char timebuf[80];
	struct stat *buffer = malloc(sizeof(struct stat));

	stat(file, buffer);

	if (a == true) {

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
		if (file[0] != '.') {
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
	}

	return 0;
}
