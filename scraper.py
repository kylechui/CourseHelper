from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import sys

driver = webdriver.Chrome("/usr/bin/chromedriver")
driver.get("https://registrar.ucla.edu/academics/course-descriptions?search=" + ' '.join(sys.argv[1:]))
# Defining some macros
lowerDivPrefix = "//div[starts-with(@id, 'lower-division-courses')]/div[@class='course-record']"
upperDivPrefix = "//div[starts-with(@id, 'upper-division-courses')]/div[@class='course-record']"


# Wait for page elements to load before grabbing them
try:
    elem = WebDriverWait(driver, 30).until(
        # EC.presence_of_element_located((By.XPATH, "//div[@class='course-record']/h3"))
        EC.presence_of_element_located((By.XPATH, lowerDivPrefix))
    )
finally:
    lowerDivHeaders = driver.find_elements(By.XPATH, lowerDivPrefix + "/h3")
    lowerDivDescriptions = driver.find_elements(By.XPATH, lowerDivPrefix + "/p[2]")
    upperDivHeaders = driver.find_elements(By.XPATH, upperDivPrefix + "/h3")
    upperDivDescriptions = driver.find_elements(By.XPATH, upperDivPrefix + "/p[2]")

    # Add lower/upper div course IDs to a text file
    fileID = open("./Courses/IDs.txt", "w")
    for header in lowerDivHeaders:
        title = header.get_attribute('innerText')
        fileID.write(title[:title.find('.')] + '\n')
    fileID.close()
    fileID = open("./Courses/IDs.txt", "a")
    for header in upperDivHeaders:
        title = header.get_attribute('innerText')
        fileID.write(title[:title.find('.')] + '\n')
    fileID.close()

    # Add lower/upper div course descriptions to a text file
    fileDescription = open("./Courses/descriptions.txt", "w")
    for description in lowerDivDescriptions:
        fileDescription.write(description.get_attribute('innerText') + '\n')
    fileDescription.close();
    fileDescription = open("./Courses/descriptions.txt", "a")
    for description in upperDivDescriptions:
        fileDescription.write(description.get_attribute('innerText') + '\n')
    fileDescription.close();
    driver.quit()
