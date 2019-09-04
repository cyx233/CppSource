import requests
import pickle
import json
import crawler_head

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
    teams.add(team)
    print(team)

with open('teams.pick', 'wb') as f:
    pickle.dump(teams, f)
