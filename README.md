# CourseHelper
> A CLI program created to help you prepare for course registration.

**Note:** At the moment, this project is built specifically for other UCLA
students, although feel free to clone this repository and modify the code to
work for other universities!

### Dependencies
* A compiler capable of building C++17 code (g++ preferred, otherwise you'll
  have to edit `Makefile`)
* Python 3
* [Selenium](https://www.selenium.dev/): Provides a Python library to retrieve
  information from websites
* [WebDriver Manager](https://pypi.org/project/webdriver-manager/): Helps you
  manage browser drivers for simulated interactions with [Google
  Chrome](https://www.google.com/chrome/index.html)

## Building
Make sure you have the above dependencies, and then just run:
```sh
git clone https://github.com/kylechui/CourseHelper.git
cd CourseHelper && make
```

## Commands
- [X] `help`: Shows this list of all commands
- [X] `exit`: Exits the program
- [X] `list`: Shows list of courses that have already been taken
- [X] `take [course]`: Adds course to the list of taken courses
- [X] `info [course]`: [WIP] Shows information about a given course
- [X] `prereq [course]`: [WIP] Lists all prereqs needed to take a given course
- [X] `available [department]`: Shows list of courses that are currently available
- [X] `update [department]`: Updates course information for a given department

**Note:** The above commands are all quite particular about capitalization and
spelling (hopefully patch coming soon), so make sure to spell things correctly!

## Examples
```sh
take Computer Science 35L
info Mathematics 170E
prereq Computer Science 180
available Mathematics
update Computer Science
```
**TODO:** Add more examples of other commands as they're being implemented

## Limitations
Don't take this project as gospel---always double check with UCLA's websites to
make sure your four-year plan is up-to-date and viable. If you are experiencing
any bugs/instability, feel free to open an issue or PR.

As of right now, the pre-requisite relationships between courses has to be
parsed manually and inputted into a local text file (while the course IDs and
descriptions are web scraped). I would have liked to web scrape for the course
relationships, but I'm not good enough at RegExp to filter the natural language
of each course description down to just the required classes ([trust me, I
tried](https://github.com/kylechui/CourseHelper/blob/4624247913b1b03347b7efa801a8c4eeb9705abe/prereq.sh)).
If you've found an alternate website that lists these relationships in a simpler
manner, feel free to open an issue!

## TODO/Goals
This section has been moved to [the wiki](https://github.com/kylechui/CourseHelper/wiki/Goals-for-the-future-of-this-project).

## Contributing
Suggestions and contributions welcome through issues and pull requests! More
information about the project can be found in the [the
wiki](https://github.com/kylechui/CourseHelper/wiki).
