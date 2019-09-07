from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader
from Browser.views import mytask
import jieba.analyse
import jieba
import pickle
import time

with open('static/data/dic_title.pick', 'rb') as f:
    dic_title = pickle.load(f)
with open('static/data/dic_key_word.pick', 'rb') as f:
    dic_key_word = pickle.load(f)
with open('static/data/dic_content.pick', 'rb') as f:
    dic_content = pickle.load(f)


def newsSite(request):
    with open('static/data/news.pick', 'rb') as f:
        a = pickle.load(f)
        news = list(a)
    with open('static/data/teams.pick', 'rb') as f:
        a = pickle.load(f)
        teams = list(a)

    news_id = request.GET['id']
    response = HttpResponse()
    this_news = news[0]
    for i in news:
        if int(i.id) == int(news_id):
            this_news = i
            break

    for i in teams:
        name = i.chName
        href = '<a href="/TeamRank/TeamSite/' + str(i.id) + '">' + name + '</a>'
        this_news.content[0] = this_news.content[0].replace(name, href)

        for j in i.player:
            name = j.cnName
            href = '<a href="/TeamRank/TeamSite/' + str(i.id) + '">' + name + '</a>'
            this_news.content[0] = this_news.content[0].replace(name, href)

    t = loader.get_template('news.html')
    c = {'news': this_news}
    response.write(t.render(c))
    return response


def searchsite(request):

    if mytask.get_change() == 1:
        global dic_title
        global dic_key_word
        global dic_content
        with open('static/data/dic_title.pick', 'rb') as f:
            dic_title = pickle.load(f)
        with open('static/data/dic_key_word.pick', 'rb') as f:
            dic_key_word = pickle.load(f)
        with open('static/data/dic_content.pick', 'rb') as f:
            dic_content = pickle.load(f)
        mytask.set_changed()

    response = HttpResponse()
    keywords = request.GET['keywords']

    keyword = jieba.analyse.extract_tags(keywords)

    if 'page' not in request.GET:
        nowpage = 1
        prepage = 1
        nextpage = 2
    else:
        nowpage = int(request.GET['page'])

    if nowpage > 1:
        prepage = nowpage - 1
        nextpage = nowpage + 1
    else:
        prepage = 1
        nextpage = 2

    news_set = set()
    news_set2 = set()
    news_set3 = set()
    news_line = []

    start = time.time()
    for i in keyword:
        if i in dic_title:
            news_set = news_set | dic_title[i]
    news_line.extend(list(news_set))
    for i in keyword:
        if i in dic_key_word:
            news_set2 = news_set2 | dic_key_word[i]
    news_line.extend(list(news_set2 - news_set))
    for i in keyword:
        if i in dic_content:
            news_set3 = news_set3 | dic_content[i]
    news_line.extend(list(news_set3 - news_set2 - news_set))
    end = time.time()

    if 30 * nowpage > len(news_line):
        page_news = news_line[30 * (nowpage - 1):]
    else:
        page_news = news_line[30 * (nowpage - 1):30 * nowpage]

    for word in keyword:
        for i in page_news:
            i.title[0] = i.title[0].replace(word, '<span class="red-text">' + word + '</span>')

    usetime = end - start
    num = len(news_line)
    t = loader.get_template('searchsite.html')
    keyword = request.GET['keywords']
    c = {
        'all_news': page_news,
        'word': keyword,
        'prepage': prepage,
        'nextpage': nextpage,
        'nowpage': nowpage,
        'time': usetime,
        'num': num,
    }
    response.write(t.render(c))
    return response


# Create your views here.
