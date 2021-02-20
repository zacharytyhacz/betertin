#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// my first c program

// 1. parse setup configuration environment
// 2. parse arguments
// 3. store as simple csv
//
// each sheet is own csv file
//
// index,in,out,message


FILE *config = NULL;
char *config_path = "";

char *CONFIG_SHEETS_FOLDER;
char *CONFIG_CURRENT_SHEET;
char *CONFIG_EDITOR_COMMAND;
char *CONFIG_TIME_FORMAT;

bool empty(const char *text) {
    return text[0] == '\0';
}

bool prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

int error(char *message) {
    fprintf(stderr, message);
    return 1;
}

bool config_initialize(const char *path) {
    FILE *init_config = fopen(path,"a");

    if (!init_config) {
        return false;
    }

    fprintf(init_config, "sheetsfolder=/home/zac/.config/betertin/sheets/\n");
    fprintf(init_config, "currentsheet=\n");
    fprintf(init_config, "editorcommand=vim\n");
    fprintf(init_config, "timeformat=\n");

    fclose(init_config);

    return true;
}

bool config_load() {
    int bufferLength = 255;
    char config_line[bufferLength];
    char config_delim[] = "=";

    if ( empty(config_path) ){
        config_path = strcat(getenv("HOME"), "/.config/betertin/config");
    }

    config = fopen(config_path,"r");

    if (!config && !config_initialize(config_path)) {
        error("Failed to inititalize the config\n\n");
        return false;
    }

    while(fgets(config_line, bufferLength, config)) {
        char *config_key = strtok(config_line, config_delim);
        char *config_value = strtok(NULL, config_delim);

        if (strcmp("sheetsfolder", config_key) == 0) {
            // CONFIG_SHEETS_FOLDER = config_value;
            CONFIG_SHEETS_FOLDER = malloc(strlen(config_value) + 1);
            strcpy(CONFIG_SHEETS_FOLDER, config_value);
            printf("\nconfig sheets folder: %s", CONFIG_SHEETS_FOLDER);
        }

        if (strcmp("currentsheet", config_key) == 0) {
            CONFIG_CURRENT_SHEET = config_value;
            printf("\nconfig current sheet: %s", CONFIG_CURRENT_SHEET);
        }

        if (strcmp("editorcommand", config_key) == 0) {
            CONFIG_EDITOR_COMMAND = config_value;
            printf("\nconfig edior command: %s", CONFIG_EDITOR_COMMAND);
        }

        if (strcmp("timeformat", config_key) == 0) {
            CONFIG_TIME_FORMAT = config_value;
            printf("\ntime format: %s", CONFIG_TIME_FORMAT);
        }
    }

    printf("\nconfig sheets folder: %s", CONFIG_SHEETS_FOLDER);
    return true;
}

void parse_flag(const char *flag, const char *flag_argument) {
    printf("\n\n got a flag: %s", flag);
    printf("\n\n got flag's argument: %s", flag_argument);

    if (strcmp("--config", flag) == 0) {
        printf("got a config flag %s", flag_argument);

        *config_path = *flag_argument;
    }
}

void sheet_show() {
    if( empty(CONFIG_CURRENT_SHEET)) {
        printf("\nNo sheet currently selected.\n\n");
    } else {
        printf("\nCurrent sheet: %s\n\n", CONFIG_CURRENT_SHEET);
    }
}

void sheet_create(const char *new_sheet_name) {
    if (empty(CONFIG_SHEETS_FOLDER)) {
        error("No sheets folder specified in config.");
        return;
    }

    char *sheet_name = "";
    sheet_name = strcat(sheet_name, new_sheet_name);

    //sheet_name = strcat(sheet_name, ".csv");

    printf("\n\n new sheet name: %s", sheet_name);

    FILE *new_sheet = fopen(strcat(CONFIG_SHEETS_FOLDER, sheet_name),"a");

    if (!new_sheet) {
        printf("Cannot create new sheet in %s", CONFIG_SHEETS_FOLDER);
        return;
    }

    fprintf(new_sheet, "index,in,out,message");
    fclose(new_sheet);
}

int execute_command(const char *command, const char *command_argument) {
    if(strcmp(command, "in") == 0
    || strcmp(command, "i") == 0) {
        printf("\n time in command");
        return 0;
    }

    if(strcmp(command, "out") == 0
    || strcmp(command, "o") == 0) {
        printf("\n time out command");
        return 0;
    }

    if(strcmp(command, "sheet") == 0
    || strcmp(command, "s") == 0) {
        printf("\nconfig sheets folder: %s", CONFIG_SHEETS_FOLDER);
        if (empty(command_argument)) {
            sheet_show();
        } else {
            sheet_create(command_argument);
        }
        return 0;
    }

    if(strcmp(command, "append") == 0
    || strcmp(command, "a") == 0) {
        printf("\n append message command");
        return 0;
    }

    if(strcmp(command, "edit") == 0
    || strcmp(command, "e") == 0) {
        printf("\n edit sheet command");
        return 0;
    }

    if(strcmp(command, "display") == 0
    || strcmp(command, "d") == 0) {
        printf("\n display time sheet command");
        return 0;
    }

    if(strcmp(command, "help") == 0
    || strcmp(command, "h") == 0) {
        printf("\n display help command");
        return 0;
    }

    printf("\n %s is an invalid command", command);
    return 1;
}

int main(int argc, char *argv[]) {
    int result = 0;
    char *command = "";
    char *command_argument = "";

    for(int i = 1; i < sizeof(&argv); i++) {
        char *prev_arg = argv[i - 1];
        char *this_arg = argv[i];

        if(this_arg == NULL){
            break;
        }

        if(prefix("--", this_arg)) {
            parse_flag(this_arg, argv[i + 1]);
            continue;
        }

        if(!prefix("--", prev_arg)) {
            if (empty(command)) {
                command = this_arg;
                printf("\n\n COMMAND: %s\n", command);
            } else if (empty(command_argument)) {
                command_argument = this_arg;
                printf("\n\n COMMAND ARG: %s\n", command_argument);
            } else {
                printf("\n\nERROR: Unknown command '%s'\n\n", this_arg);
                return error("Invalid command given.\n");
            }
        }
    }

    if (empty(command)) {
        return error("Invalid command given.\n");
    }

    config_load();

    result = execute_command(command, command_argument);
    fclose(config);
    return result;
}
