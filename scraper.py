from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import sys

# Get the department search query from the command line
department = ' '.join(sys.argv[1:])

# Setup driver and point it towards the correct URL (using UCLA's department search)
driver = webdriver.Chrome("/usr/bin/chromedriver")
driver.get("https://registrar.ucla.edu/academics/course-descriptions?search=" + department)

# Defining some macros
lowerDivPrefix = "//div[starts-with(@id, 'lower-division-courses')]/div[@class='course-record']"
upperDivPrefix = "//div[starts-with(@id, 'upper-division-courses')]/div[@class='course-record']"


# Wait for page elements to load before scraping (otherwise we get no hits)
while True:
    try:
        elem = WebDriverWait(driver, 30).until(
            EC.presence_of_element_located((By.XPATH, lowerDivPrefix))
        )
        break
    except:
        continue

# Replace spaces with underscores in the department name to make it a valid path substring
department = department.replace(' ', '_');

# Scrape the data into a list of nodes
lowerDivHeaders = driver.find_elements(By.XPATH, lowerDivPrefix + "/h3")
lowerDivDescriptions = driver.find_elements(By.XPATH, lowerDivPrefix + "/p[2]")
upperDivHeaders = driver.find_elements(By.XPATH, upperDivPrefix + "/h3")
upperDivDescriptions = driver.find_elements(By.XPATH, upperDivPrefix + "/p[2]")

# Add lower/upper div course IDs to a text file
# For UCLA's webpage, the format is [ID].[NAME], so we parse to get the ID
fileID = open("./Courses/" + department + "_IDs.txt", "w")
for header in lowerDivHeaders:
    title = header.get_attribute('innerText')
    fileID.write(title[:title.find('.')] + '\n')
for header in upperDivHeaders:
    title = header.get_attribute('innerText')
    fileID.write(title[:title.find('.')] + '\n')
fileID.close()

# Add lower/upper div course descriptions to a text file
fileDescription = open("./Courses/" + department + "_descriptions.txt", "w")
for description in lowerDivDescriptions:
    fileDescription.write(description.get_attribute('innerText') + '\n')
for description in upperDivDescriptions:
    fileDescription.write(description.get_attribute('innerText') + '\n')
fileDescription.close();
driver.quit()
