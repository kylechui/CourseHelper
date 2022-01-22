from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

driver = webdriver.Chrome("/usr/bin/chromedriver")
driver.get("https://registrar.ucla.edu/academics/course-descriptions?search=Mathematics")
# Wait for page elements to load before grabbing them
try:
    elem = WebDriverWait(driver, 30).until(
        EC.presence_of_element_located((By.XPATH, "//div[@class='course-record']/h3"))
    )
finally:
    courseDescriptions = driver.find_elements(By.XPATH, "//div[@class='course-record']/p[2]")
    print(len(courseDescriptions))

    print([course.get_attribute('innerText') for course in courseDescriptions])
    driver.quit()
