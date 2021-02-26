#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

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

char CONFIG_DELIM[1] = "=";
char *CONFIG_SHEETS_FOLDER;
char *CONFIG_CURRENT_SHEET;
char *CONFIG_EDITOR_COMMAND;
char *CONFIG_TIME_FORMAT;

bool empty(const char *text) {
    if (!text) {
        return true;
    }
    return text[0] == '\0';
}

bool prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

int error(char *message) {
    fprintf(stderr, message);
    return 1;
}

unsigned int convert(char *st) {
  char *x;
  for (x = st ; *x ; x++) {
    if (!isdigit(*x))
      return 0L;
  }
  return (strtoul(st, 0L, 10));
}

void strip_whitespace(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

void config_set_key(FILE *opened_config, char *key, char *value) {
    fprintf(opened_config,"%s%s%s\n",key,CONFIG_DELIM, value);
}

bool config_initialize(const char *path) {
    FILE *init_config = fopen(path, "w");

    if (init_config == NULL) {
        return false;
    }

    config_set_key(init_config, "sheetsfolder" , "/home/zac/.config/betertin/sheets/");
    config_set_key(init_config, "currentsheet" , "");
    config_set_key(init_config, "editorcommand", "vim");
    config_set_key(init_config, "timeformat"   , "%FT%TZ");

    fclose(init_config);

    return true;
}

char* config_default_path() {
    char *default_path = "/.config/betertine/config";
    
    // create folder betertin
    // create folder sheets/ in betertin folder

    char *default_config_path = malloc(strlen(getenv("HOME")) + 1);
    default_config_path = malloc(strlen(default_config_path) + strlen(default_path) + 1);

    strcat(default_config_path, getenv("HOME"));
    strcat(default_config_path, default_path);

    // printf("ok config default path: %s\n\n", default_config_path);
    return default_config_path;
}

bool config_load() {
    int bufferLength = 255;
    char config_line[bufferLength];

    if ( empty(config_path) ){

        // printf("config path is empty so default. \n\n\n");

        // char *default_config_path = config_default_path();
        // config_path = malloc(strlen(default_config_path));
        
        config_path = malloc(strlen("/home/zac/.config/betertin/config") + 1);
        strcpy(config_path, "/home/zac/.config/betertin/config");
    }
    
    config = fopen(config_path, "r");

    if (config == NULL){
        if(!config_initialize(config_path)) {
            return error("Failed to inititalize the config\n\n");
        }
        config = fopen(config_path, "r");
    }

    while(fgets(config_line, bufferLength, config)) {
        strip_whitespace(config_line);

        char *config_key = strtok(config_line, CONFIG_DELIM);
        char *config_value = strtok(NULL, CONFIG_DELIM);

        if (config_value == NULL) {
            config_value = malloc(1);
            config_value = "";
        }

        if (strcmp("sheetsfolder", config_key) == 0) {
            CONFIG_SHEETS_FOLDER = malloc(strlen(config_value) + 1);
            strcpy(CONFIG_SHEETS_FOLDER, config_value);
        }

        if (strcmp("currentsheet", config_key) == 0) {
            CONFIG_CURRENT_SHEET = malloc(strlen(config_value) + 1);
            strcpy(CONFIG_CURRENT_SHEET, config_value);
        }

        if (strcmp("editorcommand", config_key) == 0) {
            CONFIG_EDITOR_COMMAND = malloc(strlen(config_value) + 1);
            strcpy(CONFIG_EDITOR_COMMAND, config_value);
        }

        if (strcmp("timeformat", config_key) == 0) {
            CONFIG_TIME_FORMAT = malloc(strlen(config_value) + 1);
            strcpy(CONFIG_TIME_FORMAT, config_value);
        }
    }

    printf("\n-----------debug-------------------\n");
    printf("CONFIG_SHEETS_FOLDER : %s \n", CONFIG_SHEETS_FOLDER);
    printf("CONFIG_CURRENT_SHEET : %s \n", CONFIG_CURRENT_SHEET);
    printf("CONFIG_EDITOR_COMMAND: %s \n", CONFIG_EDITOR_COMMAND);
    printf("CONFIG_TIME_FORMAT   : %s \n", CONFIG_TIME_FORMAT);
    printf("-----------------------------------\n");

    return true;
}

void parse_flag(const char *flag, const char *flag_argument) {
    if (strcmp("--config", flag) == 0) {
        *config_path = *flag_argument;
    }
    if (strcmp("--help", flag) == 0) {
        printf("\n\nshow help\n");
    }
}

int sheet_change(const char *new_current_sheet) {
    // check if new sheet is same as current
    if(strcmp(new_current_sheet, CONFIG_CURRENT_SHEET) == 0) {
        printf("Already on the time sheet '%s'\n", new_current_sheet);
        return 1;
    }

    // open config
    if (config == NULL) {
        printf("Something went wrong trying to open your config.\n");
        return 1;
    }

    // close read-only config, reopen to write new config
    fclose(config);
    config = fopen(config_path, "w");

    // set current time sheet to new current sheet
    CONFIG_CURRENT_SHEET = malloc(strlen(new_current_sheet) + 1);
    strcpy(CONFIG_CURRENT_SHEET, new_current_sheet);

    // rewrite and save the config
    config_set_key(config, "sheetsfolder",  CONFIG_SHEETS_FOLDER);
    config_set_key(config, "currentsheet",  CONFIG_CURRENT_SHEET);
    config_set_key(config, "editorcommand", CONFIG_EDITOR_COMMAND);
    config_set_key(config, "timeformat",    CONFIG_TIME_FORMAT);

    fclose(config);

    // reopen as read-only safely
    config = fopen(config_path,"r");

    printf("\n\nSwitched to sheet '%s'\n\n", new_current_sheet);
    return 0;
}

int sheet_show() {
    if( empty(CONFIG_CURRENT_SHEET)) {
        printf("\nNo sheet currently selected.\n\n");
        return 1;
    } else {
        printf("\nCurrent sheet: %s\n\n", CONFIG_CURRENT_SHEET);
        return 0;
    }
}

int sheet_create(const char *new_sheet_name) {
    if (empty(CONFIG_SHEETS_FOLDER)) {
        error("No sheets folder specified in config.");
        return 1;
    }

    char sheet_name[128];

    strcpy(sheet_name, CONFIG_SHEETS_FOLDER);
    strcat(sheet_name, new_sheet_name);
    strcat(sheet_name, ".csv");

    strip_whitespace(sheet_name);

    FILE *new_sheet = fopen(sheet_name,"w");

    if (!new_sheet) {
        printf("Cannot create new sheet in %s", CONFIG_SHEETS_FOLDER);
        return 1;
    }

    fprintf(new_sheet, "index,in,out,message");
    fclose(new_sheet);

    return sheet_change(new_sheet_name);
}

char *parse_time_record(char* in_line, char *key) {
    char* line = malloc(strlen(in_line) + 1);

    strcpy(line, in_line);

    // printf("\n\n full line: %s", line);
    char *index = strtok(line, ",");

    // printf("\n\n fetching key: %s", key);
    if (strcmp(key, "index") == 0) {
        // printf("\n\n returning: %s", index);
        return index;
    }

    char *time_in = strtok(NULL, ",");
    if (strcmp(key, "in") == 0) {
        return time_in;
    }

    char *time_out = strtok(NULL, ",");
    if (strcmp(key, "out") == 0) {
        return time_out;
    }

    char *last_message = strtok(NULL, ",");
    if (strcmp(key, "message") == 0) {
        return last_message;
    }

    return "invalid";
}

void get_now(char* out_time) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    //strftime(out_time, 32, "%Y-%m-%d %H:%M:%S", t);
    // strftime(out_time, 32, "%FT%TZ", t);
    strftime(out_time, 32, CONFIG_TIME_FORMAT, t);
}

