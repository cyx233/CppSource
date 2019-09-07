import pickle
from django.shortcuts import render
from django.template import loader, Context
from django.http import HttpResponse


def teamrank(request):
    with open('static/data/teams.pick', 'rb') as f:
        a = pickle.load(f)
    teams = list(a)

    teams.sort(key=lambda x: -x.rank)
    response = HttpResponse()
    t = loader.get_template('teams.html')
    c = {'teams': teams}
    response.write(t.render(c))
    return response


def team(request, team_id):
    with open('static/data/teams.pick', 'rb') as f:
        a = pickle.load(f)
    teams = list(a)
    with open('static/data/news.pick', 'rb') as f:
        a = pickle.load(f)
    news = list(a)

    response = HttpResponse()
    this_news = []
    this_team = teams[0]
    for i in teams:
        if i.id == team_id:
            this_team = i
            break
    for i in this_team.team_news:
        for j in news:
            if j.id == i:
                this_news.append(j)
                break

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

    if 10 * nowpage > len(this_news):
        page_news = this_news[10 * (nowpage - 1):]
    else:
        page_news = this_news[10 * (nowpage - 1):10 * nowpage]

    t = loader.get_template('teamsite.html')
    c = {'team': this_team, 'news': page_news, 'prepage': prepage, 'nowpage': nowpage, 'nextpage': nextpage}
    response.write(t.render(c))
    return response


# Create your views here.
