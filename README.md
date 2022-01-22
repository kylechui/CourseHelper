# CourseHelper
> A CLI program created to help you prepare for course registration.

**Note:** At the moment, this project is built specifically for other UCLA
students, although feel free to clone this repository and modify the web
scraping code (found in `scraper.py`) to work for other websites!


## Dependencies
This project requires [Selenium](https://www.selenium.dev/) to scrape data from
UCLA's course catalog and generate databases, as well as
[ChromeDriver](https://chromedriver.chromium.org/downloads) to simulate browser
interactions.

## Quick Start
WIP

## Commands
- [X] `help`: Shows this list of all commands
- [X] `exit`: Exits the program
- [ ] `available`: Shows list of courses that are currently available
- [X] `list`: Shows list of courses that have already been taken
- [X] `take [course]`: Adds course to the list of taken courses
- [ ] `info [ID]`: Shows information about a given course ID
- [X] `update [department]`: Updates course information for a given department

## TODO/Future Ideas
- [X] Add user information file that contains list of completed courses
- [X] Implement interactive command that allows the user to add/remove courses
  from their own list
- [ ] Implement command that gives the user a list of all classes that can be
  taken
- [ ] Implement command that gives the user information about a certain class:
  - [ ] Course ID/name/description
  - [ ] List of pre-requisite courses necessary to take that class
  - [ ] Whether or not the user can take that class, and if not, which courses
    are still needed to take it
- [ ] Visualizer for the course graph
- [X] Web scraping to automatically generate the input data
