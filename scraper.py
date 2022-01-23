from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from webdriver_manager.chrome import ChromeDriverManager
import sys, os

# Get the department search query from the command line
query = ' '.join(sys.argv[1:])

# Setup driver to run silently, without graphics (headless mode)
os.environ['WDM_LOG_LEVEL'] = '0'
options = webdriver.ChromeOptions()
options.add_argument("--headless")
service = Service(ChromeDriverManager().install())

# Start driver and query UCLA's course descriptions page
driver = webdriver.Chrome(options=options, service=service)
driver.get("https://registrar.ucla.edu/academics/course-descriptions?search=" + query)

# Defining some macros
lowerDivPrefix = "//div[starts-with(@id, 'lower-division-courses')]/div[@class='course-record']"
upperDivPrefix = "//div[starts-with(@id, 'upper-division-courses')]/div[@class='course-record']"


# Wait for page elements to load before scraping (otherwise we get no hits)
try:
    # Adjust this time to be longer if the page takes too long to load
    elem = WebDriverWait(driver, 2).until(
        EC.presence_of_element_located((By.XPATH, lowerDivPrefix))
    )
except:
    driver.quit()
    print("Invalid department entered, please try again")
    exit()

# Replace spaces with underscores in the department name to make it a valid path substring
# department = department.replace(' ', '_');
titleNode = driver.find_element(By.XPATH, "//div[@id='block-ucla-sa-page-title']/h1")
department = titleNode.get_attribute('innerText').split('(')[0][:-1].replace(' ', '_')
if (department[0:6] == "Search" or department[0:6] == "Course"):
    driver.quit()
    print("Invalid department entered, please try again")
    exit()

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
print("Successfully updated information for the " + department.replace('_', ' ') + " department")
fileDescription.close();
driver.quit()
