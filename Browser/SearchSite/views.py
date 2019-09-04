from django.shortcuts import render


def hello(request):
    return render(request, 'searchsite.html')


# Create your views here.
