from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

fileID = open("./Courses/IDs.txt", "w")
fileDescription = open("./Courses/descriptions.txt", "w")

driver = webdriver.Chrome("/usr/bin/chromedriver")
driver.get("https://registrar.ucla.edu/academics/course-descriptions?search=Mathematics")
# Wait for page elements to load before grabbing them
try:
    elem = WebDriverWait(driver, 30).until(
        EC.presence_of_element_located((By.XPATH, "//div[@class='course-record']/h3"))
    )
finally:
    courseTitles = driver.find_elements(By.XPATH, "//div[@class='course-record']/h3")
    courseDescriptions = driver.find_elements(By.XPATH, "//div[@class='course-record']/p[2]")

    fileID.writelines([title.get_attribute('innerText')[:title.get_attribute('innerText').find('.')] + '\n' for title in courseTitles])
    fileDescription.writelines([course.get_attribute('innerText') + '\n' for course in courseDescriptions])
    driver.quit()

fileID.close()
fileDescription.close()
