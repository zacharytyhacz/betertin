# `betertin`
# Simple Fast Time Tracker Made in C

I used to use `timetrap` that was made in Ruby.

I wanted to implement it with other Linux programs like `polybar` and customize it better, but due to it being it Ruby and SqlLite, it was a little slow.

This is mostly a learning project to learn C as well.

## About

Instead of using sqlLite, timesheets are stored in simple csv files. 
- they can be easily and quickly parsed and directly edited
- one issue i had with timetrap was that it was difficult to edit past time stuff
- with csv's, it's easy to organized a dedicated betertin folder.

I plan on keeping the config folder like this:
```
.betertin/
├── config
└── sheets
    ├── csc123.csv
    ├── other_app.csv
    └── working.csv
```

## Configuration

```
sheetsfolder=~/.betertin/sheets
currentsheet=some time sheet
editorcommand=vim
timeformat=<some date format in c>
```


## Usage

since this is in the middle of development, you have to create the defaul config folders:
- `$ mkdir -p ~/.config/betertin/sheets`

this will be automatic soon

#### Flags

### Specify a different config file
```
--config <path> 
```

### select or create new time sheet
```bash
$ t sheet working

# or 

$ t s working
```

### what is my current time sheet
```bash
$ t sheet

# or 

$ t s
```

### view all time sheets
```bash
$ t sheets

# or 

$ t ss
```

### clock in to current time sheet
```bash
$ t in "working on the readme"

# or 

$ t i "working on the readme"
```

### append message to clock-in message
```bash
$ t append "and on the run shell script"

# or 

$ t a "and on the run shell script"
```

### clock out of current time sheet
```bash
$ t out

# or 

$ t o
```

### edit time sheet
> this will open the timesheet `.csv` file in your editor
```bash
$ t edit 

# or 
$ t e
```

### view details of my time sheet
Options:
- `d` view today's time sheet
- `w` view this week's time sheet
- `m` view this month's time sheet
- `y` view this year's time sheet
- `a` view everything in current time sheet

```bash
$ t display w

# or 

$ t d w
```

### 

<a href="https://www.buymeacoffee.com/zactyh" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>
