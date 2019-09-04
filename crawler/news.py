import requests
import re
import pickle

import crawler_head

web = set()
news = set()
num = 0

for j in range(1, 101):
    web.clear
    url = "https://voice.hupu.com/nba/" + str(j)
    pattern = re.compile('<a href="(.*?)"  target="_blank">')
    response = requests.get(url)
    result = re.findall(pattern, response.text)
    print("PAGE ", j)

    for i in result:
        num = num + 1
        response = requests.get(i)
        web_source = response.text

        pattern_keyword = re.compile('<meta http-equiv="Keywords" content="(.*?)" />')
        key_word = re.findall(pattern_keyword, web_source)

        pattern_title = re.compile('<title>(.*?)</title>')
        title = re.findall(pattern_title, web_source)

        pattern_image = re.compile('<div class="artical-importantPic">.*?\
                <img src="(.*?)" alt=".*?">.*?</div>', re.S)
        image_link = re.findall(pattern_image, web_source)

        pattern_source_link = re.compile('<span class="comeFrom" id=".*?">.*?\
                .*?<a href="(.*?)" target="_blank">.*?</a>.*?\
                </span>', re.S)
        source_link = re.findall(pattern_source_link, web_source)

        pattern_source = re.compile('<span class="comeFrom" id=".*?">.*?\
                .*?<a href=".*?" target="_blank">(.*?)</a>.*?\
                </span>', re.S)
        source = re.findall(pattern_source, web_source)

        pattern_postTime = re.compile('<a href=".*?" class="time">.*?\
                <span id=".*?"> (.*?) </span>', re.S)
        time = re.findall(pattern_postTime, web_source)

        pattern_content = re.compile('<div class="artical-main-content">.*?\
                (.*?)</div>', re.S)
        content = re.findall(pattern_content, web_source)

        new = New(key_word, title, image_link, source, time, content, i, source_link)
        news.add(new)

        print(num, " ", i)
        print(title)

with open('news.pick', 'wb') as f:
    pickle.dump(news, f)
