from django.shortcuts import render
from Browser.tasks import task

nowstate = 0
mytask = task(0)
mytask.start()


def index(request):
    global nowstate
    print(request.GET)
    if 'state' in request.GET:
        if request.GET['state'][0] == '1' and nowstate == 0:
            mytask.state = 1
            nowstate = 1
        elif request.GET['state'][0] == '0':
            mytask.state = 0
            nowstate = 0
            pass

    if nowstate == 0:
        data = 'Begin Search News'
        nextstate = 1
    else:
        data = 'Searching News'
        nextstate = 0

    return render(request, 'index.html', {'data': data, 'state': nextstate})


# Create your views here.