int time_in(const char *message) {
    if(empty(CONFIG_CURRENT_SHEET)) {
        return error("\nNo time sheet selected.\n");
    }

    char *sheet_path = malloc(128);
    strcat(sheet_path,CONFIG_SHEETS_FOLDER);
    strcat(sheet_path,CONFIG_CURRENT_SHEET);
    strcat(sheet_path, ".csv");

    // check sheet file and last entered record
    char line[1024]={0,};
    FILE *check = fopen(sheet_path, "r");

    if(check == NULL) {
        return error("\n\nFailed to open time sheet");
    }

    while( fgets(line, 1024, check) != NULL ) {
        // Just search for the latest line, do nothing in the loop
    }

    fclose(check);

    char *is_timed_out = parse_time_record(line, "out");
    if(empty(is_timed_out)) {
        return error("\n\nAlready timed in.\n\n");
    }

    char *last_printed_index = parse_time_record(line, "index");
    int last_index = 0;

    if (!empty(last_printed_index)) {
        last_index = convert(last_printed_index);
    }

    int index = last_index + 1;

    // open sheet file to append
    FILE *sheet = fopen(sheet_path, "a");

    if(sheet == NULL) {
        return error("\n\nFailed to open time sheet");
    }

    char now[64];
    get_now(now);

    fprintf(sheet, "\n%d,%s,,%s", index, now, message);
    fclose(sheet);

    printf("\n\nTimed into '%s'...\n\n", CONFIG_CURRENT_SHEET);

    return 0;
}

int execute_command(const char *command, const char *command_argument) {
    if(strcmp(command, "in") == 0
    || strcmp(command, "i") == 0) {
        if (empty(command_argument)) {
            return time_in("");
        } else {
            return time_in(command_argument);
        }
    }

    if(strcmp(command, "out") == 0
    || strcmp(command, "o") == 0) {
        return 0;
    }

    if(strcmp(command, "sheet") == 0
    || strcmp(command, "s") == 0) {
        if (empty(command_argument)) {
            return sheet_show();
        } else {
            return sheet_create(command_argument);
        }
    }

    if(strcmp(command, "append") == 0
    || strcmp(command, "a") == 0) {
        return 0;
    }

    if(strcmp(command, "edit") == 0
    || strcmp(command, "e") == 0) {
        if (empty(CONFIG_CURRENT_SHEET)) {
            return error("\nNo current sheet selected.");
        }
        char *open = malloc(128);

        strcat(open, CONFIG_EDITOR_COMMAND);
        strcat(open, " ");
        strcat(open, CONFIG_SHEETS_FOLDER);
        strcat(open, CONFIG_CURRENT_SHEET);
        strcat(open, ".csv");

        printf("\nEditing timesheet...\n%s \n\n", open);
        system(open);

        return 0;
    }

    if(strcmp(command, "display") == 0
    || strcmp(command, "d") == 0) {
        return 0;
    }

    if(strcmp(command, "help") == 0
    || strcmp(command, "h") == 0) {
        printf("\n\nshow help\n");
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
                // printf("\n\n COMMAND: %s\n", command);
            } else if (empty(command_argument)) {
                command_argument = this_arg;
                // printf("\n\n COMMAND ARG: %s\n", command_argument);
            } else {
                printf("\n\nERROR: Unknown command '%s'\n\n", this_arg);
                return error("Invalid command given.\n");
            }
        }
    }

    if (empty(command)) {
        return error("Invalid command given.\n");
    }

    if(!config_load()) {
        return error("Cannot load config.\n");
    }

    result = execute_command(command, command_argument);
    fclose(config);
    return result;
}
