# CourseHelper
> A CLI program created to help you prepare for course registration.

**Note:** At the moment, this project is built specifically for other UCLA
students, although feel free to clone this repository and modify the web
scraping code (found in `scraper.py`) to work for other websites!

## Quick Start
Just C++17 is strictly necessary for using this project :)

### Optional Dependencies
* [Selenium](https://www.selenium.dev/): Provides library to retrieve
  information from websites
* [WebDriver Manager](https://pypi.org/project/webdriver-manager/): Helps you
  manage browser drivers for simulated interactions

**TODO:** Add mention for how to set up and use ChromeDriver (path)

## Commands
- [X] `help`: Shows this list of all commands
- [X] `exit`: Exits the program
- [ ] `available`: Shows list of courses that are currently available
- [X] `list`: Shows list of courses that have already been taken
- [X] `take [course]`: Adds course to the list of taken courses
- [ ] `info [ID]`: Shows information about a given course ID
- [X] `update [department]`: Updates course information for a given department

## Examples
```sh
update Computer Science
```
**TODO:** Add more examples of other commands as they're being implemented

## Customization
**TODO:** Implement/explain how to set up aliases to decrease likelihood of
duplicated local databases

## Limitations
As of right now, the pre-requisite relationships between courses has to be
parsed manually and inputted into a local text file, as I'm not good enough at
regexp to parse the description files into a short list (particularly handling
words like "and"). If you've found an alternate website that lists these
relationships, feel free to open an issue!

## TODO/Future Ideas
- [X] Add user information file that contains list of completed courses
- [X] Implement interactive command that allows the user to add/remove courses
  from their own list
- [ ] Read the course data from the input files in `Courses/` and store them in
  `Course` class objects
  - [ ] Parse the pre-requisite classes out of the course description via regex
- [ ] Implement command that gives the user a list of all classes that can be
  taken
- [ ] Implement command that gives the user information about a certain class:
  - [ ] Course ID/name/description
  - [ ] List of pre-requisite courses necessary to take that class
  - [ ] Whether or not the user can take that class, and if not, which courses
    are still needed to take it
- [ ] Add aliases for the departments and make them case-insensitive
- [ ] Visualizer for the course graph
- [X] Web scraping to automatically generate the input data
- [ ] Maybe write a shell script to set up the project on install?
- [ ] Add checks for whether the user has Chrome/ChromeDriver/Selenium
  installed, and show if not, show warnings when they run `update`
