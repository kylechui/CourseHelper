# CourseHelper
> A CLI program created to help you prepare for course registration.

**Note:** At the moment, this project is built specifically for other UCLA
students, although feel free to clone this repository and modify the web
scraping code (found in `scraper.py`) to work for other websites!

### Dependencies
* A compiler capable of building C++17 code
* Python 3
* [Selenium](https://www.selenium.dev/): Provides library to retrieve
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

**Note:** The commands are usually a bit finnicky in terms of getting them to
work, make sure to spell things correctly!

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
of each course description down to just the required classes. If you've found an
alternate website that lists these relationships in a simpler manner, feel free
to open an issue!

## TODO/Future Ideas
- [X] Add user information file that contains list of completed courses
- [X] Implement interactive command that allows the user to add/remove courses
  from their own list
- [X] Read the course data from the input files in `Courses/` and store them in
  `Course` class objects
  - [ ] Parse the pre-requisite classes out of the course description via regex
- [X] Implement command that gives the user a list of all classes that can be
  taken
- [X] Implement command that gives the user information about a certain class:
  - [X] Course ID/name/description
  - [X] List of pre-requisite courses necessary to take that class
  - [X] Whether or not the user can take that class, and if not, which courses
    are still needed to take it
- [ ] Store all user data in a folder 
  - [ ] Allow users to have blacklists in addition to courses taken
- [ ] Add aliases for the departments and make them case-insensitive
- [ ] Visualizer for the course graph
- [X] Web scraping to automatically generate the input data
- [ ] Maybe write a shell script to set up the project on install?
  - [ ] Alternatively, turn this into a web app
- [ ] Add checks for whether the user has Chrome/ChromeDriver/Selenium
  installed, and show if not, show warnings when they run `update`
