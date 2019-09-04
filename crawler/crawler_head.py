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

    def __str__(self):
        return str(self.__dict__)

    def __repr__(self):
        return str(self.__dict__)


class Team:
    def __init__(self, dic):
        self.image = dic['logoNew']
        self.chName = dic['cnName']
        self.fullname = dic['fullCnName']
        self.enName = dic['enName']
        self.coach = dic['coach']
        self.city = dic['city']
        self.encity = dic['cityEnName']
        self.venue = dic['venue']
        self.player = []

    def __str__(self):
        return str(self.__dict__)

    def __repr__(self):
        return str(self.__dict__)
