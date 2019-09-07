#!/usr/bin/env python
"""Django's command-line utility for administrative tasks."""
import os
import pickle
import sys


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


def main():
    os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'Browser.settings')
    try:
        from django.core.management import execute_from_command_line
    except ImportError as exc:
        raise ImportError("Couldn't import Django. Are you sure it's installed and " "available on your PYTHONPATH environment variable? Did you " "forget to activate a virtual environment?") from exc
    execute_from_command_line(sys.argv)


if __name__ == '__main__':
    main()
