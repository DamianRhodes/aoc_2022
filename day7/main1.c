#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LINE_LEN 100
#define MAX_FILE_COUNT 500
#define MAX_PATH_LEN 100
#define MAX_FILE_NAME_LEN  MAX_PATH_LEN

#define FOLDER_SIZE_TRESHOLD 100000



typedef enum command_name {
	UNKNOWN,
	CD,
	LS,
} command_name_e;

typedef struct command command_t;
struct command {
	command_name_e name;
	char path[MAX_FILE_NAME_LEN];
};


typedef struct file file_t;
struct file {
	char path[MAX_PATH_LEN];
	char name[MAX_FILE_NAME_LEN];
	unsigned size;
};

void file_init(file_t *f) {
	memset(&f->path, 0, MAX_LINE_LEN);
	f->size = 0;
}

command_t cmd_parse(char *line) {
	command_t cmd;
	char command_string[3];
	sscanf(line, "$ %s %s\n", command_string, cmd.path);
	//printf("%s", line);

	if (!strcmp(command_string, "cd")) {
		cmd.name = CD;
	} else if (!strcmp(command_string, "ls")) {
		cmd.name = LS;
	} else {
		assert(false);
		cmd.name = UNKNOWN;
	}

	return cmd;
}

file_t file_parse(const char *line, const char *dir_path) {
	file_t file;

	char file_name[MAX_FILE_NAME_LEN];
	sscanf(line, "%u %s\n", &file.size, file_name);

	// dont want to bother with strlen and such
	strcpy(file.path, dir_path);
	strcpy(file.name, file_name);

	return file;
}

char *last_occurence(char *str, char c) {
	char *occurance = NULL;
	for (size_t i = 0; i < strlen(str); i++) {
		if (str[i] == c) {
			occurance = str + i;
		}
	}
	return occurance;
}

void add_folder(
		file_t (*folders)[MAX_FILE_COUNT],
		size_t *folder_cnt,
		char dir_path[MAX_PATH_LEN]) {

	for (size_t i = 0; i < *folder_cnt; i++) {

		if (strcmp(dir_path, (*folders)[i].path) == 0) {
			return;
		}
	}

	file_t *new_folder = (*folders) + *folder_cnt;
	strcpy(new_folder->path, dir_path);
	//printf("folder %s\n", new_folder->path);
	(*folder_cnt)++;
}

void get_files(file_t (*files)[MAX_FILE_COUNT], size_t *file_cnt,
			   file_t (*folders)[MAX_FILE_COUNT], size_t *folder_cnt) {
	// not checking file_cnt vs MAX_FILE_COUNT
	FILE *f = stdin;
	char line[MAX_LINE_LEN] = {'0'};
	char cur_dir[MAX_PATH_LEN] = {'0'};
	while (fgets(line, MAX_LINE_LEN, f)) {

		command_t cur_command;

		// if its a command
		if (line[0] == '$') {
			cur_command = cmd_parse(line);
			if (cur_command.name == CD) {
				//printf("changing direcory %s\n", cur_command.path);
				// cd /
				if (cur_command.path[0] == '/') {
					//printf("%s", cur_command.path);
					strcpy(cur_dir, "/");

				// cd ..
				} else if (cur_command.path[0] == '.' && cur_command.path[1] == '.') {
					char *last_slash = last_occurence(cur_dir, '/');
					size_t tail_len = strlen(last_slash);
					memset(last_slash, 0, tail_len);
					//*last_slash = '\0';

				// cd DIRNAME
				} else {
					if (strlen(cur_dir) != 1) {
						strcat(cur_dir, "/");
					}
					strcat(cur_dir, cur_command.path);
					// add dir to dir_list
					add_folder(folders, folder_cnt, cur_dir);
				}
			}

		//if its a printed output
		} else if ( line[0] == 'd') {
			continue;
		} else {
			(*files)[*file_cnt] = file_parse(line, cur_dir);
			//file_t *f = &(*files)[*file_cnt];
			//printf("file %s | %s| %u\n", f->path, f->name, f->size);
			(*file_cnt)++;
		}
	}
}

unsigned get_dir_size(file_t files[MAX_FILE_COUNT], size_t file_cnt, const char dir_path[MAX_PATH_LEN]) {
	unsigned total_size = 0;

	for (size_t i = 0; i < file_cnt; i++) {

		file_t cur_file = files[i];

		if (strstr(cur_file.path, dir_path) == cur_file.path) {
			total_size += cur_file.size;
		}
	}
	return total_size;
}

int main() {
	file_t files[MAX_FILE_COUNT];
	size_t file_cnt = 0;
	file_t folders[MAX_FILE_COUNT];
	size_t folder_cnt = 0;

	//zero all files
	for (int i = 0; i < MAX_FILE_COUNT; i++) {
		file_init((file_t *)files + i);
		file_init((file_t *)folders + i);
	}

	//parse input
	get_files(&files, &file_cnt, &folders, &folder_cnt);

	unsigned folder_size_above_treshold_sum = 0;
	for (size_t i = 0; i < folder_cnt; i++) {
		unsigned folder_size = get_dir_size(files, file_cnt, folders[i].path);
		if (folder_size <= FOLDER_SIZE_TRESHOLD) {
			//printf("%s is below the treshold\n", folders[i].path);
			folder_size_above_treshold_sum += folder_size;
		}
	}


	printf("Total folders %zu\n", folder_cnt);
	printf("Total files %zu\n", file_cnt);
	printf("Total size %u\n", folder_size_above_treshold_sum);
}
