import requests
import pickle
import json


class New:
    def __init__(self, key_word, title, image_link, source, postTime, content, link, source_link):
        self.key_word = key_word
        self.title = title
        self.image_link = image_link
        self.source = source
        self.postTime = postTime
        self.content = content
        self.link = link
        self.source_link = source_link
        self.id = 0

    def __str__(self):
        return str(self.__dict__)

    def __repr__(self):
        return str(self.__dict__)


class Player:
    def __init__(self, dic):
        self.birth = dic['birthDate']
        self.cnName = dic['cnName']
        self.cnAlias = dic['cnAlias']
        self.enName = dic['enName']
        self.height = dic['height']
        self.image = dic['picFromSIB']
        self.position = dic['position']
        self.weight = dic['weight']
        self.jerseyNum = dic['jerseyNum']
        self.rank = 0

    def __str__(self):
        return str(self.__dict__)

    def __repr__(self):
        return str(self.__dict__)


class Team:
    def __init__(self, dic):
        self.image = dic['logoNew']
        self.brief = dic['brief']
        self.chName = dic['cnName']
        self.fullname = dic['fullCnName']
        self.enName = dic['enName']
        self.coach = dic['coach']
        self.city = dic['city']
        self.encity = dic['cityEnName']
        self.venue = dic['venue']
        self.joinData = dic['joinNBADate']
        self.No1 = dic['historyChampion']
        self.player = []
        self.rank = 0
        self.id = 0
        self.team_news = set()

    def __str__(self):
        return str(self.__dict__)

    def __repr__(self):
        return str(self.__dict__)


teams = set()

for i in range(1, 31):
    url = "https://ziliaoku.sports.qq.com/cube/index?callback=&cubeId=1&dimId=1&params=t1%3A" \
        + str(i) + "&from=sportsdatabase"
    response = requests.get(url)
    dic = json.loads(response.text)
    dic = dic['data']['baseInfo']
    team = Team(dic)
    url = "https://ziliaoku.sports.qq.com/cube/index?callback=&cubeId=10&dimId=31&params=t2%3A2018|t3%3A1|t4%3A"\
        + str(i) + "&from=sportsdatabase"
    response = requests.get(url)
    dic = json.loads(response.text)
    player_list = dic['data']['nbaTeamPlayerSeasonStat']
    for j in player_list:
        url = "https://ziliaoku.sports.qq.com/cube/index?callback=&cubeId=8&dimId=5&params=t1%3A"\
            + str(j['playerId']) + "&from=sportsdatabase"
        response = requests.get(url)
        player_dic = json.loads(response.text)
        player_dic = player_dic['data']['playerBaseInfo']
        player = Player(player_dic)
        team.player.append(player)
        team.id = i
        team.rank = 31 - i
    teams.add(team)

with open('teams.pick', 'wb') as f:
    pickle.dump(teams, f)
