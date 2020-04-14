# Python Webscraper

I took this as an opportunity to get my head into the Python language really and to try out things which I feel would be useful for creating and tidying up text-based datasets. So I took some time to learn about Regular Expressions and comment the functions as I wrote them. I have a much more solid understanding of RegEx's after cleaning URL's such as this:

```
https://www.who.int/search%3Fquery%3Dcorona%26page%3D1%26pagesize%3D10%26sortdir%3Ddesc%26sort%3Drelevance%26default%3DAND%26f.Countries.size%3D100%26f.Lang.filter%3Den%26f.RegionalSites.size%3D100%26f.Topics.size%3D100%26f.contenttype.size%3D100%26f.doctype.size%3D101%26facet.field%3DRegionalSites%26facet.field%3DTopics%26facet.field%3Ddoctype%26facet.field%3DCountries%26facet.field%3Dcontenttype%26facet.field%3DLang%26tune%3Dtrue%26tune.0%3D3%26tune.1%3D2%26tune.2%3D2%26tune.3%3D3%26tune.4%3D180%26tune.5%3D75
```

I also wanted to program to have some user interaction and options within, which meant writing `try` `except` blocks throughout, which is also necessary when attempting to retreive data from the web - also using `while` loops to pause the program when awaiting input. Finally I wanted to program to run without crashing, so any errors are caught and explained to the user, and any websites it fails to get data from are logged in `failed_urls.txt`.

---

### To install:

```
python3 -m venv simple_scraper
source simple_scraper/bin/activate
pip install -r requirements.txt
```

### To run:

```
python3 simple_scraper.py {keyword}
```

You will receive as many options as appear on the first page of a Google search to choose to investigate furthur. Choosing one will return a 150 word summary of that page.

```
---
Retreiving Google results for: 'corona'
---
Your options to choose from are:

  0  - telegraph
  1  - theguardian
  2  - thesun
  3  - watfordobserver
  4  - socialeurope
  5  - express
  6  - 1843magazine
  7  - gov
  8  - nhs
  9  - who
  10 - com
  11 - wikipedia
  12 - worldometers
  13 - nasa
  14 - co

Please enter a number to investivate a site further: 1
---
Checking URL...
URL: https://www.theguardian.com/lifeandstyle/2020/apr/12/love-in-the-time-of-corona-stories-of-the-heart-told-from-behind-quarantined-walls
---
Summarizing text...

  I’ve been with Claire for nearly four years, and I decided I wanted to marry her quite early on.She loves the royals and all things historical, so one evening I Googled “Where was the Queen proposed to?” Turns out it was Balmoral.
  I heard there was a room going through a friend and, after one of those awkward housemate interviews, I got the call to tell me the place was mine if I wanted it.When we’d chatted over beer, one of the boys had mentioned he was gay and I told him that I was, too.
  When the schools closed, I didn’t want to freak her out by making a big deal that this could be the last time I saw her.The other day she walked past my house with her mum, so we talked from each end of the driveway.

---
Finding keywords...
Keywords: housemate proposed schools
---
Would you like to automate the process and view the results? (y/n):

```

After this you automate the process. Automation will return up to 10 most common keywords which appear on all websites:

```
...

---
Checking URL...
URL: https://coronaextra.co.uk/
Found no 'p' tags
---
Summarizing text...
---
Finding keywords...
telegraph   :football        protected       university      ...
theguardian :housemate       room going
thesun      :news            deaths          death           ...
watfordobserver:care            helped          helping         ...
socialeurope:capitulation    health          unfortunately   ...
express     :remarks         newspaper       response
1843magazine:coronavirus     new             like            ...
gov         :information     government      insurance       ...
nhs         :people          risk            coronavirus     ...
wikipedia   :
worldometers:
nasa        :solar           eclipse         eclipsed        ...
co          :
```


