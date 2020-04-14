import sys
from gensim.summarization import summarize, keywords
import urllib.request
from bs4 import BeautifulSoup
import requests
import re
import regex
import ssl


def get_links_from_google_search(query):
    """
    Performs some regex manipulations on the received HTML to clean up the links.

    - Positive Lookbehind:

        ?<=

    - This searches for '/url?q=' BEHIND (http:// | https:// | any variant thereof):

        (?<=/url\?q=)(htt.*://.*)

    - link["href"] returns the 'href' element in the link object

    - re.compile()  Compile a regular expression pattern into a regular expression object,
    which can be used for matching using its match() and search() methods, described below.

    - The sequence

        prog = re.compile(pattern)
        result = prog.match(string)

    is equivalent to

        result = re.match(pattern, string)

    - Find all "a" tags where the contents of the "href" element contain
    the pattern in re.compile():

        find_all("a",href=re.compile("(?<=/url\?q=)(htt.*://.*)"))

    - Look AHEAD assertion:

        :(?=http)     a colon FOLLOWED BY 'http'

    """
    page = requests.get("https://www.google.co.uk/search?q="+query)
    soup = BeautifulSoup(page.content, features="html.parser")
    links = []

    for link in soup.find_all("a", href=re.compile("(?<=/url\?q=)(htt.*://.*)")):
        links.append(link["href"].replace("/url?q=", ""))

    return links


def find_site_from_url(links):
    sites = dict()
    for link in links:
        res = regex.search('(?<=((\/{2}\.)|(w{3}\.)|(\w+)\.))\w+', link)
        site = res.group(0)
        if(site != "google"):
            sites[site] = link
    return sites


def print_list_of_sites(sites):
    print("---")
    print("Your options to choose from are:")
    print()
    options = dict()
    for i, site in enumerate(sites.keys()):
        options[i] = site
        # Left align i with 3 character spaces
        print(f'  {i:<3}- {site}')
    print()
    user_num = int(
        input("Please enter a number to investivate a site further: "))
    while(user_num < 0 or user_num > len(sites)):
        user_num = input("Please select a number from the list above: ")
    return options[user_num]


def check_url(url):
    print("---")
    print("Checking URL...")
    print(f"URL: {url}")
    try:
        # I think this is necessary otherwise you get bad permission HTTP responses.
        ssl._create_default_https_context = ssl._create_unverified_context
        html = urllib.request.urlopen(url)
        http_response_code = html.getcode()
        #  print(f"!!! urllib Response code: {http_response_code}")
        if(http_response_code >= 200 and http_response_code < 300):
            html = html.read()
            return html
        else:
            # Bad response
            raise Exception
    except:
        print("Error retreiving HTML content with 'urllib'.")
        try:
            print("Trying with 'requests'...")
            html = requests.get(url)
            http_response_code = html.status_code
            #  print(f"!!! requests Response code: {http_response_code}")
            if(http_response_code >= 200 and http_response_code < 300):
                return html
            else:
                # Bad response
                raise Exception

        except:
            print("Error retreiving HTML content with 'requests'.")
            print("See 'failed_urls.txt'.")
            with open("failed_urls.txt", "a") as file:
                file.write(url + '\n')
            return False


#  def get_html_content(user_selection, url):
    #  print("---")
    #  print("Retreiving text for: " + user_selection)
    #  # print(url)
    #  text = []
    #  html = urllib.request.urlopen(url).read()
#
    #  soup = BeautifulSoup(html, features="html.parser")
    #  ps = soup.body.find_all('p')
    #  # Check if is empty
    #  if not ps:
        #  print("Found no 'p' tags")
    #  for p in ps:
        #  line = p.get_text()
        #  # Removes any blank strings:
        #  if line and not regex.search("^\s*$", line):
            #  text.append(line)
#
    #  return text

def get_html_content_from_request(html):
    if(type(html) == requests.models.Response):
        # requests Response
        html = html.content
    text = []

    soup = BeautifulSoup(html, features="html.parser")
    ps = soup.body.find_all('p')
    # Check if is empty
    if not ps:
        print("Found no 'p' tags")
    for p in ps:
        line = p.get_text()
        # Removes any blank strings:
        if line and not regex.search("^\s*$", line):
            text.append(line)

    return text


def get_summary(text):
    print("---")
    print("Summarizing text...")
    # word_count=150 seems to be the minimum...
    summary = summarize(text, word_count=150)
    return summary


def get_key_words(text):
    print("---")
    print("Finding keywords...")
    words = keywords(text).split('\n')
    return words


def main(searchterm):
    print("---")
    print("Retreiving Google results for: '"+searchterm+"'")
    links = get_links_from_google_search(searchterm)
    sites = find_site_from_url(links)
    user_selection = print_list_of_sites(sites)
    # .split("&sa")[0] removes some extra Google junk on the url
    url_to_search = sites[user_selection].split("&sa")[0]
    html = check_url(url_to_search)

    while(not html):
        print("---")
        print("! Error retrieving data, please try again.")
        user_selection = print_list_of_sites(sites)

    text_list = get_html_content_from_request(html)
    text_block = "".join(text_list)

    summary = get_summary(text_block)
    print()
    print(summary)

    words = get_key_words(summary)

    print(f"Keywords: ", end='')
    for word in words:
        print(word, end=' ')
    print()
    print("---")
    automate = input(
        "Would you like to automate the process and view the results? (y/n): ")
    while(automate != "y" and automate != "n"):
        automate = input("Please choose 'y' or 'n': ")

    if(automate == "n"):
        sys.exit(0)
    elif(automate == "y"):
        results = dict()
        for site, url in sites.items():
            url_to_search = url.split("&sa")[0]
            html = check_url(url_to_search)
            if(html):
                text_list = get_html_content_from_request(html)
                text_block = "".join(text_list)
                summary = get_summary(text_block)
                words = get_key_words(summary)
                results[site] = words
            else:
                continue

        for site, words in results.items():
            print(f"{site:<12}:", end="")
            limit=10 # Good line length for my terminal output
            i=0
            for word in words:
                if(i < limit):
                    print(f"{word:<14}", end="  ")
                i += 1
            print()


if __name__ == "__main__":
    if(len(sys.argv) < 2):
        searchterm = input("Please enter a keyword to search: ")
        while(' ' in searchterm or searchterm == ''):
            print("Please enter only one keyword to search.")
            searchterm = input("Search term: ")
        main(searchterm)
    elif(len(sys.argv) > 2):
        # Too many arguments
        print("Please enter only one keyword to search.")
        searchterm = input("Search term: ")
        while(' ' in searchterm or searchterm == ''):
            print("Please enter only one keyword to search.")
            searchterm = input("Search term: ")
        main(searchterm)
    elif(len(sys.argv) == 2 and sys.argv[1] != ''):
        main(sys.argv[1])
