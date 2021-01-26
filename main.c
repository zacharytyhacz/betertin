#include <stdio.h>
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

// if the given or default config folder is empty or non-existant, we initialize it.
void initiate_config_folder() {
}

void check_configuration_folder(char *configuration_folder[]) {
    
}

void parse_flag(const char *flag, const char *flag_argument) {
    if (strncmp(flag, "--config", strlen(flag))) {
        
    }
}

bool prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

bool empty(const char *text) {
    return text[0] != '\0';
}

int main(int argc, char *argv[]) {
    char *command = "";
    char *command_argument = "";

    for(int i = 1; i < sizeof(&argv); i++) {
        char *this_arg = argv[i];

        if(this_arg == NULL){
            break;
        }

        if(prefix("--", this_arg)) {
            printf("\n\n got a flag: %s", this_arg);
            parse_flag(this_arg, argv[i + 1]);
            continue;
        }

        if (empty(command)) {
            command = this_arg;
            printf("\n\n COMMAND: %s", command);
        } else if (empty(command_argument)) {
            command_argument = this_arg;
            printf("\n\n COMMAND ARH: %s", command_argument);
        } else {
            printf("\n\nERROR: Unknown command '%s'\n\n", this_arg);
            return 1;
        }
    }

    return 0;
}
