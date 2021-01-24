#include <stdio.h>

// commands
//
// clock into current sheet
// t in / t i
//
// clock out of current sheet
// t out / t o
//
// use new sheet
// t sheet <sheetname>
//
// show help menu for commands
// t help
// 
// open the raw csv to edit ( uses 
// t edit / t e
// 
// append text to current clocked message
// t append / t a
//
// show time records for current sheet
// t display < w, d, m, y, a>


// configuration fields:
//
// timefolder=~/.bettertin
// sheetsfolder=~/.bettertin/sheets
// currentsheet=my app
// editorcommand=vim
// timeformat=some date form in c



// 1. parse setup configuration environment
// 2. parse arguments
// 3. store as simple csv
//
// each sheet is own csv file
//
// index,in,out,message



int main(int argc, char *argv[]) {
    char string[] = "Title";

    printf("Hello World! \n");
    return 0;
}
